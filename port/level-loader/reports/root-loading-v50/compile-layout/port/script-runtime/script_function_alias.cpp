#include "script_function_alias.h"
#include <map>
#include <string>
#include <cstring>
#include <exception>

struct dh2_script_aliases {
 std::map<uint32_t,std::string> names,backup;
 bool recording=false;
};
extern "C" dh2_script_aliases* dh2_script_alias_create(void){try{return new dh2_script_aliases;}catch(...){return nullptr;}}
extern "C" void dh2_script_alias_destroy(dh2_script_aliases* a){delete a;}
extern "C" uint32_t dh2_script_alias_hash(const char* name){
 if(!name)return 0;
 uint32_t hash=0;
 for(const auto* p=reinterpret_cast<const unsigned char*>(name);*p;++p){
  const uint32_t character=static_cast<uint32_t>(*p<128?int(*p):int(*p)-256);
  hash^=character+0x9e3779b9u+(hash<<6)+(hash>>2);
 }
 return hash;
}
extern "C" const char* dh2_script_alias_resolve(const dh2_script_aliases* a,const char* requested){
 if(!a||!requested)return nullptr;
 const auto found=a->names.find(dh2_script_alias_hash(requested));
 return found==a->names.end()?requested:found->second.c_str();
}
extern "C" int dh2_script_alias_contains(const dh2_script_aliases* a,const char* name){
 if(!a||!name)return -1;return a->names.count(dh2_script_alias_hash(name))!=0;
}
extern "C" int dh2_script_alias_add(dh2_script_aliases* a,const char* name,const char* replacement){
 if(!a||!name||!replacement)return -1;
 try{
  const auto key=dh2_script_alias_hash(name);
  if(a->recording&&a->backup.find(key)==a->backup.end()){
   // Both source map::operator[] calls occur before the old string is copied.
   auto& prior=a->backup[key];auto& current=a->names[key];prior=current;
  }
  a->names[key]=replacement;return 0;
 }catch(...){return -2;}
}
extern "C" int dh2_script_alias_push(dh2_script_aliases* a){
 if(!a)return -1;a->backup.clear();a->recording=true;return 0;
}
extern "C" int dh2_script_alias_clear_contents(dh2_script_aliases* a){
 if(!a)return -1;
 a->backup.clear();a->names.clear();return 0;
}
extern "C" int dh2_script_alias_pop(dh2_script_aliases* a){
 if(!a)return -1;
 try{
  for(const auto& prior:a->backup){
   if(prior.second.empty())a->names.erase(prior.first);
   else a->names[prior.first]=prior.second;
  }
  a->backup.clear();a->recording=false;return 0;
 }catch(...){return -2;}
}
extern "C" int dh2_script_alias_add_values(dh2_script_aliases* a,const dh2_script_value* args,uint32_t count){
 if(!a||(count&&!args))return -1;
 if(count<2||args[0].type!=DH2_SCRIPT_STRING||args[1].type!=DH2_SCRIPT_STRING)return 0;
 return dh2_script_alias_add(a,args[0].text,args[1].text);
}
namespace {
int callback(void* raw,const dh2_script_value* args,uint32_t count,
 dh2_script_value*,uint32_t,uint32_t* results,char* error,size_t capacity,int op){
 if(!raw||!results||(count&&!args))return -1;*results=0;
 int status;
 if(op==0)status=dh2_script_alias_push(static_cast<dh2_script_aliases*>(raw));
 else if(op==1)status=dh2_script_alias_pop(static_cast<dh2_script_aliases*>(raw));
 else status=dh2_script_alias_add_values(static_cast<dh2_script_aliases*>(raw),args,count);
 if(status&&error&&capacity){const char* message="Native script alias operation failed";
  std::strncpy(error,message,capacity-1);error[capacity-1]=0;}
 return status;
}
int push(void* c,const dh2_script_value* a,uint32_t n,dh2_script_value* r,uint32_t cap,uint32_t* out,char* e,size_t ec){return callback(c,a,n,r,cap,out,e,ec,0);}
int pop(void* c,const dh2_script_value* a,uint32_t n,dh2_script_value* r,uint32_t cap,uint32_t* out,char* e,size_t ec){return callback(c,a,n,r,cap,out,e,ec,1);}
int add(void* c,const dh2_script_value* a,uint32_t n,dh2_script_value* r,uint32_t cap,uint32_t* out,char* e,size_t ec){return callback(c,a,n,r,cap,out,e,ec,2);}
}
extern "C" int dh2_script_alias_bind(dh2_script_vm* vm,dh2_script_aliases* a){
 if(!vm||!a)return -1;
 int status=dh2_script_vm_bind_source_values(vm,"AddToVFTable",add,a);if(status)return status;
 status=dh2_script_vm_bind_source_values(vm,"PushVFTable",push,a);if(status)return status;
 return dh2_script_vm_bind_source_values(vm,"PopVFTable",pop,a);
}
extern "C" int dh2_script_alias_call_discard_source(dh2_script_vm* vm,const dh2_script_aliases* a,
 const char* name,const dh2_script_value* arguments,uint32_t count){
 if(!vm||!a||!name)return -1;
 return dh2_script_vm_call_discard_source(vm,dh2_script_alias_resolve(a,name),arguments,count);
}
