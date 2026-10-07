// Native app global-scope include after the actual HUD/menu resource directory.
#include "source_campaign_death_rewards_v84.hpp"
#include "source_campaign_items_v88.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_settings_update_job_v102.hpp"
#include <integration-v42/audio_application_manager_v42.hpp>
#include <cstring>
bool model_renderer::bind_native_death_presentation_v84(const std::shared_ptr<void>& world,
 SourceDeathRewardsLeavesV84& leaves,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<void> menu;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||
  !borrow_native_menu_manager_v58(world,menu,e)){if(e.empty())e="Required SAME native death/XP MenuManager world";return false;}
 if(!bind_campaign_music_aggro_v101(world,leaves,e))return false;
 const std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application=candidate.application;
 leaves.settings_job_start=[application](std::string& e){auto app=application.lock();if(!app){e="Released SAME LevelUp settings Application";return false;}return start_process_settings_job_v102(app,e);};
 return original_ui.bind_death_rewards_text_v84(candidate,leaves,e);
}
bool model_renderer::bind_native_item_presentation_v88(const std::shared_ptr<void>& world,
 SourceCampaignItemLeavesV88& leaves,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world)return false;
 if(!original_ui.bind_item_presentation_v88(candidate,leaves,e))return false;
 struct Prefix {
  std::weak_ptr<void> world;
  static int invoke(void* raw,const dh2::sound::VoxPlay3DRequestV2& q,dh2::sound::VoxPlay3DResponseV2& out){
   auto& self=*static_cast<Prefix*>(raw);auto world=self.world.lock();std::string e;
   model_renderer::SourceCampaignCandidateBorrowV55 c;std::shared_ptr<model_renderer::SourceWorldBorrowV61> w;
   if(!world||!model_renderer::borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=world||!model_renderer::borrow_source_campaign_condition_world_v70(c,w,e))return -1;
   if(q.operation==dh2::sound::VoxPlay3DOperationV2::disabled){std::uint32_t bits{};
    if(!w->debug||!w->debug_files||dh2_character_debug_load(w->debug.get(),w->debug_files)!=1||dh2_character_debug_get(&bits,w->debug.get(),"IsDisablingSounds",w->debug_files)!=1)return -1;out.value=bits!=0;return 0;}
   if(q.operation==dh2::sound::VoxPlay3DOperationV2::current_level){dh2::loader::CanonicalCurrentLevelBorrowV1 level;if(!model_renderer::borrow_current_native_level_v27(level,e))return -1;
    out.identity=level.identity();out.value=0;if(level){dh2::loader::CanonicalLevelContextV1::LoadingFieldsV26 fields;if(!level.level()->loading_fields_v26(fields,e))return -1;std::memcpy(&out.value,fields.state130,4);}return 0;}
   return -1;
  }
 };
 struct AudioLease {std::shared_ptr<void> prior,source;std::shared_ptr<Prefix> prefix;dh2::audio::AudioApplicationBorrowV42 audio;};
 auto lease=std::make_shared<AudioLease>();lease->prior=leaves.owner;lease->prefix=std::make_shared<Prefix>();lease->prefix->world=world;
 //The composed WorldLootGameplay owns Debug/current-Level itself. This
 //callback is still the exact prefix transport required by the common Vox
 //borrower and is retained; submission never restarts the source Play3D body.
 if(!borrow_campaign_vox_services_v101(world,{lease->prefix.get(),Prefix::invoke},lease->audio,leaves.sound,lease->source,e))return false;
 leaves.vox_identity=lease->audio.identity();leaves.owner=lease;e.clear();return true;
}
bool model_renderer::borrow_native_item_localization_v88(const std::shared_ptr<void>& world,
 dh2::ui::HudTextV1*& text,dh2::ui::HudTextEnvironmentV1& environment,std::shared_ptr<void>& owner,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world)return false;
 std::shared_ptr<void> text_owner;if(!original_ui.localization_borrow_v4(text,owner,e)||
  !original_ui.gameplay_text_environment_v67(environment,text_owner,e))return false;
 if(!text||owner!=text_owner||owner.owner_before(text_owner)||text_owner.owner_before(owner)){e="Item formatting must borrow the SAME actual StringManager cache/environment";return false;}e.clear();return true;
}

bool model_renderer::bind_native_character_reward_text_v114(const std::shared_ptr<void>& world,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world)return false;
 return original_ui.bind_character_reward_text_v114(candidate,e);
}

bool model_renderer::bind_native_campaign_combat_presentation_v115(const std::shared_ptr<void>& world,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world)return false;
 return original_ui.bind_combat_presentation_v115(candidate,e);
}
