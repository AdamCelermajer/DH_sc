# B087 report: no "!" marker over the quest-giver NPC (shot-8)

## Evidence
- Video (P1, observed): t~404-407 s, the Hungry Prisoner on the swamp bridge shows a white/pale-blue "!" billboard floating above his head (green target ring under him) BEFORE the dialogue starts at ~407 s ("Stranger, are you hungry?" -> "New Quest: Kill 8 Bog Moths", t 408-418). Frames: `.local-inputs/claude-v20.1/b087/keep/video-t404-411.jpg`. Inferred: the "!" disappears after the quest is taken (no marker in the following gameplay frames, not checked beyond ~420 s).
- IDA (observed in pseudocode): `Objective_TalkToNPC::InstallObjectiveMarker` 0x47d8e8 / `RemoveObjectiveMarker` 0x47da6c: for a TalkToNPC objective (type 5) it finds the first Character whose `SafeGetCharPropsId == objective oid1`, sets Character+762=1 / +763=(priority==3), grabs an AnimatedFX (`VisualFXManager::GrabAnimFX`) from DesignSettings (+100 QuestMarker, +104 QuestMarkerCompleted, +108 QuestMarkerSecondary, +112 QuestMarkerSecondaryCompleted; Secondary when priority==1, Completed when state>7), attaches it to the Character, shows it and loops its animator. Callers: `Quest::SetState` 0x480c78 (state 2/3 accept objective, 5 objectives, 8/9 end) and `Quest::Compile` 0x480178 (state 6 -> `ObjectiveList::InstallObjectiveMarkers` 0x47a4ec). Remove on completion/leave.
- Data (observed): design-default.json QuestMarker=137, Secondary=138, both Completed=-1. Effects names: 137 `quest_marker`, 138 `quest_marker_blue`. Mesh z 2..568 (carries its own height above the NPC origin).
- Ours before: the Windows build had no draw path at all (only the map icon and the target ring). `quest_marker.bdae` was only used by the map code.

## Match statement
After the fix our behaviour matches the video (a "!" above the NPC while his TalkToNPC accept/objective is pending) and the IDA rule (first NPC with the objective's oid1; accept states 2-3, objective states 5-6, removed when done; Secondary variant for priority 1; Completed variants are -1 => none). Colour: secondary quests use `quest_marker_blue` (pale blue "!"); the video "!" looks white/pale-blue, consistent but not pixel-verified.

## Change
- `features/quest_runtime/quest_npc_marker_v1.{hpp,cpp}` (pure, data-driven selection from the quest table + runtime state), test `quest_npc_marker_v1_tests` (available/active/locked/completed/completed-objective/primary-secondary), CMake hooks.
- `main.cpp`: loads quest_marker(.bdae/_blue), draws one marker mesh over each matching placed NPC (live position, skipped if dead/disabled), log line `Quest marker drawn ...`.
Works for any NPC/quest in the table (Lizman 366, Moths 365 verified in the Swamp).

## Verification
- Observed: real EXE (quiet_run, isolated save), Swamp, hero next to the Lizman NPC: yellow "!" with the primary mesh (blue missing), pale-blue "!" with `quest_marker_blue.bdae` present (`keep/after-*.png`). Build exit 0, ctest all pass except `session_skill_binding` (allowed).
- Not verified: marker bob/rotation animation (static mesh is drawn; the original loops the FX animator), hiding during cutscenes/dialogue, marker vanishing right after talking in the EXE (state logic unit-tested only), end-objective markers (Completed effect is -1 in the data).

## Package files required
`data/3d/interface/quest_marker_blue.bdae` (19,868 bytes; source `.local-inputs/assets-extra/android/data/3d/interface/quest_marker_blue.bdae`). Texture `atlas_fx_particles_002` is shared with quest_marker. Without it the code falls back to the yellow primary marker.
