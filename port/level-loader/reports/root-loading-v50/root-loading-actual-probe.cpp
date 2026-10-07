#include "native_driver_unused_v50.hpp"
#include "canonical_gameobject_base_owner_v1.hpp"
#include "character_game_design.hpp"
#include "loot_tables_v2.hpp"
#include "native_root_loading_connection_v50.hpp"
#include "character_design_services.hpp"
#include "canonical_module_graph_v3.hpp"
#include <iomanip>
#include <cstdlib>
#include "retained_level_module_graph_v1.hpp"
#include "object_enable_condition_v2.hpp"
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
 require(scenario<3,"bad source scenario");std::string e;CaseResultV49 result;bool found=false;auto bytes=std::make_shared<std::vector<std::uint8_t>>();require(archive.read("data/3D/Modules/Swamp/swamp.bdae",found,*bytes,e)&&found,e);
 ModuleRuntimeGlobalsV1 module_globals;module_globals.next_module_id=99;std::uint32_t source_debug_loads=0;
 auto design=std::make_shared<character::CharacterGameDesign::Borrow>(tables.design.borrow());auto files=std::make_shared<NativeCtorFilesFixtureV24>(archive,directory);
 auto lua=std::make_shared<scripts::LuaScriptCacheOwnerV13>(scripts::LuaScriptCacheServicesV13{files,files.get(),NativeCtorFilesFixtureV24::script});
 LevelConstructorApplicationV4 app;app.owner=design;app.debug_level_load_count=&source_debug_loads;app.module_id_global=&module_globals.next_module_id;app.levels=design->levels();app.lua_cache=lua;app.lua.owner=design;app.lua.design=*design->design();app.private_vm_limit=16u*1024u*1024u;
 app.saves.files.context=files.get();app.saves.files.read_file=NativeCtorFilesFixtureV24::save;app.saves.files.storage_lease=files;app.online_byte5=[](std::uint8_t& out,std::string&){out=0;return true;}; // Explicit offline transport fixture.
 LevelSourceRequestV1 request;request.identity="SWAMP";request.definition="data/scene/001_swamp.mlx";request.seed=7;const std::string raw="001_swamp.mlx";GSLevelArgumentsV2 arguments{raw,0,7,1,0,1,0,-1,0};
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
 using Connection=NativeRootLoadingConnectionV50;std::unique_ptr<Connection> connection;
 require(!Connection::create({},connection,e)&&!connection&&!globals->s_level,"unconstructedGS published loader");++result.rejections;
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

 NativeRootLoadingInputsV50 input;input.gs=gs;input.globals=globals;input.application=appborrow;input.frame=services;input.source=inputs;
 require(candidate->manager.borrow_fresh_source_v50(inputs.manager,input.manager_freshness,e),e);
 input.stage0_application={stageapp,stageapp,reinterpret_cast<std::uintptr_t>(stageapp.get()),&stageapp->bigI,&stageapp->bigV,&stageapp->byteb4};
 auto cheat=std::make_shared<std::uint8_t>(0);
 struct DebugFixture {struct Delete {void operator()(character::DebugSwitches* value)const{dh2_character_debug_destroy(value);}};std::unique_ptr<character::DebugSwitches,Delete> source{dh2_character_debug_create()};unsigned attempts{};static int open(void* raw,const char* name,std::uintptr_t* handle){auto& d=*static_cast<DebugFixture*>(raw);require(name&&std::string(name)=="DebugSwitches.savegame"&&handle,"wrong Debug file");++d.attempts;*handle=0;return 0;}static int close(void*,std::uintptr_t){throw std::runtime_error("Debug fixture never opened a file");}character::DebugFileServices24 files{this,open,close};};
 auto debug=std::make_shared<DebugFixture>();require(bool(debug->source),"actual Debug constructor failed");
 input.prefix.actual_application_owner=appgate;input.prefix.cheat_global_owner=cheat;input.prefix.debug_owner=debug;
 if(scenario) input.prefix.handle_cheats_inGame=cheat.get();
 input.prefix.debug_load=[&result,debug](std::string& error){++result.prefix_calls;if(dh2_character_debug_load(debug->source.get(),&debug->files)!=1){error="actual Debug load failed";return false;}return true;};
 input.prefix.debug_get_switch=[debug](const char* key,bool& out,std::string& error){std::uint32_t enabled{};if(dh2_character_debug_get(&enabled,debug->source.get(),key,&debug->files)!=1){error="actual Debug get failed";return false;}out=enabled!=0;return true;};
 unsigned log_calls{};input.prefix.debug_out_loading_step=[&](std::uint32_t state,std::string&){require(state==c1.field130,"root logger observed shadow phase");++log_calls;return true;}; // Logging transport fixture, not whole _DEBUG_OUT.
 unsigned releases{},sound_calls{},published{};
 auto allocator=std::make_shared<int>(1);auto driver=std::make_shared<NativeDriverUnusedOwnerV50>();
 if(scenario==2){
  require(driver->bind_driver(allocator,1,[](std::string&){return true;},[](std::string&){return true;},e),e); // Native backend default/placeholder transport fixture.
  for(auto d:{UnusedDomainV50::batch_baker,UnusedDomainV50::material_instance,UnusedDomainV50::material_renderer,UnusedDomainV50::texture})require(driver->bind_collection(d,allocator,e),e);
  UnusedKeyV50 texture{UnusedDomainV50::texture,1,81};std::weak_ptr<NativeDriverUnusedOwnerV50> weakdriver=driver;
  require(driver->register_allocation(texture,allocator,[weakdriver,texture,&releases](bool lost,std::string& error){require(!lost,"stage0 fabricated contextloss");auto live=weakdriver.lock();if(!live){error="driver expired";return false;}if(!live->unregister_allocation(texture,error))return false;++releases;return true;},e),e);
  input.stage0.clean_glitch=[driver](const auto&,std::string& error){return driver->clean_glitch(error);};
  input.stage0.sound_manager_owner=allocator;input.stage0.sound_manager_identity=reinterpret_cast<std::uintptr_t>(allocator.get());
  input.stage0.stop_all_sounds=[&](std::uintptr_t id,int fade,std::string&){require(id==reinterpret_cast<std::uintptr_t>(allocator.get())&&fade==500,"originalStage0 Voxarguments changed");++sound_calls;return true;}; // Audio transport fixture, not wholeVox.
  input.source.external.publish_progress=[&](std::int32_t state,std::int32_t value,std::string& error){require(state==std::int32_t(c1.field130)&&value==std::int32_t(c1.phase30),"source progress callback shadow scalar");std::int32_t actual{};if(!ui::loading_menu_read_progress_v1(loading,actual,error))return false;require(actual==value,"real native Loading query differs");++published;return true;}; // AS delivery transport fixture.
 }
 require(Connection::create(input,connection,e),e);auto* kept=connection.get();
 auto no_producer=input;no_producer.manager_freshness={};require(!Connection::create(no_producer,connection,e)&&connection.get()==kept,"caller omitted authentic source manager producer");++result.rejections;
 no_producer={};
 auto conflicting=input;conflicting.source.external.stage_body[0]=[](std::string&){return LifecycleStepV36::complete;};
 require(!Connection::create(conflicting,connection,e)&&connection.get()==kept&&c1.field130==0,"source0 callback conflict silently overwritten");++result.rejections;
 conflicting=input;conflicting.source.external.stage_body[5]=[](std::string&){return LifecycleStepV36::complete;};
 require(!Connection::create(conflicting,connection,e)&&connection.get()==kept,"source5 callback conflict silently overwritten");++result.rejections;
 conflicting=input;conflicting.prefix.current_level=[](auto&,std::string&){return true;};
 require(!Connection::create(conflicting,connection,e)&&connection.get()==kept,"foreign current provider silently overwritten");++result.rejections;
 conflicting=input;conflicting.globals=std::make_shared<NativeGSLevelGlobalsV27>();
 require(!Connection::create(conflicting,connection,e)&&connection.get()==kept,"parallel globals accepted");++result.rejections;
 conflicting.globals->s_level=level;
 require(!Connection::create(conflicting,connection,e)&&connection.get()==kept,"parallel storage with SAME C1 accepted");++result.rejections;
 conflicting=input;auto foreign_manager=std::make_shared<Fixture>(bytes);conflicting.source.manager={foreign_manager,&foreign_manager->manager};
 require(!Connection::create(conflicting,connection,e)&&connection.get()==kept,"foreign manager accepted");++result.rejections;
 auto foreign_gs=std::make_shared<NativeGSLevelRuntimeV27>(globals);conflicting=input;conflicting.gs=foreign_gs;
 require(!Connection::create(conflicting,connection,e)&&connection.get()==kept,"foreign unconstructed GS accepted");++result.rejections;
 const auto script=c1.script44,save=c1.save_ec;const auto phase=c1.field130,counter=c1.field134,current=c1.field138,file=c1.field13c;const auto byte144=c1.byte144;
 require(connection->tick()==GSLevelUpdateResultV44::advanced&&gs->fields().loading38==2&&result.menus==1&&c1.field130==phase,"frame1 ran source body");
 require(connection->tick()==GSLevelUpdateResultV44::advanced&&gs->fields().loading38==3&&result.menus==2&&c1.field130==0,"frame2 Level.Load changed source semantics");
 if(scenario==2){
  require(connection->tick()==GSLevelUpdateResultV44::advanced&&c1.field130==1&&stageapp->bigI==0&&stageapp->bigV==0&&stageapp->byteb4==1&&c1.field134==0&&c1.field138==0&&c1.field13c==0&&c1.byte144==0&&releases==1&&sound_calls==1&&published==1,"native Stage0 and original dispatcher/tail did not complete");
  require(connection->tick()==GSLevelUpdateResultV44::advanced&&c1.field130==2&&published==2&&releases==1&&sound_calls==1,"source default Stage1 changed/replayed nativeStage0");
  require(connection->tick()==GSLevelUpdateResultV44::failed&&c1.field130==2&&published==2&&connection->error().find("Unload")!=std::string::npos,"missing authentic stage2 was forced complete");
  auto receipt=connection->error();require(connection->tick()==GSLevelUpdateResultV44::failed&&connection->error()==receipt&&releases==1&&sound_calls==1&&published==2,"failed sourceStage2 replayedcompletedprefix");
  require(level->source_word150()==0&&c1.script44==script&&c1.save_ec==save&&preparation->status().stage==RetainedModulePreparationStageV1::idle,"nativeearlyStages fakedfullInit/lootreadiness");
  require(gs->destroy(e)&&!globals->s_level,e);input={};conflicting={};connection.reset();result.debug_file_attempts=debug->attempts;return result;
 }
 require(connection->tick()==GSLevelUpdateResultV44::failed&&connection->required_service()=="Level.Update(false)"&&c1.field130==0&&gs->fields().loading38==3,"missing root provider advanced phase");
 const auto failure=connection->error();
 if(!scenario)require(failure.find("handle_cheats_inGame")!=std::string::npos&&debug->attempts==0&&log_calls==0&&stageapp->bigI==9&&stageapp->bigV==11&&connection->diagnostics().service_calls==0,"unavailable global invented false and entered stage0");
 else require(failure.find("Application.CleanGlitch")!=std::string::npos&&debug->attempts==1&&log_calls==1&&stageapp->bigI==0&&stageapp->bigV==0&&connection->diagnostics().service_calls==1,"real debug prefix and stage0 stores lost");
 require(connection->tick()==GSLevelUpdateResultV44::failed&&connection->error()==failure&&result.prefix_calls==scenario&&result.menus==2,"failed root prefix replayed");
 require(c1.field134==counter&&c1.field138==current&&c1.field13c==file&&c1.byte144==byte144&&stageapp->byteb4==127&&c1.script44==script&&c1.save_ec==save&&preparation->status().stage==RetainedModulePreparationStageV1::idle&&candidate->manager.source_init_phase7c_v38()==0&&candidate->manager.source_map_size1c_v38()==1&&level->source_word150()==0,"unreached source suffix changed");
 require(gs->destroy(e)&&!globals->s_level&&!gs->fields().level34&&unloads==1&&destroys==1,e); // Deep cleanup transport fixtures only.
 require(!Connection::create(input,connection,e)&&connection.get()==kept,"destroyed GS adopted again");++result.rejections;
 result.debug_file_attempts=debug->attempts;result.source_global_prefix=scenario==1;
 std::weak_ptr<NativeGSLevelRuntimeV27> weak=gs;input={};conflicting={};foreign_gs.reset();gs.reset();require(!weak.expired(),"root facade did not retain original GS");connection.reset();require(weak.expired(),"root sibling facade cycle");return result;
}
int main(int argc,char** argv){if(argc!=4)return 2;try{auto archive=pack(argv[1]);CharacterTables tables(archive,argv[2]);auto a=run_case(0,archive,tables,argv[3]);auto b=run_case(1,archive,tables,argv[3]);auto c=run_case(2,archive,tables,argv[3]);std::cout<<"{\"validation\":\"PASS\",\"actual_gs_cases\":3,\"logical_name\":\"SWAMP\",\"raw_name\":\"001_swamp.mlx\",\"actual_row\":41,\"same_source_owners\":true,\"constructor_only_does_not_fake_gameplay\":true,\"create_rejections\":"<<a.rejections+b.rejections+c.rejections<<",\"genuine_debug_missing_file_attempts\":"<<b.debug_file_attempts<<",\"actual_source_early_phases\":[0,1,2],\"native_cleanup_called_real_registry_release\":true,\"root_transport_fixtures\":true,\"whole_init_verified\":false}\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
