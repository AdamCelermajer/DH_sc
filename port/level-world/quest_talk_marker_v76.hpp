#pragma once
#include "../game-data/quest_persistence_v51.hpp"
#include "character_mesh_fx_owner_v4.hpp"
namespace dh2::world {
struct QuestMarkerCharacterBorrowV76 {
 std::shared_ptr<void> receiver;std::uintptr_t identity{};
 std::function<void(std::uint8_t required2fa,std::uint8_t primary2fb)> store_flags;
};
struct QuestTalkMarkerServicesV76 {
 std::shared_ptr<void> provider;
 std::function<bool(std::int32_t,QuestMarkerCharacterBorrowV76&,std::string&)> first_character;
 std::function<bool(std::int32_t byte_offset,std::int32_t&,std::string&)> settings_effect;
 std::function<bool(std::shared_ptr<void>&,fx::CharacterMeshFxOwnerV4*&,std::string&)> actual_fx;
 //Actual AnimatedFX2c -> source visual8 -> field204 conditional store;
 //GetAnimator49267c -> virtual44 -> virtual40(bool). No plain Model clone.
 std::function<bool(fx::CharacterMeshFxOwnerV4&,std::uintptr_t,std::uintptr_t,std::string&)> visual_owner204;
 std::function<bool(fx::CharacterMeshFxOwnerV4&,std::uintptr_t,bool,std::string&)> animator_loop;
};
bool quest_remove_talk_marker_v76(data::QuestObjectivePersistenceV51&,const QuestTalkMarkerServicesV76&,std::string&);
bool quest_install_talk_marker_v76(data::QuestObjectivePersistenceV51&,std::int32_t priority,
 std::int32_t state,const QuestTalkMarkerServicesV76&,std::string&);
}
