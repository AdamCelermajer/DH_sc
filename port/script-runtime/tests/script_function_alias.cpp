#include "../script_function_alias.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
namespace {
unsigned checks=0;
void check(bool ok,const char* why){++checks;if(!ok)throw std::runtime_error(why);}
std::vector<unsigned char> read(const char* name){std::ifstream f(name,std::ios::binary);check(bool(f),"missing fixture");return {std::istreambuf_iterator<char>(f),{}};}
struct Reader {
 const std::vector<unsigned char>& b;size_t p=0;
 uint32_t word(){check(p+4<=b.size(),"truncated word");uint32_t v;std::memcpy(&v,b.data()+p,4);p+=4;return v;}
 std::string text(size_t n){check(p+n<=b.size(),"truncated text");std::string s(reinterpret_cast<const char*>(b.data()+p),n);p+=n;return s;}
};
using Aliases=std::unique_ptr<dh2_script_aliases,decltype(&dh2_script_alias_destroy)>;
using VM=std::unique_ptr<dh2_script_vm,decltype(&dh2_script_vm_destroy)>;
int load(dh2_script_vm* vm,const char* source){return dh2_script_vm_load(vm,source,std::strlen(source),"alias-audit");}
float global(dh2_script_vm* vm,const char* name){dh2_script_value v{};check(!dh2_script_vm_get_global(vm,name,&v)&&v.type==DH2_SCRIPT_NUMBER,"global number absent");return v.number;}
}
int main(int argc,char** argv){try{
 if(argc!=3)return 2;const auto gold=read(argv[1]);Reader r{gold};check(r.text(4)=="FAL1","wrong corpus");const auto cases=r.word();
 Aliases a(dh2_script_alias_create(),dh2_script_alias_destroy);check(bool(a),"map allocation failed");
 unsigned resolutions=0,hashes=0,guards=0;
 for(unsigned i=0;i<cases;++i){
  const auto input_size=r.word(),output_size=r.word();const auto begin=r.p;
  auto op=r.word(),n=r.word(),m=r.word(),kind0=r.word(),kind1=r.word(),count=r.word();
  const auto name=r.text(n),replacement=r.text(m);check(r.p==begin+input_size,"input extent differs");const auto end=r.p+output_size;
  if(op==0){check(dh2_script_alias_hash(name.c_str())==r.word(),"original hash differs");++hashes;}
  else if(op==1){const auto identity=r.word(),length=r.word();const auto expected=r.text(length);const char* out=dh2_script_alias_resolve(a.get(),name.c_str());
   check(out&&std::string(out)==expected,"original mapped text differs");check((out==name.c_str())==bool(identity),"original hit/miss pointer identity differs");++resolutions;
  }else if(op==2)check(dh2_script_alias_contains(a.get(),name.c_str())==int(r.word()),"original contains differs");
  else if(op==3){std::vector<dh2_script_value> values(count);if(count){values[0].type=kind0;values[0].text=name.c_str();}if(count>1){values[1].type=kind1;values[1].text=replacement.c_str();}
   check(!dh2_script_alias_add_values(a.get(),values.data(),count),"source add failed");guards+=count<2||kind0!=4||kind1!=4;
  }else if(op==4)check(!dh2_script_alias_push(a.get()),"push failed");
  else if(op==5)check(!dh2_script_alias_pop(a.get()),"pop failed");
  else if(op==6){a.reset(dh2_script_alias_create());check(bool(a),"constructor failed");}
  else check(false,"unknown operation");
  check(r.p==end,"output extent differs");
 }
 check(r.p==gold.size(),"gold suffix");const auto replay_checks=checks;
 // Source copies replacement strings, and a hit remains an owned map pointer.
 char mutable_value[]{'c','o','p','y',0};check(!dh2_script_alias_add(a.get(),"Copied",mutable_value),"copied add");mutable_value[0]='X';
 check(!std::strcmp(dh2_script_alias_resolve(a.get(),"Copied"),"copy"),"replacement was borrowed");
 const char* retained=dh2_script_alias_resolve(a.get(),"Copied");
 check(!dh2_script_alias_add(a.get(),"Another","different")&&dh2_script_alias_resolve(a.get(),"Copied")==retained,"unrelated insertion invalidated mapped identity");
 check(dh2_script_alias_hash("Alias16038")==dh2_script_alias_hash("Alias16275"),"original hash collision absent");
 check(!dh2_script_alias_add(a.get(),"Alias16038","collision")&&!std::strcmp(dh2_script_alias_resolve(a.get(),"Alias16275"),"collision"),"collision checked string name");
 check(!dh2_script_alias_add(a.get(),"A","B")&&!dh2_script_alias_add(a.get(),"B","C"),"chain setup");
 check(!std::strcmp(dh2_script_alias_resolve(a.get(),"A"),"B"),"alias resolution recursively followed chain");
 VM vm(dh2_script_vm_create(4*1024*1024),dh2_script_vm_destroy);check(bool(vm),"VM allocation");check(!dh2_script_alias_bind(vm.get(),a.get()),"binding install");
 check(!load(vm.get(),"hits=0; function B() hits=hits+10; return {tag='ignored'} end; function C() hits=hits+100 end"),"script load");
 check(!dh2_script_alias_call_discard_source(vm.get(),a.get(),"A",nullptr,0)&&global(vm.get(),"hits")==10,"resolved call selected wrong global");
 check(!load(vm.get(),"function B() hits=hits+20 end"),"live global replacement");
 check(!dh2_script_alias_call_discard_source(vm.get(),a.get(),"A",nullptr,0)&&global(vm.get(),"hits")==30,"cached old function closure");
 // Wrong types silently return zero values; ignored extra tables still receive
 // source Value projection and their ordinary _this metamethod can mutate aliases.
 check(!load(vm.get(),"arity=select('#',AddToVFTable(7,'bad')); AddToVFTable('Nul','first\\0suffix'); effects=0; AddToVFTable('A','B',setmetatable({},{__index=function(t,k) effects=effects+1; AddToVFTable('Side','B'); return nil end})); PushVFTable(); AddToVFTable('A','C'); AddToVFTable('A','ignored'); PopVFTable()"),"binding script");
 check(global(vm.get(),"arity")==0&&global(vm.get(),"effects")==1,"source return arity/projection effects differ");
 check(!std::strcmp(dh2_script_alias_resolve(a.get(),"A"),"B")&&!std::strcmp(dh2_script_alias_resolve(a.get(),"Nul"),"first"),"copy/backup/NUL semantics differ");
 check(dh2_script_alias_contains(a.get(),"Side")==1,"synchronous projection mutation absent");
 // Calls can mutate alias values during their Lua body without caching closure
 // identity or recursively re-entering the public VM call ABI.
 check(!load(vm.get(),"function B() AddToVFTable('A','C'); hits=hits+1 end"),"self modification setup");
 check(!dh2_script_alias_call_discard_source(vm.get(),a.get(),"A",nullptr,0)&&global(vm.get(),"hits")==31,"first self modifying call");
 check(!dh2_script_alias_call_discard_source(vm.get(),a.get(),"A",nullptr,0)&&global(vm.get(),"hits")==131,"next call did not use live alias");
 // Exact commons has no alias producers. Execute its actual bytes in a fresh
 // native VM/map with real alias globals; timer globals remain absent here.
 Aliases commons_aliases(dh2_script_alias_create(),dh2_script_alias_destroy);VM commons(dh2_script_vm_create(4*1024*1024),dh2_script_vm_destroy);
 check(bool(commons)&&bool(commons_aliases)&&!dh2_script_alias_bind(commons.get(),commons_aliases.get()),"commons setup");const auto source=read(argv[2]);
 check(!dh2_script_vm_load(commons.get(),source.data(),source.size(),"ai/_commons"),"actual commons load failed");
 const char* on_timer="OnTimer";check(!dh2_script_alias_contains(commons_aliases.get(),on_timer)&&dh2_script_alias_resolve(commons_aliases.get(),on_timer)==on_timer,"commons installed an alias");
 dh2_script_value missing{};check(!dh2_script_vm_get_global(commons.get(),"StartTimer",&missing)&&missing.type==DH2_SCRIPT_NIL,"fake timer installed");
 check(dh2_script_alias_add(nullptr,"A","B")==-1&&dh2_script_alias_contains(nullptr,"A")==-1&&!dh2_script_alias_resolve(nullptr,"A"),"malformed map guard");
 check(dh2_script_alias_add(a.get(),nullptr,"B")==-1&&dh2_script_alias_add_values(a.get(),nullptr,2)==-1&&dh2_script_alias_bind(nullptr,a.get())==-1,"malformed value guard");
 std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<cases<<",\"hash_cases\":"<<hashes<<",\"lookup_cases\":"<<resolutions<<",\"source_guard_cases\":"<<guards<<",\"replay_checks\":"<<replay_checks<<",\"integration_checks\":"<<checks-replay_checks<<",\"commons_alias_absent\":true,\"mismatches\":0}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 3;}}
