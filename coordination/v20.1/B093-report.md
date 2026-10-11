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
