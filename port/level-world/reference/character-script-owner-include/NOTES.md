# Source file and Include ownership integration

This is a source-only extension of the frozen external-owner milestone. It does
not change or validate the tested 0df1 APK. Historical ownership/external reports
remain bound to their historical sources and generic loader boundary.

## Source order and explicit providers

Original `_Include` **0x37efe4** projects its arguments, uses only the first
string and calls `LuaManager::AddFile` **0x37b23c** on the same LuaScript. It
returns no Lua values and ignores the source AddFile boolean. AddFile resolves
the script path and suffix, checks that script's loaded-file set, obtains/resets
the manager-cached stream, executes `Instance::loadFile` **0x31acf4**, and inserts
the resolved filename only on success. Manager cache insertion precedes VM
execution. Consequently the same filename can execute recursively before its
first successful loaded-set insertion. There is no source cycle/once guard.

These facts are instruction-tested in the immutable
`port/script-runtime/reference/base-bindings/include-original-probe.json`:
188 cases, including 80 Include guards and 48 cached AddFile gates. Original
STL/cache/registration services remain explicit boundaries. Ownership and
external stage probes separately establish Stage3 commons, Stage4 external,
Stage5 Init/VCB, then Stage6 publication. No full original composed VM/owner
instruction execution is claimed by this integration audit.

Both source root stages now call `dh2_script_vm_load_source_file`. Nested Include
uses `dh2_script_include_load` with a currently live scope. Both genuine runtime
operations use the 1024-byte reader, chunk name `loadFile()`, lua_load followed
only on success by pcall(0,0,0), and one error pop. Five retained source-library
stack values remain. Returning tables are discarded without Value projection.

The owner retains the per-session loaded set and path. `owner_cached_file`
remains the mandatory genuine manager/cache provider: it must implement resource
identity, cache-before-execution/reset-on-retry and bytes lifetime. The owner
does not manufacture that manager backend or install absent game globals.

## Stable additive API

```
owner.include(identity, requested_name, persistent_services, scope)
owner.last_source_load_status()
```

Stage1's existing Include registration descriptor is delivered to the borrowed
registration provider. That provider binds the genuine runtime Include binding
with a context which independently outlives the corresponding VM/finalizers.
It passes explicit persistent services to `owner.include`; the owner never
captures an `advance` stack's ephemeral service pointer for future callbacks.

Before cache callbacks or even a loaded-set hit, include validates the exact
private session/VM and `dh2_script_include_scope_valid`. Shadowed/expired scopes
are rejected without effects. A fresh inner scope permits nested execution;
the outer scope becomes live again after the nested runtime callback returns.
Empty filenames follow the source suffix rule, and zero-length files are valid.

Return0 means native delivery, including missing files and positive source Lua
statuses. Positive parser/pcall errors retain effects preceding the error, do
not insert a loaded filename, and permit the caller to continue. The exact
positive status is available from `last_source_load_status`; the legacy VM
diagnostic maps it to -2. Malformed scopes/callers return -1. Provider/native
unsupported errors return a failure (normally -2), never source acceptance.
The runtime's unsupported nonstring/nonnumber error-object result is preserved
as a negative source-load diagnostic.

Session shared ownership spans outer root/initial VM calls and Include service
delivery. An ownership operation with work remaining is rejected while an owned
VM is executing; completed loader calls remain effect-free and permitted.
This is a bounded native lifetime contract, not a new recovered original gate.
Generic busy guards remain enforced. **Providers must not destroy the owner,
current receiver, or VM while outer Lua is executing.** Borrowed timer/direct VM
callers likewise retain the selected owner/session until their outer call
returns. A local shared_ptr inside a callback alone would not make destruction
at callback return safe. Include from coroutine or VM-close remains explicitly
unsupported by the runtime; no provider is accepted in those contexts.

## Verification and central handoff

`tests/character_script_owner_include.cpp` is a new genuine runtime integration
audit. It does not replace historical tests. The source-root/Include host runner
links current owner source into an isolated DSO and executes actual sanitized
world and Lua DSOs. PASS: 2,005 Include checks, 19 actual callbacks, 114 atomic/
busy guards, four nested callbacks, same-file preinsert recursion, private VM
isolation, empty/missing/parse/runtime/unsupported files, failed-file retry,
outer scope restoration and original follower flags **0x3c2**. Legacy 6,283 and
external 2,863 checks also pass against the same current owner/runtime. No ASan,
UBSan or leak findings. Provider contexts survive VM finalization by construction.

The new optimized ARM64 helper report covers 304 constructor/catalogue
comparisons and four atomic rejections against original evidence. It explicitly
does **not** execute the full C++ owner or Include class on ARM64. Actual nested
class behavior is the host DSO audit, with source/runtime original evidence
bound separately; neither is a whole game/AI/APK parity claim.

Central target: `character_script_owner_include_audit`, source
`tests/character_script_owner_include.cpp`, links `dh2_level_world`,
`dh2_script_runtime`, `dl`, usual ASan/UBSan flags; no direct owner source in the
test executable. Arguments are the exact commons and follower resources in
`.local-inputs/character-script-owner-extension`. After central DSOs rebuild:

```
python port/level-world/tests/character_script_owner_include_host.py --main-linked --runtime-dir /home/adampalace/dh2-world-build/script-runtime
```

That creates a distinct main-linked report; it must not rewrite historical
owner/external milestone reports. Remaining boundaries are the full namespace,
manager resource cache, unrecovered AIS state/update registry/virtual services,
actual persistent provider binding and live owner receiver publication. No
renderer, central CMake, Android build, installation or device operation was
performed by this task.
