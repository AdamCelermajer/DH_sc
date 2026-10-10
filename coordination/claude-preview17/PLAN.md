# Preview 17 plan (draft, user accepted the P16/P17 split on 2026-10-10)

Goal: finish Act 1 end to end and open the first later-act levels. Everything below is a GENERAL, data-driven, portable system; Act 1 is the proving ground.

## Core systems (in this order)
1. **Condition evaluator + world state.** One evaluator for authored conditions (`IsAfter_Swamp_KillWitch2`, `IsBefore_*`, quest state, boss kills, flags), fed by the quest/kill event bus. Used by zones, doors, spawns, objects, scripts. Fixes the 255 "condition unknown" actors. Persisted in the save.
2. **Animated world objects.** `AnimatedDecor`, `Door`, `Script_OpenDoor`/`CloseDoor` bound live (e.g. `open_SwampKingDoor` -> `_prim_SwampDoor_FIRE`, waterfall active until the witch dies). Condition flip fires the animation; saved state restored on load. Builds on `features/progression_barriers/source_door_commands`.
3. **Level transition manager.** `TriggerZoneExitLevel`, `QuestMoveInZone`, `CheckpointZone`; target level + entry point; save/checkpoint, loading screen, unload/load, state carry-over, difficulty/act rows (schema v4). Covers Swamp -> witch cave, Swamp -> Darkwood, and quest-gated exits.
4. **Procedural level generator** (37 of 76 levels use `.rule.xml`; the witch cave is one). Canonical port already has it.

## Also in P17
- Finish Act 1 end to end; first Act 2/3 levels; I021 and I022 from the tracker.
- Leftovers from P16: cinematic runner, container rewards/summon, quest tab/banners, SKIP/dialogue/tutorial prompts, spawn engagement/death/despawn, moth profile.

## Rules
Same standing rules: IDA first, video fidelity, no Swamp-specific scans, placeholders labelled and reported, quiet hidden runs, Haiku high fresh agents.
