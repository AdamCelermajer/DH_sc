# B102 EQUIP button does not work — ready for integration review

## Status
Worker fix verified in the real hidden EXE; ready for read-only review and root integration. The earlier handoff below is retained as investigation history and is superseded by the completion section. No merge/push or accepted-package changes were made in this resumed session.

## Observed (real EXE build-fix102, real Win32 mouse messages via tools/quiet_click.ps1 from B103 branch, hidden desktop, silent)
1. Torso: Vagrant Armor (--bag-item LeatherArmor01) over StartingSuit: click Equipment tab, Torso slot, 2nd row, EQUIP -> "Equipment state ... slot=0:bag-0-LeatherArmor01", visual changes (poncho body). Works at 1201x720 and 1783x1072 with 60ms clicks.
2. SHORT CLICKS ARE LOST (root cause 1, shared with B103/B095): the Win32 window only records LBUTTON as a per-frame key state; a click whose down+up land inside one poll (10ms hold) was missed 1 of 3 runs. Fix: latch in platform_win32.cpp (commit "B102: latch sub-frame left clicks"); after fix 5/5 with 10ms clicks.
3. Weapon equip (BattleAxe01 over Longsword01, right-hand rail icon then row 2 then EQUIP): the equip does nothing. Two causes seen:
   a. packaged assets/models only has mc_rweapon_longsword/dagger/quarterstaff_01; stderr "Character menu action diagnostic: Original resource not found: data/3d/characters/prince/weapons/MC_RWeapon_BattleAxe_01.bdae" and the transaction is rolled back silently. Models exist in .local-inputs/visual-skin-owner-v6/weapons/ (packaging gap: the package must include the item models of all equippable items; selection-based package manifest lives outside the repo).
   b. with the model present the EXE aborted: "Foundation error: Source stance selection: Requested exact current equipment pair was not prepared by this source library" (main.cpp bindSourcePlayerLocomotion; RuntimePlayerLocomotionLibraryV1 is only preloaded for the startup pair). UNCOMMITTED-then-committed edit: on select_exact failure rebuild the library for the new (main,off) pair and merge clips (small hunk in bindSourcePlayerLocomotion). BUILT (exit 0) BUT NOT YET RUN in the EXE; clips merged into options.character after the visual loaded may not reach the live visual/session (unverified).
4. Not reproduced: user's torso failure in shot-14. Hypotheses: lost short click (fixed), or user's real items (generated instance ids/powers) hitting a failure not seen with --bag-item rows; check stderr "Character menu action diagnostic" in the user's run.

## Remaining steps
- Run weapon equip again with the fixture below (needs pkg overlay with battleaxe model); confirm no abort, equipped item/stat/visual change, unequip and auto-equip buttons (clicks: auto-equip ~ (235,115)px, unequip ~(1000,115)px at 1201x720).
- Decide asset gap: ship all weapon models (and check armour model parts) in the package; make a missing item model non-fatal AND visible (do not roll back silently).
- Test via real mouse on a pickup-acquired item; unit test for the latch is not written (platform layer, no test harness).
- Video/IDA match statement still owed.

## Files touched
port/windows-foundation/platform_win32.cpp (latch; plus cherry-picked B103 WM_MOUSEMOVE/pointer tracking + tools/quiet_click.ps1), port/windows-foundation/main.cpp (stance rebuild fallback).

## Fixture / traps
Scratch: DH_sc/.local-inputs/claude-v20.1/fix102 (gen3.ps1, click.sh: `./click.sh tag exe "T:X:Y:HOLD,..." frames "ItemA,ItemB" menuFrame "--arg;value"`; clicks are client px, T seconds after window; portrait click (85,75) opens menu when menuFrame=99999, else menu opens by --profile-click-frame). Args = windows-source-clock-v20/swamp.args minus --fresh-player plus --bag-item, --campaign-skip-frame 60. pkg/assets = hardlink copy of the v20 package assets (models is a real dir where extra weapon models may be added). TRAPS: assets junctions inside the asset root fail ("escapes asset root"); I accidentally wrote models/models + battleaxe into the accepted package windows-source-clock-v20 and removed them again (exe SHA 8ba1e537... unchanged; only the models dir mtime changed). Never Copy-Item into a junction. Item ids: LeatherArmor01 = "Vagrant Armor", BattleAxe01 = "Useless Hand Axe" (assumed). Equip button px at 1201x720 = (1005,433); Drop is (1073,691).

## Resume investigation (2026-10-11; before further behavior changes)
- Visual: directly inspected P1 v1.0.3 frames 509-523 s at 1 s spacing, saved original509-523.jpg. At 510 s Torso shows Ceremonial Garb; 515 s Feet shows Vagrant Boots with Ceremonial Boots below; 517 s Hands shows Vagrant Bracers; 519-522 s Waist changes from Vagrant Belt to Conscript Belt after selection/EQUIP, and the equipped name/checkmark changes on the next frame. Right-hand and left-hand pages show unmet claw rows with red X and a disabled equip button. These frames establish successful armor selection/equip and disabled requirement UI; they do not demonstrate axe, unequip or auto-equip timing.
- Logic: reused B057 exact IsEquippableBy 0x3fa330 requirement/class/online gate and SWF btn_EquipItem admission. Re-read NativeInvEquipItem 0x43e600 (3 numeric args -> local character virtual EquipItemToSlot), NativeInvUnequipItem 0x43ef64 (2 args -> UnEquipItemFromSlot), NativeInvAutoEquipSlot 0x43da2c (-1 -> all, valid slot -> unequip then auto-equip). Character wrappers 0x3a9eb0/0x3a9ee8/0x3a9f30 update gear properties, check requirements, update skin and validate HP/MP. ItemInventory::_EquipItemToSlot 0x400634 validates slot/index/equippability, returns unchanged for the same instance, removes conflicting hands for a two-hander, and splits stacks. Reused source stance consumer audit: GetAnimStance 0x3a53e0, CSIdle::OnFocus 0x3c3020, CSMove::UpdateType 0x3c0f18 consume CURRENT equipment, not startup equipment.
- Expected: an admitted equipped item updates the same hero's slots, gear sheet, skin/attachments and source locomotion. Duplicate equip remains harmless, unavailable/requirement-blocked item stays unequipped. Rebuild a missed equipment pair using the existing source resolver and bind its bank on the same session; include source item models in the future package.
- Focused test plan: real hidden-window short clicks on right-hand axe EQUIP, duplicate EQUIP, UNEQUIP and AUTO-EQUIP; visual equipped name/weapon plus logs and exit 0. Repeat with a distinct stance (staff or two-hander) to check the live clip consumer. Drop a bag item to the real world store, close menu and walk/pick it up, then equip its generated instance. Run equipment gates/atomicity and source library tests plus full ctest. Existing torso two-resolution and latch five-run checks are reused, not rerun.
- Uncertainty: video v1.0.3 and recovered Android target v1.0.2 may differ. No direct original axe/equipment-to-locomotion footage identified; these are source-backed tests. PC short-click latch is a deliberate input adaptation. Missing package model is packaging, not a guessed original equip rule.


## Completion — 2026-10-11

### Implementation
- Input dependencies: `e06a934e` = B103 private-desktop click injector and WM_MOUSEMOVE/WM_LBUTTONDOWN client-position tracking; `8b5bfcee` = B102 left-click press latch. Keep both. No temporary DH_CLICK_DEBUG diagnostics occur in this branch.
- `592dacf7` replaces the unverified `117c2b34` main.cpp fallback. The old fallback reproduced a real abort: `Source stance selection: Locomotion named clip alias collides with existing visual bank` (resume-axe.log). Rebuilding against the merged config also would not load a newly needed stance clip into the existing visual.
- `RuntimePlayerLocomotionLibraryRequestV1::preload_all_equipment_stances` is opt-in. With it enabled, source ItemTable main/null and null/off tuples resolve every reachable GetAnimStance result; only one program per source stance is prepared, with the caller/hard budget still enforced. No item/category/class-specific replacement is used. The library retains the actual tables/constants and resolves current IDs on select_exact before choosing a prepared stance bank; the selection receipt preserves those actual IDs. Main opts in with the existing hard bound of 8. Knight uses 5 programs. All named clips are merged BEFORE loading CharacterVisual, and the unchanged same-session locomotion binder can find them after equip, unequip and pickup.
- Strict existing current/null-only mode retains its previous exact-pair contract for other callers. Invalid IDs, insufficient bounds, collisions and failed builds remain atomic.

### Original match statement
- **Matches video:** original P1 509-523 s equipment page arrangement, selecting an admitted item and updating equipped label/checkmark are preserved; blocked items retain red X/disabled EQUIP. Direct sequence inspection is documented above. The new axe and two-hander visuals are runtime evidence, not original-video timing claims.
- **Matches IDA:** menu entry/admission, requirement/class gates and equip/unequip/auto-equip gear/skin refresh preserve the recovered paths listed above. Locomotion now consumes current equipment through the existing source GetAnimStance resolver rather than failing for a pair acquired after startup.
- **Not verified:** original continuous axe/two-handed equip-to-walk timing; audible menu cues (runs are silent); powered/generated item persistence and descriptors (the baseline CharacterState InventoryItem only stores instance_id/definition_id/quantity). No new power rule was guessed or implemented. PC short clicks are the declared input adaptation.

### Focused and full tests
- Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name fix102 -Jobs 2` -> build exit 0.
- Strict native source-bank test: `run_runtime_player_locomotion_library_v1_tests.ps1 -AssetRoot <scratch>/pkg/assets` -> PASS. Added tests prove all-equipment banks for Knight/Rogue/Mage, every selected clip already present in the initial visual configuration, exact current ID receipts, invalid acquired-ID rejection and insufficient cache budget preserving the previous bank. Final output: `.local-inputs/claude-v20.1/fix102/runs/library-test-final.log`.
- Full ctest: 167/168 PASS, 16.99 s; only known worktree `session_skill_binding` failure. `.local-inputs/claude-v20.1/fix102/runs/ctest.log`. No unrelated failing test is omitted.
- `git diff --check` passes. No runtime behavior changed after the frozen EXE below; the final later edit extends tests/report only.

### Frozen real-EXE verification
Frozen file: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-v20.1/fix102/pkg/dh-b102-592dacf7.exe`.
SHA256: `ABB11B47F4D3A3D0ACC1F916E72020DFF5162E161C75B9921FF5024FAB8BD7F5`.
Every run used quiet_click.ps1, hidden private desktop, DH_AUDIO_SILENT=1 and scratch assets. The generator now gives each fresh test its own profile, game-save and world-save paths. Accepted v20 was only read. These are worker production-path checks; root must check the integrated frozen preview too.

| Run (all under .local-inputs/claude-v20.1/fix102/runs) | Observed result | Exit |
| --- | --- | --- |
| fixed-axe.log / .png | 10 ms mouse EQUIP changes Longsword01/main 664 to BattleAxe01/main 31, one attachment; avatar displays axe and Useless Hand Axe. Duplicate EQUIP keeps one slot/attachment. | 0 |
| fixed-unequip-auto.log / .png | Real EQUIP -> UNEQUIP gives main -1, count 3, attachments 0; duplicate UNEQUIP stays empty; real AUTO-EQUIP chooses source Longsword01, count 4, attachments 1. | 0 |
| fixed-twohand2.log / .png | Real EQUIP on TwoHandsSword01 changes main to 1178, source stance 1; avatar shows the two-handed pose and (2H) Useless Cleaver. Binding checks the same live visual's source animation range successfully. | 0 |
| fixed-blocked.log / .png | Staff01 REQ 8 ENG selected on Warrior ENG 3: red X/dark EQUIP; real EQUIP click leaves main 664 and blade attached. | 0 |
| fixed-pickup.log / .png | Real DROP sends the bare axe to world store; scheduled pickup at frame 1400 runs the production receiver (picked=1, inventory 4->5, store 0). Reopened menu then real row/EQUIP clicks select the acquired axe; main 31/one attachment. Pickup itself used --pickup-frame, not walkover. The source drop preserves original instance_id bag-0-BattleAxe01. | 0 |
| b081-potion2.log / .png | Supplemental root-requested B081 screenshot: category9 Potion0 singleton rows have no red X. Does not establish a five-potion stack label. | 0 |

The initial resumed torso checks at two resolutions and five short-click trials are reused; unchanged long campaign scenes were not rerun. The early `fixed-twohand` attempt clicked before the menu was ready and never reached equipment (not counted as a pass); repeating with an 8 s first-click delay produced the verified fixed-twohand2 result. No equipment/model/animation errors occur in the successful logs. They retain the unrelated known B087 missing quest_marker_blue model warning because the scratch package is based on v20.

### Package requirement and caveats
- v20 only shipped three starting weapon models; axes and other equippable models must be added to the NEW preview. Copy missing `.local-inputs/visual-skin-owner-v6/weapons/*.bdae` to the new package `assets/models/`. There are 781 files totaling 11,955,072 bytes; all were copied ONLY into the scratch package's real models directory for tests. The existing portable content_path_candidates resolves source `data/3d/characters/prince/weapons/<file>` to `models/<file>` case-insensitively. Preserve existing files and accepted packages. No new torso assets were required in the original two-resolution check (same modular prince model).
- Complete 781-model source/target/size/SHA256 list: `.local-inputs/claude-v20.1/fix102/runs/package-equipment-models.json`. Three source models already exist in accepted v20 and all three match source hashes; preserve them and copy only missing targets to the NEW package.
- Tested axe resource: `mc_rweapon_battleaxe_01.bdae`, SHA256 `AE1BA9CD537D47F35E5D7063AD06D66C99713CE0E388FC1DF1159560872EC61F`; tested two-handed sword resource: `mc_rweapon_twohandssword_01.bdae`.
- Missing item resources still reject the existing equipment transaction and write its existing diagnostic. The concrete B102 asset omissions must be fixed in packaging; this patch does not substitute another item's art or conceal missing resources.
- Actual generated/powered equipment is a pre-existing schema/gameplay gap. The acquired-item check covers the real store and current bare-item implementation; it does not claim powers or generated descriptions/stats work.
- Ready for root's fresh read-only reviewer. Root owns shared main integration and the final package/acceptance; this worker does not remove B102 from the active bug table.
