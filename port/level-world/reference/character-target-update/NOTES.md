# CharAI target-update producer

`character_target_update.hpp/.cpp` reconstructs the complete `CharAI::_UpdateTarget()` body at `0x3cb908` (556 bytes, function SHA256 `d8f8d308b4778d2c4bd43b1e72df8476cb18f1137a7a774cced65cc3c7054ce0`). The original ELF SHA256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. Twelve complete routines are captured in `original-functions.json` and `reference/original-functions.asm`; four additional helpers and the state getter have separate captures. Literal pools are included in function-sized captures and are data, even where the disassembler prints them as instructions.

## Source order

1. Ask the live owner's `CharStateMachine::SM_IsAwaitingToSpawn()` (`0x3c0230`), then reload the owner and ask `SM_IsInLimbus()` (`0x3c01c0`). Either true returns before target queries. These are **not** a dead-state predicate. Both call `SM_GetState()` (`0x3c01ac`), which reads the word at the pointer stored at state-machine offset `+0x20`, or returns `UINT32_MAX` for a null pointer. Awaiting means state `17`; InLimbus is true only for state `0`.
2. If target `AI+0x40` is null, return. Invoke target virtual `+0x88` (`IsInteractive(owner)`). A false result zeros current target and last target (`+0x44`) and raises Character event `0x0c` with null payload. Otherwise reload target and return if it has become null.
3. Execute the owner's discarded `GetCharAIId()` (`0x3a2fec`). Read target virtual `+0x34` (`IsDead()`). Capture `uint8_t(result ^ 1)` as new alive. Compare with the live prior alive byte `AI+0x48`: nonzero to zero raises `0x0a`; zero to nonzero raises `0x0b`. Event payload is the target loaded at the event call. Store the captured alive byte **after** the callback.
4. Reload target; if nonnull, invoke `AI_IsInSight(target)` (`0x3d4ed8`). Compare the raw result word with the live previous sight byte `AI+0x49`: leaving sight raises `0x0c`; entering sight raises `0x0d`. Store the captured low byte after the callback. A raw `256` stores byte zero but still enters the range branch, because the branch tests the raw result.
5. Reload target and require the captured sight word to be nonzero. Invoke target `IsInteractive()` again, then owner virtual `+0x124` (`CanRangeAttack()`). For ranged owners, ask `AI_IsInCloseRange()` (`0x3d63d8`), then, only when false, `AI_IsInRange()` (`0x3d6604`). Raise close-range `0x10`, ranged-range `0x0f`, or out-of-range `0x0e`. For melee owners ask `AI_IsInMeleeRange()` (`0x3d6188`) and raise `0x11` or `0x0e`.

Range events are generated on every qualifying update; they are not deduplicated transition notifications. No Character `RaiseEvent` return is used as a veto here. Delivery failure in the native callback API is a separate boundary result.

## Native interface and lifetime

`dh2_character_target_update(TargetState48*, TargetUpdateServices16*)` borrows the exact live target/owner projection shared with the frozen `AI_SetTarget` reconstruction. Every request carries a typed service number, event ID, receiver identity, and other/payload identity. Predicate replies remain raw words. Synchronous services may change owner, target, alive, sight, or recursively execute the coordinator. The coordinator reloads them at original instruction points; it does not snapshot an entire frame or queue callbacks.

All backing state, owner projections, services and receiver objects must remain live until the outermost call returns. Object identities are opaque source identities rather than dereferenceable native object pointers. Return `0` is complete/source skip, `1` is malformed entry before effects, and `2` is failed provider/lifetime projection after the executed prefix. There is no rollback. Clearing the target during the discarded GetCharAIId call would make the original dereference null at its next instruction; native reports `2`. This invalid lifetime case is a native guard, not an extra successful source branch.

## Connecting genuine queries

The new differential corpus isolates `_UpdateTarget` choreography with explicit query and Character `RaiseEvent` services. It does **not** prove those query implementations or full Character event routing merely by supplying their results.

Existing native source backends can supply parts of these services:

- `character_target_providers`: original Character/base-object virtual classification, dead and interactive policies, with real borrowed AI/property/object fields. The caller must select the actual receiver's virtual implementation.
- `character_target_bindings`: exact GetCharAIId and strict sight arithmetic. `AI_IsInSight` obtains both original `GameObject::GetTargetPosition()` results, forms `((dx*dx + dy*dy) + dz*dz)`, then compares the squared decoded AI-row sight radius strictly greater than that distance. Preserve original GetTargetPosition selection and property/AI row ownership, rather than substituting rendered translations.
- `character_combat_queries` and `character_attack_geometry`: decoded equipment/property/radius and inclusive ranged/strict melee geometry. The complete close/melee helpers additionally resolve the target handle, Character/type/interaction and debug/fallback branches; missing non-Character interaction/range services remain explicit. Do not wire a generic distance predicate as the full helper.
- The current state-machine owner can provide the two exact state predicates. Their source pointer/value selection is documented above, not inferred from a controller animation state.

## Character events precede external AIS callbacks

The service `target_update_raise_event` must invoke the original Character event route. It must not directly invoke the root session's `dispatch_target` endpoint. The captured CharAI handlers include these prefixes, before dispatch through the active `AI+0x1c` receiver:

| Character event | CharAI handler | Prefix following debug load/query | Active AIS virtual |
|---|---|---|---|
| `0x0a` | OnTargetDied `0x3d1f60` | clear `continued` byte `AI+0x78` | `+0x40` |
| `0x0b` | OnTargetRevived `0x3d1eb4` | no target-byte write | `+0x44` |
| `0x0c` | OnTargetOutOfSight `0x3d2410` | owner's embedded AI `AI_IsTargetACharacter 0x3d5484`; when true, capture that AI receiver, `AI_GetTargetAsCharacter 0x3d5450`, then `AI_ClearAggro 0x3d6d68`; clear changed `AI+0x4c` | `+0x48` |
| `0x0d` | OnTargetInSight `0x3d22e4` | original AI ID selects the 68-byte AI row's sound word `+0x24`; reload owner GetTargetPosition; copy XYZ; `VoxSoundManager::Play3D 0x36b5d8(sound,point,true,0,-1,-1)` | `+0x4c` |
| `0x0e` | OnTargetOutOfRange `0x3d1e00` | clear changed | `+0x50` |
| `0x0f` | OnTargetInRangedRange `0x3d1d4c` | clear changed | `+0x54` |
| `0x10` | OnTargetInCloseRange `0x3d1ca0` | no changed-byte write | `+0x58` |
| `0x11` | OnTargetInMeleeRange `0x3d1bf4` | no changed-byte write | `+0x5c` |

All eight handlers execute `DebugSwitches::load 0x337888`, construct a string with `0x3140ec`, call `0x337a88`, and destroy it with `0x3139ac`; the debug query result is discarded. The active AIS receiver is loaded only after those calls and the extra OutSight/InSight services. Died, OutSight and ranged/out-range handlers load the active pointer immediately before clearing their byte and call that captured pointer. No service occurs between that load and store. Other handler return values are ignored. Full native handler-prefix implementation and Character RaiseEvent/CharAI routing are separate integration boundaries at this freeze.

`AI_IsTargetACharacter` and `AI_GetTargetAsCharacter` must not be renamed as master lookups; `AI_ClearAggro` must not be substituted with SetTarget. Root's external AIS dispatch endpoint is only the endpoint after these prefixes.

## Proof and reproduction

- O2 ARM64 original-instruction differential: **2,225 cases, 10,567 ordered services, 1,625 Character RaiseEvents, zero mismatches**. It executes complete original `_UpdateTarget`, including virtual branch instructions. The 2,048 Boolean combinations cover all decisions; nine gate combinations and 60 noncanonical raw-word cases exercise source byte truncation; 108 cases mutate or recursively reenter at query/event boundaries.
- ASan/UBSan host replay of exactly the same gold: **2,225 cases, 57,843 word checks, eight malformed guards, 12 provider/lifetime failure checks**, zero findings. Failure paths retain prior source effects. Misaligned state, owner and service pointers are rejected before dereference.
- Gold: `target-update-fixtures.bin`, magic `CTU1`, SHA256 `854ddc5c7b7e681be831862fc0ebe21c699027f04958a1d47332096f89731fce`. Record contains 16 input words, seven resulting state words, a trace count, then four-word logical service entries. Entries 11/12 mark synchronous nested call boundaries.
- Reports: `reports/character-target-update-arm64-differential.json` and `reports/character-target-update-host-audit.json`. Source, original, corpus and actual binary hashes are recorded; host runner checks source hashes before and after compilation/replay.

From the repo root:

```powershell
& port/level-world/tools/build_character_target_update_oracle.ps1
$env:PYTHONPATH='C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages'
$env:PYTHONDONTWRITEBYTECODE='1'
& C:\Users\adamc\AppData\Roaming\uv\python\cpython-3.12.13-windows-x86_64-none\python.exe port/level-world/tests/character_target_update_differential.py --library .local-inputs/character-target-update/libcharacter_target_update.so
& C:\Users\adamc\AppData\Roaming\uv\python\cpython-3.12.13-windows-x86_64-none\python.exe port/level-world/tests/character_target_update_host.py
```

For central integration add only `character_target_update.cpp` to the world library and link `tests/character_target_update.cpp` as `character_target_update_audit`. Test argument is this original CTU1 gold. The runner accepts `--main-linked /home/adampalace/dh2-world-build/character_target_update_audit` and saves a separate report, with executable/dependency hashes before and after replay. A prebuilt replay does not claim central compiler provenance; the central build owner must bind compile commands separately. No CMake, renderer, runtime, session or APK files were edited for this stage. No live full-target-event or gameplay parity claim is made.
