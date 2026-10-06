// PREPARATION ONLY: do not hook Hit8 until root binds every mandatory reached
// Level/DropLoot/XP/event source service. Include after renderer_npc_death_v2.inc.
// Headers: character_kill_live_v21.hpp and character_kill_loot_connection_v10.hpp.
// Root adds unique_ptr<RendererCharacterKillLiveV21> to SAME player runtime.
static bool renderer_kill_table101c_v21(void* raw,std::uintptr_t id,
 const std::int32_t*& out,std::string& error){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);out=nullptr;const auto found=t.world_actors.find(id);
 if(found==t.world_actors.end()||!found->second.npc||!found->second.npc->session||!found->second.object||!found->second.object->properties){
  error="Required actual NPC cached Character GetCharDecl101c owner";return false;
 }
 auto& view=found->second.npc->session->property_view();
 if(view.resolved!=found->second.object->properties->resolved.data()){
  error="Required SAME NPC cached property owner for GetCharDecl101c";return false;
 }
 // Original Structs header atff4, first actual property atff8: 101c=word9.
 // This is the raw authored declaration/table ID, not fixed-point conversion.
 out=view.resolved+9;return true;
}
static bool renderer_kill_npc_raise_v21(MonsterScriptHandle& npc,
 unsigned event,std::uintptr_t payload,const dh2_script_callback_scope* scope,std::string& error){
 if(event!=2||!npc.death||!npc.machine||!npc.object||npc.machine->native_fsm().character!=npc.object->identity||
    (scope&&!dh2_script_callback_scope_valid(scope))){
  error="Required actual NPC RaiseEvent2/source callback capability";return false;
 }
 struct Context {MonsterScriptHandle& npc;const dh2_script_callback_scope* scope;std::string& error;};
 Context context{npc,scope,error};
 const dh2::character::AIEventServices24 services{&context,
  [](void* raw,dh2::character::AIEventState64* state,const dh2::character::AIEventRequest40* q,std::uint32_t*){
   auto& c=*static_cast<Context*>(raw);auto& m=c.npc;using namespace dh2::character;
   if(!q||state!=&m.ai_events||q->event!=2)return -1;
   if(q->service==ai_event_virtual&&q->operation==0x24&&q->callee==0x3d1000&&q->subject==m.ai_events.ai){
    const int status=m.death->on_died(q->payload,c.scope);
    if(status)c.error=m.death->error();return status;
   }
   if(q->service==ai_event_state_event&&m.ai_events.owner&&q->subject==m.ai_events.owner->state_machine&&
      q->subject==reinterpret_cast<std::uintptr_t>(&m.machine->native_fsm())){
    const int status=m.machine->event(2,q->payload);
    if(status<0){c.error=m.machine->error();return -1;}return 0;
   }
   c.error="Required original NPC RaiseEvent2 service "+std::to_string(q->service);return -1;
  },63,0};
 const dh2::character::AIEventPayload24 data{payload,0,0,0};dh2::character::AIEventResult16 result{};
 if(dh2_character_ai_event(&result,&npc.ai_events,2,&data,&services)){
  if(error.empty())error="Required source Character RaiseEvent2/CharAI phase "+std::to_string(result.phase);return false;
 }return true;
}
struct RendererCharacterKillServicesV21 {
 std::shared_ptr<void> actual_provider_lease;
 // Borrow actual retained actor fields, including its sole C1/adopted
 // CharacterKillFieldsV21. Never create a fake base/metadata in this callback.
 std::function<bool(std::uintptr_t,dh2::character::CharacterKillLiveBorrowV21&,std::string&)> actor;
 // SAME current Level150 / full DropLoot / participant event4+victim event2 /
 // Trophy/XP / remote / constants / synchronous immediate RaiseAsync owner.
 std::function<bool(dh2::character::KillActor56&,const dh2::character::KillRequest56&,
  dh2::character::KillResponse16&,std::string&)> source;
 dh2::character::KillWorld16* actual_globals{};
 dh2::character::CharacterKillLootConnectionV10* actual_loot{};
};
class RendererCharacterKillLiveV21 {
 PlayerSkillsRuntime& t_;RendererCharacterKillServicesV21 services_;
 std::unique_ptr<dh2::character::CharacterKillLiveWorldV21> owner_;
 std::string error_;
public:
 RendererCharacterKillLiveV21(PlayerSkillsRuntime& t,RendererCharacterKillServicesV21 s)
  :t_(t),services_(std::move(s)){}
 bool bind(std::string& error){
  if(owner_||!t_.world||!t_.targets||!t_.player_manager||!services_.actual_globals||
     !services_.actual_provider_lease||!services_.actor||!services_.source){
   error="Required whole same World live Kill providers before publication";return false;
  }
  owner_=std::make_unique<dh2::character::CharacterKillLiveWorldV21>(*t_.targets,
   t_.player_manager->manager(),*services_.actual_globals,
   dh2::character::CharacterKillLiveBackendsV21{services_.actual_provider_lease,
    [this](auto& actor,const auto& q,auto& out,std::string& e){
     if(q.service==dh2::character::kill_drop_loot&&services_.actual_loot){
      bool handled{};if(!services_.actual_loot->route(actor,q,out,handled,e))return false;
      if(!handled){e="Required actual ordered Kill DropLoot route";return false;}return true;
     }
     return services_.source(actor,q,out,e);
    }});
  for(const auto& pair:t_.world_actors){dh2::character::CharacterKillLiveBorrowV21 fields;
   dh2::character::skills::WorldTargetActorBorrowV1 actual;
   if(t_.targets->actor(pair.first,&actual)){error="Required actual World actor classification for Kill";return false;}
   if(!actual.character)continue;
   if(!services_.actor(pair.first,fields,error)||fields.identity!=pair.first||!owner_->add(std::move(fields),error))return false;
  }return true;
 }
 // Handled1 / other0 / required failure-1. Root's Hit service converts1→0
 // actual delivery. Preserve actor pointer and q.subject controller identity.
 int hit(dh2::character::HitActor32* actor,const dh2::character::HitRequest32* q,std::uintptr_t* out){
  if(!q||q->service!=dh2::character::hit_controller_kill)return 0;
  error_.clear();if(!owner_||!actor||!out){error_="Required fully bound live Cmd_Kill owner";t_.error=error_;return -1;}
  const auto found=t_.world_actors.find(actor->identity);
  if(found==t_.world_actors.end()||!found->second.object||!found->second.object->life||!found->second.object->properties||!actor->properties||found->second.object->life->dead>1||
     actor->properties->resolved!=found->second.object->properties->resolved.data()){
   error_="Required same HitFor Character property/life graph";t_.error=error_;return -1;
  }
  const int status=owner_->command(q->subject,actor->identity,q->target,q->force);
  if(status!=1){error_=owner_->error();t_.error=error_;return -1;}
  *out=0;return 1;
 }
 const std::string& error()const noexcept{return error_;}
};
