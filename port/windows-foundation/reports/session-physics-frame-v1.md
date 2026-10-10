# Shared body stepping and source position publication

Investigation before implementation. The actual Mage incoming screenshot/log
in profile-projection/mage-incoming-normal shows two Lizard bodies overlapping
the player. The source-radius regression measures penetrations161.940/138.823
and74.202; this is current implementation evidence, not original-game behavior.
The run explicitly logs world Step unbound. Existing original-video evidence
does not isolate this contact; no original visual separation timing is claimed.

Source Level::Update runs PhysicalWorld::update at0x3f84c8 before ObjectManager
actor updates at0x3f84e8. PhysicalWorld0x34bd08 uses GetDt milliseconds times
float0.001 and Step(dt,10). Later GameObject::UpdateSubObjects0x3943cc observes
the genuine body: flags0x8 is SLEEPING, not the separate pinned byte. If awake
and either gameXY delta exceeds1, it imports bodyXY; the actual virtual floor
predicate chooses PF validation, whose rejected/clamped point is still kept.
Current actorXY is published back to body later. NativeBody query/observation
and the existing PF/source setters already provide these source operations.

Pin/unpin is separate mass/inertia policy: construction pins source characters;
Move focus0x3c3bf8 unpins, Move blur Stop+pins; Attack focus0x3c404c pins static
attacks and unpins moving attacks. Stepping still-pinned bodies is insufficient
to fix the current overlap. The earlier audit's wording 'pinned bit8' is
explicitly corrected; native sleeping and pin state must not be conflated.

Minimal reusable infrastructure: the existing PlayableActorBodies pool steps
its actual NativeWorld once for an explicit current frame/dt, validates current
ActorState records through the caller's real registry, rejects foreign/stale
worlds and reentrant body destruction, then exposes a separate source position
reconciliation operation for the correct actor phase. It receives the actual
floor predicate rather than guessing from a class/state. The existing whole
position setter maintains shared actor fields, destination policy and AABB.
No animation, HP, target, RNG, private native object graph or second world.
Contact services remain mandatory; they must reach the actual modern source
contact owner. Missing services are not replaced by no-op success.

Focused verification defined before coding: genuine source Mage/Lizard body
plans in one CombatSession; dynamic versus pinned mass and actual sleeping
observation; source filter/contact events from real Step; solver movement is
published to the same actor with the >1 threshold, preserved Z/destination,
floor acceptance/clamp, no HP/target/RNG changes. Duplicate frame does not Step,
stale/malformed/foreign registry rejects before effects, source body removal is
absent rather than fabricated, and reached callback failure keeps its prefix.
Source Box2D linear slop is an explicit geometry tolerance, not guessed spacing.

Remaining integration: full state focus/blur pin transitions, modern AIS contact
effects, normal frame phase, floor/visual/root-motion chronology and live GPU
overlap rerun. This infrastructure test alone cannot close those gameplay gates.

Independent test passes with actual Mage/Lizard same-Session bodies and original
Swamp floor/PF world. It covers awake/sleep versus pin/mass, real contacts, frame
guards, same-actor position publication, missing-navigation atomic rejection,
valid-floor acceptance, outside-floor cached-point clamp, reached-callback
failure/no replay and safe reentrant-release refusal. Private runner:
tools/run_playable_actor_physics_frame_tests.ps1; log:
.local-inputs/playable-actor-physics-frame-test/run.log; executable SHA256:
366C6534407FC857A5330376EDD9824B9AA069935D4FA83FC6EA3533BF7A36C5.
Root inspected log/hash and test branches. Production phase composition, state
pinning and contact effects remain open.

Next source-phase investigation: original subobjects_update.cpp/NOTES.md
imports awake body XY before `visual_apply_position`; accepted physics position
suppresses visual root displacement for that phase. Its false-floor policy has
late validation after visual/rotation sync, distinct from early validation when
validating_floor==1. Current Session MotionHandler applies authored root deltas
immediately during retained sampling and main publishes native body at once.
Thus enabling Step plus a late reconciliation is not sufficient and may erase
solver movement or apply root twice. The modern integration must preserve queued
root displacement and apply it at the actual per-actor phase after physics
admission, retaining event/hit chronology, same pose/motor/body and real source
floor/heading flags. Full source GameObject.Stop separately drops the existing
path, copies current destination, clears heading activity while retaining angle,
and only performs native Stop under its position-from-physics predicate. No
new native object graph is needed to represent these modern state/effect fields.
Source references: GameObject0x3943cc, gameobject_stop_source_v111.cpp3938f8,
source-focus-transition-integration-audit-v14.json. Root owns the next seam;
production Step remains disabled until the composed behavior is tested.

Implementation/test plan before edit: add an opt-in Session actor motion phase
which collects actual retained/direct authored samples in their per-actor order
instead of invoking the immediate motion handler during animation. After the
existing animation/event pass, invoke the actual current actor handler once per
Session frame with that immutable sample list, actual serial/dt and moveGO facts.
Run the phase even for actors without a root sample; physics publication does not
depend on walking. Existing immediate behavior remains the default. Do not create
new actor/pose/HP/target or infer source focus/pin flags from input. The host can
compose the existing source body/PF/visual owners in the proved import-before-root
order. This seam does not claim original global ObjectManager traversal order.

Verification defined: same Session with original player/enemy poses and actual
NativeWorld/body/floor owners; enabled handler sees root samples without prior
Actor movement, then imports actual solver/body XY, suppresses root on accepted
physics branch, or applies source movement when that branch rejects. Original
marker hit/RNG occurs before the actor position phase. Multiple samples preserve
their order/moveGO; no-sample actor still gets phase; zero elapsed never invents
displacement. Missing/rejected handler is a reached error and delivered prefix
cannot replay on next frame. Pending pre-frame samples block checkpoints and
cannot be discarded by switching handler; detach discards transient borrows and
requires fresh phase binding after restore. Reentry/frame mode changes reject.

Adjacent modern lifetime investigation: contact owns_binding() and projectile
synchronize() access a borrowed Session reference before knowing the Session
still exists. Actor binding tokens may be retained by body/feature borrowers,
so weak binding expiration alone cannot prove object lifetime. A small read-only
Session lifetime witness belongs to the existing Impl, survives same-object
initialize/restore, and is explicitly invalidated by destructor even if a caller
pins the witness. Features must check it before touching their raw Session
reference. This adds no original gameplay/state/callback semantics and changes
no public Session object size. Test defined: pin lifetime and actor binding,
destroy Session, verify witness invalid and reached feature calls reject without
dereferencing it; ordinary reinitialize/restore keeps object lifetime valid but
still invalidates stale actor bindings. Body cleanup remains caller-owned.

Final private motion-phase test passes on executable SHA256
6F734CC3111B3E37993E41DB81FE510988777F42098A205FC8F9EE432C349A72.
Root inspected the actual test, PASS log and final hash. The fixture uses the
original Rogue moving bank and asserts nonzero authored root metadata, an
original level-20 Lizard only for action continuity (not Act1 balance), real
NativeWorld bodies and the original Swamp PF floor. It exercises both accepted
physics suppression of roots and no-import root movement, actual hit/result/RNG
before phase delivery, no-sample/zero-time callbacks, mode/reentry/checkpoint
guards, reached failure without replay and fresh binding after restore.
Its world Step occurs in its phase callback: this proves the reusable seam and
both position branches, not original global Level order or production gameplay.
Lead has the coherent-build handoff; focus/blur, contact and host composition
remain open until normal runtime verification.

Next bounded Stop consumer investigation: source GameObject.Stop3938f8 drops
the actual PF path, copies current position to destination, clears requested and
heading fields, then calls native Stop only when a physical receiver exists and
the actual virtual updating-position-from-physics predicate accepts. Existing
`dh2_native_body_stop` already executes linear0, angular0, current gameXY
SetXForm, PutToSleep; source physical-controls instruction audit proves this
order. Main needs a guarded pool entry point to that exact existing kernel,
not hand-written Box2D mutation or a second path/controller owner. Add
`stop_physical(id,currentActorLookup,stopped,error)`: validate the registered
current actor, forbid calls during Step, borrow the actual physical owner for
the synchronous call, preserve mass/pin and all Actor gameplay/destination
fields, and report absent physical as stopped=false. Caller retains the whole
source Stop prefix and physical-position predicate. Test with real same-Session
Mage/Lizard bodies: velocity/angular reset, sleep, unchanged actor/destination/
HP/target/RNG/mass/pin, currentXY reseating, stale-owner/Step rejection and an
actually removed physical receiver. This is infrastructure with no additional
footage timing claim; normal Stop/movement verification remains lead-owned.

Accepted-transition investigation before the next core edit: the actual normal
Mage capture above still demonstrates overlapping live bodies; enabling Step
alone would leave construction-pinned bodies immovable. Source Move/Attack
Focus and Blur recipes above supply pin/unpin/Stop effects at actual admitted
state boundaries, not every retained leaf/frame. Source `_SetState`0x3c1938
also executes Blur/Focus on an explicitly admitted same-state request. Current
modern Runtime::begin validates clips/markers but CombatSystem::begin commits
action/target/cooldown before selection. Runtime release/finish clears target
and resets action before a late observer could capture outgoing facts. Injury
has a separate outcome/3000ms/exact-state gate, and restored corpses must not
emit another live admission. Generic Skill/Cast departure already has a Post
owner and its reached-prefix semantics must remain unchanged.

Implementation decision: reuse the existing World/Runtime/pose owners. Add a
side-effect-free attack admission check and bounded before/after transition
hooks with captured source state and occurrence, before destructive action
changes. Source recipes stay in the host consumer; no native state graph or
second FSM is required. Missing or failed consumers are explicit reached
errors, not logging-only acceptance or automatic retry. Teardown/restore
remains silent and drops transient hooks; the host binds the new lease before
frames. Focused tests will cover rejected admission without callbacks, genuine
entry/completion/interruption ordering, accepted injury/death versus ordinary
damage, no held-leaf reentry, failure without replay, and silent restore using
actual player/enemy assets. This is an internal consumer seam; no new original
footage timing proof is claimed. Normal body/contact/movement verification is
still required after the core and host integration.

Normal Skill/Cast completion correction, investigated before editing: the
retained closed callback owns `stateServices.finished`; accepted interruption
owns `stateServices.departed`. The existing skill coordinator's
`source_departed_v1` marks its generation interrupted, while `complete_v1`
requires its authored Use to have fired and explicitly rejects an interrupted
generation. Calling departed and then finished on ordinary completion therefore
breaks normal skill completion. Keep accepted Blur first, retire the completed
cursor's occurrence without interruption, run normal finished/Post while the
outgoing World state remains available, then publish the incoming state and
Focus prefix/suffix. Failure preserves the reached prefix and forbids retry.
This is an internal sequencing correction; it does not claim new visual proof.
Focused verification: actual Skill347 playback for states6/7, one finished,
zero departed, exact Blur/Post/Focus order, plus failed Post without replay;
the existing interruption tests must continue to pass.

Attack physical-branch handoff: original Attack Focus chooses its moving or
stationary authored root before unpin/pin (recovered character_state.cpp focus5;
player uses previous state4, enemy uses availability of AttackStatic). The
Session already prepares that exact root before Runtime admission. Append its
semantic choice to the accepted receipt from `sequenceCallerSelection.state`
only for recognized Attack/AttackStatic roots. Keep that optional value with the
same pending occurrence through Blur/Focus; do not infer it from the sampled
clip, keyboard state or actor class. Focus suffix can now apply the selected
body policy without duplicating animation selection. Focused source-bank tests
must assert false for Idle3/AttackStatic and true for Move4/Attack; other state
transitions have no attack-root fact.

Root inspected the strengthened test source and terminal artifacts:
`.local-inputs/session-actor-transition-test-final3/` executable SHA256
71E8CFE100DD37C9EFF7EB87F51519B74D2D0EBC8C29BE43479796B46D1D9743,
PASS log SHA256
4DA5B4ABF166FD7281D18426AF8734D00B3E0EA296F4DB366E115F8C1D59220B.
The test uses actual predecessor-selected Knight Attack/AttackStatic graphs
and preserves the selected optional fact across all transition stages. The
root-selection cases isolate selection from combo continuation; they do not
claim complete held-combo gameplay. Actual Skill347 playback exercises generic
state6/state7 normal completion with one finished/Post and no departed, old
World state available during Post, and failed Post without Focus or replay.
This proves the internal completion ordering, not all skills or Cast art.
The existing source-departure regression also passed with a fresh current
CombatSystem/ActorState source closure: executable SHA256
A07A28F2B928448EF8257B676C027A764EEBC55C83BC9CD9B413E4F8C6A7FF9C,
log SHA256
48744312F6D8557E5D8E4DCA334C0C713FA8AB9A82E8DFDDF0DAF620FC689854.
Its old standalone runner omits current CombatSystem symbols; that stale
runner link failure is not a gameplay regression. Normal EXE physical
Step/contact/transition integration and visual verification remain open.

Root repaired the standalone source-departure runner to compile current
CombatSystem/ActorState and snapshot its archive dependencies with hash checks.
The repaired runner itself executed successfully at
`.local-inputs/session-source-departure-runner-fixed/`; its focused interruption,
direct/death Post, cancellation, lease and checkpoint cases pass. The previous
stale-runner limitation above records the earlier failure, not the current tool.

Follow-up source-bank/cooldown regression was independently inspected by root:
`.local-inputs/session-actor-transition-followup-final/` executable SHA256
8C410F17D458F843BE338AD07C9FDCEF6B5A47B7C556495D7A39522BE49B56D6,
PASS log SHA256
5A5DC9AF87AAF8C0461163C7926BA8422AD651C919806293450AAAF749400538.
The actual equipped Knight full bank with sourceCombo enabled retains target2
and selected actor2 through groups0/1/2 and normal completion. The actual Lizard
AI row30 supplies AttackDelay800ms; after its39-frame attack, a new action is
blocked at departure and750ms later, then admitted at810ms. Blur observes the
outgoing action/target, while suffix follows cleanup. This proves the modern
Session departure interval with real authored data; original TMR event0x2a and
separate gate528 delivery remain outside this test. Enemy level20 is a
continuity fixture, not evidence for Act1 balance or population fidelity.

Target-command extraction investigation: the existing explicit targetSelect
input block sorts eligible current World actors by distance/stable ID and commits
the player's current target and sticky intent. Main currently executes this
after frameBegin, while the contact controller's distinct preferred/current
target projection must be updated at the accepted command, not mirrored every
frame. Existing RuntimeEnemyControllerV1::set_contact_target implements the
audited flags0 preferred-before-current setter on that same Session controller.
Keep the existing PC cycling adaptation and extract its calculation into a
small public select_next_player_target command, with no frame/clock/animation
advance or RNG draw. Legacy InputActions calls the same implementation; the
modern host may call once after Step/current clocks, publish the selected intent
to its existing controller, and consume the input edge to avoid a second cycle.
This is internal transport for a deliberate PC adaptation, not a claim that
the original touchscreen game cycles with Tab. Verification defined before
editing: direct command and legacy input agree on nearest/tie/wrap/permission,
no serial/pose/vital/transform/RNG change, frameBegin use does not advance twice,
detached/reentrant commands reject, and a real held attack still damages the
selected enemy without a reload. No new callback/state owner is introduced.

The current-source focused target runner passed in
`.local-inputs/session-target-command-test/` (executable SHA256
4D96327BFCD7E161D85D9D70D7F1C4AFA45A30B6C055594E4C6E5934334F0CA9,
log SHA256
89047DE269F293A9924D7706A18A74FC639EDAEFA6A65883D296334428D2EE90).
Root executed and inspected direct/InputActions nearest/tie/wrap selection,
one frameBegin command and one frame, preserved RNG/vitals/position, accepted
transition reentry rejection, empty-permission clear, unbound/detached rejection,
and actual selected-enemy held-attack damage. Its standalone runner now compiles
the current Runtime/CombatSystem/ActorState closure and snapshots link archives.
Main's queued controller target publication is staged by lead, not yet verified
in a normal enabled-physics executable.

Root also inspected the terminal Skill6/Cast7 consumer source/tests and actual
same-Session native-body artifacts: executable SHA256
1EAC30181FC0513A6E9E5C2919CACE01E47035C198487A4916B68349B87AFFC9,
log SHA256
CA2375CDE52AD7A10504E20717BB36A6A95945B6385F21F65C2FE348F782B3B0.
Actual Knight SkillTable7/root347 and alternate row51 exercise both moving-byte
gate branches, unpin/immediate-pin versus timer request, while Cast root348
preserves pin. Normal/interrupted Post and duplicate suppression pass with the
same current body. The timer-expiry test explicitly advances20ms and invokes a
manual source witness; it does not prove a production scheduler or exact10ms
expiry. Lead must bind real current coordinator/table/gate and a measured
one-shot timer, pin only current Idle/body on expiry, reject checkpoint with
unpersisted pending timer, freeze during pause and discard stale restore lease.
No new accepted package or full visual/playable completion is established here.

Normal integrated launch investigation: the frozen1EBCA6 candidate passed97
tests but its first fresh enabled-physics launch terminated during overlapping
player body construction, before frame binding. NativeWorld::create_character
calls CreateShape/SetMass, which can synchronously call ShouldCollide. The
existing pool publishes Entry only after initialize returns, so an already
registered peer cannot resolve the actual constructing PhysicalObject context.
This is a modern registry lifetime/order bug, not an original no-collision rule.
Loan the same stable Entry/PhysicalObject identity in the existing registry
before allocation, keep it available through cleanup, and erase on failed
admission only after physical release succeeds. No duplicate registry, guessed
peer cast or no-op filter is introduced. The source physical allocation order
and filters remain unchanged. Define immediate test before editing: real
overlapping Lizard then Knight bodies, synchronous filter resolves both actual
receiver IDs including the pending actor; invalid-plan rollback removes only
the candidate; successful release preserves identity during callbacks and leaves
no dangling context. A separate backend failure-safe allocation test is assigned
to the physical owner for strict provider failure before body return. This is
internal lifetime infrastructure with no invented visual timing claim; rerun
the exact failed normal sandbox after both fixes.

Root's immediate current-source overlapping-body test passes: actual Lizard
then Knight allocation produced10 filter calls and3 pending-peer resolutions;
actual Step delivered2 Add callbacks and release delivered2 Remove callbacks.
An invalid post-allocation source plan rolled back only its candidate, removed
the context from the same registry, and restored native body count. Both
existing actors retained HP/target, and final clear left no registry/native leak.
Executable SHA256
EE955602DE76ACF36DFA7C33C79E1007282F8759E4C066A3310CC88E2FBE5443,
log SHA256
8DEB0421E0D458C51197E5912F1F580E8D39049D3B91A6C6F78BE8788FEEE6E3
in `.local-inputs/actor-body-construction-test/`.
Root also inspected and executed the separate source-terminal NativeWorld
creation failure test5345A161F83AAA457E20B6C022A0E2E26EC62068B3663F2DC5467CE207AD7151:
provider failure cleans its partial body, ordinary denied collision succeeds,
repaired retry and subsequent Step succeed, and orphan count is zero. Its
cpp-local cooperative creation scope changes no public layout and leaves
ordinary Step/destroy behavior intact. Normal GUI startup still needs the
fresh coherent build; the previous1EBCA6 failed artifact remains preserved.

The construction-fixed normal retry reaches all25 bodies and then a clean
initial-state guard: pure presentation actor2423516001965560686 has World
state-1 while Session reports3. Investigation: initialization has explicitly
prepared its configured Idle source root, but the presentation-only facts pass
deliberately skips HP/action projection to preserve authored vital sentinels.
Keep that preservation and publish state3 once from the actual initialIdle
choice only when no explicit originalCombatState was provided. Presentation
state queries use that canonical fact, so zero projected HP or a diagnostic
Actor.action cannot turn an idle NPC into Dead. Explicit source states remain
untouched; state programs/lifecycle overrides remain separate authorities.
Define focused test before editing: original Lizard presentation-only profile
with no attack/damage capability, prepared Idle, preserved raw source vitals and
targetabilityfalse; initial and restored canonical/Session state agree even at
projected zeroHP, no transition/clock/RNG replay; explicit17 stays17. Then rerun
the exact normal startup. This is source-data projection, not synthetic Focus
or a reconstruction of the original native object graph.

Focused presentation-state test caught the restore path too: GameSave
deliberately normalizes transient source facts to-1, so the same prepared
presentation initial-state definition must be re-published during silent
rebind. The test now passes initial canonical Idle, unchanged authored
property/vital sheets and no added target/attack capability, explicit17
preservation, zeroHP/action independence, and silent restore with no transition
or RNG replay. Root executed the current-source private test at
`.local-inputs/session-presentation-state-test/`. This preserves the existing
save format; initial presentation state comes from its loaded definition, not
from a serialized native FSM or an HP-derived fallback. The original97-test
normal candidate had no presentation-state/overlapping-allocation coverage;
these regressions specifically cover the two normal-launch failures.

Preview10 regression closure (2026-10-10): the presentation canonical-state
change exposed CTest85's legacy finite generic completion publishing its old
World6 after retiring the state policy. The loaded presentationInitialState is
the explicit completion destination for a pure animation-only/non-damage actor;
it also replaces HP-derived Idle/Dead selection on the accepted-transition path.
Ordinary gameplay completion and Post order retain their existing owners.
The current private source-departure runner passes; EXE SHA256
84926718FFD8DF5A8F3965305A11D1C0C2CF83DE998A91DA9F0C654AA3C194D2,
log849086E34252609FE7B440A262815EE4763544EB5C69C5677BCA4848E8BEEABF.
Full coherent production CTest85 subsequently passes.

Held Attack facing reuses features/combat/target-facing-20261010.json's original
video frames and CSAttack::OnUpdate0x3c14f8 investigation. Selection remains
selection-only. An admitted active attack now runs bounded LookAt on held-input
frames too. The existing private Entry records the current update serial so
admission and update cannot each spend the same frame's elapsed rotation budget.
No public/save ABI changed; the serial resets on rebind. Both original Knight
and Rogue profiles pass the nonaligned same-Lizard source-kernel comparison,
fresh admission single-budget assertion, held tracking of the live moved target,
and unchanged target/generation/RNG checks. Private EXE SHA256
AC56040DDD8A8C41B8A29569FC343E7DEB93E8E4641C4D688EA53B3C6540EF71.
Normal production rendering acceptance remains owned by the integration lead.
