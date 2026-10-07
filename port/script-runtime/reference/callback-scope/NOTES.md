# Synchronous source callback capability

The additive native64 scope permits only LuaScript::Call-style discarded calls
while a native source callback is executing. It does not clear VM.busy or grant
GetGlobal/Bind/load access. Genuine receiver/alias/state registry ownership stays
with the caller. No general reentry or always-accepted AI service is introduced.

Stable API in script_runtime.h:

```c
typedef struct { dh2_script_vm* vm; uint64_t generation; } dh2_script_callback_scope;
typedef int (*dh2_script_scoped_function)(void*, const dh2_script_callback_scope*,
  const dh2_script_value*, uint32_t, char*, size_t);
int dh2_script_vm_bind_source_scoped(dh2_script_vm*, const char*,
  dh2_script_scoped_function, void*);
int dh2_script_callback_scope_valid(const dh2_script_callback_scope*);
int dh2_script_callback_call_discard_source(const dh2_script_callback_scope*,
  const char*, const dh2_script_value*, uint32_t);
const char* dh2_script_callback_error(const dh2_script_callback_scope*);
```

Provider has zero Lua returns, matching original RegisterAIState/ChangeAIState
callbacks. It receives ALL source-projected arguments: table->_this identity
kind7 with normal getfield; string first-NUL; function/thread/raw full userdata
to nil. No fixed arity cap. Its borrowed context and actual VM/receiver must
survive the complete outer invocation; it cannot throw/destroy VM or call raw
Lua APIs. Nonzero provider status raises its protected diagnostic after restoring
the outer capability. Generic operations reject throughout.

Each invocation receives a unique VM-wide generation. Only the current native
provider capability is valid: nested same-binding callbacks and Include shadow
one another, then restore the outer capability. A copied token expires at return;
it may be tested later only while its VM is alive. Use scope_valid before actual
owner/alias/cache services; do not infer validity from error message text.
Coroutine and VM-close callbacks remain explicitly unsupported.

Caller resolves alias freshly for each Post/Init, then passes the resolved name.
Scoped call performs fresh global lookup, pushes validated public scalar Values
(nil/bool/identity/number/string), calls Lua and projects EVERY returned value
before discard, including normal table._this/__index and first-NUL strings.
No fixed return/argument count cap; actual stack and VM budget apply. Type7
object pushes require the separate original game-object push boundary and reject
here rather than being silently treated as lightuserdata. Existing generic APIs
retain their policies and busy protections.

Return0 means success; positive Lua status2/4 means actual protected Lua failure;
-1 means malformed/expired/shadowed capability; -4 is the original unsupported
nonstring/nonnumeric error-object domain. Numeric error conversion is itself
protected. Stack is restored, earlier Lua mutations persist, and scope_error
holds diagnostics until its next call. Original ChangeAIState may ignore positive
Post/Init Lua errors; the registry decides that source behavior. The runtime does
not silently accept a missing function/provider.

Source semantics reuse captures in `reference/game-bindings/return-discard`:
LuaScript.Call37c41c/37c390, Instance named lookup31abe8 then
pCall31ab4c/pCall_31aa78, ReturnValues._addFromStack31b4ac and
Value._setFromStack31c9c8. Source
Binder.__functionCallback31a250 constructs a native Arguments vector; source
base callback/state registry captures are maintained in the level-world module.
This scope is a native wrapper contract, not an asserted original wrapper ABI
or whole original VM/state-registry differential.

The initial scope implementation used Lua userdata for argument storage. That
added VM memory/GC effects before provider invocation, unlike the source native
vector. Its proof `script-callback-scope-host-audit.json` remains historical.
Corrected storage uses native allocation and a direct protected projection
region with cleanup before rethrow, adds no Lua userdata/C argument frame, and
roots borrowed string data through the original argument stack. Native argument
allocation failure is an explicit protected diagnostic; original C++ allocation
failure parity is not claimed.

Final corrected source: header6609f8b0..., Ccfb4aaa0..., test5ab8fe8f...;
`reports/script-callback-native-storage-host-audit.json` SHA-256
`0fb93ebd4e44d2a426c99220de0ebc545763195021eff25ef98f8864273fb1b9`.
Actual separate sanitized DSO:
`/home/adampalace/dh2-script-callback-native-build/libdh2_script_runtime.so`.

Final genuine host proof:16 provider callbacks,85 discarded Lua calls,
543 scope/busy guards,33 successful argument table projections,162 successful
returned-table projections,64 argument projection error/cleanup cases, one
native-storage Lua-memory check plus actual newproxy GC probe. Same-binding
reentry, expired/shadowed/foreign tokens, error-object/numeric/OOM recovery,
source library stack5 and Include mutual shadow/restore are exercised.
ASan/UBSan/leaks0.

Same DSO regression proof: Include80 original guards/34 callbacks/56 loads/
17 root checks/341 guards; ownership921; VM/commons438; genuine timers123;
original timer replay5493/3194 services; both scalar gold replays13978 with
37-check baseline and separate40-check source extension. Scalar production and
gold remain unchanged. Historical scalar main report recorded40-source hash
while running37 binary; `script-scalar-test-source-correction.json` records that
fact and preserves both versions without rewriting historical reports.

Reproduce with `reference/callback-scope/run_native_storage_audit.py` using the
configured Python/WSL environment. Parent owns main CMake integration. No APK,
device, physical ARM64, full AI or original full-VM parity is claimed.
