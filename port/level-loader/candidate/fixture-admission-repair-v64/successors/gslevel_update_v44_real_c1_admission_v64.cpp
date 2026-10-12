#include "../../../gslevel_update_v44_level_borrow.hpp"
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
  
 GSLevelReadBorrowV44<CanonicalLevelContextV1> read;
 require(borrow_gslevel_read_v44(level,read,e),e);auto source=level->constructor_borrow_v3();
 require(read.actual_owner==level&&read.identity==level->identity()&&read.state130==&source.fields->field130&&read.byte145==&source.fields->byte145,"GS read borrowed copied phase/145 authority");
 require(*read.state130==0&&*read.byte145==0,"real C1 scalar defaults differ");
 const auto script=source.fields->script44;const auto save=source.fields->save_ec;
 auto gs=std::make_shared<GSLevelFieldsV2<CanonicalLevelContextV1>>();gs->level34=level;gs->loading38=2;gs->active3c=1;
 // Explicit selected GS.loading2 entry fixture, not real GS Ctor/publication.
 source.fields->field130=17;source.fields->phase30=37;source.fields->field134=9;source.fields->field138=11;source.fields->field13c=4;
 auto application_fixture=std::make_shared<std::uint8_t>(0);auto menu_fixture=std::make_shared<int>(1);unsigned menu_updates=0;
 GSLevelUpdateServicesV44<CanonicalLevelContextV1> services;
 services.borrow_level=borrow_gslevel_read_v44<CanonicalLevelContextV1>;
 services.level_load=actual_level_load_v44<CanonicalLevelContextV1>;
 services.menu_instance=[menu_fixture](GSMenuBorrowV44& out,std::string&){out={menu_fixture,reinterpret_cast<std::uintptr_t>(menu_fixture.get())};return true;};
 services.menu_update=[&](const GSMenuBorrowV44& menu,bool flag,std::string&){require(menu.actual_owner==menu_fixture&&flag,"real source Menu(true) transport changed");++menu_updates;return true;};
 GSLevelUpdateV44<CanonicalLevelContextV1> update(gs,*gs,{application_fixture,application_fixture.get(),nullptr,nullptr},std::move(services));
 require(update.tick()==GSLevelUpdateResultV44::advanced&&gs->loading38==3&&source.fields->field130==0&&menu_updates==1,"real C1 Level.Load/GS loading2 envelope differs");
 require(gs->level34==level&&gs->active3c==1&&source.fields->phase30==37&&source.fields->field134==9&&source.fields->field138==11&&source.fields->field13c==4&&source.fields->script44==script&&source.fields->save_ec==save,"Level.Load changed non-source cells/owners");
 require(update.tick()==GSLevelUpdateResultV44::failed&&gs->loading38==3&&source.fields->field130==0&&update.required_service()=="Level.Update(false)","absent loading/frame service invented Level.Init");
 std::cout<<"{\"validation\":\"PASS\",\"real_c1_pointer_aliases\":2,\"real_level_load_source_store\":true,\"untouched_non_source_cells\":5,\"same_script_save_owners\":true,\"missing_level_update_preserved\":true,\"gs_entry_fixture\":2,\"current_level_global_created\":false,\"gs_ctor_publication_verified\":false,\"whole_level_init_verified\":false}\n";return 0;
 }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
