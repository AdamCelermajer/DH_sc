#include "external_monster_natives.hpp"
#include "../../../script-runtime/script_return_observer_v3.h"
#include <cstring>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation::enemy_ai;
void check(bool v,const char* e){if(!v)throw std::runtime_error(e);}
int observe(void* p,const dh2_script_first_return_v1* v,char*,std::size_t){*static_cast<unsigned*>(p)=v->count&&v->type==DH2_SCRIPT_BOOLEAN&&v->boolean;return 0;}
int main(){dh2_script_vm* vm=nullptr;try{
 vm=dh2_script_vm_create(1024*1024);check(vm,"VM constructor failed");
 ExternalMonsterDoSkill b;b.character=7;b.receiver_lease=std::make_shared<int>(0);bool missing=false;unsigned calls=0;
 b.get_unsigned=[](const dh2_script_value& a,std::uint32_t& v,std::string&){v=static_cast<unsigned>(a.number);return true;};
 b.skill_count=[](std::uint32_t& n,std::string&){n=1;return true;};
 b.get_number_integer=[](const dh2_script_value& a,std::int32_t& v,std::string&){v=static_cast<int>(a.number);return true;};
 b.use_skill=[&](std::int32_t i,std::string& e){
  check(i==0&&b.current_scope&&b.current_scope->vm==vm,"same native DoSkill capability unavailable");++calls;
  const char* callbacks[]={missing?"MissingCheck":"Check","Pre","Use","Post"};
  for(const auto* callback:callbacks){unsigned answer=0;
   const int code=dh2_script_callback_call_indexed_source_v112(b.current_scope,callback,nullptr,0,0,observe,&answer);
   if(code){e=dh2_script_vm_error(vm);return false;}
   if(!missing&&std::strcmp(callback,"Check")==0&&!answer){e="source Check returned false";return false;}
  }return true;
 };
 check(bind_external_monster_do_skill_scoped(vm,&b)==0,"scoped source registration failed");
 const char* script="prefix=0;tail=0;checked=0;pre=0;used=0;post=0;function Check() checked=checked+1;return true end;function Pre() pre=pre+1 end;function Use() used=used+1 end;function Post() post=post+1 end;function EnemyMelee() prefix=prefix+1;DoSkill(0);tail=tail+1 end;function State() return prefix,tail,checked,pre,used,post end;function CatchNativeFailure() prefix=prefix+1;pcall(DoSkill,0);tail=tail+1 end";
 check(dh2_script_vm_load(vm,script,std::strlen(script),"native-scope-test")==0,"test source load failed");
 check(dh2_script_vm_call_source_status_objects(vm,"EnemyMelee",nullptr,0)==0&&calls==1&&!b.current_scope,"nested genuine VM Check failed or scope retained");
 dh2_script_value out[6]{};unsigned n=0;check(dh2_script_vm_call(vm,"State",nullptr,0,out,6,&n)==0&&n==6&&out[0].number==1&&out[1].number==1&&out[2].number==1&&out[3].number==1&&out[4].number==1&&out[5].number==1,"actual Check/Pre/Use/Post VM mutations missing");
 missing=true;check(dh2_script_vm_call_source_status_objects(vm,"CatchNativeFailure",nullptr,0)==DH2_SCRIPT_REQUIRED_FAILURE_STATUS&&!b.current_scope,"caught missing native Check became success");
 check(dh2_script_vm_call(vm,"State",nullptr,0,out,6,&n)==0&&out[0].number==2&&out[1].number==2&&out[2].number==1&&out[3].number==1&&out[4].number==1&&out[5].number==1,"failure prefix rolled back or later skill callbacks ran after failed Check");
 dh2_script_vm_destroy(vm);std::cout<<"PASS genuine same-VM nested Check/Pre/Use/Post callbacks, capability restoration and caught required failure prefix\n";return 0;
 }catch(const std::exception& e){if(vm)dh2_script_vm_destroy(vm);std::cerr<<e.what()<<'\n';return 1;}}
