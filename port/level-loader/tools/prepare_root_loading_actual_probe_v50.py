from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[3]
PRIVATE=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
OUT=ROOT/'port/level-loader/reports/root-loading-v50'
source=PRIVATE/'port/level-loader/tests/native_gslevel_loading_v49.cpp'
text=source.read_text()
# Reuse only the independently captured actual cache/Arrays/C1/GS setup.
# Root-facing facade assertions below are new, not its implementation copied
# into a fixture. Transport ownership is explicitly fixture-only.
text=text[:text.index(' inputs.external.stage_body[0]=')]
text=text.replace('#include "../native_gslevel_loading_connection_v49.hpp"',
                  '#include "native_root_loading_connection_v50.hpp"')
text=text.replace('#include "../','#include "')
needed={'native_root_loading_connection_v50.hpp','character_design_services.hpp',
        'canonical_module_graph_v3.hpp','retained_level_module_graph_v1.hpp',
        'object_enable_condition_v2.hpp','canonical_gameobject_base_owner_v1.hpp'}
text='\n'.join(line for line in text.splitlines()
               if not line.startswith('#include "') or line.split('"')[1] in needed)+'\n'
text='#include "native_driver_unused_v50.hpp"\n#include "canonical_gameobject_base_owner_v1.hpp"\n#include "character_game_design.hpp"\n#include "loot_tables_v2.hpp"\n'+text
text=text.replace('require(scenario<2','require(scenario<3')
text=text.replace('using Connection=NativeGSLevelLoadingConnectionV49;std::unique_ptr<Connection> connection;',
                  'using Connection=NativeRootLoadingConnectionV50;std::unique_ptr<Connection> connection;')
text=text.replace('require(!Connection::create(gs,appborrow,services,{}, {},{},connection,e)',
                  'require(!Connection::create({},connection,e)')
text=text.replace('const std::string raw="worlds/001_swamp.mlx"','const std::string raw="001_swamp.mlx"')
text+=r'''
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
'''
out=OUT/'root-loading-actual-probe.cpp';out.write_text(text)
(OUT/'actual-probe-provenance.json').write_text(json.dumps({'source':str(source),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'generated_probe_sha256':hashlib.sha256(out.read_bytes()).hexdigest(),'scope':'Only actual cache/Arrays/C1/GS construction setup reused; root facade assertions are new. Transport callbacks remain explicit fixtures.'},indent=2))
print(out)
