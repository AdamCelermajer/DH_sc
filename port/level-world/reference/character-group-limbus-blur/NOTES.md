# Limbus Blur role-3 group status producer

This source kernel implements the bounded group branch of
`CSLimbus::OnBlur` after its controller unlock, visibility restoration, pose
restoration, and Revive calls. Those earlier operations remain separate.
It reuses the existing `character_group::GroupState` and member-query
`Services` types; it does not duplicate `GroupInfo::CanRespawn`.

## Exact branch and ordering

At `0x3c2c94`, the source reads Character role `+0x400`. Only value 3 reads
GroupInfo at Character `+0x3fc`. It captures the vector length at
`0x3c2cc0`–`0x3c2ccc` once. The vector begin pointer is reloaded on each
iteration at `0x3c2cdc`; identities are not snapshotted as an immutable list.
Each owner entry is skipped. Every other entry invokes
`CharStateMachine::SM_IsInLimbus()` (`0x3c01c0`) in source vector order,
including duplicate identities and entries after an earlier positive answer.

Register r8 starts at 1. A nonzero query result sets it to 0 at `0x3c2cfc`.
After all entries, r8==0 writes GroupInfo status `+0x24=2`; otherwise it
writes 0. Therefore **any** other Limbus member selects 2. Empty and
self-only vectors select 0. This differs from the all-members eligibility
predicate in `GroupInfo::CanRespawn`, and from an active-peer predicate.

The caller supplies a role read after its Revive service. Group/member/actor
ownership remains external. The entire captured-count range must stay live;
changing the count during a callback does not shorten this source invocation.
A callback may redirect the pointer only to another valid retained range of
at least that captured length. The host test explicitly checks that distinction.

## Port errors and validation

Unused role branches never dereference GroupInfo or query members. Used
header/member ranges must not overlap the result or Services, wrap the address
space, or contain a null Character identity. Missing query services are errors
only when an actual non-owner query is needed. Normal query answers are source
bools 0/1. Errors preserve output and make no kernel-owned status write;
exceptions propagate and callback effects are not rolled back. This input
protocol is port protection, not an added original group predicate.

The host gate has 16 cases covering role gates, empty/self-only groups, mixed
Limbus/non-Limbus answers, full ordered traversal, duplicates, live pointer
redirection with captured count, alias/address errors, and query failures.
With `--original-elf` it additionally executes seven bounded original ARM
cases using Unicorn, after checking the complete library hash. These run the
role/group branch from `0x3c2c94` to normal convergence at `0x3c2ca0`, plus the
real `SM_IsInLimbus` and `SM_GetState` leaves. They observe query entry without
replacing its result. The mixed case confirms all three other members are
queried even when the first is in Limbus; the pointer-redirection case confirms
the captured count continues against the new begin pointer. The original
earlier OnBlur prefix and a live native group are outside this execution.

```powershell
python port/level-world/tests/run_character_group_limbus_blur_host.py
# Optional direct original-code check (requires pyelftools and Unicorn):
python port/level-world/tests/run_character_group_limbus_blur_host.py --original-elf <original-libDungeonHunter2.so>
```

Source contribution: one group-status ordering kernel. The 344-byte caller
range includes earlier OnBlur operations that this module does not implement;
the two 20-byte state query leaves remain injected actor-owned services in the
C++ port. Executing their original code for validation does not add a source
implementation claim.
