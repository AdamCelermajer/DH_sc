#include "../canonical_level_context_v1.hpp"
#include "../level_constructor_bindings_v4.hpp"
#include "../native_gslevel_runtime_v27.hpp"
#include "../../engine-ui/loading_menu_v1.hpp"
#include "../../level-world/character_game_design.hpp"
#include <array>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
using namespace dh2;using namespace loader;
using Raw=std::vector<std::uint8_t>;
unsigned checks{};void check(bool b){++checks;if(!b)throw std::runtime_error("integration check "+std::to_string(checks));}
std::uint32_t word(std::istream& f){std::uint32_t n{};f.read(reinterpret_cast<char*>(&n),4);check(bool(f));return n;}
Raw blob(std::istream& f){Raw out(word(f));f.read(reinterpret_cast<char*>(out.data()),out.size());check(bool(f));return out;}
struct Inputs {
 std::array<std::array<Raw,3>,5> tables;std::vector<Raw> constants;std::vector<data::Bytes> views;character::GameDesignInputs256 input;
 explicit Inputs(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f)&&word(f)==0x314f4447);for(auto& t:tables)for(auto& b:t)b=blob(f);auto n=word(f);for(unsigned i=0;i<n;++i){blob(f);constants.push_back(blob(f));}n=word(f);for(unsigned i=0;i<n;++i)blob(f);check(f.peek()==EOF);
  character::GameDesignTableInput48* target[]={&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};for(unsigned i=0;i<5;++i)*target[i]={{tables[i][0].data(),tables[i][0].size()},{tables[i][1].data(),tables[i][1].size()},{tables[i][2].data(),tables[i][2].size()}};
  for(auto& b:constants)views.push_back({b.data(),b.size()});input.constants=views.data();input.constant_count=views.size();
 }
};
struct ApplicationFilesFixture {
 std::string script_dir,save_dir;unsigned script_reads{},save_reads{};bool fail_saves{};
 std::vector<std::string> attempted;
 static bool read(const std::string& path,Raw& out,bool& found){std::ifstream f(path,std::ios::binary);found=bool(f);if(found)out={std::istreambuf_iterator<char>(f),{}};return true;}
 static bool script(void* p,const std::string& path,Raw& out,bool& found,std::string& error){
  auto& app=*static_cast<ApplicationFilesFixture*>(p);++app.script_reads;
  const std::string prefix="data/scripts/level/";
  if(path.rfind(prefix,0)!=0){error="Unknown requested original script path";return false;}
  return read(app.script_dir+'/'+path.substr(prefix.size()),out,found);
 }
 static bool save(void* p,const std::string& path,bool& found,Raw& out,std::string& error){
  auto& app=*static_cast<ApplicationFilesFixture*>(p);++app.save_reads;app.attempted.push_back(path);
  if(app.fail_saves){error="explicit mandatory file service failure";return false;}
  return read(app.save_dir+'/'+path,out,found);
 }
};
int main(int argc,char** argv){try{
 check(argc==4);Inputs inputs(argv[1]);character::CharacterGameDesign design;std::string error;check(design.initialize(inputs.input,error));
 auto actual=std::make_shared<character::CharacterGameDesign::Borrow>(design.borrow());check(actual->levels()&&actual->levels()->levels.size()==51);
 auto files=std::make_shared<ApplicationFilesFixture>();files->script_dir=argv[2];files->save_dir=argv[3];
 NativeLevelFilesV25 native_files;native_files.owner=files;
 native_files.script=[files](const auto& path,auto& bytes,bool& found,auto& e){return ApplicationFilesFixture::script(files.get(),path,bytes,found,e);};
 native_files.saved=[files](const auto& path,bool& found,auto& bytes,auto& e){return ApplicationFilesFixture::save(files.get(),path,found,bytes,e);};
 auto application_facet=std::make_shared<NativeLevelApplicationV25>(std::move(native_files));
 application_facet->module_globals.next_module_id=99;
 LevelConstructorApplicationV4 application;application.owner=actual;application.levels=actual->levels();
application.lua.owner=actual;application.lua.design=*actual->design();application.private_vm_limit=16u*1024u*1024u;
 application.saves.files.context=files.get();application.saves.files.read_file=ApplicationFilesFixture::save;
 application.saves.files.storage_lease=files;
 application.online_byte5=[](auto& value,auto&){value=0;return true;}; // Explicit offline source-state fixture.
 application=application_facet->bind(std::move(application));
 std::string selected;
 for(auto& row:actual->levels()->levels){std::string lower=row.file;for(char& c:lower)c=char(std::tolower(static_cast<unsigned char>(c)));if(lower.find("swamp")!=std::string::npos){selected="worlds/"+lower;break;}}
 check(!selected.empty());
 std::shared_ptr<CanonicalLevelContextV1> first,second;std::shared_ptr<LevelConstructorBindingsV4> first_bindings,second_bindings;
 for(unsigned i=0;i<2;++i){
  LevelSourceRequestV1 source;source.identity=selected;source.definition="isolated-source-selection";source.seed=7;
  auto globals=std::make_shared<NativeGSLevelGlobalsV27>();
  NativeGSLevelRuntimeV27 runtime(globals);unsigned flushes{},pushes{},callbacks{};
  check(runtime.prepare_loading({},error));
  const auto loading=runtime.loading_services();
  std::int32_t progress{};check(!ui::loading_menu_read_progress_v1(loading,progress,error));
  GSLevelServicesV2<CanonicalLevelContextV1> outer;
  // Explicit deeper Menu/AnimSet fixtures. Full Level C1, current storage,
  // loading reads, source publication order and failure retention are real.
  outer.flush_animation_sets=[&](auto&){++flushes;check(!globals->s_level);return true;};
  outer.get_menu=[&](const char* name,auto& value,auto&){check(globals->s_level!=nullptr);check(std::string(name)=="menu_Loading");value=17;return true;};
  outer.online_byte5=application.online_byte5;
  outer.push_menu=[&](auto id,auto&){check(id==17);++pushes;return true;};
  outer.menu_render_fx=[](auto id,auto& value,auto&){check(id==17);value=23;return true;};
  outer.check_menu_weak_proxy=[](auto id,auto&){check(id==17);return true;};
  outer.menu_character=[](auto id,auto& value,auto&){check(id==17);value=31;return true;};
  outer.invoke_as_no_arguments=[&](auto render,auto character,const char* method,auto& e){
   check(render==23&&character==31&&std::string(method)=="onProgress");++callbacks;
   check(runtime.fields().loading38==1&&runtime.fields().active3c==1&&runtime.fields().level34==globals->s_level);
   return ui::loading_menu_read_progress_v1(loading,progress,e);
  };
  outer.unload_level=[](const auto&,auto& e){e="declared unrecovered original Unload";return false;};
  check(runtime.construct(source,{selected,0,7,1,0,1,0,-1,0},application,std::move(outer),{},error));
  auto& connection=*runtime.connection()->level_connection();
  auto level=connection.candidate();auto bindings=connection.bindings();
  check(connection.complete()&&bool(level)&&bool(bindings)&&globals->s_level==level);
  CanonicalCurrentLevelBorrowV1 current;check(runtime.current(current,error)&&current.level()==level);
  check(flushes==1&&pushes==1&&callbacks==1&&progress==int(level->constructor_fields_v3().phase30));
  bool advanced=true;check(ui::loading_menu_finish_v1(loading,advanced,error)&&!advanced&&level->constructor_fields_v3().field130==0);
  const auto calls=application_facet->debug_level_load_count;
  check(!runtime.construct(source,{selected,0,7,1,0,1,0,-1,0},application,{}, {},error));
  check(!runtime.prepare_loading({},error)&&!runtime.destroy(error)&&globals->s_level==level&&runtime.fields().level34==level);
  check(!runtime.destroy(error)&&globals->s_level==level);
  check(application_facet->debug_level_load_count==calls);
  auto script=bindings->script();auto save=bindings->save();check(script&&save&&script.get()==level->constructor_fields_v3().script44.get()&&save.get()==level->constructor_fields_v3().save_ec.get());
  check(script->path()=="data/scripts/"&&script->loaded_files().size()==2&&script->bindings().size()==33);
  check(save->owner().ready()&&save->owner().fields().level8==reinterpret_cast<const void*>(level->identity()));
  check(save->owner().fields().row28==level->constructor_fields_v3().row3c&&save->owner().fields().loaded_row2c==-1);
  dh2_script_value value{};check(!dh2_script_vm_get_global(script->vm_borrow(),"CF_CalcDamage",&value)&&value.type==DH2_SCRIPT_FUNCTION);
  bool success{};check(script->call("CF_ClearCombatants",nullptr,0,success,error)&&success);check(script->call("CF_CalcDamage",nullptr,0,success,error)&&success);
  if(i==0){first=std::move(level);first_bindings=std::move(bindings);}else{second=std::move(level);second_bindings=std::move(bindings);}
 }
 check(application_facet->debug_level_load_count==2&&application_facet->module_globals.next_module_id==0&&application_facet->script_cache()->cached_count()==2&&files->script_reads==2);
 check(first_bindings->script()->vm_borrow()!=second_bindings->script()->vm_borrow());
 const std::string setting="SetInt('first_private_state',77)";check(!dh2_script_vm_load_source_file(first_bindings->script()->vm_borrow(),setting.data(),setting.size()));
 const std::string query="isolated_value=GetInt('first_private_state')";check(!dh2_script_vm_load_source_file(second_bindings->script()->vm_borrow(),query.data(),query.size()));dh2_script_value value{};check(!dh2_script_vm_get_global(second_bindings->script()->vm_borrow(),"isolated_value",&value)&&value.number==0);
 check(files->save_reads==4&&files->attempted.size()==4&&files->attempted[1]==files->attempted[0]+".bak");
 {auto bad=application;files->fail_saves=true;auto bindings=LevelConstructorBindingsV4::create(bad,error);check(bool(bindings));
  LevelSourceRequestV1 source;source.identity=selected;source.definition="isolated-source-selection";source.seed=7;std::shared_ptr<CanonicalLevelContextV1> level;check(CanonicalLevelContextV1::create(source,actual,level,error));
  check(!level->construct_source_v3({selected.c_str(),0,7,1,0,1,0,-1,0},bindings->services(),error));check(level->constructor_fields_v3().script44&&!level->constructor_fields_v3().save_ec&&level->constructor_owner_v3()->failed_at()==LevelConstructorPhaseV3::save_constructor);
  check(bindings->script()&&bindings->script()->loaded_files().size()==2&&!bindings->save());
 }
 std::weak_ptr<scripts::GenericLuaScriptOwnerV13> script=first_bindings->script();std::weak_ptr<level::LevelSavegameRuntimeV1> save=first_bindings->save();first.reset();check(script.expired()&&save.expired()&&!first_bindings->script()&&!first_bindings->save());
 std::cout<<"PASS real native GS publication/Loading and complete Level C1 with actual51 design rows, original Swamp selection, sole real script path/private states/shared file cache and staged native Save owner; checks="<<checks<<" | selected="<<selected<<'\n';
 }catch(const std::exception& x){std::cerr<<x.what();return 1;}}
