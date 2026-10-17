#include "../ais_external_update.hpp"

#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace k = dh2::ais_external_update;
constexpr std::uintptr_t AIS = 0x10001000;
constexpr std::uintptr_t OWNER_A = 0x10002000;
constexpr std::uintptr_t OWNER_B = 0x10003000;
constexpr std::uintptr_t CONTROLLER_A = 0x10004000;
constexpr std::uintptr_t CONTROLLER_B = 0x10005000;

struct Call {
    std::uint32_t operation;
    std::uintptr_t subject;
    std::uint32_t argument;
    std::string name;
};

struct Fixture {
    k::State state{AIS, OWNER_A, 0, 0};
    k::Services services{this, invoke};
    k::Result result{};
    bool state_table = false;
    unsigned mutation = 0;
    unsigned fail_at = 0;
    unsigned callback_count = 0;
    std::vector<Call> calls;

    static std::int32_t invoke(void* opaque, k::State* state, const k::Request* request) {
        auto& self = *static_cast<Fixture*>(opaque);
        if (state != &self.state || !request || request->ais != AIS || request->reserved)
            return 1;
        ++self.callback_count;
        if (self.fail_at == self.callback_count) return 1;
        switch (request->operation) {
        case k::Operation::pause_character_ai:
            if (request->argument != 1000 || request->owner != state->owner) return 1;
            self.calls.push_back({0, request->owner + 0x3c8, request->argument, {}});
            if (self.mutation == 1) state->owner = OWNER_B;
            if (self.mutation == 2) state->flags_b8 |= 1u;
            if (self.mutation == 3) state->flags_b8 &= ~1u;
            break;
        case k::Operation::stop_character_controller:
            if (request->argument || request->owner != state->owner) return 1;
            self.calls.push_back({1, request->owner == OWNER_A ? CONTROLLER_A : CONTROLLER_B, 0, {}});
            break;
        case k::Operation::call_ais_on_update:
            if (request->callback != k::ScriptCallback::on_update) return 1;
            self.calls.push_back({2, request->ais, 0, "OnUpdate"});
            if (self.mutation == 4) self.state_table = true;
            if (self.mutation == 6) self.state.counter_bc = 0x12345678u;
            break;
        case k::Operation::call_state_update:
            if (request->callback != k::ScriptCallback::state_update) return 1;
            if (self.state_table) self.calls.push_back({2, request->ais, 0, "TickState"});
            if (self.mutation == 5) self.state_table = false;
            break;
        case k::Operation::call_state_conditions:
            if (request->callback != k::ScriptCallback::state_conditions) return 1;
            if (self.state_table) self.calls.push_back({2, request->ais, 0, "CheckState"});
            break;
        }
        return 0;
    }

    k::Status run() { return k::update(&state, &services, &result); }
};

void dump(const Fixture& f, k::Status status) {
    std::cout << "{\"status\":" << static_cast<int>(status)
              << ",\"counter\":" << f.state.counter_bc
              << ",\"owner\":" << f.state.owner
              << ",\"flags\":" << f.state.flags_b8
              << ",\"table\":" << (f.state_table ? 1 : 0)
              << ",\"pause_due\":" << f.result.default_pause_due
              << ",\"on_update\":" << f.result.script_on_update_called
              << ",\"service_calls\":" << f.result.service_calls
              << ",\"calls\":[";
    for (std::size_t i = 0; i < f.calls.size(); ++i) {
        if (i) std::cout << ',';
        const auto& call = f.calls[i];
        std::cout << '[' << call.operation << ',' << call.subject << ',' << call.argument
                  << ",\"" << call.name << "\"]";
    }
    std::cout << "]}\n";
}

void require(bool pass, const char* message) {
    if (!pass) throw std::runtime_error(message);
}

void host_cases() {
    unsigned cases = 0;
    {
        Fixture f;
        require(f.run() == k::Status::complete && f.state.counter_bc == 0 &&
                f.result.phase == k::Phase::complete && f.result.service_calls == 2 &&
                f.calls.empty(), "empty state callbacks did not remain a no-op");
        ++cases;
    }
    {
        Fixture f; f.state.counter_bc = 199; f.state.flags_b8 = 2; f.state_table = true;
        require(f.run() == k::Status::complete && f.result.service_calls == 2 &&
                f.calls.size() == 2 && f.calls[0].name == "TickState" &&
                f.calls[1].name == "CheckState", "unsigned threshold/flag bit changed");
        ++cases;
    }
    {
        Fixture f; f.state.counter_bc = 200; f.state.flags_b8 = 1; f.state_table = true;
        require(f.run() == k::Status::complete && f.state.counter_bc == 0 &&
                f.calls.size() == 5 && f.calls[0].operation == 0 &&
                f.calls[1].operation == 1 && f.calls[2].name == "OnUpdate" &&
                f.calls[3].name == "TickState" && f.calls[4].name == "CheckState",
                "default/update/state callback source order changed");
        ++cases;
    }
    {
        Fixture f; f.state.counter_bc = 0xffffffffu; f.mutation = 1;
        require(f.run() == k::Status::complete && f.state.owner == OWNER_B &&
                f.calls.size() == 2 && f.calls[0].subject == OWNER_A + 0x3c8 &&
                f.calls[1].subject == CONTROLLER_B,
                "Cmd_Stop did not reload owner after AI_PauseUpdate");
        ++cases;
    }
    {
        Fixture f; f.state.counter_bc = 200; f.mutation = 2;
        require(f.run() == k::Status::complete && f.calls.size() == 3 &&
                f.calls[2].name == "OnUpdate", "live flags were not read after default update");
        ++cases;
    }
    {
        Fixture f; f.state.counter_bc = 200; f.state.flags_b8 = 1; f.mutation = 3;
        require(f.run() == k::Status::complete && f.calls.size() == 2,
                "live cleared OnUpdate override flag was ignored");
        ++cases;
    }
    {
        Fixture f; f.state.flags_b8 = 1; f.mutation = 4;
        require(f.run() == k::Status::complete && f.calls.size() == 3 &&
                f.calls[1].name == "TickState" && f.calls[2].name == "CheckState",
                "state update did not read the live state table");
        ++cases;
    }
    {
        Fixture f; f.state_table = true; f.mutation = 5;
        require(f.run() == k::Status::complete && f.calls.size() == 1 &&
                f.calls[0].name == "TickState", "conditions reused a stale state table");
        ++cases;
    }
    for (unsigned fail = 1; fail <= 5; ++fail) {
        Fixture f; f.state.counter_bc = 200; f.state.flags_b8 = 1; f.state_table = true;
        f.fail_at = fail;
        const auto status = f.run();
        require(status == k::Status::service_failed && f.callback_count == fail &&
                f.state.counter_bc == 0 && f.result.service_calls == fail,
                "service failure did not preserve the completed source prefix");
        ++cases;
    }
    {
        Fixture f;
        auto* alias = reinterpret_cast<k::Result*>(&f.state);
        require(k::update(&f.state, &f.services, alias) == k::Status::invalid_argument &&
                f.callback_count == 0, "state/result alias was not rejected atomically");
        ++cases;
    }
    {
        Fixture f; f.state.counter_bc = 200; f.state.owner = 0;
        require(f.run() == k::Status::invalid_source_fact && f.state.counter_bc == 0 &&
                f.callback_count == 0, "null source owner crossed an unsafe pause call");
        ++cases;
    }
    std::cout << "{\"validation\":\"PASS\",\"ais_external_update_host_cases\":"
              << cases << ",\"mismatches\":0}\n";
}

int main(int argc, char** argv) {
    try {
        if (argc == 1) { host_cases(); return 0; }
        if (argc != 6) throw std::runtime_error("expected counter flags table mutation fail_at");
        Fixture f;
        f.state.counter_bc = static_cast<std::uint32_t>(std::strtoul(argv[1], nullptr, 0));
        f.state.flags_b8 = static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0));
        f.state_table = std::strtoul(argv[3], nullptr, 0) != 0;
        f.mutation = static_cast<unsigned>(std::strtoul(argv[4], nullptr, 0));
        f.fail_at = static_cast<unsigned>(std::strtoul(argv[5], nullptr, 0));
        dump(f, f.run());
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
