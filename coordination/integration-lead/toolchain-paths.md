# Working toolchain paths

PATH absence is not a toolchain blocker. Invoke these executables directly:

- LLVM compiler: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe`
- CMake, CTest and Ninja: `C:/Users/adamc/AppData/Local/Android/Sdk/cmake/3.22.1/bin/`
- Python: `C:/Users/adamc/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe`

Root owns `.local-inputs/windows-foundation-build` and `features/actor_frame/source_character_owner_factory_native_build`. Ask before reusing their writable build artifacts. Feature-specific objects/tests may use separate output paths.

`skill_menu_luna` exclusively refreshes WSL `/home/adampalace/dh2-world-build` and `.local-inputs/character-menu-native-v1-host/snapshot` from authoritative current sources. Preserve reference/golden files. Old Oct 5 DSOs are ABI-incompatible with current Scene headers; a successful link against that snapshot does not establish current runtime acceptance.
