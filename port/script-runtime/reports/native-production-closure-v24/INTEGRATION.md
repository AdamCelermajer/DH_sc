# Native Lua/runtime production closure

This is the current root application's exact script-runtime translation-unit
list on both ARM64 and x86_64, with public/internal headers and Lua headers.
It supplies the Level constructor packet's real GenericLua and Save consumers.
It is not full Level Init, current GSLevel publication, loot or campaign readiness.

Restore paths unchanged. Set DH2_SCRIPT_V24_ROOT and include the supplied
production-only CMake recipe when dh2_script_runtime has not been defined.
Use LANGUAGES C CXX in the enclosing project. Link Level/context owners to this
ONE library. Remove the old overlay compilation of script_constants.cpp; it
is compiled inside this library exactly once. Never add script_runtime.c,
script_runtime_return_v1.c or an alternate copy of Lua alongside V3.
The real return engine is script_runtime_return_v3.c and its object bridge;
function-alias/scalar/design/game/int bindings and constants are all included.

Add player_profile_index_v1.cpp once to the same actual game-data target,
with the supplied matching header/data.hpp. It indexes actual campaign
count/size/tag streams, not dh2_settings.savegame or a fabricated empty save.

Included test sources are audit references, not an independent complete host
project. Existing root proof/build receipts are included as evidence. The
root candidate was built coherently for both Android ABIs; this packet does
not prove the loader's new integrated application until it builds and runs.
