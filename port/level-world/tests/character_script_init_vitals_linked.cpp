// Observe real calls from the production DSO without compiling a second kernel.
#include <dlfcn.h>
#define __wrap_dh2_property_add dh2_property_add
#define __real_dh2_property_add central_real_property_add
#include "character_script_init_vitals.cpp"
#undef __wrap_dh2_property_add
#undef __real_dh2_property_add
extern "C" unsigned central_real_property_add(dh2::data::PropertyView* view,
 std::int32_t property,std::int32_t amount){
 using Function=unsigned(*)(dh2::data::PropertyView*,std::int32_t,std::int32_t);
 static auto implementation=reinterpret_cast<Function>(dlsym(RTLD_NEXT,"dh2_property_add"));
 if(!implementation)throw std::runtime_error("Production PropertyAdd symbol missing");
 return implementation(view,property,amount);
}
