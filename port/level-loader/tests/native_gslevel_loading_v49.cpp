#include "../native_gslevel_loading_connection_v49.hpp"
#include "../stage_loader_v46_early.hpp"
#include "character_design_services.hpp"
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

struct CaseResultV49 {unsigned rejections{},menus{},prefix_calls{},debug_file_attempts{};bool source_global_prefix{};};
static CaseResultV49 run_case(unsigned scenario,assets::ZipAssetPackV1 archive,CharacterTables& tables,const char* directory){
 require(scenario<2,"bad source scenario");std::string e;CaseResultV49 result;bool found=false;auto bytes=std::make_shared<std::vector<std::uint8_t>>();require(archive.read("data/3D/Modules/Swamp/swamp.bdae",found,*bytes,e)&&found,e);
 ModuleRuntimeGlobalsV1 module_globals;module_globals.next_module_id=99;std::uint32_t source_debug_loads=0;
 auto design=std::make_shared<character::CharacterGameDesign::Borrow>(tables.design.borrow());auto files=std::make_shared<NativeCtorFilesFixtureV24>(archive,directory);
 auto lua=std::make_shared<scripts::LuaScriptCacheOwnerV13>(scripts::LuaScriptCacheServicesV13{files,files.get(),NativeCtorFilesFixtureV24::script});
 LevelConstructorApplicationV4 app;app.owner=design;app.debug_level_load_count=&source_debug_loads;app.module_id_global=&module_globals.next_module_id;app.levels=design->levels();app.lua_cache=lua;app.lua.owner=design;app.lua.design=*design->design();app.private_vm_limit=16u*1024u*1024u;
 app.saves.files.context=files.get();app.saves.files.read_file=NativeCtorFilesFixtureV24::save;app.saves.files.storage_lease=files;app.online_byte5=[](std::uint8_t& out,std::string&){out=0;return true;}; // Explicit offline transport fixture.
 LevelSourceRequestV1 request;request.identity="SWAMP";request.definition="data/scene/001_swamp.mlx";request.seed=7;const std::string raw="worlds/001_swamp.mlx";GSLevelArgumentsV2 arguments{raw,0,7,1,0,1,0,-1,0};
 auto globals=std::make_shared<NativeGSLevelGlobalsV27>();auto gs=std::make_shared<NativeGSLevelRuntimeV27>(globals);require(gs->prepare_loading({},e),e);auto loading=gs->loading_services();std::shared_ptr<CanonicalLevelContextV1> level;
 std::vector<std::string> outer_trace;unsigned flushes=0,pushes=0,progress=0,unloads=0,destroys=0;
 GSLevelServicesV2<CanonicalLevelContextV1> outer;
 // Actual outer GS algorithm/C1/native Loading kernels. Below animation/menu/
 // online/deepUnload/Destroy are explicit transports, not whole engine bodies.
 outer.flush_animation_sets=[&](std::string&){require(!globals->s_level,"GS publication before flush");++flushes;outer_trace.push_back("flush");return true;};
 outer.get_menu=[&](const char* name,std::uintptr_t& out,std::string&){require(globals->s_level&&globals->s_level==gs->fields().level34,"menu saw foreign current");outer_trace.push_back(name);if(std::string(name)=="menu_Loading")out=17;else{require(std::string(name)=="menu_HUD_0","wrong Dtor menu");out=0;}return true;};
 outer.online_byte5=[](std::uint8_t& out,std::string&){out=0;return true;};
 outer.push_menu=[&](std::uintptr_t id,std::string&){require(id==17,"loading menu ID changed");++pushes;return true;};
 outer.menu_render_fx=[](std::uintptr_t id,std::uintptr_t& out,std::string&){require(id==17,"loading renderFX changed");out=23;return true;};
 outer.check_menu_weak_proxy=[](std::uintptr_t id,std::string&){require(id==17,"loading weak proxy changed");return true;};
 outer.menu_character=[](std::uintptr_t id,std::uintptr_t& out,std::string&){require(id==17,"loading character changed");out=31;return true;};
 outer.invoke_as_no_arguments=[&](std::uintptr_t fx,std::uintptr_t character,const char* method,std::string& error){require(fx==23&&character==31&&std::string(method)=="onProgress","loading AS callback changed");CanonicalCurrentLevelBorrowV1 current;require(gs->current(current,error)&&current&&current.level()==globals->s_level,error);require(current.level()->constructor_owner_v3()->phase()==LevelConstructorPhaseV3::complete&&current.level()->constructor_fields_v3().field130==0,"AS progress before genuine C1");std::int32_t value{};require(ui::loading_menu_read_progress_v1(loading,value,error)&&value==0,error);++progress;return true;};
 outer.unload_level=[&](const auto& same,std::string&){require(same==level&&same==globals->s_level,"Dtor Unload receiver changed");++unloads;outer_trace.push_back("unload");return true;};
 outer.destroy_level=[&](const auto& same,std::string&){require(same==level&&same==globals->s_level,"Dtor Destroy cleared current early");++destroys;outer_trace.push_back("destroy");return true;};
 auto appgate=std::make_shared<std::uint8_t>(0);auto menu=std::make_shared<int>(1);GSApplicationBorrowV44 appborrow{appgate,appgate.get(),nullptr,nullptr};
 GSLevelUpdateServicesV44<CanonicalLevelContextV1> services;services.menu_instance=[menu](GSMenuBorrowV44& out,std::string&){out={menu,reinterpret_cast<std::uintptr_t>(menu.get())};return true;};
 services.menu_update=[&](const auto& same,bool flag,std::string&){require(same.actual_owner==menu&&flag&&globals->s_level==level,"GS menu/current identity mismatch");++result.menus;return true;};
 using Connection=NativeGSLevelLoadingConnectionV49;std::unique_ptr<Connection> connection;
 require(!Connection::create(gs,appborrow,services,{}, {},{},connection,e)&&!connection&&!globals->s_level,"unconstructedGS published loader");++result.rejections;
 require(gs->construct(request,arguments,app,outer,{},e),e);level=globals->s_level;
 require(level&&level==gs->fields().level34&&gs->fields().loading38==1&&gs->fields().active3c==1,"actualGS C1 publication changed");const auto& c1=level->constructor_fields_v3();
 require(c1.row3c==41&&c1.seed114==7&&c1.name_f8==raw&&c1.field130==0&&level->source_request().identity=="SWAMP"&&level->source_request().seed==7,"actualrow/seed/raw/logicalNAME changed");
 require(flushes==1&&pushes==1&&progress==1&&source_debug_loads==1&&module_globals.next_module_id==0&&level->source_word150()==0,"GS/C1 source prefix replayed");
 const auto actual_save=gs->connection()->level_connection()->bindings()->save();const auto actual_script=gs->connection()->level_connection()->bindings()->script();
 require(files->script_reads==2&&files->save_reads==2&&lua->cached_count()==2&&actual_save&&actual_script&&actual_save.get()==c1.save_ec.get()&&actual_script.get()==c1.script44.get()&&actual_save->owner().ready(),"actualC1 Lua/Save receivers missing");
 auto candidate=std::make_shared<Fixture>(bytes);require(candidate->manager.source_init_phase7c_v38()==0&&candidate->manager.source_map_size1c_v38()==1,"actualmanager C1phase/nullmap defaults changed");
 auto roots=std::make_shared<GameObjectSceneRootRegistryV1>();auto floors=std::make_shared<floors::World>();auto map=std::make_shared<SceneManagerMapOwnerV2>(roots);auto rooms=std::make_shared<ModulePFRoomsV3>(floors,map,ModulePFDebugV3{});
 CanonicalModuleGraphServicesV3 graph_services;graph_services.candidate=candidate;graph_services.rooms=rooms;auto graph=std::make_shared<CanonicalModuleGraphV3>(std::move(graph_services));
 CanonicalClassServicesV1 classes;classes.context=candidate.get();classes.construct=[](void*,const CanonicalFactoryEntryV1&,const CanonicalSourceObjectRequestV1&,CanonicalObjectBorrowV1&,std::string& error){error="Required actual class provider; unopened source-entry fixture";return false;};
 std::shared_ptr<RetainedLevelModuleGraphV1> preparation;require(RetainedLevelModuleGraphV1::create({archive,level,candidate,&candidate->manager,classes,{},graph,floors,rooms},preparation,e),e);
 SourceLoadingInputsV43 inputs;inputs.preparation=preparation;inputs.manager={candidate,&candidate->manager};auto source_seeds=std::make_shared<std::array<std::uint32_t,2>>();inputs.globals={source_seeds,&(*source_seeds)[0],&(*source_seeds)[1]}; // Unreached original static-global transport fixture.
 struct StageAppFixture {std::uint32_t bigI{9},bigV{11};std::uint8_t byteb4{127};};auto stageapp=std::make_shared<StageAppFixture>();Stage0LevelBorrowV46 stage0;require(borrow_stage0_level_v46(level,stage0,e),e);
 require(stage0.loading.fields.state130==&c1.field130&&stage0.loading.fields.counter134==&c1.field134&&stage0.loading.fields.current138==&c1.field138&&stage0.file13c==&c1.field13c&&stage0.byte144==&c1.byte144,"loadingwrapper borrowed shadowC1");
 inputs.external.stage_body[0]=stage0_body_v46(stage0,{stageapp,stageapp,reinterpret_cast<std::uintptr_t>(stageapp.get()),&stageapp->bigI,&stageapp->bigV,&stageapp->byteb4},{}); // Real CleanGlitch/Vox missing.
 SourceLoadProcessPrefixV46 prefix;
 struct DebugFixture {struct Delete {void operator()(character::DebugSwitches* value)const{dh2_character_debug_destroy(value);}};std::unique_ptr<character::DebugSwitches,Delete> source{dh2_character_debug_create()};unsigned attempts{};static int open(void* raw,const char* name,std::uintptr_t* handle){auto& d=*static_cast<DebugFixture*>(raw);require(name&&std::string(name)=="DebugSwitches.savegame"&&handle,"wrong Debug file");++d.attempts;*handle=0;return 0;}static int close(void*,std::uintptr_t){throw std::runtime_error("Debug fixture never opened a file");}character::DebugFileServices24 files{this,open,close};};
 auto debug=std::make_shared<DebugFixture>();require(bool(debug->source),"actual Debug constructor failed");
 if(scenario==1){
  prefix.actual_provider_owner=debug;
  // Explicit flag0 and logging/menu transport fixture. Debug load/query itself
  // uses the genuine retained missing-file implementation, not forced false.
  // Whole original per-call prefix/cheat/logger bodies are not certified.
  prefix.before_dispatch=[&,debug](std::uint32_t state,std::string& error){++result.prefix_calls;require(state==*stage0.loading.fields.state130&&state==0,"prefix detached phase");std::uint32_t enabled{};if(dh2_character_debug_load(debug->source.get(),&debug->files)!=1||dh2_character_debug_get(&enabled,debug->source.get(),"IsDisplayLoadingStepName",&debug->files)!=1){error="actual missing-file Debug kernel failed";return false;}require(enabled==0,"missing-file Debug invented true");return true;};
 }
 require(Connection::create(gs,appborrow,services,inputs,prefix,{},connection,e),e);auto* kept=connection.get();
 const auto script=c1.script44,save=c1.save_ec;const auto phase=c1.field130,counter=c1.field134,current=c1.field138,file=c1.field13c;const auto byte144=c1.byte144;
 auto conflict=services;conflict.level_update=[](const auto&,bool,std::string& error){error="conflicting test callback";return false;};
 require(!Connection::create(gs,appborrow,conflict,inputs,prefix,{},connection,e)&&connection.get()==kept&&globals->s_level==level,"preexistingLevelUpdate replaced loader");++result.rejections;
 conflict=services;conflict.borrow_level=borrow_gslevel_read_v44<CanonicalLevelContextV1>;
 require(!Connection::create(gs,appborrow,conflict,inputs,prefix,{},connection,e)&&connection.get()==kept,"preexistingC1borrow replaced loader");++result.rejections;
 conflict=services;conflict.level_load=actual_level_load_v44<CanonicalLevelContextV1>;
 require(!Connection::create(gs,appborrow,conflict,inputs,prefix,{},connection,e)&&connection.get()==kept,"preexistingLevelLoad replaced loader");++result.rejections;
 require(!Connection::create({},appborrow,services,inputs,prefix,{},connection,e)&&connection.get()==kept,"nullGS replaced loader");++result.rejections;
 auto foreign_gs=std::make_shared<NativeGSLevelRuntimeV27>(globals);
 require(!Connection::create(foreign_gs,appborrow,services,inputs,prefix,{},connection,e)&&connection.get()==kept&&globals->s_level==level,"foreign unconstructedGS read globals as fields");++result.rejections;
 auto foreign_inputs=inputs;auto foreign_manager=std::make_shared<Fixture>(bytes);foreign_inputs.manager={foreign_manager,&foreign_manager->manager};
 require(!Connection::create(gs,appborrow,services,foreign_inputs,prefix,{},connection,e)&&connection.get()==kept,"foreignmanager replaced loading composition");++result.rejections;
 std::shared_ptr<CanonicalLevelContextV1> foreign_level;require(CanonicalLevelContextV1::create({"FOREIGN","data/scene/001_swamp.mlx"},bytes,foreign_level,e),e);std::shared_ptr<RetainedLevelModuleGraphV1> foreign_preparation;
 require(RetainedLevelModuleGraphV1::create({archive,foreign_level,candidate,&candidate->manager,classes,{},graph,floors,rooms},foreign_preparation,e),e);foreign_inputs=inputs;foreign_inputs.preparation=foreign_preparation;
 require(!Connection::create(gs,appborrow,services,foreign_inputs,prefix,{},connection,e)&&connection.get()==kept&&c1.field130==phase,"foreign prepC1 replaced actual GS Level");++result.rejections;
 auto conflicting_source=inputs;conflicting_source.external.stage_body[7]=[](std::string& error){error="conflicting original source provider";return LifecycleStepV36::failed;};
 require(!Connection::create(gs,appborrow,services,conflicting_source,prefix,{},connection,e)&&connection.get()==kept,"conflictingsource7 overwritten silently");++result.rejections;
 require(c1.field130==phase&&c1.field134==counter&&c1.field138==current&&c1.field13c==file&&c1.byte144==byte144&&stageapp->bigI==9&&stageapp->bigV==11&&connection->loading_diagnostics().service_calls==0,"createguards changed actualC1sourceprefix");
 require(connection->tick()==GSLevelUpdateResultV44::advanced&&gs->fields().loading38==2&&c1.field130==0&&result.menus==1,"actualGS frame1->2 ran stage0");
 require(connection->tick()==GSLevelUpdateResultV44::advanced&&gs->fields().loading38==3&&c1.field130==0&&result.menus==2,"actualGS frame2 Load0 differs");
 require(connection->tick()==GSLevelUpdateResultV44::failed&&connection->required_service()=="Level.Update(false)"&&gs->fields().loading38==3&&c1.field130==0&&result.menus==2,"missing actual loading service did not stopGS3");
 const auto failure=connection->error();
 if(!scenario)require(failure.find("per-call")!=std::string::npos&&result.prefix_calls==0&&connection->loading_diagnostics().service_calls==0&&stageapp->bigI==9&&stageapp->bigV==11,"missing authentic prefix entered stage0");
 else require(failure.find("Application.CleanGlitch")!=std::string::npos&&result.prefix_calls==1&&connection->loading_diagnostics().service_calls==1&&stageapp->bigI==0&&stageapp->bigV==0&&debug->attempts==1,"genuine stage0 prefix/missingCleanup differs");
 require(connection->tick()==GSLevelUpdateResultV44::failed&&connection->error()==failure&&result.prefix_calls==scenario&&result.menus==2&&source_debug_loads==1,"failedGSsourceprefix replayed");
 CanonicalCurrentLevelBorrowV1 borrowed;require(gs->current(borrowed,e)&&borrowed&&borrowed.level()==level&&borrowed.kill_level()==level->kill_level(),e);
 require(c1.field134==counter&&c1.field138==current&&c1.field13c==file&&c1.byte144==byte144&&c1.phase30==0&&stageapp->byteb4==127&&c1.script44==script&&c1.save_ec==save&&preparation->status().stage==RetainedModulePreparationStageV1::idle&&candidate->manager.source_init_phase7c_v38()==0&&candidate->manager.source_map_size1c_v38()==1,"failedloaderchangedunreachedsuffix/manager/graph");
 require(gs->destroy(e)&&!globals->s_level&&!gs->fields().level34&&unloads==1&&destroys==1,e); // Explicit deeper cleanup fixture only.
 require(outer_trace==std::vector<std::string>({"flush","menu_Loading","unload","menu_HUD_0","destroy"}),"actualGSouterDtororder changed");
 require(!Connection::create(gs,appborrow,services,inputs,prefix,{},connection,e)&&connection.get()==kept,"destroyedGS replaced preservedconnection");++result.rejections;
 result.debug_file_attempts=debug->attempts;result.source_global_prefix=scenario==1;
 std::weak_ptr<NativeGSLevelRuntimeV27> weak=gs;foreign_gs.reset();gs.reset();require(!weak.expired(),"connection sibling did not retainGS");connection.reset();require(weak.expired(),"GS/loading/frame introduced selfcycle");return result;
}
int main(int argc,char** argv){if(argc!=4)return 2;try{auto archive=pack(argv[1]);CharacterTables tables(archive,argv[2]);auto missing_prefix=run_case(0,archive,tables,argv[3]);auto missing_clean=run_case(1,archive,tables,argv[3]);
 std::cout<<"{\"validation\":\"PASS\",\"composition\":\"NativeGSLevelLoadingConnectionV49\",\"actual_gs_cases\":2,\"source_publication_before_menu\":true,\"logical_identity\":\"SWAMP\",\"raw_name\":\"worlds/001_swamp.mlx\",\"row3c\":41,\"seed114\":7,\"frame_states\":[1,2,3],\"same_c1_loading_pointer_aliases_per_case\":5,\"missing_per_call_prefix_stops_before_stage0\":true,\"missing_clean_glitch_preserves_original_global_prefix\":true,\"genuine_debug_missing_file_attempts\":"<<missing_clean.debug_file_attempts<<",\"create_rejections\":"<<missing_prefix.rejections+missing_clean.rejections<<",\"menu_updates\":"<<missing_prefix.menus+missing_clean.menus<<",\"source_manager_phase7c\":0,\"state130\":0,\"same_lua_save_owners\":true,\"same_global_cleared_by_outer_dtor\":true,\"sibling_cycle_free\":true,\"flag_logging_menu_deep_cleanup_transports_are_fixtures\":true,\"whole_load_process_prefix_verified\":false,\"source_abort_teardown_verified\":false,\"whole_level_init_verified\":false,\"gameplay_ready\":false}\n";return 0;}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
