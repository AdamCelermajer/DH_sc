# Private Session integer ownership: proposal only

No production owner/runtime/integer/CMake file or historical test/report is
changed by this discovery. The new fixture plan is **not executed integration
proof**. The existing integer module is frozen and independently proved; this
proposal connects its actual map to each real `ScriptOwner::Impl::Session`.

## Current facts

`character_script_owner.cpp` owns one heap-stable Session per selected AIS. Its
VM and aliases are already private. `view()` exposes VM/aliases/path, and the
registration provider receives only an ephemeral `ScriptSessionView` and
descriptor. `bindings()` preserves all170 source global deliveries, with
Stage1 opening the libraries before35 bindings and Stage2 delivering135 globals.

The source catalogue's Stage1 indices2 and3 are respectively `SetInt`/`GetInt`.
`LuaScript::BindFunctions` at `0x37b5a0` calls their registration at `0x37b628`
and `0x37b644`, with original callbacks `0x37de5c` and `0x37ec14`. This order
matters for prefix/reentry observations. Calling `dh2_script_int_bind` at index2
would prematurely install both callbacks; instead register exactly one callback
at each captured descriptor delivery.

The current historical host fixture deliberately leaves SetInt nil at
`tests/character_script_owner.cpp:99`. It is a missing-binding fixture boundary,
not original namespace gold and not an implemented fake integer map. Preserve
that old file/report unchanged and add a distinct integrated test/report.

`LuaScript` constructor `0x37c674` calls `Instance` first, then initializes its
own integer map in `0x37c6c4..0x37c6d8`: root/count zero and minimum/maximum at
the owner's own `+0x1c` sentinel. It then initializes main aliases `+0x34` and
backup aliases `+0x4c`. This map exists for every private LuaScript, independent
of active vs pending and the scripted flag. It must not be keyed by Character
alone, nor reset when Stage1 bindings repeat, files load, or pending publishes.

Destructor `0x37c004` clears loaded paths, then path, alias backup/main, and
integer map at `0x37c0bc..0x37c0e8`, before owned `Instance` closes at `0x37c100`.
The new integer proof executes this exact integer-only clear region and recursive
erase with the sized deallocation service. A VM finalizer may call the already
installed bindings after contents have been cleared. Destroying the wrapper or
binding receiver before close would make that callback unsafe.

## Minimal additive interface for the next approved stage

Keep `ScriptSessionView`56, `ScriptConstructorFields72`, and the existing
two-argument constructor unchanged. Add a persistent services configuration:

```cpp
struct ScriptIntegerServices32 {
  void* context = nullptr;
  dh2_script_int_identity identity = nullptr;
  dh2_script_int_format_fraction format_fraction = nullptr;
  std::uint64_t reserved = 0;
};
// New overload; the existing constructor delegates with empty services.
ScriptOwner(std::uintptr_t character, std::size_t vm_memory_limit,
            const ScriptIntegerServices32& persistent_integer_services);
// Returns a borrowed stable Session receiver. No new map or VM is constructed.
bool integer_bindings(std::uintptr_t session_identity,
                     const dh2_script_int_bindings*& out) const noexcept;
```

The owner copies the configuration and each Session copies it into its own
`dh2_script_int_bindings`40 with that Session's freshly owned map. Native
configuration pointers/context must outlive the owner and all its VM finalizers;
copying a pointer does not extend the external context's lifetime. An ephemeral
`ScriptOwnerServices` supplied to `advance()` cannot serve as this retained
binding context. Validate reserved input before constructing a new Session;
missing providers are valid and intentionally leave the corresponding coercion
unsupported, rather than replacing them with guessed identity/format behavior.

Suggested Session additions:

```cpp
dh2_script_int_map* integers = nullptr;
dh2_script_int_bindings integer_receiver{}; // Stable Session member address.
```

Create the map during Session construction before it is published pending.
Initialize the receiver once; callbacks borrow its address. Do not put this
receiver in a registration request stack variable or capture the ephemeral
view. Account for constructor failure explicitly: C++ does not invoke Session's
destructor when its constructor throws. Release already allocated wrappers/VM
without leaking or closing the VM after releasing a live callback context.
The extra opaque-map wrapper allocation is a native allocation boundary; it is
not a claim that the original embedded empty map allocated a heap node.

The registration provider can then use this exact additive bridge:

```cpp
const dh2_script_int_bindings* receiver = nullptr;
if (phase == 1 && descriptor.original_callback == 0x37de5c) {
  // Also validate source descriptor name == "SetInt" and index == 2.
  require(owner.integer_bindings(session.identity, receiver));
  bind_source_values(session.vm, "SetInt", dh2_script_int_set_callback, receiver);
} else if (phase == 1 && descriptor.original_callback == 0x37ec14) {
  // Also validate source descriptor name == "GetInt" and index == 3.
  require(owner.integer_bindings(session.identity, receiver));
  bind_source_values(session.vm, "GetInt", dh2_script_int_get_callback, receiver);
}
```

This is pseudocode: actual existing binding API takes `void*`, so the const
borrowed receiver needs its deliberate ABI cast. All170 original registration
deliveries remain in place; owner does not install the callbacks earlier,
coalesce them, manufacture unrelated globals, or silently absorb provider
failure. The provider stores only the stable receiver in the Lua closure. It
does not repeatedly look up the Session when the callback executes: a Session
being erased/closed may no longer be discoverable through the owner's map,
while its direct borrowed receiver is still alive.

Teardown order becomes:

1. Clear loaded filenames and path, matching the current owner.
2. Clear alias contents (backup then main, recording flag unchanged).
3. Clear this Session's integer contents.
4. Close the owned VM with aliases, integer map, receiver and external providers
   still alive. Finalizer insertions are allowed and are not cleared again
   before later finalizers merely to simulate a frozen empty map.
5. Destroy alias and integer wrappers; release the remaining Session storage.

The source identity-mapping service must map native opaque identities to genuine
explicit source32 identities. It must not truncate Session or Character pointers.
Missing mapping is an explicit failure for nonzero identity operands; null
identity needs no provider. Fractional/NaN name formatting requires the imported
printf spelling service. Ordinary authored string/integer names work without it;
do not make a guessed libc formatter mandatory for those ordinary calls.

## New integration test plan

`integration-cases.json` separates existing source proof from proposed owner
composition checks. Add a NEW test, leaving the historical SetInt=nil fixture
untouched. Link the actual world and runtime DSOs rather than recompiling an
alternate owner or integer kernel into the test. Require `dladdr`/binary and
source binding in its new report.

The essential cases are: constructor-created distinct maps; exact registration
prefix between2/3; repeated Stage1 retains the same map/receiver and values;
ordinary default/player/iPhone/external sessions; successful/missing/error root
loads retain the same map; real nested Include shares the outer Session map;
two Characters and same-owner replacement isolate maps; pending publication
does not reset state; explicit missing formatting/identity service errors before
mutation; and a genuine `newproxy` close finalizer observing cleared aliases and
integers, then safely reinserting through SetInt/GetInt. An outside observer must
capture the finalizer results before wrappers are destroyed; do not dereference
a destroyed Session map after replacement or owner destruction.

Replacement testing should use the captured lifecycle's actual replacement
entry, not changing a raw session identity or manually deleting its map. The
new report must retain source ignored-load-error publication semantics and
generic busy/Include scope gates. Full game namespace, ARM64 packaged owner
execution, and live manager/actor claims remain outside this scaffolding.
