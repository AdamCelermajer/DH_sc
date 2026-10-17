#include "../ais_state_callbacks.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
using namespace dh2::ais_state_callbacks;
namespace {
constexpr std::uintptr_t ais_id = 0x10001000, table_id = 0x10002000;
const char* names[] = {"TickState", "CheckState", "InitState", "PostState"};
struct Fixture {
    Table table{table_id, names[0], names[1], names[2], names[3]};
    Table next{table_id + 0x1000, "NextUpdate", "NextCheck", "NextInit", "NextPost"};
    State state{ais_id, &table};
    unsigned calls = 0, mutation = 0;
    Request captured{};
    static std::int32_t call(void* context, State* state, const Request* request) {
        auto& f = *static_cast<Fixture*>(context);
        ++f.calls; f.captured = *request;
        if (f.mutation == 1) state->active = nullptr;
        if (f.mutation == 2) state->active = &f.next;
        if (f.mutation == 3) f.table.conditions = "ChangedCheck";
        if (f.mutation == 4) { state->active = nullptr; return 1; }
        return 0;
    }
    Services services{this, call};
};
void require(bool pass, const char* message) {
    if (!pass) { std::cerr << message << '\n'; std::exit(1); }
}
const char** selected(Table& t, unsigned index) {
    switch (index) {
        case 0: return &t.update; case 1: return &t.conditions;
        case 2: return &t.init; default: return &t.post;
    }
}
void emit(unsigned index, unsigned mode, unsigned mutation) {
    Fixture f; f.mutation = mutation;
    if (mode == 0) f.state.active = nullptr;
    if (mode == 2) *selected(f.table, index) = nullptr;
    if (mode == 3) *selected(f.table, index) = "";
    Result result{};
    auto status = invoke(&f.state, static_cast<Callback>(index), &f.services, &result);
    std::cout << "{\"status\":" << static_cast<int>(status)
              << ",\"calls\":" << f.calls << ",\"ais\":" << (f.calls ? f.captured.ais : 0)
              << ",\"table\":" << result.table << ",\"offset\":" << result.source_name_offset
              << ",\"name\":";
    if (!f.calls || !f.captured.name) std::cout << "null";
    else std::cout << '"' << f.captured.name << '"';
    std::cout << ",\"active\":" << (f.state.active ? f.state.active->identity : 0) << "}\n";
}
}
int main(int argc, char** argv) {
    if (argc == 4) {
        emit(static_cast<unsigned>(std::strtoul(argv[1], nullptr, 0)),
             static_cast<unsigned>(std::strtoul(argv[2], nullptr, 0)),
             static_cast<unsigned>(std::strtoul(argv[3], nullptr, 0)));
        return 0;
    }
    unsigned cases = 0;
    for (unsigned index = 0; index != 4; ++index) {
        Fixture f; Result r{};
        require(invoke(&f.state, static_cast<Callback>(index), &f.services, &r) == Status::complete &&
                f.calls == 1 && f.captured.name == names[index], "wrong callback field"); ++cases;
        f.state.active = nullptr; f.calls = 0;
        require(invoke(&f.state, static_cast<Callback>(index), nullptr, &r) == Status::complete &&
                !r.called && !r.table && !r.name && !f.calls, "null table did work"); ++cases;
    }
    for (unsigned mutation = 1; mutation <= 3; ++mutation) {
        Fixture f; f.mutation = mutation; Result r{};
        require(invoke(&f.state, Callback::update, &f.services, &r) == Status::complete,
                "update failed"); ++cases;
        f.mutation = 0;
        require(invoke(&f.state, Callback::conditions, &f.services, &r) == Status::complete,
                "conditions failed");
        require(mutation == 1 ? f.calls == 1 && !r.called :
                std::string(f.captured.name) == (mutation == 2 ? "NextCheck" : "ChangedCheck"),
                "conditions did not read live table"); ++cases;
    }
    Fixture f; Result r{99, 99, 99, nullptr};
    require(invoke(nullptr, Callback::update, &f.services, &r) == Status::invalid_argument && r.called == 99,
            "invalid state changed output"); ++cases;
    require(invoke(&f.state, static_cast<Callback>(4), &f.services, &r) == Status::invalid_argument && r.called == 99,
            "invalid callback changed output"); ++cases;
    require(invoke(&f.state, Callback::update, nullptr, &r) == Status::service_unavailable && !r.called,
            "missing Call was accepted"); ++cases;
    f.mutation = 4;
    require(invoke(&f.state, Callback::update, &f.services, &r) == Status::service_failed &&
            r.called && !f.state.active, "failed Call rolled back its effects"); ++cases;
    std::cout << "{\"validation\":\"PASS\",\"host_cases\":" << cases << "}\n";
}
