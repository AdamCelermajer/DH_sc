# Preview 14 plan (draft, from the seven surveys; waits for user decisions)

Goal (user, 2026-10-10): full character menu working and faithful, main-menu slot metadata connected to character state, ground drops visible and pickable. Chests/pots come after drops.

## Survey findings in one table
| Stream | State today | Root causes / gaps |
|---|---|---|
| Stats/Skills | Skill Upgrade wired; Stat +/- not | `register_stat_training` has no caller, main never builds the stat owner (every click fails). Load-time starter grant eats the only skill point on legacy saves. No level-up feedback. Difficulty hard-coded Normal. |
| Faery | Stub | Page code exists (`features/faery_menu/character_state_page_v1.*`) but is not in CMake and never ran. Unlock = script (Swamp_Intro slot 0). Nothing sets unlock/level at runtime; fresh characters have no Faery. Difficulty hard-coded 0. Only Celest+Hotty spells exist. |
| Quests | Not coded in Windows | features/quests not in CMake; no Quest tab; no kill events, state machine, rewards, banners, counters in saves. Act 1 data and IDA addresses known. |
| Map | Not reachable | No Map tab/hook; no visited-room save; `minimapcameras.bdae` missing. ORIGINAL HAS NO HUD MINIMAP in play (menu_miniMap movie absent, no video frame). |
| Equipment | Partly | Auto-Equip wrong (original equips best item per slot via NativeInvAutoEquipSlot; ALL button unwired). Drop/Transmute no-ops. No class restriction. Texture decoder is fine (B042 = UV mapping/colour transforms). |
| Menu metadata | Blank | Slot file has no location/act/difficulty/date. Projection code exists, unused. Need CharacterState schema v4 + stamping at save points. |
| Drops | Data only | Items exist in the store; renderer and pickup are not in CMake or main. Original: sensor target + Interact gates + loot_orb_fx + tutorial; WAVs missing. |

## Proposed order
0. **Schema v4 first (one owner, S/M):** CharacterState adds slot metadata (save date, current/unlocked difficulty, LevelList row + act per difficulty), difficulty field, quest counters (CQPG v2), optional visited-room section. Legacy v1-v3 load as blank/unvisited. Avoids five workers racing on the save format.
1. **Wave 1 (parallel, after step 0):** Stats/Skills; Faery page; Equipment (Auto-Equip per original, ALL button, Drop/Transmute, class restriction, B042 UV/colour); Menu metadata wiring; Drops (render + pickup + inventory insertion).
2. **Wave 2:** Quest runtime + Quest Log tab + banners/objective hooks/rewards; Map page (+ visited rooms).
3. Verify each with the quiet parallel batch; build one integrated candidate per wave.

## Decisions needed from the user
1. HUD minimap: faithful (none in play) or add an optional one? (recommended: Map page only first)
2. Map: acceptance level (Swamp vs Gothicus Darkwood), marker families, source of minimapcameras.bdae.
3. Saves: legacy slots blank or defaulted (Boglands/Act 1/Normal)? Accept that schema v4 saves cannot be read by Preview 13? F5 also rewrites character.save?
4. Pickup input on PC (recommended: key while targeted; walk-over targets, as in the original).
5. Missing pickup/drop WAVs: other source, or stay silent?
6. Faery scope: Celest + Hotty only? Reject locked Faery in provider and UI?
7. Quest runtime: thin Windows runtime + counters (recommended) vs reuse the Android runtime.
8. Models: Haiku high for S/M tasks; Sonnet for Equipment, Quests, Drops (L tasks)?
