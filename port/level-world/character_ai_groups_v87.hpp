#pragma once
#include "character_save_restore_v3.hpp"
#include <memory>
#include <vector>
#include <functional>
namespace dh2::world {struct CanonicalCharacterCandidateRecordV60;}
namespace dh2::character {
// Native storage of original process CharAI.s_groups, keyed by CString/room.
// Members borrow the real canonical records; no FSM, Character or pose copy.
class CharacterAiGroupV87 final {
 friend bool source_character_add_to_group_v87(const std::shared_ptr<world::CanonicalCharacterCandidateRecordV60>&,std::string&);
 friend void source_character_clear_group_info_v87()noexcept;
 std::vector<std::weak_ptr<world::CanonicalCharacterCandidateRecordV60>> master0_,leader_c_,ordinary18_;
 std::int32_t field24_{-1};std::uint8_t byte28_{},byte29_{};
 bool source_present_{true}; //native lifetime marker, not a game/source flag
public:
 CharacterSaveGroupBorrowV3 saved_fields()noexcept{return {&field24_,&byte28_,&byte29_};}
 bool source_present()const noexcept{return source_present_;}
 bool can_respawn(world::CanonicalCharacterCandidateRecordV60&,bool&,std::string&);
 bool source_limbus_blur_v118(world::CanonicalCharacterCandidateRecordV60&,std::string&);
 bool remove(world::CanonicalCharacterCandidateRecordV60&,std::string&);
 using Kill=std::function<bool(world::CanonicalCharacterCandidateRecordV60&,std::uintptr_t,std::string&)>;
 bool on_died(world::CanonicalCharacterCandidateRecordV60&,std::uintptr_t,const Kill&,std::string&);
 bool on_enemy_spotted(world::CanonicalCharacterCandidateRecordV60&,std::string&);
 bool can_spawn()const noexcept{return false;} //whole3d24fc literal return0
};
bool source_character_add_to_group_v87(const std::shared_ptr<world::CanonicalCharacterCandidateRecordV60>&,std::string&);
bool source_character_remove_from_group_v87(world::CanonicalCharacterCandidateRecordV60&,std::string&);
bool source_character_group_on_died_v87(world::CanonicalCharacterCandidateRecordV60&,
 std::uintptr_t actual_group,std::uintptr_t actual_attacker,const CharacterAiGroupV87::Kill&,std::string&);
bool source_character_group_enemy_spotted_v87(world::CanonicalCharacterCandidateRecordV60&,
 std::uintptr_t actual_group,std::string&);
bool source_character_can_respawn_v87(world::CanonicalCharacterCandidateRecordV60&,bool&,std::string&);
//Whole CSM_Spawn3ad2e4. Original next-state reference is untouched.
bool source_character_spawn_predicate_v87(world::CanonicalCharacterCandidateRecordV60&,
 std::int32_t source_state,std::uint32_t& word,std::string&);
// Genuine ClearGroupInfo3d2fb0 destroys process map. Call at its source unload
// site, after member callbacks; this never manufactures a per-map group world.
void source_character_clear_group_info_v87()noexcept;
}
