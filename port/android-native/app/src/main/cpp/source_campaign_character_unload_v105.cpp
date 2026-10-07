#include "source_campaign_character_unload_v105.hpp"
#include "renderer_character_campaign_v62.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <character_loading_queue_v105.hpp>
namespace model_renderer {
namespace {
struct UnloadV105 {std::shared_ptr<void> world;std::string* error;};
int unload_ai(void* raw,dh2::character::CharacterDeferredQueue*,const dh2::character::DeferredQueueRequest32* q){
 auto& context=*static_cast<UnloadV105*>(raw);
 if(!q||q->operation!=dh2::character::deferred_queue_unload_ai)return -1;
 SourceCampaignCharacterBorrowV62 receiver;
 if(!borrow_source_campaign_character_v62(context.world,q->owner,receiver,*context.error))return -1;
 auto& record=*receiver.character;auto& actor=*record.actor;int status{};
 if(record.player_script_owner_v62)status=record.player_script_owner_v62->native_unload_script_v105(q->final!=0);
 else if(record.npc_skills_v84)status=record.npc_skills_v84->unload_script_v105(actor,q->final!=0);
 else if(!actor.session)return 0; // actual AI20 NULL before first script load.
 else{auto& state=actor.session->owner().lifecycle();
  if(!state.active||(!q->final&&!state.delayed))return 0;
  const auto counts=actor.source_ctor_empty_skill_vectors_v84();
  if(!counts||(*counts)[0]||(*counts)[1]){*context.error="Required same skill/spell ownership for AIUnLoadScriptProcess";return -1;}
  // Constructor-empty vectors still execute both cleanup bodies. They have
  // no elements/callbacks; only their owned AIS D0/reset remains.
  status=actor.session->owner().source_release_active_v105();
 }
 if(status<0){if(context.error->empty())*context.error="Actual AIUnLoadScriptProcess failed after source prefix";return -1;}
 actor.ai_events.active=0;actor.ai_events.ais_virtuals=nullptr;return 0;
}
}
bool source_campaign_character_unload_script_v105(const std::shared_ptr<void>& world,std::uintptr_t id,bool final,std::string& e){
 SourceCampaignCharacterBorrowV62 receiver;if(!borrow_source_campaign_character_v62(world,id,receiver,e))return false;
 auto queue=dh2::character::character_loading_queue_v105();
 if(!queue||!receiver.character->actor){e="Required original Character loading-map lifetime";return false;}
 dh2::character::DeferredQueueOwner16 owner{id,receiver.character->actor->animation_ai.controller};
 dh2::character::DeferredQueueToken24 end{queue.get(),0,0,0};UnloadV105 context{world,&e};
 dh2::character::DeferredQueueServices24 services{&context,unload_ai,1u<<dh2::character::deferred_queue_unload_ai,0};
 const auto result=dh2_character_deferred_queue_unload(queue.get(),&owner,&end,final,&services);
 if(result){if(e.empty())e="Source UnLoadScriptProcess pending-map/AI delivery failed";return false;}return true;
}
bool source_campaign_character_unload_ai_v108(const std::shared_ptr<void>& world,std::uintptr_t id,bool final,std::string& e){
 UnloadV105 context{world,&e};const dh2::character::DeferredQueueRequest32 request{dh2::character::deferred_queue_unload_ai,static_cast<unsigned>(final),id,0,0,0};
 const bool okay=unload_ai(&context,nullptr,&request)==0;
 if(!okay&&e.empty())e="Actual AI_UnLoadScriptProcess body failed after queue erase";return okay;
}
}
