#pragma once
#include "character_menu_stats_owner_v1.hpp"
#include "character_menu_skill_authority_v1.hpp"
#include "../level-world/player_equipment_render_owner_v1.hpp"
#include "../level-world/character_player_skills_v3.hpp"
#include "../level-world/player_initial_grants_v2.hpp"
#include "../game-data/player_savegame_v1.hpp"
namespace dh2::ui {
struct CharacterMenuActionsGraphV1 {
    // Retains every borrowed provider/graph projection below through callbacks.
    std::shared_ptr<void> owner;
    player::PlayerEquipmentRenderOwnerV1* equipment{};
    CharacterMenuSkillAuthorityV1 skills;
    data::PlayerSavegameV1* save{};
    // V3 presently exposes its property identity, but not its external saved
    // skill getter binding. Caller must prove THIS save is that native binding.
    std::function<bool(data::PlayerSavegameV1&,const CharacterMenuSkillAuthorityV1&,std::string&)> save_binding;
    data::SkillTables::Borrow skill_tables;
    // Actual Character+1068 GetSkillsID field, read afresh before each lookup.
    const std::int32_t* skill_list_index{};
    CharacterMenuStatGraphV1 stats;
    // Genuine full source IncSkill services. No cap/default/difficulty policy
    // is supplied by this UI adapter. Each request carries the same identity.
    player::InitialGrantServices16V2 increment{};
    // Exact Character3bcee4/V4 live byte field store after source recalculation.
    // Required on successful training, even when current capacity is unchanged.
    std::function<bool(std::uint8_t,std::string&)> potion_capacity_store;
    // NativeSwapEquipment reaches these even when its selected player is null.
    // Caller supplies exact retained HUD AS receiver and original numeric arg.
    std::function<bool(const char*,std::string&)> swap_hud;
};
// Whole action coordinator for the in-game menu's gear, skill-slot, training
// and stat assignment sections. This is separate from query field production
// and AS argument conversion. Missing live V3/save services reject explicitly.
class CharacterMenuActionsOwnerV1 {
    CharacterMenuActionsGraphV1 graph_;
    bool equipment(std::string&)const;
    bool skills(std::string&)const;
    const data::SkillRecord* skill(std::int32_t,std::string&)const;
    static bool update_skills(void*,std::uintptr_t,std::string&);
    static int increment_service(void*,const player::InitialGrantRequest32V2*,player::InitialGrantResponse8V2*);
public:
    explicit CharacterMenuActionsOwnerV1(CharacterMenuActionsGraphV1);
    const CharacterMenuActionsGraphV1& bindings()const noexcept{return graph_;}
    bool validate_graph(bool require_skills,std::string& e)const{return require_skills?skills(e):equipment(e);}
    const data::SkillRecord* skill_record(std::int32_t row,std::string& e)const{return skill(row,e);}
    bool can_increment(std::uint32_t row,bool&,std::string&)const;
    bool equip(std::uint32_t slot,std::uint32_t item,std::string&);
    bool unequip(std::uint32_t slot,std::string&);
    bool swap(std::string&);
    bool assign_stat(std::uint32_t stat,std::string&);
    bool equip_skill(std::int32_t slot,std::int32_t row,std::string&);
    bool train_skill(std::int32_t row,std::int32_t& points_left,std::string&);
    // The authored callback APPENDS three saved row numbers to its supplied
    // AS array, then reports bool true. It never clears or replaces the array.
    bool append_equipped_skills(const std::function<bool(std::int32_t,std::string&)>&,std::string&);
    bool skill_points(std::int32_t&,std::string&)const;
};
}
