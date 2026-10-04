# CharAI target event prefixes

`character_target_events.hpp/.cpp` reconstructs the eight complete CharAI target-event handler bodies, following the Character `RaiseEvent` route and preceding any active AISExternal/Lua endpoint. It does not replace Character event routing with a direct script call. Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

The related `character-target-update` stage reconstructs the source producer `CharAI::_UpdateTarget` separately. Its captured transition values and low-byte stores occur **after** these synchronous event callbacks. This module must operate on that same caller-owned live `TargetState48`; duplicating the target fields into a second unrelated state would break those reloads.

## Exact handlers

| Character ID | Handler address | Body following debug prefix | Captured active AIS virtual |
|---|---|---|---|
| `0x0a` | OnTargetDied `0x3d1f60` | clear `AI+0x78` continued | `+0x40` |
| `0x0b` | OnTargetRevived `0x3d1eb4` | no target-byte write | `+0x44` |
| `0x0c` | OnTargetOutOfSight `0x3d2410` | query owner's inline AI target-Character; resolve target-Character and clear its aggro when true; clear changed `AI+0x4c` | `+0x48` |
| `0x0d` | OnTargetInSight `0x3d22e4` | original AI-row sound; copied owner target position; source Play3D | `+0x4c` |
| `0x0e` | OnTargetOutOfRange `0x3d1e00` | clear changed | `+0x50` |
| `0x0f` | OnTargetInRangedRange `0x3d1d4c` | clear changed | `+0x54` |
| `0x10` | OnTargetInCloseRange `0x3d1ca0` | no changed-byte write | `+0x58` |
| `0x11` | OnTargetInMeleeRange `0x3d1bf4` | no changed-byte write | `+0x5c` |

Every handler executes `DebugSwitches::load 0x337888`, string construction `0x3140ec` with the exact case-sensitive name **`isTracingCharAIEvents`**, query `0x337a88`, then string destruction `0x3139ac`. Query return is discarded for every raw value. The native services preserve all four calls in that order. Allocation/string backing and the fixed source DebugSwitches receiver are borrowed provider ownership; no invented tracing gate suppresses the handler.

OutOfSight executes `AI_IsTargetACharacter 0x3d5484` on the live owner's embedded AI (`owner+0x3c8`). If true, it **reloads owner**, captures that inline AI receiver, invokes `AI_GetTargetAsCharacter 0x3d5450`, then calls `AI_ClearAggro 0x3d6d68` on the captured inline AI, using the returned Character identity. The native requests carry the owning Character identity as the projection key for that embedded receiver; ClearAggro retains the same owner projection even if the GetTargetAsCharacter callback changes the current owner. These functions are not master lookup or SetTarget aliases. Both target-Character helpers construct a true handle and invoke `GetChar 0x33ff54`, with a null-target short circuit. Their complete captures and the complete red-black-tree aggro body are included for future provider implementation; they remain explicit services here.

InSight captures the global AI table pointer before calling the owner's `GetCharAIId 0x3a2fec`. It selects the **68-byte original AI row's sound word at `+0x24`** using that returned ID. It then captures the global VoxSoundManager receiver before calling the reloaded owner's `GetTargetPosition 0x3935dc`. Returned XYZ words are copied into local scratch and passed to `VoxSoundManager::Play3D 0x36b5d8` as `(soundID, XYZ, true, 0, -1.0f, -1.0f)`. Native `ai_sounds` is an explicit row-order projection of that genuine member, not a guessed sound ID or transformed AI schema offset. The table/count snapshot occurs before the AI-ID callback, the sound-manager identity snapshot after it and before the position callback. Raw signed sound words and all position float bits are retained, including signed zero, NaNs and infinities.

After debug and any extra services, handlers reload `AI+0x1c` active AIS. Died, OutSight and ranged/out-range capture it immediately before writing their byte; no intervening source call occurs between load and store. They still perform the prefix/store when active is null. When nonnull, the captured receiver is dispatched. The AIS return value is ignored. No callback-availability mask is read by these CharAI handlers.

## Interface and downstream wiring

`dh2_character_target_event(TargetEventState32*, uint32_t event, const TargetEventServices40*)` accepts original Character IDs `0xa..0x11`. This is a different enum space from `object_identity::TargetEvent`; callers must map the active virtual implementation explicitly. `TargetEventState32` borrows the stable live target projection and carries the active AIS identity and continued byte. `state.target` and all backing state must remain stable/live until the outermost handler returns; callbacks may change its owner, target bytes, active and continued at source reload points, and may synchronously reenter. They must not replace the receiver projection or destroy any still-borrowed receiver.

`TargetEventRequest64` carries typed service/event IDs, source receiver/argument identities, debug name and exact sound ABI words. Query replies use `TargetEventResponse24` word/identity/XYZ fields. For the three OutSight services, `subject` is the Character owner of the inline AI, as described above. For sound, it is the captured sound-manager identity. For `active_dispatch`, it is the captured active AIS identity; `event` selects the exact virtual slot from the table above. These are synchronous services, not a work queue or an always-accepted policy.

Return `0` is source completion/null-active skip; `1` is malformed entry before callbacks; `2` is failed provider/lifetime projection after already executed effects. Sound table validation is only performed on InSight after the source prefix and original AI-ID call. Other handlers do not require a sound table. String/provider cleanup on a delivery failure belongs to the provider; this coordinator owns no allocated source strings or receiver objects.

At the active endpoint, use the actual active AIS virtual body:

- AISExternal Died/OutSight/InSight wrappers call their named script methods unconditionally; ranged/close/melee wrappers use their genuine ScriptOwner callback flags. Read **live loaded session availability** only at that endpoint. The separately frozen `object_identity` dispatch module proves those wrappers; it is not a substitute for these CharAI prefixes.
- Revived `+0x44` must resolve the selected AIS implementation. Do not invent an `OnTargetRevived` Lua callback if the actual selected virtual is empty/inherited.
- For AISPlayerIPhone, inherited Default handlers can be empty. Resolve the actual selected receiver and vtable instead of forcing every event into a monster script.
- The complete upstream Character RaiseEvent/CharAI OnEvent gate/order, genuine target-Character conversion and aggro ownership, source sound resource/backend, and safe same-VM scoped script dispatch remain caller integration responsibilities. This stage changes no runtime/session/renderer files and makes no live monster-target, audio output, full-frame or gameplay parity claim.

## Original and native proof

`original-functions.json` captures eight complete handlers and three OutSight helpers (11 routines), with exact byte hashes. `ai-row-loader/original-functions.json` separately captures `AIProps::read 0x506f3c`. The differential executes that actual loader over bundled `ai_pyarray.bin`, consumes all bytes, and uses all **76** decoded row sound members in both original and native handler tests. Asset SHA256: `bfe56f4a07dbe49bb55543137ae343334082b6a340ca156f367ee51e3b2771bc`. Fixture assertions are not presented as proof of unresolved target/aggro/sound/VM services.

- Complete original ARM32 versus O2 ARM64: **2,013 cases / 11,534 ordered services / zero mismatches**. Cases cover all eight handlers, null/two active receivers, byte extremes, raw debug replies, all decoded AI rows, null/replaced target-Character results, noncanonical Character predicates, exact nonfinite XYZ copies, and mutation or synchronous reentry at each service. Table replacement during AI-ID retains the prior pointer; manager replacement during position retains the prior manager; owner replacement during target-Character resolve retains the captured inline AI for ClearAggro.
- ASan/UBSan exact gold replay: **2,013 cases / 196,622 word checks / ten atomic malformed guards / 14 provider/lifetime failure checks / zero sanitizer findings**. It checks every ordered request and callback-visible owner/active/continued/changed snapshot. Failure during active Died dispatch preserves its prior continued-byte clear.
- Corpus `target-events-fixtures.bin`, magic `CTE1`, SHA256 `c10972d9b56a6f72c749065367db1a4cfbb23fbfd8a9a5ac85a7ec9946852317`. Header is count/AI-row-count/genuine sound words. Each record stores 13 input words, six result words, a trace length, then 16-word request/snapshot entries. Entries 11/12 mark nested handler boundaries.
- Stable production header SHA256 `36ecab0282c65c34b782a09aececdf0249f0374afbdda34c5125a651a1df30cc`; source SHA256 `ce3d46cae0cd40e5de2cbd1312a4cb9d3ecadbefedff30fd3fabeeb7ebec07a5`; isolated O2 ARM64 DSO SHA256 `47b4cfbfb96e9827428ece740f28ddc5b6107e4bde5e269aeaa6613e1d6a7343`.

Reports are `reports/character-target-events-arm64-differential.json` and `reports/character-target-events-host-audit.json`. Both bind source/original/gold inputs; the sanitizer runner hashes its inputs before and after compile/replay and records the actual compiler command and executable hash.

## Integration and reproduction

From repo root, build the isolated optimized oracle with `port/level-world/tools/build_character_target_events_oracle.ps1`. Set the same Python/Unicorn environment used by the other frozen original-instruction proofs, then run:

```powershell
& C:\Users\adamc\AppData\Roaming\uv\python\cpython-3.12.13-windows-x86_64-none\python.exe port/level-world/tests/character_target_events_differential.py --library .local-inputs/character-target-events/libcharacter_target_events.so
& C:\Users\adamc\AppData\Roaming\uv\python\cpython-3.12.13-windows-x86_64-none\python.exe port/level-world/tests/character_target_events_host.py
```

Central build owner may add `character_target_events.cpp` to the world DSO and link `tests/character_target_events.cpp` as target `character_target_events_audit`. Argument is the original CTE1 corpus above. The host runner's `--main-linked /home/adampalace/dh2-world-build/character_target_events_audit` path records a **separate** main-linked report and actual executable/dependency hashes before/after replay. It does not claim central compiler provenance; that must be bound by the build owner. No CMake or APK modifications were made by this worker.
