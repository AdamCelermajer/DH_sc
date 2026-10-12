#include "generic_lua_script_owner_v13.hpp"
#include "../script-runtime/script_function_alias.h"
#include <cstring>
#include <cstdio>
#include <stdexcept>
namespace dh2::scripts {
bool LuaScriptCacheOwnerV13::file(const std::string& filename,std::shared_ptr<const std::vector<std::uint8_t>>& out,bool& found,std::string& e){
 out.reset();found=false;auto i=files_.find(filename);if(i!=files_.end()){out=i->second;found=true;return true;}
 if(!services_.read_file){e="Required SAME global LuaManager file-cache producer";return false;}
 std::vector<std::uint8_t> bytes;if(!services_.read_file(services_.context,filename,bytes,found,e))return false;
 if(!found)return true;out=std::make_shared<const std::vector<std::uint8_t>>(std::move(bytes));files_.emplace(filename,out);return true;
}
namespace {
struct Descriptor{const char* name;std::uint32_t callback;};
constexpr Descriptor descriptors[]={
 {"Include",0x37efe4},{"Trace",0x37ee80},{"SetInt",0x37de5c},{"GetInt",0x37ec14},
 {"AddToVFTable",0x37ec70},{"PushVFTable",0x37be00},{"PopVFTable",0x37dc44},
 {"ToFixed",0x37ebc4},{"FromFixed",0x37ee84},{"MulFixed",0x37e1a4},{"DivFixed",0x37e068},
 {"Rand",0x37df30},{"RandF",0x37e2d8},{"BitNot",0x37f814},{"BitAnd",0x37e9ec},{"BitOr",0x37e814},{"BitXOr",0x37f750},
 {"GetPyCst",0x37f354},{"GetPyStruct",0x37f4a8},{"GetPyOID",0x37f5fc},{"CallPyScript",0x37ef3c},
 {"GetNumPlayers",0x37cbd8},{"GetHostPlayer",0x37ca60},{"GetHostPlayerLevel",0x37cc00},{"GetHostPlayerDifficulty",0x37cb8c},
 {"GetCurrentLevelRange",0x37f1f0},{"GetGameObjectsByType",0x37f03c},{"SetGameType",0x37f878},{"GetGameScript",0x37c934},
 {"OnTargetDied",0x37ddc8},{"PlayMusic",0x37e420},{"PlaySound",0x37e578},{"StopSound",0x37e730}};
int failure(char* e,std::size_t n,const std::string& text){if(e&&n)std::snprintf(e,n,"%s",text.c_str());return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
}
struct GenericLuaScriptOwnerV13::Impl {
 std::shared_ptr<LuaScriptCacheOwnerV13> cache;LuaScriptServicesV13 services;
 dh2_script_vm* vm{};dh2_script_aliases* aliases{};dh2_script_int_map* integers{};
 dh2_script_int_bindings integer_receiver{};
 dh2_script_scalar_bindings scalar_receiver{};dh2_script_design_bindings design_receiver{};bool dependency_failed{};
 std::string path;std::set<std::string> loaded;std::vector<LuaBindingObservationV13> observations;
 struct Required{std::string name;std::uint32_t callback;};std::vector<std::unique_ptr<Required>> required;
 std::vector<std::shared_ptr<void>> native_leases;
 bool busy{};int status{};
 Impl(std::shared_ptr<LuaScriptCacheOwnerV13> c,LuaScriptServicesV13 s,std::size_t limit):cache(std::move(c)),services(std::move(s)){
  vm=dh2_script_vm_create_empty(limit);if(!vm)throw std::runtime_error("Actual LuaScript Instance allocation failed");
  aliases=dh2_script_alias_create();integers=dh2_script_int_create();
  if(!aliases||!integers){if(aliases)dh2_script_alias_destroy(aliases);if(integers)dh2_script_int_destroy(integers);dh2_script_vm_destroy(vm);vm=nullptr;throw std::runtime_error("Actual LuaScript native maps allocation failed");}
  integer_receiver={integers,this,integer_identity,integer_fraction,0};
  scalar_receiver={this,scalar_identity,divide_zero,0};design_receiver={this,design_lookup,0};
 }
 ~Impl(){loaded.clear();path.clear();dh2_script_alias_clear_contents(aliases);dh2_script_int_clear_contents(integers);
  // Callback receivers/maps/services survive all source lua_close finalizers.
  dh2_script_vm_destroy(vm);dh2_script_alias_destroy(aliases);dh2_script_int_destroy(integers);}
 static int unavailable(void* p,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t* n,char* e,std::size_t z){auto& r=*static_cast<Required*>(p);*n=0;return failure(e,z,"Required original LuaScript native backend: "+r.name+" ("+std::to_string(r.callback)+")");}
 static int trace(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t* n,char*,std::size_t){*n=0;return 0;} // whole37ee80 bx lr
 static int integer_identity(void* p,std::uintptr_t value,std::uint32_t* out){auto& t=*static_cast<Impl*>(p);if(!t.services.integer_identity||t.services.integer_identity(t.services.integer_context,value,out)){t.dependency_failed=true;return 1;}return 0;}
 static int integer_fraction(void* p,float value,char* out,std::size_t size,std::size_t* written){auto& t=*static_cast<Impl*>(p);if(!t.services.integer_fraction||t.services.integer_fraction(t.services.integer_context,value,out,size,written)){t.dependency_failed=true;return 1;}return 0;}
 static int scalar_identity(void* p,std::uintptr_t value,std::uint32_t* out){auto& t=*static_cast<Impl*>(p);if(!t.services.scalar.identity||t.services.scalar.identity(t.services.scalar.context,value,out)){t.dependency_failed=true;return 1;}return 0;}
 static int divide_zero(void* p,std::int32_t a,std::int32_t* out){auto& t=*static_cast<Impl*>(p);if(!t.services.scalar.divide_zero||t.services.scalar.divide_zero(t.services.scalar.context,a,out)){t.dependency_failed=true;return 1;}return 0;}
 static int design_lookup(void* p,std::uint32_t kind,const char* a,const char* b,std::int32_t* out){auto& t=*static_cast<Impl*>(p);if(!t.services.design.lookup||t.services.design.lookup(t.services.design.context,kind,a,b,out)){t.dependency_failed=true;return 1;}return 0;}
 static int kernel(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* e,std::size_t z,dh2_script_function callback,void* receiver){auto& t=*static_cast<Impl*>(p);t.dependency_failed=false;const int status=callback(receiver,a,n,out,capacity,returned,e,z);return t.dependency_failed?DH2_SCRIPT_REQUIRED_SERVICE_FAILURE:status;}
#define SOURCE_KERNEL(name,callback,receiver) static int name(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* returned,char* e,std::size_t z){return kernel(p,a,n,o,c,returned,e,z,callback,&static_cast<Impl*>(p)->receiver);}
 SOURCE_KERNEL(set_int,dh2_script_int_set_callback,integer_receiver)
 SOURCE_KERNEL(get_int,dh2_script_int_get_callback,integer_receiver)
 SOURCE_KERNEL(to_fixed,dh2_script_scalar_to_fixed,scalar_receiver)
 SOURCE_KERNEL(from_fixed,dh2_script_scalar_from_fixed,scalar_receiver)
 SOURCE_KERNEL(mul_fixed,dh2_script_scalar_mul_fixed,scalar_receiver)
 SOURCE_KERNEL(div_fixed,dh2_script_scalar_div_fixed,scalar_receiver)
 SOURCE_KERNEL(bit_not,dh2_script_scalar_bit_not,scalar_receiver)
 SOURCE_KERNEL(bit_and,dh2_script_scalar_bit_and,scalar_receiver)
 SOURCE_KERNEL(bit_or,dh2_script_scalar_bit_or,scalar_receiver)
 SOURCE_KERNEL(bit_xor,dh2_script_scalar_bit_xor,scalar_receiver)
 SOURCE_KERNEL(get_constant,dh2_script_design_get_constant,design_receiver)
 SOURCE_KERNEL(get_struct,dh2_script_design_get_struct,design_receiver)
 SOURCE_KERNEL(get_oid,dh2_script_design_get_oid,design_receiver)
#undef SOURCE_KERNEL
 static int alias(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value*,std::uint32_t,std::uint32_t* count,char* e,std::size_t z,unsigned op){auto& t=*static_cast<Impl*>(p);*count=0;const int code=op==0?dh2_script_alias_add_values(t.aliases,a,n):op==1?dh2_script_alias_push(t.aliases):dh2_script_alias_pop(t.aliases);return code?failure(e,z,"Actual LuaScript alias map rejected"):0;}
 static int add(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* count,char* e,std::size_t z){return alias(p,a,n,o,c,count,e,z,0);}
 static int push(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* count,char* e,std::size_t z){return alias(p,a,n,o,c,count,e,z,1);}
 static int pop(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* count,char* e,std::size_t z){return alias(p,a,n,o,c,count,e,z,2);}
 bool file(const char* name,bool& result,std::string& e,const dh2_script_include_scope* scope=nullptr){
  result=false;status=0;if(!name||!*name)return true;
  std::string filename=path+name;const char* suffix=std::strstr(name,".lua");if(!suffix)filename+=".luac";else if(std::strncmp(suffix,".luac",5))filename+='c';
  if(loaded.count(filename)){result=true;return true;}
  std::shared_ptr<const std::vector<std::uint8_t>> bytes;bool found{};if(!cache||!cache->file(filename,bytes,found,e))return false;if(!found)return true;
  const auto epoch=dh2_script_vm_required_failure_epoch(vm);
  status=scope?dh2_script_include_load(scope,bytes->data(),bytes->size()):dh2_script_vm_load_source_file(vm,bytes->data(),bytes->size());
  if(dh2_script_vm_required_failure_epoch(vm)!=epoch){e="Required native backend failed during LuaScript file load: "+std::string(scope?dh2_script_include_error(scope):dh2_script_vm_error(vm));return false;}
  if(status){e=scope?dh2_script_include_error(scope):dh2_script_vm_error(vm);return status>0;}
  loaded.insert(filename);result=true;return true;
 }
 static int include(void* p,const dh2_script_include_scope* scope,const char* name,char* error,std::size_t size){auto& t=*static_cast<Impl*>(p);bool loaded{};std::string e;
  if(!scope||scope->vm!=t.vm||!dh2_script_include_scope_valid(scope))return failure(error,size,"Required SAME LuaScript Include scope");
  if(!t.file(name,loaded,e,scope))return failure(error,size,e);return 0; // source ignores delivered Load false
 }
 bool bind(std::string& e){
  for(std::uint32_t library=0;library<4;++library)if(dh2_script_vm_open_source_library(vm,library)){e=dh2_script_vm_error(vm);return false;}
  if(services.objects&&dh2_script_vm_set_source_objects(vm,services.objects)){e="Actual LuaScript object provider installation failed";return false;}
  for(const auto& d:descriptors){dh2_script_function function{};void* context{};bool builtin=true,backend=true;int code{};
   switch(d.callback){
   case 0x37efe4:code=dh2_script_vm_bind_source_include(vm,include,this);break;
   case 0x37ee80:function=trace;break;
   case 0x37de5c:function=set_int;context=this;break;
   case 0x37ec14:function=get_int;context=this;break;
   case 0x37ec70:function=add;context=this;break;case 0x37be00:function=push;context=this;break;case 0x37dc44:function=pop;context=this;break;
   case 0x37ebc4:function=to_fixed;context=this;break;case 0x37ee84:function=from_fixed;context=this;break;
   case 0x37e1a4:function=mul_fixed;context=this;break;case 0x37e068:function=div_fixed;context=this;break;
   case 0x37f814:function=bit_not;context=this;break;case 0x37e9ec:function=bit_and;context=this;break;
   case 0x37e814:function=bit_or;context=this;break;case 0x37f750:function=bit_xor;context=this;break;
   case 0x37f354:function=get_constant;context=this;backend=services.design.lookup!=nullptr;break;
   case 0x37f4a8:function=get_struct;context=this;backend=services.design.lookup!=nullptr;break;
   case 0x37f5fc:function=get_oid;context=this;backend=services.design.lookup!=nullptr;break;
   default:{builtin=false;LuaNativeBindingV13 native;std::string missing;
    backend=services.resolve_native&&services.resolve_native(services.context,d.name,d.callback,native,missing);
    if(backend){if(bool(native.values)==bool(native.scoped_values)||(native.scoped_values&&native.object_results)){e="Malformed actual LuaScript native binding "+std::string(d.name);return false;}
     native_leases.push_back(native.owner);
     code=native.scoped_values?dh2_script_vm_bind_source_scoped_values(vm,d.name,native.scoped_values,native.context):native.object_results?dh2_script_vm_bind_source_objects(vm,d.name,native.values,native.context):dh2_script_vm_bind_source_values(vm,d.name,native.values,native.context);
    }else{auto required_binding=std::make_unique<Required>();required_binding->name=d.name;required_binding->callback=d.callback;context=required_binding.get();required.push_back(std::move(required_binding));function=unavailable;}
    break;}
   }
   if(function)code=dh2_script_vm_bind_source_values(vm,d.name,function,context);
   observations.push_back({d.name,d.callback,builtin,backend,code==0});if(code){e="Actual LuaScript binder failed: "+std::string(d.name);return false;}
  }return true;
 }
};
GenericLuaScriptOwnerV13::GenericLuaScriptOwnerV13(std::unique_ptr<Impl> p):impl_(std::move(p)){}
GenericLuaScriptOwnerV13::~GenericLuaScriptOwnerV13()=default;
std::unique_ptr<GenericLuaScriptOwnerV13> GenericLuaScriptOwnerV13::create(bool deferred,std::shared_ptr<LuaScriptCacheOwnerV13> cache,LuaScriptServicesV13 services,std::size_t limit,std::string& e){
 if(!cache||services.scalar.reserved||services.design.reserved){e="Required actual LuaScript cache/services";return nullptr;}
 try{auto p=std::make_unique<Impl>(std::move(cache),std::move(services),limit);if(!deferred&&!p->bind(e))return nullptr;return std::unique_ptr<GenericLuaScriptOwnerV13>(new GenericLuaScriptOwnerV13(std::move(p)));}catch(const std::exception& x){e=x.what();return nullptr;}
}
bool GenericLuaScriptOwnerV13::bind_functions(std::string& e){if(impl_->busy||dh2_script_vm_stack_size(impl_->vm)<0){e="LuaScript binder reentry unavailable";return false;}return impl_->bind(e);}
bool GenericLuaScriptOwnerV13::assign_path(const std::string& path,std::string& e){if(impl_->busy||dh2_script_vm_stack_size(impl_->vm)<0){e="LuaScript path mutation during VM execution unavailable";return false;}impl_->path=path;return true;}
bool GenericLuaScriptOwnerV13::load(const char* name,bool& result,std::string& e){if(impl_->busy||dh2_script_vm_stack_size(impl_->vm)<0){e="Use dedicated SAME LuaScript Include capability";return false;}impl_->busy=true;struct Guard{bool& b;~Guard(){b=false;}}g{impl_->busy};return impl_->file(name,result,e);}
bool GenericLuaScriptOwnerV13::call(const char* name,const dh2_script_value* args,std::uint32_t n,bool& result,std::string& e){
 result=false;if(!name||impl_->busy||dh2_script_vm_stack_size(impl_->vm)<0){e="Required valid nonreentrant LuaScript Call";return false;}impl_->busy=true;struct Guard{bool& b;~Guard(){b=false;}}g{impl_->busy};
 const auto epoch=dh2_script_vm_required_failure_epoch(impl_->vm);impl_->status=dh2_script_vm_call_source_status_objects(impl_->vm,dh2_script_alias_resolve(impl_->aliases,name),args,n);
 if(impl_->status||dh2_script_vm_required_failure_epoch(impl_->vm)!=epoch)e=dh2_script_vm_error(impl_->vm);
 if(impl_->status<0||dh2_script_vm_required_failure_epoch(impl_->vm)!=epoch)return false;result=impl_->status==0;return true;
}
dh2_script_vm* GenericLuaScriptOwnerV13::vm_borrow()const noexcept{return impl_->vm;}
const std::string& GenericLuaScriptOwnerV13::path()const noexcept{return impl_->path;}
const std::set<std::string>& GenericLuaScriptOwnerV13::loaded_files()const noexcept{return impl_->loaded;}
const std::vector<LuaBindingObservationV13>& GenericLuaScriptOwnerV13::bindings()const noexcept{return impl_->observations;}
std::uintptr_t GenericLuaScriptOwnerV13::identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
int GenericLuaScriptOwnerV13::last_source_status()const noexcept{return impl_->status;}
}
