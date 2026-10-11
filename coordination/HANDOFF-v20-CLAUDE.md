# v20 handoff back to Claude — 2026-10-11

The user stopped Codex implementation and requested committed worktrees and a full handoff. Continue from the current checkpoints; do not restart the completed investigation. **v20 is NOT shipped.** No final package, full final CTest, independent release verdict, accepted receipt, final main merge, or push occurred.

## Paste this prompt to Claude

Continue Dungeon Hunter 2 reconstruction in `C:/Users/adamc/Desktop/workspace/DH_sc`, branch `windows-foundation`. Read `coordination/HANDOFF-v20.md`, `AGENTS.md`, `docs/BUGS-AND-IMPLEMENTATION.md` section 2j and user inbox, `coordination/claude-preview161/BUGFIX-BRIEF.md`, then this entire handoff. The older HANDOFF-v20 branch/status information is superseded by this document and the worktree inventory beside it.

Finish ONE release called **v20**, package `windows-source-clock-v20`. The actual combined game code is in **`C:/Users/adamc/Desktop/workspace/DH_wt/v20int`, branch `v20/integrate`**, not main. It already combines p16.1, P17 and extensive B068/source fixes. Read that worktree's `coordination/v20/INTEGRATION-HANDOFF.md` and `INTEGRATION-report.md` before editing. Main currently contains Preview16 plus root reports/packaging tools. Do not audit old main game code as the v20 candidate.

The user additionally requested the authentic Gameloft launch buzz (B078), complete source-audited player death/Continue/checkpoint/world restoration (I024), and gate/teleport light effects (I028). They are part of v20; their investigation and helpers are largely done, but production verification is incomplete. Finish the specific remaining tasks below, build, run full CTest (only session_skill_binding may fail in a worktree), create the immutable package, obtain an independent verifier A/B against accepted Preview16, update tracker/RESOLVED only for demonstrated fixes, merge to windows-foundation and push **publish only, NEVER origin**. Subsequent user bugs become v20.1. The red-door flame may remain a known issue.

Follow standing rules: video first, then canonical IDA/callers/gates, evidence before behavior changes, explicit unknowns. Reuse valid evidence. Every user bug gets a fresh implementation session and reviewer; all cinematic issues share ONE B068 session. Own shared main/CMake exclusively through integration. Quiet hidden silent games ONLY through `port/windows-foundation/tools/quiet_run.ps1`, isolated saves/windows, DH_AUDIO_SILENT=1. Never touch the user's game, kill by name, scan all DH_wt, or run Python except `py -3 -I`. Remove a worktree junction with `cmd /c rmdir <wt>/.local-inputs` before removing the worktree. Preserve accepted packages and saves. Do not invent a video match when only source/tests establish behavior.

## Committed checkpoints and ownership

See `coordination/v20/WORKTREE-HANDOFF.json` for exact final HEADs and status. No worktree was removed. Relevant paths under `C:/Users/adamc/Desktop/workspace/DH_wt/`:

| Worktree | Branch | Status at handoff |
|---|---|---|
| v20int | v20/integrate | clean 948f44a8; WIP checkpoint and INTEGRATION-HANDOFF.md committed |
| fix068 | fix/b068 | clean 48c721f8; sole cinematic owner, final condition correction report committed |
| b078 | codex/b078-gameloft-audio | clean a0062e71, media replacement ready |
| v20death | codex/v20-death-flow | clean afd9fca1, helpers d790d51c independently approved |
| v20gatefx | codex/v20-gate-teleport-fx | clean 60e60e85, manifest/evidence ready |
| v20doors | codex/v20-door-collision | clean 810f6a9a, collision source/fixture findings |
| p161int | p16.1/integrate | clean 2f0030e2; B069–B077 merged |
| p17int | p17/integrate | clean 955d486b; already integrated into v20int |
| p17fire3 | p17/fire3 | clean e447e668; unresolved flame |

fix069–fix077 are clean existing source checkpoints, already integrated. p16lifecycle2's apparent tracked change was line-ending-only; staging normalized it and left it clean, no new semantic commit needed. The unrelated detached `generic-level-loader` worktree contains large pre-existing work from another session; it was deliberately left untouched. Main's untracked `docs/ida-fidelity-audit-2026-10-11/` and `tools/texture_lab/` also belong to other sessions: preserve, do not blindly add them.

## What is implemented and what remains

### Integration and B068 cinematics

P17 conflict resolution and p16.1 integration are done. Compiled common/active-level script banks load on the normal path (15 common + 24 Witch scripts). Actor retention, logical dead state independent of HP, source spawn admission, full-width actor IDs/opaque FX anchors, and shared actor world transforms are implemented. Stable node-lifetime fix c853de9c passed actual-model pose/reload/foreign-token tests; the old graph replacement invalidated cached target-node pointers. A meaningful forced post-mutation deform-failure rollback test was requested but may still be pending.

Normal-bank eight-case diagnostic batch on EXE 6845E2EC… exited 0: opening played/SKIP/movement, Serpent, Captain, Witch arena, and two WitchActivate contacts. Captain source kill produced one reward/event and End34 with physical retirement. Serpent reached PutLimbus. E8F1 facing publication fix corrected the hero motor overwriting scripted yaw: Serpent hero is visible; opening recheck unchanged. These are intermediate tests, not a release verdict.

Opening cage moth/halo/dialog speakers/single title and skip-to-tutorial have evidence. Read `fix068/coordination/claude-preview161/B068-report.md`, `B068-Act1-cinematic-inventory.md`, and main `coordination/v20/CINEMATIC-CALLERS-audit.md`, `EVIDENCE-review.md`. The inventory has 142 scripts over maps001–009, but not all are Act1 (Abbey quest rows can be Act8). Later-map caller probes are not normal-route acceptance. Complete appropriate Act1 cinematic coverage, including outcome/skip branches and later startup/routes; preserve explicit unobserved boundaries.

**Critical condition correction:** NPC05 invisibility exposed wrong condition consumer semantics. ObjectBase constructor 0x33f15c sets enabled byte138=true. InitPost 0x33ec0c initializes condition data without initially disabling. TestEnableCondition 0x33e6d4 with a false evaluated activation condition DOES SetEnable(false), despite an earlier mistaken “false is no-op” summary. TestDisableCondition 0x33e61c disables when its deactivation condition is false. Follow exact tested/list/profile gates and enable→disable order; empty/unknown conditions need source-backed policy. Separate spawn MeetCondition/probability gates from activation lists. Integration has a lifecycle correction; **audit it against this final clarification before accepting**. Verify NPC05 visible for the actual BeforeKillWitch quest state, with startup/flip/empty/unknown/ordered tests and opening regression. Do not infer Disabled means pose-stop: GameObject Disabled0x38ba04/filter flags and Character Disabled0x3a5974/selfFX do not directly CmdStop.

**Witch camera framing is unresolved.** E8F1 f60 still shows huge Rene center and Prince cropped left; reference P1 first two-person shot is **945–949s**, dialogue around950s (not953s). The actual v20 camera host adds rig root exactly once; the reviewer withdrew an initial wrong-main-code claim of missing root. The 12-channel camera template matches native defaults. Native default vertical FOV is 0.42963rad (~24.616°), aspect1.6678; do not widen FOV as a guessed fix. Source CameraLevel PlayAnim0x40f904 preserves applied zoom+144 while resetting requested/max136/140; Update0x410390 decays zoom by .75 during animation and applies child Z=-zoom*field148 plus offset/centering/ghost/damping/root operations. Current clip sampler zeros child instance offset. This is a real parity question, but isolated initial zoom isn't established and decay alone may not explain f60. Trace natural camera history/actor placement/timing before changing. Source PlayCamera0x460110 and SetCameraTarget0x460004; cam555 duration4966. Read the corrected EVIDENCE-review.

B068 commit3b821a9b adds source PlaySound/StopSound/ScriptSaveGame orchestration/helpers/tests; integration bindings may remain pending. ScriptSaveGame0x45c994 is **regular save**, not death checkpoint: unblockLevel(false), QuickSave(false), unblockcharacter(false), SG_Save even during skip. QuickSave0x3f059c has native lifecycle/dead/network gates. Do not alias it to checkpoint or invent readiness from URI/HP. Check integration handoff for current wiring.

### B078 authentic launch buzz

No runtime change required. Authentic converted movie and sidecar:
`DH_sc/.local-inputs/v20/b078/media/intro_v1.mpg` SHA256 `7F5EFD15…047F0118`, `intro_v1.segments.txt`.
Package destinations `assets/converted-media/intro_v1.mpg` and sidecar. Source authentic iOS MOV includes logo audio/animation; source Android head also has hum. Raw reference envelope correlation .95386 at offset1.93 supports source authenticity, not exact-version identity. Silent decoder/lifecycle, ignored logo presses, story skip and natural voice release passed on CA62EAFF… sourcea35b34f1. Ear verification remains unverified because runs are silent. See b078 report; independent final package checks pending.

### I024 source death, UI, checkpoint/world restoration

Helpers/tests implemented in v20death and cherry-picked into integration. Independent reviewer reran snapshot/checkpoint-zone/death suites on d790d51c and approved helper/API semantics. Four death suites and build pass per integration. **No genuine live death→Continue→revive verification and no completed main UI wiring.** Finish providers, UI, timing, world/body effects; don't report helper pass as feature complete.

Read v20death `coordination/v20/I024*` reports/source modules and main `NEW-WORK-review.md`. Exact source facts:
- IsDead is byte5193, not HP. Local death check0x376100 requires currentLevel, field304==38, local character, IsDead virtual+52, PMphase>0. Field304 is lifecycle state, not inferred map type. Offline0x375fe8 pushes FadeToBlack/mutesFX once. NPC CanRespawn is not local Continue gate.
- SWF dqhud_droid sprite742 has89frames, stage480x32030fps. Original fullscreen onRelease button jumps to Hide frame76; no invented Retry UI. Space is documented PC adaptation. Panel black/red exact geometry, original Fontin18 quote centered. Black remains opaque, quote fades; frame88 calls ReviveAllPlayers then PlayPreviousMusic. Native offline popFadeToBlack/pushFadeFromBlack and ReviveLocalPlayers. Sprite743 is exact 33frame post-revive fade (not instant disappearance).
- Main needs MenuLocalization/$player quote, HudGlyphFont/Fontin, modal captured pointer-up, unpaused30fps accumulator, overlay afterHUD, orderedframe88 callbacks/postfade. Actual NativeBody/physics/UI/main consumers must change, not only a shadow world.
- Offline revive: restoreFXvolume, loadcheckpoint, clearbuffs, nonorigin revive coordinates, CharacterRevive(true) HP/MP/body/floor/skills/Idle, MinimumPotionsOnDeath, levelmusic1000ms then separate previousMusic.
- CheckpointZone0x395f04: player local0 entry, changed key; RAWASM0x396034–0x396044 passes **false** to CheckpointSave. Zone authored position is key; CURRENT entering character position is revive coordinates. Main uses declaration placement[12..14], not box center.
- Selective checkpoint restores eligible existing named world objects, no create/delete; preserve player gear/skills/level/properties/faery/inventory/RNG/procedural seed. Player quest QEST init/defaults/load+sync are separate; OBJS player callback is SetVisible, NOT SG_Load. INFO level row is not seed.
- ObjectBase byte40 false by default; native Module/Trigger/Container/Character/Door/TriggerObject set true; authored skip_checkpoint clears it. Zone/AreaZone/TriggerZone are NOT Trigger: base false. Lever/AnimatedObject unresolved failclosed, no guessed classname eligibility. CharacterDeserialize source state/dead/body/CanRespawn branches are implemented and tested; current physical-body presence is not persisted.
- If LevelSavegame absent, source skips its load/ReInit; don't invent calls. Portable selective backend allowed (no need original object graph), retain source externally visible behavior/readiness.

Video: no death observed in reviewed P1/P2 grids. Full blackdetect threshold scan logs under `.local-inputs/v20/death-video-scan/` decoded39947 P1 +27482 P2 frames; only opening logo meets95%black/.5s. This is NOT proof no death or a visual-match claim.

Legitimate test fixture: normal Camp/Bogwomp attacking, 1500frames HP64.8672/165.098, likely3000–4000frames to lethal. Use fresh private args/save and real hits, not synthetickill. First establish a genuine checkpoint with distinct zonekey/playerrevivepos. Test Continue once/duplicate, modal input, source fades/music sequence, actual body restoration and selective world/quest/profile preservation. Owner stopped without launching this test.

### I028 gate/teleport effects and I027 collision

v20gatefx report/manifest commits60e60e85 etc are clean. Correct full-width actor anchor IDs, native IsDead/deleted rather than enabled check, lazy named dummy/waypoint generation, direct source XYZ rotation and abs(scale)<.0001→1 normalization are integrated/source-audited. Keep unknown anchors failclosed and session reset tests.

Set132 teleport shockwave original assets are identified. Compiled Door_Teleport sequence: FX132, sound604, Wait0, moveLocalPlayer to authored waypoint, Wait0, FX132. Wait0 is nonblocking in source (0<0 false); do not invent a frame barrier. Door states1/3(open/opening) suppress closed-zone trigger;0/2 admit. BlueDoor starts open; normal Intro_Witch command63 kind55 closes it.

6559D139… diagnostic pair: opened suppresses teleport, replay actual compiled Intro_Witch63 closes then ordinary authored zone starts it; both FX132 dispatched and player moved to `(5515.36,-12643.6,88.1133)`. Both exit0. This combines diagnostic placement with real production zone, NOT natural traversal. Brightness still cannot uniquely attribute FX/lifetime. Log/captures `.local-inputs/v20/integration/i028-compiled-bank-replay-6559/`.

Prisoner reference P1 612–622s orange gate,624/634–644s blue-white effect; NOT Witch room945s. Old7:59/8:03 stills belong to a different2:49:10 reference. Cage model `cs_swamp_intro_cage.bdae` and original SwampIntro set119 reflection / set120 explosion are pinned. Existing normal full opening c684 reaches commands101/107 around2600/2672 and shows shaft after120, but overlapping thunder/faery and no per-cue logs make attribution ambiguous. E8F1 300frame run onlysets116/117, NOT119/120.

Remaining: latest-build long normal opening with per-set target/anchor/generation/resolvedresource/playresult logs and adjacent frames2599–2602,2671–2809, plus temporal132 and reset/missingtarget branches. Do not merge prisoner cage and distinct merchantcamp firedoor identity. Red flame may remain explicit caveat.

Physical door source/filter/root-motion checks are integrated; actual closed/open paired fixture remains. Saved supported OPEN fixture start `(5203.18,-11704.4,87.2637)`, `--move-segment 10:160:-1,0`,170frames, room6/floor8 end `(3665.27,-12272.2,87.2637)`. Run named witch-closed-crossline was actually state1(open), not closed proof. Close using actual source command and complete marker/end/filter, prove closed blocks while floor support isn't the cause; open must cross. Check an intermediate rejected floor sample explicitly.

## Evidence inputs, builds and diagnostic identities

Canonical source: main `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`, `assembly-functions.asm`, vtables/RTTI. Source APK1.0.2 vs footage1.0.3 possible difference stays explicit.
Silent P1: `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`.
Raw P1/P2: `C:/Users/adamc/Desktop/dh2_video_research/video/part1_z_Zky7qQdYs.mp4`, `part2_cIAW38IfLxY.mp4`.

Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name v20int -Jobs 4 [-Test]`. BuilddirDH_wt/build-v20int. ToolchainPATH `.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin`. Final full CTest must run with no simultaneous game jobs; pacer is timing-sensitive. Build `[167/167]` counts actions, NOT tests.

Diagnostics under `.local-inputs/v20/integration/`, mutable/new source does not change copied EXEs:
- combined-target-stable-node.exe SHA `5D88EA4162BCDDD8543C93ED6FA565F936099021FEBAD7F90B5EC87722901DF3`
- combined-shared-actor-transform.exe SHA `6845E2EC1FF553E5345F573293F262C5DB5BE1F1B45B8B955C9A80655E2EC12B`
- compiled-bank replay EXE SHA `6559D139262F07E9E14557862FA7367EFAB3DB5F48A0AE263E0F916AE16474D`
- combined-e8f1.exe SHA `E8F1AF3639EF0F7552513099058F43E5B35499082ADBA0916EF534E87C009B4A`
All are diagnostic, some sourceHEAD+dirty diff; none is frozen release. Use integration handoff for newest rebuilt EXE.
Latest condition candidate `.local-inputs/v20/integration/condition-lifecycle-fb1/dh-foundation.exe` SHA `FB1EFF2FDD29E3E852F0B99015AA6614342258C119FFEDAEAFAF04BFA62A6271`: source37c617f8+WIP now committed948f44a8. True row52=3 and false row52=13 runs each2000frames exited0 before stop; captures were NOT visually inspected. Logs/summary in that folder. No owner game jobs remain active.
Accepted P16 `.local-inputs/windows-source-clock-v19-preview-16/dh-foundation.exe` SHA `3ACF50D2A249962156583C3F13CE010443F6AB65A94A7E0DBE423D71CF4D3D1D`. Scratch16.1 unverified, not baseline acceptance.

## Packaging and final sequence

Root tools committed on main: `coordination/v20/package_v20.py`, `accept_package.py`. Read them. `package_v20.py --inputs <main/.local-inputs> --exe <clean frozenexe> --metadata <json>` copies acceptedP16, rejects existing output, adds finite assets, enforces B076 dependencies, generates manifest/receipt. Metadata requires actualsource_commit/ctest/fixes/open/verification/readme. No package created. Draft README `.local-inputs/v20/README-v20-draft.txt`.

Staged diagnostic asset root `.local-inputs/v20/asset-stage/assets` (no EXE). Finite manifests:
- main `coordination/v20/P17-package-assets.json`267 records (MGX/MVX/index/common banks/actors/props)
- main `ACTOR-package-assets.json`28 original Darkwood model/statue clips
- fix068 `coordination/claude-preview161/B068-package-assets.json`11 (serpent FX, dialogs speaker table676bytes, Witch portal)
- v20gatefx `coordination/v20/GATE-TELEPORT-FX-assets.json` nested source_path/uri records, including latest119/120/cage dependencies
- two B078 media files above.

Last root `.local-inputs/v20/package-assets-preflight.json` had314 unique additions with source ANDstage hashes matched, **before latest expanded gate manifest**. Rebuild finite preflight recursively from latest manifests; source+destination ORsource_path+uri→assets/uri, casefolddedup only identicalhash. Do not recursively scan DH_wt. Four missing Abbey textures remain explicit unknowns, not fabricated. Preserve both original source/stage and diagnostic captures.

Package script removes legacy forced arguments only after normal-bank proof: `--condition-active RENE_FOLLOW`, `--source-root-scopes original-cache/data/scene/001_swamp.mlx`, `--campaign-commands original-campaign.xml`; retain --campaign-triggers. Fresh unforced Rene following passed; resumed CQPG>=13 false still needs test. Actual source condition13>queststate (fresh0true).

Finish production gates, clean commit, rebuild/fullCTest, stage/hashclosure, package frozen v20. Independent verifier already prepared checklist main `FROZEN-VERIFICATION-report.md` but has **no verdict/launch**. Resume a fresh independent verification using exactpackage/hash/cleancommit/fullCTest, P16 A/B, normal fixes/death/transitions/FX. Reject→fix with appropriate fresh bug sessions, preserve rejected rc; never overwriteacceptedpackages. Then update RESOLVED/tracker accurately, `accept_package.py <package> --sha256 <verified> --verdict ... --report ...` updates receipt+manifest without EXEchange. Merge v20/integrate to main while retaining root docs; push `publish windows-foundation` only.

Retain scoped caveats: incomplete B071/B072/B073/B075/B076 visual/interaction checks, B074 non-divergence, Rene ranged attack, boss combatI007, Abbeytextures, flame, ear/audio verification. Source reports distinguish investigation/helper tests/integratedruntime/release acceptance. Read tracker before assigning; do not erase user additions.

## Stop state

Codex feature owners were instructed to stop all new implementation and launches. No final work is authorized for Codex beyond committing/preserving and this handoff. Claude owns resuming. Exact inventory generated after owner checkpoints; unrelated concurrent work explicitly preserved. Nothing pushed.
