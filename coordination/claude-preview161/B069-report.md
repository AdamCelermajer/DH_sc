# B069 report: Space does not open the chest (branch fix/b069, build `fix069`)

Status: root cause found and fixed; verified in the real EXE (quiet runs) and isolated tests. ctest 139/140 (only the allowed `session_skill_binding` fails).

## 1. Evidence

### 1.1 User screenshot (`user-shots/b069-space-does-not-open-chest.png`)
Tutorial chest of the Swamp (bones behind it, light shafts). The hero stands one tile beside it. The bottom-right button shows the fist/chest icon, NOT the sword: the chest IS the object of interest (OOI). So selection, facing and HUD are fine; only the use fails.

### 1.2 Reference video (Part 1, z_Zky7qQdYs, local ~208-212 s, 3 fps contact sheet `.local-inputs/claude-preview161/fix069/vid/seq.png`)
Direct observation: at 208.0-208.7 s the button is the sword while the hero walks toward the chest; at ~209.3 s, with the hero about 1.5 tiles from the chest, the button switches to the same chest icon as in the user's screenshot; at ~210 s the chest lid is open while the hero still stands about one tile away (not flush against it). Inference: the original does not require the hero to touch the chest. Not observable in the video: the exact distance and whether the hero walked on after the press (touch controls).

### 1.3 Reproduction (Preview 16 EXE, quiet runs, `r1`/`r2` in the scratch dir)
Hero placed at 100..300 units west/north of each of the 5 Swamp chests, Space press at frame 20:
- OOI = the chest at every distance up to 300 (icon 0).
- `Context container ... status=accepted` only up to 200 units; at 200.0x and beyond: `status=out_of_range`, no message, nothing happens (the user's "nothing"). Chest at 250: `out_of_range distance=250`.
Cause: `ContainerRuntimeV1::interact` had an invented horizontal gate `kContainerInteractRange = 200` ("the caller's query supplies it") while the OOI query (3D centre distance minus the owner melee radius <= 200 = OOI_Distance) selects the chest much farther away. Between the two the icon says "chest" and Space does nothing.
(With campaign triggers the controller is locked for the whole headless opening run: `refused_controller_locked`; that is the cinematic lock, B068 territory, not this bug. The user could move, so it was not locked.)

### 1.4 IDA (`pseudocode-all.c`)
- `Container::Interact` 0x3a0b38 (line 118492): no range check (only state 3/4 reject). Same for `OpenableContainer::Interact` (119125), `DestructibleContainer::Interact` (118603).
- Button chain: `Cmd_UseOOI -> Character::UseOOI -> CharAI::AI_SetTarget(OOI)`; the AI states `AISDefault::OnTargetInMeleeRange` 0x3dc284 (line 159090), `OnTargetInCloseRange` (159190), `OnTargetInRangedRange` 0x3dc560 call `CharAI::AI_IsInInteractionRange` (0x3d4f98, line 153119). If true: `Cmd_Stop` + `CharAI::AI_InteractWith` 0x3cff34 (149591: type from vt+144, `SM_SetInteractState`). If false: `Cmd_MoveTo(GameObject::GetInteractionSpot(target))` (GetInteractionSpot 0x38b228, line 103419: scene node `interaction_position`, else GetTargetPosition = object position +352), i.e. the hero WALKS to the object.
- `AI_IsInInteractionRange`: `d = |GetTargetPosition(owner) - spot|` (3D). Node present: radiusT=0, meleeO=0, limit 80. No node: radiusT = vt+148(target) (GameObject::GetInteractionRadius 0x38ad7c: half the smaller horizontal bbox side), meleeO = `AI_GetMeleeRadius`, limit = owner AITable row +24 = `InteractRadius`. `v = d - meleeO - radiusT`; in range = (type==8) ? v <= 0 : v <= limit.
- Data: Knight melee reach 200 (`PlayableActorWorld::melee_reach`), `InteractRadius` 250, chest radius 122.5, destructible radius 130.3. No `interaction_position` node was found in the chest scene files (byte grep of `go_chest_*.bdae`), so the "no node" branch applies.

## 2. Expected behaviour
Space with a chest OOI: if the source range rule holds, the chest opens at once (up to ~570 from its origin for this hero, the OOI query caps it at ~400); otherwise the hero walks toward it and opens it on arrival. Destructibles (type 8) need `v <= 0` (inside weapon reach), so a vase/barrel selected at 330-400 units makes the hero walk up and hit it. Manual movement, a lock, a spent container, a stall or 12 s cancel the walk.

## 3. Change (commits on fix/b069)
- New `features/interactions/interaction_approach_v1.{hpp,cpp}`: `interaction_in_range_v1` (the rule above) and `camera_relative_move_v1` (world direction -> move2D). Test `interaction_approach_v1_tests.cpp` (added to `features/combat/run_context_p16_tests.ps1`; CMake: one `target_sources` line).
- `ContainerRuntimeV1::interact(index, pos, max_distance = 200)` (default unchanged for existing callers) and `interaction_radius(index)` (visual bounds x placement scale).
- `main.cpp` (one commented block in the P16 CONTEXT dispatcher + one state line + one include): in-range -> `interact` with no distance cap; out of range -> approach state (`Context approach start / arrived / cancelled reason=...` log lines), walk injected into `gameplayInput.move2D` through the normal player movement. NPC talk and enemy paths unchanged.

## 4. Verification (real EXE `build-fix069/dh-foundation.exe`, quiet_run, scratch `.local-inputs/claude-preview161/fix069/`)
| Case | Before (P16) | After |
|---|---|---|
| tutorial chest, 230 units west | `out_of_range` (r2 `Aw250`) | `accepted`, `Container opened`, loot delivered once (`r9/after250`) |
| chest at 300 | `out_of_range` | `accepted distance=300` (inside source range 572) |
| chest, second press | - | `rejected_state` (no second loot) (`r9/dup`) |
| barrel/vase at 327-350 (needs a walk) | `out_of_range`/nothing | `approach start`, `arrived after=10 frames`, `accepted`, opened + loot (`r9/afterBarrel`, `r7/Ce350`, `r7/Cn340`) |
| start point off the floor (z 300, hero cannot move) | - | `approach cancelled reason=stalled` after 0.75 s (no endless walk) |
| 400+ from the chest | sword icon | sword icon, no OOI (unchanged; `r5/Aw400.png`) |
Frames: `r2` baseline vs `after250.png` / `before250.png` in the scratch dir (chest closed vs open).
Isolated tests: `interaction_approach_v1_tests` ALL PASS (range edge/limit, type 8, node branch, 3D, non-finite, camera basis round trip). Full ctest: 139/140.

## 5. Gaps / uncertainties (not guessed)
- Chest bbox semantics of `GetInteractionRadius` (object +324..+340) approximated by the visual's indexed bounds x placement scale; the real physical-owner bounds may differ (affects the limit by tens of units).
- The OOI candidate radius of containers is still 0 in the registry (source uses vt+148 there too); not changed here (would widen OOI selection; B075 territory).
- Walk speed/anim: the hero walks (not runs) through the normal locomotion; the original `Cmd_MoveTo` gait is not decoded. The stall/12 s cancel is a port safety, no source leash decoded.
- NPC talk (`talkToNpcObject`) has no range/approach yet (same source rule would apply); out of scope.
- Video does not show the walk-then-open sequence on mouse/keyboard; touch original. Distance at the open frame only estimated by eye.
- The tutorial chest in a full campaign run after the opening was not replayed headless (cinematic lock, B068).

## 6. Package files required
None new.
