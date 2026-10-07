#include "../character_script_assets_v1.hpp"
#include "../character_player_skills_v2.hpp"
#include "../character_script_init_vitals.hpp"
#include "../character_design_services.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2;using namespace dh2::character;
namespace {
using Raw=std::vector<std::uint8_t>;unsigned checks{};
void check(bool v,const std::string& e){++checks;if(!v)throw std::runtime_error(e);}
Raw file(const std::string& p){std::ifstream f(p,std::ios::binary);check(bool(f),"Read "+p);return {std::istreambuf_iterator<char>(f),{}};}
data::Bytes bytes(const Raw& raw){return {raw.data(),raw.size()};}
std::uint32_t word(std::istream& f){std::uint32_t n{};check(bool(f.read(reinterpret_cast<char*>(&n),4)),"Fixture word");return n;}
Raw blob(std::istream& f){Raw b(word(f));check(bool(f.read(reinterpret_cast<char*>(b.data()),b.size())),"Fixture blob");return b;}
void skip_text(std::istream& f){auto n=word(f);check(bool(f.seekg(n,std::ios::cur)),"Fixture text");}
struct DesignInputs {
 std::array<std::array<Raw,3>,5> tables;std::vector<Raw> constants;std::vector<data::Bytes> views;GameDesignInputs256 input{};
 explicit DesignInputs(const char* path){
  std::ifstream f(path,std::ios::binary);check(word(f)==0x314f4447,"Original cache GameDesign fixture");
  for(auto& t:tables)for(auto& v:t)v=blob(f);
  auto n=word(f);while(n--){skip_text(f);constants.push_back(blob(f));}
  n=word(f);while(n--)skip_text(f);check(f.peek()==EOF,"Fixture end");
  GameDesignTableInput48* out[]{&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};
  for(unsigned i=0;i<5;++i)*out[i]={bytes(tables[i][0]),bytes(tables[i][1]),bytes(tables[i][2])};
  for(auto& v:constants)views.push_back(bytes(v));input.constants=views.data();input.constant_count=views.size();
 }
};
struct Unit {
 State state{};NativeFsm24 fsm{};
 std::shared_ptr<data::PropertyState> properties=std::make_shared<data::PropertyState>();
 std::shared_ptr<data::CombatActorState> combat=std::make_shared<data::CombatActorState>();
 std::shared_ptr<data::PlayerSavegameV1> saved=std::make_shared<data::PlayerSavegameV1>();
 std::unique_ptr<skills::CharacterPlayerSkillsV2> player;
};
int no_debug_file(void*,const char*,std::uintptr_t* out){*out=0;return 0;}
int no_close(void*,std::uintptr_t){return -1;}
int vitals(void* p,CharacterScriptSessionV2& s,const ScriptLifecycleRequest32& request){
 check(request.service==script_refresh_vitals&&request.subject==s.timers().owner,"Same-owner InitHpMp");
 ScriptInitVitals32 in{s.timers().owner,&s.property_view(),*static_cast<fx::PreloadServices16*>(p)};
 ScriptInitVitals24 out{};return dh2_character_script_init_vitals(&out,&in)==1?0:-1;
}
}
int main(int argc,char** argv){try{
 check(argc==4,"Usage: cache_bootstrap design-fixture extracted-cache exact-directory.txt");std::string error;
 DesignInputs raw(argv[1]);auto design=std::make_unique<CharacterGameDesign>();check(design->initialize(raw.input,error),error);auto d=design->borrow();
 const std::string cache=argv[2];auto read=[&](const std::string& p){return file(cache+"/"+p);};
 data::SkillTables skills;
 auto a=read("data/pydata/skills_pyarray.bin"),b=read("data/pydata/skills_pyarraynames.bin"),c=read("data/pydata/skills_pystructnames.bin");
 check(skills.load(bytes(a),bytes(b),bytes(c),error),error);
 ScriptAssetServicesV1 services;
 services.directory=[&](auto& out,auto& e){std::ifstream f(argv[3]);if(!f){e="Exact cache directory missing";return false;}std::string p;out.clear();while(std::getline(f,p))out.push_back(p);e.clear();return true;};
 services.read=[&](const auto& p,auto& found,auto& out,auto& e){auto path=p;for(auto& ch:path)if(ch>='A'&&ch<='Z')ch=char(ch-'A'+'a');std::ifstream f(cache+"/"+path,std::ios::binary);found=bool(f);if(found)out.assign(std::istreambuf_iterator<char>(f),{});else out.clear();e.clear();return true;};
 CharacterScriptAssetsV1 resources;check(resources.load(services,skills.borrow(),error),error);auto files=resources.borrow();
 auto* debug=dh2_character_debug_create();check(debug,"Debug owner");
 std::unique_ptr<DebugSwitches,decltype(&dh2_character_debug_destroy)> debug_owner(debug,dh2_character_debug_destroy);
 DebugFileServices24 debug_files{nullptr,no_debug_file,no_close};
 auto* modules=dh2_fx_debug_modules_create(debug,&debug_files);check(modules,"Debug modules");
 std::unique_ptr<fx::DebugModules,decltype(&dh2_fx_debug_modules_destroy)> module_owner(modules,dh2_fx_debug_modules_destroy);
 fx::PreloadServices16 debug_services{modules,dh2_fx_debug_preload_service};
 skills::PlayerSkillInitServicesV2 init{&debug_services,vitals};auto temporary=std::make_shared<data::PropertySheet>();
 std::vector<std::unique_ptr<Unit>> units;unsigned instances=0;
 for(const auto* name:{"KnightPlayerBase","MagePlayerBase","RoguePlayerBase"}){
  auto unit=std::make_unique<Unit>();auto row=std::find(d.characters()->names.begin(),d.characters()->names.end(),name);check(row!=d.characters()->names.end(),"Actual player class row");
  data::reset_properties(*d.rules(),*unit->properties,&d.characters()->rows[row-d.characters()->names.begin()]);
  check(data::recalc_properties_with_class(*d.classes(),*d.rules(),*unit->properties,error),error);
  const auto id=std::uintptr_t(0xcac4000000000000ull+units.size()+1);unit->saved->set_character(id);
  const auto sb=files.skills();auto list=unit->properties->resolved[28];if(list<0||std::size_t(list)>=sb.lists().size())list=3;
  check(unit->saved->initialize_skills(sb.lists().at(list),error),error);
  // Explicit test FSM/initial saved rows. Starting grants and full gameplay
  // remain unsupported until the real Buff/state services are connected.
  unit->state.current=3;unit->fsm={&unit->state,id,1,0};
  CharacterScriptSessionInputV2 input;input.identity=id;input.name=name;input.source_is_character=1;
  input.properties=unit->properties;input.combat=unit->combat;input.temporary=temporary;input.savegame=unit->saved;
  input.common=files.common();input.state_machine=&unit->fsm;
  check(files.session_files("",input.include_files,error),error);
  unit->player=skills::CharacterPlayerSkillsV2::create(design->borrow(),input,files.skills(),files.faeries(),debug_services,unit->fsm,init,error);
  check(bool(unit->player),error);check(unit->player->initialize(1)==1,unit->player->error());
  check(unit->player->session().properties()==unit->properties&&unit->player->session().combat_state()==unit->combat,"Same player backing through actual VM");
  check(unit->player->session().timers().owner==id&&unit->saved->character()==id,"Same saved/timer identity");
  const auto& state=unit->player->state();check(state.skills.count==sb.lists()[list].size(),"Actual configured skill rows");
  for(unsigned i=0;i<state.skills.count;++i)if(state.skills.items[i])++instances;
  check(unit->player->initialize(1)==0,"Active startup does not duplicate init/refill");
  check(unit->saved->skills_initialized()&&std::all_of(unit->saved->skills().begin(),unit->saved->skills().end(),[](const auto& s){return s.level==0;}),"Unmodified actual initial saved rows");
  units.push_back(std::move(unit));
 }
 check(instances==19,"Nineteen actual class skill instances");
 for(auto& unit:units)check(unit->player->update()==1,unit->player->error());
 units.clear();
 std::cout<<"{\"validation\":\"PASS\",\"actual_player_sessions\":3,\"actual_skill_instances\":"<<instances<<",\"script_universe\":"<<files.files().size()<<",\"checks\":"<<checks<<",\"same_renderable_backings\":true,\"starting_grants_applied\":false,\"live_Android_connected\":false}"<<std::endl;
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
