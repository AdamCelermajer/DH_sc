# Source combo integration

This module borrows the existing `AttackState64`, source animator cursor, and original animation RNG. It has no combat/cooldown state or visual clock.

Call `combo::command` when the controller actually delivers an attack command, including while the actor is already attacking. Supply the live original `AttackServices16` backend. Its `IsAttacking`, target/search, eligibility, controller and flags queries must describe the existing actor. Do not return solely because combat already owns the pose: source continued input is accepted in that state when `last == 0`.

At each original animator boundary, call `combo::begin` or `combo::end` with the real stack depth, current step and source step count. Root depth zero identifies the authored combo group. Depth one identifies the group's pre/strike/recovery steps. Use live source ranged-capability and target-death providers. Supply `onOperation` to synchronously execute owner operations when they can reenter or mutate state. Applying recorded operations afterward is suitable only when those operations have no synchronous owner effects.

Apply source `SetStep` and `SkipNext` to the same animator frame before its normal completion logic. The recovered live scheduler permits `SetStep(count)` to stop the action, although its older body-only completion helper rejects an already-outside cursor. `advance_group` preserves this live behavior. Dispatch `ClearNonSticky`, raises `0x1a/0x1b/0x1c`, preattack and look operations through their existing owners. Do not substitute a second combo counter or restart an independent cooldown for each redirected group.

The root sequence's source type decides progression. Type one selects the first step initially and advances its authored root groups when continued input permits it. Type two selects an initial step using `choose_animation_start` and the existing original `AnimationRandom` stream. Type two must not inherit a player's ordered progression policy.

Source pre and recovery steps set `last = 1`; input delivered there is ignored and is not buffered. A live-target strike can accept continued input, and the source callback can skip the recovery step. No continued input stops a nonranged chain. A fresh attack after the previous chain has ended starts the first authored group again; separate late presses are not an instruction to cycle arbitrary animations.

Use one retained action timeline across the complete chain. Any skipped step must also skip its markers. Cooldown starts only when the source attack state actually departs, including interruption. Dynamic clock integration, input device delivery policy, sequence swaps after `0x1c`, controller locking and audiovisual consequences remain with the root runtime.

Required link sources are this module plus `character_ai_attack.cpp`, `character_attack_animation_v1.cpp`, `animation_selection.cpp` and `animation_scheduler.cpp`; existing content libraries provide the source metadata and asset interfaces.

Live `CombatSession` wiring is now available through `CombatSessionProfile.sourceCombo`. Enable it only with retained phase ownership and an empty `sequenceAction.group_path` for full-root playback. `combo_boundaries()` supplies per-frame source path/count/clip and one action generation across redirected groups. `source_attack_state()` exposes the same retained continued/last/index fields. Save rebind clears these transient AI fields and restarts a fresh attack at root zero.

The live test uses original character/AI/property data and confirms held input produces three varied Knight swings and three same-generation receipts. Release gives one; release after accepted continuation gives two; blocked controller input gives one. Diagnostic flags/search/OOI projections and missing look/preattack/sticky producers are logged. Original ranged, frontal-heading, and online backends reject when reached rather than pretending those providers exist.
