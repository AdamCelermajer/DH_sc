# B085 report: breaking a vase had no sound and no sword swing

## Verdict
Both were real gaps in ours; fixed generally for every destructible (vase, barrel, crate).

## Evidence
- Screenshot shot-5: vase broken (debris, 5 GOLD) while the hero stands with the sword upright (no swing).
- IDA AISDefault::OnAttack 0x3dc12c: swing hit marker; target is not a Character -> target->Interact (vt+152). AISDefault::OnInteract 0x3dc1b0 uses the same call. So the original hits a destructible through the hero's attack animation.
- IDA DestructibleContainer::Interact 0x3a0da0: non-final hit = stage clip + VoxSoundManager::Play3D(row sound); final hit = DestroyGameObject event + Container::Interact (activate clip, loot) + sound.
- Video Part 1 (t=100-260 s, 4 s montage, plus the earlier B075 montages 200-800 s): no destructible visible, so no direct video observation of the swing/sound (video has no audio anyway). Tutorial caption seen: "You will continue to attack the target until you move or the target is destroyed."

## Root cause
- Space on a destructible OOI called ContainerRuntimeV1::interact directly (no attack action, so no swing).
- interact() returned sound_id but main.cpp never played it (and the final hit returned none).

## Change
- features/combat/context_button_v1.cpp: press edge on a non-actor type 8 starts the weapon swing (attack_held); no repeat by holding (PC adaptation).
- features/containers/container_runtime_v1.cpp: final destructible hit also returns the row sound.
- main.cpp (B085 hunks): ctxUseContainer for an idle destructible faces it and starts the swing instead of interacting; out-of-range keeps the B069 approach (no swing until arrived); the hero's mainhand/offhand hit marker (classified by classify_original_named_animation_event) calls containerRuntime.interact for the in-range unbroken destructible OOI (no actor target) and plays the row sound through submit_world_item_sound.
- Test updated: context_button_v1_tests (barrel press = OOI + swing).

## Verification (real EXE, quiet, silent, isolated save)
- Scenario: player 120 units from barrel _prim_DestructibleContainer_..._47, Space press at frame 40.
- Log: "Context destructible swing ... frame=40" -> "attack_mainhand ... frame=56" -> "Destructible swing hit ... state=2->3 sound=325 frame=56" -> "Destructible sound ordinal=325 status=submitted uid=166 uri=data/sounds/sfx_break_vase.wav" -> "Container opened ... loot" frame 72.
- Frame at 48 (.local-inputs/claude-v20.1/fix085/mid/cap.png): hero mid-swing (arm raised), ring under barrel. Before: no swing at all.
- Build exit 0; ctest 165/166 (only session_skill_binding fails, allowed).

## Not verified / gaps
- Video of an actual vase break not found; swing timing vs original unverified (we use the authored hit marker, as in source).
- Multi-stage destructibles (stages>0) not present in Swamp (all stages=0); stage hit sound path wired, untested live.
- Real hearing of the sound not tested (silent mode); submission and file resolved only. Chest/drop sound belongs to B079.
- Swing-start range: uses B069 interaction range, not decoded for the melee swing.
