# v21.8 act1mage report: Act 1 played with MagePlayerBase

Branch `v21.8-act1mage`, worktree `.claude/worktrees/act1mage`, base `windows-foundation` 586efa0c, subst `M:`, build dir `C:/dh218/act1mage-build` (Release, dh-foundation.exe built from base, no code change). Harness and case folders are outside the repo: `C:/dh218/tools/run-act1.ps1`, cases `C:/dh218/cases/<case>`.

## Verdict

**Act 1 cannot be played start to finish with the Mage in this build.** Every segment up to the Mud Lord works with real keys. The Mud Lord fight is blocked by the package (the boss AI script is not shipped in the staged v21.8 package). With the original boss files overlaid, the Mage can damage the boss but dies to its spit in about three hits unless potions are used at the right time. The real exit from the Mud Lord area to Darkwood was not reached (see blocker 3). Darkwood itself loads for the Mage.

No code changed on this branch. All temporary diagnostics were reverted (`git status` clean). Findings are below, with the evidence case for each.

## Harness (what was run and what was a test aid)

- `run-act1.ps1` is built from the packaged `startup.args` of `windows-source-clock-v21.8-staged-magewire` (production arg set, `--campaign-triggers` kept). Changes: `KnightPlayerBase` becomes `MagePlayerBase`, `Longsword01` becomes `Staff01`, the longsword `--attach` is dropped, and the save is the Mage profile.
- Starting profile: `C:/dh218/seeds/mage/profile.save` (h-mage, MagePlayerBase, 141 HP, 78.5 MP, Staff01 equipped, ColdRay on key 2, Faery on 4, 5 potions).
- Real keys only for play: Enter (SKIP and caption advance), Space (attack, context, caption tap), Tab (target), E (transition), 5 (potion), 2 (ColdRay).
- Test aids used, all recorded:
  - `--menu-actions` (menu start), `--skip-boot`.
  - `--player-to ZONE@FRAME` and `--player-to X,Y,Z@FRAME` (teleport).
  - `--follow-actor SwampKing@2000-6100:350` (keeps the hero at 350 units from the boss).
  - `--test-player-hp N[@FRAME]`. Caveat: N above the Mage max (141) corrupts the vital invariant, see blocker 9.
  - `--capture-at` frames.
- Seeds: `C:/dh218/seeds/mage-witch-done` (cave save after the witch fight, from case a7) and `C:/dh218/seeds/mage-after-boss` (Swamp save after the boss, from case b9).
- Asset overlay for the boss runs: `C:/dh218/overlay-mage-assets` = the staged-magewire package (hard links) plus the 10 Mud Lord original files from `.local-inputs/assets-extra/android/data` (same files as `C:/dh217/assets-overlay`, the Knight's overlay).
- Injector gotcha: clicks run in list order, not time order. Events must be time-sorted or later presses fire late (found in case a1g: potion presses fired at t=90).

## Per segment

| # | Segment | Result | Evidence |
|---|---|---|---|
| 0 | Boglands prison intro, Swamp start | **WORKED** (Enter skips the cutscene) | `a1b-s1/run.log`: `SKIP fast-forward` at frame 110, then hero at 791.8,-416.2 in control. Frame `a1b-s1/f1000.png` shows the SKIP control during the cutscene. Frame `a1c-intro/f900.png` shows the Mage with the staff in the prison area. Quest 50 banner at frame 111. |
| 1 | Swamp field combat (lizards, moths) with staff basic attack, Tab, potions | **PARTIAL** | Tab selects in 5 of 5 presses during free gameplay (`Source target command`, e.g. `a1-s1-swamp` frame 79, `a1d-fight` frame 393). Staff clip `skill_dh2_prince_mage_wand_attack` starts and reaches its `do_skill` marker. Bolts (row 20) reach targets: kill of lizard 12512... at frame 623 (`a1d-fight`), `xpRecipients=1 xp=5`. Bolt impacts at the caster position: see blocker 4. Frame `a1-s1-swamp/f170.png` shows the bolt on a Bogwomp, 5 potions, ColdRay on slot 2. |
| 2 | ColdRay (skill 2) in a real fight | **WORKED** (damage low) | `a9-coldray/run.log`: `Source skill key=2 slot=0 frame=588 skill=ColdRay MP=53.5` (MP 78.5 to 53.5, cost 25), second cast at frame 771 (MP to 28.5), third at 1126. `Source skill target FX name=elem_dmg_02_water`. Ticks 7.2 and 8.0 (`Damage ... marker=do_skill`). The free staff bolt does about 23 per hit. |
| 3 | Quest zone `_prim_WitchQuestStart` | **WORKED** | Entering the zone (x -11821,y 12247,z 255) gives `Quest banner kind=NEW QUEST row=52` (`a6-witch-quest`, `a7-witch-quest`). Merchant camp cinematic follows; needs Enter. |
| 4 | Swamp to Bogwitch Cavern (E at `_prim_ExitLevelZone`) | **WORKED** | `a1e-cave/run.log`: `Transition confirmed key=E zone=_prim_ExitLevelZone target=SWAMP_CAVE_WITCH_A`, `Session start ... 002_swamp_witchcave`. Cave exit back to Swamp: `Transition confirmed key=E zone=_prim_ExitLevelZone_001 target=SWAMP` (same run). |
| 5 | Witch fight (B154) | **WORKED with potions; DID NOT WORK without** | Intro via `--player-to 6346.94,-10602.9,60.779` triggers `Intro_Witch`. Without potions (`a1f-witch`): Mage dead at frame 2910 (witch hits 17 to 19 about every 1.3 s); witch still 400/400 at reset. With potions (`a1h-witch-potion` at 2292/2770/3248; `a7-witch-quest` at frames 3545/4025/4981): `Source death reward frame=4252 victim=witch xpRecipients=1 xp=41 selected=2 spawned=2`, `Quest event kind=0 property=435 applied=1`, `Quest banner kind=QUEST COMPLETED row=52`, return cinematic captions (Celeste: "Her heart... it turned into an orb!", a7 frames 2663 to 2790). |
| 6 | Back to the Swamp, Swamp Serpent intro, lever, Mud Lord | **DID NOT WORK as packaged; PARTIAL with the overlay** | The cave exit `_prim_ExitLevelZone_42` (E) returns to Swamp (`b3-boss`). Serpent intro zone `_prim_TriggerZone_17` (`-15437,12000,-112`) starts `Swamp_Serpent_Intro` (Enter skips it, `b1-boss-from-witch/f1500.png` shows SKIP). Packaged (no overlay): `[boss-ai] profile=SwampKing swampking_core owner unavailable: Original resource not found: data/scripts/ai/swampking_core.luac` (`b1`, `b2`), so the boss has no AI. With overlay (`b3-boss`): `swampking_core owner bound`, `dynamic level host_level=1 current_range=1..4`, `awake`, `intimidate`, `DoSkill(2) SwampKingAttack3`, spit projectiles, `phase 2 hp%=73.4`, dive postponed. Lever: no lever position was found in the logs; the Mage pressed Space at the sealed-door area (`-12136.77,11990,258`) and the door state was not checked. Frame `b3-boss/f3000.png` shows the Mud Lord head at the top and a white-blue AOE on the Mage. |
| 7 | Mud Lord kill | **PARTIAL** | Real play (`b3-boss`, potions at fixed times): spit rolls 46.7 (HP 141 to 57) and 47.8 (HP 57 to 9.6), then the Mage is dead at frame 3824. One potion at 9.6 HP (frame 3371); the later potion presses were refused at full HP. God mode with follow (`b5-boss-god`, `b9-darkwood`): Mage hits the boss (28 bolts with `target=14582...`), boss `DESPAWN death end ... frame=6078` (b5). Boss XP not evidenced: death reward refused (blocker 9). |
| 8 | Mud Lord outro and quest 50 completion | **PARTIAL** | `Quest banner kind=QUEST COMPLETED row=50` at about frame 7019 (log line 6994 in `b5-boss-god`, 7023 in `b10-darkwood`), with `Condition flip IsAfter_Swamp_Escape false->true`. Outro captions need Space taps. Hero held at `(-18000,12000,0)` from about frame 6000 to 6700 (outro), then moves. |
| 9 | Swamp to Darkwood (E at `_prim_ExitLevelZone_001`) | **DID NOT WORK in the real route** (blocker 3) | After the condition flipped, `--player-to` reports `placed=1` but the hero position stays at `-18462.8,11536.2` (`b8`, `b9`, `b10`). The prompt `Gothicus Darkwood` appears for one frame and hides. A restored Swamp session does accept teleports (`m2-tp-after-boss`: hero at `-20810.1,10892.6` and the zone enters), but the condition is false in that save, so no prompt. |
| 10 | Darkwood load | **WORKED (direct load only)** | `m4-darkwood-direct`: `Session start ... data/scene/003_darkwood.mlx`, `Level triangles=40142`, `Collision triangles=1827 floors=24`, Darkwood location zones fed. Frame `m4-darkwood-direct/f600.png` shows the Darkwood forest, the Mage with a bolt, and the HUD. Notice: `Unknown authored condition IsDuring_Darkwood_Ring`. |

## Blockers, ranked

1. **Package: Mud Lord AI and assets missing** (blocks the Mud Lord for every packaged run). `windows-source-clock-v21.8-staged-magewire` lacks `scripts/ai/swampking_core.luac`, `scripts/skills/swamp_king_attack_1..4.luac`, `swampking_atk_spit.luac`, `3d/projectiles/elemental_bolt_water.bdae`, and the 3 Swamp King FX BDAEs. The 10 files are in `.local-inputs/assets-extra/android/data`. Fix: add them to `coordination/v21.8/asset-selection-v21.8.json` and restage (the same list as `C:/dh217/tools/make-overlay.ps1`).
2. **Mud Lord survivability vs the Mage (balance, not verified against the original).** Spit hits 46 to 54 per hit at dynamic level 1 (boss 700 HP, Mage 141 HP). Staff bolts do 20 to 25 about every 1.3 s (clip-bound), so about 37 s of hits are needed. Three or four spits kill the Mage. No reactive potion logic exists in the harness; scheduled potions failed. The B155 notes give the Knight 169 HP at level 2 (melee, not checked in this session). Needs an original-based check of the boss damage formula and the Mage's HP/damage at the same level.
3. **Gothicus exit not reached after the Mud Lord outro.** In the same session the hero does not move on `--player-to` after the outro (`b8`-`b10`), so the exit prompt cannot be reached. Not isolated: the hero is in limbo at `(-18000,12000,0)` during the outro (frames about 6000 to 6700), and the restored session does move (`m1`, `m2`). Next step: check the outro end / Limbus state restore for the Mage. Not checked: whether the Knight run hits the same state.
4. **Bolts contact at the caster position.** The sensor radius is 162 game units. A target 176 units away is hit at the caster's own position (`a1-s1-swamp`: `Mage projectile impact id=1 ... pos=-6852.64,860,255 damage=22.0`; the target was at 1036.01, so 176 away). Damage happens before the bolt leaves. In-flight contact was seen only in earlier runs (magewire `mw-staff1`).
5. **Space with no target keeps launching bolts that expire at one point.** `a1d-fight`: 15+ launches with `target=0`, each `kind=1` expiry at `1190.34,1527.36,255`. Whether the original fires an untargeted bolt is not established here (inference: the staff attack should fire forward; the expiry point does not change).
6. **ColdRay per-tick damage is low.** 25 MP for 7 to 8 damage per tick (`a9-coldray`); the free staff bolt does 23. Balance unverified against the original ColdRay row.
7. **Potion restores to full HP in every use observed** (4 of 4: HP raw 2467, 26178, 17284, 26660 to 36096). The original Potion0 heal amount was not traced, so this may be correct or a deviation.
8. **Witch without potions is lost for the Mage** (a1f: dead at frame 2910; witch 400 HP). With 3 potions the Mage wins (a7). Balance unverified.
9. **Test aid artifact: `--test-player-hp N` with N above the Mage max (5000 vs 141).** The death-reward vital check refuses XP (`current/max vital differs ... current_raw=1162907 actor_max_raw=36096`, `xpRecipients=0`, `b5-boss-god`, `b9-darkwood`). This is a harness problem, not a Mage problem: `a1d-fight` (no test HP) gives XP 5. Also, the test HP applies only to the first session (cave runs show 141).
10. **Quest gate for the witch (route fact, not a bug).** The witch kill event is applied only after quest 52 is accepted by entering `_prim_WitchQuestStart` (debug print in `a5-witch-god`, before the revert: row 52 `state=3` available, kill `applied=0`). A cave-only route skips the quest and no return cinematic follows.
11. **Tab in cinematics.** Tab pressed during the cave intro (`a1f`, `a7`) selects nothing; this is expected.

## Tests

- No code change on the branch; the investigation diagnostics were reverted (`git status` clean at 586efa0c). The runs used builds whose only differences from HEAD were stderr diagnostics (DEBUG prints in the death-reward and quest-runtime code, and a longer vital error text); none changes game behaviour. The final build was rebuilt from the reverted HEAD.
- Full CTest on the reverted HEAD (Release, llvm-mingw, `ctest -j4`, log `C:/dh218/act1mage-ctest.log`): **100% tests passed, 0 tests failed out of 202**.

## Remaining work (needed for a Mage Act 1 pass)

1. Restage the Mud Lord package files (blocker 1).
2. Find the cause of blocker 3 (the Mage cannot be moved after the Mud Lord outro). Until fixed, Act 1 is not finishable with the Mage.
3. Calibrate the Mud Lord damage and the Mage's stats and potion use against the original (blocker 2). Add a reactive potion rule to the harness (potion at HP below 40) if the test is to be repeated.
4. Bolt contact geometry and no-target bolt behaviour (blockers 4 and 5), checked against the original and frames.
