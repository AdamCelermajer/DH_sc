# v20.1 integration evidence

## Initial earlier-merged batch, 2026-10-11

Source: `v20.1/integrate` at `dfc67d8d`, clean at build time. Build wrapper
`p14_build.ps1 -Name v201int -Jobs 2`: exit 0 (1,042 Ninja steps).
Frozen test executable:
`.local-inputs/v20.1/root/dh-foundation-initial-integration.exe`, SHA256
`18708E183185B68C512807AEA448357A072A6B65BD76B033D06D168014D05CEB`.
Full PowerShell ctest, LLVM-MinGW DLL directory on PATH, timeout 120, jobs 4:
167/168 pass, only allowed worktree `session_skill_binding` fails; 12.33 seconds.
Full log: `.local-inputs/v20.1/root/ctest-initial-integration.log`.

These checks cover the already-merged B079/B080/B081/B082/B083/B085/B087/B088/B089
batch only. They do not establish completion of unmerged work or acceptance.

### Focused normal executable checks

Fixture inputs reuse the recorded bug investigations (original footage/IDA
locations are in their reports). Runtime arguments were normalized to accepted
v20 read-only assets and new isolated per-job working directories, save paths,
logs and captures. No campaign cinematic was replayed. Fixed step 1/60;
`quiet_run.ps1`, hidden desktop, silent output. All four runs exit 0.
Evidence directory: `.local-inputs/v20.1/root/initial-runtime/`.

| Scope | Observed integration result | Limits |
|---|---|---|
| B079 chest | Interaction starts frame 260; authored marker 467 ms opens at 289; container opened and four items delivered once at 303; hero returns from interaction at 332. Frames 250/270/290/310/330/360/390 show the pose, lid change and moving drops. Later Space gives an ordinary swing, no second open/loot. | Reuses source/video investigation; no fresh pixel/timing match to original, gold individual bone approximation unchanged, sound heard by nobody in silent run. |
| B085/B089 vase | Delayed visual run avoids startup quest banner. Frames 250/268/276 show selected vase and weapon swing; 292/320 show broken vase without ring. Marker hit 276, sound submission and opening/loot path occur once. A second Space at 360 does not re-interact or duplicate loot. | First run (40 press) passed logs but its banner obscured the visual result; retained, not counted as swing visual proof. Disappearance at the exact hit frame is not established by the sampled sequence; zero-stage source vase only. |
| B088 NPC | Row365 talk at 100 accepts quest53, NEW QUEST text appears. Icon log changes type3 to type-1 at120; frames300/650 show sword. Second Space at600 raises no second TalkToNPC event. | Campaign scripts are off, so dialogue start and nearby-monster competition remain unverified here. |
| B087 marker asset | Marker draw dispatch is present. | Accepted v20 lacks `quest_marker_blue.bdae`; log explicitly reports missing resource and fallback yellow is visible. Add original blue asset to NEW package and check it there. This is not a completed correct-colour check. |

No root code behavior change was made to produce these results. Original visual
and logic match statements remain those in the individual investigations;
integrated observations above must not be promoted to broader gameplay claims.

### Baseline preservation

All 3,640 accepted v20 manifest entries passed full size/SHA256 and file-roster
verification before these runs. Accepted EXE SHA256 is
`8BA1E537D33D4D4FD5B7DD9822C4CAB5080917FE2427A3903AB7C247827E8CCE`.
Record `.local-inputs/v20.1/root/v20-baseline-full-hash.json`.

## Reviewed input and training integration

Source389c88064c0b4a96178205031317ff78bb8c3ec8, frozen
`dh-foundation-input-training.exe`, SHA256
`950EA00E9B5F392C5D756311727E8D3A91683933142CA39C2AA41ACE88F0B1EC`.
Build passed; ctest167/168, only permitted session_skill_binding failure.
Evidence `.local-inputs/v20.1/root/reviewed-runtime/` uses fresh scratch saves,
silent hidden windows and real posted10ms pointers. Frontend Continue actions
are diagnostic menu actions and page opening uses the retained page-frame hook;
physical frontend navigation and natural level-up are not established.

- B096 success: level2 Hardiness rank0→1, points1→0, one transaction and duplicate
  refusal. Locked EARTHSTRIKE selection gives two refusals/no transaction.
  Copied success profile reload retains rank1/points0 with two further refusals.
  Inspected trained and reloaded UI captures. Passive detail effects remain a gap.
- B102 corrected axe-row fixture: main664→31 once with one attachment, duplicate
  unchanged; duplicate UNEQUIP remains main−1/no attachment; AUTO-EQUIP restores
  main664/one attachment. First wrong-row attempt selected Blade, preserved as
  a failed fixture rather than axe proof.
- Corrected two-handed row: main1178, stance1, one attachment on both equip and
  duplicate. Inspected f620 shows the equipped (2H) Useless Cleaver and held model.
- Blocked Staff row: main664/stance0 remains, f620 displays red X and disabled
  EQUIP for (2H) Flame Staff. Early wrong-row two-handed attempt also preserved.
  Pickup was verified privately with a hook, not a root walk-over test.

## Reviewed effects and cinematic core integration

Source586a358716cab4d781e7f5b7d8e5bc1a04466e1e includes reviewed B101 core,
B105 and B091/B094. Build passed, ctest169/170 in16.86s with only permitted
session_skill_binding. Frozen `dh-foundation-core-glow-boss.exe` SHA256
`2DF560F3AD8CB49083B4CBA804B509605481478C03BD2F5E6AD7228F594DE23F`.
Normal actor-renderer glow check `.local-inputs/v20.1/root/core-glow-runtime/`
passes360frames/exit0 in9.4s, one Celeste attachment, no FX presentation failure.
Inspected f240: golden glow accompanies faery, ordinary moth geometry remains.
Dense frames30–360 retained for continuity review. The first launch rejected
the unsupported --world-save flag before gameplay; corrected scratch-cwd launch
is the successful test. Full boss evidence is the independently reviewed five
private runs; short combined trigger admission verification remains pending.
B101 residual red-sheet appearance is OPEN and is not resolved by core tests.

B109 material repair and B095 closed-release repair have since merged cleanly
at ef6360b1. Their combined frozen checks remain pending. User confirms B109
rendering fixed; original exact pixel parity and inherited renderer limitations
remain caveats, while travel-message layout is owned independently as B113.

## Reviewed decor and closed-input integration

Behavior sourceef6360b1; report-only correction99c6000f. Build passes, full
ctest169/170 in16.65s with only permitted session_skill_binding. Frozen
`dh-foundation-decor-bag.exe` SHA256
`445A167198FE2A5CA67B9E063F571E795F824A1B52C9C30BF17104B1DB3F9047`.

Quiet isolated `.local-inputs/v20.1/root/decor-bag-runtime/` checks:

- B109 exit/cave controlled source fixture: exit0 in7.3/13.5s. One ambient
  decor owner, baked decor0; Swamp37instances specular14/opaque8, Cave15instances
  specular7/opaque1. Inspected exitf140/cavef300 confirm rendered material path
  and geometry. Original benchmark quest states are explicitly seeded; this is
  a render/transition test, not proof of natural quest progression. Old travel
  box is expected in this pre-B113 frozen build. User rendering acceptance and
  re-review bound B109; exact original renderer parity remains unclaimed.
- B095 main-loop queued Close→UNEQUIP sameframe340: exit0, menuclosed, equipped
  StartingSuit/count4 unchanged. Inspected f430 ordinary game view. Uses retained
  --menu-release main dispatch hook; real10ms navigation at both aspects is
  independently reviewed private evidence and is not repeated as a new claim.

B113 independently reviewed9623f483 and B111 independently verified8903fff5
have now merged. B113's final private frozen EXE includes a recorded temporary
B106 dependency absent from its branch; formal B106 review remains required.
B111 reviewer fresh physical clicks at two aspects reproduce source-backed
values and authored pages; older captures used other save states, preserved as
qualified historical evidence. B103 stat allocation remains a separate gate.

## Merchant, status circles and normal close integration

Behavior source0f7bc40f contains repaired B110 and B086 plus B112 documentation
and reviewed B100 source once through B110. CMake conflict retained both actor
effect tests and new merchant/text tests. Build passes, fullctest171/172 in23.28s,
only permitted session_skill_binding. Frozen `dh-foundation-merchant-close.exe`
SHA256 `D84799FE8C5B78F9B553FFB1E5BDDEE4C6A6835434D6F16026C67521C66B2168`.

Independent scratch asset tree `assets-integration` copies accepted assets and
adds verified778 models/two markers; no junction/hardlink or accepted asset edits.
Receipt `test-assets-receipt.json`. This is an isolated test tree, not a release.

- Five merged merchant cases exit0: equipped SELL blocked, UNEQUIP→SELL once,
  BUY cancel/confirm/duplicate and equip controls, maximum potions12/12 then
  blocked ordinary/max BUY, full-bag modal background/OK. Transactions and saved
  gold/items/equipment asserted in `merchant-close-runtime/asserted-result.json`.
  Equipped case was repeated with a seeded save because the first read-only
  no-transaction run created no profile file; both receipts preserved. Inspected
  blocked SELL and12/12 disabled BUY/MAX UI. Fixtures use diagnostic source menu
  releases and gold10000, not natural gold acquisition; independent physical
  pointer evidence remains in reviewed B110 reports.
- Eight combined NPC near/distant runs at1280×720/1024×768 exit0. Inspected all
  f260 views, quest f340 after acceptance and merchant f580 after close/reopen.
  Green circles/blue!/money-bag appear, distant circles persist, acceptance removes
  quest marker exactly once, repeat Space raises no duplicate accept. Merchant
  normal Space opens real shop twice around explicit Back; no merchant interaction
  while distant. `npc-shop-circles/contact-order.json`, contact.png, run logs and
  strict inspector retained. Campaign scripts are off in this marker fixture;
  dialogue proof belongs separately to reviewed B093 source run.
- Real WM_CLOSE twice on normal frontend Continue/movement saves once atframe91.
  Fresh copied profile without old world starts own session, then snapshot saves
  hero386.857,−927.156,255. Copy both files to fresh resume directory; normal
  frontend Continue validates44actor world and restores that exact position at0/1;
  further movement ends−917.296,−117.013,255. Inspected f1/f200. Root first shell
  launch rejected malformed array arguments before gameplay; corrected PS wrapper
  succeeded. Initial resume preparation rejected an overbroad prefix assertion;
  corrected exact output-path assertion succeeded. Neither failure touched user
  or accepted files. Evidence `merchant-close-runtime/normal-close[-resume]/`.

Reviewed B106 binding/held-key repair092b3501 and B108 initial-checkpoint repair
ab1ee317 are now merged at10f8fa3f. New frozen combined death/travel checks pending.
An initial merge met a transient Git index lock; clean status and subsequent
ordinary merge succeeded without deleting a lock or resetting files.

## Remaining production and release gates

Per-bug reviews, remaining implementation/runtime checks, integration of reviewed
branches, combined save/death/transition checks, full ctest after changes, final
asset manifest, frozen v20.1 independent verification and corrected release
notes are pending. See CODEX-RUN.md and the shared bug tracker for live ownership.
No active bug has been moved to RESOLVED-BUGS and no package is accepted by this
baseline report.

## Combined root verification continuation — requested release v21

The user requests the finished release as v21. Existing working coordination
paths retain v20.1 for continuity; package/manifest/receipt/README use v21.

- Reviewed B093 a2e1e5f7, B1031d69d39e, sounds758d970a, B07945571237 and
  B097af2bf2cd integrated. Audio merge retains both plain B111 menu cues and the
  typed original UI sound result API; no accepted cue implementation discarded.
- Frozen root quest/skill build926ECA2E0DD4FDF62B09E945F5C1D2A62849E36FCDC01483AF970E7B5D7BE9B8
  uses independent staged assets and own saves. Actual eighth moth death157
  yields one row53 completion,20EXP/150GOLD, saved count8/state13. Authentic
  QuestComplete165/UID12/token13 has300275 output sample frames. A subsequent
  actual death is applied0, no quest repayment or completion cue, enemyXP5 only.
  Strict decoded-save/token assertions and inspected f150/200/300 sequence are
  preserved at `.local-inputs/v20.1/root/quest-sound-runtime`. These are copied
  genuine count7/count8 profiles with bounded world-reset/startup relocation,
  not an uninterrupted Act1 playthrough; silent output does not prove audibility.
- Same frozen build: ordinary10ms mixed allocation controls spend stats then
  skill and No, or skill then stats and Yes. No restores Strength10/statpoints2,
  Hardiness0/skillpoints1; Yes saves Strength11/statpoints1, Hardiness1/skillpoints0.
  Both reopen Skills correctly. Strict saved-field assertions and inspected
  f1450 views in `root/mixed-allocation-runtime`. Existing isolated profile
  has2stat/1skill points; no point grant or forced menu-release used.
- Same frozen build: isolated sourceHP10 fixture with44actor campaign roster
  genuinely dies132, Continue400 restores44actors/25objects and revives422 at
  fullHP165.098/MP27.25. Space440/470 absorbed. Real duplicate WM_CLOSE at15s
  saves once withHP98.8867 after further ordinary combat. Fresh copied profile,
  world and selective checkpoint launch through normal frontend Continue,
  validate44actors and restore exactHP98.8867/RNG10090467:129 atframe0, then
  ordinary movement/combat. Inspected death300/revive430/resume30 sequence.
  `root/death-close-runtime` and `root/death-frontend-resume` retain source
  args/logs/captures/private close receipts. This is not dead-moth persistence proof.
- Full root suite on that pre-B114/pre-B088 build:171/173 pass, known
  session_skill_binding path failure and campaign_host timeout. Unchanged
  isolated campaign_host retry passes0.92s; timeout cause remains unproven.
  Full LastTest and failed list preserved as `ctest-quest-sound-rank*`.
- Reviewed source specular patchf6720b2d, B115 reportb0499ca2 and B0882a95fed7
  merged. New frozen2CC32B26ADEB44334C6C1B1853AA9628070A778645E194E157C260DCC6E90004,
  `root/dh-foundation-npc-specular.exe`, builds successfully. Focused6/6 checks
  (normal world/campaign, mixed training, source specular logic and WGL rendered
  pixel/negative controls) pass. Overall B114 brightness/fog remains OPEN.
- That new frozen build: normal frontend, own copied profile, E down held3500ms
  confirms Cave192/arrival193, remains there through the hold; release then
  new press confirms Swamp389/arrival390. Exactly2confirmations, no automatic
  return. Inspected frame80 upper Bogwitch Cavern bronze destination, frame280
  The Boglands in cave, frame500 back in Swamp. Fixture uses source player-to
  exit relocation and a bounded3actor roster, not full campaign progression.
  `root/travel-input-prompt-runtime` retains captures and Win32 input receipt.
- Fresh moved-NPC marker repairc02bd8dd/docsd9dbfc60 independently reviewed and
  merged after that freeze. Original raw setter evidence corrects +138 to
  enabled(SetEnable0x33ddc8), +128visible(SetVisible0x33dcf8); B100/B112 reports
  corrected. Original MothIntro footage267–283.5 supersedes B080's old absence
  claim; B082 death scan absence is qualified rather than asserted for all footage.

Remaining confirmed gates have owners: B083 final injury/skill focus ordering,
B092/B098 reproduced persistent corpse restore, B101 original fire appearance,
and B114 overall lighting/distance behavior. Final combined rebuild/full tests,
asset packaging, independent frozen verification and acceptance remain pending.
