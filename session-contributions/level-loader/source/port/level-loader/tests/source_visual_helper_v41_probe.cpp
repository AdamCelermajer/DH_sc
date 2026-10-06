#include "../generic_actor_draw_submission_v38.hpp"
#include "../render_eligibility_v40.hpp"
#include "canonical_module_graph_v3.hpp"
#include "canonical_openable_graph_v21.hpp"
#include <iomanip>
#include <cstdlib>
#include "../module_draw_frame_v1.hpp"
#include "../retained_level_module_graph_v1.hpp"
#include "../canonical_level_class_dispatch_v1.hpp"
#include "openable_container_data_connection_v11.hpp"
#include "retained_character_position_owner_v7.hpp"
#include "../device_pipeline_borrow_v1.hpp"
#include "canonical_dummy_owner_v14.hpp"
#include "canonical_destructible_container_v16.hpp"
#include "canonical_spawn_point_v15.hpp"
#include "../canonical_auxiliary_families_v16.hpp"
#include "module_room_zone_spawn_v3.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include "object_enable_condition_v2.hpp"
#include "navigation_producers.hpp"
#include "module_pf_actor_connection_v3.hpp"
#include "light_set_name_owner_v3.hpp"
#include "canonical_receiver_transport_v1.hpp"
#include "canonical_cached_file_v1.hpp"
#include "canonical_module_files_v1.hpp"
#include "canonical_level_context_v1.hpp"
#include "level_constructor_bindings_v4.hpp"
#include "level_config_publication_v2.hpp"
#include <stdexcept>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iterator>
#include <iostream>
using namespace dh2;using namespace dh2::world;using namespace dh2::loader;
static void require(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
static assets::ZipAssetPackV1 pack(const char* path){
 auto f=std::make_shared<std::ifstream>(path,std::ios::binary|std::ios::ate);require(bool(*f),"cache unavailable");
 assets::ZipBackingV1 b;b.owner=f;b.bytes=std::uint64_t(f->tellg());b.read=[f](std::uint64_t at,void* out,std::size_t n,std::string& e){f->clear();f->seekg(std::streamoff(at));f->read(static_cast<char*>(out),std::streamsize(n));if(!*f){e="cache read failure";return false;}return true;};
 assets::ZipAssetPackV1 z;std::string e;require(z.mount(std::move(b),"com.gameloft.android.GAND.GloftD2SS/files/",e),e);return z;
}
// Standalone original-cache GameDesign table setup for this source probe.
// The application must borrow its existing initialized Arrays/GameDesign.
struct CharacterTables {
 std::vector<std::vector<std::uint8_t>> raw;
 character::CharacterGameDesign design;data::Dictionary models;data::LootTablesV2 loot;std::vector<data::Bytes> constant_views;
 explicit CharacterTables(assets::ZipAssetPackV1& archive,const char* design_fixture){
  raw.reserve(32);std::string e;
  auto bytes=[&](const std::string& name){raw.emplace_back();bool found{};require(archive.read("data/pydata/"+name,found,raw.back(),e)&&found,"Required original Character table "+name+": "+e);auto& b=raw.back();return data::Bytes{b.data(),b.size()};};
  character::GameDesignInputs256 input{};character::GameDesignTableInput48* tables[]{&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};const char* prefixes[]{"character_properties","character_classes","ai","ai_factions","levels"};
  for(unsigned i=0;i<5;++i)*tables[i]={bytes(std::string(prefixes[i])+"_pyarray.bin"),bytes(std::string(prefixes[i])+"_pyarraynames.bin"),bytes(std::string(prefixes[i])+"_pystructnames.bin")};
  // Original full GameDesign constant names from the independently captured
  // source fixture. Every stream is reread and compared with the real ZIP.
  std::ifstream fixture(design_fixture,std::ios::binary);require(bool(fixture),"Original design fixture unavailable");
  auto word=[&](){std::uint32_t value{};fixture.read(reinterpret_cast<char*>(&value),4);require(bool(fixture),"Short original design word");return value;};
  auto blob=[&](){std::vector<std::uint8_t> value(word());fixture.read(reinterpret_cast<char*>(value.data()),value.size());require(bool(fixture),"Short original design blob");return value;};
  require(word()==0x314f4447,"Original design fixture magic differs");
  for(unsigned i=0;i<15;++i){auto original=blob();require(original==raw[i],"Original design table differs from source ZIP");}
  const auto count=word();constant_views.reserve(count);
  for(unsigned i=0;i<count;++i){auto name_bytes=blob(),expected=blob();std::string name(name_bytes.begin(),name_bytes.end());auto actual=bytes(name);require(actual.size==expected.size()&&std::equal(expected.begin(),expected.end(),actual.data),"Original constant stream differs: "+name);constant_views.push_back(actual);}
  input.constants=constant_views.data();input.constant_count=constant_views.size();
  require(design.initialize(input,e),e);
  require(data::load_dictionary(bytes("character_models_dictionary_pyarraynames.bin"),bytes("character_models_dictionary_pyarray.bin"),models,e),e);
  require(loot.load(bytes("loot_table_pyarray.bin"),bytes("loot_table_pyarraynames.bin"),bytes("loot_table_pystructnames.bin"),e),e);
 }
};
// Source-probe extraction of frozen original group5 slices. Production
// borrows the engine-owned initialized Arrays/cache, not these fixture offsets.
static std::shared_ptr<OpenableContainerDataConnectionV11> chest_tables(assets::ZipAssetPackV1& archive){
 std::string e;auto read=[&](const char* name){std::vector<std::uint8_t> b;bool found{};require(archive.read(std::string("data/pydata/")+name,found,b,e)&&found,e);return b;};
 auto records=read("game_objects_pyarray.bin"),names=read("game_objects_pyarraynames.bin"),dict=read("game_objects_dictionary_pyarray.bin"),dictnames=read("game_objects_dictionary_pyarraynames.bin");
 require(records.size()>=5007&&names.size()>=3614,"Short original group5 source fixture");
 auto data=std::make_shared<OpenableContainerDataConnectionV11>();require(data->load(records.data()+3031,1976,names.data()+1749,1865,dict.data(),dict.size(),dictnames.data(),dictnames.size(),e),e);
 require(data->container_count()==68&&data->dictionary_count()==85,"Original chest/dictionary group sizes differ");return data;
}
// Platform/local-player/network/Handle transports below are explicit fixtures.
// Constructors, defaults, conditions, visual, PF room, Spawn and Zone are real.
struct NativeCtorFilesFixtureV24 {
 assets::ZipAssetPackV1 archive;std::string private_directory;unsigned script_reads{},save_reads{};
 explicit NativeCtorFilesFixtureV24(assets::ZipAssetPackV1 source,const char* directory):archive(std::move(source)),private_directory(directory){}
 static bool script(void* context,const std::string& name,std::vector<std::uint8_t>& bytes,bool& found,std::string& e){auto& files=*static_cast<NativeCtorFilesFixtureV24*>(context);++files.script_reads;return files.archive.read(name,found,bytes,e);}
 static bool save(void* context,const std::string& name,bool& found,std::vector<std::uint8_t>& bytes,std::string&){auto& files=*static_cast<NativeCtorFilesFixtureV24*>(context);++files.save_reads;std::ifstream input(files.private_directory+"/"+name,std::ios::binary);found=bool(input);if(found)bytes={std::istreambuf_iterator<char>(input),{}};return true;}
};
struct Fixture {
 std::shared_ptr<void> pin;CanonicalObjectManagerV1 manager;
 Fixture(std::shared_ptr<void> p):pin(p),manager({this,nullptr,nullptr,nullptr,nullptr,network,nullptr}){}
 static bool network(void*,CanonicalObjectBorrowV1&,std::string&){return true;}
 static bool resolve(void* p,target_providers::Handle16& h,bool,const CanonicalObjectBorrowV1*& out,std::string&){out=static_cast<Fixture*>(p)->manager.object(h.key);if(out)h.cached=out->identity;return true;}
 static bool profile(void*,bool& character,bool& profile,std::uint8_t& flag,std::string&){character=true;profile=false;flag=0;return true;}
 static bool condition(void*,const CanonicalObjectBorrowV1& o,bool mark,std::string& e){auto* b=static_cast<CanonicalGameObjectBaseOwnerV1*>(o.context);bool enabled;return object_test_enable_condition_v2({b->byte(0x8a),b->integer(0xec),b->byte(0xf1),b->pointer(0xa8),b->byte(0xac)},{nullptr,profile,nullptr,nullptr,nullptr},mark,enabled,e);}
};
int main(int argc,char** argv){
#ifdef DH2_PREVIEW_ENTITY_EXPORT_V30
 if(argc!=5)return 2;
#else
 if(argc!=4)return 2;
#endif
try{
 auto archive=pack(argv[1]);CharacterTables character_tables(archive,argv[2]);auto chest_data=chest_tables(archive);auto application=std::make_shared<ApplicationSpawnRandomOwnerV4>();std::string e;bool found=false;auto bytes=std::make_shared<std::vector<std::uint8_t>>();
 require(archive.read("data/3D/Modules/Swamp/swamp.bdae",found,*bytes,e)&&found,e);
 std::shared_ptr<CanonicalLevelContextV1> level;require(CanonicalLevelContextV1::create({"SWAMP","data/scene/001_swamp.mlx"},bytes,level,e),e);
 // Online/Handle are explicit standalone transport fixtures; RNG and SAME
 // canonical base state are actual. Required delete/visibility paths stay absent.
 auto spawn_for=[application](CanonicalGameObjectBaseOwnerV1& base,std::int32_t& roll,std::string& error){CanonicalSpawnApplicationServicesV4 services;services.application_lease=application;services.random=application.get();services.online_byte5=[](bool& value,std::string&){value=false;return true;};services.handle_as_player=[](bool& value,std::string&){value=false;return true;};std::int32_t probability{};return canonical_check_spawn_probability_v4(base,services,roll,probability,error);};
   ModuleRuntimeGlobalsV1 globals;globals.next_module_id=99;std::uint32_t source_debug_loads=0;
  auto actual_design=std::make_shared<character::CharacterGameDesign::Borrow>(character_tables.design.borrow());
  auto native_files=std::make_shared<NativeCtorFilesFixtureV24>(archive,argv[3]);
  auto lua_cache=std::make_shared<scripts::LuaScriptCacheOwnerV13>(scripts::LuaScriptCacheServicesV13{native_files,native_files.get(),NativeCtorFilesFixtureV24::script});
  LevelConstructorApplicationV4 application_services;application_services.owner=actual_design;application_services.debug_level_load_count=&source_debug_loads;application_services.module_id_global=&globals.next_module_id;application_services.levels=actual_design->levels();application_services.lua_cache=lua_cache;application_services.lua.owner=actual_design;application_services.lua.design=*actual_design->design();application_services.private_vm_limit=16u*1024u*1024u;
  application_services.saves.files.context=native_files.get();application_services.saves.files.read_file=NativeCtorFilesFixtureV24::save;application_services.saves.files.storage_lease=native_files;
  application_services.online_byte5=[](std::uint8_t& value,std::string&){value=0;return true;}; // Explicit offline transport fixture.
  auto native_ctor=LevelConstructorBindingsV4::create(std::move(application_services),e);require(bool(native_ctor),e);
  require(level->construct_source_v3({"worlds/001_swamp.mlx",0,7,1,0,1,0,-1,0},native_ctor->services(),e),e);
  require(level->constructor_owner_v3()->phase()==LevelConstructorPhaseV3::complete&&source_debug_loads==1&&globals.next_module_id==0&&level->source_word150()==0,"whole SAME Level source constructor fields differ");
  auto actual_script=native_ctor->script();auto actual_save=native_ctor->save();require(actual_script&&actual_save&&actual_script.get()==level->constructor_fields_v3().script44.get()&&actual_save.get()==level->constructor_fields_v3().save_ec.get(),"Level C1 duplicated script/save owners");
  require(actual_script->loaded_files().size()==2&&native_files->script_reads==2&&lua_cache->cached_count()==2,"original combat/death scripts did not load through real ZIP");
  require(actual_save->owner().ready()&&actual_save->owner().fields().level8==reinterpret_cast<const void*>(level->identity())&&native_files->save_reads==2&&!actual_save->cache()->has_cached_file(),"actual native Save C1/filesystem miss differs");
  // C1 is now real. Complete source Level Init/GSLevel publication remain open;
  // the retained map API below still exercises explicit preparation stages.
auto level_fields=level->config_fields();CanonicalLevelModuleBindingsV2* owned_bindings=nullptr;
 // Explicit empty name-vector/provider fixture; authored sound names remain
 // intact and publish -1. Complete Arrays/Sound/GSLevel initialization is open.
 std::vector<std::string> sound_names;unsigned config_queries=0;
 auto roots=std::make_shared<GameObjectSceneRootRegistryV1>();auto floors=std::make_shared<floors::World>();auto map=std::make_shared<SceneManagerMapOwnerV2>(roots);ModulePFDebugV3 debug;debug.owner=bytes;debug.query=[](const char* key,bool& v,std::string&){assert(std::string(key)=="isTracingNavMeshLoadTime");v=false;return true;};auto rooms=std::make_shared<ModulePFRoomsV3>(floors,map,debug);
 unsigned template_assertions=0;
 // Explicit diagnostic-delivery fixture. Production callers must supply their
 // real Debug/assertion policy; this does not invent templates or descriptors.
 CanonicalPropertySourceServicesV1 property_sources{&template_assertions,&canonical_vec3_origin_v1(),[](void* p,std::string&){++*static_cast<unsigned*>(p);return true;}};
 property_sources.constant_context=const_cast<dh2_script_design_bindings*>(actual_design->design());
 property_sources.constant_integer=[](void* opaque,const char* group,const char* name,std::int32_t& value,std::string& error){auto* design=static_cast<dh2_script_design_bindings*>(opaque);if(!design||!design->lookup||design->lookup(design->context,0,group,name,&value)){error="Required actual initialized GameDesign constant lookup";return false;}return true;};
 CanonicalPropertyMapV1 properties(property_sources);Fixture fixture(bytes);unsigned zone_pf=0;
 CanonicalRoomZoneFactoryV3 zone_factory({bytes,[&](const std::shared_ptr<CanonicalRoomZoneRecordV3>& r,RoomZoneServicesV3& s,std::string&){std::weak_ptr<CanonicalRoomZoneRecordV3> weak=r;s.game_object.owner=bytes;s.game_object.check_spawn_probability=[weak,spawn_for](std::int32_t& roll,std::string& e){auto r=weak.lock();return r&&spawn_for(r->receiver->base(),roll,e);};s.game_object.condition_init=[weak](std::uint32_t offset,std::string& e){auto r=weak.lock();return r&&condition_data_init_v3(r->receiver->base(),offset,{},e);};s.game_object.set_position=[weak](const float* p,bool dest,std::string& e){auto r=weak.lock();return r&&game_object_set_position_v2(r->receiver->base(),p,dest,{},e);};s.game_object.device_high_performance=[](bool& high,std::string&){high=true;return true;};s.game_object.load_visual=[weak](std::string& e){auto r=weak.lock();GameObjectVisualAssetOwnerV1 asset(r->receiver->base(),{});return asset.load_visual(e);};s.game_object.update_pf_object=[&,weak](std::string&){auto r=weak.lock();navigation::ProducerRequest q{};q.object=&r->runtime.object;++zone_pf;return dh2_nav_update_game_object(&q)==0;};return true;}});
 CanonicalSpawnServicesV1 spawn;spawn.context=&fixture;spawn.resolve=Fixture::resolve;spawn.test_enable_condition=Fixture::condition;ModuleRoomZoneSpawnV3 zones(fixture.manager,properties,zone_factory,spawn);
 std::uint32_t counter=0;unsigned pf_calls=0;navigation::ObstacleEntry obstacle_entries[128]{};std::uint32_t obstacle_floors[64]{};navigation::ObstacleRegistry obstacles{obstacle_entries,0,128,obstacle_floors,0,64};LightSetNameOwnerV3 lights;assert(lights.get_id("SceneLight")==1&&lights.get_id("absent")==0);CanonicalModuleGraphServicesV3 services;services.candidate=bytes;services.rooms=rooms;services.visual.owner=bytes;services.visual.roots=roots;services.visual.root_update_counter=&counter;services.visual.driver_type=[](std::int32_t& d,std::string&){d=8;return true;};services.visual.modular_meshes=[](std::vector<std::uintptr_t>& meshes,std::string&){meshes.clear();return true;};services.read_asset=[bytes](const std::string& name,std::shared_ptr<const std::vector<std::uint8_t>>& b,bool& found,std::string&){assert(name=="data/3D/Modules/Swamp/swamp.bdae");b=bytes;found=true;return true;};services.update_pf=[&](CanonicalGameObjectBaseOwnerV1& b,std::string& e){assert(b.identity());++pf_calls;ModulePFActorConnectionV3 connection(b,bytes,&floors->collision_world,&obstacles);return connection.update(e);};services.spawn_zone=[&](const std::string& name,target_providers::Handle16& h,std::uint32_t& type,std::string& e){return zones.spawn(name,h,type,e);};services.zone_init=[&](target_providers::Handle16& h,const std::array<float,6>& box,std::uintptr_t module,std::string& e){return zones.init(h,box,module,e);};auto graph=std::make_shared<CanonicalModuleGraphV3>(services);std::vector<std::shared_ptr<CanonicalModuleRecordV2>> modules;
CanonicalLevelModuleConstructionV2 construction;construction.candidate=bytes;construction.module_globals=&globals;construction.level_config.owner=bytes;
construction.level_config.debug_switch=[](const char* key,bool& value,std::string&){assert(std::string(key)=="isTracingLevel");value=false;return true;};
construction.level_config.set_level_config=[&](std::uintptr_t id,std::string& e){++config_queries;LevelConfigPublicationBorrowV2 b{level_fields.level_owner,level_fields.config38,level_fields.music11c,level_fields.safezone120,level_fields.ambient124,bytes,&sound_names,[&](std::uintptr_t identity){return owned_bindings->level_config(identity);}};return level_config_publication_v2(b,id,e);};construction.module_services=[&](const CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalModuleRecordV2>& record,GameObjectInitializationServicesV1& init,ModuleInitServicesV1& module,std::string& e){init.owner=bytes;std::weak_ptr<CanonicalModuleRecordV2> spawn_record=record;init.check_spawn_probability=[spawn_record,spawn_for](std::int32_t& roll,std::string& e){auto r=spawn_record.lock();return r&&spawn_for(r->receiver->base(),roll,e);};init.device_high_performance=[](bool& high,std::string&){high=true;return true;};std::weak_ptr<CanonicalModuleRecordV2> weak=record;init.init_pf_object=[&,weak](bool flag,const float* p,float radius,std::uintptr_t identity,std::string& e){auto r=weak.lock();ModulePFActorConnectionV3 pf(r->receiver->base(),bytes,&floors->collision_world,&obstacles);return pf.init(flag,p,radius,identity,e);};init.light_set_id=[&](const std::string& name,std::int32_t& id,std::string&){id=lights.get_id(name);return true;};return graph->bind(record,init,module,e);};CanonicalLevelModuleBindingsV2 bindings(properties,{},construction);owned_bindings=&bindings;
CanonicalCharacterFamilyServicesV4 character_services;character_services.world=level;character_services.design=&character_tables.design;character_services.models=&character_tables.models;character_services.loot_tables=&character_tables.loot;character_services.loot_random=&application->channel(0);
character_services.set_position=[level](CanonicalCharacterRecordV4& record,const std::array<float,3>& p,bool destination,std::string& e){character::RetainedCharacterPositionBackendsV7 backends;backends.world=level;character::RetainedCharacterPositionOwnerV7 source(*record.actor,std::move(backends));return source.set_position(p.data(),destination,e);};
// Later Character InitPost providers remain absent. The
// factory must stop at its actual offset stage instead of writing a fake pose.
auto character_factory=std::make_shared<CanonicalCharacterFamilyFactoryV4>(character_services);
CanonicalLevelClassDispatchInputsV1 class_input;class_input.providers_owner=level;class_input.modules=&bindings;class_input.characters=character_factory;class_input.application_owner=application;class_input.random=application.get();
auto chest_slots=std::make_shared<std::vector<std::shared_ptr<std::weak_ptr<CanonicalOpenableGraphV21>>>>();
class_input.openable_v21_services_with_receiver=[level,roots,chest_slots,chest_data,archive](const CanonicalSourceObjectRequestV1&,const auto& slot,CanonicalOpenableGraphServicesV21& s,std::string&){require(slot&&slot->expired(),"chest construction slot must precede SAME C1");chest_slots->push_back(slot);s.world=level;s.roots=roots;
 s.visual.owner=level;
 // Genuine pre-InitFinal PFObject.user==NULL branch, using the existing whole
 // producer kernel. Positive registered PF objects need Root's full fields.
 s.visual.update_pf=[slot](std::string& e){auto graph=slot->lock();if(!graph){e="Required SAME chest PF receiver";return false;}navigation::ProducerRequest request{};request.object=&graph->receiver().base().runtime().object;request.key=graph->receiver().base().identity();const auto status=dh2_nav_update_game_object(&request);if(status){e="Required actual Container registered PF/body producer fields";return false;}return true;};
 s.visual.read_asset=[archive](const std::string& name,std::vector<std::uint8_t>& bytes,bool& found,std::string& e){return archive.read(name,found,bytes,e);};
 s.container.resolve_row=[chest_data](const auto& name,auto& id,auto& row,auto& e){return chest_data->resolve_row(name,id,row,e);};
 // Explicit standalone device facts. Production binds the real initialized
 // owner instead; the predicate itself is the exact frozen root source.
 s.initialization.device_high_performance=[level](bool& out,std::string& e){DevicePipelineBorrowServicesV1 device;device.actual_device_owner=level;device.borrow_actual=[](ui::HudDevicePipeline16& actual,std::string&){actual={{0,0,0},8};return true;};return device_high_performance_from_borrow_v1(device,out,e);};
 s.initialization.condition_init=[slot,level](std::uint32_t offset,std::string& e){auto graph=slot->lock();if(!graph){e="Required SAME chest ConditionData receiver";return false;}ConditionDataInitServicesV3 actual;actual.owner=level;return condition_data_init_v3(graph->receiver().base(),offset,actual,e);};
 s.container.visual_asset=[chest_data,slot](int id,std::string& e){auto graph=slot->lock();if(!graph){e="Required SAME chest for dictionary visual";return false;}return chest_data->visual_asset(graph->receiver().base(),id,e);};return true;};
class_input.spawn_v21_services=[application](const std::shared_ptr<CanonicalOpenableGraphV21>& graph,CanonicalSpawnApplicationServicesV4& s,std::string&){require(graph&&graph->receiver().base().identity(),"Container RNG requires actual receiver");s.application_lease=application;s.random=application.get();s.online_byte5=[](bool& value,std::string&){value=false;return true;};s.handle_as_player=[](bool& value,std::string&){value=false;return true;};return true;};
CanonicalAuxiliaryInputsV16 auxiliary_inputs;auxiliary_inputs.world=level;
auxiliary_inputs.dummy=[](const CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalDummyRecordV16>& record,GameObjectInitializationServicesV1& initialization,auto&,std::string&){
 std::weak_ptr<CanonicalDummyRecordV16> weak=record;initialization.set_position=[weak](const float* p,bool destination,std::string& e){auto record=weak.lock();return record&&game_object_set_position_v2(record->owner->base(),p,destination,{},e);};return true;
};
auxiliary_inputs.spawn_point=[](const CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalSpawnPointRecordV16>& record,GameObjectInitializationServicesV1& initialization,auto&,std::string&){
 std::weak_ptr<CanonicalSpawnPointRecordV16> weak=record;initialization.set_position=[weak](const float* p,bool destination,std::string& e){auto record=weak.lock();return record&&game_object_set_position_v2(record->owner->base(),p,destination,{},e);};return true;
};
auxiliary_inputs.decor=[](const CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalDecorRecordV16>& record,GameObjectInitializationServicesV1& initialization,auto&,std::string&){
 std::weak_ptr<CanonicalDecorRecordV16> weak=record;initialization.set_position=[weak](const float* p,bool destination,std::string& e){auto record=weak.lock();return record&&game_object_set_position_v2(record->owner->base(),p,destination,{},e);};return true;
};
auxiliary_inputs.destructible=[](const CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalDestructibleRecordV16>& record,GameObjectInitializationServicesV1& initialization,auto&,std::string&){
 std::weak_ptr<CanonicalDestructibleRecordV16> weak=record;initialization.set_position=[weak](const float* p,bool destination,std::string& e){auto record=weak.lock();return record&&game_object_set_position_v2(record->owner->base(),p,destination,{},e);};return true;
};
auxiliary_inputs.trigger_zone=[](const CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalTriggerZoneRecordV22>& record,GameObjectInitializationServicesV1& initialization,auto&,std::string&){
 std::weak_ptr<CanonicalTriggerZoneRecordV22> weak=record;initialization.set_position=[weak](const float* p,bool destination,std::string& e){auto record=weak.lock();return record&&game_object_set_position_v2(record->owner->base(),p,destination,{},e);};return true;
};
auxiliary_inputs.animated_decor=[](const CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalAnimatedDecorRecordV23>& record,GameObjectInitializationServicesV1& initialization,auto& services,std::string&){
 std::weak_ptr<CanonicalAnimatedDecorRecordV23> weak=record;initialization.set_position=[weak](const float* p,bool destination,std::string& e){auto record=weak.lock();return record&&game_object_set_position_v2(record->owner->base(),p,destination,{},e);};services.decor.owner=services.owner;return true;
};
auxiliary_inputs.checkpoint=[](const CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalCheckpointRecordV26>& record,GameObjectInitializationServicesV1& initialization,auto&,std::string&){
 std::weak_ptr<CanonicalCheckpointRecordV26> weak=record;initialization.set_position=[weak](const float* value,bool destination,std::string& e){auto record=weak.lock();return record&&game_object_set_position_v2(record->owner->base(),value,destination,{},e);};return true;
};
auxiliary_inputs.door=[](const CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalDoorRecordV27>& record,GameObjectInitializationServicesV1& initialization,auto&,std::string&){
 std::weak_ptr<CanonicalDoorRecordV27> weak=record;initialization.set_position=[weak](const float* value,bool destination,std::string& e){auto record=weak.lock();return record&&game_object_set_position_v2(record->owner->base(),value,destination,{},e);};return true;
};
auxiliary_inputs.trigger_object=[](const CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalTriggerObjectRecordV28>& record,GameObjectInitializationServicesV1& initialization,auto&,std::string&){
 std::weak_ptr<CanonicalTriggerObjectRecordV28> weak=record;initialization.set_position=[weak](const float* value,bool destination,std::string& e){auto record=weak.lock();return record&&game_object_set_position_v2(record->owner->base(),value,destination,{},e);};return true;
};
auxiliary_inputs.exit_zone=[](const CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalExitZoneRecordV29>& record,GameObjectInitializationServicesV1& initialization,auto&,std::string&){
 std::weak_ptr<CanonicalExitZoneRecordV29> weak=record;initialization.set_position=[weak](const float* value,bool destination,std::string& e){auto record=weak.lock();return record&&game_object_set_position_v2(record->owner->base(),value,destination,{},e);};return true;
};
auxiliary_inputs.quest_move=[](const CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalQuestMoveRecordV31>& record,GameObjectInitializationServicesV1& initialization,auto&,std::string&){
 std::weak_ptr<CanonicalQuestMoveRecordV31> weak=record;initialization.set_position=[weak](const float* value,bool destination,std::string& e){auto record=weak.lock();return record&&game_object_set_position_v2(record->owner->base(),value,destination,{},e);};return true;
};
auxiliary_inputs.sound=[](const CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalSoundEmitterRecordV32>& record,GameObjectInitializationServicesV1& initialization,auto&,std::string&){
 std::weak_ptr<CanonicalSoundEmitterRecordV32> weak=record;initialization.set_position=[weak](const float* value,bool destination,std::string& e){auto record=weak.lock();return record&&game_object_set_position_v2(record->owner->base(),value,destination,{},e);};return true;
};
auto auxiliary=std::make_shared<CanonicalAuxiliaryFamiliesV16>(std::move(auxiliary_inputs));
class_input.remaining=[auxiliary](const CanonicalFactoryEntryV1& entry,const CanonicalSourceObjectRequestV1& source,CanonicalClassReceiverV1& out,std::string& e){return auxiliary->construct(entry,source,out,e);};
const auto& dummies=auxiliary->dummies();const auto& destructibles=auxiliary->destructibles();const auto& spawn_points=auxiliary->spawn_points();const auto& decor=auxiliary->decor();const auto& triggers=auxiliary->triggers();const auto& animated=auxiliary->animated_decor();const auto& checkpoints=auxiliary->checkpoints();const auto& doors=auxiliary->doors();const auto& trigger_objects=auxiliary->trigger_objects();const auto& exit_zones=auxiliary->exit_zones();const auto& quest_moves=auxiliary->quest_move_zones();const auto& sound_emitters=auxiliary->sound_emitters();
CanonicalLevelClassDispatchV1 class_dispatch(std::move(class_input));
auto& containers=class_dispatch.containers_v21();
loader::CanonicalReceiverTransportV1 transport(properties,{level,&class_dispatch,[](void* p,const CanonicalFactoryEntryV1& f,const CanonicalSourceObjectRequestV1& q,CanonicalClassReceiverV1& r,std::string& e){return static_cast<CanonicalLevelClassDispatchV1*>(p)->construct(f,q,r,e);},nullptr});auto dispatch=transport.services();
// No copied level-specific declarations: feed the original complete MLX walk.
std::shared_ptr<RetainedLevelModuleGraphV1> preparation;
require(RetainedLevelModuleGraphV1::create({archive,level,bytes,&fixture.manager,dispatch,{[](bool ok,std::string&){return ok;},[](std::string&){return true;}},graph,floors,rooms},preparation,e),e);
const auto& source=preparation->root_file();
require(preparation->level()==level&&&preparation->manager()==&fixture.manager&&preparation->floor_world()==floors,"retained API replaced an authoritative owner");
LevelFileWalkStepV1 result=LevelFileWalkStepV1::pending;
for(unsigned n=0;n<1000&&result==LevelFileWalkStepV1::pending;++n)result=preparation->load_root_step();
require(result==LevelFileWalkStepV1::complete,source.error());require(source.attempts().size()==10,"original root declaration count changed");
for(const auto& attempt:source.attempts()){
 const auto* kind=attempt->source().entry().source().attribute("gametype");
 if(!kind||(*kind!="Module"&&*kind!="Block"))continue;
 const auto* object=fixture.manager.object(attempt->factory_attempt()->handle().key);
 require(object&&*object->type_f4==5,"actual Module registration missing");
 auto record=std::static_pointer_cast<CanonicalModuleRecordV2>(object->lease);
 require(record->receiver->base().room64()==-1,"diagnostic occurrence replaced source room64");
 require(preparation->initialize_next_module(e),e);
 require(record->receiver->module_id()==static_cast<std::int32_t>(modules.size())&&!record->receiver->load_floor_pending(),"actual Module counter/pending state mismatch");
 modules.push_back(record);
}
require(fixture.manager.object(0)==nullptr&&fixture.manager.source_next_key4c()==20,"reserved-null manager key or allocation counter mismatch");
 require(config_queries==1&&*level_fields.config38!=0,"actual config early publication missing");
assert(counter==27);assert(modules.size()==9&&rooms->rooms().size()==9&&floors->records.size()==16&&rooms->exits().size()==19);assert(fixture.manager.pending().size()==9&&fixture.manager.source_count50()==19&&globals.next_generated_zone==9&&zone_pf==9);for(unsigned i=0;i<9;++i){auto* zone=zone_factory.find(fixture.manager.pending()[i]);assert(zone);auto* object=fixture.manager.object(zone->base().shared_handle().key);assert(object&&*object->type_f4==11);assert(zone&&zone->module()==modules[i]->receiver->base().identity());assert(rooms->rooms()[i]->flags24==1&&rooms->rooms()[i]->source_room1c==UINT32_MAX);}if(!preparation->prepare_floors(e)){std::cerr<<e;return 1;}assert(preparation->status().floors_prepared);for(unsigned i=0;i<9;++i)assert(!std::memcmp(&floors->collision_rooms[i].bounds,&rooms->rooms()[i]->bounds,sizeof(octree::Box)));assert(!std::memcmp(&floors->collision_world.bounds,&rooms->bounds(),sizeof(octree::Box)));std::vector<std::shared_ptr<RetainedModuleVisualV3>> retained_visuals;for(auto& m:modules){if(!preparation->finalize_next_module(e)){std::cerr<<e;return 1;}assert(m->runtime.object.user==m->receiver->base().identity());assert(*m->receiver->base().pointer(0x180)==0);auto visual=graph->visual(*m->receiver->base().pointer(0x2d8));require(visual&&visual->controller_identity()!=0&&visual->controller_root_identity()==visual->root_identity(),"Module controller/root identity mismatch");retained_visuals.push_back(visual);}
// Rendering bridge reads the real post-PF Module graph, including removals.
std::vector<ModuleDrawFrameV1> frames;require(preparation->capture_draw_frames(frames,e),e);require(frames.size()==9&&preparation->status().geometry_prepared,"retained API geometry prefix missing");unsigned render_meshes=0;
for(unsigned k=0;k<modules.size();++k){
 auto visual=retained_visuals[k];const auto& frame=frames[k];
 
 require(frame.root_identity==visual->root_identity()&&frame.visual_identity==visual->identity(),"render frame lost SAME root/visual identity");
 const auto& root=visual->scene();const auto& selected=root.selected();unsigned expected=0;
 for(unsigned i=0;i<selected.scene.instances.size();++i)if(root.mesh_attached(i)&&selected.instance_visibility[i]&&(root.root_flags()&1u))++expected;
 require(frame.meshes.size()==expected&&expected>0,"render frame skipped attached visible meshes");
 for(const auto& draw:frame.meshes){const auto& actual=selected.scene.instances[draw.source_instance];require(draw.cached_world==actual.world&&draw.geometry==actual.geometry,"render bridge recomputed optimized cache");}
 render_meshes+=frame.meshes.size();
 // Explicit visibility transport exercise, not a fabricated condition result.
 require(visual->scene().notify_root_visibility(false,e),e);ModuleDrawFrameV1 hidden;
 require(capture_module_draw_frame_v1(visual,modules[k]->receiver->module_id(),hidden,e)&&hidden.meshes.empty(),"hidden actual root still submitted meshes");
 require(visual->scene().notify_root_visibility(true,e),e);ModuleDrawFrameV1 restored;
 require(capture_module_draw_frame_v1(visual,modules[k]->receiver->module_id(),restored,e)&&restored.meshes.size()==expected,"actual root visibility restore lost draw membership");
}
auto file_relay=preparation->module_files();auto load_borrow=file_relay->load_borrow();
bool source_failed=false;
while(!preparation->status().object_sources_complete){if(!preparation->load_next_module_sources({},e)){source_failed=true;break;}}
const bool all_object_sources_complete=preparation->status().object_sources_complete;require(source_failed!=all_object_sources_complete,"module source completion/failure state differs");
const auto loaded_source_modules=preparation->status().loaded;
require(preparation->status().geometry_prepared&&(source_failed?preparation->status().failed_at==RetainedModulePreparationStageV1::module_files:all_object_sources_complete),"retained source outcome lost prepared geometry or misreported completion");
std::vector<ModuleDrawFrameV1> after_failure;require(preparation->capture_draw_frames(after_failure,e)&&after_failure.size()==9,"retained partial graph is inaccessible after object failure");
 const auto module_error=preparation->status().error;
 const auto stopped_file_index=source_failed?static_cast<std::int32_t>(file_relay->file_count()-1):-1;
 std::vector<const CanonicalBoundSourceAttemptV1*> attempts;
 for(std::size_t file_index=0;file_index<file_relay->file_count();++file_index)for(const auto& attempt:file_relay->file(file_index).attempts())attempts.push_back(attempt.get());
 const auto stopped_type=source_failed?*attempts.back()->source().entry().source().attribute("gametype"):std::string{};
 const bool first_mgp_complete=file_relay->file(0).completed();
 unsigned character_count=0;
 for(std::size_t i=0;i<attempts.size()-(source_failed?1:0);++i){
  const auto& attempt=*attempts[i];auto* factory=attempt.factory_attempt();require(factory&&factory->stage()==CanonicalFactoryStageV1::complete,"earlier authored object did not complete its real factory");
  const auto* original=canonical_source_entry_v1(attempt.source().request());auto* object=fixture.manager.object(factory->handle().key);require(original&&object,"completed authored object lost XML/manager identity");
  const auto* kind=original->source().attribute("gametype");
  if(kind&&(*kind=="Character"||*kind=="Player")){
   ++character_count;auto character=character_factory->find(object->identity);require(character&&character->actor,"authored Character factory lost SAME receiver");
   const auto* description=original->source().attribute("charpropsname");const auto* authored_template=original->source().attribute("char_template");
   require(*character->actor->source_string(0x13b0)==(description?*description:std::string{})&&*character->actor->source_string(0x1398)==(authored_template?*authored_template:std::string{}),"authored Character direct/template fields lost");
   std::array<float,3> actual{};require(character->actor->source_position(actual,e),e);const char* value=original->source().attribute("position")->c_str();
   for(unsigned axis=0;axis<3;++axis){char* end{};auto expected=static_cast<float>(std::strtod(value,&end));require(actual[axis]==expected+attempt.source().request().module_offset[axis],"authored Character placement/module offset lost");value=*end==','?end+1:end;}
  }
 }
 const auto supported_objects=containers.size()+character_count+dummies.size()+destructibles.size()+spawn_points.size()+decor.size()+triggers.size()+animated.size()+checkpoints.size()+doors.size()+trigger_objects.size()+exit_zones.size()+quest_moves.size()+sound_emitters.size();
require(file_relay->file_count()>=2&&first_mgp_complete&&attempts.size()==supported_objects+(source_failed?1:0)&&!containers.empty(),"actual MGP constructor journal differs: "+module_error);
if(source_failed)require(*attempts.back()->source().entry().source().attribute("gametype")!="Dummy"&&attempts.back()->factory_attempt()->prefix()==CanonicalFactoryStageV1::empty,"next actual class boundary differs");
 if(all_object_sources_complete)require(loaded_source_modules==9&&file_relay->file_count()==18&&supported_objects==195&&character_count==50,"complete source walk differs from independent original SWAMP inventory");
 const auto* config=fixture.manager.object(source.attempts().front()->factory_attempt()->handle().key);std::uint8_t across{};
 require(config&&config->read_across_rooms87(config->context,across,e)&&across==0,"documented Config repair lost");
 for(auto id:fixture.manager.pending()){auto* zone=zone_factory.find(id);const auto* object=zone?fixture.manager.object(zone->base().shared_handle().key):nullptr;require(object&&object->read_across_rooms87(object->context,across,e)&&across==0,"documented RoomZone repair lost");}
 require(fixture.manager.source_count50()==19+supported_objects&&fixture.manager.source_next_key4c()==20+supported_objects&&transport.retained_count()==10+supported_objects,"actual Container registration prefix differs");
 const auto chest_count=containers.size();
 for(std::size_t i=0;i<containers.size();++i){
  const auto& chest=containers[i];const CanonicalBoundSourceAttemptV1* selected=nullptr;
  for(const auto* attempt:attempts){auto* factory=attempt->factory_attempt();auto* object=factory?fixture.manager.object(factory->handle().key):nullptr;if(object&&object->identity==chest->receiver().base().identity())selected=attempt;}
  require(selected&&selected->factory_attempt()->stage()==CanonicalFactoryStageV1::complete&&chest_slots->at(i)->lock()==chest,"same source chest graph/constructor lost");
  const auto& request=selected->source().request();const auto* object=fixture.manager.object(selected->factory_attempt()->handle().key);
  require(object&&*object->type_f4==7&&*object->room64==request.runtime_module_id,"SAME Container publication/module identity lost");
  const auto* original=canonical_source_entry_v1(request);require(original&&original->source().attribute("data_desc")&&original->source().attribute("position"),"authored Container source missing");
  require(chest->receiver().fields().data_desc==*original->source().attribute("data_desc"),"authored Container data description lost");
  const char* value=original->source().attribute("position")->c_str();
  for(unsigned axis=0;axis<3;++axis){char* end{};auto expected=static_cast<float>(std::strtod(value,&end));require(chest->receiver().base().vector3(0x160)[axis]==expected+request.module_offset[axis],"authored Container placement/module offset lost");value=*end==','?end+1:end;}
 }
unsigned expected_template_assertions=0;
 auto count_template_producers=[&](const CanonicalBoundSourceAttemptV1& attempt){
  if(!attempt.factory_attempt()||attempt.factory_attempt()->stage()!=CanonicalFactoryStageV1::complete)return;
  const auto request=attempt.source().request();for(const char* key:{"template","_templateName"}){auto value=request.attribute(request.source_context,request.element,key);if(value&&*value)++expected_template_assertions;}
 };
 for(const auto& attempt:source.attempts())count_template_producers(*attempt);
 for(const auto* attempt:attempts)count_template_producers(*attempt);
 require(template_assertions==expected_template_assertions,"original named template producer/delivery count differs");
if(source_failed){require(!preparation->load_next_module_sources({},e)&&e==module_error&&containers.size()==chest_count,"retained preparation replayed a failed prefix");
 require(!modules[loaded_source_modules]->receiver->load({},load_borrow,e)&&e==module_error&&containers.size()==chest_count&&file_relay->file(0).completed()&&file_relay->file(stopped_file_index).attempts().back()->factory_attempt()->prefix()==CanonicalFactoryStageV1::empty,"failed source replayed constructors");
}else{const auto retained_objects=fixture.manager.source_count50();require(preparation->load_next_module_sources({},e)&&preparation->status().object_sources_complete&&file_relay->file_count()==18&&fixture.manager.source_count50()==retained_objects&&containers.size()==chest_count,"completed source preparation replayed constructors");}
 if(source_failed)require(*load_borrow.object_module_id18c==modules.at(loaded_source_modules)->receiver->module_id(),"failed source erased SAME Level context");else require(*load_borrow.object_module_id18c==-1&&load_borrow.module_offset160[0]==0.f&&load_borrow.module_offset160[1]==0.f&&load_borrow.module_offset160[2]==0.f,"completed module source load did not restore SAME Level context");
 auto character_attempt=attempts[2]->factory_attempt();require(character_attempt->stage()==CanonicalFactoryStageV1::complete,"actual Character whole-position factory did not complete");auto npc=character_factory->find(fixture.manager.object(character_attempt->handle().key)->identity);
 require(npc&&*npc->actor->source_string(0x13b0)=="WanderingPriest"&&npc->actor->source_string(0x1398)->empty(),"actual Character description/template fields lost");
 const auto* original_character=canonical_source_entry_v1(attempts[2]->source().request());std::array<float,3> source_position{};require(npc->actor->source_position(source_position,e),e);
 const char* at=original_character->source().attribute("position")->c_str();for(unsigned axis=0;axis<3;++axis){char* end{};auto expected=static_cast<float>(std::strtod(at,&end));require(source_position[axis]==expected+character_attempt->source().module_offset[axis],"authored Character position/module offset lost after whole SetPosition");at=*end==','?end+1:end;}
require(!dummies.empty(),"source did not reach any real Dummy constructor");
 for(const auto& record:dummies){auto& dummy=record->owner;decltype(canonical_source_entry_v1(attempts.front()->source().request())) original=nullptr;
  for(const auto& attempt:attempts){auto* factory=attempt->factory_attempt();auto* object=factory?fixture.manager.object(factory->handle().key):nullptr;if(object&&object->identity==dummy->base().identity()){require(factory->stage()==CanonicalFactoryStageV1::complete,"actual Dummy factory incomplete");original=canonical_source_entry_v1(attempt->source().request());}}
  require(original&&dummy->base().type_f4()==20&&*dummy->base().byte(0x84)==1&&*dummy->base().string(0x30)==*original->source().attribute("name"),"actual Dummy source identity/defaults lost");
 }

 require(!destructibles.empty(),"source did not reach a real Destructible constructor");
 for(const auto& record:destructibles){
  auto& barrel=*record->owner;decltype(attempts.front()->factory_attempt()) selected=nullptr;
  for(const auto& attempt:attempts){auto* factory=attempt->factory_attempt();auto* object=factory?fixture.manager.object(factory->handle().key):nullptr;if(object&&object->identity==barrel.base().identity())selected=factory;}
  require(selected&&selected->stage()==CanonicalFactoryStageV1::complete&&barrel.base().type_f4()==1&&*barrel.base().byte(0x84)==0,"actual Destructible constructor/default/manager identity differs");
  require(*barrel.base().pointer(0x100)==reinterpret_cast<std::uintptr_t>(&barrel.network(0))&&*barrel.base().pointer(0x104)==reinterpret_cast<std::uintptr_t>(&barrel.network(1)),"Destructible network owners differ");
  decltype(canonical_source_entry_v1(attempts.front()->source().request())) original=nullptr;
  for(const auto& attempt:attempts)if(attempt->factory_attempt()==selected)original=canonical_source_entry_v1(attempt->source().request());
  require(original&&*barrel.base().string(0x30)==*original->source().attribute("name"),"actual Destructible identity lost");
  const char* value=original->source().attribute("position")->c_str();
  for(unsigned axis=0;axis<3;++axis){char* end{};auto expected=static_cast<float>(std::strtod(value,&end));require(barrel.base().vector3(0x160)[axis]==expected+selected->source().module_offset[axis],"authored Destructible position/module offset lost");value=*end==','?end+1:end;}
 }
 require(!spawn_points.empty(),"source did not reach a real SpawnPoint constructor");
 for(const auto& record:spawn_points){
  auto& spawn_point=*record->owner;decltype(attempts.front()->factory_attempt()) selected=nullptr;
  for(const auto& attempt:attempts){auto* factory=attempt->factory_attempt();auto* object=factory?fixture.manager.object(factory->handle().key):nullptr;if(object&&object->identity==spawn_point.base().identity())selected=factory;}
  require(selected&&selected->stage()==CanonicalFactoryStageV1::complete&&spawn_point.base().type_f4()==13,"actual SpawnPoint constructor/manager identity differs");
  decltype(canonical_source_entry_v1(attempts.front()->source().request())) original=nullptr;
  for(const auto& attempt:attempts)if(attempt->factory_attempt()==selected)original=canonical_source_entry_v1(attempt->source().request());
  require(original&&*spawn_point.base().string(0x30)==*original->source().attribute("name"),"actual SpawnPoint identity lost");
  const auto* entrypoint=original->source().attribute("entrypointID");const auto* script=original->source().attribute("script");
  require(spawn_point.entrypoint()==(entrypoint?std::atoi(entrypoint->c_str()):-1)&&spawn_point.script_name()==(script?*script:std::string{}),"authored SpawnPoint entry/script fields lost");
  const char* value=original->source().attribute("position")->c_str();
  for(unsigned axis=0;axis<3;++axis){char* end{};auto expected=static_cast<float>(std::strtod(value,&end));require(spawn_point.base().vector3(0x160)[axis]==expected+selected->source().module_offset[axis],"authored SpawnPoint position/module offset lost");value=*end==','?end+1:end;}
 }
 require(!triggers.empty(),"source did not reach a real TriggerZone constructor");
 for(const auto& record:triggers){auto& zone=*record->owner;CanonicalSourceObjectRequestV1 captured;decltype(canonical_source_entry_v1(attempts.front()->source().request())) original=nullptr;
  for(const auto& attempt:attempts){auto* factory=attempt->factory_attempt();auto* object=factory?fixture.manager.object(factory->handle().key):nullptr;if(object&&object->identity==zone.base().identity()){captured=attempt->source().request();require(factory->stage()==CanonicalFactoryStageV1::complete,"actual TriggerZone factory incomplete");original=canonical_source_entry_v1(attempt->source().request());}}
  require(original&&zone.base().type_f4()==20&&*zone.base().string(0x30)==*original->source().attribute("name"),"actual TriggerZone source identity lost");
  const char* value=original->source().attribute("position")->c_str();
  for(unsigned axis=0;axis<3;++axis){char* end{};auto expected=static_cast<float>(std::strtod(value,&end));require(zone.base().vector3(0x160)[axis]==expected+captured.module_offset[axis],"authored TriggerZone position/module offset lost");value=*end==','?end+1:end;}
 }
 require(!animated.empty(),"source did not reach a real AnimatedDecor constructor");
 for(const auto& record:animated){auto& scenery=*record->owner;CanonicalSourceObjectRequestV1 captured;decltype(canonical_source_entry_v1(attempts.front()->source().request())) original=nullptr;
  for(const auto& attempt:attempts){auto* factory=attempt->factory_attempt();auto* object=factory?fixture.manager.object(factory->handle().key):nullptr;if(object&&object->identity==scenery.base().identity()){captured=attempt->source().request();require(factory->stage()==CanonicalFactoryStageV1::complete,"actual AnimatedDecor factory incomplete");original=canonical_source_entry_v1(attempt->source().request());}}
  require(original&&scenery.base().type_f4()==20&&*scenery.base().byte(0x84)==1&&scenery.load_floor()==0&&*scenery.base().string(0x30)==*original->source().attribute("name"),"source AnimatedDecor constructor/identity differs");
  const auto* animation=original->source().attribute("startanim");require(scenery.start_animation()==(animation?*animation:std::string{}),"authored startanim lost");
  const char* value=original->source().attribute("position")->c_str();
  for(unsigned axis=0;axis<3;++axis){char* end{};auto expected=static_cast<float>(std::strtod(value,&end));require(scenery.base().vector3(0x160)[axis]==expected+captured.module_offset[axis],"authored AnimatedDecor position/module offset lost");value=*end==','?end+1:end;}
 }
 const auto animation_before=animated.front()->owner->start_animation();
 std::array<float,3> animated_position_before{};std::copy_n(animated.front()->owner->base().vector3(0x160),3,animated_position_before.begin());
 require(!animated.front()->owner->init_post(e)&&e=="Required actual AnimatedDecor InitPost389128"&&animated.front()->owner->start_animation()==animation_before&&std::equal(animated_position_before.begin(),animated_position_before.end(),animated.front()->owner->base().vector3(0x160)),"missing AnimatedDecor lifecycle fabricated success or mutated constructor prefix");
 require(!checkpoints.empty(),"source did not reach a real CheckpointZone constructor");
 for(const auto& record:checkpoints){auto& checkpoint=*record->owner;CanonicalSourceObjectRequestV1 captured;decltype(canonical_source_entry_v1(attempts.front()->source().request())) original=nullptr;
  for(const auto& attempt:attempts){auto* factory=attempt->factory_attempt();auto* object=factory?fixture.manager.object(factory->handle().key):nullptr;if(object&&object->identity==checkpoint.base().identity()){require(factory->stage()==CanonicalFactoryStageV1::complete,"actual Checkpoint factory incomplete");captured=attempt->source().request();original=canonical_source_entry_v1(captured);}}
  require(original&&checkpoint.base().type_f4()==12&&*checkpoint.base().byte(0x84)==1&&checkpoint.physical()&&checkpoint.trigger()&&*checkpoint.base().string(0x30)==*original->source().attribute("name"),"actual Checkpoint constructor/identity differs");
  const auto* dimension=original->source().attribute("dimensions");std::array<float,3> expected_dimensions{{200,200,200}};
  if(dimension){const char* value=dimension->c_str();for(unsigned axis=0;axis<3;++axis){char* end{};expected_dimensions[axis]=static_cast<float>(std::strtod(value,&end));value=*end==','?end+1:end;}}
  require(checkpoint.dimensions()==expected_dimensions,"authored Checkpoint dimensions/defaults lost");
  const char* value=original->source().attribute("position")->c_str();
  for(unsigned axis=0;axis<3;++axis){char* end{};auto expected=static_cast<float>(std::strtod(value,&end));require(checkpoint.base().vector3(0x160)[axis]==expected+captured.module_offset[axis],"authored Checkpoint placement/module offset lost");value=*end==','?end+1:end;}
 }
 require(!checkpoints.front()->owner->init_post(e)&&e=="Required actual Checkpoint Zone InitPost39771c","checkpoint lifecycle fabricated success");
 require(!checkpoints.front()->owner->collision_begin(1,e)&&e=="Required actual Checkpoint collision/save395f04"&&checkpoints.front()->owner->source_checkpoint388().empty(),"checkpoint activation/save fabricated success or changed empty C1 storage");
 require(!doors.empty(),"source did not reach actual Door");
 for(const auto& record:doors){auto& door=*record->owner;CanonicalSourceObjectRequestV1 captured;decltype(canonical_source_entry_v1(attempts.front()->source().request())) original=nullptr;
  for(const auto* attempt:attempts){auto* factory=attempt->factory_attempt();auto* object=factory?fixture.manager.object(factory->handle().key):nullptr;if(object&&object->identity==door.base().identity()){require(factory->stage()==CanonicalFactoryStageV1::complete,"actual Door factory incomplete");captured=attempt->source().request();original=canonical_source_entry_v1(captured);}}
  require(original&&door.base().type_f4()==2&&*door.base().byte(0x84)==1&&*door.base().byte(0x28)==1&&*door.base().byte(0xf8)==3&&!door.physical()&&door.trigger()&&*door.base().string(0x30)==*original->source().attribute("name"),"Door source constructor/identity differs");
  auto expected_string=original->source().attribute("data");require(door.source_data388()==(expected_string?*expected_string:std::string{}),"authored Door data lost");
  auto opened=original->source().attribute("opened"),collision=original->source().attribute("door_collision");
  require(door.source_opened3a4()==(opened?std::atoi(opened->c_str())!=0:1)&&door.source_collision3a5()==(collision?std::atoi(collision->c_str())!=0:1)&&!door.source_table_id3a0().has_value()&&door.source_state3a8()==0,"Door source properties or unproduced data row differ");
  std::array<float,3> expected_dimensions{{200,200,200}};if(auto value=original->source().attribute("dimensions")){const char* at=value->c_str();for(auto& component:expected_dimensions){char* end{};component=static_cast<float>(std::strtod(at,&end));at=*end==','?end+1:end;}}
  require(door.dimensions()==expected_dimensions,"Door dimensions lost");
  for(unsigned index=0;index<2;++index){auto& net=door.source_network(index);require(*door.base().pointer(index?0x104:0x100)==reinterpret_cast<std::uintptr_t>(&net)&&net.count104==3,"Door same network owner lost");for(unsigned member=0;member<3;++member)require(net.declared[member]==&net.members[member]&&net.members[member].type134==1&&net.members[member].value14d==0,"Door bool network declaration/backing differs");}
  const char* value=original->source().attribute("position")->c_str();for(unsigned axis=0;axis<3;++axis){char* end{};const auto expected=static_cast<float>(std::strtod(value,&end));require(door.base().vector3(0x160)[axis]==expected+captured.module_offset[axis],"Door authored position/module offset lost");value=*end==','?end+1:end;}
 }
 {actor::RuntimeState runtime{};CanonicalDoorV27 fresh(level,runtime,{},{});
  require(fresh.source_opened3a4()==0&&fresh.source_collision3a5()==1&&fresh.dimensions()==std::array<float,3>{}&&!fresh.source_table_id3a0()&&fresh.source_data388().empty(),"Door C1 prematurely applied properties");
  fresh.base().class_name20()="Door";auto fields=fresh.properties();require(properties.init_properties(fields,e)&&properties.load_defaults(fields,e),e);
  require(fresh.source_opened3a4()==1&&fresh.dimensions()==std::array<float,3>{{200,200,200}},"Door C1/property default distinction lost");
 }
 const auto door_data_before=doors.front()->owner->source_data388();
 require(!doors.front()->owner->init_post(e)&&e=="Required actual Door InitPost3e7da8"&&doors.front()->owner->source_data388()==door_data_before,"Door missing InitPost fabricated success/mutated prefix");
 require(!doors.front()->owner->init_final(e)&&e=="Required actual Door InitFinal3e7c1c","Door missing InitFinal fabricated success");
 require(!doors.front()->owner->destroy(e)&&e=="Required actual Door/NetStruct/Zone/GameObject destruction","Door missing destructor fabricated success");
 require(!trigger_objects.empty(),"source did not reach actual TriggerObject");
 for(const auto& record:trigger_objects){auto& trigger=*record->owner;CanonicalSourceObjectRequestV1 captured;decltype(canonical_source_entry_v1(attempts.front()->source().request())) original=nullptr;
  for(const auto* attempt:attempts){auto* factory=attempt->factory_attempt();auto* object=factory?fixture.manager.object(factory->handle().key):nullptr;if(object&&object->identity==trigger.base().identity()){require(factory->stage()==CanonicalFactoryStageV1::complete,"TriggerObject factory incomplete");captured=attempt->source().request();original=canonical_source_entry_v1(captured);}}
  require(original&&trigger.base().type_f4()==20&&*trigger.base().byte(0x85)==1&&*trigger.base().byte(0x28)==1&&*trigger.base().byte(0xf8)==4&&!trigger.physical()&&trigger.trigger()&&*trigger.base().string(0x30)==*original->source().attribute("name"),"TriggerObject source identity/C1 differs");
  const char* string_names[]{"data","script","scriptOff","unlock_cond"};const unsigned string_offsets[]{0x718,0x734,0x750,0x76c};
  for(unsigned i=0;i<4;++i){auto value=original->source().attribute(string_names[i]);require(*trigger.source_string(string_offsets[i])==(value?*value:std::string{}),"TriggerObject authored data/script/unlock string lost");}
  auto count=original->source().attribute("triggercount"),delay=original->source().attribute("triggerdelay"),reset=original->source().attribute("resetonhubchange");
  require(*trigger.source_integer(0x3a8)==(count?std::atoi(count->c_str()):1)&&*trigger.source_integer(0x3ac)==(delay?std::atoi(delay->c_str()):0)&&*trigger.source_byte(0x3b0)==(reset?std::atoi(reset->c_str())!=0:0),"TriggerObject inherited trigger properties lost");
  require(*trigger.source_integer(0x730)==-1&&*trigger.source_integer(0x74c)==-1&&!trigger.source_integer(0x768)&&!trigger.source_script_id768()&&trigger.source_condition788()==0&&*trigger.source_byte(0x784)==0&&trigger.source_contacts().empty(),"TriggerObject premature script/condition/contact initialization");
  for(unsigned i=0;i<2;++i){auto& net=trigger.source_network(i);require(*trigger.base().pointer(i?0x104:0x100)==reinterpret_cast<std::uintptr_t>(&net)&&net.count104==3,"TriggerObject SAME network owner lost");for(unsigned member=0;member<3;++member)require(net.declared[member]==&net.members[member]&&net.members[member].type134==32&&net.members[member].value150==0,"TriggerObject shared source integer network projection differs");}
  std::array<float,3> expected_dimensions{{200,200,200}};if(auto value=original->source().attribute("dimensions")){const char* at=value->c_str();for(auto& component:expected_dimensions){char* end{};component=static_cast<float>(std::strtod(at,&end));at=*end==','?end+1:end;}}
  require(trigger.dimensions()==expected_dimensions,"TriggerObject authored dimensions lost");
  const char* value=original->source().attribute("position")->c_str();for(unsigned axis=0;axis<3;++axis){char* end{};auto expected=static_cast<float>(std::strtod(value,&end));require(trigger.base().vector3(0x160)[axis]==expected+captured.module_offset[axis],"TriggerObject authored position/module offset lost");value=*end==','?end+1:end;}
 }
 {actor::RuntimeState runtime{};CanonicalTriggerObjectV28 fresh(level,runtime,{},{});
  require(fresh.dimensions()==std::array<float,3>{}&&*fresh.base().byte(0x84)==0&&*fresh.base().byte(0x85)==1&&fresh.source_string(0x718)->empty()&&!fresh.source_integer(0x768),"TriggerObject constructor invented defaults/IDs");
  fresh.base().class_name20()="TriggerObject";auto fields=fresh.properties();require(properties.init_properties(fields,e)&&properties.load_defaults(fields,e),e);require(fresh.dimensions()==std::array<float,3>{{200,200,200}}&&*fresh.source_integer(0x3a8)==1&&*fresh.source_integer(0x3ac)==0&&*fresh.source_byte(0x3b0)==0,"TriggerObject source defaults differ");
 }
 const auto trigger_data_before=*trigger_objects.front()->owner->source_string(0x718);
 require(!trigger_objects.front()->owner->init_post(e)&&e=="Required actual TriggerObject InitPost399ff8"&&*trigger_objects.front()->owner->source_string(0x718)==trigger_data_before,"TriggerObject missing InitPost fabricated success/mutated prefix");
 require(!trigger_objects.front()->owner->interact(1,e)&&e=="Required actual TriggerObject Interact399af0"&&trigger_objects.front()->owner->source_condition788()==0,"TriggerObject missing interaction fabricated condition/progression");
 require(!trigger_objects.front()->owner->update(e)&&e=="Required actual TriggerObject Update3994a8","TriggerObject missing Update fabricated success");
 require(!exit_zones.empty(),"source did not reach actual ExitZone");
 for(const auto& record:exit_zones){auto& exit=*record->owner;CanonicalSourceObjectRequestV1 captured;decltype(canonical_source_entry_v1(attempts.front()->source().request())) original=nullptr;
  for(const auto* attempt:attempts){auto* factory=attempt->factory_attempt();auto* object=factory?fixture.manager.object(factory->handle().key):nullptr;if(object&&object->identity==exit.base().identity()){require(factory->stage()==CanonicalFactoryStageV1::complete,"ExitZone factory incomplete");captured=attempt->source().request();original=canonical_source_entry_v1(captured);}}
  require(original&&exit.base().type_f4()==14&&exit.base().identity()==reinterpret_cast<std::uintptr_t>(&exit)&&*exit.base().byte(0x28)==1&&*exit.base().byte(0xf8)==4&&!exit.physical()&&exit.trigger()&&*exit.base().string(0x30)==*original->source().attribute("name"),"ExitZone SAME base/type/identity differs");
  const char* integer_names[]{"levelID","entrypointID","maploc","question"};const unsigned integer_offsets[]{0x7d8,0x7f4,0x7f8,0x7fc};
  for(unsigned i=0;i<4;++i){auto value=original->source().attribute(integer_names[i]);require(exit.source_integer(integer_offsets[i])&&*exit.source_integer(integer_offsets[i])==(value?std::atoi(value->c_str()):-1),"ExitZone authored destination integer lost");}
  const char* string_names[]{"levelName","fasttravel","script","script_move_out","script_all_player","script_all_player_move_out","effect_one_player","is_door_closed"};const unsigned string_offsets[]{0x7dc,0x800,0x724,0x740,0x75c,0x778,0x794,0x7bc};
  for(unsigned i=0;i<8;++i){auto value=original->source().attribute(string_names[i]);require(exit.source_string(string_offsets[i])&&*exit.source_string(string_offsets[i])==(value?*value:std::string{}),"ExitZone authored/inherited reference lost");}
  require(*exit.source_integer(0x7d4)==-1&&*exit.source_byte(0x818)==0&&*exit.source_byte(0x819)==0,"ExitZone prematurely resolved destination or transitioned");
  const char* value=original->source().attribute("position")->c_str();for(unsigned axis=0;axis<3;++axis){char* end{};auto expected=static_cast<float>(std::strtod(value,&end));require(exit.base().vector3(0x160)[axis]==expected+captured.module_offset[axis],"ExitZone authored placement/module offset lost");value=*end==','?end+1:end;}
  for(unsigned i=0;i<2;++i){auto& net=exit.source_network(i);require(*exit.base().pointer(i?0x104:0x100)==reinterpret_cast<std::uintptr_t>(&net)&&net.count104==3,"ExitZone inherited SAME network owner lost");}
 }
 {actor::RuntimeState runtime{};ExitZoneServicesV29 services;services.owner=level;services.trigger.owner=level;services.trigger.initialization.owner=level;CanonicalExitZoneV29 fresh(level,runtime,std::move(services));
  require(fresh.base().type_f4()==14&&*fresh.base().byte(0x84)==1&&fresh.dimensions()==std::array<float,3>{}&&*fresh.source_integer(0x7d4)==-1&&!fresh.source_integer(0x7d8)&&!fresh.source_integer(0x7f4)&&!fresh.source_integer(0x7f8)&&!fresh.source_integer(0x7fc),"ExitZone C1 invented property-stage values");
  fresh.base().class_name20()="TriggerZoneExitLevel";auto fields=fresh.properties();require(properties.init_properties(fields,e)&&properties.load_defaults(fields,e),e);
  require(fresh.dimensions()==std::array<float,3>{{200,200,200}}&&*fresh.source_integer(0x7d8)==-1&&*fresh.source_integer(0x7f4)==-1&&*fresh.source_integer(0x7f8)==-1&&*fresh.source_integer(0x7fc)==-1,"ExitZone inherited/derived defaults differ");
 }
 const auto exit_name_before=*exit_zones.front()->owner->source_string(0x7dc);
 require(!exit_zones.front()->owner->init_post(e)&&e=="Required actual TriggerZone CheckSpawnProbability38bd64"&&*exit_zones.front()->owner->source_string(0x7dc)==exit_name_before&&*exit_zones.front()->owner->source_integer(0x7d4)==-1,"ExitZone parent/lookup InitPost order fabricated state");
 require(!exit_zones.front()->owner->update(e)&&e=="Required actual ExitZone Update39c33c/transition","ExitZone missing transition fabricated success");
 require(!quest_moves.empty(),"source did not reach actual QuestMoveInZone");
 for(const auto& record:quest_moves){auto& zone=*record->owner;CanonicalSourceObjectRequestV1 captured;decltype(canonical_source_entry_v1(attempts.front()->source().request())) original=nullptr;
  for(const auto* attempt:attempts){auto* factory=attempt->factory_attempt();auto* object=factory?fixture.manager.object(factory->handle().key):nullptr;if(object&&object->identity==zone.base().identity()){require(factory->stage()==CanonicalFactoryStageV1::complete,"QuestMoveInZone factory incomplete");captured=attempt->source().request();original=canonical_source_entry_v1(captured);}}
  require(original&&zone.base().type_f4()==20&&zone.base().identity()==reinterpret_cast<std::uintptr_t>(&zone)&&zone.physical()&&!zone.trigger()&&zone.source_colzone384()==0&&*zone.base().string(0x30)==*original->source().attribute("name"),"QuestMoveInZone SAME receiver flags/identity/name differs");
  const char* value=original->source().attribute("position")->c_str();for(unsigned axis=0;axis<3;++axis){char* end{};auto expected=static_cast<float>(std::strtod(value,&end));require(zone.base().vector3(0x160)[axis]==expected+captured.module_offset[axis],"QuestMoveInZone authored placement/module offset lost");value=*end==','?end+1:end;}
  auto dimensions=original->source().attribute("dimensions");value=dimensions?dimensions->c_str():"200,200,200";for(unsigned axis=0;axis<3;++axis){char* end{};auto expected=static_cast<float>(std::strtod(value,&end));require(zone.dimensions()[axis]==expected,"QuestMoveInZone authored dimensions lost");value=*end==','?end+1:end;}
  require(!zone.init_post(e)&&e=="Required actual QuestMoveInZone Zone InitPost39771c","QuestMoveInZone fabricated activation");
  require(!zone.collision_begin(1,e)&&e=="Required actual QuestMoveInZone collision/quest396120","QuestMoveInZone fabricated quest execution");
 }
 require(!sound_emitters.empty(),"source did not reach actual SoundEmitter");
 for(const auto& record:sound_emitters){auto& sound=*record->owner;CanonicalSourceObjectRequestV1 captured;decltype(canonical_source_entry_v1(attempts.front()->source().request())) original=nullptr;
  for(const auto* attempt:attempts){auto* factory=attempt->factory_attempt();auto* object=factory?fixture.manager.object(factory->handle().key):nullptr;if(object&&object->identity==sound.base().identity()){require(factory->stage()==CanonicalFactoryStageV1::complete,"SoundEmitter factory incomplete");captured=attempt->source().request();original=canonical_source_entry_v1(captured);}}
  auto name=original?original->source().attribute("sound"):nullptr;
  require(original&&sound.base().type_f4()==20&&*sound.base().byte(0x85)==1&&sound.loop()&&sound.sound()==(name?*name:std::string{})&&sound.sound_id()==-1&&sound.source_byte39c()==0,"SoundEmitter authored identity/reference/constructor fields differ");
  for(auto pair:{std::pair<const char*,float>{"distMin",sound.distance_min()},{"distMax",sound.distance_max()}}){auto authored=original->source().attribute(pair.first);std::int32_t default_value{};const auto* constant=std::string(pair.first)=="distMin"?"CameraReferenceDistance":"CameraMaxDistance";require(property_sources.constant_integer(property_sources.constant_context,"AudioConstants",constant,default_value,e),e);require(pair.second==(authored?std::strtof(authored->c_str(),nullptr):static_cast<float>(default_value)),"SoundEmitter authored or original dynamic distance default differs");}
  const char* position=original->source().attribute("position")->c_str();for(unsigned axis=0;axis<3;++axis){char* end{};auto expected=static_cast<float>(std::strtod(position,&end));require(sound.base().vector3(0x160)[axis]==expected+captured.module_offset[axis],"SoundEmitter authored placement/module offset lost");position=*end==','?end+1:end;}
  require(!sound.init_post(e)&&e=="Required actual Arrays::Sounds name snapshot"&&sound.sound_id()==-1,"SoundEmitter fabricated audio binding");
 }
 const auto& position_fields=npc->actor->position_fields_v7();require(position_fields.constructed&&!position_fields.physical2dc&&!position_fields.attached2e0&&!npc->actor->source_visual(),"fresh SAME Character pointer fields differ");
 require(std::equal(source_position.begin(),source_position.end(),npc->actor->runtime.controller.destination)&&npc->actor->runtime.object.motion.floor==UINT32_MAX,"whole Character SetPosition reset PF or lost destination");
 for(unsigned i=0;i<6;++i)require(npc->actor->runtime.subobjects.absolute_bounds[i]==npc->actor->runtime.subobjects.local_bounds[i]+source_position[i%3],"whole Character absolute bounds source differs");
 const auto* condition0=containers.front()->receiver().base().string(0x90);const auto* condition1=containers.front()->receiver().base().string(0xb4);
 require(condition0&&condition1,"Required SAME constructor-backed chest condition names");const std::array<std::string,2> authored_conditions{*condition0,*condition1};
 const bool empty_condition_branch=std::all_of(authored_conditions.begin(),authored_conditions.end(),[](const auto& name){return name.empty()||name=="Invalid";});
 const std::string expected_chest_boundary=empty_condition_branch?"OpenableContainer required source service: MeetCondition":"Required actual Arrays::Conditions snapshot";
 const auto rng_calls=application->channel(0).calls;
 require(!containers.front()->factory_receiver().init_post(e)&&e==expected_chest_boundary,"actual Container reached: "+e);
 require(application->channel(0).calls==rng_calls+1&&*containers.front()->receiver().base().integer(0x270)==-2,"Container did not consume/cache shared application RNG");
 require(!containers.front()->factory_receiver().init_post(e)&&application->channel(0).calls==rng_calls+1,"Container replay re-rolled shared RNG");

 const auto chest_error=e;
#ifdef DH2_PREVIEW_ENTITY_EXPORT_V30
 #include "canonical_preview_entity_export_v30.inc"
#endif

 
#include "source_visual_helper_v41_capture.inc"
require(*containers.front()->receiver().base().string(0x290)=="data/3D/GameObjects/go_chest_swamp.bdae"&&containers.front()->receiver().fields().data374>=0,"actual chest row/dictionary was not bound on SAME CString290");for(auto& chest:containers){auto visual=chest->visual();if(chest==containers.front()){require(visual&&visual->ready()&&visual->root_identity()&&*chest->receiver().base().pointer(0x2d8)==reinterpret_cast<std::uintptr_t>(visual.get()),"actual chest visual not attached to SAME base/root");require(!visual->scene().instances.empty()&&chest->receiver().base().runtime().object.user==0,"actual chest scene or genuine null-PF domain differs");}else require(!visual,"unattempted chest visual fabricated");require(chest->release(e),e);}
 // Explicit graph/map fixture teardown, not whole Level/candidate destruction.
// Failed MGP journals remain retained until this test scope is destroyed.
for(auto& m:modules)require(graph->release(m.get(),e),e);
 for(auto& visual:retained_visuals)require(visual->controller_identity()==0&&visual->controller_root_identity()==0&&visual->root_identity()==0,"Module release retained controller/root");
for(unsigned i=0;i<frames.size();++i){
 auto& frame=frames[i];const auto root_id=frame.root_identity;const auto count=frame.meshes.size();
 require(!capture_module_draw_frame_v1(retained_visuals[i],modules[i]->receiver->module_id(),frame,e)&&frame.root_identity==root_id&&frame.meshes.size()==count,"released root capture succeeded or destroyed prior frame");
 for(const auto& draw:frame.meshes){assets::Mesh payload{};require(dh2_mesh_open(&payload,&frame.bres,draw.geometry)==assets::Error::ok,"frame resource pin expired during visual release");}
}
unsigned release_calls=0;require(preparation->discard_after_owner_release([&](std::string& e){++release_calls;for(auto id:map->children())if(!map->remove(id,e))return false;return map->release(e);},e),e);
require(preparation->discard_after_owner_release({},e)&&release_calls==1,"retained cleanup replayed owner release");
require(roots->roots().empty(),"retained roots remained after actual release");
std::vector<ModuleDrawFrameV1> preserved=frames;require(!preparation->capture_draw_frames(preserved,e)&&preserved.size()==frames.size(),"released preparation submitted stale roots");std::cout<<"{\"validation\":\"PASS\",\"original_mlx_declarations\":10,\"actual_modules\":9,\"floor_clones\":16,\"exits\":19,\"generated_zones\":9,\"actual_init_final\":9,\"render_frame_modules\":9,\"render_frame_meshes\":"<<render_meshes<<",\"same_root_controllers\":9,\"controllers_released\":9,\"reserved_null_key0\":true,\"actual_container_constructed\":"<<chest_count<<",\"actual_container_published\":"<<chest_count<<",\"factory_failure_prefix\":"<<std::quoted(source_failed?"empty":"none")<<",\"config_across_rooms_modern_repair\":true,\"room_zone_across_rooms_modern_repair\":true,\"authored_container_properties_verified\":true,\"named_template_context_modern_repair\":true,\"template_assertion_fixture\":true,\"template_context_problem\":false,\"actual_character_constructed\":"<<character_count<<",\"authored_objects_constructed\":"<<supported_objects<<",\"character_authored_fields_verified\":true,\"whole_character_factory_position_verified\":true,\"shared_application_RNG_connected\":true,\"actual_chest_table_dictionary_connected\":true,\"chest_condition_names\":["<<std::quoted(authored_conditions[0])<<","<<std::quoted(authored_conditions[1])<<"],\"chest_condition_source_kernel_connected\":true,\"actual_chest_visual_constructed\":true,\"source_null_user_pf_kernel_connected\":true,\"source_device_predicate_connected\":true,\"device_facts_fixture\":true,\"actual_dummy_constructed\":"<<dummies.size()<<",\"actual_destructible_constructed\":"<<destructibles.size()<<",\"actual_spawn_point_constructed\":"<<spawn_points.size()<<",\"actual_decor_constructed\":"<<decor.size()<<",\"actual_trigger_zone_constructed\":"<<triggers.size()<<",\"actual_animated_decor_constructed\":"<<animated.size()<<",\"animated_decor_whole_init_verified\":false,\"actual_checkpoint_constructed\":"<<checkpoints.size()<<",\"checkpoint_activation_save_verified\":false,\"actual_door_constructed\":"<<doors.size()<<",\"door_whole_init_verified\":false,\"actual_trigger_object_constructed\":"<<trigger_objects.size()<<",\"trigger_object_whole_init_verified\":false,\"actual_exit_zone_constructed\":"<<exit_zones.size()<<",\"exit_zone_transition_verified\":false,\"actual_quest_move_zone_constructed\":"<<quest_moves.size()<<",\"quest_collision_verified\":false,\"actual_sound_emitter_constructed\":"<<sound_emitters.size()<<",\"audio_playback_verified\":false,\"source_container_graph_v21_selected\":true,\"positive_animation_callbacks_verified\":false,\"completed_module_source_pairs\":"<<loaded_source_modules<<",\"all_module_object_sources_complete\":"<<(all_object_sources_complete?"true":"false")<<",\"first_mgp_complete\":true,\"first_missing_file_index\":"<<stopped_file_index<<",\"first_missing_source_type\":"<<std::quoted(stopped_type)<<",\"module_source_error\":"<<std::quoted(module_error)<<",\"container_init_post_missing_service\":"<<std::quoted(chest_error)<<",\"outer_provider_fixtures\":true,\"retained_application_API_verified\":true,\"whole_source_level_ctor_verified\":true,\"source_gate150\":0,\"whole_level_init_verified\":false,\"current_GSLevel_published\":false,\"full_loader_verified\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
