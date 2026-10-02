# Source-built Lua runtime for modern Android

The original engine contains a Lua 5.1.4 version string. This module imports
56 exact C/header/license files from the official
[Lua 5.1.4 archive](https://www.lua.org/ftp/lua-5.1.4.tar.gz), whose published
[SHA-256](https://www.lua.org/ftp/) is
`b038e225eaf2a5b57c9bcc35cd13aa8c6c8288ef493d52970c9545074098af3a`.
The archive has 216,679 bytes. The exact MIT license is retained in
`vendor/lua-5.1.4/COPYRIGHT`; [the manifest](vendor-manifest.json) pins each
imported file. Upstream bytes are unchanged. The original game library may
contain local changes: full original interpreter equivalence has not been tested.

## Original number profile

The original `lua_pushnumber` stores one 32-bit value and a 32-bit type tag;
`lua_tonumber` returns that word. The source now uses **float32 numbers and
int32 integers** through Lua's supported `LUA_USER_H` hook and the separate
[configuration header](dh2_lua_config.h). Upstream bytes remain exact. Other
configuration choices retain upstream defaults and are not established as
engine equivalents.

## Integer game constants

`GetPyCst` now reads owned mappings imported from checked integer constant files.
The [table module](../pydata-constants/README.md) documents the format, original
reader trace and authored ownership rules. All 5,608 values in 26 complete files
match original reader writes and pass authored Lua queries on host and both
Android 17 page sizes, after releasing caller buffers. The mixed sound file is
rejected completely; the original reader stops partway through it. Malformed
input and allocation failure preserve earlier mappings. Host sanitizer corpus
checks pass. No original game script is executed.

Current build/selftest evidence: [host](constants-host-build-validation.json),
[NDK](constants-android-build-validation.json). Current script parse/arithmetic
evidence: [host](constants-host-script-validation.json),
[4 KiB](constants-android-4k-script-validation.json),
[16 KiB](constants-android-16k-script-validation.json). The `bridge-` reports
below retain the previous checkpoint before `GetPyCst` integration.

[Original instruction evidence](original-numeric-validation.json) checks 13
push/read cases, including both signed zeros, subnormals, infinities and a NaN
payload, through positive and negative stack indexes. It also records 504
numeric arithmetic outputs from the actual original `Arith` body. The current
source runtime evaluates matching authored snippets for addition, subtraction,
multiplication, division, modulo, power and negation. All 504 match on host and
both Android 17 emulators, with signed zero treated as equivalent. Original
arithmetic imports use the existing host oracle; floor/power use host libm.
This does not prove historical Android libm, metamethods, numeric string
coercion, integer edge conversions or full interpreter equivalence.

The new owned C wrapper installs the base, math, table and string libraries
observed in the original registration callers. It removes filesystem loaders
and `print`; it installs `GetPyCst` and nine reconstructed numeric callbacks (see
[numeric bridge scope](../lua-numeric/README.md)). Gameplay object callbacks
are not installed. The base opener
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

Host builds and selftests pass, including ASan/UBSan and float-cast overflow
checks that stop on the first error. These run bridge assertions, extreme
numeric table keys and the 504 arithmetic vectors.
NDK r29 builds ARM64 and x86_64 shared libraries and standalone runners.
ARM64 load segments align to 16 KiB. Upstream indentation/empty-body warnings
are retained in build reports. Sanitizers exposed signed subtraction overflow
and out-of-range integer conversion in upstream table lookup. A separate
[tracked patch](patches/ltable-array-index.json) uses unsigned subtraction and
guards numeric-to-int conversion. It is applied to a generated build copy;
all 56 imported files remain exact. Patch and compiled-copy hashes are recorded
in build evidence. This is an authored modern safety repair, not an established
original-engine change.

The exact x86_64 runner passed on both official Android 17 emulators:
SDK 37, 4 KiB pages and 16 KiB pages. All 220 staged source inputs, the list
and runner were hash-checked on each device. In all three environments,
218 exact original game scripts pass parsing, the unchanged sandworm original
returns syntax status 3, and its separate override passes. Original scripts
are compiled only and **never executed**. Runtime tests execute authored snippets
for standard library access, nine numeric callbacks, missing gameplay callbacks, instruction/memory limits,
error recovery, compile-only behavior and invalid/bytecode inputs.

- [Earlier numeric bridge host build](bridge-host-build-validation.json)
- [Earlier numeric bridge Android build](bridge-android-build-validation.json)
- [Earlier numeric bridge host corpus](bridge-host-corpus-validation.json)
- [Earlier numeric bridge Android / 4 KiB](bridge-android-4k-validation.json)
- [Earlier numeric bridge Android / 16 KiB](bridge-android-16k-validation.json)

The `float-` reports record the previous float32 build before numeric callbacks
and the table safety repair, with their distinct source/binary identities.

The earlier reports without the `float-` prefix record the initial upstream
double-number builds and their distinct binary/source identities. They confirm
parse/selftest behavior for those earlier builds, not the current number profile.

This is a standalone source runtime. It is not packaged in the Android preview
APK yet. ARM64 runtime execution, remaining native game callbacks, includes, AI/skills, world state
and full source-built gameplay remain unfinished. No Android 9 or physical
device was tested for this module.

## Rebuild and verify

```sh
python3 port/lua-runtime/tests/original_numeric_vectors.py --original /path/to/libDungeonHunter2.so --oracle port/skin-payloads/build/oracle.so --header port/lua-runtime/tests/numeric-vectors.h --report /path/to/original-numeric.json
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

The numeric vector generator additionally needs Unicorn/pyelftools and the
existing host arithmetic oracle. Checked vectors are already tracked for normal
builds; regeneration deliberately executes only the identified original Lua
API/arithmetic bodies. No game script or gameplay callback is executed.
