#include "../character_ai_skill_script_constructor.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace k = dh2::character_ai_skill_script_constructor;

struct Call { k::Operation op; std::uintptr_t args; const char* text; std::uint32_t value; };
struct Fixture {
    k::State state{0x1000, k::DispatchTable::none, 0, nullptr, 0, 0};
    k::Globals globals{0};
    k::Result result{};
    std::vector<Call> calls;
    std::vector<k::Assertion> assertions;
    k::Operation fail = static_cast<k::Operation>(99);
    bool mutate_during_arguments = false;
    k::Services services{this, &Fixture::invoke, &Fixture::report};

    static std::int32_t invoke(void* context, k::State* state, const k::Request* request) {
        auto& f = *static_cast<Fixture*>(context);
        assert(request != nullptr);
        f.calls.push_back({request->operation, request->arguments_identity,
                           request->text, request->integer});
        if (request->operation == k::Operation::arguments_construct && f.mutate_during_arguments) {
            state->character = 0x7777;
            state->skill_index_14 = 0;
        }
        return request->operation == f.fail ? 1 : 0;
    }
    static std::int32_t report(void* context, k::State*, const k::Request* request) {
        auto& f = *static_cast<Fixture*>(context);
        assert(request && request->operation == k::Operation::report_assertion);
        assert(request->source_line == 0x2e);
        f.assertions.push_back(request->assertion);
        return 0;
    }
    k::Status run(std::uintptr_t owner, const char* name, std::uint32_t index = 6) {
        return k::construct(&state, owner, name, index, &globals, &services, &result);
    }
};

const char* operation_name(k::Operation op) {
    switch (op) {
        case k::Operation::arguments_construct: return "construct";
        case k::Operation::arguments_push_string: return "push_string";
        case k::Operation::arguments_push_integer: return "push_integer";
        case k::Operation::report_assertion: return "assertion";
    }
    return "unknown";
}

int main(int argc, char** argv) {
    if (argc == 2) {
        Fixture f;
        std::uintptr_t owner = 0x2000;
        const char* name = "Faery_Bolt";
        if (argv[1] == std::string("null_owner")) owner = 0;
        else if (argv[1] == std::string("null_name")) name = nullptr;
        else if (argv[1] == std::string("both_null")) { owner = 0; name = nullptr; }
        else if (argv[1] != std::string("valid")) return 2;
        const auto status = f.run(owner, name, 6);
        std::cout << "{\"status\":" << static_cast<std::int32_t>(status)
                  << ",\"dispatch\":" << static_cast<std::uint32_t>(f.state.dispatch_table)
                  << ",\"owner\":" << f.state.character
                  << ",\"index\":" << f.state.skill_index_14
                  << ",\"last_id\":" << f.state.last_skill_id_18
                  << ",\"returned\":" << f.result.returned_identity
                  << ",\"calls\":[";
        for (std::size_t i = 0; i < f.calls.size(); ++i) {
            if (i) std::cout << ',';
            const auto& c = f.calls[i];
            const auto offset = c.args - f.state.identity;
            std::cout << "{\"op\":\"" << operation_name(c.op) << "\",\"args_offset\":" << offset
                      << ",\"value\":" << c.value << ",\"text\":";
            if (c.text) std::cout << '"' << c.text << '"'; else std::cout << "null";
            std::cout << '}';
        }
        std::cout << "],\"assertions\":" << f.assertions.size() << "}\n";
        return status == k::Status::complete ? 0 : 1;
    }

    std::uint32_t cases = 0;
    {
        Fixture f;
        assert(f.run(0x2000, "Ghost_Spark") == k::Status::complete);
        assert(f.state.dispatch_table == k::DispatchTable::char_ai_skill_script);
        assert(f.state.character == 0x2000 && std::string(f.state.script_name) == "Ghost_Spark");
        assert(f.state.skill_index_14 == 6 && f.state.last_skill_id_18 == -1);
        assert(f.result.returned_identity == f.state.identity && f.calls.size() == 3);
        assert(f.calls[0].op == k::Operation::arguments_construct && f.calls[0].args == 0x100c);
        assert(f.calls[1].op == k::Operation::arguments_push_string &&
               std::string(f.calls[1].text) == "Ghost_Spark");
        assert(f.calls[2].op == k::Operation::arguments_push_integer && f.calls[2].value == 6);
        ++cases;
    }
    {
        Fixture f;
        assert(f.run(0, "Ghost_Spark") == k::Status::complete);
        assert(f.state.character == 0 && f.assertions.empty() && f.calls.size() == 3);
        ++cases;
    }
    {
        Fixture f;
        assert(f.run(0x2000, nullptr) == k::Status::complete);
        assert(f.assertions.empty() && f.calls.size() == 3 && f.calls[1].text == nullptr);
        ++cases;
    }
    {
        Fixture f;
        f.globals.assert_level = 1;
        assert(f.run(0, nullptr) == k::Status::complete);
        assert(f.assertions.size() == 2);
        assert(f.assertions[0] == k::Assertion::character_nonnull);
        assert(f.assertions[1] == k::Assertion::name_nonnull);
        assert(f.calls.size() == 3);
        ++cases;
    }
    {
        Fixture f;
        f.globals.assert_level = 2;
        assert(f.run(0, "Ghost_Spark") == k::Status::fatal_source_assertion);
        assert(f.calls.size() == 1 && f.state.skill_index_14 == 6 &&
               f.state.last_skill_id_18 == -1 && f.result.returned_identity == 0);
        ++cases;
    }
    {
        Fixture f;
        f.fail = k::Operation::arguments_construct;
        assert(f.run(0x2000, "Ghost_Spark") == k::Status::service_failed);
        assert(f.calls.size() == 1 && f.state.dispatch_table == k::DispatchTable::char_ai_skill_script);
        assert(f.state.skill_index_14 == 0);
        ++cases;
    }
    {
        Fixture f;
        f.fail = k::Operation::arguments_push_string;
        assert(f.run(0x2000, "Ghost_Spark") == k::Status::service_failed);
        assert(f.calls.size() == 2 && f.state.skill_index_14 == 6 &&
               f.state.last_skill_id_18 == -1 && f.result.returned_identity == 0);
        ++cases;
    }
    {
        Fixture f;
        f.mutate_during_arguments = true;
        assert(f.run(0x2000, "Ghost_Spark", 9) == k::Status::complete);
        assert(f.state.character == 0x7777 && f.state.skill_index_14 == 9);
        assert(f.calls[1].args == 0x100c && f.calls[1].text == std::string("Ghost_Spark"));
        assert(f.calls[2].value == 9);
        ++cases;
    }
    {
        Fixture f;
        assert(k::construct(&f.state, 0x2000, "x", 1, &f.globals, &f.services,
                            reinterpret_cast<k::Result*>(&f.state)) == k::Status::invalid_argument);
        ++cases;
    }
    std::cout << "{\"validation\":\"PASS\",\"host_cases\":" << cases
              << ",\"source\":\"CharAISkillScript constructor\"}\n";
}
