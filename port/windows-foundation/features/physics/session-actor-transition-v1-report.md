# Session actor physical transition projection v1

## Evidence and expected behavior

This is an internal effect adapter with no direct original visual counterpart;
no gameplay video or continuous screenshot sequence was supplied for this task.
Accordingly, the report makes no visual-timing or in-game fidelity claim. The
source state report `port/level-world/reference/character-state/NOTES.md`
identifies `_SetState` at `0x3c1938` (outgoing blur, state assignment, incoming
focus; same-state changes still run both) and the exact physical transition
rules. Move4 focus `0x3c3bf8` writes flags `0x23c1`, movement type 0, publishes
movement, then unpins an existing body. Move4 blur `0x3c3aa4` calls complete
`GameObject::Stop` at `0x3938f8`, then freshly reloads body `+0x2dc` and pins
it. The Stop source first drops the PF path and copies the current PF position
to its requested target (`source-motion-flags-v14.json`), then copies current
position to destination, clears movement/path-request/heading-active state and
zeros heading direction. The physical Stop gate is the source
`IsUpdatingPositionFromPhysics` query (Character flags `+0x520`, bit1/mask2),
not the in-house `CharacterAction` value. Move4 focus writes `0x23c1`, whose
bit1 is clear, so the real Move4→Idle3 test skips physical Stop and preserves
the body's velocities while applying the subsequent Pin. `DropPath`'s source
copy of a nonempty route's target is not implemented by modern route-cache
release; it remains part of the caller's typed Stop provider.

Idle3 focus `0x3c3020` reads byte `+0x538`; when clear it stores flags `0x2380`.
Idle3 blur `0x3c2d3c` clears that byte. Attack5 focus `0x3c404c` stores flags
`0x2341`; the actual prepared `Attack`/`AttackStatic` root fact on the same
Session transition receipt determines unpin/pin after animation publication.
The fact is identity-checked across receipt stages; no clip/predecessor guess is
used. Attack5 focus also queries actual GetAttackDelay and sets gate528 bit1
when nonzero. Its blur `0x3c3f74` has a separate nonzero-delay timer prefix
before pinning; both are supplied by typed gate/timer providers. Dead12 focus
`0x3c4d50` stores `0x241`, plus
`0x2000` for an actual player; when a body exists its source physical filter is
`(group=0, category=0x51c, mask=3, applySecondary=false)`. Dead12 blur
`0x3c499c` resets the body's saved filter when present. Injury11 focus
`0x3c33e8` stores flags `0x2b41`. Caller ordering and lifecycle tails are
covered in the same source NOTES; their controller, animation, timer, FX, buff,
event, speed and cleanup work remains with existing owners.

The reusable projection therefore consumes only the source flags/type cells,
the current PF route release and caller-provided remainder of `GameObject::Stop`,
predicate-gated physical Stop, same-current-body pin/unpin and typed physical
filter leaves. It does not require a native FSM owner graph and does not replay
gameplay lifecycle work. The prepared Attack branch is a same-receipt source
fact. Physical filter arguments, Stop field mutation/predicate, Idle
suppression, Attack gate/timer and source IsPlayer remain typed inputs; missing
reached providers fail closed. No gravity or
unverified Z behavior is introduced.

## Verification

The focused test is
`session_actor_transition_v1_tests.cpp`; invoke it with
`run_session_actor_transition_v1_tests.ps1`. It creates an actual source Knight
player and Swamp Lizard enemy in the same `CombatSession`, binds both actual
body plans to one `NativeWorld`, and records the same retained body/PF identity.
For each actor it checks Idle3->Move4 then Move4->Idle3: flags/type, source
Idle-byte ordering, Move focus unpin, route-release-before-Stop callback order,
the source bit1-clear predicate skipping physical Stop, subsequent fresh-body
pin, retained physical/PF identity and Idle flags/type. The fixture has no
populated modern route-cache entry, so it proves the release call ordering, not
eviction of a nonempty route.

The extended branch test issues a real player Attack request using the actual
Knight/Longsword `Attack`/`AttackStatic` bank and predecessor selector. It
checks flags `0x2341`, the same receipt's prepared `AttackStatic` root fact,
typed Attack delay-gate admission and actual-body pin. It uses a real source-calculated
melee result carrying Injure bit `0x10` to enter enemy Injury11 and checks flags
`0x2b41` plus unchanged body identity/pin. A known-good lethal result
(`mask=0x20080000`, `category=-1`, `direct_amount=ceil(health*256)`) enters
Dead12, checks nonplayer flags `0x241`, applies the exact source filter through
`PlayableActorBodies::set_source_physical_filter` to the same current Lizard
body, verifies group/category/mask `0/0x51c/3`, retains body identity and pin at
transition entry, and suppresses duplicate-hit replay. A separate
fixture removes the receiver before applying the lethal result and verifies
Dead12 skips filter work when no physical receiver is current. This deliberate
precondition test does not remove a body at HP0 or on Dead entry.

The standalone `tests/source_physical_filter_ops_tests.cpp` runs against real
Knight and Lizard bodies in one `NativeWorld`; it verifies set/reset/repeat,
disable/enable interaction, stale/missing owners, and rejection during native
collision delivery while preserving actor and body fields. It isolates the
body primitive rather than the Session transition. The Session test supplies
the same-current actor lookup and reaches the exact source `setFilter` operation
on Dead admission.

The `stop_after_route_drop` fixture is only a witness for caller-owned Stop
fields and the source flags520 bit1 predicate; it is not production
GameObject::Stop integration. Attack delay/timer, Dead reset transition, and
initial Idle/Dead hydration remain caller-provided/integration leaves (the
standalone body test covers the reset primitive only). No
main/CMake hookup or same-window in-game verification is claimed. Gameplay
tails intentionally outside this projection are controller LookAt/lock,
highlight changes, attack delay timer and gate, attack animation/speed and
CancelSneaking, Injury look-at, Dead FX/buffs/events/timer/despawn and body
detach on Dead Event22. Existing runtime owners must continue to handle those
effects.

## Skill6/Cast7 physical extension investigation (before implementation)

No original gameplay video or timestamped screenshot sequence for Skill347 or
Cast7 was supplied in this task. This adapter has no independent visual output;
the physical observables are flags, body identity/mass/pin, route/Stop, and the
real retained source clip. The upcoming same-Session test uses the authored
Skill347 and Cast bank, but that is isolated runtime verification, not visual
footage or in-game acceptance.

The source evidence is the existing ARM32 export
`port/level-world/reference/character-skill-state-v4/original/reference/original-functions.asm`,
the state table in `port/level-world/reference/character-state-methods/NOTES.md`,
and the recovered lifecycle handoff
`port/windows-foundation/reports/source-sequence-departure-v1.md`. `_SetState`
`0x3c1938` orders old Blur, state assignment, then new Focus. CSSkill Focus
`0x3c4480` sets flags `0x6341`, clears Character `+0x528` mask `0x140`, raises
event `0x1e`, sets animation/speed and clears byte `+0x412`, then runs
CancelSneaking. It reads byte `+0x554`; if nonzero it ORs gate bit `0x100` into
+0x528. It then unpins an existing body. The caller `SM_SetSkillState`
`0x3c6670` writes that byte from its moving argument; the recovered caller
`AI_SkillMode` supplies the low byte of SkillTable scalar word2. The current
Session receipt's generation must therefore be joined to the actual selected
SkillTable row by its caller; actor/action/animation name is insufficient.

CSSkill Blur `0x3c434c` first syncs last target, calls full `GameObject::Stop`
`0x3938f8`, raises event `0x1f`, then reads Character `+0x528` bit `0x100`.
When set it starts timer `10` with event `0x30` and does not pin in this branch;
otherwise it pins the current body if present. Its nonphysical timer/event
delivery stays with the typed current owner. CS_CAST Focus `0x3c39d0` sets flags
`0x6301`, raises `0x20`, sets animation/speed and clears byte `+0x412`, then
CancelSneaking; it contains no Pin/Unpin. CS_CAST Blur `0x3c3934` only raises
event `0x21` in the inspected body and contains no Stop or Pin/Unpin. These
rules are independent of authored animation names. Unknown source SkillTable
row, gate528 owner, Stop fields/predicate, or timer owner must reject at the
reached physical prefix; no default moving byte or fake timer success is valid.

Expected physical result: Skill Focus unpins only after actual clip publication;
Skill Blur releases the modern route and runs the caller's exact Stop prefix,
then queries the current gate/timer provider and pins only when that provider
reports the source no-timer branch. Cast focus/blur publish flags and no body
pin change. Both normal completion and interruption must preserve one same
current body and avoid duplicate physical effects. Focus/blur callbacks do not
replace the retained animation owner, source Post, Use, timers, cast gameplay,
or controller/Fx work.

Verification is planned against the actual Skill347 root and a real Cast-bank
root in one bound Session and NativeWorld. Assert source root/table/generation,
flags `0x6341`/`0x6301`, exact gate bits, body identity/mass/pin at each stage,
both Skill blur gate branches, Cast unchanged pin state, and normal completion
plus explicit interruption with no duplicate receipts/effects. This remains
isolated Session verification; there is no same-window visual/in-game proof.

The private C++17 runner compiles the feature consumer, current
`combat_session.cpp`, and fresh `playable_actor_bodies.cpp` plus
`original_actor_physical.cpp`; it also builds the standalone physical-filter
test. Run from the repository root with
`powershell -NoProfile -ExecutionPolicy Bypass -File port/windows-foundation/features/physics/run_session_actor_transition_v1_tests.ps1`.
Default assets are `.local-inputs/windows-main-frontend-v1/assets` because the
smaller shared-asset root omits `data/animations_pycst.bin` needed to prepare
the real player attack bank. Both executables passed. Session output records
Move4>Idle3 with StopPhysics=0 for player/enemy, Attack5 flags `0x2341`,
Injury11 flags `0x2b41`, Dead12 flags `0x241`, exact filter application, body
retention, absent-receiver skip and duplicate count zero. The standalone output
passes real-body set/reset, callback-delivery and absent-body cases.

Private runner artifacts: Session executable SHA256
`67AFE0592F1935163A1B9A60FA46070860C64BF74CBAA49F36BFEC7694BE4824`, log
`3F57BF6555272A483DA557860D6C19DBE36D2F220947BA94571032052CFEDA59`;
filter executable SHA256
`F9C3C55C108D2E061C9950D8DFAEB051F7B94DF0230B525049FE9E2515123F78`, log
`564E82605039F23C8CD48A4AFFEC3FFC022C4A61DD031CBFBF6F743D82D33DB2`.

Skill/Cast extension run, 2026-10-10: same private C++17 runner passed after
adding actual Knight SkillTable7/BashDown root347 and CharAnimTable Cast root348
to the same Session/NativeWorld fixture. The source SkillList also supplied
opposite moving-byte SkillTable51; the test checked each real row/root pair
against its generation-bound test source owner, Focus flags/gate, physical
identity and pin state, Skill moving-timer and clear-gate immediate-pin
branches, Cast's unchanged pin state, one normal Post, one interrupted Post,
and duplicate cancellation without replay. Output:
`skill347_moving=1 alternate_table=51 alternate_moving=0 timers=1 expiries=1 cast_root=348 same_body=1 PASS`.
The feature consumer now exposes typed Skill providers, but this run's
generation/table/gate/timer owner is a test witness, not the production
coordinator/timer wiring. Production integration still must provide the
selected current SkillTable row and real Character+0x528/timer10/event0x30
owner; absent providers fail closed. This test does not claim in-game visual
verification or deliver the nonphysical event/timer/controller/gameplay tails.

Final Skill/Cast Session executable SHA256
`1EAC30181FC0513A6E9E5C2919CACE01E47035C198487A4916B68349B87AFFC9`, log
`CA2375CDE52AD7A10504E20717BB36A6A95945B6385F21F65C2FE348F782B3B0`;
standalone body/filter executable SHA256
`7F95BBDF975E37B81B29E8ED437DE039435364C970BCF52C0DD84CD333D5575B`, log
`564E82605039F23C8CD48A4AFFEC3FFC022C4A61DD031CBFBF6F743D82D33DB2`.
