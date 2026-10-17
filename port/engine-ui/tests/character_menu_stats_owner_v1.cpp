#include "../character_menu_stats_owner_v1.hpp"
#include <iostream>
#include <stdexcept>

using namespace dh2;
using namespace dh2::ui;
static void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

int main() {
    std::string error;
    data::CharacterTable actors;
    actors.rows.resize(1);
    data::PropertyRules rules;
    rules.defaults.fill(0);
    rules.types.fill(0);
    rules.types[148] = 32;
    rules.types[151] = 32;
    data::PropertyState state;
    data::reset_properties(rules, state, &actors.rows[0]);
    auto view = data::property_view(rules, state);
    std::vector<data::ClassFormula> formula{{-1, 8, 0, 0, 0}};
    const data::ClassRow classes[]{{formula.data(), std::uint32_t(formula.size())}};
    check(!dh2_class_recalc_base(classes, 1, state.base.data(), &view),
          "initial source class recalculation failed");
    check(!dh2_property_set_int(&view, 148, 1), "initial stat points write failed");
    CharacterMenuStatGraphV1 graph{&state, &view, &actors, classes, 1, 0, {}, {}};
    unsigned debug_calls{};
    graph.debug_load = [&](std::string&) { ++debug_calls; return true; };
    graph.debug_query = [&](const char* name, bool& tracing, std::string&) {
        check(std::string(name) == "isTracingChar_Stats", "source Debug key changed");
        tracing = false;
        ++debug_calls;
        return true;
    };
    check(character_menu_assign_stat_v1(graph, 2, error), error);
    check(state.saved[148] == 0 && state.saved[151] == 256,
          "source stat point debit/credit differs");
    check(debug_calls == 2, "source Debug load/query tail was not delivered");
    auto detached = view;
    detached.saved = state.base.data();
    graph.view = &detached;
    check(!character_menu_assign_stat_v1(graph, 0, error) && !error.empty(),
          "detached PropertyState was accepted");
    std::cout << "character-menu stat action owner passed 4 focused assertions\n";
}
