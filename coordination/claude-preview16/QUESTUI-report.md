# Preview 16 QUESTUI report (live quest events, banners, Quest Log tab)

Branch `p16/questui` (worktree `DH_wt/p16questui`), from `p16/quests` (wave-1 runtime, report `QUESTS-report.md`). Never pushed.
Commits (this wave): `568db909` zones, `42abc40a` banners, `0091d756` kill/talk/pickup, `0d07981e` Quest Log tab (partial), `c4ef4197` hit-zone restore, `c3560c22` exporter guard.

## Status (short)
- DONE and verified in the EXE: live zone entry (accepts Swamp_KillWitch), live moth kills (template 94 raises the Moths kill objective, two kills confirmed), live NPC talk (Lizman NPC, row 366, accepts Swamp_KillFiveLizman), NEW QUEST / objective counter / QUEST COMPLETED banners drawn (placeholder frame), Quest Log tab selected and rendered from the live CQPG (5th tab highlighted, Assigned row shown).
- PARTIAL: Quest Log (Completed rows, detail text, SIDE QUEST tag, MAKE ACTIVE click are NOT verified); objective sentence text in banners (not decoded); NPC talk has no dialogue display.
- ctest: 115/116 pass; only `session_skill_binding` fails (worktree junction, known). One intermittent failure of `schema_v4` was seen during a parallel run and passed on every rerun (standalone and full runs).

## Investigation evidence (AGENTS.md items 1-4)
- Banners (IDA `pseudocode-all.c` ~273190-273260, `Quest::SetState`): case 6 (Active) `DialogMsg(GLOBAL_QUEST_NEW, DialogStyles.QuestMsgDialog)` plus `Character::SG_SetCurrentQuest`; case 12 (Closed) `DialogMsg(reward string, QuestCompletedMsgDialog)`. Headings are the original strings "New Quest" / "Quest Completed" (global.english) and "Reward" / "EXP" / "GOLD" (gameplaymenus.english). Style art lives in `dqhud_droid.swf` (style names present; the style-to-movie mapping is in ActionScript, not decoded).
- Quest text fields (IDA `Quest::GetTitle` 0x47f9dc uses definition offset 4, `GetPreDescription` offset 8, post check offset 12, `GetObjectiveDescription` 0x480a84 reads offset 16 and falls back to `ObjectiveList::GetDesc` when the id is the none sentinel 1835016). In the table `text_fields` = [title, pre, area/location, none]. `text_fields[2]` resolves to a LOCATION name ("The Boglands - North Wilderness", same in every pack). The objective sentence ("Defeat the Bogwitch in the Northeast Cavern.", "Kill 8 Bog Moths") is built by `ObjectiveList::GetDesc` through each objective template's virtual GetDescription; not decoded. NOTE for the owner of `features/quests/quest_text_resolver_v1.cpp`: its objective index (text_fields[2]) and post-check index (text_fields[3]) disagree with the IDA offsets (objective = [2] in the resolver vs area in the table; IDA post check = offset 12 = [2]). Not changed here.
- Kill producer: `Character::Kill` raises the four kill constants; the table uses template ids (type 10). Live template 94 = Swamp_Moths (confirmed by the EXE kill event).
- NPC identity: TalkToNPC oids are CharacterTable rows. `character_properties_pyarraynames.bin` (u32 count, then u32 length + bytes per entry, index = row) gives Swamp_MerchantCamp_NPC_B = 365 (Moths oid1; the Mothgiver declaration in `obj_2of4_brdwalk_sw_00.mgp` has `charpropsname="Swamp_MerchantCamp_NPC_B"`) and NPC_C = 366 (Lizman oid1; `_prim_NPC_LizBlood`). The population loader leaves explicit `charpropsname` NPCs with `source_character_cache=-1`; the quest bind resolves them by name.
- Interact radius: `CharacterDesign.OOI_Distance` = 200 (wave-1 CONTEXT survey, `Character::UpdateObjectOfInterest` 0x3abb9c).
- Zone: the Witch zone is `QuestMoveInZone` `_prim_WitchQuestStart` (merchantcamp_ruins_swe_00.mgp), world box centre about (-11822, 12246, 255), half extent 100 x scale (730.955, 570.48, 202.567). `activate_cond` ("IsBefore_Swamp_KillWitch") is not evaluated; the runtime's own state gate applies.
- Quest Log tab: `dqcharmenu_droid.swf` has `btnQuestLogTab` (label LogMap in export_art.py) and `btnMapTab`. Reference frames (survey, kept): `.local-inputs/claude-preview14/QUESTS/p1/quad-p1-newquest.png` (NEW QUEST panel, t~659), `.../p1/f761.png` (QUEST COMPLETED, t~761), `.../p2/f523.png` (Quest Journal, t=523).

## Implementation
1. Table path: `v2quests_pyarray.bin` / `v2quests_pyarraynames.bin` read via the normal asset root (`assets.read("original-cache/data/pydata/...")`); a missing file reports `Quest table diagnostic` (unchanged from wave 1). Listed below under Package files required.
2. Zones (`features/quest_runtime/quest_zones_v1.*`): zone names from every MoveInZone objective (str2 of accept and objectives); boxes from ANY loaded level's Block declarations (box rule 100 x |scale| about the placement translation, rotation reported and not built); rising-edge `zone_enter` with the level row. Unknown level zones are simply absent. main.cpp: build at quest bind (`Quest zones built=... [box]`), per-frame entry from the player transform (`Quest zone entered zone=... level=41`).
3. Kills: wave-1 producer kept (`raiseQuestKills`); verified live (Moths, template 94, two kills, `applied=1`).
4. NPC talk (`talkNearestNpc` in main.cpp): on the interact press edge (`--quest-talk-frame` is a scripted press, test aid only), the nearest TalkToNPC NPC (population placement) within 200 units raises `talk_to_npc{oid=row, oid2=level row}`. Approximation of `Character::Interact` / OOI (no dialogue shown, no OOI ranking).
5. Item pickup: `runWorldItemPickup` success raises `item_pickup` (no Act 1 objective consumes pickups yet; GatherLoot/PickedUpLiftable are not in the Act 1 rows).
6. Banners: runtime `QuestBannerV1::Kind::updated` (objective, quantity, required) on counted progress that does not complete; presenter `features/quests/quest_banner_presenter_v1.*` (layout, timing 3 s new / 2 s counter / 4.5 s completed with 0.5 s fade, a counter replaces its row's queued counter, a completion drops them). Drawn in main's overlay pass (`drawQuestBanner`) with the original Fontin glyphs (`drawScreenLabel`). Headings are the original English strings.
7. Quest Log tab: `Tab::quest` / `Action::quest`; `export_art.py` gained the quest variant (art4 = tab chrome with btnQuestLogTab highlighted) and a fixed hit-style stub; the committed `original_art.cpp` art0..art3 are kept verbatim, art4 is spliced in, and the quest hit zone = the faery zone shifted by the 51.9 tab pitch (the four existing zones are spaced exactly 51.9). The composition registers the quest page (`bind_source_quest_menu_page_provider_v1`, same owner as the other tabs), with the quest text resolver on the shared StringManager and the page refreshed from the CQPG each frame while the tab is open. The runtime reloads the CQPG after a menu release so MAKE ACTIVE changes are not overwritten by the next save.
8. CQPG v2 counters: the runtime's save path is unchanged (schema v4, `schema_v4` test covers CQPG v2 counters).

## Tests
- `quest_runtime_v1` (real table): zone builder (names, boxes, rising edge, rotated reported), Moths counter banner (`7 of 8`), completed banner rewards, all wave-1 checks.
- `quest_banner_presenter_v1` (new): NEW QUEST heading, completed lines (rewards, zero gold hidden), counter text, counter replacement per row, completion drops the row's counters, timing and fade.
- Full ctest: 115/116 (only `session_skill_binding`).

## Integrated runtime verification (EXE, quiet runs, hidden desktop; logs in `.local-inputs/claude-preview16/quests-run/out/`)
- `zone-walk` / `s-north.log`, `n-south.log`: `Quest zone entered zone=_prim_WitchQuestStart level=41` then `Quest banner kind=NEW QUEST row=52`.
- `moth-banner.log` (Moths accepted by debug aid at frame 60, 8 kills at frame 100 via the debug aid): NEW QUEST row 53, 7 x QUEST UPDATED, QUEST COMPLETED `xp=20 gold=150`. Capture `moth-banner.png` (frame 420): QUEST COMPLETED panel with Reward, 20 EXP, 150 GOLD.
- `moth-live.log`: live attacks from the player; moth `5657465463426793761` died at frame 108 and `11110320080226071524` at frame 130; `Quest event kind=0 property=371/372 template=94 applied=1` (live kill path verified).
- `talk-lizman.log`: `Quest talk npc row=366 distance=120.16 level=41`, `Quest event kind=2 ... applied=1`, `Quest banner kind=NEW QUEST row=51`.
- `quest-page.log` / `quest-page.png` (frame 150, `--quest-page-frame`): Quest Journal header, 5th tab of six highlighted, Assigned row "Prison Break" (Swamp_Escape, active, selected), COMPLETED header. No refresh/select diagnostics after the fixes.

## Gaps (explicit, not done)
1. Quest Log: Completed rows are NOT drawn (`RuntimeQuestMenuV1` builds a single-category frame; the Completed clip needs a two-category snapshot). Detail pane text, the SIDE QUEST tag and the MAKE ACTIVE button are not visible in the captured frame (description text not resolved, see below). The MAKE ACTIVE click and the tab click route are not exercised in the EXE (select by test aid only).
2. Banner objective sentence: `ObjectiveList::GetDesc` / objective GetDescription not decoded, so NEW QUEST shows heading only and the counter line shows "n / m" without the objective sentence; QUEST COMPLETED shows no objective line.
3. Banner art: the dqhud QuestMsgDialog / QuestCompletedMsgDialog frame (ornate frame) is not exported; the panel is the placeholder (see Placeholders).
4. NPC talk: no dialogue is shown (DialogMsg / dialogue owner not in this build); the talk is an interact-press approximation within 200 units, not the OOI ranking of `Character::UpdateObjectOfInterest`.
5. Scripts: quest scripts (`SwampScripts.*`, e.g. the Mothgiver or Witch scripts) still do not run (wave-1 gap).
6. Escape (row 50, prerequisite of Moths) is completed by event 358 (object/property event not wired), so a Moths accept by talk is still refused in a fresh game (Moths stays Available only after Escape > 3). The Lizman talk path works because it has no prerequisite.
7. Exporter: `export_art.py` regeneration is disabled (guard) because its shared hit-only style reader drops solid batches (btn_train_energy etc.) and empties hit zones; the committed art is spliced by hand. Fix the shared reader, then remove the guard.
8. The quest-text resolver index for the objective description (see investigation) should be checked by the quests owner.

## Placeholders
- Quest banner panel (`drawQuestBanner`, `quest_banner_presenter_v1.cpp`): a dark translucent fill with a bronze border, drawn in place of the original dqhud dialog frame. Reference frames to replicate: `.local-inputs/claude-preview14/QUESTS/p1/quad-p1-newquest.png` (NEW QUEST, ornate frame, gold title, ivory body text, t~659) and `.../p1/f761.png` (QUEST COMPLETED with Reward and reward lines, t~761).
- Counter banner (QUEST UPDATED, "n / m"): NOT an original banner (no original frame observed for objective counters); it is the Preview 16 tracker line. Verify against a Moths kill sequence in the reference video before keeping it.

## Package files required
- `original-cache/data/pydata/v2quests_pyarray.bin` and `original-cache/data/pydata/v2quests_pyarraynames.bin`: NOT in `windows-source-clock-v19-preview-15-rc4`. Sources: `.local-inputs/claude-preview16/levels-root/original-cache/data/pydata/` and `.local-inputs/assets-extra/android/data/pydata/`. Read through the normal asset root; without them the runtime reports `Quest table diagnostic` and stays inactive.
- `original-cache/data/pydata/character_properties_pyarraynames.bin` (for NPC rows): present in rc4 (no action).
- Text: `original-cache/data/text/global.english` and `gameplaymenus.english` (present in rc4; used for the banner headings through the existing text owner).

## Verifier script
1. Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16questui -Test` (ctest: expect only `session_skill_binding`).
2. Zone/banner/talk/Quest-tab runs: `quests-run/jobs-zone.json` / `jobs-witch.json` (Witch zone from the north or south face), `jobs-moth.json` (Moths banner sequence; capture at frame 420), `jobs-talk-lizman.json` (talk to row 366; expect `Quest talk npc row=366` and NEW QUEST row 51), `jobs-quest-page.json` (Quest Log frame capture at 220). Run each with `tools/quiet_run.ps1 -JobsFile <job> -Parallel 1`.
3. Expected log lines: `Quest zones built=1 _prim_WitchQuestStart[...] level=41`; `Quest zone entered zone=_prim_WitchQuestStart`; `Quest banner kind=NEW QUEST row=52`; `Quest banner kind=QUEST UPDATED row=53` (x7) and `QUEST COMPLETED row=53 xp=20 gold=150`; `Quest talk npc row=366`; `Character menu Quest Log selected frame=150`.
4. Test aids (not gameplay): `--quest-debug-accept F:ROW`, `--quest-debug-kill F:TEMPLATE:COUNT`, `--quest-talk-frame F`, `--quest-page-frame F`.
