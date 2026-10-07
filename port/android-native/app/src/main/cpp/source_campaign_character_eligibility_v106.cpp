#include "source_campaign_character_eligibility_v106.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include "source_campaign_runtime_v61.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <application_services_owner_v5.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <character_can_update.hpp>
#include <character_ai_groups_v87.hpp>
namespace model_renderer {
namespace {
struct EligibilityV106 {
 SourceCampaignCandidateBorrowV55 candidate;
 SourceCampaignCharacterBorrowV62 receiver;
 dh2::character::CanUpdateNode8 node{};
 dh2::character::CanUpdateVisual8 visual{};
 dh2::character::CanUpdateNode8 entry_node{};
 dh2::character::CanUpdateVisual8 entry_visual{};
 dh2::character::CanUpdateOwner40 owner{};
 std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> root;
 std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> entry_root;
 bool refresh(std::string& e){
  auto& actor=*receiver.character->actor;auto pointer=actor.source_ai_pointers_v105();
  auto visible=actor.source_bool_field(0x80),force=actor.source_bool_field(0x2fc),interaction=actor.source_bool_field(0x1480);
  if(!pointer||!visible||!force||!interaction){e="Required produced Character CanUpdate80/2fc/1480/AI50";return false;}
  owner.enabled=*visible;owner.force_update=*force;owner.interaction=*interaction;owner.player_link=pointer->master50;
  // The root loan pins both the captured entry node and reloaded projection.
  root=receiver.character->visual?receiver.character->visual->visual():nullptr;owner.visual=nullptr;
  if(root){const auto& culling=root->source_automatic_culling_v70();if(!culling){e="Required source Root automatic culling118";return false;}
   node.culling=*culling;node.animate_enabled=root->source_character_update200_v106();visual.node=&node;owner.visual=&visual;}
  return true;
 }
 static int invoke(void* context,dh2::character::CanUpdateOwner40*,const dh2::character::CanUpdateRequest24* q,dh2::character::CanUpdateResponse16* out){
  using namespace dh2::character;auto& t=*static_cast<EligibilityV106*>(context);auto& record=*t.receiver.character;auto& e=record.error;
  if(!q||!out)return -1;bool value{};
  //Source clears the captured Root200 before the first synchronous query.
  //Publish that prefix before callbacks can inspect or replace the visual;
  //refresh must not resurrect the old1 from native backing.
  if(t.entry_root)t.entry_root->source_character_update200_v106()=t.entry_node.animate_enabled;
  switch(q->operation){
   case can_update_online:{auto online=t.candidate.application->get_online_loading_v55();if(!online)return -1;out->word=online->byte5();break;}
   case can_update_remote:{auto fields=record.actor->machine?&record.actor->machine->combat_fields():nullptr;auto remote=record.actor->source_bool_field(0x118);if(!fields||!remote)return -1;out->word=fields->network_id!=-1||*remote;break;}
   case can_update_player:{auto pm=t.candidate.application->source_player_manager_v59();dh2::player::PlayerInfoFieldsV1* player{};
    if(!pm||!pm->get_local_player(0,true,player,e)||!player)return -1;out->identity=player->character660;break;}
   case can_update_culling:{std::array<std::array<float,4>,6> planes;
    if(!borrow_source_campaign_room_camera_v104(t.candidate.actual_world,planes,e))return -1;
    const auto* bounds=record.actor->runtime.subobjects.absolute_bounds;value=true;
    for(const auto& p:planes){const float x=p[0]>=0?bounds[0]:bounds[3],y=p[1]>=0?bounds[1]:bounds[4],z=p[2]>=0?bounds[2]:bounds[5];
     if(((p[0]*x+p[1]*y)+p[2]*z)+p[3]>0.f){value=false;break;}}
    out->word=value;break;}
   case can_update_dead:if(!record.life)return -1;out->word=record.life->dead!=0;break;
   case can_update_respawn:if(!source_character_can_respawn_v87(record,value,e))return -1;out->word=value;break;
   default:return -1;
  }
  return t.refresh(e)?0:-1;
 }
};
}
bool source_campaign_character_with_eligibility_v108(const std::shared_ptr<void>& world,std::uintptr_t id,
 const std::function<bool(dh2::character::CanUpdateOwner40&,const dh2::character::CanUpdateServices24&,std::string&)>& body,std::string& e){
 EligibilityV106 scope;
 if(!borrow_source_campaign_candidate_v55(scope.candidate,e)||scope.candidate.actual_world!=world||
   !borrow_source_campaign_character_v62(world,id,scope.receiver,e)||!scope.receiver.character->actor)return false;
 scope.owner.identity=id;scope.owner.bounds=reinterpret_cast<std::uintptr_t>(scope.receiver.character->actor->runtime.subobjects.absolute_bounds);
 if(!scope.refresh(e))return false;
 auto captured=scope.root;scope.entry_root=captured;
 if(captured){scope.entry_node=scope.node;scope.entry_visual.node=&scope.entry_node;scope.owner.visual=&scope.entry_visual;}
 if(!body){e="Required scoped Character.CanUpdate caller";return false;}
 dh2::character::CanUpdateServices24 services{&scope,EligibilityV106::invoke,0x3fu,0};
 const auto okay=body(scope.owner,services,e);
 // Final store targets the original captured root even after synchronous queries.
 if(captured)captured->source_character_update200_v106()=scope.entry_node.animate_enabled;
 if(!okay&&e.empty())e=scope.receiver.character->error.empty()?"Actual Character.CanUpdate source provider failed":scope.receiver.character->error;
 return okay;
}
bool source_campaign_character_can_update_v106(const std::shared_ptr<void>& world,std::uintptr_t id,bool& accepted,std::string& e){
 return source_campaign_character_with_eligibility_v108(world,id,[&](auto& owner,const auto& services,auto& error){
  std::uint32_t result{};if(dh2_character_can_update(&owner,&services,&result)){if(error.empty())error="Actual Character.CanUpdate source provider failed";return false;}
  accepted=result!=0;return true;
 },e);
}
}
