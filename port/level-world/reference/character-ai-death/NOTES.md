# CharAI OnDied and AI_SetDead

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Complete OnDied `3d1000` is80 bytes; AI_SetDead `3d6cdc` is140 bytes.
The adjacent original manifest/assembly also capture AI_SetTarget, SyncLastTarget,
TimerStop and full ClearAllAggro. Deeper routing/ownership captures remain
available in the frozen character-kill/death-routing-functions files.

New production files are character_ai_death.hpp/.cpp. Existing Kill, target,
timers, script runtime, CMake and renderer sources remain unchanged.

## Source body order

OnDied captures attacker and receiver. It reads GroupInfo at AI+34; nonnull
calls GroupInfo.OnDied `3d2628` with CURRENT owner at AI+4 and the captured
attacker (`3d1018..20`). It then reloads active AIS+1c (`3d1024`), captures
that instance's virtual+24 callee and calls it with the same attacker. It
finally tail-calls AI_SetDead. No paused/seeking/controller/global gate,
dead query, preemptive state change or once-only protection is added.

GroupInfo kind2 can synchronously Cmd_Kill group members. The outer coordinator
must survive nested Kill/event2/OnDied routing; it must reload active AIS and
owner after the group call instead of caching them. Group ownership, group
iteration and selected AIS method bodies remain required backends here.
The instruction corpus's nested group service is an explicitly labelled
reentry fixture; it is not a reconstruction of full GroupInfo.OnDied.

AI_SetDead executes in this exact order:

| Step | Source | Effect/backend |
| --- | --- | --- |
| 1 | 3d6cec → 3d6890 | AI_SetTarget(NULL,false), genuine existing target kernel |
| 2 | 3d6cf4 → 3d49c4 | SyncLastTarget: copy current target to last |
| 3 | 3d6cf8..10 → 3c58c8 | Reload owner; SM_SetDeadState(false,NULL,true) |
| 4 | 3d6d14..20 → 3db2d8 | Reload owner and AI timer+10; TimerStop |
| 5 | 3d6d24..30 → 3db2d8 | Reload owner and AI timer+14; TimerStop |
| 6 | 3d6d34..3c | Write both timer IDs=-1 |
| 7 | 3d6d44 → 3d5fa8 | AI_ClearAllAggro |
| 8 | 3d6d50 → 3d6abc | AI_ClearAllAggroTowardMe(false) |
| 9 | 3d6d58 → 3d8ae0 | _SkillCleanUp |
| 10 | 3d6d64 → 3d8a98 | tail _SpellCleanUp |

Target clear preserves the source DebugSwitches load/query order, zeroes
owner.word14d0 iff incoming differs from initial target, and then writes null
candidate/current target. SyncLastTarget is a separate subsequent operation.
Alive/sight/changed bytes are preserved by the null setter path. No blanket
AI reset or false-mode shortcut removes the debug calls.

TimerStop only clears active when the raw unsigned ID is in the owned store's
count; UINT_MAX/out-of-range does nothing. Its source return is ignored.
Both stop calls precede the ID stores. State and later callbacks may change
the live timer IDs; the source stores are neither precomputed nor repeated
after later cleanup calls. No duration, dt or repeated-expiry subtraction is
introduced in death cleanup.

## Required deeper ownership

SM_SetDeadState is substantial: it reads the current declaration/stance,
resolves source death animation choices using actual constant masks, updates
machine fields and chooses immediate transition versus registered event
delivery. Existing state12 behavior bodies alone do not implement it.
The new service emits the actual state-machine identity and (false,NULL,true).
It does not substitute current=12, remove a body, or manufacture animation.

Full ClearAllAggro `3d5fa8` differs from the existing single-target
character_clear_aggro helper: it collects outgoing recipients in source key
order while removing their reciprocal incoming entries; then clears the whole
outgoing map; only AFTER that invokes ordered OnDeAggro notifications using
the reloaded owner. The single-target helper's later target-clear/Stop tail
does not belong here. There is no existing complete native bulk coordinator
to compose, so this remains a required service. ClearAllAggroTowardMe(false)
likewise has its own incoming-map/participant ownership body. No source-like
name or empty fixture is used to claim either complete.

Skill/spell cleanup each captures vector count, reloads the current vector
begin for each index, ignores null entries and calls `3daafc` on nonnull
entries. Live resource/affector ownership and this effect body are required.
They cannot be accepted merely because a caller supplies a zero field.

OnDied is called by CharAI.RaiseAIEvent(2) before that dispatcher reloads the
owner and forwards the event to its FSM. It must complete its source prefix
and required backends before the event dispatcher continues. No duplicate
FSM event is emitted by this module.

## ABI and lifetime

AIDeathOwner24 borrows Character identity, its genuine state-machine identity
and TimerStore32. AIDeathState64 borrows AI identity, live owner/TargetState48,
nullable GroupInfo/active AIS, selected callable table and two raw timer IDs.
All backing storage and callbacks survive synchronous reentry. When replacing
the owner projection, the caller must refresh target.owner coherently from
the same real receiver; these views do not allocate or invent a Character.
Only mutable source fields are reread. The captured attacker stays unchanged.

TargetServices16 supplies genuine target/debug services. AIDeathServices16
returns0 only for actual required deeper delivery. AIDeathRequest48 carries
the service, actual subject/owner/payload, original helper or captured virtual
callee identity, virtual operation24 and source argument words. Source addresses
are provenance/dispatch keys, never executed native pointers.

Native status1 completed, -1 malformed before callbacks/mutations/output commit,
-2 failed required service or invalidated provider projection after the reached
source prefix. AIDeathResult24 records diagnostic phase/callback count, number
of in-range TimerStop calls (not unique active-to-inactive changes), whether
ID stores occurred, and completion. Original methods are void. Source effects
before required failures remain. Reentry uses independent result objects;
no suppression, synthetic callback success or arbitrary iteration cap exists.

## Proof

O2 ARM64 original-instruction differential:1600 cases,17819 ordered calls,
20 synchronous group reentries,8 atomic guards,zero mismatches. Actual original
OnDied/AI_SetDead, AI_SetTarget/SyncLastTarget and TimerStop instructions execute.
The native oracle builds existing target/timer sources with the new coordinator.
Group/AIS/SM/bulk-aggro/skill/spell services are labelled fixtures. All19 logical
state words, every complete timer record, ordered callback snapshots and
outer/nested diagnostic outputs compare. Tests cover null/non-null group and
AIS, source owner/active/timer rereads after callbacks, later ID replacement,
two-case-sensitive debug branches, raw byte values and invalid timer IDs.

Sanitizer host:1600 original gold replays including20 reentries,161578 checks,
896 required service-failure prefixes,8 atomic guards,zero ASan/UBSan findings.
The isolated module links actual stable central world/data/runtime DSOs; report
hashes prove before/after identity and dladdr/ldd origins. Test-only TimerStop
interposition observes call order then invokes the genuine world DSO function
via RTLD_NEXT; no timer result/effect is replaced by a fixture.

The extra composition invokes actual world AI event2 → this OnDied → actual
DebugSwitches missing-file backend → world target clear/sync. A real fopen
probes the supplied scratch filename; ENOENT alone is accepted as missing.
The source default switch insertions execute, with7 owned entries after target
query. The required SM_SetDeadState service then fails explicitly at phase5.
Target clear is retained, alive/sight/changed bytes stay unchanged, and timers
remain active with original IDs. This proves that prefix, not full death state,
group kill, AIS script, physics, scene ownership or whole-game execution.

Reproduce from repo root: tools/build_character_ai_death_oracle.ps1, then
tests/character_ai_death_differential.py and tests/character_ai_death_host.py
using the configured direct Python and PYTHONPATH. Parent central integration
adds character_ai_death.cpp to dh2_level_world; after rebuilding, run host.py
--main-linked for a new report. Optional character_ai_death_audit target uses
tests/character_ai_death.cpp and links world/data/runtime/dl. Executable argc3:
scratch/host-fixtures.bin plus a genuinely absent DebugSwitches filename.
The runner generates its binary gold and verifies the file is absent; it does
not delete an existing file, rebuild central targets or modify APKs.
