# Character group respawn predicate

This source slice implements the missing `CharAI::GroupInfo::CanRespawn` leaf
used by `Character::CanRespawn`. It resolves a concrete part of the enemy
Limbus/Spawn lifecycle: when an Idle actor may be allowed to respawn after its
group's members return to Limbus.

## Source behavior

The pinned source ranges are in [`original-functions.json`](original-functions.json).
`Character::CanRespawn() const` (`0x3a5248`) first checks its authored
respawnability and delay conditions. If it has a group, it tail-calls
`CharAI::GroupInfo::CanRespawn(Character*)` (`0x3d2a34`) with the actor state;
without a group it returns true after its own checks.

The group predicate recognizes only Character state IDs 0 (Limbus) and 3
(Idle). For state 0, it returns the inverse of `GroupInfo+0x28`, without
reading group status or members. Other states return false before dereferencing
GroupInfo. For Idle, status 2 allows immediately, statuses other than 1 or 2
deny, and status 1 queries `SM_IsInLimbus()` for every member in the original
`+0x18` vector order. The loop does not stop after its first false result. If
all members report Limbus—including an empty member list—the method stores 2
to `GroupInfo+0x24` once and returns true. Otherwise it keeps status 1 and
returns false.

`CharStateMachine::SM_IsInLimbus()` (`0x3c01c0`) calls
`SM_GetState()` (`0x3c01ac`) and returns true exactly for state 0. The adapter
therefore accepts an explicit synchronous query service rather than deriving
state from aggro or a display/presentation flag.

## Port boundary

[`character_group_respawn.hpp`](../../character_group_respawn.hpp) defines a
logical host-side projection, not an ARM32 memory overlay and not an owner of
group membership. The caller supplies the current status, byte `+0x28` gate,
and an ordered borrowed list of stable Character identities. The kernel keeps
duplicate identities and order exactly as supplied. Its query callback must
perform the corresponding live `SM_IsInLimbus` read.

The result marks whether group status was observed: state 0 and unsupported
actor states do not read or report the status. For status 1, callback failure
is a port-adapter error; it leaves both the group status and output unchanged.
The game source's predicate is synchronous and has no such failure return.
This `CanRespawn` kernel does not own membership construction, registration or
removal, `GroupInfo::OnDied`, aggro cleanup, `CanSpawn`, or native coordinator
integration. Membership and GroupInfo death/removal now have a separate owner
kernel documented in
[`character-group-info-lifecycle`](../character-group-info-lifecycle/NOTES.md);
that separate kernel still depends on its explicit native member providers.

## Host validation

Run:

```powershell
python port/level-world/tests/run_character_group_respawn_host.py
```

The focused test checks the Limbus byte gate; early denial for unrelated
states; status-2 readiness; status-1 all-member and any-active outcomes;
full ordered visitation including duplicates; empty-group behavior; the single
status advancement after all queries; and atomic malformed/provider failures.
