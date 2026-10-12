#include "../gameobject_lua_representation.hpp"
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace dh2::gameobject_lua;
static unsigned checks=0,methods=0,vm_checks=0,guards=0;
static void check(bool v){++checks;if(!v)throw std::runtime_error("object audit check "+std::to_string(checks));}
struct Owner {
 dh2_script_vm* vm=nullptr;dh2::character::TargetOwner16 owner{0x100000000ull,0,0,0};
 dh2::character::TargetState48 state{0x200000000ull,&owner,0,0x100020000ull,0,0,0,0,0,0};
 dh2_script_object_services services{};unsigned type_calls=0,binding_calls=0,calls=0,unsupported=0;
 bool nested=false,fail_type=false,fail_methods=false;
 std::vector<unsigned> addresses;std::vector<std::uintptr_t> receivers;dh2_script_callback_scope expired{};
 static int type(void* p,std::uintptr_t id,const char** out){auto& o=*static_cast<Owner*>(p);++o.type_calls;if(o.fail_type)return 1;return dh2_gameobject_lua_type(out,id==o.owner.identity?0:1);}
 static int bindings(void* p,std::uintptr_t id,const dh2_script_object_method** out,std::uint32_t* n){auto& o=*static_cast<Owner*>(p);++o.binding_calls;if(o.fail_methods)return 1;return dh2_gameobject_lua_methods(out,n,id==o.owner.identity?0:1);}
 static int call(void* p,const dh2_script_callback_scope* scope,std::uintptr_t receiver,std::uint32_t address,const dh2_script_value* args,std::uint32_t count,dh2_script_value* out,std::uint32_t,std::uint32_t* returned,char* error,std::size_t capacity){
  auto& o=*static_cast<Owner*>(p);++o.calls;o.addresses.push_back(address);o.receivers.push_back(receiver);check(dh2_script_callback_scope_valid(scope));o.expired=*scope;
  check(dh2_script_vm_get_global(o.vm,"attempt",out)==-1); // no generic reentry
  if(o.nested){o.nested=false;check(!dh2_script_callback_call_discard_source(scope,"DuringMethod",nullptr,0));}
  if(address==0x38ebe4)return dh2_gameobject_lua_get_id(out,returned,receiver);
  if(address==0x3b6c7c)return dh2_gameobject_lua_get_target(out,returned,&o.state);
  if(address==0x3b6f50){check(count==0||args);std::uint32_t present;check(!dh2_character_has_target(&present,&o.state));std::memset(out,0,sizeof(*out));out->type=1;out->boolean=present;*returned=1;return 0;}
  ++o.unsupported;std::snprintf(error,capacity,"Unreconstructed source method %x",address);return 1;
 }
 static int target(void* p,const dh2_script_value*,std::uint32_t,dh2_script_value* out,std::uint32_t,std::uint32_t* returned,char*,std::size_t){return dh2_gameobject_lua_get_target(out,returned,&static_cast<Owner*>(p)->state);}
 static int base(void* p,const dh2_script_value*,std::uint32_t,dh2_script_value* out,std::uint32_t,std::uint32_t* n,char*,std::size_t){out->type=7;out->identity=static_cast<Owner*>(p)->owner.identity;*n=1;return 0;}
 Owner(){services={this,type,bindings,call};}
};
static void load(Owner& o,const char* s){check(!dh2_script_vm_load_source_file(o.vm,s,std::strlen(s)));++vm_checks;}
static unsigned word(std::ifstream& s){unsigned v;s.read(reinterpret_cast<char*>(&v),4);check(bool(s));return v;}
static std::string text(std::ifstream& s){auto n=word(s);std::string v(n,'\0');s.read(v.data(),n);check(bool(s));return v;}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream gold(argv[1],std::ios::binary);check(bool(gold));char magic[4];gold.read(magic,4);check(!std::memcmp(magic,"GOL1",4));check(word(gold)==2);
 for(unsigned kind=0;kind<2;++kind){const auto name=text(gold);const auto count=word(gold);const char* type;const dh2_script_object_method* entries;unsigned n;check(!dh2_gameobject_lua_type(&type,kind)&&name==type);check(!dh2_gameobject_lua_methods(&entries,&n,kind)&&n==count);for(unsigned i=0;i<n;++i){const auto key=text(gold);const auto address=word(gold);check(entries[i].name==key&&entries[i].source_address==address&&!entries[i].reserved);++methods;}}
 check(gold.peek()==EOF);Owner o;o.vm=dh2_script_vm_create_empty(16*1024*1024);check(o.vm&&!dh2_script_vm_open_source_libraries(o.vm));
 check(!dh2_script_vm_set_source_objects(o.vm,&o.services));check(dh2_script_vm_set_source_objects(o.vm,&o.services)==-1);++guards;
 check(!dh2_script_vm_bind_source_objects(o.vm,"GetTarget",Owner::target,&o));check(!dh2_script_vm_bind_source_objects(o.vm,"GetBase",Owner::base,&o));
 load(o,"a=GetTarget(); b=GetTarget(); assert(type(a)=='table' and a~=b and type(a._this)=='userdata'); assert(getmetatable(a)==getmetatable(b)); assert(type(a.GetID)=='function' and type(a.GetTarget)=='function'); assert(a:GetID()==b:GetID()); assert(select('#',a:GetID(1,'ignored'))==1); assert(a:HasTarget()); c=a:GetTarget(); assert(type(c)=='table' and c:GetID()==a:GetID()); local z=GetBase(); assert(type(z)=='table' and z.GetTarget==nil and type(z.GetID)=='function'); assert(z:GetID()==z._this)");
 check(o.type_calls==4&&o.binding_calls==2);++vm_checks;
 load(o,"field_count=0;for k in pairs(getmetatable(a).__index) do field_count=field_count+1 end; assert(field_count==130); assert(not pcall(function() return a:GetState() end)); assert(type(a.GetState)=='function')");check(o.unsupported==1);++vm_checks;
 // Metamethod mutation: source captures self._this before remaining args.
 load(o,"saved=a._this; side=0; a:GetID(setmetatable({}, {__index=function(_,k) side=side+1;a._this=nil;return nil end})); assert(side==1); a._this=saved");check(o.receivers.back()==o.state.target);++vm_checks;
 // Shared source registry ownership remains visible and mutable in Lua.
 load(o,"getmetatable(a).__index.Marker=19; assert(GetTarget().Marker==19); getmetatable(a).__index.GetID=function() return 'changed' end; assert(GetTarget():GetID()=='changed')");check(o.binding_calls==2);++vm_checks;
 load(o,"function DuringMethod() reentry=(reentry or 0)+1; return GetTarget() end");o.nested=true;load(o,"assert(a:HasTarget());assert(reentry==1)");check(!dh2_script_callback_scope_valid(&o.expired));++vm_checks;
 o.state.target=0;load(o,"assert(GetTarget()==nil);assert(select('#',GetTarget())==1);assert(not a:HasTarget());assert(a:GetTarget()==nil)");++vm_checks;
 // Historical source-values binder retains rejection of new object output.
 check(!dh2_script_vm_bind_source_values(o.vm,"OldTarget",Owner::target,&o));load(o,"assert(not pcall(OldTarget))");++guards;
 dh2_script_value value{};value.type=7;unsigned returned=99;check(dh2_script_vm_call(o.vm,"GetTarget",&value,1,nullptr,0,&returned)==-1);++guards;
 // Protected type/method error preserves the source's partial registry commit.
 Owner failed;failed.vm=dh2_script_vm_create_empty(1024*1024);check(failed.vm&&!dh2_script_vm_open_source_libraries(failed.vm));check(!dh2_script_vm_set_source_objects(failed.vm,&failed.services));check(!dh2_script_vm_bind_source_objects(failed.vm,"GetTarget",Owner::target,&failed));failed.fail_methods=true;load(failed,"assert(not pcall(GetTarget))");failed.fail_methods=false;load(failed,"x=GetTarget();assert(type(x)=='table' and getmetatable(x).__index==nil)");check(failed.binding_calls==1);++guards;dh2_script_vm_destroy(failed.vm);
 dh2_script_vm_destroy(o.vm);Dl_info d{};check(dladdr(reinterpret_cast<void*>(&dh2_script_vm_bind_source_objects),&d));
 std::cout<<"{\"validation\":\"PASS\",\"ordered_original_methods\":"<<methods<<",\"vm_checks\":"<<vm_checks<<",\"guards\":"<<guards<<",\"native_method_calls\":"<<o.calls<<",\"type_calls\":"<<o.type_calls<<",\"first_binding_calls\":"<<o.binding_calls<<",\"checks\":"<<checks<<",\"runtime_library\":\""<<d.dli_fname<<"\",\"whole_original_VM\":false,\"unreconstructed_methods_accept\":false}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
