# B093 quest system (kill tracking / MAKE ACTIVE / completion) - PARTIAL

## Observed (real EXE, quiet_run, v20.1 build, fresh isolated save; scratch .local-inputs/claude-v20.1/fix093/run)
- Quest accept (debug aid) -> NEW QUEST banner "Kill 8 Bog Moths."; 8 kill events (template 94) -> QUEST COMPLETED banner, "20 EXP / 150 GOLD" drawn (a4.log, cap4/f300.png).
- LIVE kills of Swamp_Moth_Type1 / _RESPAWN by the player (a2.log, auto-attack) raise `Quest event kind=0 template=94 applied=1` per kill: the live kill producer works for authored moths (6 kills seen). Spawn-pool moths (chest minions, Swamp_Moth_Minions, template -1) do NOT count - consistent with the original (GameObject::_Summon sets suppress_quest +5348; Character::Kill 0x3a.. skips the quest events then).
- Journal: MAKE ACTIVE released via scripted --menu-release (cap3/sheet.png) selects/activates rows; current-quest dot moves from Bogwalker's Delight 2 to Prison Break on row click, journal shows Assigned/COMPLETED as in shot-12.
- Map page showed NO marker for the kill objective before this fix (only MoveInZone objectives); now shows (required-killed) objective crosses at the closest live moths (cap5/f45.png, 5 markers).

## Video / IDA match
- Video Part 1 ~t=761-764: QUEST COMPLETED panel "Kill 8 Bog Moths / 20 EXP 150 GOLD" (contact sheet 700-770 s, last row first cell) - ours matches. No HUD kill counter is visible in the sampled frames around the moth fights (t~730-760): no counter shown in the original; the original tracks progress only in the map (green objective crosses). IDA: ObjectiveTemplate_KillCharacter::GetPositions 0x47ca18/0x47cb1c, MenuCharMenu_Map::ShowObjectivesIcons 0x4548ac, Character::Kill (event fan-out for every non-player, non-summoned death, regardless of killer), Quest::SetState 0x480c78.

## Changes
1. map_markers_v1: map_kill_objective_positions_v1 + test; main.cpp feeds it for the current quest's kill objectives (type 0/10).
2. main.cpp raiseQuestKills: any non-player, non-summoned death counts (original Character::Kill is killer-independent), previously only player-attributed kills (missed ally/companion/DOT/skill kills).
3. DH_QUEST_TRACE env diagnostics (kill identity/attacker).

## Not verified / gaps
- I could not reproduce "8 moths killed, nothing happens" with authored moths; likely causes (inferred): kills by non-player sources (fixed), chest minions not counting (original behaviour), or quest accepted after kills (Mothgiver talk required: Swamp_Moths accepts via TalkToNPC 365). Not confirmed against the user's run.
- Real mouse click on MAKE ACTIVE in a visible window not exercised beyond the scripted release.
- Quest-save persistence across resume checked only partially (a6 runs: current=53 restored).
- ctest not run in this session.

## Resumed evidence audit (2026-10-11; before further implementation)

Visual evidence re-inspected: Part 1 frame sequence 404–418 s (1 fps, scratch `resume/ref-accept-404-418.png`) shows the Hungry Prisoner exclamation/green interaction ring, dialogue caption, then NEW QUEST / Kill 8 Bog Moths at 415–416 s. Part 1 761–764 s (3 fps, `ref-complete-761-764.png`) shows one QUEST COMPLETED banner, 20 EXP / 150 GOLD, banner dismissal and continuing gameplay. Neither sequence shows a HUD kill counter. Part 2 520–529 s (`ref-journal-map-520-529.png`) shows selected Buried Alive then Law and Order, MAKE ACTIVE, the current dot moving to Law and Order, then objective crosses on the map. This supports active selection and map updates; footage does not prove save persistence or every admission branch.

Logic evidence: IDA `Character::Kill` 0x3a5b18 starts with virtual +52 IsDead rejection, sets byte5193 dead, and reaches quest fanout independently of attacker identity. The forced/script kill argument a3 rejects its later fanout; virtual +84 remotely-updated and byte5348 suppress_quest reject it too. `GameObject::_Summon` sets byte5348 (export lines108408–108418); this is a distinct predicate from IsSummoned. Property events always carry victim property ID; template events require victim template ID != -1 (122310–122383). Ordinary combat DamageEvent target_died is the already-admitted transition. `GetPositions` 0x47ca18/0x47cb1c allocates required-minus-saved quantity, seeds player position, scans TargetList matches by property/template and rejects virtual +52 IsDead only (269972–270101); it does not reject summon byte5348 or HP0. Current main instead uses actor.alive() (HP), a fidelity defect. `Quest::SetState` 0x480c78 case6 registers objectives and sets current quest; leaving6 unregisters; case7 clears current; case12 queues completion/rewards through the state transition. `NativeSetCurrentQuest` 0x43d3b4 forwards selected quest to SG_SetCurrentQuest, not quest acceptance. Existing recovered thin runtime locations: quest_runtime_v1.cpp event state windows and set_state; runtime_quest_menu_v1.cpp release/activate; main.cpp raiseQuestKills and provider reload.

Expected implementation: preserve authored NPC/level acceptance, count one admitted non-player victim death regardless of attacking actor, preserve summon suppression, update current quest/map from ordinary pointer release and keep it in CQPG save. Use source IsDead for map candidate eligibility. No new HUD counter or map-specific quest rules.

Uncertainty: user's original v20 eight-moth failure remains not reproduced; existing six natural kills and eight synthetic bus events are distinct evidence. NPC dialogue dispatch is separately owned by B110; existing talk path raises the quest event but cannot yet establish the original dialogue sequence. Our source assets are v1.0.2, reference video v1.0.3. Silent hidden-window checks cannot verify audio.

Focused test plan before coding: authored talk event rejects wrong NPC/level and ignores kills before acceptance; seven matches stay active, eighth closes/rewards once, duplicate and save/reload do not repay. Test admission helper rejects failed/nondeath/player/suppressed victim and admits any attacker without needing attacker identity. Run real 10ms mouse releases selecting row then MAKE ACTIVE, inspect current dot/map and isolated save/reload. Run an isolated Space NPC route with compiled caption autotap, explicitly qualifying any post-opening seed. Natural eight authored moth kills remain required for full gameplay completion; scripted kill/event fixtures alone cannot close that gap.
