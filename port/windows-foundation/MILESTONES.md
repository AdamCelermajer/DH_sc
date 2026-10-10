# Shared actor checkpoint and fidelity contract

Active goal: a playable original-content checkpoint using reusable whole-game
systems, with visual fidelity and gameplay fidelity as equal requirements.
Swamp is validation content; map, class, actor and weapon rules are data inputs.

## Delivery priority (2026-10-09)

Latest steering, 2026-10-10: Preview10 remains gated on the user's confirmed
frontend/menu/combat bugs, real skill keys1–3/faery4, pause and kill XP, including
retroactive source/visual investigation and actual production verification.
The menu black-ground mismatch is an open visual gate. Earlier scoped-preview
instructions below are release history and do not override this gate. Preview11
targets authored pots/chests with break/open/reward/pickup. The current full Act1
work/allocation plan is `coordination/act1-work-plan-20261010.md`:21 bounded
feature/verification lanes plus the integration lead and workforce dispatcher.

Current release: V19 Preview 9 at `.local-inputs/windows-source-clock-v19-preview-9`.
Direct-Swamp Equipment now works; Knight current/empty equipment selects original
locomotion banks on the shared owner. 80/80 tests and 13 packaged runtime cases
pass. Loot/XP, original potion/gold geometry, and dual/staff stance primitives
have component evidence; full gameplay enrollment and fidelity remain pending.

Current release: V19 Preview 8 at `.local-inputs/windows-source-clock-v19-preview-8`.
Offline character menus now pause gameplay and resume on close while the UI and
audio device clocks stay live. Equipment reload lifetime access is fixed.
80/80 tests and nine packaged runtime cases pass on the corrected executable.
Direct-Swamp equipment source slots, full page presentation, skill execution,
equipment stance enrollment and remaining campaign fidelity are still pending.

Released: V19 Preview 7 at `.local-inputs/windows-source-clock-v19-preview-7`.
Equipment selection/unequip/re-equip and current-state save/reload pass packaged
checks; Skills selection/details pass with inspected original-art rendering.
80/80 tests and eight packaged runtime cases pass. Full pages, positive skill
spending and arbitrary equipment rollback remain gaps in that earlier build.

Use small reusable gameplay systems and original assets/data. Fidelity applies
to visible art and gameplay behavior; reconstructing the original internal
object graph, callback routes or native ownership is not a release requirement.
Recovered implementations are references or reusable components, not mandatory
architecture. A feature should normally need one feature worker and one bounded
integration into the running game.

Ship the next V19 preview with working equipment selection, unequip/re-equip,
save/reload, and Skills selection/details. Record unfinished page presentation
and positive skill spending explicitly. Do not hold this preview for loot,
weapon effects, companion AI or complete campaign fidelity; develop those in
parallel and release them when playable. Measure progress by runnable features
and end-to-end checks, rather than source line counts or internal adapters.

## Reference evidence

User-supplied Act 1 walkthrough:
https://www.youtube.com/watch?v=z_Zky7qQdYs&list=PL2REF12lEpIR8hEhx7O2pJrEoOXg9H9bp

Use original assets, definitions and behavior evidence together with timestamped
video observations. A walkthrough establishes what is visible in its captured
version and scenario; it does not establish all hidden rules or all branches.
Record version/aspect/resolution differences, reference timestamps, camera state,
equipment and gameplay state before judging visual mismatches. Do not claim a
video comparison until frames have actually been inspected.

For uncertain implementations, use the user's IDA handoff at
`.local-inputs/ida-apk-export-2026-10-07/HANDOFF-TO-MAIN-SESSION.txt` and the
library exports under `libraries/`. Original instructions, callers/callees,
xrefs and data establish behavior; inferred pseudocode must be checked where
signedness, types, ordering or control flow are ambiguous. The live hook has a
restart helper, but static exports remain usable independently. Preserve the
original input identity and trace addresses in evidence reports. Reproduce the
rules and observable results through simpler reusable machinery.

Loading original artwork is insufficient for visual acceptance. Check model and
texture selection, equipment attachments, scale, placement, animation poses and
timing, camera framing/movement, material behavior, lighting, effects, health/mana
and target indicators, and HUD layout. Preserve authored visual effects where
supported; report absent or approximate implementations instead of accepting
asset provenance as appearance parity.

## Milestones

1. **Populated world.** Load original player, mob and NPC definitions, with the
   priest/faery where the authored state requires them. Verify appearance and
   placement using original data and video. Conditional placements remain
   conditional; rendering a companion does not prove its behavior.
2. **Movement and camera.** Input-independent movement actions, authored floor
   support and collision, facing and idle/walk/run transitions. Verify gameplay
   camera behavior and movement appearance against the reference. Development
   camera tuning is labeled separately from original camera reconstruction.
3. **Equipment, targeting and minimal HUD.** Data-driven weapon/stance mappings,
   original attachments and animation choices, shared target selection and
   validity, original targeting visuals, truthful health/mana/target UI.
4. **Shared combat.** One warrior and one original enemy attack each other using
   the same actor, animation, hit-event and damage mechanisms. Verify authored
   hit timing, movement/targeting rules, reactions, death and cleanup visually
   and behaviorally. Generic mechanisms must not erase original differences.
5. **Integrated checkpoint.** One packaged build runs the populated world,
   movement/camera, targeting/HUD and shared combat. Preserve prior foundation
   checks and produce logs, screenshots/video and explicitly scoped acceptance.

## Subsequent whole-game roadmap

Historical diagnostic delivery: `.local-inputs/windows-shared-checkpoint` contains
the standalone Windows executable, its original-content asset folder and data
startup profiles. `reports/packaged-shared-runtime.json` records verified
bidirectional original-data combat, full initial attack groups, enemy death,
live save/reload and a separate-process restore. The package's default world
startup also verifies movement and grounding. That historical checkpoint passed twenty-eight component checks.
These are runtime proofs, not final fidelity acceptance; the remaining scene,
AI, movement, combo, blending and appearance gaps remain part of this goal.

Class skills and enemy/boss abilities; audio/effects coverage; companion AI;
interactions, original stats/equipment/inventory/skill screens; cinematics and
camera tracks; quests, unlocks and campaign transitions; world/campaign saves;
other classes, areas and platform builds. Full-game completion requires complete
coverage and connected playthrough verification, not this initial checkpoint.

## Work boundaries

Latest separate checkpoint: `.local-inputs/windows-source-clock-checkpoint-v18`.
75 component tests pass; 572 packaged manifest files, 29 positive runtime cases
and two unsupported-save rejection checks verify the scoped v18 development release.
The previous shared checkpoint remains available unchanged. Per-phase player
movement now follows authored MoveGO through the shared collision host; the
rendered moving attack also admits authored root travel. NPC initial attack
has zero authored XY travel; original death movement is retained. Saved corpses
hold the terminal pose without replaying displacement under the version1
canonical save policy. Source two-slot clocks, blending, events and root histories
now drive live player/NPC combat, NPC idle/reaction/death and player Idle/Walk.
Finite source timeline completion repeats through NewAnim with same-slot extra
and root reset; intermediate raw-loop v4 diagnostics are superseded.
Live forward actor camera follows admitted position and desired heading.
Finite authored Run repeats are now bound. Type2 Idle remains pinned; random reselection and source
controller/skill/attack heading gates, synchronous reentry and outer combos
remain incomplete.
NPC modular defaults come from serialized categories and referenced auxiliaries,
without inheriting player clothing. Original encounter trigger/scripts and
spawn clips and original timer/script interpreter are source-tested. Deferred
actors and exact source-world lookup now execute diagnostic serialized Spawn
commands at frames 33/128 in global context -1, including finite Spawn sequence
completion into Idle3 and enabled render/combat gating. Full original trigger,
module, cinematic and UI progression remains incomplete. Physical/body/collision
flags govern the existing render-bound movement approximation; original joint
physics, aggro, sneak and full FSM remain missing. Saves/restores/reloads reject
registered lifecycle state until lifecycle/campaign persistence is implemented.
An expected save-at-frame80 rejection preserves the existing save hash; it is a
separate error-path check, not a seventh successful rendered run. Last managed
Spawn state and later combat5/12 facts are not a complete unified source FSM.
Source camera Dummy transitions/return preserve original damping ownership.
Initial-idle controller request suppression is verified; full lock/Stop/forced/
cast/FSM gates remain incomplete. Original factory/rest joint-box bounds are
translated with live position, without replacing the physical solver. Trigger
contacts/module construction are component-only; live module IDs are unproven.
Selected COMMON blend/depth and body-unlit classification are verified;
auxiliary lit shadow does not prove body lighting or full shader/fog/light parity.
Milestones retain their full fidelity scope and remain incomplete.

Preserve the accepted foundation package and existing Android/compatibility work.
Root owns main/CMake integration; contributors have separate source ownership.
Keep gameplay independent of PC/touch input. Reuse original definitions and
verified reconstructed components where helpful, with simple boundaries.
Backend changes must demonstrate a reduction in work toward fidelity.

Track each capability as source-implemented, component-tested, runtime-tested,
reference-compared, incomplete or blocked. No fabricated balance values,
animation event timing, placement or visual appearance may be described as
original. Temporary development configuration must remain explicit.






Current v8 adds shared source attack admission for player and diagnostic AI,
independent of lifecycle binding. Blocked requests do not interrupt admitted
poses or hit markers. Forced gates are component-tested only; no forced main
gameplay backend is claimed. Two expected controller/lifecycle save rejections
preserve the existing durable file, separate from twelve rendered successes.
Original lightweight body rules/native Box2D tests verify source radii and
translation; production PF/filter/contact/debug providers remain unbound.
Unfiltered source Level construction is component-tested; the actual cache
reaches a missing LevelConfig constructor, so complete live Level/module IDs
remain unproven. All five full-fidelity milestones remain incomplete.


Current v9 uses original rest joint bounds/CharacterBodyConfig for player motor
radius113.7; NPC motor radius, height and solver remain preview, with no native
player-body publication claim. Genuine named marker event28 precedes the source
controller gate and finite closure22 is routed. Damage remains queued; full
synchronous state forwarding/reentry and numeric24..27/23 hierarchy are missing.
Navigation real-resource component tests retain explicit room/pose/bounds fixtures.
Actual cached Level now constructs LevelConfig and stops at the next missing
Module constructor; complete live Level/module IDs remain unproven. Existing
GameplayCamera900/4200 parity is a settings check, not a new live clipping fix.
All five full-fidelity milestones remain incomplete.


Current v10 shares source BodyPlan/restbounds across player and registered NPCs
using actual loaded visual selection. Native bodies borrow actual positions and
readonly properties, with source PF user0 constructor ordering before InitFinal.
Body-present physicalXY equals actorXY*.01; lifecycle removes/recreates genuine
native bodies. Save/reload/resume release and rebuild those owners. Pure source
filter prefix and native-object toggles do not establish full PF/collision Step
and contact/event continuations, movement solver, allocator/saved fields or
death-filter/FSM parity. Actual unfiltered source root constructs LevelConfig
and nine Modules with real IDs/properties, then lacks ConditionData.Init.
Main live moduleIDs/MGPchildren/InitPost are not certified. All five remain incomplete.


Final v10 controller/lifecycle expected-error saves exit1 before writing and
preserve durable save SHA11d7ac28...f6e9; separate from13rendered cases. Final
startup/controller captures were inspected qualitatively, without parity claims.


Current v11 binds genuine root constructor ModuleIDs into host scopes and rebuilds
on reload. Scoped Spawn diagnostic reads the actual authored module1 binding.
The optional source-floor probe builds nine PFrooms/sixteen source floors using
original factory caches and ResetPositionFromFile; startup query255 is verified.
ModuleID, inherited room64 and PF registry index remain separate identities.
The source-floor solver is not adopted. Condition runtime118rows/InitClear reaches
CheckSpawnProbability; complete ModuleInitPost/children/level admission remain
incomplete. Typed contacts37..3c and pinmass are component-only without Step or
SAME AIS/fullPF event continuations. All five milestones remain incomplete.


Current v12 supersedes v11 probe-only floor admission: late-attached SAME PF
objects use original nine-room/sixteen-floor world, radius and obstacle registry.
Player/NPC root motion shares source ValidatePosition; actor/AABB/native body
publish admitted XYZ without preview axis/sweep fallback. R/F9 and physical
remove/init rebuild or toggle actual registry ownership. Walk90 world59.1969
from authored276.028 reflects a true source XYfloor boundary, not precision error:
1140.5708,-196.916412 misses all triangles at both255/255.000061 with valid Zbounds.
No epsilon or ValidateDirection root veto is added. Original UpdatePath heading
phase and full heading/rotation/frame/avoidance coordinator remain incomplete,
as do native Step/contactAIS/dynamic collision. Module probability component
uses shared application RNG/nine default draws, then lacks Device.IsHighPerformance;
full ModuleInitPost/campaign gates remain incomplete. All five fidelity gates remain incomplete.


Current v13 adds generic manual PC Move-only source heading continuation after
sampled root, using source Move focus-prefix flags/movement cells and actual
Debug boundary getter. Direction bool is diagnostic; changed heading is retained.
Real late rotation borrows SAME actor Euler/heading/Session turn storage with
resolved224/sourceflags and publishes the same Euler to visuals. Short walk
checks44/slides60/rotations60 has PF91accepted60clamped0; long650checks/100slides/
650rotations has PF712accepted118clamped542. FullFSM/focus tails/path/avoidance,
nativeStep/contact/frame and matched visual/gameplay fidelity remain incomplete.
All five milestones remain incomplete; v12 package hash is independently preserved.


Current v14 supersedes the bounded source-hit queue gap: source helper sink
applies SAME actor HP, receipt and bound victim reaction/death before state
forwarding. Generation/closure/duplicate guards and latched sink failure protect
replacement actions; legacy queue and sourcebegin initial-marker edge remain
explicit. Earlier victim publication changes spawn return hits179/237/295 with
unchanged damage/RNG; movinghits78/141/204 remain stable. FullF_ApplyResult/AIS/
FSM/status/DOT/leech/skills/globalLevel chronology remain incomplete. NewAttack
focus-prefix/exactBlur/motionframe prefixes are component-only, not livefullfocus
or frame integration. All five fidelity milestones remain incomplete.


Current v15 source events publish before root motion with unchanged hitframes/RNG.
Actual VisualNAME own/target cache survives reload/restore with canonical fields
and raw-save guards. Packaged actors have no target_node (knownNULL); genuine
boss nonnull lookup is component-tested, not positive live acceptance. Source
SubObjects native/PF/staged visual fixtures are component-only, without main
independent visualRoot/fullfocus/camera binding. All five milestones remain incomplete.


Parallel feature development is tracked in reports/feature-acceptance.json.
Standalone source/component receipts do not inherit v15 runtime acceptance.
Root integrates shared actor/property/RNG/retained-clock owners through main/CMake;
HUD geometry remains with its assigned owner. Features need actual integrated
behavior, safe persistence/reload, package hashes and matched visual/gameplay
reference evidence before release acceptance. The v15 package stays preserved.


Current v16 integrates portrait alpha-center alignment and shared PC/touch
profile/C opening, Stats panel and modal close. The native source-art panel
draws actual values with original TTF fonts103/287; open60frames draws55 and
ESCclose35 draws30. OSphysicalclick is not manually inspected; owned client
handlers and injected pointer transport use the same hit surface. Stats remains
a development subset:58solid/mask/filter branches, ASvisibility/reflow, other
tab mutations and complete inventory/skills projection are absent. No matched
original Stats gameplay acceptance. Frontend/classcreation is separate and the
old reconstructed prototype FAILED reference gate; use original-game user images
only. Twenty-three tracks have source/component outputs; only this UI subset
inherits v16 runtime proof. All five full-fidelity milestones remain incomplete.


## v17 controls and source combo checkpoint

Default PC movement runs; holding Shift selects walking. Held attack commands
use the original pre/strike/recovery windows and complete root sequence. The
packaged Knight data selects the three original one-hand combo resources; the
shared system is driven by source sequence metadata rather than map/class logic.
All three groups retain one animation owner and action generation. Previously
accepted continuation survives release. Restore clears transient combo state
and a fresh attack begins at group zero; mid-swing cursor persistence is absent.

Evidence: `reports/controls-combo-integration-v17.json`, 73 native tests,
18 preserved runtime cases, four full combo cases and one default-run case.
Two unsupported provider-save attempts preserve the durable save. Original
swing captures were inspected; continuous matched-state reference parity has
not passed. Source look/pre-attack/sticky/heading/OOI integration and full actor
FSM/physics ordering remain incomplete. All five full milestones remain open.
The main menu/class creation feature remains independently under development.

## Full character-menu acceptance contract

The original character menu remains incomplete until its Stats, Equipment,
Skills and Faery pages all work. Stats-only releases do not satisfy this contract.
The original Quest and Map pages, including the HUD minimap, are also required coverage.

Each page requires original artwork and source display states across the full
viewport, matching draw/hit transforms on PC and touch, actual live owner data,
authentic selection and actions, and safe save/reload behavior. Equipment needs
item selection/details and actual equip/unequip publication to the same combat
and appearance owners. Skills need actual progression, prerequisites, details
and hotbar assignment through the same skill owner. Faery needs original unlock
and selection data, details/abilities and actual companion publication. Missing
providers must remain explicit; rendered static art does not establish page
completion. Compare each page against original gameplay independently.

Integration scheduling and page ownership are maintained by the integration
lead in `reports/integration-task-board.json`; root owns release verification.

## v18 Stats and combat-text checkpoint

Full-window original Stats art, source details/training/class text and live original scrolling combat text are verified as a development subset. Width-only resize glyph corruption was fixed and resized/fresh 4:3 text pixels matched exactly. Evidence: `reports/menu-combat-text-integration-v18.json`, 75 native tests, 29 positive packaged runtime cases and two durable-save rejection checks; 572 manifest files. V17 is preserved.

The full Equipment, Skills and Faery pages remain required and in development. Stats acceptance does not complete the character menu. Whole-game visual/gameplay fidelity and native owner/caller integration remain incomplete.

## V19 Preview 6 — current playable release

Packaged at `.local-inputs/windows-source-clock-v19-preview-6`: `Play.cmd` opens
the frontend; `Play-swamp.cmd` starts directly. Original warrior step-entry
swooshes and Priest/Faery shared-clock Idle are enrolled. Gameplay persistence
retains both companion pose owners. Skills displays original art and saved
ranks/points; full-viewport opaque backing prevents gameplay bleed.
Seven packaged runtime cases and 80 foundation tests pass. Evidence:
`reports/v19-preview-6-release.json`. Skills body selection/details, training,
slots, full equipment/faery/quest pages and companion behavior remain incomplete.
Enemy attack Sound474 references an unavailable original WAV; the audio report
records three playback diagnostics separately from the working hit observer.
Matched-reference fidelity and full Act 1 remain incomplete. Preview 5 is preserved.

## V19 Preview 5 — preserved previous release

Packaged at `.local-inputs/windows-source-clock-v19-preview-5`. The executable
and `Play.cmd` open the original-art frontend; `Play-swamp.cmd` bypasses it.
Real mouse/name input, warrior creation/save/reload and same-window gameplay
handoff pass in the packaged 33-visual world. A separate process reloads the
profile without rewriting it. Population/persistence/moth combat and original
combat audio pass against the frozen binary; all 80 foundation tests pass.
Evidence: `reports/v19-preview-5-release.json`. Mage/rogue creation, rendering
and movement are development-tested; their combat remains unavailable.
Full opening cinematic, selected-class idle/stance fidelity and complete
character pages remain in development. Preview 4 remains preserved.

## V19 Preview 4 — preserved previous release

Packaged at `.local-inputs/windows-source-clock-v19-preview-4`; run `Play.cmd`.
Original hit/hurt sounds now reach one Windows audio output using the live
player/camera and shared saved RNG. Actual packaged420-frame save/reload/restore
test starts/completes7 original voices with0 diagnostics and one initialization.
244 WAV files match original cache bytes. Populated-world resume is fixed.
828 manifest files,12 packaged runtime cases and80 foundation CTests pass.
Evidence: `reports/v19-preview-4-release.json`. Swing step-entry cues/music/global
audio, original campaign progression and full matched fidelity remain incomplete.
Schema3 quest codec transport and animation-only Priest/Faery pose support are
core/component capabilities; full pages and companion main enrollment are pending.
Earlier releases remain preserved.

## V19 Preview 3 — previous preview preserved

Packaged at `.local-inputs/windows-source-clock-v19-preview-3`; run `Play.cmd`.
Original weighted CharacterTemplate data adds20 authored mobs: visuals13→33,
shared combat actors3→23. `Test-populated-combat.cmd` starts near two original
moths which approach and damage the player through the same shared systems.
Save/content reload/restore preserve the population and shared RNG. 578 manifest
files,10 packaged runtime cases and80 foundation CTests pass. Evidence:
`reports/v19-preview-3-release.json`. Remaining28 declarations are gated or
unsupported; full campaign progression and matched-reference fidelity are pending.
NPC attack choices remain explicitly supplied first authored groups. Audio/main
enrollment and the later direction-validation patch are subsequent integrations.
Earlier packages remain preserved.

## V19 Preview 2 — previous preview preserved

The next playable integration is packaged at `.local-inputs/windows-source-clock-v19-preview-2`.
`Play.cmd` enables generic enemy AI at the authored start; `Test-enemy-combat.cmd`
uses an explicit nearby diagnostic placement. Bound enemies approach and damage
the player through shared animation/root motion and authored hit events. The
packaged fight verifies three player swings kill the enemy and clear its target.
575 manifest files, two enemy runtime cases and six baseline runtime cases pass;
current foundation CTest is 79/79. Evidence: `reports/v19-preview-2-release.json`.
Original encounter population/activation and full visual/gameplay acceptance
remain incomplete. Audio/frontend/equipment/effects component work is not yet
enrolled in this executable. V18 and Preview 1 remain preserved.

## V19 Preview 1 — previous preview preserved

### Architecture and delivery correction — user confirmed 2026-10-09

Fidelity means original content, art, animations, gameplay rules, effects and
timing. Reproducing the original internal object layout, lifecycle and callback
architecture is not a release requirement. Use the working portable gameplay
runtime and clear data-driven feature interfaces. Recovered native owners remain
reference/optional backend work; do not gate every playable feature on full native
Character InitPost, PlayerManager publication or indexed original save internals.

Immediate V19 delivery path: a generic enemy controller operates the SAME live
CombatSession actors and shared navigation, issues attacks through the shared
command/cooldown/event system, and uses original AI/property data. Combat audio
uses that session's actual markers/outcomes and one platform audio output.
Character creation uses the SAME portable character/save state with original
class metadata and assets; clearly distinguish our PC save format from original
Android files. Never overwrite existing profiles or claim unimplemented source
data/state is preserved. Original cinematic commands use generic actor, camera,
HUD/input and trigger interfaces, not a cloned World or frame schedule.

Each worker should deliver a feature that root can call through a small stable
interface. Prioritize executable behavior and end-to-end checks over additional
internal-owner wrappers. Preserve accepted packages and native/reference work.

Packaged current playable development build at `.local-inputs/windows-source-clock-v19-preview-1`; run `Play.cmd`. V18 remains unchanged (572 manifest files rechecked). Preview verification passes 29 positive runtime cases and two expected durable-save rejection checks. Evidence: `reports/v19-preview-1-release.json` and its runtime/combo/UI/save reports.

Visible behavior largely matches V18; the shared navigation lifetime integration is in the playable executable. Recent canonical profile, class-property, mask2 and Gear prepare/mask4 progress remains native integration-test scope. Initial grants, native skills and full menu pages are not exposed as completed playable features.

Deliver numbered V19 previews as meaningful playable integrations land, while retaining V18 and previous preview packages. Final V19 still requires its integrated acceptance gates.

### Next playable V19 priorities — enemy combat, audio and mob introduction

User reconfirmed these priorities on 2026-10-09. Target an integrated original
enemy aggro/approach/attack/damage/reaction/death loop through shared actor and
animation owners. Prioritize authored combat audio alongside it. Restore the
remembered jumping-mob introduction using its verified original encounter
trigger, spawn animation and camera/cinematic sequence; identify its exact
source actor/sequence and footage timestamp before selecting the implementation.
Existing lizardman spawn-jump assets are candidates, not proof of that encounter.
Do not enable all authored actors or substitute frame schedules for conditions.

Keep menu/creation work active in parallel. Numbered previews may expose verified
pieces before the full milestone; explicitly name what is playable and what is
still absent. Tests of a diagnostic stationary enemy or audio transport alone do
not establish original live AI, full combat audio or cinematic acceptance.

## Main-menu startup and complete character creation — requested 2026-10-09

The normal production executable must start at the original main menu, with its
authored animated scene/camera and class-selection animations. Preserve a direct
Swamp test launch using the same executable and an explicit startup mode. Ship
separate normal and Swamp-test launchers; the existing accepted packages remain
unchanged until a verified preview includes this integration.

1. Make the existing native frontend callable from the production entry point,
   with explicit window/render/input lifetime and a typed selected-profile result.
2. Finish indexed character creation and selection: original name/class flow,
   same canonical Character/Save initialization, genuine slot persistence,
   assignment and reload. Creation must not report success for metadata alone.
3. Complete original menu/class scene playback, camera transitions and ordinary
   idle continuation using the retained actor owners; verify lighting and art
   against original gameplay. Menu cinematics and the opening gameplay cinematic
   have separate authored triggers and must not be conflated.
4. Start gameplay using the selected character and same save authority. Cancel
   old menu input and release menu-owned render resources at the handoff. Returning
   to the menu must select the actual saved profile without duplicate creation.
5. Verify normal startup, create each base class, quit/reload/continue, selected
   character gameplay, repeat transitions, multiple resolutions and direct Swamp
   launch. Keep test saves isolated from real selected profiles. Release a numbered
   preview once the connected flow is playable; preserve earlier previews.

The integration lead coordinates frontend ownership and the workforce dispatcher
assigns bounded workers. Root owns production main/build changes and releases.
The standalone frontend diagnostic remains a development tool; it does not satisfy
the requested normal executable startup or completed creation/gameplay flow.

## Required Act 1 feature coverage

Act 1 validates reusable whole-game systems. Required coverage and acceptance criteria are recorded in `reports/act1-feature-coverage.json`; this list is not an exhaustive inventory.

- **Enemy combat and AI.** Original aggro, movement, targeting, attacks, hit timing, reactions and death; enemies and player share canonical actor/combat owners.
- **Drops, looting, money and XP.** Authored drop tables, probability and conditions; pickup grants real inventory/currency/XP once, with source leveling and durable save behavior.
- **Skills, attack animation and weapon effects.** Original skill/attack clips and event markers drive effects; source weapon data selects sword trails, staff/ranged projectiles, damage and audio.
- **Breakable pots and contained rewards or mobs.** Render authored objects, destruction state/animation and source conditional contents through shared spawn/reward systems; no assumed always-spawn policy.
- **Chests and opening animations.** Original interaction, eligibility, opening sequence and reward timing; prevent repeated rewards and restore original persistent open state.
- **Tutorial HUD and cinematics at authored triggers.** Original progression/area triggers drive prompts, camera, actor clips, HUD visibility and control transitions; no diagnostic frame schedules.
- **Audio throughout gameplay and UI.** Original IDs and event timing for combat, skills, projectiles, impacts, deaths, loot, interactions, UI, companions, cinematics, ambience and music; unavailable assets remain documented.
- **Quests, NPC interaction and quest menu.** Original objectives, dialogue, NPC services, completion/rewards and progression use same live quest/event state projected into the character-menu Quest page.
- **HUD minimap and character-menu Map page.** Original map assets, projection, player/quest/NPC markers and source visibility/update rules; consistent full-window rendering and hits.
- **Castor and faery following and AI.** Source follow/master ownership, navigation, encounter behavior, target validity and save lifecycle; verify named Castor identity and whether/when he attacks.
- **Faery abilities, animations and effects.** Selection/unlocks and source abilities drive actual companion animation, timing, effects/audio and gameplay result through shared owners.
- **Animated world objects and progression barriers.** Generic authored object animation/enable state and quest/boss conditions support gates and barriers across all maps; persist and restore original state.

Verify Castor's exact identity/attack behavior and the remembered witch/waterfall barrier from original footage and source before selecting implementation details. Original conditions and drop probabilities remain authoritative.

Prioritize the integrated enemy combat/death/drop/pickup/XP/save loop after canonical owners are available. Full character-menu pages and other independently testable features continue in parallel under the integration lead. Release subsets do not complete Act 1; connected original-reference playthrough is required.
