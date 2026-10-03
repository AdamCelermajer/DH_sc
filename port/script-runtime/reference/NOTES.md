# Native Lua 5.1.4 float32 backend

The source backend is the official Lua 5.1.4 archive from
<https://www.lua.org/ftp/lua-5.1.4.tar.gz>, listed by the official
<https://www.lua.org/ftp/> index. Archive size is 216,679 bytes and SHA-256 is
`b038e225eaf2a5b57c9bcc35cd13aa8c6c8288ef493d52970c9545074098af3a`.
The complete original archive tree remains under `vendor/lua-5.1.4`, including
its MIT license in `COPYRIGHT`. `source-provenance.json` binds every vendor file,
the three adapted source files, original instruction capture, and license.
The currently published online `/source/5.1/` tree identifies itself as 5.1.5;
the implementation here uses the exact downloaded 5.1.4 archive.

## Original evidence and numeric contract

The source game's ELF is SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Its embedded version identifies Lua 5.1.4, copyright 1994–2008. Existing
`port/level-world/reference/character-script-update/lua-inputs/discovery.json`
records execution of `luaU_header` at `0x85bc64`, yielding:

`1b4c75615100010404040400`

This means signature, version 0x51, format0, little endian, int4, serialized
size_t4, instruction4, Lua number4, and nonintegral numbers. The new capture
under `original-lua` contains original `luaO_str2d` `0x854d74`, `luaU_header`
`0x85bc64`, `luaU_undump` `0x85c320`, and `luaV_execute` `0x85d09c`.

`luaO_str2d` calls `strtod` at `0x30e6ac`, then `__aeabi_d2f` at `0x30e6a0`.
The modulo fast path at `0x85e168` performs float division, widens that result,
calls double `floor` at `0x30eac0`, multiplies/subtracts in double, then converts
back to float at `0x85e1b4`. Power at `0x85e204` calls double `pow` at
`0x30e9c4`, then converts back to float. The official expression macros reproduce
these operations when `lua_Number` is configured as float. Replacing these
functions with `strtof`, `floorf`, or `powf` would change the source contract.
Compilation disables fast math and contraction.

Only three upstream files differ in the adapted `lua/` tree:

- `luaconf.h`: `lua_Number=float`, disable the double-specific conversion
  optimization, and use `%f` for number scanning. `strtod`, `floor`, `pow`,
  double vararg promotion, and the official `%.14g` number output format remain.
- `lundump.c`: serialized string lengths use uint32_t, and the header's size_t
  field is fixed to4. Native allocation sizes and pointers remain64-bit.
- `ldump.c`: emit uint32_t serialized string lengths and reject lengths that
  would overflow that field. This keeps native-produced chunks compatible with
  the source reader format.

The reader retains the official strict whole-header comparison, negative-count
rejection, constant-kind validation, recursion guard, and opcode checker.
It loads one chunk using the official parser; it does not add a trailing-byte
rejection. This is the original little-endian format, not a universal endian or
arbitrary Lua-version converter. Input and allocator limits belong to the new
owned VM API. Native `lua_Integer` remains upstream `ptrdiff_t` (64-bit here);
the original ARM32 API uses32-bit ptrdiff_t. Integer-API equivalence, native C
`long` formatting equivalence, and the game's number-output format have not been
independently instruction-differentiated. The float arithmetic/header evidence
does not establish whole-VM parity.

## Actual AI commons and binding inventory

The exact source entry is
`com.gameloft.android.GAND.GloftD2SS/files/data/scripts/ai/_commons.luac`.
The saved source is 13,535 bytes, SHA-256
`20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c`.
It is plaintext Lua, despite the filename suffix. None of the219 recovered
script entries begins with the Lua binary magic. The binary reader audit uses
native-compiled chunks with the proven original field widths; it does not claim
to load a binary AI commons chunk recovered from the cache.

The real native compiler creates36 prototypes: the main chunk and35 global
functions. An independent 32-bit-field decoder walks every prototype and checks
the dumped chunk is consumed exactly. Its GETGLOBAL inventory is:

| Global | Source requirement |
| --- | --- |
| `type` | Real standard base library function |
| `StartTimer` | Required game binding for `StartTimerCB` |
| `StopTimer` | Required game binding for `StopTimerCB` |
| `Trace` | Required game logging function on the duplicate-registration path |
| `evnet_name` | Actual misspelled source variable, not a legitimate game binding |

Loading the main chunk defines closures and private callback tables without
calling the game bindings. All29 empty callbacks execute in the real VM. Pure
`AttachToAnimEvent`/`OnAnimEvent`/`DetachFromAnimEvent` behavior is exercised with
actual Lua closures and captured table references. `StartTimerCB` and
`StopTimerCB` fail with protected missing-global errors because no fake timer
bindings are installed. Duplicate animation-event registration preserves the
source concatenation error involving `evnet_name` before `Trace` is reached.
The runtime does not correct that typo or silently supply it.

`OnTimer` retains a callback record, invokes it, and clears a nonlooping record
after invocation. Its real game timer dispatch/reentry behavior remains a binding
integration boundary. Timer callbacks are not claimed as executed by this audit.
Selected AIS construction, LuaManager ownership/reset, source registration of
all game functions, and dispatcher handoff remain outside this new backend.

## Owned native API and proof

`script_runtime.h` exposes opaque owned VM create/destroy, load, compile, call,
global classification, and borrowed synchronous function binding. Values carry
float32 numbers, booleans, strings, and native light-userdata identities; the
64-bit typed value is40 bytes. Table/function/userdata/thread results expose
only their Lua type, not a portable handle. Service arguments/results are
restricted to the supported scalar types; full userdata/table game bindings are
not yet represented. The test-only `AuditEcho` service checks a 64-bit opaque
identity and is explicitly unrelated to game timer bindings.

Lua errors and allocation failure remain protected; the wrapper restores its
stack. Script-side mutations before an error are retained. Strings returned to
the caller are borrowed until the next VM operation. The current bounded C ABI
rejects same-VM reentry; source LuaManager synchronous callback reentry requires
an additional caller/API integration audit. Borrowed callbacks must not throw.
The installed standard libraries are base/coroutine, table, string, and math.
This is an explicit port capability selection, not proof of the game's exact
LuaManager library setup or a security sandbox. Upstream `math.random` uses the
C library generator, whose process state is not isolated between VMs.

The host proof in `reports/lua514-host-audit.json` binds actual source, archive,
original ELF, executable, library, and inventory. It passes438 checks under
ASan/UBSan with zero diagnostics: float32 rounding, original-format header,
32-bit serialized nested-function/string roundtrip, all12 header-byte rejects,
265 truncations, post-error reuse, actual commons callbacks/missing bindings,
opaque64-bit service identity, malformed typed input, nonstring Lua errors,
bounded allocator failure and recovery.

`reports/lua514-android-build.json` binds standalone arm64-v8a and x86_64
libraries built with NDK29.0.14206865/API24, ELF64 and16KiB LOAD alignment.
These are compiler/build proofs. They are not packaged APK execution, physical
ARM64 execution, an original whole-VM differential, or complete game AI.

Reproduce the host audit with:

```sh
cmake -S port/script-runtime -B /home/adampalace/dh2-lua514-build -G Ninja \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo -DDH2_SCRIPT_SANITIZERS=ON
cmake --build /home/adampalace/dh2-lua514-build -j4
```

From Windows, `python port/script-runtime/tests/run_audit.py` executes the WSL
audit and binds existing Android builds under `.local-inputs/lua514-source/`.
No existing app, shared CMake file, renderer, production AI, APK, or device was
modified for this task.
