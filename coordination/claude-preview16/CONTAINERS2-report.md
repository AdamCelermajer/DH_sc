# CONTAINERS2 report (Preview 16, branch p16/containers2, from p16/containers e2f97dcb)

Status: T4, T5, T6 implemented and verified in the real EXE (quiet runs). Gaps listed below are real and open.
ctest: green except `session_skill_binding` (allowed worktree exception).

## Scope (wave 2)
- T4 rewards: open/break call the original DropLootTable through the existing DROPS store (RuntimeWorldItemAdapterV1), loot row from the container declaration.
- T5 persistence: open/broken state in the existing OBJS v1 component (no schema bump), restored on load.
- T6 Summon + OnOpen script contract: generic data-driven verb, Summon through the p16/spawn owner (spawn_character_v1).

## Investigation (evidence)
- IDA (survey, wave 1): Container::Interact 0x3a0b38, DoOpen 0x3a0a98 = DropLootTable(row.loot, opener, -1, false) then Lua OnOpen; DestructibleContainer::Interact 0x3a0da0 (hit counter, stage clip, destroy at zero then base Interact).
- Lua OnOpen bodies read from `.local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts/objects/*.luac` (plain Lua text): moth_spawn_container (GetRand(0,100) < 25 -> Summon(Swamp_Moth_Minions, spawn=true, 0,0,0, relative)), plant_spawn_container (outer 25% test commented out; always summons Swamp_Moth_Minions after three draws), plant_spawn_container2 (25% gate, 50% branch, 20% variant: EarthTemple_*), zombie_spawn_container (25% InfectedBurned), container.luac (DealDamages only, no summon).
- Swamp declarations (live listing): 14 containers, 5 openable (interaction 0), 9 destructible (interaction 8). Barrels with script `moth_spawn_container`: 8 of 9; one barrel has no script. All barrel clip libraries have 3 clips (see gaps).
- p16/spawn (commit f4bec88f) is in the main repo history; merged into this branch (17655053, no conflicts beyond main.cpp hunks, both kept).

## Implementation
- `features/containers/container_loot_v1.{hpp,cpp}` (T4): ContainerLootV1 composes the existing SourceContainerLootV1 kernel (receipts keyed by declaration + lifecycle, so a second open publishes nothing), the session loot RNG loan (PlayableActorWorld::with_loot_random), the death rewards' LootEntryServices (new accessor RuntimeSessionDeathRewardsV1::loot_entry_services), and RuntimeWorldItemAdapterV1::publish_source_object_drop. GoldStack rows are priced from the opener's resolved[195] bonus, mirroring the death path (no bonus: RNG draw consumed, item skipped and counted).
- `features/containers/container_world_v1.{hpp,cpp}` (T5): each declaration is bound as a neutral WorldObject with its authored stable ID and the OBJS component (visible, enabled, idle). Changed declarations are persisted each frame (state394 in OBJS; remaining destructible hits in `dh2.container.hits.v1`, u32). After a GameSave load, state is restored from the objects (no rewards or events replayed).
- `features/containers/container_open_script_v1.{hpp,cpp}` (T6): the OnOpen bodies as data (decision tree of GetRand draws, `slot < percent` tests, Summon steps), one contract per script name; one generic interpreter with injected GetRand and Summon services. A refused Summon is a normal outcome (script continues, reason logged).
- `features/containers/container_runtime_v1.{hpp,cpp}` (T3 extension): destructible hits. Stage clips = BDAE clip library (decode_embedded_scene_clips), stages = clips - 3 (only above 3), each hit plays clip[stages - remaining] (status `hit`), the final hit starts `activate`. Without an authored 'opened' marker, DoOpen falls back to the clip end. Without an activate clip, the open is immediate (source no-visual branch). New: take_changed(), restore(), instance()/state()/hits_remaining() accessors.
- CMake: `foundation_container_drops` (loot, script, world, source loot, OBJS codec) and the test `container_open_script_v1_tests`.
- main.cpp hunks (all marked `P16 CONTAINERS2`): includes; `containerLoot` declaration; load-time declaration listing (`Container declaration=...` with interaction, position, script); spawn pool reservation also for contract Summon targets of this level's containers; world-object binding in initializeCombat; `bindContainerLoot` (called from every bindDeathRewards site); `makeSpawnServices` (shared by --spawn-test and container Summon); `runContainerOpen` (DoOpen loot, then OnOpen); persistence after the container update; restore after F9 and after the R-reload live snapshot.

## Isolated tests
- `container_open_script_v1_tests`: PASS (`moth=25-strict plant=3draws plant2=gated zombie=25 refused=continues`). Covers strict `<`, draw counts and order, plant2 gate and branches, refused-summon continuation, RNG failure abort, summon target set (6 characters).
- ctest: 115/116 pass; `session_skill_binding` fails (allowed). `container_open_script_v1` needs the llvm-mingw bin on PATH under ctest (environment; passes with PATH set).

## Integrated runtime verification (quiet runs, EXE built from this branch, `.local-inputs/p16-runs/c2..c7`)
Verified:
- Chest open (`_prim_OpenableContainer_2`, interact at frame 20): opened frame 35, loot 227 selected=1 delivered=1 status=ok, item published to the DROPS store (`World item draws ... count=1`, Bow02 at the chest position), drop-sound submitted to no-audio runtime (expected in quiet mode).
- Chest 1 (`_prim_OpenableContainer_1`, at 1826.65,1796.04): same path, loot 227, item at the chest position.
- Duplicate: second interact after open -> `rejected_state`, no second drop (dup, moth20).
- Out of range: 600 units -> `out_of_range`, no state change (range).
- Barrel break (`_prim_DestructibleContainer`, loot 9): first interact accepted -> state 3, opened frame 37, loot 9 selected=1 delivered=1 (GoldStack01 priced, gold_unpriced=0), further interacts `rejected_state` while activating.
- Moth barrel (`..._05_321_364_22_41_21_187_21_46`, script moth_spawn_container): OnOpen runs after the loot; one GetRand(0,100) per open; across 8 combat seeds the rolls were 86, 90, 9, 1, 26, 16, 70, 47. Rolls 9, 1, 16 (< 25) call Summon -> `Summon character=Swamp_Moth_Minions spawn=1 outcome=refused reason=profile not admitted: ... no actor profile or combat policy`; others do not summon. The first draw is the same for every run with the same seed (deterministic stream), so the variants only differ with the seed.
- Persistence (chest, `persist`): interact A at 20, F5 save at 60, interact B at 80, F9 load at 120 -> A restored `state=4` (rejected_state at 130). Chest B was not opened in that run (its interact at 80 was out of range from the test position), so the "B stays closed" case is NOT verified.
- Persistence (barrel, `bpersist`): broken barrel saved at 150 while still in `activate` (state 3), loaded at 180 -> restored state 4, re-interact `rejected_state` at 200; no second loot.
- Declaration listing: 14 containers, all bound as world objects, `notices=0`.

## Gaps (open, honest)
- Destructible hit path (stages > 0) is implemented but NOT exercised live. On Swamp every barrel clip library has 3 entries, so stages = 0 and the barrel breaks on the first interaction. The source's slot count (vtable+16 on the visual) may not equal the clip library count; unverified.
- Destructible hit and break sound are not played (the container path returns sound_id; no 3D sound binding in the container runtime).
- Attack-to-container hits (combat routing into DestructibleContainer::Interact) and the Space context-button dispatcher are not wired; `--interact-at` is the debug trigger.
- Walk-over pickup is the existing DROPS automatic path (PickUpType 0 items only). Not verified live: no automatic item was picked up in these runs.
- Summon of `Swamp_Moth_Minions` is refused in live (no actor profile; deriving one from AnimTable 44 is not implemented). Plant/zombie variants: their Summon targets (EarthTemple_*, InfectedBurned) are not declared on Swamp, so they were not exercised live. The live spawn itself is blocked by the physical-transition gap recorded in SPAWN-report section 5 (AI on: no PreSpawn17 -> Spawn1 -> Idle3). No spawned actor was observed.
- GetRand uses the combat world's RNG stream (PlayableActorWorld::random_uniform). Whether the source Lua GetRand shares that stream is not verified.
- Restore edge: a save taken during `activate` before the 'opened' marker restores as completed without the loot (loot not given). The save taken after the marker is correct (verified).
- OBJS has no destructible-hit field, so the hit count lives in `dh2.container.hits.v1` (feature-owned component, not OBJS). Documented deviation for T5.
- Behaviour change: container objects are now neutral world objects of the session. `combatSession->world()->objects().empty()` (skill cast target facts, main.cpp) is now false on levels with containers, which matches the source (containers are attackable objects) but changes that flag on Swamp. Not regression-tested beyond ctest and the quiet runs.
- Logging: `Container OnOpen GetRand(0,100)=` is printed per roll (diagnostic; keep or drop later).

## Placeholders
- None added (no art or HUD). The destructible hit and break sound is simply not played (see gaps).

## Package files required
- None new.

## Verifier script
- Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16containers2 [-Test]`
- Jobs: `node .local-inputs/p16-gen-jobs-c2.js <ABSOLUTE outdir> "name=x,y,z@fx,fy,fz!--interact-at,DECL@20,--save-frame,60,..." ...` (FRAMES env sets frame count; `~` in an extra arg becomes a comma), then `quiet_run.ps1 -JobsFile <outdir>/jobs.json -Parallel N -Summary <outdir>/summary.json`.
- Job sets: `.local-inputs/p16-runs/c2` (chest, barrel, persist, range, dup), `c3` (chest1, barrel3, bpersist), `c4` (moth barrel frames 20/24/31/45), `c5` (moth variants 20..50), `c7` (moth combat-seed 1..8; the decisive OnOpen/Summon run).
