# Source attack bank and hand events

Investigation before implementation. The class-bank audit resolves actual
Rogue325/table50 with two Dagger01 occurrences to stance2, Attack250 and
AttackStatic245; Mage290/table49 with Staff01 resolves stance3, Attack251 and
AttackStatic246. The existing typed combo planner reads these original tables
and retains all redirects, leaf paths, rates, blendOut and MoveGO. Its baseline
XML graph alone is not the equipped weapon graph. CSAttack::OnFocus0x3c404c
uses previous state4 for Attack, otherwise AttackStatic, and applies original
stance-list masks0x40/0x80. Choice remains caller-owned.

Original visual evidence reused from
features/combat/runtime-player-combo-chain-v1-report.json: footage274.7–275.2
and281.995–287.995 has changing attack poses, MISS/numeric text and target-ring
transfer. It does not establish Rogue dual-weapon or Mage projectile parity.
Recovered assets/logic arev1.0.2; reference video isv1.0.3.

CharAI::_OnAnimEvent0x3d4434 (pseudocode-all.c152650+) first queries CanRange.
In melee state5, attack_mainhand calls virtual OnAttack with hand=false and
attack_offhand with hand=true. AISPlayer::OnAttack0x3dd7ec delegates to
AISDefault::OnAttack0x3dc12c, which calls F_MeleeAttack0x3b3368 with that hand
and alternate=false. F_MeleeAttack selects inventory slot1/2 and the matching
formula properties. Ranged events instead spawn a projectile; a staff event
must not be converted into immediate melee damage.

Current defects: Session can only initialize its outgoing graph from baseline
XML and maps every configured damage marker to one profile-wide hand. A narrow
pre-initialization bank input will merge only explicit same-profile Attack and
AttackStatic metadata and exact clip aliases into the existing class visual,
with audited policy conflict rejection. Existing Idle/Injured/Died and actor,
equipment, properties, retained animation and RNG owners remain authoritative.
An explicit sourceMeleeHandMarkers option will map the two original melee
events to separate original damage sources. Legacy explicitly pinned hand
profiles remain compatible. No controller request, default stance, random
choice, projectile hit or native callback graph is invented.

Verification defined before edits: real Rogue source dual graph/gear in one
Session, actual main/off authored events and source formula payload/RNG; compare
each hand against the same current sheets, preserve generation/target, verify
held ordered groups and strict save/rebind. Reject foreign profile/model,
conflicting Type/Loop metadata, invalid bank states/aliases and unknown hand
marker without replacing a live Session. Check the existing default Knight
attack path. Full live equipment-bank switching and Mage projectile admission
remain distinct follow-ups; the pre-init input does not claim those complete.

Implementation and focused test now pass. Profile sourceAttackBank plus its
explicit sourceAttackPolicies merge Attack/AttackStatic metadata and clips before
the existing visual load. SourceMeleeHandMarkers maps main/off events separately;
legacy pinned-hand profiles remain unchanged. The source combo boundary now
queries the actually selected action state rather than always looking up Attack.

The real Rogue same-Session runner uses Dagger01 twice, source static root245,
twelve original leaves and four held ordered groups in one action generation.
Eight actual authored hand events each match the original complete result and
shared RNG when independently calculated from the current actor sheets at the
resolution boundary. Actor, retained pose, gear and binding witness stay stable.
Strict GameSave/rebind does not replay hits. Invalid profile/model/root policy,
state, clip alias, missing policy and marker configurations reject without
replacing the live Session. The fixture uses original Lizard level20 properties
to keep the defender alive through all four groups; this is explicitly not
Act1 balance evidence. With a level1 defender, early death ended the attack
before four groups; the test no longer mistakes that valid cleanup for a
continuity failure.

Runner: tools/run_combat_session_source_attack_bank_tests.ps1. Private log:
.local-inputs/session-source-attack-bank-test/run.log. Executable SHA256:
899842FAFF60EDABF9AF8B891AF67B396B4D1F2CB97639971A09B5C48AE6634E.
Current source-hit, all eight incoming cases and duplicate equipped occurrence
regressions pass again. The source-bank test uses immutable Preview9 assets,
which include the original script constants; the reduced shared asset root does
not contain all of those planner inputs. The independent reviewer reran this
frozen executable and reviewed source/atomic rejection; no concrete defect found.

Main integration/visual verification, moving/static selection on actual delivered
commands, live equipment-bank switching and Mage projectile gameplay remain open.

Next investigation before behavior edit: CSAttack::OnFocus0x3c404c chooses
Attack only for previous source state4, otherwise AttackStatic. The already
audited same-profile bank has both source roots. Existing request() runs only
after controller/AI admission, target/range, owned-pose and cooldown gates;
held active commands only set the continuation flag. A new opt-in will select
the appropriate preloaded root at that genuine new-action boundary, recomputing
source attack speed from the same live sheets. It will retain the current
two-slot animation owner and the existing validation sequence object's address,
so bound runtime callbacks remain valid. No bank selection on blocked or held
active commands, no input edge inference, actor replacement or equipment guess.
Full roots with explicit Type2 choices remain caller-owned; this opt-in is for
the source sequential roots whose input policy is proved here.

Focused test defined: actual two-root Rogue bank; blocked moving input leaves
static root/RNG unchanged; admitted moving request selects250 and stationary
request245; held input changes do not reselect while the same action is active;
all original named hand results still match source RNG, same pose/actor witness
survives, and cooldown/out-of-range/invalid policy paths do not select a root.

Selected-profile startup follow-up: lead's frozen D59F normal Rogue run reached
the correct actual gear bank (stance2, moving250/static245, two Dagger01 records)
but failed before frame0 with `actor health range is invalid`. Main previously
projected the actual selected CharacterState only after Session.initialize,
whereas full outgoing capability binds/validates vitals inside initialize.
Original source startup LoadBase/LoadGears/Recalc and saved contribution rules
are already investigated in player-profile-properties-v1.json; its existing
atomic adapter handles the negative authored current-vital sentinel. No new
health rule or refill is justified. Visible invariant: the selected Rogue starts
with saved HP/MP and equipped original model, then can attack normally.

Minimal implementation: borrow the actual selected CharacterState synchronously
in config; run that adapter after equipped source properties are prepared and
before attack-speed preparation/ActorState creation/world binding. Do not retain
the pointer, refill saved vitals, relax World validation, or replace actors later.
Main owns refreshing this borrow for every initialization. Legacy null config
preserves explicit diagnostic policy. Tests defined for partial selected HP/MP,
XP/known points, duplicate gear and persistent identity; wrong class/level/vitals
must preserve the live Session, caller profile and RNG. Existing dynamic/static
source attacks and strict checkpoint roundtrip remain covered. Production main
and GUI verification are still lead-owned; no release acceptance follows alone.

Latest runner644D6C6753BCF863A304A139F0A77BB3B8561C0FEFF755C2975C0688A1D2FDC1
passes all four cases, including moving/static admission and pre-bind selected
profile. Independent reviewer reran that frozen executable and reviewed the new
seam: no concrete defect. Earlier899842 is historical static-bank evidence;
normal main verification is pending the coherent refresh.

Coherent BE16B3600A5761AC6DB44E4EB45AF473F66DA22EB618D150FAD5BEEF1C0614EF
normal standing/moving Rogue420 runs now exit0. Logs prove source245/static
frame1 prince_dual_pre_combo_01 and source250/moving frame3 corresponding moving
clip, actual main/off events and positive damage. Root verified frozen hash,
log slices and attack24 captures with original Rogue/daggers/target ring.
Checkpoint mechanics complete, but health/progression acceptance is withheld:
ordinary XP copies stale source currentHP and heals damage. Loot owns full
live-vital source synchronization; production rerun required before acceptance.

The subsequent35685D2217BE888EE45466FA3A50AA971DEB207258E33575F5408EC3F05A71C4
production rerun closes that reproduced XP/vital reset. Standing420 keeps
25.0547 damage: HP121.742 and XP16 survive F5_340/R380/F9_400 with
RNG1605374/162. Moving420 keeps HP96.5195/XP8/RNG8651185/177 through same checks.
Normal PauseHUD430/MainMenu440/Yes450 saves HP121.742/MP40.25/XP16; a separate
process starts with that HP and XPraw4096, then actual kill81 grants another8XP
while health decreases to104.555. Root independently verified frozen EXE hash
and exact standing/moving/xp-exit/xp-restart entries. Feature tests/review also
cover spentMP, second ordinary XP, source cell equality, actual source level-up
refill and strict profile reload. These are accepted development proofs for
those reproduced routes, not Preview10 or full Rogue/physics parity acceptance.
