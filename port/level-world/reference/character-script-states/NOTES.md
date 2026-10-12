# CharAIScript owned states and External update

This new source module is separate from the frozen ScriptOwner/APK milestone.
It owns native state records; it neither allocates/publishes an AIS nor owns its
VM or alias map. Registry/receiver and all borrowed providers outlive the entire
outer Lua operation and registered callbacks, including finalization. No existing
owner, runtime core, CMake, renderer, APK or historical report was changed here.

## Exact original facts

`CharAIScript` C1 **0x3d8f44** writes owner+98=0, empty string-keyed map at+9c
and current-state+b4=0. `_State` constructor **0x3d9684** constructs four empty
owned strings at0/18/30/48 (32-bit string size24). Call helpers read their
current C-string addresses at14/2c/44/5c. The node's State value is at+28.
Native records/maps own copied strings and preserve stable record addresses;
they do not claim the original ARM STL layout. Original D1 **0x3d926c** destroys
the state tree before LuaScript teardown. This module's borrowed VM contract is
not complete AIS/destructor ownership. Closing-VM scoped callbacks are rejected
by the separate runtime capability, never silently accepted.

`RegisterAIState` **0x3da144** requires2..5 arguments, all source string type4.
Rejected count/type returns without effects. It obtains `map[name]`, constructing
an empty record only for a new key. Arguments1..4 assign update, conditions,
init, post in that order. Existing records keep omitted members; a partial
registration is not a replacement with new empty defaults. Case-sensitive
std::string lookup is distinct from the alias map's unsigned-hash membership.

`ChangeAIState` **0x3d9710** requires exactly1 argument, then calls original
Value.getString **0x31c49c** without a string-type gate. Unknown keys return
without effects. It captures the found target before calling old-state Post
**0x3d8e78**, assigns that captured target after Post returns, then calls current
Init **0x3d8e8c**. Post reentry can change current, but does not change the
outer captured target. Same-state changes still call Post and Init. The native
binding supports verified source string/nil/Boolean keys. Source numeric
formatting (`%d` vs `%f`) and identity formatting (`0x%0*x`) remain explicit
unsupported native failures; they are not silently filtered or accepted.
`string-key-probe.json` executes actual getString/getBool instructions and
literal strings for the supported conversion domain, including signed-zero.

`AISExternal::OnUpdate` **0x3dce64** calls actual Default OnUpdate **0x3dc798**,
reloads flags+b8, calls Lua `OnUpdate` if bit0 is set, calls State.Update
**0x3d8eb4**, then State.Conditions **0x3d8ea0**. Each State call reloads current
independently. Thus an Update transition selects the new state's Conditions.
State calls occur even when OnUpdate alias flag is absent. Default's substantive
body remains a required provider; the host composes genuine
`dh2_character_script_update(...,0,...)`, including collision counter200 boundary,
pause timer1000/event31 and controller Stop. Native provider failures stop at
their exact delivered prefix; delivered Lua errors are source Call diagnostics
and continue. The class does not install a fabricated Default no-op.

## API and dedicated VM capability

`character_script_states.hpp` provides owned `ScriptStates` methods:
`register_values`, `change_name`, `call_current`, `external_update`, `bind_source`,
`runtime`, `current_name`, `find`, `size`, `last_vm_status`. The standalone C
register/change/current/update kernels expose explicit synchronous lookup,
string assignment, Lua and Default services. Inputs/records are native typed
values; source arguments stay immutable, and providers cannot erase records
or destroy the registry during outer execution. Reentrant state insertion and
callback replacement are supported. Runtime/flags changes use recovered reload
points. Diagnostic native guards are not invented original permission gates.

`bind_source` uses the genuine runtime `bind_source_scoped` operation for
RegisterAIState/ChangeAIState. The scope permits only synchronous discarded Lua
calls; generic VM busy guards remain intact. Every Post/Init name resolves once
through the borrowed alias map then performs a fresh global lookup. All returns
are source-projected, including table `_this` effects, with no fixed return cap.
Nested callbacks shadow/restore the outer capability. Positive actual Lua status
is delivered and ignored by the original state transition; negative invalid or
unsupported capability failures remain explicit. Registration context must
outlive all VMs to which it is bound. No eager/default VM or active AIS is
invented by this class.

## Authored Crypt producer correction

The exact supplied `monster.luac`, `rene.luac`, and `_commons.luac` contain **zero**
RegisterAIState or ChangeAIState occurrences. They use native Character FSM
GetState/GetPyCst AIStates queries; rene registers an OnUpdate alias. It would
be incorrect to manufacture registry states for these files. Their byte sizes/
hashes and zero-occurrence inventory are recorded in the host report. Actual
top-level/Init execution still requires genuine GetPy*/GetProp/Character
providers. These tests use explicitly labelled original-derived state fixtures,
not fake accepted monster/rene callbacks.

## Proof and central integration

Actual original register/change/current/External instructions match optimized
ARM64 kernels across **1,476 cases and1,800 ordered services**, plus five atomic
native rejections. Original map insertion/string assignment and Lua/Default
callee bodies are fixture services in this differential, not original full VM
execution. `state-fixtures.bin` preserves original traces/current-state results.

The sanitized host audit replays that gold and invokes the actual dedicated
scoped VM, alias map and existing Default source kernel: **22,005 checks**,
zero ASan/UBSan/leak findings. It covers callback ownership/partial registration,
captured target across Post reentry, Update→new Conditions, same-state replay,
positive Lua errors, returned-table `_this` transitions, original nil/Boolean
keys, unsupported numeric key rejection, source stack5 and missing providers.
This is bounded class/source service composition, not full original AI/frame,
native object Value pushing, APK or device parity.

Parent central integration: add `character_script_states.cpp` to actual world
DSO; target `character_script_states_audit` uses `tests/character_script_states.cpp`
and links genuine world/runtime/dl with existing sanitizer flags. Its sole
argument is `reference/character-script-states/state-fixtures.bin`. A separate
current main-linked report can be generated after rebuilding central libraries:

```
python port/level-world/tests/character_script_states_host.py --main-linked --runtime-dir /home/adampalace/dh2-world-build/script-runtime
```

The default isolated audit runtime argument is intentionally overrideable; this
milestone ran against the frozen `/home/adampalace/dh2-script-callback-build`
runtime. Owner remains frozen and is not automatically extended with this map.
