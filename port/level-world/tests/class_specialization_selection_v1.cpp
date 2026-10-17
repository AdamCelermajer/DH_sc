#include "../class_specialization_selection_v1.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <string>

namespace {
using namespace dh2::class_specialization_selection_v1;
enum Op : std::int32_t { properties = 1, set_class = 2, reload = 3, save = 4 };
struct State {
    std::array<std::int32_t, 8> calls{};
    std::uint32_t count = 0;
    std::int32_t target = -1;
};
bool load(void* raw, std::int32_t target, std::string&) {
    auto& s = *static_cast<State*>(raw); s.target = target;
    s.calls[s.count++] = properties; return true;
}
bool set(void* raw, std::int32_t target, std::string&) {
    auto& s = *static_cast<State*>(raw);
    if (s.target != target) return false;
    s.calls[s.count++] = set_class; return true;
}
bool reload_skills(void* raw, std::string&) {
    static_cast<State*>(raw)->calls[static_cast<State*>(raw)->count++] = reload;
    return true;
}
bool save_game(void* raw, std::string&) {
    static_cast<State*>(raw)->calls[static_cast<State*>(raw)->count++] = save;
    return true;
}
bool check(bool ok, const char* message) {
    if (!ok) std::cerr << "FAIL: " << message << '\n';
    return ok;
}
}

int main() {
    unsigned checks = 0;
    bool ok = true;
    const std::array<std::int32_t, 4> expected{{properties, set_class, reload, save}};
    for (std::int32_t selector = 0; selector <= 1; ++selector) {
        State state;
        Services services{&state, load, set, reload_skills, save_game};
        Result result{}; std::string error;
        const auto status = select(10, selector, 16, services, &result, error);
        ok &= check(status == Status::complete && error.empty(), "valid selector completes"); ++checks;
        ok &= check(result.selected_class == 11 + selector && result.calls == 4 &&
                    result.stage == Stage::complete, "selector resolves to source adjacent class"); ++checks;
        ok &= check(state.target == 11 + selector && state.count == expected.size() &&
                    std::equal(expected.begin(), expected.end(), state.calls.begin()),
                    "same-owner operations run in source order"); ++checks;
    }
    for (const auto [current, selector, count] : {
             std::array<std::int32_t, 3>{10, -1, 16},
             std::array<std::int32_t, 3>{10, 2, 16},
             std::array<std::int32_t, 3>{14, 1, 16}}) {
        State state;
        Services services{&state, load, set, reload_skills, save_game};
        Result result{}; std::string error;
        const auto status = select(current, selector, std::size_t(count), services,
                                   &result, error);
        ok &= check(status == Status::invalid_argument && !error.empty(),
                    "invalid selector/target is rejected"); ++checks;
        ok &= check(state.count == 0 && result.calls == 0,
                    "invalid input mutates no owner"); ++checks;
    }
    std::cout << "{\"validation\":\"" << (ok ? "PASS" : "FAIL")
              << "\",\"checks\":" << checks << "}\n";
    return ok ? 0 : 1;
}
