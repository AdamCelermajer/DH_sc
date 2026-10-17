#include "../script_runtime.h"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

namespace {
int callback(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,
             std::uint32_t,std::uint32_t* returned,char*,std::size_t) {
 *returned=0;return 0;
}
bool number(dh2_script_vm* vm,const char* name,float expected) {
 dh2_script_value value{};
 return !dh2_script_vm_get_global(vm,name,&value)&&value.type==DH2_SCRIPT_NUMBER&&value.number==expected;
}
}
int main() {
 struct Case {const char* literal;bool key,value;};
 const Case cases[]{{"",false,false},{"k",true,false},{"v",false,true},{"kv",true,true},
                    {"\\000k",false,false},{"\\000v",false,false},{"k\\000v",true,false},{"v\\000k",false,true}};
 unsigned count=0,registrations=0;
 for(const auto& item:cases) {
  auto* vm=dh2_script_vm_create(8*1024*1024);if(!vm)return 1;
  const std::string source=std::string("local mode='")+item.literal+R"lua('
    keeper={}
    keys=setmetatable({}, {__mode=mode})
    do local key={}; keys[key]=keeper end
    collectgarbage('collect'); collectgarbage('collect')
    key_count=0; for _ in pairs(keys) do key_count=key_count+1 end
    kept_key={}
    values=setmetatable({}, {__mode=mode})
    do local value={}; values[kept_key]=value end
    collectgarbage('collect'); collectgarbage('collect')
    value_count=0; for _ in pairs(values) do value_count=value_count+1 end
  )lua";
  if(dh2_script_vm_load(vm,source.data(),source.size(),"weak-mode-compatibility")||
     !number(vm,"key_count",item.key?0.f:1.f)||!number(vm,"value_count",item.value?0.f:1.f)) {
   std::cerr<<dh2_script_vm_error(vm)<<" weak mode case "<<count<<" failed\n";return 2;
  }
  // Real userdata/closure registrations repeatedly advance GC over the base
  // library's weak proxy metatable, reproducing the Android startup route.
  for(unsigned i=0;i<1000;++i) {
   const auto name="weak_gc_binding_"+std::to_string(i%37);
   if(dh2_script_vm_bind_source_values(vm,name.c_str(),callback,nullptr))return 3;
   ++registrations;
  }
  const char* final="collectgarbage('collect'); gc_finished=1";
  if(dh2_script_vm_load(vm,final,std::strlen(final),"final-gc")||!number(vm,"gc_finished",1.f))return 4;
  dh2_script_vm_destroy(vm);++count;
 }
 std::cout<<"{\"validation\":\"PASS\",\"mode_cases\":"<<count<<",\"native_closure_registrations\":"<<registrations
          <<",\"embedded_nul_preserves_strchr_semantics\":true,\"whole_original_vm_parity\":false}\n";
}
