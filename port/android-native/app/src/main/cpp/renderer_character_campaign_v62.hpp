#pragma once
#include "source_campaign_animation_step_v100.hpp"
#include "source_campaign_character_fsm_v101.hpp"
#include "source_campaign_character_frame_v111.hpp"
#include <memory>
#include <cstdint>
#include <string>
#include <android/asset_manager.h>
#include "source_campaign_character_cache_v81.hpp"
#include "source_campaign_script_actor_v96.hpp"
#include "../../../../../level-world/gameplay_camera_target_v2.hpp"
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
struct PlayerGameplayBinding;
}
namespace dh2::world {struct CanonicalCharacterCandidateRecordV60;}
namespace dh2::world {struct CanonicalFactoryEntryV1;struct CanonicalClassReceiverV1;}
namespace dh2::data {struct AnimationTables;class WorldMapProfileTableV59;}
namespace dh2::data {class PlayerSaveLoadOwnerV1;}
namespace dh2::character {struct CharacterMenuMutationServicesV4;struct CharacterMenuFaeryServicesV8;}
namespace dh2::character {class CharacterCandidateCacheV62;}
namespace dh2::character::skills {class CharacterWorldRuntimeV1;}
namespace model_renderer {
bool source_campaign_all_players_dead_v70(const std::shared_ptr<void>& actual_world,bool& value,std::string& error);
bool source_campaign_player_manager_integer714_v70(const std::shared_ptr<void>& actual_world,std::int32_t& value,std::string& error);
bool source_campaign_character_init_all_v70(const std::shared_ptr<void>& actual_world,std::uintptr_t identity,std::string& error);
bool source_campaign_manage_characters_v70(const std::shared_ptr<void>& actual_world,std::string& error);
struct SourceCampaignCharacterBorrowV62 {
 std::shared_ptr<void> actual_world;
 std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> character;
};
struct SourceCampaignCameraActorBorrowV67 {
 std::shared_ptr<void> receiver;
 std::uintptr_t identity{};
 const float* position160{};
 std::uintptr_t* anchor2e0{};
 const std::uint8_t* disabled81{};
 const std::uint32_t* dead{}; // SAME native CombatActorState storage width.
};
bool borrow_source_campaign_camera_actor_v67(const std::shared_ptr<void>& actual_world,std::uintptr_t character,SourceCampaignCameraActorBorrowV67&,std::string&);
bool source_campaign_character_look_at_v68(const std::shared_ptr<void>& actual_world,std::uintptr_t character,dh2::camera::PointV2&,std::string&);
bool source_campaign_character_visual_up_v68(const std::shared_ptr<void>& actual_world,std::uintptr_t character,dh2::camera::PointV2&,std::string&);
bool source_campaign_character_set_initial_position160_v68(const std::shared_ptr<void>& actual_world,std::uintptr_t character,std::string&);
bool source_campaign_character_save_use_spawn_point_v84(const std::shared_ptr<void>& actual_world,std::uintptr_t character,bool,std::string&);
bool source_campaign_character_save_level_id_v84(const std::shared_ptr<void>& actual_world,std::uintptr_t character,std::int32_t,std::int32_t,std::string&);
bool source_campaign_character_save_entrypoint_v68(const std::shared_ptr<void>& actual_world,std::uintptr_t character,std::int32_t value,std::int32_t difficulty,std::string&);
bool bind_campaign_character_providers_v62(AAssetManager*,const SourceCampaignCandidateBorrowV55&,std::string&);
bool borrow_source_campaign_character_v62(const std::shared_ptr<void>& actual_world,std::uintptr_t character,SourceCampaignCharacterBorrowV62&,std::string&);
bool retire_source_campaign_closed_character_v111(const std::shared_ptr<void>& actual_world,std::uintptr_t character,std::string&);
bool borrow_source_campaign_character_targets_v88(const std::shared_ptr<void>& actual_world,std::shared_ptr<dh2::character::skills::CharacterWorldRuntimeV1>&,std::string&);
bool borrow_menu_character_ooi_type_v62(const std::shared_ptr<void>& actual_world,std::uintptr_t character,std::shared_ptr<void>& receiver,const std::int32_t*& cell,std::string&);
bool borrow_source_campaign_player_gameplay_v67(const std::shared_ptr<void>& actual_world,PlayerGameplayBinding&,std::string&);
bool borrow_source_campaign_player_pre_gameplay_v68(const std::shared_ptr<void>& actual_world,PlayerGameplayBinding&,std::string&);
bool borrow_source_campaign_animation_tables_v67(const std::shared_ptr<void>& actual_world,std::shared_ptr<const void>& lease,const dh2::data::AnimationTables*&,std::string&);
bool borrow_source_campaign_character_cache_v81(const std::shared_ptr<void>& actual_world,std::shared_ptr<const dh2::character::CharacterCandidateCacheV62>&,std::string&);
 // Source AddCharacter endpoints: initialize_save precedes SetSlot; profile
 // creation belongs to init_all after the actual slot664/SetSlot delivery.
 bool source_campaign_initialize_player_save_v67(const std::shared_ptr<void>& actual_world,std::uintptr_t character,std::string&);
bool construct_source_campaign_character_spawn_v68(const std::shared_ptr<void>& actual_world,const dh2::world::CanonicalFactoryEntryV1&,const char* actual_name,dh2::world::CanonicalClassReceiverV1&,std::string&);
 bool source_campaign_initialize_selected_profile_v67(const std::shared_ptr<void>& actual_world,std::uintptr_t character,std::string&);
 bool borrow_source_campaign_world_map_tables_v68(const std::shared_ptr<void>& actual_world,std::uintptr_t character,std::shared_ptr<const dh2::data::WorldMapProfileTableV59>&,std::string&);
bool borrow_source_campaign_menu_continuations_v68(const std::shared_ptr<void>& actual_world,std::uintptr_t character,std::shared_ptr<void>& actor_lease,std::shared_ptr<dh2::data::PlayerSaveLoadOwnerV1>&,dh2::character::CharacterMenuMutationServicesV4&,dh2::character::CharacterMenuFaeryServicesV8&,std::string&);
}





namespace model_renderer {
bool borrow_source_campaign_character_lifetime_v107(const std::shared_ptr<void>& actual_world,
 std::uintptr_t actual_character,std::weak_ptr<void>&,std::string&);
}
