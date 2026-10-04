# Exact root file load and current scope validation

This additive source API supersedes the generic filename-labelled loader for
source ScriptOwner stages3/4; generic vm_load behavior is unchanged:

```c
int dh2_script_vm_load_source_file(dh2_script_vm*,const void* bytes,size_t size);
int dh2_script_include_scope_valid(const dh2_script_include_scope*);
```

Root load requires a nonbusy live VM, accepts an empty file, temporarily marks
it busy, and invokes the exact same native source primitive as include_load:
1024-byte reader, chunk name `loadFile()`, lua_load then pcall(0,0,0), one error
pop. It retains the initial stack, including five direct source-library results.
It permits bound dedicated Include callbacks without enabling generic VM
reentry. The return is0 success, positive source Lua status for original AddFile
false, -1 malformed/busy or -4 unsupported source error object. vm_error() holds
the source diagnostic until the next generic/root operation. Source effects
before an error remain; no return-table Value projection occurs.

scope_valid is nonmutating and returns1 only for the VM's current Include frame
and generation. Owner must check it and the session's matching VM **before**
cache callbacks or a loaded-set hit. Comparing error message text to determine
validity is incorrect; executing an empty file to test scope validity introduces
an invented source operation. A copied scope may be tested after callback return
only while its VM remains alive. A shadowed outer scope becomes valid again
after its nested callback returns.

Current frozen source:

- script_runtime.h: `35b57c2e5bf9625eebd6fd0c4e35d1e8880856ac9e2caaa5c1e437ad539ffa71`
- script_runtime.c: `d01b55dbebaf3cca978b61a498ff9c4b07e6ca186cc4216231e26b5bd6281ed9`

Current proof `reports/script-source-file-host-audit.json` SHA-256
`dc78cf9797a5f0aab4187185b0d0a40ee4b186724ee27f0948db78c2975f6ae7`:
80 original Include guards,34 actual callbacks,56 scoped operations,17 exact
root-file checks,341 scope/busy guards,40 table projections; ASan/UBSan/leaks0.
The same DSO also passes ownership921, VM/commons438, genuine timers123, and
original callback replay5493/3194 ordered services. Historical Include-only
report/notes remain historical and are not rebound to current source hashes.

The root test covers genuine nested Include, zero-return table discard, empty
input, syntax/runtime errors with the exact chunk label, retained stack5, partial
mutation, unsupported error object rejection and malformed/busy guards.
Parent/owner still supply genuine resource/cache services, receiver identity and
persistent callback lifetime; this proof does not claim the full manager backend,
external AI acceptance, ARM64 instructions, APK or device execution.
