# Act 1 Bugs and Implementation Tracker

Editable source of truth for next-preview gates, reported bugs, implementation work, and user additions. Stable bug IDs are never renumbered. Update each row when assigned, tested, integrated, or accepted. A focused helper test does not close a gameplay bug.

Evidence labels: Source = IDA/recovered-source behavior; Focused = feature or isolated runtime test; Integrated = normal executable run tied to a frozen build; Release = accepted package. Preview13 (418FDB56) is the latest accepted Windows release; Preview12 (1D43942E) and Preview11 (FD850478) are preserved; earlier candidate proofs retain their original hashes. Keep user saves and candidate PID 66836 untouched. No UI automation; CLI/headless tests are allowed.

## Current release — V19 Preview 13 (Preview 12 and 11 preserved)

Accepted Windows package: `.local-inputs/windows-source-clock-v19-preview-11/Play.cmd`; direct Swamp: `Play-swamp.cmd`. Frozen EXE SHA256 `FD850478D8ADC2711F47CD3E0EE5A3897EA6FCC9C8C9BFC40E144227CD270A2F`. Receipt and release notes are inside the package. No saves are shipped or removed.

Included verified fixes: B003 held-Space recovery, B006 death-return crash, B008 selected-class model/saved weapons, B026 separation, B034 player-death reward guard, B036 HUD labels, and B033 targetless melee admission. Enemy XP/checkpoint restore and original MenuConfirm playback passed packaged tests; 100/100 CTests passed. Potion key5 consumes one charge and restores vitals in the packaged executable.

**Next preview priorities:** B007 intermittent creation/profile arrows; B004/B029 target retention; B005 authored impact FX; B035 incoming knockback. These remain open. Full character pages, containers, quests, followers and cinematics remain implementation work. Prior next-preview candidate notes below are historical evidence, not promises that all bugs were closed.
## 1. PREVIEW PLAN

**Ordering rule (user, 2026-10-10):** a defect in something that already exists (wrong art, wrong text, wrong timing, not wired in the normal EXE) is a **bug**. Something that does not exist yet is **implementation** (section 3). Bugs are ordered by how soon they ship: Preview 12 (candidate, verified, awaiting root acceptance) -> Preview 13 core (quick wins + medium) -> verify-and-close -> blocked.

### Preview 12 - ACCEPTED (2026-10-10)

Package `.local-inputs/windows-source-clock-v19-preview-12` (`Play.cmd`; direct Swamp `Play-swamp.cmd`), EXE SHA256 `1D43942ECD52DF7B86BDF77BA2E5CB306AFC76944813F38CA7CE9E6CCB252672`, source commit `f6b7f134`, 101/101 CTests, final verification `coordination/claude-preview12/verify-final-report.md` (APPROVE WITH CAVEATS: audibility not checked by ear; level-music voice inferred from code path, not named in the log). B004, B029, B037, B038, B039 moved to RESOLVED-BUGS.md. Preview 11 (FD850478) is preserved unchanged.

### Preview 13 - ACCEPTED (2026-10-10)

Package `.local-inputs/windows-source-clock-v19-preview-13` (`Play.cmd`; `Play-swamp.cmd`), EXE SHA256 `418FDB56D421E15DB540AC01A34D9AD1E9B158E6328C3FCF10AD8463028B7ECE`, source commit `e685ed13`, 102/102 CTests, final verification `coordination/claude-preview13/verify-final-report.md` (41-job quiet batch; the one FAIL, DROP on an unequipped item, was reviewed in R-report.md and overturned: the verifier's job selected the equipped item). Resolved and moved to RESOLVED-BUGS.md: B020, B043, B044. Shipped partially and still open: B007 (fix tested headless, real key input unconfirmed), B040, B041, B005, B042, B002/B024 (Faery HUD icon only). Closed by the user: B013. New: B045. B035 was NOT shipped (logic ported, not wired; push rate is zero at live-like inputs; verify the Knight push-resist 25600 first).

### Next: Preview 14 plan (user direction, 2026-10-10: feature work resumes)

Full character menu working and faithful (Stats/Skills point attribution with real effect, Faery page tied to unlock state, quest system + Quest Log, Map page + HUD minimap, Equipment pages with Auto-Equip of everything), main-menu slot metadata (map/act/difficulty) connected to the character state, and ground drops visible and pickable into the inventory (before chests/pots). Survey reports: `coordination/claude-preview14/*-survey.md`.

### Preview 13 - scope (historical)

| Group | Bugs | Kind | Notes |
|---|---|---|---|
| A Creation flow | B007 | quick | Slot allocator can return slot 4 when all slots are full (source audit). |
| B Equipment page | B042, B016, B019 | quick | Art background, text/labels, avatar. One owner (visual). |
| C Skills page | B018, B020, B021 | quick / stretch | Empty-slot labels first; level locks and subclass chains if time. |
| D Audio follow-ups | B040 | quick | Death, pause, return-to-menu, safe-zone/witch-cave/water tracks. |
| E Effects polish | B041, B005 | quick | Trail duration/colour vs reference; BashDown ground impact. |
| F Knockback | B035 | medium | Wire source state10 push into the normal EXE. |
| G Hit reactions | B013 | closed 2026-10-10 | Closed by the user: not reproducible with live-like inputs (see RESOLVED-BUGS.md). Reopen only on a new report with save/enemy/clip. |
| H PC HUD / Faery | B002, B024, B009 | medium | Key order proof, Faery spell path and visuals, page navigation. |

### Gate for every bug (unchanged)

Investigate IDA + reference video -> patch -> focused test -> independent verifier on the frozen EXE -> fidelity check against the video -> only then move the row to RESOLVED-BUGS.md.

## 2. KNOWN BUGS

Stable IDs never change. Open means not accepted in a released package, even where a component test passes. Rows are grouped by release; within a group they are ordered by the P13 groups above.

### 2a. Preview 12 - accepted 2026-10-10

B004, B029, B037, B038 and B039 are resolved in Preview 12 and moved to [RESOLVED-BUGS.md](RESOLVED-BUGS.md). B040, B041 and B005 shipped partially and stay open in 2b.

### 2b. Preview 13 core - quick wins

| ID | Observed / reproduction | Source evidence | Fix or current state | Focused evidence | Integrated runtime | Owner / reviewer | Next Preview gate |
|---|---|---|---|---|---|---|---|
| B007 | Character creation/back/confirm/remove flow is broken or inconsistent. | Source menu stack uses authored push/pop/confirmation actions and slot/player arguments. | A fresh source audit found slot allocator can return 4 when all slots are occupied; the Navigator now latches `result_unmapped` after a successful provider call with invalid slot, preventing duplicate create on repeated Confirm. | Focused menu_flow strict test passes 91 assertions; full 13-input runner used a stale archive and crashed, so it is not evidence. | Normal route remains open. | B007 owner terminal / bug_gate_review re-review; lead (main) | Rebuild current frontend archive, rerun isolated create/back/refusal/remove/recreate/start, then normal visual route. |
| B040 | Original map ambience is missing. | Locate authentic ambience assets and source map/loop/focus lifecycle. | OPEN; no fabricated replacements. | Pending | **P12 candidate: partial** - real swamp VXN (uid 467) starts once after load. Not handled: pause/focus-resume proof, death, return-to-menu, safe-zone/witch-cave/water tracks; loop/audibility needs a listener. User Preview11 report | Unassigned; documentation-only pass | Verify authentic ambience starts, loops smoothly and stops/transitions correctly in normal gameplay. |
| B041 | Recurring missing white/bluish sword swing or skill trace. | Identify exact weapon/skill effect binding, anchors, timing and blending from video and IDA. Related B005 ground impact is distinct. | OPEN; decoded effect packets are not visible trail proof. | Existing FX helper evidence only; exact trail case pending. | **P12 candidate: partial** - combo_01/02/03 swoosh assets staged; trail visible ~8 frames (0.13 s), darker blue than reference (pale white-blue, ~0.6-0.8 s). User Preview11 report; visible trail unverified | Unassigned; documentation-only pass | Match original swing/skill sequence and capture visible animated trail in the normal renderer. |
| B005 | Knight ground impact/effect is missing. | Knight SkillList first slot→SkillTable7 BashDown→Anim root347/clip1234, step0 FX164/AnchorFX1/MoveGO1, exact URI data/3D/interface/skill_dh2_prince_warrior_bash_down.bdae. GroundSlam clip1239 do_skill at566ms is separate; v1.0.3 video streak is unidentified. | Anchored observer/error propagation is tested. Exact-URI/hash resolver now selects the 34,420-byte effect BDAE and rejects the 15,528-byte animation decoy. | Strict BRES/particle-scene test passes authored-node decode, missing URI and wrong-SHA cases. | **P12 candidate: inconclusive** - BashDown FX dispatches (sequence 347), no ground impact seen in frames 90-112; no reference comparison yet. Lead must stage/resolve the exact effect asset and capture it in the normal same-frame path. | Resolver feature terminal / bug_gate_review; lead owns asset root and caller | Resolve canonical URI by exact source identity/hash, reject wrong clip, rerun the observer and capture authored BashDown FX; no fabricated nodes. |
| B042 | Equipment page: the art background does not match the original (feature exists, appearance non-conforming). New user report 2026-10-10. | Source Equipment sprite456/InvMain art and background layers in the SWF export (`port/engine-ui/reference`, `features/equipment`), reference video Part 2 menu sheets (`dh2_video_research/sheets2`). | OPEN; not yet investigated. | None | User report on Preview 11/12 | Unassigned (P13 equipment group) | Capture the page at 480x320 scale and at window size; compare every background/frame layer with source and video; fix layer set/order/scale/alpha. |
| B016 | Equipment text/font spacing and duplicate Transmute/Value labels are wrong. | Source SWF field roles, glyph metrics, enabled/disabled item frames and ValueBox derivation audited. | Overlap/triple-Transmute correction is terminal; fresh provider derives amount from same-player resolved property197 and actual design multiplier for approved original unpowered ItemTable rows. | Provider strict runner passes real Longsword01 value100→formatted18 and stale/generated-item rejection; independent B019 review passed. | Normal-build long-name/value capture and lead binding remain open. | b016_valuebox_provider_fresh (terminal) / b019_equipment_avatar_fresh reviewer; lead production binding | Rerun strict provider, wire before page bind, capture representative long name plus real ValueBox value in normal UI. |
| B018 | Empty skill button/label or wrong skill slot behavior. | Native HUD indices are zero-based; empty slot rejects before AI/mana and never shifts later slots. | Fresh AddText guard clears stale labels when same-state training admission rejects and preserves localized text when provider is absent; lead accepted for next build. | Focused composition test and independent B020 rerun pass. | Normal HUD label/circle capture still open. | Terminal `/root/b018_skill_text_fresh`; lead owns HUD render/input / bug_gate_review | Verify empty labels/buttons and physical circle/key mapping against saved rows in the normal UI; do not reinterpret visual circle position as a native slot index. |
| B019 | Equipment avatar/body does not reflect the selected item or action. | Equipment source rows and preview pane use authored class/item material/bounds. | Same-Session feature matrix passes actual Knight/Rogue/Mage source starter rows, material topology, weapon geometry, unequip, SaveStore and fresh rebind. | Strict runner and independent review PASS. | Normal Renderer cross-class capture and native GameSave remain open. | Feature matrix/review terminal; lead owns renderer integration | Verify the three source class/item avatars in normal viewport after equip/unequip and save/reload. |

### 2b-bis. New

| ID | Observed | Source evidence | State | Focused | Integrated | Owner | Gate |
|---|---|---|---|---|---|---|---|
| B045 | In the equipment lists (Right hand, Feet), clicking the second row lands on other controls (Ring 1 / Left hand) instead of selecting the bag item. Found by the B016 helper, 2026-10-10. | Not investigated; list hit regions vs row geometry in features/inventory (hit test) and the original list items. | OPEN. | None | Observed on the Preview 12/13 EXE with a bag item in the same slot. | Unassigned | Quiet batch: click the second list row, expect selection change; add a hit-region test. |

### 2c. Preview 13 core - medium

| ID | Observed / reproduction | Source evidence | Fix or current state | Focused evidence | Integrated runtime | Owner / reviewer | Next Preview gate |
|---|---|---|---|---|---|---|---|
| B035 | Source Push-bearing incoming hit has no visible knockback. | Same-Session pre-hit-Idle gate proves actual Lizard 0x98 emits one Push request; CharAnimTable row48 field8/16 contain exact two-clip MoveGO sequences. | Feature bank/pose consumer replays the authored sequence through the same Session; body moves 1.3257 units. | Strict feature runner and independent reviewer PASS. | Main state10 transition, actual collision/motion owner and OnFocus/OnBlur effects remain unbound; no normal hit capture. | Feature/review PASS; `/root/b035_state10_runtime_sol` owns the bounded feature consumer; lead owns main | Compose source state10 admission/gating and physical effects, then verify real incoming hit and clean-hit control in normal EXE.
| B002 | Visible PC skill circles and key order do not match; physical left/middle/right must bind source slots `[2,0,1]`. | Source-terminal mapping from authored Skills-page sprite496 `refreshUsedSkills`; Android/native `NativeHUDSkill(slot)` remains logical zero-based `[0,1,2]`. | Single pre-cast PC-key translation is integrated; HUD packet now carries an optional Potion0 count only when CharacterState and actor identity match. Focused HUD and key-mapping runners plus independent review pass; saved rows and Android slots stay unchanged. | Connected Rogue fixture key1→empty source2 rejects; key2→middle/source0 JumpKick row0/rank1/root521 completes. | All-key visual/cast proof on a fresh normal executable remains pending. | b002_pc_hud_fresh (feature packet) / bug_gate_review; lead owns main translation/render/hit | Capture left/middle/right labels and verify PC keys 1/2/3 reach source slots `[2,0,1]` without renumbering saved slots. |
| B024 | Cast/Faery spell key path or spell visuals fail. | Current Mage selected Faery source row and Cast sequence are validated; Hotty is Mage list2 row7, Celest is slot0; Fake_Hotty is invalid. | JumpKick/Celest and current Hotty coordinator helpers are implemented. | Connected same-Session cast suites pass; CPU source BDAE/effect adapter tests pass. | `FC19…` normal run reports JumpKick and Celest pass. Terminal strict pixel/pass test confirms zero-alpha perimeter RGB contributes under source ONE/ONE; this is source-compatible. Main13 draw-packet fields and a matched original frame are absent, so no visual mismatch is proven. | b024_spell_visual_parity_fresh (terminal) / bug_gate_review; lead owns draw-keyed normal GPU capture | Verify save/reload and normal selected spell; resolve Main13 additive ground-glow edge only with source-backed pixel/blend evidence. |
| B009 | Character menus/pages are incomplete or inconsistent across Stats, Equipment, Skills and Faery. | Original menus are source pages with per-page callbacks; one helper or screenshot does not establish complete navigation. | Faery provider now captures a weak Session lifetime at bind and checks it before any raw Session dereference; bank-backed activation and Quest exact-symbol page/hit resolver remain source-tested. | Strict connected test passes activation then destroys Session; later click rejects without invoking callbacks. Independent reviewer reran the cast suite. | Not all menu routes/actors have a current normal executable capture. | Faery lifetime feature terminal / bug_gate_review; lead owns main | Verify pushed Quest route and Faery activation on normal executable with selection persistence and current page graphics.
| B021 | Subclass chain art/icons do not appear or are incorrect. | Specialization rows share ClassID but have distinct SkillTree and SkillIcon rows; base Rogue blank cells are placeholders. | CharacterTable-backed family resolver maps actual specialization rows and SkillTrees; source-icon projection is tested. | Strict source tests and combined RuntimeSkills runner pass; visual evidence still shows base Rogue placeholders only. | No full normal-UI class-change/save/reload matrix. | `/root/b021_subclass_art_fresh` / bug_gate_review; lead normal capture | Choose actual Archer/Assassin (and Knight/Mage specs), verify source icons/chains, selection, current skill state and return after reload. |

### 2d. Verify and close (believed fixed or only needs a normal-EXE check)

| ID | Observed / reproduction | Source evidence | Fix or current state | Focused evidence | Integrated runtime | Owner / reviewer | Next Preview gate |
|---|---|---|---|---|---|---|---|
| B001 | Movement/locomotion was reported broken or inconsistent on the normal route. | Current movement/input path has since been source-wired; preserve authored movement/root-motion policy. | Reported fixed in candidate `1787`/`ACB`; not released. | Movement/physics regressions and current-source tests pass. | Candidate runtime is reported passing; no accepted-package proof. | Root + lead / bug_gate_review | Fresh ordinary Knight and Rogue movement in one normal run; package only after regression and save checks. |
| B010 | Player does not face the selected target during an attack. | Attack update/event both call LookAt; aligned fixtures could not expose the defect. | Root owns the source-backed Session correction; non-aligned regression is terminal. | `features/combat/run_target_facing_regression_tests.ps1` passes for Knight/Rogue against a Lizard: selection alone does not turn; admitted attack rotates toward a moving target with stable target/generation/RNG. This is feature-only evidence. | Normal GUI verification pending. | Root (Session fix) + combat_anim_audit (test, terminal) / bug_gate_review | Rebuild after root terminal; verify a non-aligned normal gameplay attack, not an aligned fixture. |
| B012 | Death animation stops midair or fails to finish naturally. | Source Died root-motion and End34 departure are distinct from gravity/body detach. | Corrected test samples the full authored 800 ms Died endpoint and holds terminal pose/transform after End34; no post-detach motion callbacks are expected. | Focused runner passes; B011 reviewer agrees the old four-extra-callback assertion was unsupported. | Lead normal JumpKick→Died→End34 capture remains pending. | Feature worker terminal / bug_gate_review; lead capture, root core | Capture full authored Died clip through terminal frame and body removal; verify no midair freeze or replay after reload. |
| B015 | Pause/resume does not reliably freeze or restore gameplay. | Menu/pause gates set elapsed gameplay time to zero and skip Session update; focus loss alone is not pause. | Pause runtime path passes on current candidate; keep accepted-release status separate. | Pause UI and time-control tests pass. | `FC19…` normal run reports pause pass; screenshot alone is not proof of simulation freeze. | Lead / bug_gate_review | Verify paused clip phase, actor/body position, RNG and timers across multiple frames; resume and verify one clock continuation. |
| B027 | Celeste encounter mob keeps advancing after melee until its next attack. | Source AIS/RuntimeNavigation and physical timer ordering audited. | Same-session controller regression passes: Swamp lizard ActorState stays stationary and no approach calls occur during the 800 ms cooldown; target clears at the existing Session completion sample and reacquires before next attack. Physical solver was not stepped. | `features/enemy_ai/enemy-cooldown-advance-20261010.json` focused runner passes. | Normal gameplay and physical body drift not verified. | `act1_enemy_luna` (terminal helper) / bug_gate_review; lead owns next integrated check | Step NativeWorld in normal session through the cooldown; confirm no physical advancement, then source-correct reacquisition. No guessed offsets/timers. |
| B032 | Direct-preload rejects a genuinely unlearned rank-zero current-class source skill slot before its first grant. | Rank-zero preload accepts only a saved row/name matching the active SkillList; learned/rank state still gates activation. Other learned-slot behavior remains on the normal resolver. | Focused fix PASS: actual Knight row0 rank0 preloads its authored SkillTable root outside learned hotbar, then resolves after source training. | Strict C++17 runner and independent reviewer rerun PASS; no-preload and mismatched-row rejection pass, saved row/slot unchanged. | Not yet rerun through the normal DA4C executable. | `skill_menu_luna` / `bug_gate_review`; lead owns caller | Normal current-class direct-preload→first-grant path on a frozen build; preserve rank/save data. |

### 2e. Blocked or needs missing evidence

| ID | Observed / reproduction | Source evidence | Fix or current state | Focused evidence | Integrated runtime | Owner / reviewer | Next Preview gate |
|---|---|---|---|---|---|---|---|
| B022 | Class-select camera shows a black/void ground band. | Projection, FOV/aspect, viewport and camera transform match for sampled source; current v1.0.2 mesh has no settled-hole triangles. Original v1.0.3 scene binary and target-orientation field are unavailable. | Unresolved source/version/content discrepancy; no guessed zoom, underlay, foliage light or copied floor. | Projection/frustum/33-idle-sample audits reproduce uncovered pixels; no source-backed correction. | Normal class-select capture confirms the visual issue; cause is not established across versions. | Frontend owner (terminal) + lead / bug_gate_review | Obtain v1.0.3 scene binary or a matched native camera/cursor/viewport/orientation receipt; then compare same-asset coverage. |
| B028 | Lizard attack/hurt/death sound cues are missing or inaccurate. | Source rows map injury 377→UID284 and death 374→UID285; WAV bytes are absent from searched local caches/APKs/OBBs. | No replacement media fabricated. Rogue Quickness/Roundhouse assets do match source manifest hashes. | Strict decoder tests pass for available Rogue assets; no lizard sample-byte/decoder test possible. | Audio runtime path works for available sources; exact lizard WAV playback cannot be verified. | Audio owner (terminal) + lead / bug_gate_review | Locate authentic samples or keep the cue explicitly unavailable; never substitute/generated WAVs. |
| B011 | Turning/attack motion looks clanky, including W/S reversal and lateral displacement. | Recovered order applies early root displacement using current facing, then rotates later; reference lacks a matched W/S reversal. | Fresh Knight/Rogue same-Session non-collinear tests reproduce the lateral arc predicted by source; no code change was justified. | Strict test passes twice: reversals settle within 16 frames, released input stops translation, attack pose persists while target heading changes. | Normal executable capture/matched reference remains open. | B011 feature test/report terminal / root review; lead for normal capture | Compare a normal executable or matched source clip; change rotation/root policy only if it diverges from the recovered source behavior. |

### 2g. Reported on the Preview 14 candidate (2026-10-10 evening) -> B046-B052 and I025-I026 fixed in Preview 15 (moved to RESOLVED-BUGS.md); only B042 (update) remains

| ID | Observed / reproduction | Source evidence | State | Owner | Gate |
|---|---|---|---|---|---|
| B042 (update) | Equipment page is "a tiny bit better" but text and art still not conform to the original (user screenshot Torso Details, 1798x1096). | See B042 row above and `coordination/claude-preview13/Q-report.md`; reference `l/ref/p1-t336.png`. | OPEN | agent `eq-text` | Side-by-side with reference: text font/size/position/colour, panel fills, sword texture. |


### 2i. Reported on Preview 15 (2026-10-10 night) -> one fresh Sonnet 5.5 medium session per bug (user rule); video reference FIRST, then IDA for logic

Screenshots: `.local-inputs/claude-preview15/user-shots/b0NN-*.png`.

| ID | Observed / reproduction | State | Owner |
|---|---|---|---|
| B053 | Title screen ("Touch the screen to continue"): the raw condensed glyph/sprite atlas is visible under the splash (spinning ring icons, arrow buttons, power icons, yellow dots) and the splash fills only the top-left; the needed glyph frames must be picked from the atlas, nothing else drawn. | OPEN | agent `b053` |
| B054 | SKIP button is shown on the Gameloft logo; in the original SKIP exists only on the story cinematic between the Gameloft logo and "Touch the screen to continue". | OPEN | agent `b054` |
| B055 | Loading screen is the plain placeholder (black panel, LOADING, tip box, red bar). The previous Android reconstruction found the REAL loading menu: locate it (earlier Android reconstruction work in the repo/.local-inputs and the Android cache) and reproduce it. | OPEN | agent `b055` |
| B056 | USER REFERENCE (original, video 8:32 = 512 s, `user-shots/b056-REFERENCE-original-equipment-video-0832.png`): "it is supposed to look like that" (flat list panel with damask background, NO black divider lines, grey details panel upper right, orange selected panel lower right, red X on unusable items with green names, dark disabled EQUIP, VALUE box with item icon, Cris R82 watermark is the video author). Equipment page (Right hand, screenshot) "still not resolved": compare against the reference video (Part 1 equipment pages) and fix every remaining difference (black divider lines across the avatar, sword render, text layout, plates...). B042 update 2. | OPEN | agent `b056` |
| B057 | NOTE: the original shows a red X + green name on items whose REQ is not met and a dark/disabled EQUIP (see B056 reference frame), so blocking may be correct; the question is which requirement (what is ENG, why 7) and whether it is shown/evaluated like the original. Item cannot be equipped because of `REQ: 7 ENG` (Imbued Armor) although the character lacks it; original behaviour must be checked (video + IDA): requirements may not block equipping the way implemented, or the requirement text/value is wrong. Also the Value text overlaps the avatar. | OPEN | agent `b057` |
| B058 | Walking makes no sound at all: is it intended? Check video for footsteps; implement the original footstep cues if present. | CLOSED - INTENDED (user confirmed 2026-10-10: the original has no footstep sound; IDA agrees, report on branch fix/b058; shipped sfx_fs_* rows are unused) | agent `b058` |
| B059 | Dropped loot (Conscript Helm) is shown as a brown bag/sack: check what the original shows for equipment drops (video) and fix the drop visual/icon. | OPEN | agent `b059` |
| B060 | Belt and Helm item icons are swapped (Conscript Belt shows a helm-like icon, Conscript Helm a belt-like icon). | OPEN | agent `b060` |
| B061 | Equipped helm is visible on the avatar in the equipment page but missing on the in-game character (screenshots). | OPEN | agent `b061` |
| B062 | Huge performance problem in skills and in general (slow). Profile real frame times, find the hot spots, fix generally. | OPEN | agent `b062` |
| B063 | Looting must be by walking onto the item with the original logic, not by pressing E. | OPEN | agent `b063` |

### 2h. Implementation requests from the same session (feature work, Preview 15)

| ID | Request | State | Owner |
|---|---|---|---|

### 2f. Reclassified to implementation (feature work; evidence kept)

B023 -> I005 (map markers/providers), B025 -> I003 (chest/pot enrollment), B030 -> I009 (authored intro/spawn). They remain here only so the evidence is not lost.

| ID | Observed / reproduction | Source evidence | Fix or current state | Focused evidence | Integrated runtime | Owner / reviewer | Next Preview gate |
|---|---|---|---|---|---|---|---|
| B023 | Minimap/map page lacks correct markers or interaction. | `menu_MapSheet` exact-symbol adapter is available; only family-3 same-Session player marker is supported. | Feature decoded actual SWAMP module bounds and source inclusive-XY containment; output visitation intentionally remains unknown. | Strict compile and linked decode test pass for nine modules against independent mesh unions. | No production registration or normal GUI claim; current Level membership, source visitation and camera bounds still need owners. | RoomZone producer terminal / bug_gate_review; lead owns main registration | Supply current-Level ordered zone membership and visitation, feed typed zones to existing provider, then verify map markers/ResetZoom/input. |
| B025 | Pots/chests/urns do not open/break, drop items, or persist correctly. | Actual source chest/urn IDs, opened markers, item rows and 7-byte state components are known. | Admitted-destructible binder composes prior admission, current Session/ObjectId/state and retained visual/drop owners; no main enrollment claim. | Strict runner asserts urn RNG advance, `loot_table==9`, and missing RNG/store rejection before visual enrollment; state4 restore remains silent/no replay. | Main scene enrollment/input/render/provider callsite absent; `_Summon` unclaimed. | Feature binder and failure-prefix hardening terminal; lead owns main | Wire admitted authored object enrollment and verify visible input, drop and pickup in normal EXE.
| B030 | Enemy spawn/intro timing or scripted population is inconsistent. | Authored LizardIntro trigger→Script_SpawnCharacter and Limbus→PreSpawn→Spawn source paths are known. | Feature consumer checks authored declarations/order through existing lifecycle. Wrong-module rejection occurs before mutation; actor pointer identity is a trusted caller contract, not verified by the fixture. | Corrected runner and independent review pass: wrong module leaves both actors PreSpawn17, valid order reaches Spawn1, duplicate rejects. | Trigger contact, source prefix gates and production caller remain open; current normal run covers active Lizard combat only. | Feature implementation/review terminal; lead owns caller | Wire real Module receipt and same-session actor lookup, then verify authored trigger timing/order once. |

## 3. TODO UNTIL ACT 1 COMPLETE — by system

These are implementation goals, not reported bugs. Component results are not production completion.

### Main menu, character creation, and cinematics

| ID | Work item | Current component | Missing production result / owner | Next verification |
|---|---|---|---|---|
| I001 | Complete creation/profile/menu route coverage | Headless current-archive route regression passes authored create/back/refusal/remove/recreate/confirm/start | Main menu registration, death return and fresh normal EXE integration; lead | Normal EXE repeats full profile route using isolated copies; no user profile changes |

### Character pages and PC HUD

| ID | Work item | Current component | Missing production result / owner | Next verification |
|---|---|---|---|---|
| I002 | Complete Character pages and PC HUD | `pc_gameplay_hud_v1` emits source-art draw/hit packets in physical circle order; key5 `RuntimeSessionPotionUseV1` transaction also passes a linked Session test | Lead supplies viewport layout, appends HUD packets, wires `SemanticInput::Surface::hit`, and binds potion after the source controller gate | Normal executable cross-page and key test; labels match physical positions, Android logical slots [0,1,2] stay unchanged |
| I013 | Bind PC key transport | Feature semantic-input test passes strict C++17: key edges→existing mapper once→[2,0,1]; SaveStore rows and Android slots [0,1,2] unchanged | Main’s single pre-cast conversion is integrated; physical HUD labels/clicks and all-key normal EXE proof remain | Fresh UI key1/2/3 casts and circle-label capture, no double mapping |

### Combat, movement, enemies, and effects

| ID | Work item | Current component | Missing production result / owner | Next verification |
|---|---|---|---|---|
| I008 | Complete Mage projectile impact/render | Staff01 source plan, renderer packet, NativeWorld contact and impact prefix pass | Main launch/step/render callsite and remaining F_ApplyResult secondary effects; lead/root | Real Mage projectile flight, contact, source result/effect and cleanup in normal EXE. |
| I009 | Complete enemy authored intro/spawn behavior | Active enemy AI and source lifecycle tests pass | Production trigger/contact admission, block-save/camera/controller effects; lead | Authored Intro trigger starts exact source lifecycle once. |
| I014 | Finish Space-after-Skill/target retention | Source regression owners active | Root Session fix only if reproduced; lead input hookup | Same actor Skill→Post→Space/release/reacquire |
| I015 | Close Knight BashDown impact | Authored root347/FX164 mapping known | Feature observer and lead event/render hookup | Visible exact resource/packet in normal build |

### Pots, chests, drops, pickup, and rewards

| ID | Work item | Current component | Missing production result / owner | Next verification |
|---|---|---|---|---|
| I003 | Finish authored chest/pot scene enrollment | Decoded-declaration enrollment now covers actual chest and urn records, marker-to-store flow, pre-RNG candidate rejection, probability-failure prefix, and silent restore. | Main must build actual authored candidates/admission facts and supply live Level/online/quest/audio/physical/key/script services; lead owns caller. | Wire the callable enrollment into the normal scene, then verify visible open/break, item pickup and persistence. |
| I016 | Complete reward/item lifecycle | Same-session XP/vitals, store and pickup tests pass | Lead normal kill/reward/pickup/save composition | Exact XP/currency/item once, no healing or duplicate |
| I017 | Complete potion HUD/use behavior | `RuntimeSessionPotionUseV1` linked Session test passes actual Potion0 ID925/type14, source PropertyRules and Regen kernel | Lead binds key5 behind the source controller gate; HUD reports the actual available count only | Normal EXE confirms exact source vital/count behavior, held-repeat rejection and no duplicate/lost item |

### Quests, NPCs, tutorials, and companions

| ID | Work item | Current component | Missing production result / owner | Next verification |
|---|---|---|---|---|
| I004 | Complete Quest Log pushed-page route | Exact `menu_QuestLogSheetNEW` provider and source hit resolver pass | Main stack symbol gate and viewport→480×320 conversion; root/lead | Open Quest Log, select row, details, Activate and persistent marker in live session. |
| I005 | Complete Map Sheet production page | Exact `menu_MapSheet` adapter, family-3 player marker, and decoded module-zone candidate producer pass; actor lease and inclusive XY containment are source-backed, visitation remains unknown. | Main registration plus live Level+36 membership, RoomZone↔Module backlink/visited byte, camera planes/activation/visible-room and remaining marker providers; lead | Joint SWAMP geometry+Session test; then live map open/close, authored markers, zoom/reset and coordinate correctness. |
| I006 | Complete companions | Same-session planner/movement consumer follows a moved live master; Priest/Rene policy stays distinct; WarpBehind fails closed without destination. | Bind actual production event caller and source WarpBehind destination; lead | Same-session follower follows master through source MoveTo/WarpBehind/Stop without duplicate actor registry. |
| I007 | Complete Swamp King encounter | Same-session timer/phase consumer passes | Caller-owned source HP/script/native-AI/admission facts and actual attack/transition delivery; lead | Full encounter from admission to phase transition and death in normal session. |

### Audio

| ID | Work item | Current component | Missing production result / owner | Next verification |
|---|---|---|---|---|
| I018 | Wire real audio sources and UI cues | Exact MenuConfirm/MenuSelect adapter and source WAV decoding tests pass | Lead frontend/main callback enrollment; original Lizard WAV bytes absent | Normal audible menu run with real assets only |
| I019 | Complete attack/step audio | Feature helpers pass; no Lizard substitute | Source occurrence→output caller verification | Same-frame cue evidence; no invented audio |

### Save, persistence, and release verification

| ID | Work item | Current component | Missing production result / owner | Next verification |
|---|---|---|---|---|
| I011 | Finish Linux alternate-platform backend | Private Ubuntu configure and partial compile pass; Linux owner active | SDL2/OpenGL compile/link, headless runtime and Jammy baseline; linux_preview | Linux-only verification; preserve Windows outputs and user saves |
| I012 | Package and accept next Preview | Preview11 accepted; subsequent package remains root-owned | Root acceptance after selected gates, coherent build, review and save preservation | One frozen candidate with complete evidence, versioned package and preserved user saves |
| I020 | Verify profile persistence fields | GameSave v3 and SaveStore focused tests; 9C40 body restore pass | Lead normal save/reload matrix | HP/MP, XP, currency, gear, skill rows, Faery and restart |
| I024 | Implement natural death revival/checkpoint continuation | Same-process HP0 return to Single Player is safe; no auto-revive inferred | Source-authored fade/checkpoint/revive path and modern save owner; lead/root | Verify legitimate death retry/revive, checkpoint and preserved profile/world state without duplicate rewards or bodies |

### World progression and full Act 1 fidelity

| ID | Work item | Current component | Missing production result / owner | Next verification |
|---|---|---|---|---|
| I010 | Resolve class-select ground fidelity | Projection/FOV/world-transform audit agrees with source; v1.0.2 scene has no triangles at settled holes | Need v1.0.3 scene or matched native orientation/camera receipt; frontend/lead | Compare same-asset coverage before any visual modification |
| I021 | Complete transitions/checkpoints | Modern save route tests exist | Lead authored transition caller and source fields | Normal room transition and restore |
| I022 | Complete tutorial/cinematic triggers | Source trigger slices exist | Same-world event caller and source timing | Authored Act 1 trigger order |
| I023 | Complete Act 1 fidelity/playthrough | Feature receipts span systems | Root/lead versioned normal run with independent review | Creation→combat→menus→loot→quest→boss→save/reload |

## 4. USER ADDITIONS INBOX

Editable checklist. Preserve user requests until assigned a stable bug or TODO ID. Check only after user confirmation or the documented acceptance gate passes.

- [ ] Main menu/character creation: back/cancel, delete confirmation, repeat creation, class-selection animation/camera, labels/models, death→Single Player crash.
- [ ] Character pages: Equipment avatar/actions, Skills text/locks/subclass chains, Faery, source-accurate layout.
- [ ] PC HUD physical circles left/middle/right map to source slots [2,0,1]; Faery key4; potion count/activation. Preserve Android slots [0,1,2].
- [ ] Space release after skill, target/ring retention, attack continuity and Knight ground impact.
- [ ] Death/reaction continuity, XP/rewards, pause/resume, save/reload and user-save preservation.
- [ ] Minimap, class-select black ground band, chest/pot/urn interactions and loot pickup.
- [ ] Quest Log, dialogue/NPCs, companions, boss gates, tutorials/cinematics and Act 1 transitions.
- [ ] Missing Lizard audio cues: authentic original samples only; no generated/substitute media.
- [ ] Equipment page art background does not conform to the original (2026-10-10) -> B042.
- [ ] New user report (date/build/repro/details): add here.

### Release rule

Only root accepts a release. Preview11 (FD850478) is accepted; do not modify that package. Normal-executable proof must identify one frozen build and timestamp. Keep focused/source/visual and normal-runtime evidence distinct. Preserve user saves until root accepts another candidate.
