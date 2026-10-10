# Act 1 work and staffing — 2026-10-10

Act 1 is not yet playable from beginning to end. The accepted release is V19
Preview9. Newer development builds have individual passing fixes and remaining
failures; they are not accepted releases. All systems below are reusable across
the game, with Act1 providing authored validation content.

## Roles and capacity

- Root: active goal, shared engine interfaces, milestone priorities, accepted
  releases and final integrated verification.
- Integration lead: production composition and main/CMake, fresh isolated
  runtime checks, small integrations of completed feature modules.
- Workforce dispatcher: maintain useful assignments, exclusive file ownership,
  dependency follow-up and actual running roster. Reopen helper-only sessions
  until the requested feature is integrated and verified.
- Allocation: 21 bounded feature/verification lanes plus integration lead and
  dispatcher = 23 subagent roles. Root makes 24; retain one concurrency slot for
  a difficult escalation. This is an allocation target, not a claim all are
  currently active. Fill useful disjoint tasks; do not duplicate writers or
  occupy a worker merely to increase a count.
- Ordinary bounded workers use GPT-6 Luna HIGH; escalate to GPT-6.1 Sol MEDIUM
  where needed. Root/integration lead retain their current models.

## Feature lanes

| Lane | Scope | Next concrete result |
| --- | --- | --- |
| 01 | Frontend profiles and creation | Actual model/labels/class change, repeated Confirm, four-slot arrows, recoverable Remove/refusal, create/load/start and return through the same executable. |
| 02 | Menu scene and cinematic | Source camera/viewport/scene reconciliation, black-ground gap, class showcase timing and complete intro/menu transitions at tested resolutions. |
| 03 | Equipment and inventory page | Original Details fonts/fields/Transmute, AutoEquip on its actual source page, real equip/unequip/drop actions and same equipped character preview. |
| 04 | Skills page | Active text styles, level/prerequisite/subclass chains, current/next values, skill points/upgrades and assignment UI. |
| 05 | Faery page | Actual saved selection/unlocks/ranks, original labels/art and consistent selected ability. |
| 06 | Quest page | Same live quest state, objectives/details, completed/assigned lists and saved progress. |
| 07 | Minimap and Map page | Original map art/projection, player/NPC/objective markers, page input and updates. |
| 08 | Player and weapon combat | Consecutive/held attacks, targeting, source hit/reaction/death timing, weapon stances and generic player/class combat admission. |
| 09 | Enemy AI and population | Authored active/conditional mobs, density/spawning, navigation, aggro, chase, range/attacks, reactions and death. |
| 10 | Boss encounters | Authored boss attack patterns/phases, encounters, progression conditions, death and rewards. |
| 11 | Class skills | Actual keys1–3, source costs/cooldowns/targets, original animation events, damage and secondary statuses; shared class/skill definitions. |
| 12 | Effects and projectiles | Source weapon trails, staff/ranged projectiles, skill impact/area effects and reliable visible render delivery. |
| 13 | Rewards and leveling | Same-world source drops, gold/potions/equipment pickup, kill XP, levels/points and durable save/reload. |
| 14 | Pots and chests | Authored scene enrollment, player break/open interactions, source animations/contents and once-only persistent state. |
| 15 | Quests, dialogue and NPC interaction | Source objectives, dialogue, services, rewards and progression conditions in the live world. |
| 16 | Cinematics and tutorials | Original triggers/camera/actor movement, including jumping-monster encounter, prompts, control/HUD transitions and skip behavior. |
| 17 | Companion AI and faery gameplay | Priest/Castor identity and source following/combat where applicable; faery follow and real key4 ability/animation/effects. |
| 18 | Global audio | Source music/ambience/UI, footsteps, attacks, impacts, skills, enemies, companions, objects and cinematics. |
| 19 | Movement, collision and gameplay camera | Walk/run and Shift behavior, floor/obstacles, turns, targeting camera and source-grounded shake/feedback. |
| 20 | Animated world and campaign transitions | Authored doors/barriers/world animations, boss/quest unlocks, checkpoints, death/respawn and transitions. |
| 21 | Fidelity and integrated regression | Inspect original footage plus IDA/source, verify each reported bug/fix, replay the whole Act1 route and save/reload checks. |

Each lane receives a small current deliverable and exclusive paths. Dependencies
must have named owners; the dispatcher advances independent feature work while
the integration lead verifies current fixes. A single shared file is not assigned
to several writers. Source assets/tables drive content, not Swamp-specific logic.

## Milestone order

1. **Preview10:** close the user's existing confirmed bug gates with actual
   production tests/captures: frontend/profile/model/labels/controls, current
   Equipment/Skills presentation and actions, targeting/reactions/death,
   real skills1–3/faery4, original pause controls, and actual kill XP. The menu
   black-ground mismatch is an open visual gate. Partial helper success does not
   permit a release.
2. **Preview11 target:** visible authored pots/chests, break/open animations,
   interactions, source rewards and pickup. The current root-owned object
   enrollment/persistence gap must be closed first.
3. **Act1 gameplay/content checkpoints:** normal mob population and AI, class
   combat/skills/effects, companion behavior, original quests/tutorials/cinematics,
   bosses and world unlocks, audio, complete menus and durable progression.
   Future lanes proceed alongside current release fixes.
4. **Act1 acceptance:** play the original Act1 route from creation/start to its
   authored endpoint. Verify encounters, progression, rewards, menus, visuals,
   audio and save/reload throughout. Do not infer the endpoint merely from the
   presence of a Swamp level or one boss.

## Current evidence and gaps

- World rendering, movement, original assets/animations, basic shared combat and
  HUD exist in Preview9; complete Act1 population/behavior does not.
- Source reaction/outcome gates, death XY motion and sticky target retention
  have new focused tests. Their normal gameplay acceptance still needs review.
- Actual newer class models and Rogue label/pose render. The latest fresh
  profile-slots-main-2 run now passes occupied/empty arrows, refusal preserving
  bytes, accepted recoverable Remove, Knight/Rogue/Knight selection, repeated
  Confirm creating once, exact selected-slot load/start and metadata with correct
  Achievements text. Root/lead/reviewer verified build provenance and captures
  for EXE DB0E49873C760714E18DC0BE1FC016F7F730DB67C3F78B269D373D136482E615.
  This is a tested development fix, not a released Preview10 or full menu acceptance.
- Same-session Rogue equipment body, both daggers and gear render in the menu.
  Details actions/fonts/Transmute and AutoEquip need selected-item captures.
  The main-page empty bar alone does not prove a missing AutoEquip label.
- Skills labels/ranks render; Active remains black in the inspected capture.
  Chains/current-next values and real input casts remain open.
- Pause open/Continue/reopen/confirmation visibility, freeze and Escape have
  runtime/visual evidence; actual confirmation outcomes still need tests.
- Shared skill/spell result calculation/application and source-based reward host
  now have real-data/session tests; production hotkeys/effects/status tails and
  visible main kill XP are not complete.
- Menu ground has a proven coverage gap at multiple aspects. Original footage
  is v1.0.3; available scene/source is v1.0.2. The exact mismatch is unresolved;
  don't invent a camera shift or claim original assets alone prove appearance.

## Evidence contract

Latest root integration receipts (2026-10-10 local): configured same-Session
injury/death leaf entries now publish original AnimTable metadata to the existing
presentation observer. Strict C++17 tests pass for direct and retained playback,
3000ms admission, duplicate suppression, exact borrowed frame clocks, observer
exceptions and death/restore no replay. Actual Swamp Lizard rows are 377/374;
the report is `port/windows-foundation/reports/session-pose-step-v1.json`.
The audio worker subsequently verified those genuine entries through V42/WinMM;
the exact lizard hurt/die WAVs remain absent, so this proves diagnostic-safe
dispatch rather than audible parity. The baseline source-hit regression passes.

The effects factory now verifies exact Session identity and the current binding
lease, including rejection after same-address restore. Its native CPU runner
passes; this is not live GPU evidence. Source faery timing is also resolved:
50006 admits Cast/state7, while the actual authored `do_spell` marker invokes
OnSkill. Skill/state6 `do_skill` must not substitute for it.

The loot owner completed the modern reward-suppression gate with actual source
Level constructor default zero. Suppression consumes death once with no drop,
XP or RNG and does not reward it after unblocking. Normal reward/save-reload
still passes. The interactions owner recovered the exact external itemdrop
material from the original cache, and real-material CPU packet tests pass.
Production XP enrollment, item rendering and GUI verification remain open.

Later integrated receipts: the E11BBF current-ABI build passed220tasks and81
CTests, including the new neutral_world_save test. Its actual Rogue Unequip /
Auto-equip clicks now succeed, restore the same item instance/source slot and
two attachments, and the enabled Transmute capture has one label plus Value.
The same source build proves XP after a source kill and checkpoint restore.
Root then reproduced and fixed the separate initial selection bug: PC cycling
started at a distant low-ID actor instead of the nearest. The focused same-Session
test fails before and passes after distance/stable-ID ordering. The rebuilt
3674EF main executable proves a fresh-start near-enemy kill at frame124, one
recipient/+8XP, source drop/store1 and save600/F9760; the Stats capture shows8/100.
This requires no R/content reload. Equipment durability, item draw/pickup and
character-profile restart projection remain separate open gates.

The new neutral object subset shares the character ObjectId namespace, rejects
cross-kind collisions and adds bounded feature-owned state bytes to GameSavev2;
actor-only v1 remains wire-compatible. Actual authored chest/barrel IDs and
placement, atomic restore, old Save regressions and Session rebind tests pass.
This does not yet establish visible pots/chests or their normal interactions.
The source container codec/neutral publication/visual owner remains feature work.

Every existing/resumed/new session follows root AGENTS.md: inspect relevant
visual evidence and IDA/recovered source (including callers/gates) before behavior
edits; record expected behavior, uncertainties and a focused test. Run quick tests
when testable with the production C++17 toolchain, then verify normal production
input/rendered behavior. Distinguish source-only, tested component, integrated
runtime and accepted release. Preserve user saves/live window and immutable
accepted packages. No completion estimate is supported by worker count alone.

Selected-profile startup projection now has a root-owned reusable adapter in
`player_profile_properties.cpp`. Actual Knight/Rogue/Mage tables pass fresh and
partial vitals, allocated attributes, points/integer XP and retained live XP
fraction, repeated projection, isolated character disk saves, actual equipment
adapter changes, same-world property publication without actor/RNG replacement,
and full GameSave disk/restore. Rejected class/level/vital inputs preserve output.
The test exposed additive class-base accumulation; native reset/load/recalc
callers were independently audited and the startup adapter rebuilds its derived
base from the authored row while preserving saved/gear sheets. Report:
`port/windows-foundation/reports/player-profile-properties-v1.json`.
Main/CMake integration and normal Rogue gear/profile restart checks are handed
to the integration lead, so this remains a component-tested development fix.
Equipment's own repeated class-base recomputation is under separate source audit.

That source gear audit is now terminal: UpdateGearsProperties resets/reloads gear
and recalculates against the existing base, without the startup/base reset.
Property164 is Spell_Rating_Dodge and its additive recurrence matches source;
the equipment adapter remains unchanged. Do not conflate this with startup.

The existing visual/retained animator now supports original objects whose named
clips live inside the scene BDAE. Actual original Swamp chest/urn tests retain
two textured draw meshes each (collision helper graph kept, collider draw skipped),
change the visible CPU mesh pose through activate, deliver opened once at the
original event boundary, complete once, pause on zero elapsed time and restore
idleactive without replay. The existing retained character animation regression
also passes. Report: `port/windows-foundation/reports/embedded-object-visual-v1.json`.
This is shared renderer/animation support; same-world production enrollment,
normal interaction routing, GPU captures and container save/load remain open.

The production profile projection build passed and the actual Rogue GUI now
retains source HP146.797/MP40.25 across Unequip/Auto. Its R/F9 check exposed a
separate Session startup defect: a set of ItemTable definitions collapsed the
two Dagger01 instances and projected the surviving row only as offhand. Root
reproduced this with a failing actual Session test, then preserved every equipped
occurrence and assigned distinct main/off occurrences. The new regression now
passes source per-slot gear equivalence, strict Session reinit/checkpoint restore,
and legacy selector completion. Source result and direct/retained injury/death
regressions pass again. GameSave compatibility checks remain unchanged. Report:
`port/windows-foundation/reports/session-equipment-multiplicity-v1.json`.
Lead owns the coherent production menu/equipment/F5/R/F9 rerun; durability stays
open until that normal path passes.

The normal production Rogue path now passes on frozen executable
8635E151F9F2F5CF6BDE85629178C9108F961AEF81AFD728BB3C10C5311CCAFF:
selected source HP146.797/MP40.25, actual menu Unequip/Auto revisions6/7/8,
F5 at frame40, level reload at80, F9 at100 with HP146.797 and RNG8504954/20,
and stable final health at120. Root independently read the log and checked the
executable hash. This closes that reproduced fresh-Rogue equipment/vitals
checkpoint defect; other loadouts, class flows and nonzero progression remain
separate checks. The same binary's skill-key test correctly rejects empty key2,
but key1 exposed an active visual-versus-Character roster mismatch; its owner is
fixing the same-world subset projection while preserving authored order.

That Rogue key1 path is now integrated on CD84E2800DE827AD39FF316296C8273B4D46752FA2131BAE36E7E384E6B749A7:
key1->source slot0 JumpKick debits MP40.25->30.25, actual do_skill event at37
applies one result, and completion reaches59; empty key2 does not debit. Root
read the exact log and inspected the frame38 image showing the extended kick;
lead also captured20/60. This is production behavior/render evidence, with no
matching original Rogue cast footage yet. Rogue/Mage still use animationOnly
receiver traits, so enemy targeting/full class combat remains a separate gap.

Root now owns neutral object presentation inside the same CombatSession rather
than a second world or fabricated ActorState. Source binding order is retained;
the sole Session.update advances embedded original poses/events/completion,
with source scene flags. Actual level chest/urn tests cover callback clip changes,
zero-time pause, no presentation RNG, same-world components/save restore,
silent saved-state pose rebind, failed-consumer prefix/no replay and safe removal
during dispatch. Detach discards transient object borrows/callbacks; feature
owners rebind after restore. Report: `reports/session-object-visual-v1.json`.
Interactions is composing its actual source container owners, same item store
and RNG onto these APIs; production GPU/normal input remains open.

An explicit source calculation-only continuation now shares the Session hit
occurrence dedup and World sheets/RNG while allowing an already-dead target.
The actual Session test compares original formula payload/RNG, preserves corpse
health/pose, rejects foreign/detached/rebound owners and suppresses duplicate
rerolls; presentation failure is non-vetoing. This supports Celest's unconditional
second SpellCombatRoll after a lethal first roll without repeated health/death.
Report: `reports/source-calculation-only-v1.json`. Faery owner must verify the
real source-script caller and FX order. Injury/death and dual-gear regressions
pass again. Old DEFAULT/Fake_Hotty fixtures are invalid original playable-cast
evidence; corrected source class lists/Hotty row and fresh Celest slot are required.

Generic source Skill6/Cast7 departure now has same-Session runtime tests and
independent review. Normal Injury50010 cannot interrupt Cast and only interrupts
Skill under its actual focus bit16; direct-mask Injury/death cancel the retained
cursor, perform Post once before incoming pose focus and prevent stale Use after
recovery. Pre/post-Use and same-state exits, failure/duplicate/lease guards and
240-frame no-replay/RNG checks pass. Controlled formula ratings in the fixture
exercise outcomes; they do not establish ordinary Lizard balance. Active casts
and unpersisted cooldowns reject checkpoint/legacy cleanup; expired quiescent
owners can restore through a pre-detach admission witness without clearing
unrelated observers. Animation-only source hosts return to authored Idle/state3.
Report: `port/windows-foundation/reports/source-sequence-departure-v1.json`.
Coordinator's connected private cast/checkpoint/GameSave test also passes, but
normal main/key4 GUI and real incoming player behavior remain integration gates.

The normal key4 caller now reaches source Celest/Spells0 and debits mana, but
Use exposed a current-frame timer ordering defect: main advanced timers after
the retained callback. Root added a separate actual frame-begin phase before
inputs/AI/animation, preserving diagnostic AI and existing clock ownership.
Focused same-Session marker/timer/zero-time/invalid-frame checks pass; lead is
moving its clock updates there and owns the fresh normal key4 completion test.

Latest incoming receipt: the opt-in same-Session receiver tests pass eight real
Rogue/Mage cases, including all original Type2 Injury leaves, shared normal RNG,
gate suppression, source death and strict corpse save/rebind. The independent
reviewer reran frozen06DACE and source-departureB726 successfully. Frozen769F
normal Rogue gameplay now receives actual nearby Lizard marker damage, and its
F5_320/R340/F9_350 run preserves HP87.8555 and RNG2037220/96 after360frames.
This is actual incoming capability, not full Rogue/Mage outgoing combat or
source spawn activation. The cast rerun exposed a separate GetTargetPosition
node-null/cache-zero bug; feature fixes are independently tested, awaiting
coherent production refresh. Source attack-bank and separate main/offhand
marker integration are current root work. Report: incoming-receiver-v1.json.

That source attack-bank seam now passes actual Rogue same-Session source245,
twelve leaves/four held groups and eight named main/offhand events with exact
source result/RNG comparison. The same actor/retained pose/gear/target survives;
strict save/rebind and invalid-bank atomic rejection pass. Root source-hit,
incoming8 and gear multiplicity regressions pass again. Source level20 Lizard is
only the continuity fixture, not Act1 balance evidence. Main normal outgoing
Rogue is next; dynamic moving/static root choice, equipment bank switching and
Mage projectile remain open. Reports/session-source-attack-bank-v1.json records
the exact private executable. Frozen769F also now proves normal Mage incoming
damage (HP141 to82.0586); its visible overlapping bodies led to a measured
regression identifying the unbound world Step/contact dispatch. No guessed
enemy standoff radius was added. Root owns that shared physical step next.

The next coherent production build F2429458E5FA914547E9F5E226268AB142093D1F0318CEDF1639DA241BE979E6
completed94tasks and all five focused CTests (including training86, incoming87
and source-bank88). Actual normal key4 now uses source Player14/Main13 effects,
correct target-position fallback and retained spell event26: two rolls reach the
nearby target, not all offscreen mobs. The360frame run exits0, emits242 render
packets over80frames using four real textures, reports no presentation failure,
and preserves HP70.6055/RNG8051465/116 through F5_320/R340/F9_350. Root read the
current log, verified the frozen EXE hash and inspected actual20/27/65 captures:
cyan preglow, visible lightning/ground glow, then dissipation. This is production
GPU delivery, not original Celest visual parity. The strong rectangular ground
glow boundary is assigned a source material/blend fidelity investigation;
overlapping bodies and complete outgoing class gameplay remain open. Preview9
is still accepted; no Preview10 freeze is authorized by these component passes.


The concrete 21-role owner/status/dependency matrix and live roster are maintained in [act1-current-allocation-20261010.md](act1-current-allocation-20261010.md). It records allocation separately from active concurrency.
