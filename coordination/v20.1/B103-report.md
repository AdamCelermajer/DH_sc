# B103 report: mixed stat and skill allocation repair (in progress)

## 2026-10-11 investigation and test contract (before implementation)

Fresh repair owner `b103_mixed_repair`; reviewer `b103_review`; worktree fix103 fast-forwarded to reviewed integration 0f7bc40f. Historical partial investigation below is retained; B111 now owns and repairs the five Statistics sub-pages, and B102 supplies the short-click latch.

- Visual evidence inspected: P2 v1.0.3 chronological 386–397 second sheet `.local-inputs/claude-v20.1/fix111/resume/ref-p2-386-397.png`. At 391/392 Statistics has points2/Strength18, 393 points0/Strength20, 394/395 Skills points2, 396 points1, 397 point-allocation confirmation. This directly shows repeated stat spends followed by skill spending and one confirmation. It does not show No being selected.
- Reused reproduced failure: independent `.local-inputs/v20.1/b103-review/REVIEW.md`, frozen executable SHA256 B337A6EBB420F95CC8D82DFFB0713409FBE5B47539268EE111F89076473376FD. Normal 10ms clicks spend Strength then Hardiness and select No; stat returns to10/point1, but durable Hardiness remainsrank1/skillpoints0. Source consumer audit confirms eager skill saves and stats-only cancellation.
- Logic re-inspected: SWF stat release0x149de/first save0x14a1f and skill release0x21b7e/first save0x21bb9 both test AddedStatsThisTurn AND useSkillPoint false; this gates the first pre-spend save only. Both spend branches run afterwards, with no eager post-spend skill save. btnBack SettingBoth0xe6c8 and No0x34b86 reload then clear both flags and rerun Back. Yes0x349xx/0x34a0b saves before leaving. NativeReloadSkills0x43dbb8 resolves the controlled player; Character::ReloadSkills0x3a9db4 removes buffs, SG_Load32, AI_ReloadSkills, UpdateAllSkills, recalc1 and inventory requirements. PlayerSavegame::_Load0x464f4c mask32 loads PROP; AI_ReloadSkills0x3d8cfc invokes Character::SG_ReloadSkills0x3bbe2c→PlayerSavegame::SG_ReloadSkills0x467324→SG_Load8 SKIL. Thus both saved property allocations AND ranks/slots are restored, while GEAR is not reloaded. Stat native0x43eb7c delegates IncStatStr0x3bd810/Dex0x3bd708/End0x3bd600/Nrg0x3bd4f8, each requires positive Stat_Points148 before debit/credit/recalc. Skills retain the B096 exact class-position, point, level and cap admission.
- Expected implementation: shared reusable stat/skill visit, save authoritative pre-allocation state once, mutate the actual live Session in memory, prompt for either allocation, Yes saves the whole staged character once, No restores saved property contributions plus ranks/slots and recalculates with current gear. Save failure retains staged memory/confirmation and durable baseline; no invented passive-effect expansion.
- Focused verification defined now: actual source Session mixed both orders, repeated stats/skills, Yes/No, zero/refusal/duplicates, baseline/Yes/No failed persistence retaining both allocations, identity mismatch, re-entry; normal hidden quiet 10ms mixed cancellation and confirmation plus save reload/visual screenshots. Reuse reviewed stat-only/zero runtime evidence; full ctest and frozen executable hash after repair.
- Uncertainties: native v1.0.2 vs video v1.0.3; No result not visible in reused footage but explicit native/SWF source. Existing Yes/No host confirmation art differs from original green check/red X; keyboard profile input is a PC adaptation. Passive effects absent in the inherited host and original inventory requirement rejection are separate gaps; this repair restores the source sheets/effects that the actual allocation providers publish.

## Mixed-allocation implementation and isolated tests

- `StatTrainingVisitV1` now owns one optional CharacterState baseline, the saved PROP sheet, the exact Session owner, four stat counts and a skill count. `begin_allocation_visit_v1` writes the common pre-spend save once after admission and before mutation; neither page overwrites it on later spends. The menu skill route retains all B096 source probes and increments only the accepted skill count. All menu-open paths clear the common visit. Existing close guard now prompts for stat-only, skill-only and mixed visits because `has_staged` covers both.
- Yes atomically persists the canonical staged character and clears only on success. No restores the baseline saved PROP contributions and SKIL ranks/slots, rebuilds the authored base, recalculates with the current gear/facts, mirrors stats/points/vitals into the same CharacterState/actor, and clears on successful persistence. Publication and persistence failure roll back the staged sheet/state/visit. Current gear and unrelated character metadata are retained. The inherited host writes the reverted character after No; original No only reloads its existing first-save baseline. That extra host persistence remains explicit and its failed-write branch is tested.
- The legacy direct stat-only helper accepts batches without a baseline for its existing unit consumers; the production menu always captures a baseline. No second gameplay allocation path is introduced. Skills casts/menu queries already read the same saved ranks; the inherited provider has no extra passive-effect cache to restore. Wider passive-effect expansion is outside B103.
- Focused source tests extend `runtime_skill_session_training_v1_tests`: both orders, two Endurance/Energy spends and two Hardiness spends, first save unchanged on disk until confirmation, Yes/No reload, failed prefix/Yes/No, wrong identity, zero points, duplicate confirmation and a new visit. Actual Windows file locking denies atomic replacement while keeping the pre-visit profile readable; both failed Yes and No preserve both live allocations and durable baseline. The prior stat suite remains unchanged and passes.

## Frozen normal production verification

Frozen EXE `.local-inputs/v20.1/b103-mixed-repair/frozen/dh-foundation.exe`, SHA256 **30639359BDD02FA5E3E2B5A91ABE54903AECA11F2530BB7C895CF9408A466314**; base reviewed integration0f7bc40f plus this repair. Source diff/base receipt and all fixtures/scripts/arguments/logs/PNG/PPM/save matrices are preserved in that own scratch directory. Build exits0. No packages, user saves or live windows touched.

Normal Win32 WM_MOUSEMOVE/DOWN/UP releases use10ms holds (C key180ms); hidden private desktops, DH_AUDIO_SILENT=1, 960×640. No semantic menu-release, menu test point flags or forced menu-page frames. Fixtures are copied genuine schema3 source profiles with integral stat/skill points adjusted before launch; character identity and saved class-position ranks/slots are retained. Runtime source initialization fills live HP/MP, so baseline captures those actual live values rather than the older fixture's partial vitals. These are diagnostic point fixtures, not a natural-level-up claim.

| Normal case | Observed production result and durable save |
|---|---|
| stats-first-no | Str10→11 then Hardiness0→1; shared confirmationstaged2; No frame1016; Str10, statpoint1, Hardiness0, skillpoint1 |
| skill-first-no | Hardiness0→1 then Str10→11; shared confirmationstaged2; No frame1019; same restored result |
| stats-first-yes / skill-first-yes | Shared confirmationstaged2; Yes saves Str11/statpoint0 and Hardiness1/skillpoint0 together |
| skill-only-no | Hardiness0→1; duplicate/zero upgrade refused; confirmationstaged1; No restores rank0/skillpoint1 |
| stat-only-no | End8→9/maxHP165→169 and Energy3→4/maxMP27→31; third stat refused; No restores End8/Energy3/maxHP165.09765625/maxMP27.25 and statpoints2 |
| zero | All four native stat actions refused; no allocation or confirmation; points0 and original attributes preserved |
| re-entry / fresh-baseline / reload-no / reload-yes | Menu reopens correctly after completion; fresh processes from actual No/Yes profiles preserve both outcomes |

All seven allocation jobs and all three fresh/reload jobs printed actual game `exit=0`. Inspected chronological captures: original failed review mixed/f850; repaired stats-first-no/f1450 Statistics10/point1/Attack69/R-Dam13–17 and/f1750 SkillsHardiness0/point1; skill-first-yes/f1450 Statistics11/point0/Attack72/R-Dam14–18 and/f1750 Hardiness1/point0; skill-only-no/f950 shared confirmation. Fresh process reload-no/f450/f750 shows both refunded values; reload-yes/f450/f750 shows both confirmed values. These directly verify UI state, saved identity/rank mapping, derived stat effects, and normal integrated allocation paths. Source-backed No semantics match the observed repair; original No footage remains unavailable.

Test receipts: pre-lock focused CTest2/2; actual-lock test initially failed at re-entry in full parallel CTest, then passed independently and8 consecutive focused repeats after adding failure diagnostics (no gameplay code change). That initial failure is retained honestly; its cause is not yet proven. Full CTest completion/recheck is pending; reviewer must not treat the worker runtime receipt as root package acceptance.

## Historical partial investigation (superseded where noted above)

### Root cause (what was established in the initial session)
1. The five icons on the Statistics page (btnStatisticsTab/Defence/Offense/Magic/Recovery, authored in sprite279 of the character-menu SWF,
   authored-actions.txt 0xf6ba-0xf9aa) have NO hit zone and NO handler in our port: `Presenter::hit_test` only knows the six top tabs,
   close and map controls, and `SourceCompositionV1::release` on the stats tab only resolves the four `+` buttons. The original handlers
   call NativePopMenu / NativePushMenu(menu_CharacterSheetStats|Defense|Offense|Magic|Recovery) + sound MenuSelect. So clicking the icons does
   nothing: this is a missing feature path, not a hit-test bug. NOT FIXED in this session (needs the Defence/Offense/Magic/Recovery
   sub-page art+text, which our single combined Stats page does not have).
2. The `+` buttons are disabled in the original when Points left is 0 (deactivated frame, 0x1310a); the user's shot-21 shows Points left 0.
   Inferred: the `+` "not clickable" there is correct behaviour. With points they work (below).
3. Harness-only finding: `Window::cursor_position` used GetCursorPos, which is invalid for injected WM_* messages on a hidden desktop.
   Fixed (platform_win32.cpp): the window now records the client position from WM_MOUSEMOVE/WM_LBUTTONDOWN and uses it first. Real mouse
   behaviour is unchanged (same coordinates). This is a robustness/test change, not the user's bug.
Shared with B102/B095: the click path is main.cpp uiSurface.hit -> SemanticInput (menu_item) -> `uiInput.clicks` -> `characterMenuComposition->release`.
It works end to end for real injected clicks (below); B102/B095 should look at their page's release provider, not at hit-testing/input.

## Verification (real EXE, quiet hidden desktop, injected WM_MOUSEMOVE/LBUTTONDOWN/UP; tools/quiet_click.ps1, new)
- `C` key (WM_KEYDOWN) opens the menu (log "Character menu opened via profile input").
- With `--test-stat-points 5` (test-only flag), two injected clicks on the Strength and Dexterity `+`: Points left 5 -> 3, stats raised
  (frame .local-inputs/claude-v20.1/fix103/z.png). (Strength shows 1 because the test grant re-projects the sheet; test artifact.)
- Not verified: clicks on the stat sub-tab icons (no handler), real physical mouse (GetCursorPos path unchanged), Faery/Quest/Map pages.

## Video / IDA match
IDA/SWF: authored-actions.txt as cited. Video not consulted this session (rate-limit cut); the original sub-page screens were not compared.

## Commits
017e9f79 (injector + WM_MOUSEMOVE position), ab3507a2 (test-only flag, diagnostic detail). Build -Jobs 2 exit 0; ctest not re-run.
