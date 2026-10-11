# B071 report: scripted (FakeMoth) moths untargetable / stuck in Spawn pose

Branch fix/b071, build `fix071`. Scratch: `.local-inputs/claude-preview161/fix071/` (args `run/*.args`, logs, `run/cmp.png`).

## Evidence
- Screenshot b071: two moths with wings spread flat next to the player, no target ring/name plate.
- Video (Part 1, 421-426 s, 3-4 fps crops): Bog Moths are small, dark, wings folded; each shows the red target ring and the
  "Bog Moth" plate, takes hits (green puff, damage numbers) and dies ("+4 EXP"). Direct observation. Same model/clips as ours
  (derived Swamp_Moth_Minions = same model and clips as Swamp_Moth_Type1); video lighting is darker.
- IDA: `Script_PutCharacterInIdle::SetState` 0x45971c -> `CharStateMachine::SM_SetIdleState` 0x3c1a00 -> `_SetState(3)`: a full
  state-machine change (gate: not Limbus, not awaiting spawn). Campaign `MothIntro` (zone `_prim_MothMovieTriggerZone`): SpawnCharacter
  FakeMoth_01/02, PlayActorAnim 332/333 (CS_moth_*_intro), PlayCamera 334, PutCharacterInIdle x2, LookActor LocalPlayer, Unlock.
- The barrel summon (Swamp_Moth_Minions, moth_spawn_container) was tested in the real EXE too: targetable and killable
  (`Damage ... dead=1`), so it is not the failing path.

## Root cause
`hostServices.set_actor_state` (main.cpp) only called `CombatSession::set_actor_original_state`. For lifecycle actors the state
machine lives in `actorLifecycle`; PutCharacterInIdle changed the combat fact but the lifecycle record stayed state 1 (Spawn),
physical=0. `combat_enabled` (state 3 + physical + collisions) stayed false, so the permission provider kept them untargetable and
in the Spawn pose. Before: `Lifecycle final ... FakeMoth_01 state=1 enabled=1 physical=0`.

## Fix
main.cpp, one hunk marked `P161 B071`: for an actor with a lifecycle record, state 3 -> `put_idle`, 1 -> `spawn`, 0 -> `put_limbus`;
others keep the old path.

## Verification (real EXE, quiet runs)
Recipe: swamp.args, start -4900,0, `--campaign-skip-frame 40 --caption-auto-tap-ms 200 --move-segment 150:1130:1,0`, attack from 1160.
- Preview 16 EXE (`run/p16base.log`): FakeMoths end `state=1 physical=0`; no hit on them.
- fix071 (`run/h1800.log`): `previous=1 state=3`, player damages both moths (frames 1195-1334, `dead=1`), then Despawn -> Limbus.
  `run/cmp.png`: left before (no target), middle after (Bog Moth plate, ring, damage numbers).
- ctest: all pass except `session_skill_binding` (allowed).

## Not done / unknowns
- No unit test (fix is a host binding in main.cpp; verified end to end instead).
- "Follow the player" and "animation wrong" are explained by the stuck Spawn state; inferred, not measured separately.
- CS_moth_*_intro entrance animation not compared frame by frame with the video (scene not located in the video).

## Package files required
None.
