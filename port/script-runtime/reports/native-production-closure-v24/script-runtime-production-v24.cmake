# Frozen production-only link recipe. Include AFTER your project() call.
# Set DH2_SCRIPT_V24_ROOT to this packet's restored workspace root.
# Target is one sole script_constants owner. Do not also compile that cpp into
# the Level facade/context/overlay. Historical script_runtime.c is excluded.
add_library(dh2_script_runtime SHARED
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lapi.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lauxlib.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lbaselib.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lcode.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/ldebug.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/ldo.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/ldump.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lfunc.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lgc.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/llex.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lmathlib.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lmem.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lobject.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lopcodes.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lparser.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lstate.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lstring.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lstrlib.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/ltable.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/ltablib.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/ltm.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lundump.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lvm.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua/lzio.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/script_constants.cpp"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/script_design_bindings.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/script_function_alias.cpp"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/script_game_bindings.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/script_int_bindings.cpp"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/script_object_bridge.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/script_runtime_return_v3.c"
 "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/script_scalar_bindings.c"
 )
target_compile_features(dh2_script_runtime PRIVATE cxx_std_17)
target_include_directories(dh2_script_runtime PUBLIC "${DH2_SCRIPT_V24_ROOT}/port/script-runtime" PRIVATE "${DH2_SCRIPT_V24_ROOT}/port/script-runtime/lua")
target_compile_options(dh2_script_runtime PRIVATE -fno-fast-math -ffp-contract=off)
target_link_libraries(dh2_script_runtime PRIVATE m)
if(ANDROID)
 target_link_options(dh2_script_runtime PRIVATE -Wl,-z,max-page-size=16384 -Wl,--no-undefined)
endif()
# Add player_profile_index_v1.cpp ONCE to your real dh2_game_data target.
