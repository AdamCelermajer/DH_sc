# B068 Act 1 cinematic caller and fixture audit

Read-only support for the B068 owner. No cinematic implementation changed.

## Visual evidence checked first

Reference is P1, `Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4` (640x360, 30 fps; source/version differs from the v1.0.2 IDA export). Reused the inspected frame sequences and timeline in `coordination/claude-preview15/CINE-survey.md` and the B068 inventory, and looked at `opening.jpg`, `cage-detail.jpg`, `camp-witch.jpg`, and `moth-troll.jpg` in the v20audit folder. Observed: opening/cage sequence P1 75–205 s includes the captive, a flying moth beside the cage, the troll, then Rene/Celeste dialogue; camp conversation P1 540–552 s; TrollReturn walk/roar P1 352–360 s; witch conversation P1 610–648 s. CINE-survey places the lizard encounter at 224–226 s, chest tutorial around 315–325 s, potion prompt 289–295 s, level-up prompt 378–385 s, and opening at 78–172 s. For rows marked unknown below, these sequences do not establish a caller or trigger point.

## Source caller map

Declaration source is `.local-inputs/claude-preview161/fix068/v20audit/declarations.tsv`, decoded from runtime `load_actor_definitions` for the Swamp source module; quest source is `quest-calls.tsv` plus `quest-source.txt`. Coordinates below are the decoded placement/world centers as stored in the declaration rows; fixture at the trigger center and cross its authored shape, rather than copying the `position=` property string (some rows carry an offset-form property).

| Script | Recovered caller/admission | Normal fixture and status |
|---|---|---|
| Camp_Intro | `_prim_TriggerZone_Camp_Intro`; Block TriggerZone, count 1, delay 0, no activate condition. | Cross center (-9452.37, 11816.9, 234.447). P1 camp dialogue at 540–552 s. |
| CombatTuto | Caller unresolved in source reviewed. Inventory/run harness can name-start it only; no normal trigger recovered. | Do not call a guessed actor/zone normal. Video has tutorial prompt but does not tie it to a source caller. |
| Door_Inspection_South | `_prim_TriggerZone_001`; count 1, delay 0, `IsBefore_Gothicus2Survivors`. | Cross (-12137.5, 10444, 226.608) only after condition is true. |
| LizBlood_Activate | Quest row 51 `Swamp_KillFiveLizman`, script slot 6. Quest accepts TalkToNPC oid1=366, level row 41; row objective is template 93 ×5. Exact actor↔oid mapping is not proven. | State/event fixture: level row 41 and row 51 transition into slot 6; candidate nearby source actor `_prim_NPC_LizBlood` / `Swamp_MerchantCamp_NPC_C` at (-10757.2,10985.2,255.668), but verify actor id 366 before claiming this talk reproduces it. |
| LizCaptain_Intro | `_prim_TriggerZone_BigLizIntro`; count 1, delay 0. | Cross (-6395.03, 9602.12, 243.661). P1 moth/captain sequence 466–470 s. |
| LizardMan_Intro | `_prim_TriggerZone_LizManIntro`; count 1, delay 0. | Cross (-2897.05,-3.3969,256.739). P1 lizard sequence 224–226 s; HOST report gives a tested path from (-2750,-3.4,255) walking west. |
| MothIntro | `_prim_MothMovieTriggerZone`; count 1, delay 0. | Cross (-4511.71,-42.526,257.356). Video did not show this trigger firing in the inspected sequence; do not claim a video match. |
| Movement_Tuto2 | Caller unresolved. Quest row 50 points to `Movement_Tuto` in slot 10, a distinct script name; that is not evidence for `Movement_Tuto2`. | Use source actor/zone only after caller recovery. Earlier harnesses direct-started the named script; that does not verify normal admission. |
| Swamp2_Intro | `_prim_TriggerZone_18`; count 1, delay 0, `IsAfter_Gothicus2Survivors`. | Cross (-11571,11233.1,226.608) after that condition is true. P1 return/load sequence around 1040–1090 s; conversation pairing uncertain. |
| Swamp_Camp_PrisonerB_a | Caller unresolved. | Candidate camp/NPC setup is not enough to identify this exact row. |
| Swamp_Camp_PrisonerB_b | Caller unresolved. | Candidate camp/NPC setup is not enough to identify this exact row. |
| Swamp_Camp_PrisonerC_a | Quest row 53 `Swamp_Moths`, script slot 10. Prerequisite op 2 on row 50/state 3; accept TalkToNPC oid1=365, level row 41; objective template 94 ×8. | Campaign fixture should satisfy row 50 > state 3, then use the source TalkToNPC event. Candidate `_prim_NPC_Mothgiver` / `Swamp_MerchantCamp_NPC_B` is at (-2326.9,6663.13,256.489); actor↔oid 365 still needs confirmation. P1 Bogwalker conversation candidate around 568–574 s, exact association uncertain. |
| Swamp_Camp_PrisonerC_b | Caller unresolved. | Do not reuse C_a's quest callback without source evidence. |
| Swamp_Camp_PrisonerE_a | Caller unresolved. | No exact actor/script binding recovered. |
| Swamp_Intro | Quest row 50 `Swamp_Escape`, script slot 1; prerequisite op 3, level row 41. | Normal fixture: load row 41 and allow row 50 prerequisite/state update; do not use a named-script start as normal-path proof. P1 opening/cage sequence 75–205 s. |
| Swamp_Serpent_Defeated | Quest row 50, script slot 13. | Start from a valid row-50 state transition that dispatches slot 13 (campaign quest completion/transition), not an arbitrary script start. P1 around 1220–1270 s is only a candidate death/conversation association. |
| Swamp_Serpent_Intro | `_prim_TriggerZone_17`; count is source default (not explicitly serialized), delay default 0; `IsBefore_Swamp_Escape`. | Cross (-15437.4,12000,-337.838) while row 50 has not advanced past the authored condition. B068 owner reports this as the normal intro position. |
| Swamp_Witch_Activate | Quest row 52 `Swamp_KillWitch`, script slot 10. Row accepts zone `_prim_WitchQuestStart`, level row 41; objective property 435. | Fixture row 52 at the authored zone/state; P1 camp-witch sequence 610–648 s. Quest entry/slot transition remains the admission route. |
| TrollReturn | `_prim_TriggerZone_001` (Swamp declaration; distinct hash/name scope from the other `_prim_TriggerZone_001`); count 1, delay 0, rotation -25.1947°. | Cross (-5171.43,4477.95,257.332) through the authored oriented Block. P1 352–360 s. |
| Tutorial_Merchant | Caller unresolved. | Nearby merchant interaction is plausible but not demonstrated by decoded caller evidence; keep as a candidate only. |
| chest_tuto | CINE2 report traces authored `tutorial_treasure` TriggerZone → `DoTutorial` → `chest_tuto`; block count 1, delay 0. | Cross tutorial treasure center (-2033.31,254.846,287.618), rotated -90°, along its authored long axis. CINE2 reports actual zone-contact verification; video chest prompt around 315–325 s. |
| cinematic_Tuto_levelUp | Caller unresolved in current artifacts. | P1 prompt at 378–385 s, but no world event/actor call recovered. |
| cinematic_Tuto_potionUse | Caller unresolved in current artifacts. | P1 prompt at 289–295 s, but no world event/actor call recovered. |

## Shared original admission evidence

IDA pseudocode `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`: `Trigger::CanActivate` 0x3987ac gates against authored count/counter state; `Trigger::Update` 0x398838 updates the authored timer; `TriggerZone::Update` 0x39b458 rejects the local character when its actor flag at `+5248` is set, then checks door state, `CanActivate`, `GameObject::MeetCondition`, and contact before calling `SafeStartScriptOnlyOnce`. For one-shot declarations, test first entry, continued overlap (must not re-fire), and re-entry after count is spent. Quest callers dispatch through `Quest::SetState` 0x480c78 → `Quest::ExecScript` 0x47ff20; `Quest::Update` 0x481818 is the state/prerequisite evaluator. These are distinct admission paths; a script's existence or an isolated named-start run proves neither caller.

The current XML command bank at `.local-inputs/windows-source-clock-v19-preview-16/assets/original-campaign.xml` declares command behavior, not Swamp quest/NPC placement/callers. The video is v1.0.3 while the IDA pseudocode is v1.0.2. NPC oid-to-primitive mapping, all eight unresolved callers, any timing not shown in existing P1 sequences, and version differences remain open; keep these as unknowns.

## Focused caller fixtures for B068 owner

1. Triggered scenes: use declaration-derived center, original level state/condition, a path through the Block, then check one trigger on first entry, no duplicate while inside, and spent-count behavior on re-entry.
2. Quest callbacks: use row 41 with the actual source prerequisite/accept/objective state and inspect the requested quest slot dispatch. Include an invalid level/state and a duplicate dispatch attempt.
3. Unresolved scenes: test named-start only as an isolated content fixture; do not describe it as normal gameplay verification until the source caller is recovered.
