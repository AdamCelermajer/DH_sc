#include "../character_script_objects.hpp"
#include "../character_script_session.hpp"
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <fstream>
#include <iterator>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <dlfcn.h>
using namespace dh2::character;
namespace {
using Raw=std::vector<std::uint8_t>;
unsigned checks=0;
void require(bool ok,unsigned line){++checks;if(!ok)throw std::runtime_error("Scene script objects line "+std::to_string(line));}
#define check(x) require(bool(x),__LINE__)
std::uint32_t word(std::istream& in){std::uint32_t v=0;in.read(reinterpret_cast<char*>(&v),4);check(in);return v;}
Raw blob(std::istream& in){Raw b(word(in));if(!b.empty())in.read(reinterpret_cast<char*>(b.data()),b.size());check(in);return b;}
Raw file(const char* path){std::ifstream in(path,std::ios::binary);check(in);return {std::istreambuf_iterator<char>(in),{}};}
struct Inputs {
 std::array<std::array<Raw,3>,5> tables;std::vector<Raw> constants;std::vector<dh2::data::Bytes> views;GameDesignInputs256 input{};
 explicit Inputs(const char* path){std::ifstream in(path,std::ios::binary);check(in&&word(in)==0x314f4447);
  for(auto& t:tables)for(auto& b:t)b=blob(in);auto n=word(in);for(unsigned i=0;i<n;++i){blob(in);constants.push_back(blob(in));}
  n=word(in);for(unsigned i=0;i<n;++i)blob(in);check(in.peek()==EOF);
  GameDesignTableInput48* target[]={&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};
  for(unsigned i=0;i<5;++i)*target[i]={{tables[i][0].data(),tables[i][0].size()},{tables[i][1].data(),tables[i][1].size()},{tables[i][2].data(),tables[i][2].size()}};
  for(auto& b:constants)views.push_back({b.data(),b.size()});input.constants=views.data();input.constant_count=views.size();
 }
};
struct Host {
 HostPlayer8 player{1,0};HostLevel8 level{23,0};std::vector<LevelRangeRow24> rows;
 HostContextBindings16 host{{this,invoke}};
 explicit Host(const CharacterGameDesign::Borrow& d){for(const auto& r:d.levels()->levels){LevelRangeRow24 row;std::memcpy(&row,r.scalar.words+12,24);rows.push_back(row);}}
 static int invoke(void* p,const HostContextRequest16* r,HostContextResponse16* out){auto& h=*static_cast<Host*>(p);
  if(r->service==host_get_player)out->data=&h.player;else if(r->service==host_get_current_level)out->data=&h.level;
  else if(r->service==host_get_range_rows){out->data=h.rows.data();out->count=h.rows.size();}else return 1;return 0;
 }
};
struct Debug {
 std::string path;DebugSwitches* owner=dh2_character_debug_create();unsigned opens=0;
 DebugFileServices24 files{this,open,close};DebugLevelBinding16 binding{owner,&files};LevelServices16 level{&binding,dh2_character_debug_level_service};
 explicit Debug(const char* p):path(p){check(owner);}
 ~Debug(){dh2_character_debug_destroy(owner);}
 static int open(void* p,const char* name,std::uintptr_t* out){auto& d=*static_cast<Debug*>(p);check(std::strcmp(name,"DebugSwitches.savegame")==0);++d.opens;errno=0;auto* f=std::fopen(d.path.c_str(),"rb");*out=reinterpret_cast<std::uintptr_t>(f);return f||errno==ENOENT?0:1;}
 static int close(void*,std::uintptr_t p){return !p||std::fclose(reinterpret_cast<std::FILE*>(p))?1:0;}
};
void source(dh2_script_vm* vm,const char* code){auto status=dh2_script_vm_load_source_file(vm,code,std::strlen(code));if(status)std::cerr<<dh2_script_vm_error(vm)<<'\n';check(!status);}
}
int main(int argc,char** argv){try{
 check(argc==5);Inputs raw(argv[1]);auto common=file(argv[2]),monster=file(argv[3]);CharacterGameDesign design;std::string error;check(design.initialize(raw.input,error));auto tables=design.borrow();Host host(tables);Debug debug(argv[4]);
 CharacterScriptObjects objects(design.borrow(),debug.owner,&debug.files);
 auto found=std::find(tables.characters()->names.begin(),tables.characters()->names.end(),"Crypt_Skeleton");check(found!=tables.characters()->names.end());
 auto make=[&](std::uintptr_t id,const char* name){auto props=std::make_shared<dh2::data::PropertyState>();auto life=std::make_shared<dh2::data::CombatActorState>();
  dh2::data::reset_properties(*tables.rules(),*props,&tables.characters()->rows[found-tables.characters()->names.begin()]);check(dh2::data::recalc_properties_with_class(*tables.classes(),*tables.rules(),*props,error));return objects.add(id,name,props,life,{0,0,0});};
 auto first=make(UINT64_C(0x100000101),"first"),second=make(UINT64_C(0x100000102),"second");
 CharacterScriptSessionInput in;in.identity=first->identity;in.name=first->name;in.properties=first->properties;in.combat=first->life;in.position=first->position;in.source_is_character=1;in.common={common.data(),common.size()};in.external={monster.data(),monster.size()};in.host=&host.host;in.level=&debug.level;in.target=&first->binding;in.objects=&objects.services();
 auto session=CharacterScriptSession::create(design.borrow(),in,error);check(session&&error.empty());check(!session->start()&&session->error().empty());ScriptSessionView view;check(session->owner().active(view));
 source(view.vm,"function RegisterOther(other) other_id=other:GetID() end");
 dh2_script_value id{};id.type=DH2_SCRIPT_SOURCE_OBJECT;id.identity=second->identity;
 check(!dh2_script_vm_call_discard_source_objects(view.vm,"RegisterOther",&id,1));
 source(view.vm,"assert(type(GetID())=='userdata' and not HasTarget() and GetTarget()==nil); SetTarget(other_id); saved=GetTarget(); assert(type(saved)=='table' and saved:GetID()==other_id and saved:GetTarget()==nil); assert(saved~=GetTarget() and GetTarget():GetID()==saved:GetID()); SetTarget(saved); assert(HasTarget());");
 check(first->target.target==second->identity&&first->target.last_target==second->identity&&first->target.alive==1&&first->target.sight==1&&!first->binding.scope);
 // Shared scene producers change after Lua keeps a table. The next source
 // setter reads their current dead byte and raw GameObject position.
 second->life->dead=255;second->position={1e8f,1e8f,1e8f};source(view.vm,"SetTarget(saved)");check(first->target.alive==254&&!first->target.sight);
 auto retained=second.get();second.reset();check(objects.find(UINT64_C(0x100000102)).get()==retained);
 source(view.vm,"assert(saved:GetID()==other_id); local ok,err=pcall(function() return saved:GetState() end); assert(not ok and string.find(err,'Unreconstructed scene Character')); local ok2=pcall(SetTarget, GetID()); assert(ok2); assert(GetTarget():GetID()==GetID()); ClearTarget(); assert(not HasTarget()); SetTarget(saved); proxy=newproxy(true); getmetatable(proxy).__gc=function() assert(GetTarget():GetID()==saved:GetID()); ClearTarget() end");
 check(first->target.target==UINT64_C(0x100000102));
 session.reset();
 check(!first->target.target&&!first->target.last_target&&!first->binding.scope);
 unsigned guards=0;try{objects.add(first->identity,"duplicate",first->properties,first->life,{0,0,0});check(false);}catch(const std::invalid_argument&){++guards;}
 try{objects.add(0,"zero",first->properties,first->life,{0,0,0});check(false);}catch(const std::invalid_argument&){++guards;}
 const char* type=nullptr;check(objects.services().type_name(objects.services().context,UINT64_C(0x100000999),&type)==1);++guards;
 check(debug.opens==1);
 Dl_info library{};check(dladdr(reinterpret_cast<void*>(dh2_character_ai_set_target),&library));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"guards\":"<<guards<<",\"actual_original_monster_Init\":1,\"retained_scene_records\":2,\"real_DebugSwitches_missing_file_opens\":"<<debug.opens<<",\"finalizer_target_clear\":true,\"full_enemy_AI\":false,\"world_library\":\""<<library.dli_fname<<"\"}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
