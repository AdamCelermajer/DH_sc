# B088 report: speech-bubble context button never switches to attack, Space does nothing at the NPC

Branch fix/b088 (from windows-foundation 19613921), build `DH_wt/build-fix088` (exit 0), ctest: only `session_skill_binding` fails (allowed).

## Root cause
Every TalkToNPC quest NPC was registered as a permanent type-3 object-of-interest candidate (main.cpp, P16 SPACEBTN loop; `eligible` defaulted to true). After the quest was accepted (or while it was locked) the NPC stayed the OOI forever: the HUD button kept the speech bubble, a held button never attacks while an OOI exists (source Cmd_UseOOI replaces Cmd_Attack), and Space only re-raised TalkToNPC, which matches no objective, so nothing happened. The original never offers such an NPC.

## Evidence
### Video (Part 1, z_Zky7qQdYs)
Contact sheet of the action button (crop of the bottom-right, 1 fps, t=130-405 s, `.local-inputs/claude-v20.1/fix088/vid/btn130.png`): the button is the sword almost everywhere; the chest icon shows at ~209 s and ~324 s (frame `f324.png`, a chest beside characters and the icon is the chest, not attack). No speech-bubble icon was found in this window, so the video gives NO direct observation of the talk bubble or of bubble -> sword switching. Not observed, not claimed.

### IDA (libDungeonHunter2.so pseudocode-all.c)
- `Character::UpdateObjectOfInterest` 0x3abb9c (line 126080): 500 ms refresh, TargetList mask 89 = 0x59, radius OOI_Distance, `_sortFrontal` 0x38d5b4 (flagged Characters first, then smaller angle; no distance rank). Unchanged, already ported (B004/B029, P16).
- `TargetList::_IsCharacterValid` 0x4a1ab8 (line 300280): review corrected virtual+52 to `IsDead`0x3a2ed4, not `IsSummoned`0x3a30ac. Mask0x01 admits living enemies (AI_IsEnemy); mask0x08 is the dead-friendly branch with additional state exclusions; 0x10 merchant and **0x40 = `Character+762`**. A friendly living NPC passes through merchant or +762. This evidence correction does not introduce a new selection filter.
- `Objective_TalkToNPC::Register` 0x47db34 (line 270861) sets `Character+762 = 1` on the NPC whose CharacterTable row equals the objective oid1; `Unregister` 0x47dbc4 clears it. `InstallObjectiveMarker` 0x47d8e8 / `RemoveObjectiveMarker` 0x47da6c drive the "!" billboard on the same NPC (B087 uses the same predicate).
- `Quest::SetState` 0x480c78 (line 273104): the accept objective is registered in state 3 Available (unregistered on leaving it), the objective list in state 6 Active (`ObjectiveList::Register`, `Unregister` when leaving 6), the end objective in state 9 Completed.
- `Character::GetInteractionType` 0x3a47e8: enemy -> 8, friendly NPC -> 3. `Character::Interact` 0x3a4d78: raises the TalkToNPC quest event (and Character event 5 unless +762 is 0), merchants/cleaners open their menu. `HUDControls::Update` 0x41a780: OOI != 0 -> Cmd_UseOOI else Cmd_Attack(null). `Character::UseOOI` 0x3ad614 gate: no current attack target and idle/moving.

### Match statement
- IDA: our new behaviour matches (NPC is an OOI candidate only while a TalkToNPC objective of it is registered; enemy priority stays the source frontal-angle queue).
- Video: not verified (no bubble icon observed). The "monster nearby -> attack" priority is the source rule (Characters sorted by angle). With a registered quest NPC straight ahead and a monster at the side, the NPC wins in the original too (inferred from the sort, not observed live).

## Change (small, rebase-friendly; context_button_v1.* untouched, so no clash with B085)
- `features/quest_runtime/quest_runtime_v1.{hpp,cpp}`: `QuestRuntimeV1::talk_objective_registered(npc_row)` (state/slot rule above; completed objectives are not registered).
- `main.cpp`: one line in the NPC registry loop: `entry.eligible=questRuntime->talk_objective_registered(row)`.
- Tests: `quest_runtime_v1_tests` (registered at start for NPC 365/366, unregistered after accept, unknown NPC never), `object_of_interest_owner_v1_tests` (unregistered NPC does not shadow an enemy; registered frontal NPC keeps priority; lone unregistered NPC -> no OOI). Both pass.

## Verification (real EXE, quiet, silent, isolated save; scratch `.local-inputs/claude-v20.1/fix088/base/`)
Scenario: fresh Swamp, hero 100 units from the Mothgiver NPC (row 365), Space at frame 100 (accept), second Space at frame 600, 1000 frames, no campaign triggers.
- Before (v20 EXE): `Action icon frame=0 icon=3 type=3`, talk accepted at 100 (`NEW QUEST row=53 'Kill 8 Bog Moths.'`), icon NEVER changes; second press at 600 logs `Context talk npc row=365 ... use=1` again (does nothing). Frame `zold/cap.png`: speech bubble at frame 1000.
- After (fix088): same accept at 100, then `Action icon frame=120 icon=5 type=-1` (attack, after the 500 ms OOI refresh) and the second press no longer talks. Frame `znew/cap.png`: sword.

## Not verified / gaps
- Monster + NPC together in the live EXE: no monster could be brought within OOI range of an NPC in a headless run (scripted movement did not move the hero in this harness; moths did not approach). Priority is covered by the unit test only.
- Talk -> dialogue: the original starts dialogue from the quest/accept scripts (Quest::SetState Exec scripts) and Character event 5, not from the NPC itself. With campaign triggers on, the quest runtime starts those scripts; I could not stage that in the EXE (the Swamp_Intro script teleports the hero away from the NPC). So "talking starts the dialog" is not verified here; without scripts the observable result is the NEW QUEST banner.
- Whether a completed objective stays registered in the original (`Objective+8` flag meaning) is not decoded; completed ones are treated as unregistered.
- NPC position for the OOI is the static placement (not a moved/following NPC); no walk-to-NPC approach before talking (B069 note stands).
- Quest NPCs other than TalkToNPC oids (e.g. merchants, Rene/priest) are still not candidates; merchant bit 0x10 not implemented.

## Package files required
None.
