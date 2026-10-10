# Runtime player locomotion source projection V1

`runtime_player_locomotion_v1.hpp` exposes two feature-owned entry points:

- `load_runtime_player_locomotion_constants_v1` borrows the caller's existing PyDataConstants lookup and asks for exactly `AnimStancedAnim/SL__LIST_IPHONE`, `AnimStances/COUNT_IPHONE`, and `AnimStancedAnim/SL_IDLE`, `SL_WALK`, `SL_RUN`.
- `resolve_runtime_player_locomotion_v1` takes the source AnimTable ID, the same original `ItemTable`, current main/offhand item IDs, Character `flag1324`, loaded `AnimationTables`, and its exact clip dictionary. It calls the existing original-equipment-query and `GetAnimStance` kernels, then returns the selected Idle/Walk/Run sequence roots and a complete reachable sequence/phase projection.

Each projected sequence retains its source alias, ID, `Loop`, and `Type`. Every phase retains source `Anim`, `Redir`, `BlendOut`, camera/effect/sound, `Speed`, AnchorFX/CamDir/MoveGO/Swoosh flags, and RandomCam entries. Direct phases include the exact animation-dictionary alias and URI. Redirect phases name their target sequence; the reachable sequence bank includes each child and its full phases. The feature does not start, randomize, tick, blend, or replay animation and has no native owner or body clone.

## Recovered source rule

The source capture at `port/level-world/reference/live-animation-selection/NOTES.md` and frozen `GetAnimStance` asm establish the equipment priority: staff→3, bow→4, a non-shield offhand/dual wield→2, effective `HasTwoHander(false)`→1, no mainhand→5, otherwise→0. The source then returns `candidate` only when it is below `AnimStances/COUNT_IPHONE`; Android's actual count is 5, so bare-hand candidate 5 clamps to 0. Existing `dh2_equipment_queries_v1` reads category from ItemTable word37, shield/type from word22, raw two-hand slotting from word26, and the actual caller-provided flag1324 for the effective two-hander query.

For each state the source selects `base + stance` only when the same Android list mask intersects that state's source bit. Original Android constants are list mask `210`, count `5`, Idle bit `2`, Walk bit `16`, Run bit `32`: Idle and Walk are stanced; Run remains at its base. The actual Warrior/Rogue/Mage class properties resolve to animation tables 48/50/49. Their source Idle/Walk/Run bases are 262/280/271, so the selected roots include Idle stance variants 262–266 and Walk stance variants 280–284; Android Run remains 271.

The same notes recover authored root facts: source sequences have `Loop=-1`; Idle is a six-step `Type=2` choice, Walk/Run are one-step `Type=0` leaves with `MoveGO=1` and `Speed=1.3`. The adapter returns the loaded table values, not these examples as hard-coded output.

## Verification

Run with a caller-supplied original asset root:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File port/windows-foundation/features/equipment/run_runtime_player_locomotion_v1_tests.ps1 -AssetRoot port/android-native/app/src/main/assets
```

The strict LLVM-MinGW build/test passed against the Android asset root. It loads actual Character/Class/Loot/Animation tables and `animations_pycst.bin`, checks all Warrior/Rogue/Mage table IDs, exercises real ItemTable sword, dagger, staff, bow, two-hander, shield and empty-hand rows through the source kernels, checks the Android stanced roots, and compares every projected reachable phase field and alias/URI against its source `AnimationTables` and clip dictionary entry. Invalid item IDs, a missing clip dictionary leaf, and missing constants reject atomically. Output is isolated under `.local-inputs/runtime-player-locomotion-v1-tests/`.

This validates source selection facts and a complete phase manifest. The host still needs to supply the actual currently equipped main/offhand IDs, same-owner flag1324 and constants service, then connect the manifest to its existing animator/touch input lifetime. It does not claim live heading thresholds, movement transitions, clip playback, root-motion/pinning behavior, or gameplay runtime parity.

## Combat visual-plan bridge

`runtime_player_locomotion_program_v1.hpp` adds `build_runtime_player_locomotion_program_v1`. It consumes the resolved bank together with the same actual `AnimationTables`, clip dictionary, `AssetCatalog`, and same-character `CharacterVisualConfig`; it does not repeat stance or equipment selection. It returns `RuntimePlayerLocomotionProgramV1` containing an `OriginalCombatVisualPlan`, `OriginalSequencePolicies`, the actual typed `AnimationStep` at every source path, and a per-leaf named-clip receipt.

The bridge verifies every selected root, redirect, sequence alias, Loop/Type, phase field, dictionary alias, and URI against the supplied source graph before committing output. It preserves source sequence IDs/names, loop/type values, redirect ancestry, phase Speed/BlendOut/MoveGO, and exact clip URI. Runtime clip aliases are generated from the caller role, source state, source sequence ID, and original phase path; resolved files are added to the caller's existing visual clip bank after `resolve_content_path` succeeds inside that same AssetCatalog. Missing graph facts, unsupported symbolic leaves, missing assets, or alias collisions fail atomically. No class/weapon branch or sequence playback is added.

The strict Android-asset test now feeds actual ordinary sword, dual-wield sword+dagger, and staff ItemTable cases for each of Warrior/Rogue/Mage through the stance resolver and this bridge. It verifies selected source sequence IDs, all reachable typed policies and phases, exact named clip URI/path/rate metadata, and atomic rejection for a graph mismatch. Command remains:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File port/windows-foundation/features/equipment/run_runtime_player_locomotion_v1_tests.ps1 -AssetRoot port/android-native/app/src/main/assets
```

This is a source-backed plan/clip-bank adapter, not a playback claim. The downstream existing action playback helpers currently reject nonzero sequence Loop values or require explicit type-2 choices; this bridge preserves those authored policies for a later compatible caller and does not coerce them.
