#include "game_event_runtime_v75.hpp"
#include <cstring>
#include <stdexcept>
#include <algorithm>
namespace dh2::loader {namespace {
std::int32_t signed_bits(std::uint32_t v){std::int32_t r;std::memcpy(&r,&v,4);return r;}
std::uint32_t bits(std::int32_t v){std::uint32_t r;std::memcpy(&r,&v,4);return r;}
bool quantity(ObjectiveBorrowV75& f,std::int32_t& out,std::string& e){
 if(!f.words20_2c[0]){e="Required actual Objective quantity20 producer";return false;}
 out=signed_bits(*f.words20_2c[0]);return true;
}
struct Busy {bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}};
}
ScopedGameQuestEventV75::ScopedGameQuestEventV75(std::uintptr_t id,const std::int32_t* type,GameEventQuestBorrowV75 fields):
 identity_(id),type4_(type),fields_(std::move(fields)){
 if(!identity_||!type4_||!fields_.receiver||!fields_.pending_network10||!fields_.from_network11||!fields_.quantity14||
    !fields_.character8_cell||(!fields_.object18_cell&&!fields_.id18_cell))
  throw std::invalid_argument("Required actual typed QuestEvent source fields and scoped payload lease");
}
bool ScopedGameQuestEventV75::type(void* raw,std::int32_t& result,std::string& e){
 auto* self=static_cast<ScopedGameQuestEventV75*>(raw);
 if(!self||!self->identity_||!self->type4_||!self->fields_.receiver){e="Actual QuestEvent source scope expired";return false;}
 result=*self->type4_;e.clear();return true;
}
bool project_scoped_game_quest_event_v75(const events::EventBorrowV12& event,GameEventQuestBorrowV75& out,std::string& e){
 if(event.get_type!=ScopedGameQuestEventV75::type||!event.context){e.clear();return false;}
 auto* actual=static_cast<ScopedGameQuestEventV75*>(event.context);
 if(!actual->fields_.receiver||event.identity!=actual->identity_||!event.lifetime||
    event.lifetime.get()!=actual->fields_.receiver.get()||event.lifetime.owner_before(actual->fields_.receiver)||
    actual->fields_.receiver.owner_before(event.lifetime)){e="QuestEvent scoped projection addressed another source payload";return false;}
 out=actual->fields_;out.character8=*out.character8_cell;
 if(out.object18_cell)out.object18=*out.object18_cell;
 if(out.id18_cell)out.id18=*out.id18_cell;
 e.clear();return true;
}
std::uint32_t ObjectiveWordBorrowV75::operator*()const {
 if(saved_){std::uint32_t out;std::memcpy(&out,saved_,4);return out;}
 if(constructed_&&*constructed_)return **constructed_;
 throw std::runtime_error("Required actual constructed Objective word");
}
ObjectiveWordBorrowV75& ObjectiveWordBorrowV75::operator=(std::uint32_t v){
 if(saved_)std::memcpy(saved_,&v,4);
 else if(constructed_)*constructed_=v;
 else throw std::runtime_error("Required actual Objective word storage");
 return *this;
}
ObjectiveBorrowV75 borrow_game_event_objective_v75(const std::shared_ptr<GameEventManagerV50>& manager,GameEventObjectiveV50& objective){
 auto& f=objective.fields();
 return {std::shared_ptr<void>(manager,&objective),reinterpret_cast<std::uintptr_t>(&objective),f.type4,f.byte8,f.data_c,f.owner10,f.completed14,f.byte1c,
  {ObjectiveWordBorrowV75(&f.words20_2c[0]),ObjectiveWordBorrowV75(&f.words20_2c[1]),ObjectiveWordBorrowV75(&f.words20_2c[2]),ObjectiveWordBorrowV75(&f.words20_2c[3])}};
}
bool GameEventRuntimeV75::invalidate_objective(ObjectiveBorrowV75 objective,std::string& e){
 if(native_retiring_v88_){e="Objective runtime delivery after native storage retirement began";return false;}
 if(failed_){e=failure_;return false;}
 if(!objective.receiver||!objective.identity)return fail("Required SAME Objective receiver lifetime",e);
 objective.completed14=0;objective.byte8=0;e.clear();return true;
}
struct GameEventRuntimeV75::Receiver {
 std::weak_ptr<GameEventRuntimeV75> runtime;
 std::weak_ptr<void> objective_lifetime;
 ObjectiveBorrowV75 objective;
 struct RegistrationV88 {std::weak_ptr<void> level;events::EventManagerOwnerV12* dispatcher{};std::uintptr_t dispatcher_identity{};};
 std::vector<RegistrationV88> registrations_v88;
 bool retired_v88{};
 explicit Receiver(ObjectiveBorrowV75 b):objective_lifetime(b.receiver),objective(std::move(b)){
  objective.receiver.reset(); //No Quest-owner -> shared runtime -> Quest-owner cycle.
 }
 static bool dispatch(void* raw,const events::EventBorrowV12& event,events::EventManagerOwnerV12&,
  std::int32_t& result,std::string& e){
  auto& self=*static_cast<Receiver*>(raw);auto owner=self.runtime.lock();
  auto actual=self.objective_lifetime.lock();
  if(self.retired_v88||!owner||!actual){e="Actual Objective observer lifetime expired";return false;}
  auto borrowed=self.objective;borrowed.receiver=std::move(actual);
  try{return owner->on_event(borrowed,event,result,e);}
  catch(const std::exception& ex){return owner->fail(ex.what(),e);}
  catch(...){return owner->fail("Objective observer provider threw",e);}
 }
};
GameEventRuntimeV75::GameEventRuntimeV75(std::shared_ptr<GameEventManagerV50> manager,GameEventRuntimeServicesV75 s):
 manager_(std::move(manager)),services_(std::move(s)){
 if(!manager_||!services_.provider)throw std::invalid_argument("Required SAME GameEvent194 manager and source service lifetime");
}
GameEventRuntimeV75::GameEventRuntimeV75(GameEventRuntimeServicesV75 s):services_(std::move(s)){
 if(!services_.provider)throw std::invalid_argument("Required actual shared Objective callback provider");
}
bool GameEventRuntimeV75::attach_manager_v75(std::shared_ptr<GameEventManagerV50> manager,std::string& e){
 if(native_retiring_v88_){e="Objective runtime delivery after native storage retirement began";return false;}
 if(failed_){e=failure_;return false;}
 if(busy_||!manager||!manager->diagnostics().storage_load_complete||
    (manager_&&(manager_.get()!=manager.get()||manager_.owner_before(manager)||manager.owner_before(manager_))))
  return fail("Event runtime requires the SAME completed194 storage producer",e);
 manager_=std::move(manager);e.clear();return true;
}
bool GameEventRuntimeV75::fail(const std::string& message,std::string& e){
 if(!failed_){failed_=true;failure_=message.empty()?"GameEvent source provider failed":message;}
 e=failure_;return false;
}
bool GameEventRuntimeV75::current(std::shared_ptr<void>& lease,events::EventManagerOwnerV12*& manager,
 const std::int32_t*& row,std::string& e){
 if(!services_.current_level||!services_.current_level(lease,manager,row,e))return fail(e.empty()?"Required actual Application.GetCurrentLevel":e,e);
 if(!lease||!manager||!row)return fail("Required actual nonNULL Level inherited EventManager/row3c",e);
 return true;
}
bool GameEventRuntimeV75::complete(ObjectiveBorrowV75 objective,std::string& e){
 if(native_retiring_v88_){e="Objective runtime delivery after native storage retirement began";return false;}
 if(failed_){e=failure_;return false;}
 if(!objective.receiver||!objective.identity)return fail("Required SAME Objective receiver lifetime",e);
 auto& f=objective;if(f.completed14)return true;
 if(!f.data_c)return fail("Required actual Objective.data_c",e);
 f.completed14=1; //47ba2c precedes every script lookup/callback.
 const auto script=f.data_c->fieldc;if(script<0)return true;
 std::int32_t common{};
 if(!services_.common_script_count||!services_.common_script_count(common,e))return fail(e.empty()?"Required SAME ScriptManager.common8":e,e);
 if(!services_.start_script||!services_.start_script(signed_bits(bits(script)+bits(common)),-1,false,e))
  return fail(e.empty()?"Required whole ScriptManager.StartScript4605c0":e,e);
 return true;
}
bool GameEventRuntimeV75::register_objective(ObjectiveBorrowV75 objective,bool adding,std::string& e){
 if(native_retiring_v88_){e="Objective runtime delivery after native storage retirement began";return false;}
 if(failed_){e=failure_;return false;}
 if(!objective.receiver||!objective.identity)return fail("Required SAME Objective receiver lifetime",e);
 auto& f=objective;if(f.type4==6)return true; //Automatic actual BX LR.
 if(adding&&!f.byte8)return true;
 std::shared_ptr<void> level;events::EventManagerOwnerV12* dispatcher{};const std::int32_t* row{};
 if(!current(level,dispatcher,row,e))return false;
 auto found=receivers_.find(objective.identity);
 if(adding){
  if(found==receivers_.end()){
   auto receiver=std::make_shared<Receiver>(objective);receiver->runtime=shared_from_this();
   found=receivers_.emplace(objective.identity,std::move(receiver)).first;
  }
  const auto& receiver=found->second;bool inserted{};
  // Embedded IEventReceiver+18 has its own native identity; its field/data
  // owner remains the SAME objective. Snapshot delivery pins this adapter.
  events::EventReceiverV12 route{reinterpret_cast<std::uintptr_t>(receiver.get()),receiver.get(),Receiver::dispatch,receiver};
  //Keep native registration provenance before publishing any raw callback.
  //Weak containing-level leases prevent Level->dispatcher->receiver cycles.
  if(std::none_of(receiver->registrations_v88.begin(),receiver->registrations_v88.end(),[dispatcher](const auto& r){return r.dispatcher==dispatcher&&r.dispatcher_identity==dispatcher->identity();}))
   receiver->registrations_v88.push_back({level,dispatcher,dispatcher->identity()});
  if(!dispatcher->attach(f.type4,route,0,inserted,e))return fail(e,e);
  f.byte1c=1;
 }else{
  bool scheduled{};
  if(found!=receivers_.end()&&!dispatcher->delayed_detach(f.type4,reinterpret_cast<std::uintptr_t>(found->second.get()),scheduled,e))return fail(e,e);
  f.byte1c=0; //Source writes even when no registered node was found.
 }
 //TalkToNPC qualified Register/Unregister calls its base first, then scans
 //actual ObjectManager.characters and writes only the first matching2fa.
 if(f.type4==5&&f.byte8&&f.data_c&&f.data_c->type4==5){
  if(!f.words20_2c[1])return fail("Required actual TalkToNPC compiled24",e);
  if(!services_.talk_flag||!services_.talk_flag(signed_bits(*f.words20_2c[1]),adding?1:0,e))
   return fail(e.empty()?"Required source TalkToNPC first matching Character2fa write":e,e);
 }
 if(f.type4==12&&f.byte8){
  if(!f.data_c||!services_.inventory_register||!services_.inventory_register(f.owner10,f.data_c->field24,adding,e))
   return fail(e.empty()?"Required SAME Character inventory gathering-ID lifecycle":e,e);
 }
 return true;
}
bool GameEventRuntimeV75::compile_objective(ObjectiveBorrowV75 objective,std::string& e){
 if(native_retiring_v88_){e="Objective runtime delivery after native storage retirement began";return false;}
 if(failed_){e=failure_;return false;}
 if(!objective.receiver||!objective.identity)return fail("Required SAME Objective receiver lifetime",e);
 auto& f=objective;if(!f.data_c||f.type4<0||f.type4>12)return fail("Required original Objective constructor/data",e);
 const auto& data=*f.data_c;
 if(f.type4==6){f.byte8=1;return complete(objective,e);} //47bd80.
 if(f.type4==12){ //GatherLoot47bae4 / InitWithCurrentQty47aba4.
  if(data.field24==-1&&(!services_.assertion||!services_.assertion(0x3da,"objData->Loot != -1",e)))return fail(e,e);
  if(data.field24<=0&&(!services_.assertion||!services_.assertion(0x3db,"objData->Loot > 0",e)))return fail(e,e);
  if(data.field24==-1||data.field28<=0)return true;
  std::shared_ptr<void> level;events::EventManagerOwnerV12* dispatcher{};const std::int32_t* row{};
  if(!current(level,dispatcher,row,e))return false;
  if(data.field20!=-1&&data.field20!=*row)return true;
  bool found{};std::int16_t count{};
  if(!services_.inventory_quantity||!services_.inventory_quantity(f.owner10,data.field24,found,count,e))
   return fail(e.empty()?"Required ItemInventory.FindItem/Item50 signed quantity":e,e);
  if(found)f.words20_2c[0]=bits(count); //Source miss preserves previous20.
  f.byte8=1;std::int32_t quantity20{};if(!quantity(f,quantity20,e))return fail(e,e);
  return quantity20<data.field28||complete(objective,e);
 }
 if(f.type4==2||f.type4==3||f.type4==5||f.type4==7||f.type4==8||f.type4==9){
  f.words20_2c[1]=bits(data.field20);
  if(data.field20==-1)return true;
  f.byte8=1;std::int32_t count{};if(!quantity(f,count,e))return fail(e,e);
  if(count>=data.field28)return complete(objective,e)&&register_objective(objective,false,e);
  return true;
 }
 if(f.type4==4){ //MoveInZone47c868, no owner10/player shortcut.
  GameEventObjectBorrowV75 zone;
  if(!services_.zone_by_name||!services_.zone_by_name(data.string1c.c_str(),zone,e))
   return fail(e.empty()?"Required ObjectManager.GetObjectByName/MeetCondition(actual Zone)":e,e);
  zones_[objective.identity]=std::move(zone);
  if(!zones_[objective.identity].identity)return true;
  if(!zones_[objective.identity].receiver)return fail("Actual MoveInZone compiled object has no receiver lease",e);
  if(data.field24!=-1){
   std::int32_t count{};if(!services_.enemies_loaded||!services_.enemies_loaded(false,data.field24,count,e))
    return fail(e.empty()?"Required whole HasEnemyOfTypeLoaded class query":e,e);
   if(!count)return true;
  }
  f.byte8=1;return true;
 }
 //KillEnemies0/template10 uses authored required count28; ClearEnemies1
 //captures actual loaded count. Source counts include dead retained entries.
 const bool templ=f.type4==10||f.type4==11;std::int32_t loaded{};
 std::shared_ptr<void> level;events::EventManagerOwnerV12* dispatcher{};const std::int32_t* row{};
 if(f.type4==1||f.type4==11){
  if(!current(level,dispatcher,row,e))return false;
  if(data.field20!=-1&&data.field20!=*row)return true;
  if(!services_.enemies_loaded||!services_.enemies_loaded(templ,data.field24,loaded,e))
   return fail(e.empty()?"Required whole ClearEnemies loaded class query":e,e);
  f.words20_2c[3]=bits(loaded);if(loaded<=0)return true;
 }else{
  f.words20_2c[3]=bits(data.field28);
  if(!services_.enemies_loaded||!services_.enemies_loaded(templ,data.field20,loaded,e))
   return fail(e.empty()?"Required whole HasEnemyOfTypeLoaded source cache fallback":e,e);
  if(!current(level,dispatcher,row,e))return false;
  if((data.field24!=-1&&data.field24!=*row)||loaded<=0||data.field28<=0){f.byte8=0;return true;}
 }
 f.byte8=1;std::int32_t count{};if(!quantity(f,count,e))return fail(e,e);
 if(count>=signed_bits(*f.words20_2c[3]))return complete(objective,e)&&register_objective(objective,false,e);
 return true;
}
bool GameEventRuntimeV75::compile_event(GameEventV50& event,std::string& e){
 //Source whole-list invalidation completes BEFORE any derived Compile.
 for(const auto& objective:event.objectives()){
  if(!objective)return fail("Required actual ObjectiveList initialized receiver",e);
  if(!invalidate_objective(borrow_game_event_objective_v75(manager_,*objective),e))return false;
 }
 for(const auto& objective:event.objectives())if(!compile_objective(borrow_game_event_objective_v75(manager_,*objective),e))return false;
 if(!event.fields().registered18){
  for(const auto& objective:event.objectives())if(!register_objective(borrow_game_event_objective_v75(manager_,*objective),true,e))return false;
  event.fields().registered18=1;
 }
 return true;
}
bool GameEventRuntimeV75::set_state(GameEventV50& event,std::int32_t value,std::string& e){
 if(bits(value)>2)return true;
 event.fields().state0=value;if(value!=2)return true;
 if(!event.fields().data1c)return fail("Required actual GameEvent.data1c",e);
 std::int32_t id{};
 if(!services_.script_id||!services_.script_id(event.fields().data1c->script_c.c_str(),id,e))
  return fail(e.empty()?"Required TempHackScriptIDAreStrings SAME ScriptManager lookup":e,e);
 if(id<0)return true;
 if(!services_.start_script||!services_.start_script(id,-1,false,e))return fail(e.empty()?"Required GameEvent.ExecScript StartScript":e,e);
 return true;
}
bool GameEventRuntimeV75::update_event(GameEventV50& event,std::string& e){
 auto lookup=[&](const char* key,std::int32_t& result){
  if(!services_.constant||!services_.constant("v2EventState",key,result,e))return fail(e.empty()?"Required actual PyDataConstants v2EventState":e,e);
  return true;
 };
 std::int32_t value{};auto captured=event.fields().state0;
 if(!lookup("Inactive",value))return false;
 if(captured==value){
  for(const auto& objective:event.objectives())if(!objective||!objective->fields().byte8)return true;
  if(!lookup("Active",value))return false;return set_state(event,value,e);
 }
 captured=event.fields().state0;if(!lookup("Active",value))return false;
 if(captured==value){
  for(const auto& objective:event.objectives())if(!objective||!objective->fields().completed14)return true;
  if(!lookup("Completed",value))return false;return set_state(event,value,e);
 }
 return lookup("Completed",value); //Source third lookup retained even though return value ignored.
}
bool GameEventRuntimeV75::on_event(ObjectiveBorrowV75 objective,const events::EventBorrowV12& event,
 std::int32_t& result,std::string& e){
 result=0;if(failed_){e=failure_;return false;}auto& f=objective;
 if(!f.byte8){if(!services_.assertion||!services_.assertion(0xd5,"false",e))return fail(e.empty()?"Required actual Objective assertion mode":e,e);return true;}
 if(f.completed14)return true;
 GameEventQuestBorrowV75 q;
 if(!services_.project_event||!services_.project_event(event,q,e)||!q.receiver||!q.pending_network10||!q.from_network11||!q.quantity14)
  return fail(e.empty()?"Required typed SAME QuestEvent8/10/11/14/18 payload":e,e);
 if(!f.data_c)return fail("Required actual objective data",e);
 const auto& data=*f.data_c;bool matches{};
 if(f.type4==4){
  const auto zone=zones_.find(objective.identity);
  matches=zone!=zones_.end()&&q.object18==zone->second.identity;
  if(matches){
   if(!q.character8){
    if(*q.from_network11)matches=true;
    else if(!services_.character_props||!services_.character_props(0,q.id18,e))return fail(e.empty()?"Required SafeGetCharPropsId(NULL)":e,e);
    else matches=q.id18==data.field24;
   }else if(data.field24==-1){
    if(!services_.character_is_player||!services_.character_is_player(q.character8,matches,e))return fail(e.empty()?"Required actual Character.IsPlayer":e,e);
    if(!matches){std::int32_t id{};if(!services_.character_props||!services_.character_props(q.character8,id,e))return fail(e,e);matches=id==data.field24;}
   }else{std::int32_t id{};if(!services_.character_props||!services_.character_props(q.character8,id,e))return fail(e,e);matches=id==data.field24;}
   if(matches){if(!complete(objective,e)||!register_objective(objective,false,e))return false;if(!*q.from_network11)*q.pending_network10=1;}
  }
 }else if(f.type4==6)return true;
 else if(f.type4==12){
  if(q.id18==data.field24&&q.character8==f.owner10){
   bool found{};std::int16_t count{};
   if(!services_.inventory_quantity||!services_.inventory_quantity(f.owner10,data.field24,found,count,e))return fail(e,e);
   if(found)f.words20_2c[0]=bits(count);
   if(!*q.from_network11){*q.pending_network10=1;std::int32_t quantity20{};if(!quantity(f,quantity20,e))return fail(e,e);*q.quantity14=quantity20;}
   else f.words20_2c[0]=bits(*q.quantity14);
   std::int32_t quantity20{};if(!quantity(f,quantity20,e))return fail(e,e);
   if(quantity20>=data.field28&&!complete(objective,e))return false;
  }
 }
 else{
  const auto required=(f.type4==0||f.type4==10)?data.field20:((f.type4==1||f.type4==11)?data.field24:data.field20);
  matches=q.id18==required;
  if(matches){
   std::int32_t count{};if(!quantity(f,count,e))return fail(e,e);
   if(!*q.from_network11){count=signed_bits(bits(count)+1);f.words20_2c[0]=bits(count);*q.pending_network10=1;*q.quantity14=count;}
   else if(*q.quantity14>count){count=*q.quantity14;f.words20_2c[0]=bits(count);}
   else matches=false;
   const auto needed=(f.type4==0||f.type4==1||f.type4==10||f.type4==11)?signed_bits(*f.words20_2c[3]):data.field28;
   if(matches&&count>=needed&&!(complete(objective,e)&&register_objective(objective,false,e)))return false;
  }
 }
 //Actual wrapper calls CheckMustSendThroughNetwork after every handleEvent.
 if(!services_.send_network||!services_.send_network(q,e))return fail(e.empty()?"Required actual source QuestEvent network guard":e,e);
 return true;
}
bool GameEventRuntimeV75::compile(std::string& e){
 if(native_retiring_v88_){e="Objective runtime delivery after native storage retirement began";return false;}
 if(failed_){e=failure_;return false;}if(busy_)return fail("GameEvent.Compile reentered; preserving reached prefix",e);
 if(!manager_||!manager_->diagnostics().storage_load_complete)return fail("Required real GameEvent.Load completion",e);
 Busy busy(busy_);try{for(const auto& event:manager_->events())if(!event||!compile_event(*event,e)||failed_)return fail(e,e);
  compiled_=true;e.clear();return true;}catch(const std::exception& ex){return fail(ex.what(),e);}catch(...){return fail("GameEvent.Compile provider threw",e);}
}
bool GameEventRuntimeV75::update(std::string& e){
 if(native_retiring_v88_){e="Objective runtime delivery after native storage retirement began";return false;}
 if(failed_){e=failure_;return false;}if(busy_)return fail("GameEvent.Update reentered",e);if(!manager_||!compiled_)return fail("Required whole GameEvent Compile/Register before Update",e);
 Busy busy(busy_);try{for(const auto& event:manager_->events())if(!event||!update_event(*event,e)||failed_)return fail(e,e);
  e.clear();return true;}catch(const std::exception& ex){return fail(ex.what(),e);}catch(...){return fail("GameEvent.Update provider threw",e);}
}
bool GameEventRuntimeV75::reinit(std::string& e){
 if(native_retiring_v88_){e="Objective runtime delivery after native storage retirement began";return false;}
 if(failed_){e=failure_;return false;}if(busy_)return fail("GameEvent.ReInit reentered",e);
 if(!manager_)return fail("Required actual GameEvent194 before ReInit",e);
 for(const auto& event:manager_->events())if(!event||!event->reinit_storage(e))return fail(e,e);
 return compile(e);
}
bool GameEventRuntimeV75::destroy_objective_native_v88(const std::shared_ptr<GameEventManagerV50>& storage,GameEventObjectiveV50& objective,std::string& e){
 if(busy_||!storage||(manager_&&(manager_.get()!=storage.get()||manager_.owner_before(storage)||storage.owner_before(manager_)))){e="Native Objective D1 requires SAME quiescent GameEvent manager";return false;}
 bool owns=false;
 for(const auto& event:storage->events())if(event)for(const auto& item:event->objectives())if(item.get()==&objective)owns=true;
 if(const auto* pending=storage->pending_event())for(const auto& item:pending->objectives())if(item.get()==&objective)owns=true;
 if(!owns||objective.fields().type4<0||objective.fields().type4>12){e="Native Objective D1 addressed foreign/unconstructed storage";return false;}
 native_retiring_v88_=true;
 return destroy_objective_aliases_v108(borrow_game_event_objective_v75(storage,objective),e);
}
bool GameEventRuntimeV75::destroy_objective_aliases_v108(ObjectiveBorrowV75 objective,std::string& e){
 if(busy_||!objective.receiver||!objective.identity||objective.type4<0||objective.type4>12){e="Native Objective D1 requires actual quiescent constructed fields";return false;}
 const auto found=receivers_.find(objective.identity);
 if(found!=receivers_.end()){
  auto receiver=found->second;
  auto lifetime=receiver->objective_lifetime.lock();
  if(!lifetime||lifetime.owner_before(objective.receiver)||objective.receiver.owner_before(lifetime)||
     &receiver->objective.type4!=&objective.type4||&receiver->objective.completed14!=&objective.completed14||
     &receiver->objective.byte8!=&objective.byte8||&receiver->objective.owner10!=&objective.owner10||
     &receiver->objective.byte1c!=&objective.byte1c||receiver->objective.data_c!=objective.data_c){
   e="Native Objective D1 addressed reused identity or different saved backing";return false;
  }
  for(const auto& registration:receiver->registrations_v88){
   //The SAME containing Level lease protects this actual dispatcher pointer.
   //An expired owner means its dispatcher already destroyed all aliases.
   auto level=registration.level.lock();if(!level)continue;
   if(!registration.dispatcher||registration.dispatcher->identity()!=registration.dispatcher_identity||
      !registration.dispatcher->retire_receiver_alias_v88(reinterpret_cast<std::uintptr_t>(receiver.get()),e))return false;
  }
  receiver->retired_v88=true;receiver->registrations_v88.clear();receivers_.erase(found);
 }
 //Source derived D1s (47a1f0/240/290/2e0/330,47d474,47a864) only
 //install base vptrs then ObjectiveD1(47a1e4 BXLR). No field1c, Talk2fa,
 //gathering-ID or marker mutations are present at this destruction site.
 e.clear();return true;
}
bool GameEventRuntimeV75::unregister_objectives(std::string& e){
 if(native_retiring_v88_){e="Objective runtime delivery after native storage retirement began";return false;}
 if(failed_){e=failure_;return false;}if(busy_)return fail("ObjectiveList.Unregister reentered during destructive operation",e);
 if(!manager_)return fail("Required actual GameEvent194 ObjectiveLists",e);
 Busy busy(busy_);try{for(const auto& event:manager_->events()){
  if(!event)return fail("Required actual event receiver",e);
  for(const auto& objective:event->objectives())if(!objective||!register_objective(borrow_game_event_objective_v75(manager_,*objective),false,e))return fail(e,e);
 }e.clear();return true;}catch(const std::exception& ex){return fail(ex.what(),e);}catch(...){return fail("ObjectiveList.Unregister provider threw",e);}
}
}
