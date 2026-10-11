# Source GroupInfo death and member removal

## Source behavior

The pinned ARMv7 library is identified in `original-functions.json`. Live IDA
decompilation of `CharAI::OnDied` (`0x3d1000`) shows this order: call
`GroupInfo::OnDied(group, owner, killer)` when the CharAI group pointer exists;
call the selected AIS virtual `+0x24` when non-null; then call
`AI_SetDead`. `GroupInfo::OnDied` is the 420-byte function at `0x3d2628`.

`GroupInfo::OnDied` switches on the owner Character's state (`Character+0x400`):

* State 2 (MasterLeader) writes byte `GroupInfo+0x28` to one, then dispatches
  `v2Controller::Cmd_Kill(killer, false)` to every Leader in the `+0x0c`
  vector, followed by every Normal/Group member in `+0x18`. It does not call
  `IsDead` first.
* State 1 (Leader) returns if `+0x28` is already nonzero. Otherwise it writes
  one before querying each Leader with Character virtual `+0x34` (`IsDead`).
  The first live member clears the byte to zero and stops the scan. An empty
  vector leaves the byte set to one.
* State 3 (Group) reads integer `GroupInfo+0x24` only when it is zero. It
  queries Normal/Group members in `+0x18` until the first live member, then
  stores zero; if all are dead, it stores one. Empty vectors store one.
* State 0 and unrecognized states have no GroupInfo death effect.

`Character::IsDead` (`0x3a2ed4`) returns the raw byte at `Character+0x1449`.
`v2Controller::Cmd_Kill` (`0x40570c`) forwards to its controller virtual
`+0x58`. The owner kernel calls explicit `is_dead` and `cmd_kill` providers;
provider errors retain source-order writes and any earlier member effects.

`CharAI::RemoveFromGroup` (`0x3d2ff8`) uses the Character's current GroupInfo
pointer (`CharAI+0x34`) and role (`CharAI+0x38`) to remove that Character from
exactly one vector: roles 0 and 3 remove from `+0x18`, role 1 from `+0x0c`, and
role 2 from `+0x00`. It does not clear the CharAI group pointer or alter the
GroupInfo tail or lifetime. Repeated removal of an already absent member has no
source vector effect.

## Population and native reachability

`Character::InitPost` (`0x3b4d60`) calls `CharAI::AddToGroup` (`0x3d37d0`) on
the source initialization path. `AddToGroup` reads the Character's
`char_group` string at `+0x1410`, `char_group_role` at `+0x142c`, and module key
at `+0x64`. It returns when the group string is empty or the role is not one of
`Normal`, `Leader`, `MasterLeader`, or `Group`. Otherwise it looks up the
level-wide `CharAI::s_groups` by `(group name, module key)`, appends the same
Character identity to the role vector, and stores the existing GroupInfo
identity and role in Character/CharAI fields.

The native source census copies those exact two MGP fields in
`source_layout_adapter.cpp`; the current renderer joins direct DACT Characters
to `source_char_ai.groups` through the exact source module/record/handle tuple
and runtime module ID. Faery InitPost uses its retained census row with the same
level owner. No second group registry is created. The rebuilt source-census
audit passes its 50-row SWAMP census and exact handle provenance, but the
recognized-role check alone does not prove a nonempty group. The supplied
SWAMP MGP files scanned for that audit have no nonempty `char_group`; other
cached modules do contain authored groups, including Lighthouse, Crypt, and
Icy Cavern examples. Dynamic Summon factory Records currently retain Character
Table identity but no source MGP group fields, so they have no supported group
join unless an exact source row is later retained.

## Port boundary

`character_group_info_owner_v1` owns the retained membership vectors and tail.
Its `on_died` and `remove_from_group` methods operate on the same validated
Character/AI binding. The owner does not allocate a second target or actor
store. Removal must run while the Character's AI binding is still live and
before its retained owner is retired.

The renderer currently routes Monster `group_died` through the existing
`player_ai_death_v1::Runtime` backend. Its dead query checks the matching
retained CharAI identity and Character dead projection. Its Cmd_Kill provider
supports the retained Player killer against source NPC members; Player-member
and non-Player-killer controller providers remain unsupported. A native full
compile and gameplay run have not been verified by this kernel check.

## Host verification

Run `g++ -std=c++17 -Wall -Wextra -Werror` over
`character_group_info_owner_v1.cpp` and `tests/character_group_info_owner_v1.cpp`,
then execute the result. The fixture covers source vector order, the death
one-shot/tail writes, empty vectors, missing-provider prefixes, and removal
without clearing the retained group pointer. Passing this host fixture is not
evidence of original-engine runtime behavior.
