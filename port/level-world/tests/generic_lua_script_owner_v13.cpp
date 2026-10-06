#include "generic_lua_script_owner_v13.hpp"
#include "character_game_design.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh2;using namespace scripts;
unsigned checks{};void check(bool value){++checks;if(!value)throw std::runtime_error("check "+std::to_string(checks));}
using Raw=std::vector<std::uint8_t>;
std::uint32_t word(std::istream& f){std::uint32_t n{};f.read(reinterpret_cast<char*>(&n),4);check(bool(f));return n;}
Raw blob(std::istream& f){Raw b(word(f));f.read(reinterpret_cast<char*>(b.data()),b.size());check(bool(f));return b;}
Raw file(const std::string& name){std::ifstream f(name,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
struct Inputs{std::array<std::array<Raw,3>,5> tables;std::vector<Raw> constants;std::vector<data::Bytes> views;character::GameDesignInputs256 input;
 Inputs(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f)&&word(f)==0x314f4447);for(auto& t:tables)for(auto& b:t)b=blob(f);auto n=word(f);for(unsigned i=0;i<n;++i){blob(f);constants.push_back(blob(f));}n=word(f);for(unsigned i=0;i<n;++i)blob(f);check(f.peek()==EOF);
  character::GameDesignTableInput48* t[]={&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};for(unsigned i=0;i<5;++i)*t[i]={{tables[i][0].data(),tables[i][0].size()},{tables[i][1].data(),tables[i][1].size()},{tables[i][2].data(),tables[i][2].size()}};
  for(const auto& b:constants)views.push_back({b.data(),b.size()});input.constants=views.data();input.constant_count=views.size();
 }};
struct Cache{std::map<std::string,Raw> files;unsigned reads{};
 static bool read(void* p,const std::string& name,Raw& out,bool& found,std::string&){auto& c=*static_cast<Cache*>(p);++c.reads;auto i=c.files.find(name);found=i!=c.files.end();if(found)out=i->second;return true;}};
void eval(GenericLuaScriptOwnerV13& owner,const std::string& code){check(!dh2_script_vm_load_source_file(owner.vm_borrow(),code.data(),code.size()));}
dh2_script_value get(GenericLuaScriptOwnerV13& owner,const char* name){dh2_script_value v{};check(!dh2_script_vm_get_global(owner.vm_borrow(),name,&v));return v;}
void fixture(Cache& c,const std::string& name,const std::string& text){c.files["data/scripts/fixture/"+name+".luac"]={text.begin(),text.end()};}
int main(int argc,char** argv){try{
 check(argc==3);Inputs inputs(argv[1]);character::CharacterGameDesign design;std::string e;check(design.initialize(inputs.input,e));auto pin=std::make_shared<character::CharacterGameDesign::Borrow>(design.borrow());
 Cache transport;transport.files["data/scripts/level/combat_formulas.luac"]=file(std::string(argv[2])+"/combat_formulas.luac");transport.files["data/scripts/level/death_scripts.luac"]=file(std::string(argv[2])+"/death_scripts.luac");
 auto cache=std::make_shared<LuaScriptCacheOwnerV13>(LuaScriptCacheServicesV13{pin,&transport,Cache::read});LuaScriptServicesV13 services;services.owner=pin;services.design=*pin->design();
 auto first=GenericLuaScriptOwnerV13::create(false,cache,services,16u*1024u*1024u,e);check(bool(first));check(first->bindings().size()==33&&dh2_script_vm_stack_size(first->vm_borrow())==5);
 check(get(*first,"RegisterAIState").type==DH2_SCRIPT_NIL&&get(*first,"ChangeAIState").type==DH2_SCRIPT_NIL);
 check(first->assign_path("data/scripts/",e));bool loaded{};check(first->load("level/combat_formulas",loaded,e)&&loaded);check(first->load("level/death_scripts",loaded,e)&&loaded);
 check(first->loaded_files().size()==2&&cache->cached_count()==2&&transport.reads==2&&first->last_source_status()==0);
 check(get(*first,"CF_CalcDamage").type==DH2_SCRIPT_FUNCTION&&get(*first,"CF_ClearCombatants").type==DH2_SCRIPT_FUNCTION&&get(*first,"LocalGameOver").type==DH2_SCRIPT_FUNCTION);
 bool success{};check(first->call("CF_ClearCombatants",nullptr,0,success,e)&&success);check(first->call("CF_CalcDamage",nullptr,0,success,e)&&success); // real authored no-combatants branch/Trace
 check(first->load("level/combat_formulas.lua",loaded,e)&&loaded&&transport.reads==2);
 auto second=GenericLuaScriptOwnerV13::create(false,cache,services,16u*1024u*1024u,e);check(bool(second)&&second->vm_borrow()!=first->vm_borrow());check(second->assign_path("data/scripts/",e));check(second->load("level/combat_formulas",loaded,e)&&loaded&&transport.reads==2);
 eval(*first,"SetInt('one',77)");eval(*second,"independent=GetInt('one')");check(get(*second,"independent").number==0);
 fixture(transport,"inner","SetInt('nested',GetInt('nested')+1)");fixture(transport,"outer","Include('fixture/inner');Include('fixture/inner');SetInt('outer',1)");check(first->load("fixture/outer",loaded,e)&&loaded);eval(*first,"n=GetInt('nested')");check(get(*first,"n").number==1&&first->loaded_files().count("data/scripts/fixture/inner.luac"));
 fixture(transport,"error","SetInt('prefix',GetInt('prefix')+1);error('source error fixture')");check(first->load("fixture/error",loaded,e)&&!loaded&&first->last_source_status()>0);check(first->load("fixture/error",loaded,e)&&!loaded);eval(*first,"prefix=GetInt('prefix')");check(get(*first,"prefix").number==2&&!first->loaded_files().count("data/scripts/fixture/error.luac"));
 fixture(transport,"required","pcall(Rand,0,100);SetInt('after_missing',1)");check(!first->load("fixture/required",loaded,e)&&!loaded&&!first->loaded_files().count("data/scripts/fixture/required.luac"));eval(*first,"after=GetInt('after_missing')");check(get(*first,"after").number==1);
 check(!first->call("LocalGameOver",nullptr,0,success,e)&&!success&&e.find("PlaySound")!=std::string::npos);
 check(first->load(nullptr,loaded,e)&&!loaded);check(first->load("",loaded,e)&&!loaded);check(first->load("missing-original-file",loaded,e)&&!loaded);
 transport.files["data/scripts/fixture/x.lua.tailc"]={};transport.files["data/scripts/fixture/x.luac.tail"]={};
 check(first->load("fixture/x.lua.tail",loaded,e)&&loaded&&first->loaded_files().count("data/scripts/fixture/x.lua.tailc"));
 check(first->load("fixture/x.luac.tail",loaded,e)&&loaded&&first->loaded_files().count("data/scripts/fixture/x.luac.tail"));
 auto deferred=GenericLuaScriptOwnerV13::create(true,cache,services,16u*1024u*1024u,e);check(deferred&&deferred->bindings().empty()&&dh2_script_vm_stack_size(deferred->vm_borrow())==0);check(deferred->bind_functions(e)&&dh2_script_vm_stack_size(deferred->vm_borrow())==5);check(deferred->bind_functions(e)&&dh2_script_vm_stack_size(deferred->vm_borrow())==10&&deferred->bindings().size()==66);
 eval(*first,"function OriginalAlias() SetInt('alias_called',1);return 1,2,3 end;AddToVFTable('VirtualAlias','OriginalAlias')");check(first->call("VirtualAlias",nullptr,0,success,e)&&success);eval(*first,"alias_called=GetInt('alias_called')");check(get(*first,"alias_called").number==1);
 unsigned finalized{};check(!dh2_script_vm_bind_source_values(first->vm_borrow(),"ObserveClose",[](void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value*,std::uint32_t,std::uint32_t* count,char*,std::size_t){check(n==1&&a[0].number==0);++*static_cast<unsigned*>(p);*count=0;return 0;},&finalized));
 eval(*first,"SetInt('one',77);guard=newproxy(true);getmetatable(guard).__gc=function() ObserveClose(GetInt('one'));SetInt('still_live_during_close',42) end");first.reset();check(finalized==1&&cache->cached_count()>=2);
 const auto reads=transport.reads;cache->flush_buffered_files();check(cache->cached_count()==0);check(second->load("level/combat_formulas",loaded,e)&&loaded&&transport.reads==reads); // source per-Script loaded membership survives manager flush
 std::cout<<"PASS generic LuaScript original33 bindings/private Instances, SAME real design/global file cache, actual Level combat/death bytes, scoped Include, loaded-set/error/required prefixes, alias/int maps and close ordering; checks="<<checks<<'\n';
 }catch(const std::exception& x){std::cerr<<x.what();return 1;}}
