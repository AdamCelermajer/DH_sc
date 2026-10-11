# B079 report: chest opening (hero interaction animation, items thrown out with an arc, sounds)

Branch `fix/b079` (from windows-foundation 2a8faf0f). Build `DH_wt/build-fix079` (`build exit 0`, ctest 166/167, only `session_skill_binding` fails as allowed).
Scratch/evidence: `.local-inputs/claude-v20.1/fix079/` (r2 = v20 baseline vs fix, r4 = final sheets `r4/H.png`, video sheets `vid/`).

## Reported symptom (shot-1)
Opening a chest: the hero has no opening animation, the drops "just appear" on the spot, no sound.

## Evidence
### Reference video (P1, `z_Zky7qQdYs`, 30 fps, no audio), chest at t = 209.3..212.5 s (sheets `vid/c209.png` 8 fps, `vid/d.png` 15 fps)
Observed:
- The hero arrives at the chest, the red target ring appears, then he plays a distinct interaction pose (crouch/lunge, sword held low and pointing out) for ~1.2 s.
- About 0.4 s after the pose starts the lid opens.
- Items are thrown out: a mace and a red armour piece rise clearly above the chest, tumble, one drifts left, then they fall and land (about 1.3 s after the lid opens). The name label ("Useless Mace") appears after they land.
Inferred: nothing about the sound (no audio track).

### IDA (`pseudocode-all.c`)
- `CharAI::AI_InteractWith` 0x3cff34: calls target vt+144 (`GetInteractionType`; `OpenableContainer::GetInteractionType` 0x3a16cc returns 0, `DestructibleContainer` 8) and `CharStateMachine::SM_SetInteractState` 0x3c64ac with that value as the index into CharAnimTable "Interact" -> state 13 (CSInteract). The target is NOT interacted at the press: the AI event handler (case 13, string "interact", ~line 152765) calls CharAI vt+160 on the clip's "interact" marker.
- `Container::Interact` 0x3a0b38: sets state 3, plays chest clip "activate", `VoxSoundManager::Play3D(GetSound(), object position)`; the clip's "opened" event -> `__EventCallback` 0x3a0c64 -> `Container::DoOpen` 0x3a0a98 -> `ItemObject::DropLootTable`.
- Item motion: `ItemObject::InitAgain` 0x3ec0f0 plays Play3D(drop sound) and dispatches `PlayClip(0, loop=false)` on the item visual (`IsAnimated` 0x3ebbfc = 1). The BDAE clip is the arc (below). XY slide (600 u/s, stop 80 short) from B047 is unchanged.
- Data: CharAnimTable Interact list of the prince rig = `prince_interaction.bdae` (types 0..2), `prince_dialog`, `prince_interaction_shrine` (`engine-animation/reference/prince-registration/probe.json`). `prince_interaction`: 1200 ms, marker `interact` at 467 ms. `itemdrops.bdae` has ONE scene clip `AUTO_SceneAnims_Range` 0..2566 ms; every `dummy_itemdrop_*` node rises 17.5 -> ~620 units (apex ~0.47 s), tumbles and is back at rest at ~1.6 s (gold: the six `JEROME_*` coin bones, ~660 apex).

## Match statement
- Before (v20 EXE, `r2/before`): NOT matching the video/IDA. Space opened the chest at once (no hero clip), items were static (`B047` rewrote XY but the visual had no clip), the chest-open sound (`Container::Interact` Play3D) was never played; only the drop cue was submitted.
- After: matches video and IDA for the behaviours listed under "Observed" below; remaining differences are in "Not verified".

## Root cause
1. Container interaction was fired directly from the press; the hero never entered the Interact state.
2. The item renderer drew the static subtree of itemdrops.bdae; the spawn clip (the arc) was never played.
3. `Container::Interact`'s sound was never submitted (`ContainerInteractResult::sound_id` was only set for destructible hits and nobody played it).

## Change
- `main.cpp`: generic `HeroInteractV1` state. The CharAnimTable "Interact" clips of the player's table (index = interaction type) are registered with the player's retained clip bank next to the skill bank (data from `animations_pyarray.bin`, no path hard-coded). `ctxUseContainer` for an openable container (type >= 0, not 8) plays `interact/<type>` through `CombatSession::play_actor_clip`, locks movement/attack/skills/context button and the locomotion re-select for the clip, fires `Container::Interact` at the clip's authored `interact` marker, plays the container sound at that moment, and returns the hero to idle at the clip end (cancelled on death). A missing clip or table falls back to the previous immediate open (logged).
- `container_runtime_v1.{hpp,cpp}`: `sound_id` is now also reported for accepted opens.
- `source_world_item_drop_render_v1.cpp`: at load the spawn clip is tabulated once per visual (8 ms steps, delta matrix W(t)*W(0)^-1 of the visual's animated node, otherwise mean translation of its animated bone descendants); `prepare` multiplies the item placement by the entry of `age_ms` (the store's own clock).
- Test `tests/item_spawn_clip_tests.cpp` (ctest `item_spawn_clip`): itemdrops clip 0..2566 ms, apex 624 units at 472 ms, back at rest; hero interact clip 1200 ms with marker 467 ms.

## Verification (real EXE, quiet_run, isolated save, hero 100 units from the Swamp tutorial chest, no campaign triggers)
Log (`r2/after/after.log`): `Hero interact start ... eventMs=467 durationMs=1200 frame=260`; `Context container ... accepted frame=289` (29 frames = 0.47 s at 60 fps); `Container sound ordinal=33 uid=162 status=submitted` (sfx_chest_opening.wav exists in the package); `Container opened ... frame=302`; drop cues submitted (`uid=150`/`148`); `Hero interact end frame=330`.
Frames: `r4/H.png` (16 frames, 6 frames apart): the hero changes from idle to the crouch/lunge pose with the sword pointing out, the lid opens right after the marker, items climb well above the chest, tumble and land, the hero returns to idle. Before (`r2/beforeH.png`): hero idle throughout, items static.
ctest: 166/167 (only `session_skill_binding`).

## Observed / inferred / not verified
Observed (video + our frames): hero interaction pose ~1.2 s; lid opens ~0.4-0.5 s after it starts; items rise/tumble/land; label after landing; our run shows the same sequence.
Inferred: the "interact" marker triggers the container (IDA case 13 + marker data, no video proof of the exact frame); gold coin arc approximated; Interact index = interaction type (type 0 -> prince_interaction) from the probe table order.
Not verified: audibility of any cue (WinMM output was silent in tests; voices were started: `Audio voice kind=0` for the open and drops). The user's "no drop sound" could not be reproduced as a missing submission in the v20 EXE: the drop cue was already submitted there (`uid=150 status=submitted`); the chest-open cue was the one genuinely missing. Whether the 2 s drop sample is audible/loud enough in the user's mix is unverified.
Not verified: horizontal travel timing vs the video (the video suggests the items stay in the air ~1 s; ours slide 0.28 s per B047 IDA numbers but are in the air 1.6 s per the clip). Hero facing toward the chest when pressing from the side is not applied (no source evidence of a turn). Gold (skinned) uses one rigid mean lift instead of per-coin bones. Walking approach (B069) then plays the same clip on arrival. Destructible containers (vases, B085) are untouched.

## Package files required
None new (uses `animations/prince_interaction.bdae`, `data/3d/gameobjects/itemdrops.bdae`, `sfx_chest_opening.wav` already in v20).
