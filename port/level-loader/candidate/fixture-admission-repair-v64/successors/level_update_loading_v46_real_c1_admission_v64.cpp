#include "../../../level_source_loading_update_v46.hpp"
#include "../../../stage_loader_v46_physical.hpp"
#include "canonical_module_graph_v3.hpp"
#include "canonical_openable_graph_v21.hpp"
#include <iomanip>
#include <cstdlib>
#include "../../../module_draw_frame_v1.hpp"
#include "../../../retained_level_module_graph_v1.hpp"
#include "../../../canonical_level_class_dispatch_v1.hpp"
#include "openable_container_data_connection_v11.hpp"
#include "retained_character_position_owner_v7.hpp"
#include "../../../device_pipeline_borrow_v1.hpp"
#include "canonical_dummy_owner_v14.hpp"
#include "canonical_destructible_container_v16.hpp"
#include "canonical_spawn_point_v15.hpp"
#include "../../../canonical_auxiliary_families_v16.hpp"
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
 std::shared_ptr<CanonicalLevelContextV1> level;require(CanonicalLevelContextV1::create({"SWAMP","data/scene/001_swamp.mlx",LevelSourceKindV1::fixed,7},bytes,level,e),e);
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
  require(actual_design->levels()&&actual_design->levels()->level_names.at(41)==level->source_request().identity&&actual_design->levels()->levels.at(41).file=="001_swamp.mlx","Actual original SWAMP logical identity/filename required");
  require(level->construct_source_v3({"worlds/001_swamp.mlx",0,7,1,0,1,0,-1,0},native_ctor->services(),e),e);
  require(level->constructor_owner_v3()->phase()==LevelConstructorPhaseV3::complete&&source_debug_loads==1&&globals.next_module_id==0&&level->source_word150()==0,"whole SAME Level source constructor fields differ");
  

 Stage0LevelBorrowV46 stage0;require(borrow_stage0_level_v46(level,stage0,e),e);auto c1=level->constructor_borrow_v3();
 require(level->source_request().identity=="SWAMP"&&c1.fields->name_f8=="worlds/001_swamp.mlx"&&c1.fields->row3c==41&&c1.fields->seed114==7,"actual C1 logical/raw/row/seed facts differ");
 require(stage0.loading.fields.state130==&c1.fields->field130&&stage0.loading.fields.counter134==&c1.fields->field134&&stage0.loading.fields.current138==&c1.fields->field138&&stage0.file13c==&c1.fields->field13c&&stage0.byte144==&c1.fields->byte144,"stage0 borrowed shadowC1fields");
 const auto script=c1.fields->script44,save=c1.fields->save_ec;
 struct AppFixture{std::uint32_t bigI{9},bigV{11};std::uint8_t byteb4{127};};auto app=std::make_shared<AppFixture>();
 Stage0ApplicationBorrowV46 appborrow{app,app,reinterpret_cast<std::uintptr_t>(app.get()),&app->bigI,&app->bigV,&app->byteb4};
 c1.fields->field134=9;c1.fields->field138=11;c1.fields->field13c=13;c1.fields->byte144=127;
 auto body=stage0_body_v46(stage0,appborrow,{});
 LifecycleServicesV36 stage_services;stage_services.stage_body[0]=std::move(body); // Actual clean/audio bodies intentionally absent.
 auto dispatcher=std::make_shared<LifecycleV36>(stage0.loading.fields,stage0.loading.actual_level_owner,std::vector<std::shared_ptr<const void>>{},std::move(stage_services));
 using Bridge=LevelLoadingUpdateV46<CanonicalLevelContextV1>;std::unique_ptr<Bridge> update;
 LevelLoadProcessProviderV46 provider{level,level->identity(),[dispatcher](std::string& error){const auto status=dispatcher->tick();if(status==LifecycleStatusV36::failed){error=dispatcher->diagnostics().error;return false;}return true;}};
 // Explicit dispatcher-prefix fixture: complete per-call debug/fasttravel
 // prefix is NOT supplied/certified by this focused actual-C1 alias probe.
 require(Bridge::create(level,std::move(provider),update,e),e);
 require(update->update(level,true)==LevelLoadingUpdateResultV46::main_gameplay_required&&update->load_process_calls()==0&&c1.fields->field130==0,"force gameplay gate ran loading");
 require(update->update(level,false)==LevelLoadingUpdateResultV46::failed&&update->load_process_calls()==1&&c1.fields->field130==0&&app->bigI==0&&app->bigV==0&&app->byteb4==127,"actualmissingCleanGlitchprefix differs");
 require(update->error().find("Application.CleanGlitch")!=std::string::npos&&c1.fields->field134==9&&c1.fields->field138==11&&c1.fields->field13c==13&&c1.fields->byte144==127&&c1.fields->script44==script&&c1.fields->save_ec==save,"failedstage0changedsuffixorLuaSaveowners");
 require(update->update(level,false)==LevelLoadingUpdateResultV46::failed&&update->load_process_calls()==1,"actualfailedprefixreplayed");
 // Minimal SAME-C1 preparation binding; no map/class body is entered. Its
 // constructor callback explicitly refuses missing services instead of success.
 auto candidate=std::make_shared<Fixture>(bytes);auto roots=std::make_shared<GameObjectSceneRootRegistryV1>();auto floors=std::make_shared<floors::World>();auto map=std::make_shared<SceneManagerMapOwnerV2>(roots);auto rooms=std::make_shared<ModulePFRoomsV3>(floors,map,ModulePFDebugV3{});
 CanonicalModuleGraphServicesV3 graphservices;graphservices.candidate=candidate;graphservices.rooms=rooms;auto graph=std::make_shared<CanonicalModuleGraphV3>(std::move(graphservices));
 CanonicalClassServicesV1 missingclasses;missingclasses.context=candidate.get();missingclasses.construct=[](void*,const CanonicalFactoryEntryV1&,const CanonicalSourceObjectRequestV1&,CanonicalObjectBorrowV1&,std::string& error){error="Required actual class body; unopened preparation binding fixture";return false;};
 std::shared_ptr<RetainedLevelModuleGraphV1> preparation;require(RetainedLevelModuleGraphV1::create({archive,level,candidate,&candidate->manager,missingclasses,{},graph,floors,rooms},preparation,e),e);
 SourceLoadingInputsV43 inputs;inputs.preparation=preparation;inputs.manager={candidate,&candidate->manager};auto seedfixture=std::make_shared<std::array<std::uint32_t,2>>();inputs.globals={seedfixture,&(*seedfixture)[0],&(*seedfixture)[1]};
 std::unique_ptr<LevelSourceLoadingUpdateV46> composed;require(LevelSourceLoadingUpdateV46::create(level,inputs,{},composed,e),e);
 require(composed->update(level,false)==LevelLoadingUpdateResultV46::failed&&composed->error().find("per-call")!=std::string::npos&&composed->diagnostics().service_calls==0&&c1.fields->field130==0&&preparation->status().stage==RetainedModulePreparationStageV1::idle,"missing authentic prefix invoked dispatcher");
 unsigned prefixcalls=0;SourceLoadProcessPrefixV46 prefix;prefix.actual_provider_owner=bytes;LevelSourceLoadingUpdateV46* active{};
 prefix.before_dispatch=[&](std::uint32_t state,std::string&){++prefixcalls;require(state==0,"prefix received copied phase");require(active->update(level,false)==LevelLoadingUpdateResultV46::failed,"nested composedUpdate accepted");return true;};
 std::unique_ptr<LevelSourceLoadingUpdateV46> nested;require(LevelSourceLoadingUpdateV46::create(level,inputs,prefix,nested,e),e);active=nested.get();
 require(nested->update(level,false)==LevelLoadingUpdateResultV46::failed&&prefixcalls==1&&nested->diagnostics().service_calls==0&&c1.fields->field134==9&&app->byteb4==127,"composed prefix reentry stillran dispatcher");
 // Separate SELECTED5 source-entry fixture, omitted0..4 not certified.
 c1.fields->field130=5;LifecycleBorrowV36 physicalborrow;require(borrow_lifecycle_fields_v36(level,physicalborrow,e),e);auto physical=std::make_shared<dh2::physical::NativeWorld>();auto debugfixture=std::make_shared<int>(1);
 auto stage5=stage5_physical_body_v46(physicalborrow,physical,{debugfixture,[](const char* key,bool& result,std::string&){require(std::string(key)=="isTracingLevel_Loading","sourcekeydiffers");result=false;return true;}});
 require(stage5(e)==LifecycleStepV36::complete&&physical->backend()&&c1.fields->field130==5,"realC1selectedstage5providerfailed");
 std::cout<<"{\"validation\":\"PASS\",\"real_c1_pointer_aliases\":5,\"logical_identity\":\"SWAMP\",\"raw_filename\":\"worlds/001_swamp.mlx\",\"seed\":7,\"source_row\":41,\"actual_loading_branch_missing_stage0_clean_glitch\":true,\"actual_source_prefix_stores\":2,\"unreached_suffix_preserved\":5,\"same_script_save_owners\":true,\"composed_missing_prefix_stops_before_dispatch\":true,\"composed_prefix_reentry_stops_before_dispatch\":true,\"preparation_body_not_entered\":true,\"selected_stage5_fixture_real_physics\":true,\"whole_load_process_prefix_verified\":false,\"whole_level_init_verified\":false,\"gameplay_ready\":false}\n";return 0;
 }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
