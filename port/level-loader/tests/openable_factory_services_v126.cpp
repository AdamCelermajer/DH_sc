#include "catalog_containers_v67.hpp"
#include "source_object_loading_v95.hpp"
#include <openable_container_source_services_v126.hpp>
#include <character_design_services.hpp>
#include <audio_world_producer_v38.hpp>
#include <vox_play3d_owner_v2.hpp>
#include "../../level-world/tests/openable_container_real_cache_fixture_v1.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
namespace {
unsigned checks{};
void check(bool value,const std::string& why){++checks;if(!value)throw std::runtime_error(why);}
struct Providers {
 std::shared_ptr<character::DebugSwitches> debug{dh2_character_debug_create(),dh2_character_debug_destroy};
 character::DebugFileServices24 files{this,[](void*,const char*,std::uintptr_t* out){*out=0;return 0;},[](void*,std::uintptr_t){return 0;}};
 std::map<std::uintptr_t,std::weak_ptr<world::CanonicalOpenableGraphV21>> graphs;
 std::vector<std::string> calls;
 unsigned updates{},pf{},sounds{},drops{},script_calls{};
 bool fail_debug{},fail_pf{},fail_drop{};
 std::uintptr_t fail_debug_id{};
 std::unique_ptr<scripts::GenericLuaScriptOwnerV13> script;
 bool debug_get(const char* key,bool& out,std::string& e,std::uintptr_t receiver=0){calls.emplace_back(key);
  if(fail_debug&&(!fail_debug_id||fail_debug_id==receiver)){e="source Debug fixture failure";return false;}std::uint32_t value{};
  if(dh2_character_debug_load(debug.get(),&files)!=1||dh2_character_debug_get(&value,debug.get(),key,&files)!=1)return false;
  out=value!=0;return true;
 }
};
}
int main(int argc,char** argv){try{
 check(argc==2,"requires actual three-chest cache directory");std::string e;
 //The genuine production catalog/C1/scene/native-body/Lua kernels run below.
 //External PF/RNG/device/Character/Arrays and filesystem responses are named
 //boundary fixtures, not a claim of an Android campaign/audio-device launch.
 auto containing=std::make_shared<int>(1);auto provider=std::make_shared<Providers>();
 auto manager=std::make_shared<world::CanonicalObjectManagerV1>(world::CanonicalObjectManagerServicesV1{});
 world::CanonicalPropertyMapV1 properties({});std::shared_ptr<loader::CanonicalLevelContextV1> level;
 check(loader::CanonicalLevelContextV1::create({"SWAMP","fixture.xml"},provider,level,e),e);
 auto roots=std::make_shared<world::GameObjectSceneRootRegistryV1>();physical::NativeWorld physics;
 const float bounds[]{-30000,-30000,30000,30000};physics.load(bounds);
 auto table=std::make_shared<world::OpenableContainerTableV1>();
 check(table->load(source_openable_records,sizeof source_openable_records,source_openable_names,sizeof source_openable_names,e),e);
 auto lua_pin=std::make_shared<int>(3);
 auto cache=std::make_shared<scripts::LuaScriptCacheOwnerV13>(scripts::LuaScriptCacheServicesV13{lua_pin,nullptr,
  [](void*,const std::string&,auto&,bool& found,std::string&){found=false;return true;}});
 dh2_script_object_services userdata{};userdata.type_name=[](void*,std::uintptr_t id,const char** type){if(id!=0x1234)return -1;*type="Character";return 0;};
 userdata.methods=[](void*,std::uintptr_t,const dh2_script_object_method** methods,std::uint32_t* count){*methods=nullptr;*count=0;return 0;};
 userdata.invoke=[](void*,const dh2_script_callback_scope*,std::uintptr_t,std::uint32_t,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t)->int{return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}; //No object method is used by this argument-projection fixture.
 scripts::LuaScriptServicesV13 lua;lua.owner=lua_pin;lua.objects=&userdata;
 provider->script=scripts::GenericLuaScriptOwnerV13::create(false,cache,lua,1024*1024,e);check(bool(provider->script),e);
 const char code[]="function OnOpen(...) opened=select('#',...);if opened>0 then opener=(...)._this end end; function OnAnimEvent(s) anim_event=s end";
 check(!dh2_script_vm_load_source_file(provider->script->vm_borrow(),code,sizeof(code)-1),"load actual Lua fixture");
 loader::ScopeV67 scope{containing,level,manager,&properties,provider};loader::CatalogContainerServicesV67 catalog;
 catalog.openable_table=table;catalog.destructible_table=std::make_shared<world::DestructibleContainerTableV16>();
 catalog.openable_services=[&](const auto&,const auto&,const auto& slot,world::CanonicalOpenableGraphServicesV21& out,std::string& error){
  check(slot&&slot->expired(),"service preparation occurs before SAME C1 slot publication");
  auto get=[slot](){auto actual=slot->lock();check(bool(actual),"same factory weak slot still live");return actual;};
  out.world=provider;out.roots=roots;out.physics=&physics;out.visual.owner=provider;
  out.visual.read_asset=[&](const auto& uri,auto& bytes,bool& found,std::string&){auto at=uri.find_last_of('/');std::ifstream f(std::string(argv[1])+"/"+uri.substr(at==std::string::npos?0:at+1),std::ios::binary);found=bool(f);if(found)bytes.assign(std::istreambuf_iterator<char>(f),{});return true;};
  out.initialization.owner=provider;out.position.owner=provider;out.decor.owner=provider;out.container.owner=provider;
  out.initialization.condition_init=[](auto,std::string&){return true;};out.initialization.check_spawn_probability=[](auto& roll,std::string&){roll=0;return true;};
  out.initialization.device_high_performance=[](bool& high,std::string&){high=true;return true;};
  out.initialization.init_pf_object=[get,provider](bool,const float*,float,std::uintptr_t id,std::string&){check(id==get()->base().identity(),"PF initializes SAME C1");provider->calls.emplace_back("PF.Init");++provider->pf;return true;};
  out.initialization.light_set_id=[](const auto&,auto& id,std::string&){id=0;return true;};
  out.initialization.update_pf_object=[get,provider](std::string& error){auto actual=get();++provider->pf;if(provider->fail_pf&&*actual->base().pointer(0x2dc)){error="source PF fixture failure";return false;}return true;};
  out.visual.update_pf=out.initialization.update_pf_object;
  out.container.spawn_roll_and_probability=[](auto& roll,auto& probability,std::string&){roll=0;probability=100;return true;};
  out.container.visual_asset=[get](std::int32_t id,std::string&){const char* uri=id==47?"data/3D/GameObjects/go_chest_swamp.bdae":id==48?"data/3D/GameObjects/go_chest_swamp_big.bdae":id==49?"data/3D/GameObjects/go_chest_swamp_rotten.bdae":nullptr;check(uri,"actual dictionary fixture row");*get()->base().string(0x290)=uri;return true;};
  out.container.meet_condition=[](bool& value,std::string&){value=true;return true;};
  out.container.precache_complete_source_v42=[get,table](std::string& error){auto actual=get();std::int32_t id{};world::OpenableContainerRowV1 row;return table->resolve(actual->fields().data_desc,id,row,error);};
  out.container.load_object_script=[](const char* script,const char*,std::string&){check(script&&!*script,"actual authored chest script is empty");return true;};
  out.container.has_script=[get](bool& present,std::string&){present=*get()->base().pointer(0x300)!=0;return true;};
  out.container.source_on_interact=[get,provider](std::string&){get();provider->calls.emplace_back("Update");++provider->updates;return true;};
  out.container.find_key=[get](auto actor,auto id,bool& found,std::int16_t& quantity,std::string&){get();check(actor==0x1234&&id==1,"SAME key lookup arguments");found=true;quantity=1;return true;};
  out.container.consume_key=[get](auto actor,auto id,auto quantity,bool& consumed,std::string&){get();check(actor==0x1234&&id==1&&quantity==1,"SAME key consumption arguments");consumed=true;return true;};
  out.container.drop_loot_table=[get,provider](auto table,auto opener,auto fixed,bool flag,std::string& error){get();check(table>=0&&opener==0x1234&&fixed==-1&&!flag,"source DropLootTable arguments");++provider->drops;if(provider->fail_drop){error="source DropLoot fixture failure";return false;}return true;};
  world::RetainedGameObjectDecorServicesV1 physical;physical.owner=provider;
  physical.debug_switch=[get,provider](const char* key,bool& value,std::string& error){return provider->debug_get(key,value,error,get()->base().identity());};
  physical.update_pf=out.initialization.update_pf_object;
  physical.peer_owner=[provider](void* address,std::uintptr_t& id,std::string& error){for(const auto& entry:provider->graphs){auto graph=entry.second.lock();if(!graph)continue;bool handled{};std::shared_ptr<void> pin;if(!graph->physical_peer_v90(address,id,pin,handled,error))return false;if(handled)return true;}error="unowned physical callback";return false;};
  physical.peer_visible80=[provider](auto id,std::uint8_t& value,std::string&){auto graph=provider->graphs.at(id).lock();check(bool(graph),"SAME peer owner remains live");value=*graph->receiver().base().byte(0x80);return true;};
  world::OpenableContainerServicesV1 source;
  source.resolve_item_name=[get](const auto& name,auto& id,std::string&){get();data::ItemTable original;original.identifiers={"other","SwampKey"};id=data::item_id(original,name);return true;};
  source.is_character=[get](auto id,bool& result,std::string&){get();result=id==0x1234;return true;};
  source.play_sound_3d=[get,provider,level](auto id,std::string& error){auto actual=get();const auto* raw=actual->base().vector3(0x160);std::array<float,3> position;std::copy_n(raw,3,position.begin());auto play=audio::audio_world_request_v38(0x500,actual->base().identity(),id,position);check(id==33&&play.position[0]==100&&play.source_integer==1&&!play.source_bool&&play.source_float0==-1&&play.source_float1==-1,"SAME raw Container Play3D arguments");++provider->sounds;
   struct Prefix {Providers& provider;loader::CanonicalLevelContextV1& level;std::string& error;} prefix{*provider,*level,error};
   sound::VoxPlay3DOwnerV2 vox({&prefix,[](void* raw,const auto& request,auto& response){auto& p=*static_cast<Prefix*>(raw);using O=sound::VoxPlay3DOperationV2;if(request.operation==O::disabled){bool value{};if(!p.provider.debug_get("IsDisablingSounds",value,p.error))return -1;response.value=value;return 0;}if(request.operation==O::current_level){response.identity=p.level.identity();response.value=p.level.constructor_fields_v3().field130;return 0;}return -1;}});return vox.play(play)==0;
  };
  source.script_call=[get,provider](const char* name,auto actor,const char* event,std::string& error){check(*get()->base().pointer(0x300)==provider->script->identity(),"SAME LuaScript300");++provider->script_calls;return world::call_container_script_source_v126(*provider->script,name,actor,event,error);};
  return world::connect_openable_source_services_v126(out,physical,source,error);
 };
 catalog.destructible_services=[](const auto&,const auto&,const auto&,auto&,auto&,std::string&){return false;};
 catalog.admit_openable=[provider](const auto&,const auto&,const auto& graph,std::string&){provider->graphs[graph->receiver().base().identity()]=graph;return true;};
 catalog.admit_destructible=[](const auto&,const auto&,const auto&,std::string&){return false;};
 catalog.teardown_openable=[](const auto& graph,std::string& error){return graph->release(error);};
 catalog.transport_unpublished=[](const auto&,bool& absent,std::string&){absent=true;return true;};
 loader::PartV67 part;std::shared_ptr<loader::ContainerCatalogOwnerV67> owner;
 check(loader::make_container_catalog_part_v67(scope,catalog,part,e,&owner),e);
 auto transport=std::make_shared<loader::CanonicalReceiverTransportV1>(properties,loader::CanonicalReceiverTransportServicesV1{provider});
 auto make=[&](const char* row){world::CanonicalClassReceiverV1 receiver;world::CanonicalSourceObjectRequestV1 request;request.source_lease=std::make_shared<int>(2);
  check(part.construct({"OpenableContainer",0x340da4},request,receiver,e),e);auto graph=owner->openable(receiver.object.identity);check(graph&&receiver.object.lease.get()==&graph->receiver(),"production catalog returns SAME alias-owned C1");
  check(transport->admit_constructed_source_v91(receiver,e),e);target_providers::Handle16 handle;const auto name=std::string(row)+"-"+std::to_string(provider->graphs.size());check(manager->add(receiver.object,name.c_str(),row,-1,false,handle,e),e);
  graph->receiver().fields().data_desc=row;graph->receiver().fields().key_name="SwampKey";auto& base=graph->receiver().base();const float position[]{100,200,250};std::copy_n(position,3,base.vector3(0x160));check(base.store_byte(0x80,1,e),e);check(receiver.init_post(e),e);check(graph->receiver().fields().key_id710==1,"real name resolver reached through production-composed services");return graph;
 };
 auto first=make("Swamp_Normal_Chest");provider->calls.clear();check(first->init_final(e),e);
 check(provider->calls==std::vector<std::string>({"PF.Init","Update","MP_NoCollisions","MP_NoPhysics"}),"InitFinal preserves PF→Update→constructor Debug→assignment Debug order");
 check(*first->receiver().base().pointer(0x2dc)!=0,"actual body assigned after InitFinal");
 auto second=make("SwampCave_Big_Chest");check(second->init_final(e),e);
 physical::NativePhysicalFilterBorrowV1 a,b;check(first->source_filter_borrow_v105(*first->receiver().base().pointer(0x2dc),a,e),e);check(second->source_filter_borrow_v105(*second->receiver().base().pointer(0x2dc),b,e),e);
 check(physics.ShouldCollide(a.body->body->GetShapeList(),b.body->body->GetShapeList()),"real PODecor filter traverses composed peer callbacks");
 check(second->receiver().base().store_byte(0x80,0,e),e);check(!physics.ShouldCollide(a.body->body->GetShapeList(),b.body->body->GetShapeList()),"real peer visible80 changes filtering");
 bool unlocked{};check(first->receiver().receiver().try_unlock(0x1234,unlocked,e)&&unlocked,"first locked interaction reaches Character/inventory leaves");
 *first->receiver().base().pointer(0x300)=provider->script->identity();level->constructor_borrow_v3().fields->field130=17;
 check(first->receiver().receiver().interact_base(0x1234,e),e);check(provider->sounds==1,"first interaction reaches authentic Vox loading prefix");
 check(first->receiver().receiver().animation_event("opened",e),e);check(first->receiver().receiver().animation_event("fixture_event",e),e);
 dh2_script_value value{};check(!dh2_script_vm_get_global(provider->script->vm_borrow(),"opened",&value)&&value.number==1,"OnOpen receives one actual source UserData argument");
 check(!dh2_script_vm_get_global(provider->script->vm_borrow(),"anim_event",&value)&&value.type==DH2_SCRIPT_STRING&&std::string(value.text)=="fixture_event","OnAnimEvent receives source event string");
 check(!dh2_script_vm_get_global(provider->script->vm_borrow(),"opener",&value)&&value.identity==0x1234,"OnOpen source UserData preserves the exact opener");
 check(world::call_container_script_source_v126(*provider->script,"OnOpen",0,nullptr,e),e);check(!dh2_script_vm_get_global(provider->script->vm_borrow(),"opened",&value)&&value.number==0,"NULL opener adds zero arguments");
 provider->fail_drop=true;const auto calls=provider->script_calls;check(!first->receiver().receiver().animation_event("opened",e)&&e=="source DropLoot fixture failure"&&provider->script_calls==calls,"loot failure stops before Lua suffix");provider->fail_drop=false;
 auto failed=make("Swamp2_Rotten_Chest");provider->fail_debug=true;provider->fail_debug_id=failed->receiver().base().identity();
 loader::SourceLoadingInputsV43 loading;loading.manager=manager;loader::SourceObjectLoadingLeavesV95 leaves;leaves.owner=provider;
 leaves.current=[](std::string&){return true;};leaves.test_enable=[](const auto&,bool,std::string&){return true;};
 check(loader::SourceObjectLoadingV95::bind(manager,transport,{},leaves,loading,e),e);
 check(loading.external.stage_body[17](e)==loader::LifecycleStepV36::failed&&e=="source Debug fixture failure","actual Stage17 dispatcher reaches composed Debug failure");
 const auto updates=provider->updates;const auto pf=provider->pf;const auto debug_calls=provider->calls.size();
 check(loading.external.stage_body[17](e)==loader::LifecycleStepV36::failed&&e=="source Debug fixture failure"&&provider->updates==updates&&provider->pf==pf&&provider->calls.size()==debug_calls,"latched Stage17 failure never replays Update/PF/Debug");
 check(!*failed->receiver().base().pointer(0x2dc),"failed new receiver remains unassigned");provider->fail_debug=false;
 auto failed_pf=make("Swamp2_Rotten_Chest");provider->fail_pf=true;check(!failed_pf->init_final(e)&&e=="source PF fixture failure"&&*failed_pf->receiver().base().pointer(0x2dc),"PF failure retains already assigned native body");provider->fail_pf=false;
 check(owner->teardown(failed_pf->receiver().base().identity(),e),e);auto& base=failed->receiver().base();*base.pointer(0x300)=0;check(owner->teardown(base.identity(),e),e);check(owner->teardown(first->receiver().base().identity(),e),e);check(owner->teardown(second->receiver().base().identity(),e),e);
 const char ordinary[]="function OnOpen() error('ordinary Lua error') end";
 check(!dh2_script_vm_load_source_file(provider->script->vm_borrow(),ordinary,sizeof(ordinary)-1),"install ordinary Lua result branch");
 check(world::call_container_script_source_v126(*provider->script,"OnOpen",0,nullptr,e)&&e.empty(),"Container ignores ordinary protected-call failure");
 const char required[]="function OnOpen() Rand(0,1) end";
 check(!dh2_script_vm_load_source_file(provider->script->vm_borrow(),required,sizeof(required)-1),"install missing native service branch");
 check(!world::call_container_script_source_v126(*provider->script,"OnOpen",0,nullptr,e)&&e.find("Rand")!=std::string::npos,"Container preserves required Lua native-service failure");
 check(provider->script->close_source_v88(e),e);
 check(roots->roots().empty(),"all production catalog visual/body prefixes release");
 std::cout<<"PASS production ContainerCatalog C1→shared Openable connector→InitFinal/Box2D peer filters→key/audio/Lua first interaction; explicit external boundaries; checks="<<checks<<"\n";
}catch(const std::exception& x){std::cerr<<x.what()<<'\n';return 1;}}
