# Combo worker handoff

Current worker stopped further implementation edits for the human's GPT-6 Luna HIGH worker policy. No core Session/main/CMake edits were made during the latest consumer task.

## Ready files

- `port/windows-foundation/features/combo_chain/source_combo_consumers.hpp`
- `port/windows-foundation/features/combo_chain/source_combo_consumers.cpp`
- `port/windows-foundation/features/combo_chain/source_combo_consumers_tests.cpp`

The consumer implementation compiles to a native Windows object with the bundled compiler. The test executable has NOT linked or run successfully. Do not report its assertions as passing.

Compiler: `.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe`.

## Genuine source behavior

`SourceComboConsumers` borrows a lease, SAME `AttackState64`, SAME `TargetBindings48`, SAME `ControllerCommandState32`, actual game position and heading-angle storage. It creates no private target/FSM/clock.

- Look operation uses `dh2_character_controller_character` to execute original Cmd_LookAt admission and Character object LookAt; genuine stable target-position backing is copied into the original point body `dh2_character_look_at_point`. Only the caller's actual heading-angle cell is published; current visual angle is not snapped.
- ClearNonSticky checks actual `TargetState48.changed` (AI+4b). Sticky nonzero returns without reaching target services. Zero calls full `dh2_character_clear_target`, which performs deep AI_SetTarget(NULL,false) then SyncLastTarget. Prefix effects are published even if the deep setter fails. The actual host target view publisher is required when reached.
- Preattack uses `world_ai_can_attack_v1(owner,NULL,currentTarget,actualQueries)`. Only a true result reloads actual active AIS+1c storage; null active AIS returns without inventing a callback. Nonnull active AIS requires actual virtual OnPreAttack(index) backend.
- `refresh_owner_fields` copies actual flags528, heading-active1b5, OOI14a4, signed OOI type14a8 into the same AttackState projection. All actual backing/lease is required. Continued/index/last/finisher are preserved.
- The binder handles only look/preattack/clear. Cursor operations stay with existing retained playback.

Primary evidence:

- `port/level-world/reference/character-animation-ai/helpers/reference/original-functions.asm`: complete OnPreAttack0x3d0ed4.
- `port/level-world/reference/character-animation-ai/NOTES.md`: preattack predicates, active AIS reload, clear helper, source cursor behavior.
- `port/level-world/reference/character-attack-initialization-v1/attack-steps.asm`: ClearNonStickyTarget0x3d8d70.
- `port/level-world/character_world_ai_can_attack_v1.cpp`.
- `port/level-world/character_controller_commands.cpp` and `character_path_commands.cpp`.
- `port/level-world/character_target_bindings.cpp`.

## Current native test blocker

Linking the actual `character_target_bindings.cpp` also pulls Lua binding functions from the same translation unit. With `-ffunction-sections -fdata-sections -Wl,--gc-sections` the Windows linker still reports these unresolved symbols:

- `dh2_script_vm_bind_source_values`
- `dh2_script_vm_bind_source_scoped`
- `dh2_script_callback_scope_valid`

Do not add fake VM stubs or claim the test ran. Options to assess: link the genuine native script runtime, or require an explicitly supplied genuine clear-target backend callback so the feature does not force root's unbound Lua translation unit into its Windows build. The actual setter semantics must remain the existing source kernel, not an invented clone.

Latest attempted command included consumer files plus `character_target_bindings.cpp`, `character_world_ai_can_attack_v1.cpp`, `character_controller_commands.cpp`, `character_path_commands.cpp`, and existing foundation/recovered/XML static libraries. No executable was produced.

Tests authored (unrun): source LookAt lock/forced gates, sticky-return/no-provider path, nonsticky deep clear/publication, failed setter prefix retention, eligibility before preattack, active AIS reload during callback, rejected target without active storage, genuine null AIS, missing-provider failures, owner-field refresh preservation.

## Root API/integration request

Root must add an operation consumer provider to the existing `CombatSession::combo_boundary` operation dispatch. Let it bind the above actual canonical records and handle look/preattack/sticky operations before the current explicit diagnostic fallback. Root must supply real target services/publication, actual position/heading borrow, active AIS storage/endpoint, and actual owner field provider. No new mirroring target store should be introduced just to make the binder appear integrated.

Root owns Session/main/CMake exclusively. Existing v17 live combo path is already tested and released; these new consumers are not yet integrated and are not part of that verification.

## Earlier verified combo work

The original `source_combo_chain` feature, retained hierarchy hooks and live Session tests passed native tests. Held input produced actual groups0/1/2 and three receipts in one action generation; release one, accepted-then-release two, controller block one. Save restore reset unpersisted continuation/index/target and fresh attack began root0. Reports and integration documentation already exist.

## Unresolved W/S evidence

New read-only probe `tests/source_reversal_probe.cpp` and `reports/source-reversal-diagnosis.json` reproduce the v17 reversal arc with exact v17 Run assets and original heading/rotation/quaternion displacement kernels. U-turn settles15frames (~250ms). Motor versus direct original displacement differs only0.000183 source units. Exact released SHA: `c1713329afa44790b53b2ce8a5a3d9ed3545b4bd0b770104d7d4500bab983706`.

Source finite turn and continued old-facing root displacement explain repeated lateral arcs. Original reference footage uses joystick; video_fidelity found no defensible matched exact repeated180-degree reversal. Complete-game intended feel/magnitude remains unverified. No instant-turn or lateral-clamp change was made or recommended as an original fidelity fix.
