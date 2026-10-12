# B137: everything breaks after the E level transition (Bogwitch Cavern)

Owner: B137 session. Worktree branch `worktree-agent-a9b30d9193252b9eb`, based on 45a0c602 (v21.3). Root cause is ONE fault: the session entered by a level transition ran the diagnostic *direct bootstrap* instead of being bound to the saved profile. B141 (brown beast / green lizard, camera lock, a character with no level change) is NOT part of this report (the user corrected that; a separate session owns it).

## 1. Investigation (original behaviour + our code)

Visual evidence (user screenshots, directly viewed): chest/belt shot (`...6d29f0c6...png`): hero, priest, a belt model and white label "Imbued Belt"; the priest and swampy ground are exactly what the Bogwitch Cavern entry area looks like (reproduced below at f700/f1200 of `t10-fight-fix`), so these were taken in the cave after the transition. Skills shot: Battle Hardened rank 0, 1 point, Upgrade Skill. Death shot: hero lying, empty HP bar.

Original logic (reused from `coordination/v20.1/B106-report.md`, IDA NativeGoToZone 0x004422b8): the transition runs QuickSave + SG_SaveAllPlayer and LoadLevel; the *same* Character (profile) is restored in the target level. There is no "fresh/diagnostic" player at arrival. Therefore every profile-bound rule that applies to a Continue/menu session must apply to the arrival session too.

Our code (`port/windows-foundation/main.cpp`): `frontendStarted` is declared at the top of the outer session loop (line ~903) and is `false` for every iteration that is not entered through the frontend. The arrival block loaded the saved profile (`Transition character loaded`) but left `frontendStarted=false`, so the arrival went through the direct-launch branches: `equipmentStateInitialized=false`, `options.combat.selectedPlayerProfile=nullptr`, no "Selected profile source projection", and the diagnostic bootstrap (~line 2796) which regenerated the starter items from `options.combat.equippedItemIds` under **new instance ids** and pushed them into the shared `CharacterState` next to the saved ones. Result: two equipment bindings for one source slot (equipment rows 4+4, inventory 11->15), level/stats rebuilt from the class table (cave Stats page said Lvl 1, 165/165 instead of Lvl 2, 153/169). Everything that validates the whole profile then refuses (`runtime_equipment_binding_v1.cpp:108 "Shared CharacterState duplicates a source equipment slot"`, and the persistence validators), while gold (no validation) still works. This confirms the B138 hypothesis (item pickup goes through `Presenter::pickup`/`validate_character_state`).

## 2. Reproduction on the production path (before the fix)

Frozen EXEs: `.local-inputs/v21.4/b137/frozen/` (hashes below). Harness: `.local-inputs/v21.4/b137/tools/run-case.ps1` (+`ppm2png.ps1`), hidden private desktop via `port/windows-foundation/tools/quiet_click.ps1` with ordinary WM_KEYDOWN/WM_LBUTTONDOWN, COPIES of the accepted v21.3 package progress (`character.save` + seed files), assets read-only from `.local-inputs/windows-source-clock-v21.3`. Path: normal menu `btn_MENU_SINGLE_PLAYER` (Continue) -> Swamp -> E at `_prim_ExitLevelZone` (prompt "Bogwitch Cavern", scripted E through the real SemanticInput) -> cave arrival. Test aids used (flags only; no HP/dead/quest state injected): `--player-to ZONE@FRAME` (positions the hero; I made it go through the SetActorPosition owner because the profile-bound hero is owned by the Session body), `--player-to X,Y,Z@FRAME` (new), `--transition-key FRAME`, `--capture-at`. Combat/loot/potion/death/menus are ordinary physical input (Tab, Space, WASD, 5, C, mouse clicks).

Legacy arrival (`baseline+aid` 0AB6B068 / toggle build 7A10AC7C with the fix switched off):
| Symptom | Evidence (case dir under `.local-inputs/v21.4/b137/cases/`) |
|---|---|
| 3 Equipment tab dead | `t4-equip-cave/f1200.png`: Equipment tab click (620,30) leaves the Stats page; log `Equipment binding diagnostic: Shared CharacterState duplicates a source equipment slot`. Same clicks in a swamp-only session open Equipment (`t6-equip-swamp-control/f1420.png`). |
| 2 potion plays sound, not used | `t12-loot-potion-legacy`: `Source potion key=5 ... result=0 consumed=0 quantity=0->0 ... diagnostic=duplicate equipment slot: slot-0` + `Potion sound ... submitted`. |
| 1 loot: item not picked, gold ok | `t16-item-legacy-1/2`: `World item pickup id=ClothBelt01 reason=walkover outcome=7 picked=0 ... stacks=15->15 store=1` (item stays); `t16-item-legacy-3/4`: gold picked normally. Inventory rows 15 instead of 11. |
| 5 death: stays lying | `t18-death-legacy`: level-start checkpoint `not_saved result=5 detail=duplicate equipment slot: slot-0` retried every frame (631 times); `Death Continue (Space)` -> `Death host failed: No source checkpoint record exists for this level yet`. |
| 4 Upgrade Skill does nothing | `t21-skills-cave-legacy/after.png`: 2 points, click Battle Hardened then Upgrade: rank stays 1, points stay 2. Same clicks work in a swamp-only session (`t19-skills-swamp-control/after.png`, rank 2, 1 point). |

The user's saves are not corrupted on disk: every later save in the broken session is refused by the same validation (character.save stays at the transition-time 3178 bytes in all legacy cases). The breakage is the live session.

## 3. Implementation (smallest reusable change)

`port/windows-foundation/main.cpp`, arrival block (`if(!frontendStarted&&options.transitionEntryId>=0)`): after the explicit profile load it now makes the session profile-bound, exactly as the frontend route does:
- `frontendStarted=true; options.freshPlayer=true;` (the existing profile-bound branches then apply: selected-profile property projection, state-owned equipment, no diagnostic bootstrap, no Continue-restore because `transitionEntryId>=0`);
- loads the creation/loot/skill source owner and starts the startup RNG stream before combat init (`load_runtime_creation_source_v1` over the same source root the frontend uses, `frontendRandomState`/`initialRandomState`, diagnostic seed consumed once), because the profile-bound item-metadata refresh needs it from the first frame.
Test-aid only: `--player-to` places via `hostServices.actor_verbs.teleport` and accepts `X,Y,Z`. No guard (locomotion admission, checkpoint, validation) was weakened or bypassed. Commits on the worktree branch (see end).

## 4. Isolated tests

None added: the change is in `main.cpp` (not linked by any test target); the failing consumer is exercised only by the full session. No library file changed, so the 177-test CTest roster is untouched in code; a full CTest rerun was NOT done.

## 5. Integrated runtime and visual verification (final EXE, same harness, real E transition)

Final EXE `final2-E86ABDF9BA08.exe`, SHA256 `E86ABDF9BA08917A9F7FE42EEF639708CFCDCC9FE3FDDEF391E5697B4F77E1F8` (built `port/windows-foundation` Release with llvm-mingw clang++ 20261006, CMake 3.22.1/Ninja, fresh build dir `.local-inputs/v21.4/b137/build`, target `dh-foundation`). All results in `.local-inputs/v21.4/b137/cases/` (`run.log`, `run.args`, `physical-inputs.txt`, `exe-sha256.txt`, PNGs inspected).
- Arrival session log now shows `Selected profile source projection HP=153.184/169.195 ... prebind=1`, `Selected equipment initialization count=4`, no `Direct player`, no duplicate-slot diagnostic; Stats shows the real profile.
- Equipment (`t8-equip-cave-fix2/f1300.png`, `F-full-*`/`F-roundtrip/f1350.png`): tab opens, slot details open, Potions count shown.
- Loot + potion + Equipment + Skills in one cave run `F-full-2`: fight (Tab, Space), `World item target ClothBelt01`, walkover pickup `outcome=0 picked=1 stacks=11->12`; gold `F-full-1` `16->22`; potion with damaged HP `Source potion key=5 result=1 consumed=1 quantity=4->3 HPraw=22545->43314`; C -> Equipment page (`f2000.png`, Potions: 3) -> Skills page, select Battle Hardened, Upgrade Skill: rank 1->2, points 2->1, `Skills sound MenuSpending` (`f2430.png`). Skill points granted by `--test-skill-points 2` in the swamp session survived the transition save/load.
- Natural death -> Continue -> revive (`F-death2`): enemies kill the hero (no HP injection), `Death flow admitted 811`, Space `Death Continue 2380`, `Death checkpoint restored ... actorsRestored=34`, revive at the cave checkpoint, then potion `result=1` (HPraw 33679->43314), Equipment page responsive (`F-death2/f2800.png`). Level-start checkpoint saves again (`Source checkpoint saved zone=level_start`, seen in `v2-death-final` on the previous final build E-5AF8AA9A and `t17-death-fix`; the identical source apart from the asset-root line was then rebuilt as final2 and `F-death2` repeats death/Continue/revive/potion/Equipment).
- Round trip swamp -> cave -> swamp (`F-roundtrip`, two real E transitions): swamp arrival entry 4, `Selected profile source projection` each arrival, Equipment page responsive (`f1350.png`).
- Duplicate branch: E held six frames (`F-held-e`): exactly one `Transition confirmed`, arrival, no return (B106 behaviour preserved). Full-HP potion refusal remains legitimate (not retested; unchanged branch).
- Item labels/pick-up rules: see section 7.

## 6. Remaining gaps (honest)

- Direct-launch (`Play-swamp.cmd`) post-transition: the arrival no longer crashes and is profile-bound (`t23-direct-fix`), but I could not drive menus there (known B106 caveat: the direct-start HWND is replaced during travel, injected keys miss), so Equipment/potion/death after a direct-start transition were NOT physically verified. In the legacy direct path the starter ids were deterministic, so it did not show the duplicate-slot failure (`t24-direct-legacy`).
- Menu Continue after a transition save always starts the Swamp (profile level stamp ignored by the existing menu route, `v4-continue-cave-final`, world file `status=none`); pre-existing, not changed.
- Original screenshot sequence differs in RNG/time; I used teleports to the cave enemy/chest areas (aids only), not a walked route; the cave chest open/loot path was not run to completion (`t15-chest-fix` only started it).
- Full CTest not rerun; only `main.cpp` changed. Direct-launch first-session behaviour unchanged.
- Not verified: Faery/other pages after transition, F5/F9 in the arrival, a second death in the arrival after Continue beyond what `t17` shows (second death/Continue also worked in `t17-death-fix` on the intermediate build).
- B140: the Skills Learn/Upgrade click symptom is reproduced as caused by this fault (legacy `t21`) and works after the fix; the click/label code itself was not touched.

## 7. Item label / pickup (B138 coordination)

Reproduced: our post-fight cave shows the same white "Imbued Belt" over the dropped belt (`t10-fight-fix/f700.png`, `f1200.png`). Per `coordination/v21.4/B138-report.md` the original has no floating name (a transient HUD status line on pickup, fixed ~59% height) and pickup is walk-over (no key), which is what our walkover branch does; chest loot placement/label are secondary and were NOT changed here.

## 8. Artifacts

Build dir `.local-inputs/v21.4/b137/build`; frozen EXEs in `.local-inputs/v21.4/b137/frozen/`: `baseline-3582FC601B3B.exe` (45a0c602 source, 3582FC601B3B708F...), `aid-0AB6B068.exe` (+--player-to via Session owner, legacy arrival, SHA 0AB6B06890BE0831A678A8C4D3F0407483E81A444283908436BAE31698A18CBF), `toggle-7A10AC7C.exe` (fix switchable by env DH_B137_LEGACY_ARRIVAL, used for the before/after A/B only; SHA 7A10AC7C6E0C372AB42D79B01EEDD3711906DD3FE32BB122F2B204934AF52F65; this env switch is NOT in the final source), `fix3-409F6EC6.exe` (intermediate fix), `final2-E86ABDF9BA08.exe` (final).
