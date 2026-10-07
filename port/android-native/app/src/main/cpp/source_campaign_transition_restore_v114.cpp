#include "source_campaign_transition_restore_v114.hpp"
#include "player_gameplay_binding.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_release_v88.hpp"
#include "source_campaign_retirement_v88.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include <canonical_character_candidate_v60.hpp>
namespace model_renderer {
bool capture_source_campaign_transition_restore_v114(const std::shared_ptr<void>& world,
 dh2::loader::AreaTransitionRestoreReceiptV114& out,std::string& error){
 out={};SourceCampaignCandidateBorrowV55 candidate;
 std::shared_ptr<SourceWorldBorrowV61> source;PlayerGameplayBinding player;
 SourceCampaignCharacterBorrowV62 selected;
 if(!world||source_campaign_retirement_requested_v88()||
    !borrow_source_campaign_candidate_v55(candidate,error)||candidate.actual_world!=world||
    !borrow_source_campaign_condition_world_v70(candidate,source,error)||
    !candidate.level||candidate.level->constructor_fields_v3().field130!=38||
    !borrow_source_campaign_player_gameplay_v67(world,player,error)||!player.active||
    !borrow_source_campaign_character_v62(world,player.character,selected,error)){
  if(error.empty())error="Required current playable campaign for confirmed-area save";return false;
 }
 const auto record=selected.character;
 const auto bootstrap=record?record->profile_bootstrap:nullptr;
 const auto profile=bootstrap?bootstrap->profile():nullptr;
 const auto writer=bootstrap?bootstrap->campaign_writer():nullptr;
 if(!record||!record->save||!record->load||!bootstrap||!bootstrap->finished()||
    !writer||!writer->ready()||!profile||!profile->ready()||profile->destroyed_v108()||
    record->save.get()!=player.save||record->save->slot()<0||record->save->slot()>3||
    record->load->save_disabled()||record->load->profile().identity!=reinterpret_cast<std::uintptr_t>(profile.get())||
    source->files_directory.empty()){
  error="Required unblocked SAME live profile and registered campaign writers";return false;
 }
 const auto slot=record->save->slot();
 // Preserve the existing source level/date/entry and full named-section
 // writer ordering. No second Save or hand-written serialization is created.
 if(!source_campaign_save_all_players_v88(candidate,false,error))return false;
 SourceCampaignCandidateBorrowV55 current;SourceCampaignCharacterBorrowV62 same;
 if(source_campaign_retirement_requested_v88()||
    !borrow_source_campaign_candidate_v55(current,error)||current.actual_world!=world||
    current.level!=candidate.level||current.objects!=candidate.objects||
    !borrow_source_campaign_character_v62(world,player.character,same,error)||
    same.character!=record||record->save->slot()!=slot||bootstrap->profile()!=profile){
  if(error.empty())error="Campaign/profile changed during area save prefix";return false;
 }
 auto bytes=std::make_shared<dh2::data::CampaignProfileFileV1>();
 {
  const auto cache=profile->cache();
  if(!cache||cache.bytes().empty()){error="Actual serialized profile cache absent after save";return false;}
  bytes->bytes=cache.bytes();
 } // Release the index Borrow BEFORE unload can reload the same profile.
 out={std::move(bytes),source->files_directory,slot,player.character};
 error.clear();return true;
}
}
