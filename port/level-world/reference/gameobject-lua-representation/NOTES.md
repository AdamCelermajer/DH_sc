# Genuine source GameObject Lua representation

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The main manifest captures twenty complete routines. `arguments/` separately
captures the complete Arguments constructor whose signed parameter explains
the static method receiver exclusion. These are new reference files; old
target/runtime proof reports were retained.

## Producer and ownership

`0x386f28` is **Arguments::pushUserData**, not itself a GameObject Lua-table
constructor. `_GetTarget` (`0x3b6c7c`) reads Character target `+0x408` and
tailcalls ReturnValues::pushUserData (`0x37c9f8`). Both UserData append methods
construct a Value through `0x37c978` and append it to native vectors. Actual
Value constructor executes in the new original probe: type 7, UserData
identity at Value `+0x6c`. Value::_pushOnStack (`0x31cac4`) produces the Lua
representation:

* Null UserData pushes **nil**, one result.
* Nonnull creates a **fresh table** on every push, normal settable field
  `_this` = lightuserdata of that raw identity. The return itself is never
  lightuserdata.
* Calls virtual UserData getUDTypeName at `+8` each push. Genuine bodies are
  GameObject `0x340124` → `GameObject`, Character `0x3a2f08` → `Character`.
  This is C++ UserData class identity, not AI-table Type.
* Registry key is literal prefix `sfc_vftable_` plus that virtual name.
  `luaL_newmetatable` creates or fetches this shared metatable.
* First use creates its `__index` method table and calls virtual
  createBindings at `+0xc`. Existing registry entries skip registration.
* Setmetatable attaches the shared metatable to the fresh receiver table.

Original type and createBindings bodies execute completely with explicit Lua
stack/allocation primitive services. GameObject produces **41 ordered method
registrations**; Character first runs GameObject then its own registrations,
producing **130**, all static-method closures through `0x319fd0`. There are
no duplicate names in these two actual lists. Each method name and original
function identity is preserved, including currently unreconstructed methods.
`gameobject_lua_catalog.inc` is deterministically generated from the executed
registration fixture and binds its SHA256, not a guessed method list.

Original table contains a raw borrowed pointer. Neither this path nor the
native bridge retains/refcounts the actor, or adds actor destruction/GC.
Lua owns tables, registry metatables and closures. Caller must retain actor
session/provider backing through VM close and use a genuine identity/world
resolver for any dereferencing method. This module does not implement that
world registry or manufacture successful object casts.

## Source method gate and results

Binder::__smethodCallback (`0x319fd0`) normal-lookups `self._this`, calls
lua_touserdata and captures its result before argument construction. Signed
Arguments constructor parameter `-1` projects stack values starting at index
2; positive `1` projects one closure upvalue (method function identity).
The original gate then calls the captured static function with Arguments,
ReturnValues and captured receiver pointer. Result append/projection is
preserved as an explicit service in the source gate probe. Debug assertion
logging/fault policies for malformed native method pointers are outside the
valid source domain; native bridge raises protected errors for unavailable
provider/context instead of dereferencing invalid memory.

* GameObject::_GetID (`0x38ebe4`) tailcalls ReturnValues::pushPointer
  (`0x38eb00`) with receiver identity. **GetID returns a pointer identity,
  not a numeric object identifier.** Even pointer zero is source type 2
  lightuserdata, different from GetTarget's null type7/nil result.
* Character::_GetTarget returns source Value type7 of its live target.
* HasTarget remains the frozen genuine scalar target getter; host method
  dispatch composes it. No new state-reset or target acceptance policy.

Actual cached monster script uses colon `GetID`. Follower `GetName` examples
occur in commented-out tracing, not executable dependencies. All method catalog entries are exposed to Lua as functions;
their presence is source-derived, **not a claim of implemented bodies**.
The test's GetState provider explicitly fails. Live caller must route each
recovered method to genuine session services; remaining methods are explicit
provider delivery failures, never accepted/default predicates.

## Additive runtime bridge

New public `port/script-runtime/script_object_bridge.h` declares:

* `dh2_script_vm_set_source_objects(vm, borrowed_services)` once per VM.
* `dh2_script_vm_bind_source_objects(vm,name,callback,context)` for callbacks
  allowed to return `DH2_SCRIPT_SOURCE_OBJECT=7`.
* Services: virtual type-name projection, ordered method registration only
  on first metatable creation, and scoped method invocation carrying captured
  receiver plus original function address.

Existing generic bind, source-values bind, VM call argument/result validation
and busy guards continue rejecting object type7. No raw lua_State is public.
Object methods source-project all remaining arguments without an arity cap,
and permit only existing scoped discarded same-VM calls during provider
execution. The new global binder retains the existing bounded bridge's 16
arguments/results; supported native GetTarget/GetID/HasTarget outputs need
one result. This is not unrestricted arbitrary ReturnValues support.
Copied scopes expire after return; generic VM reentry remains rejected.

The native backend owns Lua userdata closure records and a temporary rooted
registry-key buffer. Those private allocation details differ from original
native STL/string storage; allocator/GC byte counts and closure-upvalue
introspection are not claimed equal. Source-visible object table fields,
shared metatable/method shape and captured identities are audited. Source
method invocation during coroutine/VM-close contexts remains unsupported,
consistent with existing scoped callback guards.

New world `gameobject_lua_representation.hpp/.cpp` supplies the genuine two
class catalogs and exact GetID/GetTarget result kernels. It does not bind a
global on its own or choose actor registry/context ownership. Parent can
register ObjectServices once, bind GetTarget via bind_source_objects, and
dispatch known method addresses to the appropriate genuine borrowed actor.
This replaces the previous unimplemented GetTarget representation boundary;
remaining object world/method backends stay explicit.

## Evidence

* Original versus O2 ARM64: **133 object pushes**, **266 GetID/GetTarget
  wrapper cases**, **171 ordered/effective method registrations**, zero
  mismatches. Original Value constructor/push, actual virtual type and
  full createBindings execute; native compiled catalog/producer execute.
  Comparison includes null, repeated same-type registry reuse, both genuine
  classes, high-bit/zero/random pointer words and exact ordered method names
  and original function identities. Native64 identity maps explicitly to
  original32 fixture identity. Lua primitives and allocator are explicit
  services; this is not whole-original-VM differential execution.
* Original static Binder gate executes GetID and GetTarget in two saved
  cases, confirming receiver and `Arguments(-1), Arguments(1)` order.
* Actual float32 Lua host ASan/UBSan/LSan: **171 catalog comparisons,
  16 VM checks, 13 method calls, four guard cases, zero findings**. Tests
  exercise fresh-table/shared-metatable identity, exact 130 Character fields,
  GetTarget→GetID, source receiver captured before argument metamethod
  mutation, Lua-visible shared __index mutation, scoped nested callback,
  generic-reentry rejection, expired scope, null and partial registry commit
  after method-provider error. Unsupported methods produce explicit errors.

Reports are `reports/gameobject-lua-arm64-differential.json` and
`reports/gameobject-lua-host-audit.json`. The latter binds exact new core,
Lua sources/headers, world kernels, compiler arguments and actual isolated
runtime/executable hashes. Historical frozen central-runtime dependency
hashes in prior reports are not rewritten or presented as current.

## Reproduction and parent integration

```
python port/level-world/tests/gameobject_lua_original.py
python port/level-world/tools/generate_gameobject_lua_catalog.py
& port/level-world/tools/build_gameobject_lua_oracle.ps1
python port/level-world/tests/gameobject_lua_differential.py
python port/level-world/tests/gameobject_lua_host.py
```

Parent adds script_object_bridge.c to runtime sources (already coordinated),
gameobject_lua_representation.cpp to world, and a C++17 host target from
tests/gameobject_lua_representation.cpp linked world/runtime/dl. Its sole
argument is reference/gameobject-lua-representation/object-methods-original.bin.
After the central target exists, runner option
`--main-linked /home/adampalace/dh2-world-build/gameobject_lua_representation_audit`
writes a separate main-linked host report; it never rebuilds central CMake.
