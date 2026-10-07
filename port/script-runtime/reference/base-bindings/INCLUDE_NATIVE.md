# Native source Include capability

The additive runtime API is now implemented and frozen in script_runtime.h/.c.
Historical discovery/feasibility reports remain unchanged. Current proof is
`port/script-runtime/reports/script-include-host-audit.json`, SHA-256
`c42fcaae7cb9903a9b0bbda483f6c845b1696e08dcc771e8a83d786edb65c8e0`.

Stable APIs:

```c
dh2_script_vm_bind_source_include(vm, provider, borrowed_context);
dh2_script_include_load(scope, bytes, size);
dh2_script_include_error(scope);
```

The provider receives first-string filename and a synchronous borrowed
`dh2_script_include_scope16 {vm,generation}`. It returns0 for delivered owner
operations, including original missing/load-false. Nonzero produces an explicit
protected native-provider error. The provider must not throw or destroy/reenter
the VM through generic APIs. A copied scope can be checked after return while
its VM is still alive; it rejects as expired. During a nested callback its outer
scope rejects until the nested callback returns; then the outer scope is restored.

`include_load` returns0 on success, **positive original Lua status** for source
AddFile false, -1 for malformed/expired scope and -4 for unsupported source
non-string/non-number error objects. Provider must diagnose negative results;
the real-VM harness rejects them rather than treating them as delivered source
success. error() is borrowed through the next scoped load, and copies source
error text through first NUL into a bounded diagnostic buffer. No source success
depends on diagnostic truncation.

The source load uses 1024-byte chunks, chunk name `loadFile()`, lua_load followed
by lua_pcall(0,0,0) only on successful parse, and a single error pop. Internal
direct protection additionally catches allocation failure while converting a
numeric error object to its string; it adds no Lua C frame or arguments. Empty
files are valid. The current VM allocator/budget remains active; there is no new
input-length or argument-count cap. Dedicated binding projects all arguments
before first-string guard; 39 unused table arguments plus a rejected first table
exercise40 actual `_this` lookups. Returned tables are not projected by this
zero-result load path. Coroutine and VM-close Include remain explicit unsupported
contexts. Generic VM busy gates are unchanged.

`script_include_audit` is the only new runtime CMake target. Its assert checks
remain enabled under all build types. Reproduction:

`python port/script-runtime/reference/base-bindings/run_include_audit.py`

The runner builds only an isolated WSL runtime at
`/home/adampalace/dh2-script-include-build`; it preserves prior libraries and
reports. Current host proof:80 original Include guard cases,30 actual callbacks,
49 scoped load operations,6 handled Lua errors,1 unsupported error-object
rejection,196 malformed/expired/shadowed-scope and generic-busy checks. Private
VMs, cache persistence, post-success loaded insertion, bounded recursion, partial
mutation on errors, Lua memory exhaustion and callback-provider rejection all
pass ASan/UBSan/LeakSanitizer with zero diagnostics. Existing source ownership921,
VM/commons438, genuine timers123, and original timer-callback replay5493/3194
ordered services also pass against the same current DSO. Report binds source,
executable and actually loaded DSO hashes.

Current source SHA-256:

- script_runtime.h: `ba7802072736a15806a69ba949b5c68f84a07232efd4e1009ad95bdacc707910`
- script_runtime.c: `8988cbbfa3cecffcd78d475dafcaa70665af61fcd4c32eabf18c0d8800ee087f`

ScriptOwner session/cache/provider lifetime integration remains parent-owned;
see the concrete plan in NOTES.md. This host proof does not establish the complete
manager/resource backend, external AI acceptance, optimized ARM64 instruction
parity, packaged APK or physical ARM64 execution.
