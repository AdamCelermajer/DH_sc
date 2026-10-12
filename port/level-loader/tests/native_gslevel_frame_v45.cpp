#include "../native_gslevel_frame_v45.hpp"
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
 std::shared_ptr<CanonicalLevelContextV1> level;
 // Online/Handle are explicit standalone transport fixtures; RNG and SAME
 // canonical base state are actual. Required delete/visibility paths stay absent.
 auto spawn_for=[application](CanonicalGameObjectBaseOwnerV1& base,std::int32_t& roll,std::string& error){CanonicalSpawnApplicationServicesV4 services;services.application_lease=application;services.random=application.get();services.online_byte5=[](bool& value,std::string&){value=false;return true;};services.handle_as_player=[](bool& value,std::string&){value=false;return true;};std::int32_t probability{};return canonical_check_spawn_probability_v4(base,services,roll,probability,error);};
   ModuleRuntimeGlobalsV1 module_globals;module_globals.next_module_id=99;std::uint32_t source_debug_loads=0;
  auto actual_design=std::make_shared<character::CharacterGameDesign::Borrow>(character_tables.design.borrow());
  auto native_files=std::make_shared<NativeCtorFilesFixtureV24>(archive,argv[3]);
  auto lua_cache=std::make_shared<scripts::LuaScriptCacheOwnerV13>(scripts::LuaScriptCacheServicesV13{native_files,native_files.get(),NativeCtorFilesFixtureV24::script});
  LevelConstructorApplicationV4 application_services;application_services.owner=actual_design;application_services.debug_level_load_count=&source_debug_loads;application_services.module_id_global=&module_globals.next_module_id;application_services.levels=actual_design->levels();application_services.lua_cache=lua_cache;application_services.lua.owner=actual_design;application_services.lua.design=*actual_design->design();application_services.private_vm_limit=16u*1024u*1024u;
  application_services.saves.files.context=native_files.get();application_services.saves.files.read_file=NativeCtorFilesFixtureV24::save;application_services.saves.files.storage_lease=native_files;
  application_services.online_byte5=[](std::uint8_t& value,std::string&){value=0;return true;}; // Explicit offline transport fixture.
  
 const std::string raw_name="worlds/001_swamp.mlx";
 LevelSourceRequestV1 request;request.identity="SWAMP";request.definition="data/scene/001_swamp.mlx";request.seed=7;
 GSLevelArgumentsV2 arguments{raw_name,0,7,1,0,1,0,-1,0};
 auto globals=std::make_shared<NativeGSLevelGlobalsV27>();auto gs=std::make_shared<NativeGSLevelRuntimeV27>(globals);
 require(gs->prepare_loading({},e),e);const auto loading=gs->loading_services();
 unsigned animation_flushes=0,menu_pushes=0,progress_callbacks=0,unloads=0,destroys=0;
 std::vector<std::string> outer_trace;
 GSLevelServicesV2<CanonicalLevelContextV1> outer;
 // Source outer GS lifecycle and whole Level C1 are real. Deeper AnimSet/Menu/
 // offline/Unload/Destroy transports are explicit fixtures. No cleanup/save/
 // complete Level.Init or GPU gameplay claim is made.
 outer.flush_animation_sets=[&](std::string&){require(!globals->s_level,"GS published before original animation flush");++animation_flushes;outer_trace.push_back("flush");return true;};
 outer.get_menu=[&](const char* name,std::uintptr_t& value,std::string&){
  require(globals->s_level&&globals->s_level==gs->fields().level34,"menu observed a foreign/unpublished Level");outer_trace.push_back(name);
  if(std::string(name)=="menu_Loading")value=17;else{require(std::string(name)=="menu_HUD_0","wrong original Dtor menu");value=0;}return true;
 };
 outer.online_byte5=[](std::uint8_t& value,std::string&){value=0;return true;};
 outer.push_menu=[&](std::uintptr_t menu,std::string&){require(menu==17,"source loading menu mismatch");++menu_pushes;return true;};
 outer.menu_render_fx=[](std::uintptr_t menu,std::uintptr_t& out,std::string&){require(menu==17,"source menu renderFX mismatch");out=23;return true;};
 outer.check_menu_weak_proxy=[](std::uintptr_t menu,std::string&){require(menu==17,"source menu proxy mismatch");return true;};
 outer.menu_character=[](std::uintptr_t menu,std::uintptr_t& out,std::string&){require(menu==17,"source menu character mismatch");out=31;return true;};
 outer.invoke_as_no_arguments=[&](std::uintptr_t fx,std::uintptr_t character,const char* method,std::string& error){
  require(fx==23&&character==31&&std::string(method)=="onProgress","source menu callback mismatch");
  CanonicalCurrentLevelBorrowV1 current;require(gs->current(current,error)&&current&&current.level()==globals->s_level,error);
  require(current.level()->constructor_owner_v3()->phase()==LevelConstructorPhaseV3::complete&&current.level()->constructor_fields_v3().field130==0,"onProgress preceded whole actual C1");
  std::int32_t progress{};require(ui::loading_menu_read_progress_v1(loading,progress,error)&&progress==0,error);++progress_callbacks;return true;
 };
 outer.unload_level=[&](const std::shared_ptr<CanonicalLevelContextV1>& same,std::string&){require(same==globals->s_level&&same==level,"Dtor Unload replaced actual Level");++unloads;outer_trace.push_back("unload");return true;};
 outer.destroy_level=[&](const std::shared_ptr<CanonicalLevelContextV1>& same,std::string&){require(same==globals->s_level&&same==level,"Dtor Destroy unpublished too early");++destroys;outer_trace.push_back("destroy");return true;};
 auto application_gate=std::make_shared<std::uint8_t>(0);auto frame_menu=std::make_shared<int>(1);
 GSApplicationBorrowV44 application_borrow{application_gate,application_gate.get(),nullptr,nullptr};
 GSLevelUpdateServicesV44<CanonicalLevelContextV1> frame_services;unsigned frame_menus=0;
 frame_services.menu_instance=[frame_menu](GSMenuBorrowV44& out,std::string&){out={frame_menu,reinterpret_cast<std::uintptr_t>(frame_menu.get())};return true;};
 frame_services.menu_update=[&](const GSMenuBorrowV44& menu,bool flag,std::string&){require(menu.actual_owner==frame_menu&&flag&&globals->s_level==level,"frame menu/source current borrow mismatch");++frame_menus;return true;};
 std::unique_ptr<NativeGSLevelFrameV45> frame;unsigned rejected=0;
 require(!NativeGSLevelFrameV45::create(gs,application_borrow,frame_services,frame,e)&&!frame&&!globals->s_level,"unconstructed GS produced frame/current state");++rejected;
 require(gs->construct(request,arguments,application_services,outer,{},e),e);level=globals->s_level;
 require(level&&level==gs->fields().level34&&gs->fields().loading38==1&&gs->fields().active3c==1,"authentic GS C1 publication fields mismatch");
 const auto& c1=level->constructor_fields_v3();require(c1.row3c==41&&c1.name_f8==raw_name&&c1.seed114==7&&c1.field130==0&&level->source_request().identity=="SWAMP"&&level->source_request().seed==7,"GS raw name/logical identity/real table row provenance lost");
 require(source_debug_loads==1&&globals->s_level->source_word150()==0&&animation_flushes==1&&menu_pushes==1&&progress_callbacks==1,"whole C1 or source outer prefix replayed");
 const auto actual_save=gs->connection()->level_connection()->bindings()->save();const auto actual_script=gs->connection()->level_connection()->bindings()->script();
 require(native_files->script_reads==2&&lua_cache->cached_count()==2&&native_files->save_reads==2&&actual_script&&actual_save&&actual_script.get()==c1.script44.get()&&actual_save.get()==c1.save_ec.get()&&actual_save->owner().ready(),"actual C1 Lua/Save owners missing");
 require(NativeGSLevelFrameV45::create(gs,application_borrow,frame_services,frame,e),e);auto* retained_frame=frame.get();
 // Each rejection retains the already-created actual frame and published C1.
 auto conflict=frame_services;conflict.borrow_level=borrow_gslevel_read_v44<CanonicalLevelContextV1>;
 require(!NativeGSLevelFrameV45::create(gs,application_borrow,conflict,frame,e)&&frame.get()==retained_frame&&globals->s_level==level,"foreign C1 read callback replaced authoritative frame");++rejected;
 conflict=frame_services;conflict.level_load=actual_level_load_v44<CanonicalLevelContextV1>;
 require(!NativeGSLevelFrameV45::create(gs,application_borrow,conflict,frame,e)&&frame.get()==retained_frame&&globals->s_level==level,"conflicting Level.Load replaced frame");++rejected;
 require(!NativeGSLevelFrameV45::create({},application_borrow,frame_services,frame,e)&&frame.get()==retained_frame,"null runtime replaced frame");++rejected;
 auto foreign_gs=std::make_shared<NativeGSLevelRuntimeV27>(globals);
 // A foreign GS wrapper can see the same actual globals but has no constructed
 // fields/lifecycle. Reading the global alone cannot authorize a frame owner.
 require(!NativeGSLevelFrameV45::create(foreign_gs,application_borrow,frame_services,frame,e)&&frame.get()==retained_frame&&globals->s_level==level,"foreign unconstructed GS lent actual frame fields");++rejected;foreign_gs.reset();
 const auto script=c1.script44;const auto save=c1.save_ec;
 require(frame->tick()==GSLevelUpdateResultV44::advanced&&gs->fields().loading38==2&&c1.field130==0&&frame_menus==1,"actual GS frame1->2 advanced Level.Init");
 require(frame->tick()==GSLevelUpdateResultV44::advanced&&gs->fields().loading38==3&&c1.field130==0&&frame_menus==2,"actual GS frame2 Load source store differs");
 require(frame->tick()==GSLevelUpdateResultV44::failed&&frame->required_service()=="Level.Update(false)"&&gs->fields().loading38==3&&c1.field130==0&&frame_menus==2,"missing actual Level.Update did not stop source frame3");
 const auto frame_error=frame->error();require(frame->tick()==GSLevelUpdateResultV44::failed&&frame->error()==frame_error&&source_debug_loads==1&&frame_menus==2,"failed GS frame replayed source prefix");
 CanonicalCurrentLevelBorrowV1 current;require(gs->current(current,e)&&current&&current.level()==level&&current.kill_level()==level->kill_level(),e);
 CanonicalLevelContextV1::LoadingFieldsV26 loading_fields;require(level->loading_fields_v26(loading_fields,e)&&loading_fields.state130==&level->constructor_fields_v3().field130,"current Loading/C1 aliases differ");
 require(c1.script44==script&&c1.save_ec==save&&c1.phase30==0&&c1.field134==0&&c1.field138==0&&c1.field13c==0,"GS envelope replaced C1 owners or fake loading progress");
 // Execute actual outer Dtor algorithm with explicit deeper source transports.
 // Full Unload/destruction/abort are NOT verified by those fixture callbacks.
 require(gs->destroy(e)&&!globals->s_level&&!gs->fields().level34&&unloads==1&&destroys==1,e);
 require(outer_trace==std::vector<std::string>({"flush","menu_Loading","unload","menu_HUD_0","destroy"}),"original GS outer destructor/publication order changed");
 CanonicalCurrentLevelBorrowV1 empty;require(gs->current(empty,e)&&!empty,"GS current remained after original outer Dtor clear");
 require(!NativeGSLevelFrameV45::create(gs,application_borrow,frame_services,frame,e)&&frame.get()==retained_frame,"destroyed GS replaced retained frame");++rejected;
 std::weak_ptr<NativeGSLevelRuntimeV27> weak=gs;gs.reset();require(!weak.expired(),"frame sibling did not retain actual GS instance");frame.reset();require(weak.expired(),"GS/frame created strong self cycle");
 std::cout<<"{\"validation\":\"PASS\",\"logical_identity\":\"SWAMP\",\"raw_name\":\""<<raw_name<<"\",\"real_row3c\":41,\"source_seed114\":7,\"real_c1_script_reads\":2,\"real_c1_save_reads\":2,\"source_gs_publication_before_menu\":true,\"frame_states\":[1,2,3],\"state130\":0,\"missing_level_update_stops3\":true,\"atomic_create_rejections\":"<<rejected<<",\"frame_menu_updates\":"<<frame_menus<<",\"same_global_cleared_by_outer_dtor\":true,\"frame_sibling_cycle_free\":true,\"deep_unload_destroy_are_fixtures\":true,\"whole_level_init_verified\":false,\"gameplay_ready\":false}\n";
 return 0;
 }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
