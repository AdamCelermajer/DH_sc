# Source-built Lua runtime for modern Android

The original engine contains a Lua 5.1.4 version string. This module imports
56 exact C/header/license files from the official
[Lua 5.1.4 archive](https://www.lua.org/ftp/lua-5.1.4.tar.gz), whose published
[SHA-256](https://www.lua.org/ftp/) is
`b038e225eaf2a5b57c9bcc35cd13aa8c6c8288ef493d52970c9545074098af3a`.
The archive has 216,679 bytes. The exact MIT license is retained in
`vendor/lua-5.1.4/COPYRIGHT`; [the manifest](vendor-manifest.json) pins each
imported file. Upstream bytes are unchanged. The original game library may
contain local changes: original interpreter equivalence has not been tested.

The new owned C wrapper installs the base, math, table and string libraries
observed in the original registration callers. It removes filesystem loaders
and `print`; it does not install the game engine callbacks. The base opener
also supplies upstream coroutine support. This environment is a reconstruction
component, not the complete original scripting environment or a security sandbox.

The wrapper supports source-only compilation and controlled execution of
authored test snippets. It caps Lua allocation and sets an instruction hook
in blocks of 1,000 instructions. It clears stack/hook state after each call,
reports errors and owns the Lua state's lifetime. These ownership/budget rules
are new code. Callers use one thread, avoid reentry and provide independent
source/error buffers. Source limit is 1 MiB; runtime memory limit is 256 KiB
through 64 MiB. No file path resolver or engine object model is implemented.

## Checked results

Host builds and selftests pass, including ASan/UBSan for the selftest.
NDK r29 builds ARM64 and x86_64 shared libraries and standalone runners.
ARM64 load segments align to 16 KiB. Upstream indentation/empty-body warnings
are retained in build reports; no upstream source was changed to silence them.

The exact x86_64 runner passed on both official Android 17 emulators:
SDK 37, 4 KiB pages and 16 KiB pages. All 220 staged source inputs, the list
and runner were hash-checked on each device. In all three environments,
218 exact original game scripts pass parsing, the unchanged sandworm original
returns syntax status 3, and its separate override passes. Original scripts
are compiled only and **never executed**. Runtime tests execute authored snippets
for standard library access, missing game callbacks, instruction/memory limits,
error recovery, compile-only behavior and invalid/bytecode inputs.

- [Host build/sanitizer evidence](host-build-validation.json)
- [Android cross-build evidence](android-build-validation.json)
- [Host corpus](host-corpus-validation.json)
- [Android 17 / 4 KiB](android-4k-validation.json)
- [Android 17 / 16 KiB](android-16k-validation.json)

This is a standalone source runtime. It is not packaged in the Android preview
APK yet. ARM64 runtime, native game callbacks, includes, AI/skills, world state
and full source-built gameplay remain unfinished. No Android 9 or physical
device was tested for this module.

## Rebuild and verify

```sh
python3 port/lua-runtime/build.py --host --sanitize --report /path/to/host-build.json
python3 port/lua-runtime/build.py --ndk /path/to/ndk --report /path/to/android-build.json
python3 port/lua-runtime/tests/corpus.py --runner port/lua-runtime/build/lua-host-runner --report /path/to/host-corpus.json
python3 port/lua-runtime/tests/corpus.py --runner port/lua-runtime/build/lua-x86_64-runner --adb /path/to/adb --serial emulator-5560 --report /path/to/android-corpus.json
```

Build verification always checks all 56 vendor hashes. The corpus test checks
all script and override hashes and refuses physical devices, non-Android-17
images and non-x86_64 ABIs. Binary runners and staging inputs are ignored by Git.
`tools/import_lua_runtime.py` can reproduce the exact vendor import from the
pinned archive. The earlier Lua 5.1.5 parse/inventory reports retain their
distinct compiler identities and scope.
