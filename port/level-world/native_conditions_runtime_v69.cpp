#include "native_conditions_v69.hpp"
#include <exception>
namespace dh2::world {
struct NativeConditionRuntimeV69::List {
 struct Condition {
  const NativeConditionStubV69* stub4{}; //Condition C1 478674 actual NULL
  std::int32_t selected_factory{};
  std::optional<std::int32_t> quest_comparison8;
  explicit Condition(std::int32_t op):selected_factory(op){
   if(op==0)quest_comparison8=0;
   else if(op==1)quest_comparison8=1;
   else if(op==2)quest_comparison8=2;
  }
 };
 //ConditionList C1 4786f0 writes all three fields zero.
 std::int32_t count0{};const NativeConditionStubV69* data8{};
 std::shared_ptr<const void> data_lease;
 std::vector<std::unique_ptr<Condition>> receivers4;
 //AssignPyData does not free a previous pointer4. Retain these orphaned source
 //allocations throughout this arena; modern arena teardown reclaims metadata.
 bool evaluating{};
};
namespace {
bool required(std::string& e,const char* leaf){if(e.empty())e=std::string("Required actual original condition service: ")+leaf;return false;}
bool same(const std::shared_ptr<void>& a,const std::shared_ptr<void>& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);}
}
bool NativeConditionRuntimeV69::create(std::shared_ptr<const NativeConditionTableV69> table,NativeConditionServicesV69 services,
 std::shared_ptr<NativeConditionRuntimeV69>& out,std::string& e){
 if(out||!table||!table->ready()){e="Condition runtime requires one actual immutable decoded Arrays table";return false;}
 auto runtime=std::make_shared<NativeConditionRuntimeV69>();runtime->tables_=std::move(table);runtime->services_=std::move(services);out=std::move(runtime);e.clear();return true;
}
bool NativeConditionRuntimeV69::bind_services(NativeConditionServicesV69 services,std::string& e){
 if(!services.transport||(services_.transport&&!same(services_.transport,services.transport))){e="Condition provider adoption requires SAME native transport owner";return false;}
 for(const auto& pair:lists_)if(pair.second->evaluating){e="Condition callback transport cannot change during actual evaluation";return false;}
 services_=std::move(services);e.clear();return true;
}
ConditionDataInitServicesV3 NativeConditionRuntimeV69::condition_data_services(){
 ConditionDataInitServicesV3 services;auto self=shared_from_this();const std::weak_ptr<NativeConditionRuntimeV69> weak=self;
 services.owner=self;services.conditions=&tables_->condition_data_rows();
 services.construct_condition=[weak](auto& out,auto& e){auto self=weak.lock();if(!self)return required(e,"live ConditionList arena");return self->construct(out,e);};
 services.initialize_condition=[weak](auto id,auto stubs,auto count,auto& e){auto self=weak.lock();if(!self)return required(e,"live ConditionList arena");return self->assign_py_data(id,stubs,count,e);};
 services.destroy_condition=[weak](auto id,auto& e){auto self=weak.lock();if(!self)return required(e,"live ConditionList arena");return self->destroy(id,e);};
 return services;
}
bool NativeConditionRuntimeV69::construct(std::uintptr_t& out,std::string& e){
 try{auto list=std::make_shared<List>();out=reinterpret_cast<std::uintptr_t>(list.get());
  if(!lists_.emplace(out,std::move(list)).second){e="Duplicate native ConditionList allocation identity";return false;}
  e.clear();return true;
 }catch(const std::exception& exception){e=std::string("ConditionList C1 allocation failed: ")+exception.what();return false;}
}
bool NativeConditionRuntimeV69::assign_py_data(std::uintptr_t id,std::uintptr_t stub_argument,std::uintptr_t count_argument,std::string& e){
 const auto* row=tables_->argument_receiver(stub_argument,count_argument);
 if(!row){e="AssignPyData requires SAME decoded Arrays condition row";return false;}
 return assign_native(id,row->stubs8.get(),row->count4,tables_,e);
}
bool NativeConditionRuntimeV69::assign_authored_py_data_v76(std::uintptr_t id,std::shared_ptr<const void> lease,
 const NativeConditionStubV69* stubs,std::int32_t count,std::string& e){
 if(!lease||count<0||(count&&!stubs)){e="Required actual retained v2Quest inline condition row";return false;}
 return assign_native(id,stubs,count,std::move(lease),e);
}
bool NativeConditionRuntimeV69::assign_native(std::uintptr_t id,const NativeConditionStubV69* stubs,
 std::int32_t count,std::shared_ptr<const void> lease,std::string& e){
 auto found=lists_.find(id);
 if(found==lists_.end()||found->second->evaluating){e="AssignPyData requires SAME allocated inactive ConditionList";return false;}
 auto& list=*found->second;
 //Leaked prior Condition receivers still point at their prior immutable row.
 //Keep its actual source lease for the arena lifetime as well.
 if(list.data_lease)source_orphans_.push_back(list.data_lease);
 list.data_lease=std::move(lease);
 list.data8=stubs;list.count0=count; //478920,478928 before allocation/factories
 if(list.count0<=0){e.clear();return true;} //does NOT clear existing pointer4
 try{
  std::vector<std::unique_ptr<List::Condition>> array(static_cast<std::size_t>(list.count0));
  if(!list.receivers4.empty())source_orphans_.push_back(std::make_shared<decltype(list.receivers4)>(std::move(list.receivers4)));
  list.receivers4=std::move(array);
  for(std::int32_t i=0;i<list.count0;++i){const auto& stub=list.data8[i];
   if(stub.op4<0||stub.op4>=7){e="Source condition factory index outside captured table969258";return false;}
   auto receiver=std::make_unique<List::Condition>(stub.op4);
   list.receivers4[static_cast<std::size_t>(i)]=std::move(receiver); //publish returned ctor before stub4 store
   list.receivers4[static_cast<std::size_t>(i)]->stub4=&stub;
  }e.clear();return true;
 }catch(const std::exception& exception){e=std::string("ConditionList.AssignPyData retained failed allocation prefix: ")+exception.what();return false;}
}
bool NativeConditionRuntimeV69::destroy(std::uintptr_t id,std::string& e){
 auto found=lists_.find(id);if(found==lists_.end()||found->second->evaluating){e="ConditionList D1 requires SAME inactive allocated receiver";return false;}
 auto& list=*found->second;
 //478eac walks current count, selected deleting D0, zeroes each slot, then
 //frees pointer4 without clearing count0/data8. No query/callback is in D0.
 for(std::int32_t i=0;i<list.count0;++i){
  if(static_cast<std::size_t>(i)>=list.receivers4.size()){e="ConditionList destructor reached unallocated slot after failed AssignPyData";return false;}
  list.receivers4[static_cast<std::size_t>(i)].reset();
 }
 if(list.count0<=0&&!list.receivers4.empty())source_orphans_.push_back(std::make_shared<decltype(list.receivers4)>(std::move(list.receivers4)));
 list.receivers4.clear();lists_.erase(found);e.clear();return true;
}
bool NativeConditionRuntimeV69::evaluate(std::uintptr_t id,bool& result,std::string& e){
 auto found=lists_.find(id);if(found==lists_.end()||found->second->evaluating){e="ConditionList Eval requires SAME live nonrecursive receiver";return false;}
 const auto pin=found->second;auto& list=*pin;
 struct Scope {bool& active;~Scope(){active=false;}} scope{list.evaluating};list.evaluating=true;
 auto local=[&](NativeConditionPlayerV69& player){
  if(!services_.transport||!services_.local_player||!services_.local_player(0,true,player,e))return required(e,"PlayerManager.GetLocalPlayer(0,true)");
  if(!player.receiver||!player.player_info||!player.character660)return required(e,"SAME actual PlayerInfo.character660 cell");return true;
 };
 auto current=[&](NativeConditionLevelV69& level){
  if(!services_.transport||!services_.current_level||!services_.current_level(level,e))return required(e,"Application.GetCurrentLevel31f594");
  if(!level.identity){if(level.receiver||level.identifier3c||level.events194){e="NULL current Level carries foreign condition fields";return false;}return true;}
  if(!level.receiver)return required(e,"SAME published current Level lease");return true;
 };
 try{
  for(std::int32_t i=0;i<list.count0;++i){
   if(static_cast<std::size_t>(i)>=list.receivers4.size()||!list.receivers4[static_cast<std::size_t>(i)]||!list.receivers4[static_cast<std::size_t>(i)]->stub4)return required(e,"actual constructed condition/stub4 at ordered Eval index");
   const auto& condition=*list.receivers4[static_cast<std::size_t>(i)];const auto& stub=*condition.stub4;bool truth{};
   switch(condition.selected_factory){
    case 0:case 1:case 2:{
     NativeConditionPlayerV69 player;if(!local(player))return false;
     if(!*player.character660){truth=false;break;}
     NativeConditionStateV69 state;
     if(!services_.quest_state||!services_.quest_state(*player.character660,stub.argument8,-1,state,e))return required(e,"Character.SG_GetQuestByID3bc2f8(quest,-1)");
     if(!state.state0){if(state.receiver){e="NULL quest state carries an unrelated receiver lease";return false;}truth=false;break;}
     if(!state.receiver||!condition.quest_comparison8)return required(e,"SAME quest state0/native comparison8");
     if(*condition.quest_comparison8==0)truth=stub.argument_c==*state.state0;
     else if(*condition.quest_comparison8==1)truth=stub.argument_c>*state.state0;
     else if(*condition.quest_comparison8==2)truth=stub.argument_c<*state.state0;
     else truth=false;
     break;
    }
    case 3:{NativeConditionLevelV69 level;if(!current(level))return false;
     //478d94 dereferences the current Level; genuine NULL is source-invalid,
     //not a fabricated false comparison or an unpublished C1 fallback.
     if(!level.identity||!level.identifier3c)return required(e,"nonnull current Level.identifier3c for IsPlayerInLevel");
     truth=stub.argument8==*level.identifier3c;break;
    }
    case 4:case 5:
     //Captured selected Eval478638/478640 are literal TRUE in THIS game ELF.
     //They do not query/save/infer a level or player state.
     truth=true;break;
    case 6:{NativeConditionLevelV69 level;if(!current(level))return false;
     if(!level.identity){truth=false;break;}
     if(!level.events194)return required(e,"SAME actual Level.GameEvents194 pointer cell");
     if(!*level.events194){
      std::int32_t mode{};if(!services_.assertion_mode||!services_.assertion_mode(mode,e))return required(e,"actual assertion mode on missing GameEvents194");
      if(mode==2){e="Original IsEventInState assertion mode2 would write through NULL GameEvents194";return false;}
      if(mode==1){constexpr const char* source_file="..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Progression\\Condition.cpp";
       if(!services_.assertion||!services_.assertion(source_file,0x93,"gEvtMgr != 0",e))return required(e,"original Condition.cpp assertion callback");}
      truth=false;break;
     }
     NativeConditionStateV69 state;
     if(!services_.event_state||!services_.event_state(*level.events194,stub.argument8,state,e))return required(e,"SAME GameEvents.GetEventByID4796f0");
     if(!state.receiver||!state.state0)return required(e,"nonnull actual event state0 source dereference");
     truth=stub.argument_c==*state.state0;break;
    }
    default:return required(e,"captured native selected Condition factory");
   }
   if(!truth){result=false;e.clear();return true;} //47874c short-circuit false
  }
  result=true;e.clear();return true; //478754 count<=0 or all true
 }catch(const std::exception& exception){e=std::string("Actual condition provider failed: ")+exception.what();return false;}
}
bool NativeConditionRuntimeV69::condition_data_is_true(CanonicalGameObjectBaseOwnerV1& base,std::uint32_t offset,bool& result,std::string& e){
 if(offset!=0x8c&&offset!=0xb0){e="Required SAME ConditionData subobject offset";return false;}
 const auto* tested=base.byte(offset+0x20);const auto* compiled=base.pointer(offset+0x1c);
 if(!tested||!compiled)return required(e,"constructor-backed ConditionData tested/compiled cells");
 if(*tested||!*compiled){result=true;e.clear();return true;}
 return evaluate(*compiled,result,e);
}
bool NativeConditionRuntimeV69::condition_data_is_true(const NativeConditionDataBorrowV69& fields,bool& result,std::string& e){
 if(!fields.receiver||!fields.tested20||!fields.compiled1c)return required(e,"SAME actual ConditionData receiver/+1c/+20 fields");
 if(*fields.tested20||!*fields.compiled1c){result=true;e.clear();return true;}
 return evaluate(*fields.compiled1c,result,e);
}
}
