#include "../level_source_loading_v43.hpp"
#include "../stage_loader_v38_root_file.hpp"
#include "../stage_loader_v38_stage10.hpp"
#include "../stage_loader_v39_file_counter.hpp"
#include "../stage_loader_v40_root_cache.hpp"
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
 CanonicalPropertyMapV1 properties(property_sources);auto fixture_owner=std::make_shared<Fixture>(bytes);auto& fixture=*fixture_owner;unsigned zone_pf=0;
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
CanonicalLevelClassDispatchV1 class_dispatch(std::move(class_input));
loader::CanonicalReceiverTransportV1 transport(properties,{level,&class_dispatch,[](void* p,const CanonicalFactoryEntryV1& f,const CanonicalSourceObjectRequestV1& q,CanonicalClassReceiverV1& r,std::string& e){return static_cast<CanonicalLevelClassDispatchV1*>(p)->construct(f,q,r,e);},nullptr});auto dispatch=transport.services();
// No copied level-specific declarations: feed the original complete MLX walk.
std::shared_ptr<RetainedLevelModuleGraphV1> preparation;
require(RetainedLevelModuleGraphV1::create({archive,level,bytes,&fixture.manager,dispatch,{[](bool ok,std::string&){return ok;},[](std::string&){return true;}},graph,floors,rooms},preparation,e),e);
const auto& source=preparation->root_file();
require(preparation->level()==level&&&preparation->manager()==&fixture.manager&&preparation->floor_world()==floors,"retained API replaced an authoritative owner");


 // C1 aliases remain the only state authority. This probe explicitly selects
 // entry7 and then entry10; omitted0..6/8/9 are never called successful.
 Stage7FieldsV38 stage7_fields;require(borrow_stage7_fields_v38(level,stage7_fields,e),e);auto fields=stage7_fields.level.fields;
 require(*fields.state130==0&&*fields.counter134==0&&*fields.current138==0,"real C1 load defaults differ");
 FileCounterBorrowV39 file_counter;require(borrow_file_counter_v39(level,file_counter,e),e);
 require(file_counter.state130==fields.state130&&*file_counter.file13c==0,"counter borrow replaced C1 authority");
 const auto raw=*stage7_fields.name_f8;const std::vector<RootCacheAliasV40> aliases{{"worlds/","data/scene/"}};
 RootCacheResolutionV40 resolved;require(resolve_root_cache_v40(archive,raw,aliases,resolved,e),e);
 require(resolved.raw_name==raw&&resolved.cache_uri=="data/scene/001_swamp.mlx","root raw provenance differs");
 RootCacheResolutionV40 untouched{"sentinel","sentinel","sentinel"};
 require(!resolve_root_cache_v40(archive,"worlds/does_not_exist_v40.mlx",aliases,untouched,e)&&untouched.raw_name=="sentinel","missing archive alias fabricated success");
 require(!resolve_root_cache_v40(archive,"worlds/../001_swamp.mlx",aliases,untouched,e),"root alias accepted traversal");
 auto source_globals=std::make_shared<std::array<std::uint32_t,2>>(); // Explicit platform-global transport fixture.
 SourceLoadingInputsV43 base;base.preparation=preparation;base.manager={fixture_owner,&fixture.manager};base.globals={source_globals,&(*source_globals)[0],&(*source_globals)[1]};base.filename_provider=native_files;
 unsigned filename_calls=0,progress_callbacks=0,condition_calls=0;
 base.resolve_filename=[&](const std::string& actual,std::string& uri,std::string& error){++filename_calls;RootCacheResolutionV40 result;if(!resolve_root_cache_v40(archive,actual,aliases,result,error))return false;require(actual==raw&&result.raw_name==raw,"runtime detached raw filename");uri=result.cache_uri;return true;};
 base.external.publish_progress=[&](std::int32_t state,std::int32_t progress,std::string&){++progress_callbacks;require(state==std::int32_t(*fields.state130)&&progress==std::int32_t(*fields.progress30),"progress callback copied source authority");return true;}; // Explicit standalone AS callback observer.
 base.object_services.make_handle=[](const CanonicalObjectBorrowV1* object,target_providers::Handle16& handle,std::string& error){if(!object){handle={};return true;}if(!object->shared_handle){error="Required same actual Handle fields";return false;}handle=*object->shared_handle;return true;};
 base.object_services.resolve=[&](auto& handle,bool refresh,const auto*& object,std::string& error){return fixture.manager.resolve_handle_v4(handle,refresh,object,{},error);};
 base.object_services.test_enable_condition=[&](const CanonicalObjectBorrowV1& object,bool mark,std::string& error){
  ++condition_calls;require(!mark,"original phase3 condition mark changed");
  const auto& status=preparation->status();require(status.loaded==9&&status.object_sources_complete&&!status.floors_prepared&&!status.geometry_prepared,"generic InitPost/condition preceded all Module sources");return Fixture::condition(&fixture,object,mark,error);
 };
 // Conflict rejection is atomic: keep an existing output and all C1 cells.
 std::unique_ptr<SourceLoadingV43> runtime;require(SourceLoadingV43::create(base,runtime,e),e);auto* saved=runtime.get();
 require(*fields.state130==0&&*fields.current138==0&&fixture.manager.source_map_size1c_v38()==1,"runtime create changed C1 fields/registry");
 unsigned rejected=0;for(unsigned which=0;which<7;++which){auto conflict=base;
  auto body=[](std::string&){return LifecycleStepV36::complete;};
  switch(which){case 0:conflict.external.stage_body[7]=body;break;case 1:conflict.external.stage_body[10]=body;break;case 2:conflict.external.after_source_increment[7]=[](std::string&){return true;};break;
  case 3:conflict.procedural.root_file_step=[](const std::string&,const char*,std::string&){return LifecycleStepV36::complete;};break;
  case 4:conflict.procedural.increment_actual_file13c=[](std::string&){return true;};break;
  case 5:conflict.object_services.load_module=[](std::uintptr_t,std::string&){return LifecycleStepV36::complete;};break;
  case 6:conflict.object_services.init_post=[](const CanonicalObjectBorrowV1&,std::string&){return true;};break;}
  require(!SourceLoadingV43::create(std::move(conflict),runtime,e)&&runtime.get()==saved&&e=="Source7/10 bindings must have one authoritative provider","conflicting provider replaced output/prefix");++rejected;
 }
 auto foreign_owner=std::make_shared<Fixture>(bytes);auto foreign=base;foreign.manager={foreign_owner,&foreign_owner->manager};
 require(!SourceLoadingV43::create(std::move(foreign),runtime,e)&&runtime.get()==saved&&*fields.state130==0,"foreign manager replaced C1 runtime");++rejected;runtime.reset();
 // Lease-only cancellation fixtures. These callbacks test ownership policy;
 // they are not a proof of actual Level abort/Unload/save or actor destruction.
 unsigned cleanup_calls=0;auto token=std::make_shared<unsigned>(1);std::weak_ptr<unsigned> weak=token;auto cancellation=base;cancellation.resource_pins.push_back(token);
 cancellation.external.cancel_and_unload=[&](std::string&){return ++cleanup_calls==1?LifecycleStepV36::pending:LifecycleStepV36::complete;};
 require(SourceLoadingV43::create(std::move(cancellation),runtime,e),e);token.reset();runtime->request_cancel();
 require(runtime->tick()==LifecycleStatusV36::cancelling&&!weak.expired()&&runtime->diagnostics().owned_pins>0,"pending cleanup released resource lease");
 require(runtime->tick()==LifecycleStatusV36::cancelled&&weak.expired()&&runtime->diagnostics().owned_pins==0&&*fields.state130==0,"completed cleanup retained source lease or changed C1 state");runtime.reset();
 auto failed_token=std::make_shared<unsigned>(2);std::weak_ptr<unsigned> failed_weak=failed_token;auto failed_cleanup=base;failed_cleanup.resource_pins.push_back(failed_token);
 failed_cleanup.external.cancel_and_unload=[](std::string& error){error="Required actual incomplete-load abort provider";return LifecycleStepV36::failed;};
 require(SourceLoadingV43::create(std::move(failed_cleanup),runtime,e),e);failed_token.reset();runtime->request_cancel();
 require(runtime->tick()==LifecycleStatusV36::failed&&!failed_weak.expired()&&runtime->diagnostics().owned_pins>0,"failed cleanup dropped actual resource pins");runtime.reset();require(failed_weak.expired(),"runtime destructor retained failed-cleanup token");
 // Select source7 fixture on actual C1 word BEFORE runtime construction.
 require(*stage7_fields.procedural_e8==0,"fixed SWAMP unexpectedly procedural");*fields.state130=7;
 require(SourceLoadingV43::create(base,runtime,e)&&*fields.state130==7,e);
 for(unsigned n=0;n<1000&&*fields.state130==7;++n)require(runtime->tick()==LifecycleStatusV36::loading,runtime->diagnostics().error);
 require(*fields.state130==8&&*file_counter.file13c==1&&filename_calls==1,"actual source7/130then13c hook or resolver replay differs");
 require(*fields.current138==500&&(*source_globals)[0]==*stage7_fields.party_dc&&(*source_globals)[1]==*stage7_fields.word_e0,"actual source7 scalar prefix differs");
 require(runtime->tick()==LifecycleStatusV36::failed&&runtime->diagnostics().failed_state==8&&*fields.state130==8,"absent real stage8 did not stop authentic runtime");
 const auto stage8_error=runtime->diagnostics().required_service;runtime.reset();
 require(source.attempts().size()==10&&preparation->status().modules==9&&config_queries==1&&*level_fields.config38,"SWAMP root factory/early config prefix differs");
 require(fixture.manager.source_map_size1c_v38()==11&&fixture.manager.source_count50()==10&&fixture.manager.source_init_phase7c_v38()==0,"real root map1c or manager phase differs");
 require(!preparation->status().floors_prepared&&!preparation->status().geometry_prepared&&preparation->status().initialized==0,"root silently ran later InitPost/floor/geometry");
 const auto initial_map=fixture.manager.source_map_size1c_v38();
 // Separate source10 fixture: explicitly omit actual script/fog/light bodies8/9.
 *fields.state130=10;require(SourceLoadingV43::create(base,runtime,e)&&*fields.state130==10,e);
 LifecycleStatusV36 result=LifecycleStatusV36::loading;for(unsigned n=0;n<1000&&result==LifecycleStatusV36::loading;++n)result=runtime->tick();
 require(result==LifecycleStatusV36::failed&&runtime->diagnostics().failed_state==10&&fixture.manager.source_init_phase7c_v38()==3,"source10 phase3 must preserve actual first missing service");
 const auto& status=preparation->status();const auto error=runtime->diagnostics().error;
 require(error=="OpenableContainer required source service: MeetCondition"&&condition_calls==10&&status.initialized==9,"first ordered per-class failure differs");
 require(*fields.current138==initial_map&&initial_map==11&&*fields.state130==10,"stage10 replayed real map1c counter or advanced failed state");
 require(status.loaded==9&&status.object_sources_complete&&!status.floors_prepared&&!status.geometry_prepared&&status.finalized==0,"source API omitted journal or ran Floor11/geometry17 early");
 auto relay=preparation->module_files();unsigned authored=0,characters=0;
 for(std::size_t f=0;f<relay->file_count();++f){require(relay->file(f).completed(),"original MGP/MVP incomplete");for(const auto& attempt:relay->file(f).attempts()){
  require(attempt->factory_attempt()&&attempt->factory_attempt()->stage()==CanonicalFactoryStageV1::complete,"authored factory incomplete");++authored;const auto* kind=attempt->source().entry().source().attribute("gametype");if(kind&&(*kind=="Character"||*kind=="Player"))++characters;
 }}require(relay->file_count()==18&&authored==195&&characters==50,"pre-InitPost actual SWAMP source inventory differs");
 const auto count=fixture.manager.source_count50(),map_count=fixture.manager.source_map_size1c_v38(),callbacks=condition_calls;
 require(runtime->tick()==LifecycleStatusV36::failed&&runtime->diagnostics().error==error&&fixture.manager.source_count50()==count&&fixture.manager.source_map_size1c_v38()==map_count&&condition_calls==callbacks,"failed source10 replayed constructor/InitPost prefix");
 require(!runtime->diagnostics().whole_level_init_verified&&!runtime->diagnostics().gameplay_ready,"bounded source entry claimed whole initialization");
 std::cout<<"{\"validation\":\"PASS\",\"composition\":\"SourceLoadingV43\",\"atomic_create_rejections\":"<<rejected<<",\"cancellation_transport_cases\":2,\"source_abort_teardown_verified\":false,\"selected_source_entry_fixture_states\":[7,10],\"omitted_source_bodies\":[0,1,2,3,4,5,6,8,9],\"raw_name\":\""<<raw<<"\",\"cache_uri\":\""<<resolved.cache_uri<<"\",\"root_factories\":10,\"module_sources\":"<<status.loaded<<",\"authored_factories\":"<<authored<<",\"character_factories\":"<<characters<<",\"source_file13c\":"<<*file_counter.file13c<<",\"state130\":"<<*fields.state130<<",\"progress30\":"<<*fields.progress30<<",\"manager_phase7c\":"<<fixture.manager.source_init_phase7c_v38()<<",\"initial_map1c\":"<<initial_map<<",\"map1c_at_stop\":"<<map_count<<",\"successful_class_conditions\":"<<condition_calls<<",\"stage8_required_service\":\""<<stage8_error<<"\",\"error\":\""<<error<<"\",\"floor11_ran\":false,\"geometry17_ran\":false,\"whole_level_init_verified\":false,\"gameplay_ready\":false}\n";
 return 0;
 }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
