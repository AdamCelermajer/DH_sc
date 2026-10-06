# Actual Level counter producers and teardown boundary V36

This packet proposes **only** semantic field134/138 `uintptr_t` -> `uint32_t` against the frozen state-type-v38 header. Other fields and selected build graph are untouched. Original source SHA36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80. Base header SHAe3559999aff2a78e4dbb6e149ca06bf8d42162f00f0dc152e34a32f9adc9ac95.

## Complete retained-method counter access inventory

| Source receiver/method | Address | Access and actual producer |
|---|---|---|
| Level C1 3f3128 |3f317c;3f3224;3f322c|MOV r6,0 produces STR32 zero to134 and138. |
| Level constructor alias3f34c0 |3f35bc;3f35c4|Same STR32 zero stores. |
| LoadProcess state0 |3f74cc;3f751c;3f7520|MOV r7,0 produces zero to134 and138 (13c also reset separately). |
| LoadProcess state7 |3f726c;3f7270|MOV r3,500 produces literal scalar500 into138 before LoadFile retry loop. |
| LoadProcess state10 |3f7104..3f7110|Actual Application+38 ObjectManager receiver, read its32-bit field1c and copy to Level138 before InitPost retry loop. |
| LoadProcess common tail |3f6ea8..3f6ebc|LDR current138 and previous134, CMP current,previous, signed LT/GE select, STR32 minimum back to134. State38 skips this. |

These are all13 direct Level-field accesses found across retained original Level method ranges. The inventory preserves symbol ownership and excludes PC literal offsets, SP offsets and unrelated LevelConfig string pointers. Two ADD immediate308/304 matches are SP scratch addresses, not field-address producers. No address-of134/138 was found in these retained methods. This is a scoped retained-method audit; it does not claim arbitrary external script/native callbacks cannot access a Level.

The source values zero, literal500, ObjectManager scalar1c and signed min are scalars rather than addresses. Physical32-bit raw-word storage is faithful; signed counter comparisons use memcpy bit interpretation. No pointer casting from legacy64-bit uintptr_t cells. Counter134 initialized0 remains0 for ordinary nonnegative138 producers under the original min; do not replace it with guessed elapsed time, decrement counts, percentages or smoothed progress.

Actual ARM instructions verified19 producer slices (constructor aliases/state0/500 across three poison patterns; ObjectManager1c copies across seven boundary values). C1 r6 and state0 r7 zero-register inputs are explicit slice fixtures justified by preceding MOV; full C1 is separately owned. The prior lifecycle packet already verifies195 original signed-tail/progress cases.

## Save and incomplete cancellation

Full original QuickSave3f059c executed2560 cases with explicit player/IsDead/online/hosting/Save observer services. **Zero** LevelSavegame.Save requests at incomplete states0..37 or invalid terminal39. Eligibility requires real save_ec, exact state38, real nondead player, and offline or permitted host. Four eligible cases verify original saved-position copy and temporary39 flag clearing/restoration for force.

Original `Unload` still calls `SG_SaveAllPlayer` separately. Full original SG_SavePlayer(Character*,bool)3efa54 executed80 cases with explicit service observers:16 SG_Save requests were observed at incomplete states0/7/36/37 for real player branches. The method has no Level.state gate. This verifies save requests/control flow, **not actual persistent writes**. Character SG_Save is main-owned and may itself gate/block writes. Thus QuickSave's guard does not make full Unload an appropriate incomplete-abort implementation.

`lifecycle_v36_counter_teardown.hpp` supplies separate required providers: `abort_incomplete` for actual states0..37, `unload_and_destroy_completed` for exact38. It selects once and preserves the normal-Unload path when actual source resets state130 to0 while teardown is still pending. No fallback into normal Unload for an absent abort service. Completion drops only retained borrow/service pins after real teardown finishes. Main must implement the actual incomplete destruction path without normal player-save side effects.

## Real borrow and integration

`lifecycle_v36_counter_borrow.hpp` is a typed template over the actual selected CanonicalLevelContextV1. It requires actual completed C1 owner/phase, existing loading_fields_v26, exact shared owner/identity and pointer equality between progress30/state130 and the same constructor cells. It borrows addresses of same134/138, creates no state copies, and statically rejects legacy uintptr_t134/138. A native field-width/layout update alone does not supply missing stage effects.

1. Apply reviewed scalar header diff only to the private coherent successor graph; do not overwrite root/menu work. Native C1 body already writes exact zeros through those fields and needs no source-body change.
2. Select successor context + prepared-source TU exactly once. Instantiate the new typed borrow against actual context after successful C1; preserve its pin throughout lifecycle callbacks.
3. State0 producer must execute at its original source prefix. State7 must write real138=500 before the authentic file-loading loop. State10 must lend actual same ObjectManager1c producer (native authoritative registry/tree count), once before original InitPost loop. Do not supply fixture map counts in production or reread per retry when source reads once.
4. Native real progress tail reads/writes these actual cells; actual NativeEndLoading remains sole36->37 menu producer.
5. Bind separate actual abort and normal Unload/destruction providers using `lifecycle_teardown_provider_v36`; feed that closure to lifecycle_v36's cancellation callback. Providers must retain source cursors, be bounded and release actual resource owners before complete.

Private ASAN/UBSAN shape tests pass6 borrow cases and3 teardown routing cases. They are explicit fake-shape binding tests; actual production C1 did not execute in this probe. Parent will validate actual context instantiation/full C1 in the composed graph.

## Reproduce

```text
python reference/level-init-v36/level-counter-producers-v36-proof.py
wsl.exe -d Ubuntu -- cmake -S /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/tests/cmake-lifecycle-v36-counter-borrow -B /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/lifecycle-v36-counter-borrow -G Ninja
wsl.exe -d Ubuntu -- cmake --build /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/lifecycle-v36-counter-borrow -j 1
wsl.exe -d Ubuntu -- /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/lifecycle-v36-counter-borrow/dh2_loader_lifecycle_v36_counter_borrow
```

The original oracle needs the existing local Unicorn runtime and original shared ELF as a read-only input. No shared/core graph edits, emulator launches or persistent save writes occurred. Source bodies, original map/module/activation effects and actual save services remain required main/loader integration; whole Init is unfinished.
