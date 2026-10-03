#include "script_runtime.h"
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
namespace {
unsigned checks=0,headers=0,truncations=0,empty_callbacks=0;
void check(bool v){++checks;if(!v){std::cerr<<"failed check "<<checks<<"\n";std::abort();}}
int load(dh2_script_vm* vm,const std::string& code){return dh2_script_vm_load(vm,code.data(),code.size(),"@audit");}
dh2_script_value get(dh2_script_vm* vm,const char* name){dh2_script_value v{};check(dh2_script_vm_get_global(vm,name,&v)==0);return v;}
int call(dh2_script_vm* vm,const char* name,const dh2_script_value* args=nullptr,unsigned count=0){unsigned n=0;return dh2_script_vm_call(vm,name,args,count,nullptr,0,&n);}
int echo(void* context,const dh2_script_value* a,uint32_t count,dh2_script_value* out,
  uint32_t capacity,uint32_t* n,char*,size_t){
  if(count!=2||capacity<2||a[0].identity!=reinterpret_cast<uintptr_t>(context))return 1;
  out[0]=a[0];out[1]=a[1];*n=2;return 0;
}
}
int main(int argc,char** argv){
  check(argc==3);std::ifstream f(argv[1],std::ios::binary);
  std::string commons((std::istreambuf_iterator<char>(f)),{});check(commons.size()==13535);
  dh2_script_vm* vm=dh2_script_vm_create(8*1024*1024);check(vm!=nullptr);
  check(dh2_script_vm_create(1)==nullptr);
  check(load(vm,"rounding=16777216+1; decimal=16777217; modulo=-3%2; power=2^3; signed_zero=-0.0")==0);
  check(get(vm,"rounding").number==16777216.f);check(get(vm,"decimal").number==16777216.f);
  check(get(vm,"modulo").number==1.f);check(get(vm,"power").number==8.f);
  check(std::signbit(get(vm,"signed_zero").number));
  std::string source="value=16777217; function child(x) local s='a\\000b'; return x+value,s end";
  size_t written=0;check(dh2_script_vm_compile(vm,source.data(),source.size(),"@roundtrip",nullptr,0,&written)==-3);
  std::vector<unsigned char> binary(written);size_t actual=0;
  check(dh2_script_vm_compile(vm,source.data(),source.size(),"@roundtrip",binary.data(),binary.size(),&actual)==0);
  check(actual==binary.size());
  const unsigned char header[12]={0x1b,0x4c,0x75,0x61,0x51,0,1,4,4,4,4,0};
  check(std::memcmp(binary.data(),header,12)==0);
  dh2_script_vm* second=dh2_script_vm_create(8*1024*1024);check(second!=nullptr);
  check(dh2_script_vm_load(second,binary.data(),binary.size(),"@binary")==0);
  check(get(second,"value").number==16777216.f);
  dh2_script_value arg{},out[2]{};arg.type=DH2_SCRIPT_NUMBER;arg.number=2;uint32_t count=0;
  check(dh2_script_vm_call(second,"child",&arg,1,out,2,&count)==0&&count==2);
  check(out[0].number==16777218.f&&out[1].text_bytes==3&&out[1].text[1]==0);
  for(unsigned index=0;index<12;index++){
    auto bad=binary;bad[index]^=0x80;
    check(dh2_script_vm_load(second,bad.data(),bad.size(),"@bad-header")==-2);++headers;
  }
  for(size_t size=1;size<binary.size();size++){
    check(dh2_script_vm_load(second,binary.data(),size,"@truncated")==-2);++truncations;
  }
  check(dh2_script_vm_load(second,binary.data(),binary.size(),"@after-errors")==0);
  dh2_script_vm_destroy(second);
  check(dh2_script_vm_load(vm,commons.data(),commons.size(),"@ai/_commons.luac")==0);
  for(const char* name:{"Trace","StartTimer","StopTimer","evnet_name","io","os","package","debug"})
    check(get(vm,name).type==DH2_SCRIPT_NIL);
  for(const char* name:{"OnInit","OnInitPost","OnInitFinal","OnTerminate","OnUpdate",
      "OnEndOfAnim","OnDied","OnRevived","OnFriendSpotted","OnEnemySpotted",
      "OnNeutralSpotted","OnTargetDied","OnTargetOutOfSight","OnTargetInSight",
      "OnTargetOutOfRange","OnTargetInRangedRange","OnTargetInCloseRange","OnTargetInMeleeRange",
      "OnProjectileHit","OnTargetMissed","OnTargetHit","OnCombatResults","OnMasterDied",
      "OnMasterOutOfSight","OnMasterInSight","OnMasterOutOfRange","OnMasterInRangedRange",
      "OnMasterInCloseRange","OnMasterInMeleeRange"}) {
    check(get(vm,name).type==DH2_SCRIPT_FUNCTION);check(call(vm,name)==0);++empty_callbacks;
  }
  check(load(vm,"observed=0; local ref={n=7}; AttachToAnimEvent('Hit',function(ev,r) observed=r.n; observed_name=ev end,ref)")==0);
  dh2_script_value event{};event.type=DH2_SCRIPT_STRING;event.text="Hit";event.text_bytes=3;
  check(call(vm,"OnAnimEvent",&event,1)==0);check(get(vm,"observed").number==7.f);
  auto name=get(vm,"observed_name");check(name.text_bytes==3&&std::memcmp(name.text,"Hit",3)==0);
  check(load(vm,"DetachFromAnimEvent('Hit'); observed=0")==0);
  check(call(vm,"OnAnimEvent",&event,1)==0);check(get(vm,"observed").number==0.f);
  check(load(vm,"AttachToAnimEvent('Duplicate',function() end); AttachToAnimEvent('Duplicate',function() end)")==-2);
  check(std::strstr(dh2_script_vm_error(vm),"evnet_name")!=nullptr);
  check(load(vm,"StartTimerCB(100,false,function() end)")==-2);
  check(std::strstr(dh2_script_vm_error(vm),"StartTimer")!=nullptr);
  check(load(vm,"StopTimerCB(1)")==-2);
  check(std::strstr(dh2_script_vm_error(vm),"StopTimer")!=nullptr);
  constexpr uintptr_t identity=0x123456789abcdef0ULL;
  check(dh2_script_vm_bind(vm,"AuditEcho",echo,reinterpret_cast<void*>(identity))==0);
  dh2_script_value args[2]{};args[0].type=2;args[0].identity=identity;args[1].type=3;args[1].number=3.25f;
  check(dh2_script_vm_call(vm,"AuditEcho",args,2,out,2,&count)==0&&count==2);
  check(out[0].identity==identity&&out[1].number==3.25f);
  args[0].reserved=1;check(dh2_script_vm_call(vm,"AuditEcho",args,2,out,2,&count)==-1);
  check(call(vm,"MissingFunction")==-2);
  check(load(vm,"error({})")==-2);check(std::strstr(dh2_script_vm_error(vm),"no text")!=nullptr);
  written=0;check(dh2_script_vm_compile(vm,commons.data(),commons.size(),"@ai/_commons.luac",nullptr,0,&written)==-3);
  binary.resize(written);check(dh2_script_vm_compile(vm,commons.data(),commons.size(),"@ai/_commons.luac",binary.data(),binary.size(),&actual)==0);
  std::ofstream dump(argv[2],std::ios::binary);dump.write(reinterpret_cast<const char*>(binary.data()),binary.size());dump.close();
  dh2_script_vm_destroy(vm);
  auto bounded=dh2_script_vm_create(65536);check(bounded!=nullptr);
  check(load(bounded,"large=string.rep('x',1000000)")==-2);
  check(dh2_script_vm_memory(bounded)<=65536);check(load(bounded,"recovered=3")==0);
  check(get(bounded,"recovered").number==3.f);dh2_script_vm_destroy(bounded);
  std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks
    <<",\"header_rejections\":"<<headers<<",\"truncation_rejections\":"<<truncations
    <<",\"actual_empty_callbacks\":"<<empty_callbacks
    <<",\"actual_commons_loaded\":true,\"pure_anim_event_callbacks\":2,\"required_game_bindings_installed\":0,\"full_game_bindings\":false,\"mismatches\":0}\n";
}
