#pragma once
#include "../game-data/properties.hpp"
#include <functional>
namespace dh2::ui {
// Borrow the same retained actor table, class rows and PropertyState as the
// equipment/skill/save authority. No copied menu properties are accepted.
struct CharacterMenuStatGraphV1 {
    data::PropertyState* state{};
    data::PropertyView* view{};
    const data::CharacterTable* actors{};
    const data::ClassRow* classes{};
    std::uint32_t class_count{};
    std::int32_t actor_index{};
    // Actual Debug LoadScript/GetSwitch are reached on both source branches.
    std::function<bool(std::string&)> debug_load;
    std::function<bool(const char*,bool&,std::string&)> debug_query;
};
// Exact IncStatStr/Dex/End/Nrg and RecalcProperties(actorIndex) sequence.
// stat 0/1/2/3 are Strength/Dexterity/Endurance/Energy. Source prefixes remain
// after a required provider failure, as on inventory and skill actions.
bool character_menu_assign_stat_v1(CharacterMenuStatGraphV1&,std::uint32_t stat,std::string&);
}
