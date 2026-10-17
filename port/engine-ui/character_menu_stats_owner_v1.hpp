#pragma once
#include "../game-data/properties.hpp"
#include <functional>

namespace dh2::ui {
// Adapted from Adam Celermajer's `character_menu_stats_owner_v1` at checkpoint
// 11fa5242. Borrows the same retained actor table, class rows and PropertyState
// as the live equipment/skill/save authorities; it owns no duplicate state.
struct CharacterMenuStatGraphV1 {
    data::PropertyState* state{};
    data::PropertyView* view{};
    const data::CharacterTable* actors{};
    const data::ClassRow* classes{};
    std::uint32_t class_count{};
    std::int32_t actor_index{};
    std::function<bool(std::string&)> debug_load;
    std::function<bool(const char*,bool&,std::string&)> debug_query;
};

// Source IncStatStr/Dex/End/Nrg and RecalcProperties(actorIndex) order.
// Reached stores remain applied if a later required source service fails.
bool character_menu_assign_stat_v1(CharacterMenuStatGraphV1&,std::uint32_t stat,
                                   std::string& error);
}
