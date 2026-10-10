# Preview 16 plan: "the living world" (one big release) - user decisions 2026-10-10

## Principle (user)
Act 1 is mainly a TECHNICAL milestone. Do the most work possible now so the rest of the acts are extremely easy and quick. That means: build EVERY general system, data-driven and compatible with all maps, acts and levels; Act 1 is the proving ground, not the target. No Swamp-specific code, no per-map lists, no hard-coded names/positions; everything comes from the level data and original tables through registries. Unknown classes/commands are logged once and reported, never silently ignored. Portable to Linux and Android (platform code only in the platform layer).

## Scope (order of work)
1. Shared groundwork: script-host providers for the campaign executor (3 of ~15 bound today: HUD hide, SKIP, control lock, dialogue, tutorial, block-save, camera...), a generic "spawn character from template at position with spawn clip, admit to lifecycle" function, the Space context button (attack by default; chest/NPC/other interactions via the object-of-interest interaction types; press edge only; enemy priority as decoded from IDA), trigger-zone construction from authored declarations (RoomZone/trigger prims) for any level.
2. General container loader (Openable 0 / Destructible 8 / traps/triggers/shrines through a class registry) + animations + rewards via drops + persistence + moth/plant/zombie summon via the Lua OnOpen (generic `Summon`).
3. Quest runtime (thin Windows runtime on schema v4 CQPG v2: accept/progress/complete/reward, kill/area/talk events, journal, banners, tracker) + Quest Log tab (Part 2 t=523: Assigned/Completed lists, SIDE QUEST tag, MAKE ACTIVE) + NPC dialogue/merchant services hooks.
4. Cinematic runner (BeginScriptedCutScene/EndScriptedCutScene contract, camera clips, actor clips, HUD hide, SKIP) -> LizardMan_Intro ambush + CombatTuto, Movement_Tuto, chest_tuto, Swamp opening.
5. Map page (+ visited rooms in the save; minimap camera files are now staged) - no HUD minimap (original has none in play).
6. Generality gate (acceptance): a SECOND level from the Android device folder (e.g. 002 swamp witchcave and one later-act scene such as darkwood) must load and instantiate its containers, triggers, NPC/spawn declarations and scripts through the same loaders WITHOUT code changes; unsupported classes appear in one log list. Document the coverage per level.

## Inputs now available
- iPad 1.0.0 assets (assets-extra/ios: level_up.bdae, drop/pickup/StaticBall/Lizard sounds, minimap cameras, 14 VXN banks) staged in package rc3; Android device folder (assets-extra/android + C:/Users/adamc/Downloads/dungeonhunter2/...): 98 cutscene camera clips (cs_*), 38 later-act scenes, characters/bosses.
- Surveys: coordination/claude-preview15/{CINE,CONTAINERS}-survey.md, coordination/claude-preview14/{QUESTS,MAP}-survey.md.

## Process (unchanged)
Fresh subagent per task in its own worktree (p16/<name>), quiet hidden parallel verification, tracker rows with IDs, branch merges by the root, release packaged only after an independent verifier approves the frozen EXE.
