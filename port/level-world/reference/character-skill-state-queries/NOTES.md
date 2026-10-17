# Source skill-state predicates

## Original functions and fields

Original ELF SHA-256 is pinned in `original-functions.json`. Complete callers:

| Function | Address / bytes | Original operation |
| --- | --- | --- |
| `CharStateMachine::SM_IsUsingSkill() const` | `0x3c02e8` /24 | Call `SM_GetState` once; exact raw-word equality to6; return0 or1 |
| `CharStateMachine::SM_IsCasting() const` | `0x3c0334` /24 | Call `SM_GetState` once; exact raw-word equality to7; return0 or1 |
| `CharStateMachine::SM_GetState() const` | `0x3c01ac` /20 | Read current-state pointer at machine+0x20; null returns`0xffffffff`; otherwise read state word+0 |

The getter executes unmodified in the original ARM comparison. Its existing
logical state projection is reused; no additional getter body credit is claimed.
There is no callback, virtual call, float comparison, import, mask conversion or
state transition inside either predicate. IDs6 and7 come directly from the
original compare immediates. IDs3/4 are the source Idle/Move projections in the
existing coordinator; those IDs and every other unequal word return false here.
The52B `SM_IsUsingSkill(unsigned skill_id)` overload is outside this unit.

## Maintained API and native ownership boundary

`character_skill_state_queries.{hpp,cpp}` exposes a borrowed `Machine` pointing
to an `int32_t` source state ID. This is a logical pointer/word view, not an
ARM32 structure overlay. Each call reads the current pointer and current word
again. A null current pointer projects the actual getter's missing-state branch.
`Result.state_word` preserves all32bits, and`value` is canonical0/1.

An adapter may point to `dh2::character::State.current` when that field is the
actor's installed source state ID. Its initialized`-1` also yields false in both
predicates. These helpers do not prove that native actor state construction or
later transitions installed a particular state; those owners supply that fact.
The source `UpdateAllSkills` caller retains its separate fresh-owner read before
each predicate; an adapter must select the actor's machine again for each call.
No source owner or current-state identity is cached across invocations here.

One owning thread retains all borrowed storage throughout synchronous return.
Invalid port enums, null machine/output, unaligned/overflowing or overlapping
control/state ranges are rejected before output mutation. A null source`this`
would be dereferenced by the original and is not a supported source branch.
Null current is valid. Read-only state and output must be disjoint. No allocator,
rollback, cleanup, timer effect or successful placeholder provider is added.

## Verification and limits

`tests/character_skill_state_queries.cpp` checks106 host predicate calls and13
guard cases. It includes noncanonical ID words, changing live pointer/pointee,
and absence of a current state. Invalid guards preserve output.

`tests/run_character_skill_state_queries_host.py` verifies the complete ELF hash,
symbol addresses/sizes and exact range hashes before executing real original
instructions. Both callers invoke the real original20B getter without hooks
replacing its behavior. Read observers verify exactly one machine+0x20 read and,
only for present state, one word+0 read; no state/control memory is written.
The original and compiled results are compared for canonical IDs, signed/raw
edge words, deterministic random words and pointer changes between calls.
Per-instruction observation covers all17instructions across both callers/getter.
No import or dependency body is modeled.

Count: **two complete predicate callers; zero new dependency bodies**. This
standalone unit is not wired into the APK, coordinator or source skill caller by
this change. `UpdateAllSkills`, skill handlers and complete skill gameplay are
separate work.
