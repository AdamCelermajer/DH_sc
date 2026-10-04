# Native ClearAggro stage

Additive production files: `character_clear_aggro.hpp/.cpp`. The original-only discovery and its reports remain unchanged. This stage uses real `dh2_aggro_apply(...,aggro_clear)` for outgoing/reciprocal storage and the existing actual `dh2_character_ai_set_target` for the null-target tail. It supplies the synchronous ownership choreography missing from the storage-only primitive.

## Borrowed API

`ClearAggroState16` contains the actual receiver `TargetState48` and its outgoing `AggroTable`. `ClearAggroCharacter32` contains the captured target identity, its reciprocal table, actual target bindings and live controller identity. These records and their table/binding projections are stable and caller-owned through all callbacks. Their receiver-owner, target-state and controller values may change at the source reload points. Callbacks must not retarget the stable receiver, table or binding pointers, or destroy their borrowed storage; alignment checks do not establish lifetime of arbitrary aligned memory.

`ClearAggroServices24::invoke` delivers OnDeAggro to the captured target embedded AI, then (if the source tail qualifies) Stop to the freshly loaded controller. A nonzero status is an explicit failed required service. The callback receives the private same-VM scope when invoked through Lua; it must not use generic VM reentry. `resolve` supplies a genuine retained Character projection for the raw Value identity; a missing/unknown identity fails delivery. It does not create objects or substitute lightuserdata for source object tables.

`dh2_character_clear_aggro(state,target,services,scope)` returns `0` complete, `1` malformed entry/storage rejection, or `2` provider/lifetime failure after observed effects. Null target returns before receiver storage is accessed. An absent outgoing entry does not touch reciprocal storage or notify, but it still evaluates the live target-ownership tail. Existing relation erases both stored directions before notification. After notification, the coordinator reloads receiver owner and target's current target, calls actual `AI_SetTarget(null,false)` if equal, then reloads the controller and requests Stop. It deliberately does **not** predict the tail using precomputed aggro facts. ClearAggro itself does not sync the target's `last_target`; original AI_SetTarget leaves that cached identity intact.

`dh2_character_clear_aggro_values` implements the source projected-Value wrapper: zero args returns zero results; first types2/7 accepted, null identity no-op, all other types ignored, extra args ignored. `dh2_character_clear_aggro_bind(vm,bindings)` installs only a genuine scoped `ClearAggro` callback, preserving the private VM/busy guard. The implementation owns no VM or world registry. `dh2_character_clear_aggro_scoped` is also available for future session registration without rebinding shared runtime APIs.

## Original versus optimized ARM64

`tests/character_clear_aggro_differential.py` compares the frozen original 1,792-case discovery to optimized ARM64 execution. It passes **1,792 cases / 200 ordered notification and ownership-tail calls / zero mismatches**. Original RB lookup/insertion/erase/rebalance and Value.getUserData execute in the gold producer; notification, original null-setter and Stop bodies are explicit ownership services there. Native execution uses actual aggro storage and actual native null-setter, with its separately proved debug services. The comparison captures map contents, live receiver owner, target identity, notification arguments and reloaded controller. It includes incoming-only/outgoing-only relations and synchronous owner/target/controller mutation.

`clear-aggro-fixtures.bin` SHA256 `c0971ea0f20a0333d30926ca7d0672145b8d50313c8b70d892db3c594944db0c`. Header words are `0x31414743,count`. Each record contains eight input words, twelve before-state words, twelve after-state words, trace count, then fifteen words per trace. Snapshot contains owner index, current target index, outgoing count/three key-value slots and incoming count/one key-value slot. Index `UINT32_MAX` means null. Record traces distinguish OnDeAggro, null SetTarget and Stop.

Reproduce the independent O2 instruction proof:

```powershell
& port/level-world/tools/build_character_clear_aggro_oracle.ps1
python port/level-world/tests/character_clear_aggro_differential.py
```

## Actual Lua host composition

`tests/character_clear_aggro.cpp` replays the source gold and uses the frozen target-pipeline fixture helpers to execute actual commons/monster top-level/Init and the real `monster_OnTargetOutOfSight`. Its NEW prefix service invokes the real ClearAggro coordinator; its Lua suffix uses the genuine scoped binding. The earlier historical target-pipeline test and its named fixture closure are unchanged.

The host checks map erasure before notification, target-side null setter before Stop, actual monster Stop/ClearTarget, nested InSight delivery through the same private VM capability, provider failure effects and actual VM-close finalization. Notification and Stop bodies remain explicit fixtures. A notification failure preserves completed map erasure and leaves the source tail unexecuted. The target scope is restored on every successful/error path. Invalid storage/value alignment and unaligned receiver replacement after notification are separately tested as native safety behavior.

Run only while central dependencies are stable:

```powershell
python port/level-world/tests/character_clear_aggro_host.py --build /home/adampalace/dh2-world-build
```

The actual host run passes **64,700 checks / 1,792 source records / 200 ordered calls / 24,504 compared words / 10 native guards**, plus the actual monster OutSight, one nested same-VM event and VM-close finalizer. Four notification and three Stop fixture deliveries occur in the Lua cases. ASan/UBSan/LeakSanitizer report zero findings.

The runner compiles only NEW ClearAggro shim/test and binds actual sanitized central world/data/runtime DSOs. It records compiler commands, `ldd`, `dladdr` and exact source/input/dependency hashes before/after. The parent central binder supplies central compiler provenance; current source hashes alone do not establish those DSOs' compiler inputs. Exact source/binary hashes are in `reports/character-clear-aggro-host-audit.json`. Dependency-bound world SHA256 is `6c6db1f6dc8da056d888c0592ef95c15818cdc97d066486d685a063a982268ca`; runtime `3f1886b22c2230e6ddda5c75fae248d0a710b75034eac3144489f9992b7f9945`; data `777e58075cab4a266df8c8dafbff4273e5dff7c1268dbbdab84ef5b512fe60fb`.

Frozen native source SHA256: header `878fdd198713793aa2e24917b019e0518a876eea0208ccf25dd377af328d576f`; cpp `3f365bef641bfb522c2c8ec3255b506ff527d9199868e97aed28aeefe150c33d`. O2 ARM64 oracle DSO SHA256 `e60674dcd49121ef867f43ee5d60a82217cf15e13969affe1b9ca3d0d5e993f5`.

No complete aggro world owner, notification implementation, controller/FindPath/combat/sound body, enemy acquisition, whole frame, device/APK or full monster AI parity is claimed. Future session integration can register the new scoped function and caller-owned binding without replacing the native identity/object producer or weakening errors for genuinely missing services.
