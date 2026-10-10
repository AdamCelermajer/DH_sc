// Standalone test-only unsupported Lua endpoints. These allow linking the full
// original target kernel while testing offline controller/HUD paths. They never
// return success and must never be linked into foundation_data or the game.
#include "../../../script-runtime/script_runtime.h"
extern "C" int dh2_script_vm_bind_source_values(dh2_script_vm*,const char*,dh2_script_function,void*) {return -1;}
extern "C" int dh2_script_vm_bind_source_scoped(dh2_script_vm*,const char*,dh2_script_scoped_function,void*) {return -1;}
extern "C" int dh2_script_callback_scope_valid(const dh2_script_callback_scope*) {return 0;}
