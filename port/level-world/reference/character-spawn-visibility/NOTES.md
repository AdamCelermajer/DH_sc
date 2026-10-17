# Character spawn visibility mapping correction

This is a read-only original-source audit. It corrects the identity of three
virtual calls in the existing Limbus/PreSpawn reconstruction; it does not
claim the full visibility or property pipeline is rebuilt here.

## Address point and callsites

`Character` constructor C2(ObjectBase::GO_IDS), ELF `0x3a9340`/1448 bytes,
loads `_ZTV9Character` (`0x965f30`) through GOT `0x9978a0`, adds 8 at
`0x3a9404`, and stores the first vptr at `0x3a9418`. The address point is
therefore `0x965f38`. Its slot `+0x40`, ELF `0x965f78`, contains exact bytes
`f0b03800`: `GameObject::SetVisible(bool)` at `0x38b0f0`. The slot has an
`R_ARM_RELATIVE` relocation. Full-table offset `+0x40` instead points to
`IsUpdatable`; that is vptr offset `+0x38`. Confusing these offsets caused
the former `query_is_updatable` mapping.

| Original caller | Verified source order |
| --- | --- |
| `CSLimbus::OnFocus`, `0x3c2e58`/292 B | Store Character word `+0x520=0` at `0x3c2ec0`; call `SetVisible(false)` with r1=0 at `0x3c2ecc`; then read byte `+0x530`, run optional timer branch, and clear all aggro on every normal path. |
| `CSLimbus::OnBlur`, `0x3c2be4`/344 B | Read controller at Character `+0x378`; clear its lock byte `+8` at `0x3c2c50`; call `SetVisible(true)` with r1=1 at `0x3c2c64`; restore position/rotation; call `Revive(nullptr,false)`; then apply role-3 group status producer. |
| `CSPreSpawn::OnBlur`, `0x3c67d4`/184 B | Call `SetVisible(true)` with r1=1 at `0x3c6844`; call `Revive(nullptr,false)`; enable collisions. |

`CSSpawn::OnBlur` (`0x3c2f7c`/164 B) has no slot-`+0x40` call; its branch
conditionally calls `InitPhysicalObject` when flag `0x2000` is clear.

## Restored byte and bounded fresh-actor fact

`GameObject::SetVisible` (`0x38b0f0`/32 B) writes zero to current visibility
byte `+0x80` for a false argument. For a true argument it first reads the
ObjectBase enabled byte `+0x8a` and writes that exact byte to `+0x80`.
If a VisualObject exists at `+0x2d8`, it tail-calls `SyncVisibility`
(`0x4713d0`/108 B), whose further scene and actor predicates remain separate.
Showing an actor is therefore a restoration of the enabled byte, not an
unconditional write of 1 to current visibility.

Both ObjectBase constructors initialize enabled byte `+0x8a` to 1:
C1 at `0x33f254`, C2 at `0x33f408`. `ObjectBase::SetEnable(bool)`
(`0x33ddc8`) can change that byte. `ObjectBase::Deserialize` (`0x33e0f8`)
reads serialized current visibility and enabled state separately.
`ObjectBase::DeclareProperties` binds the authored `visible` property to
`+0x80` with default 1, not to `+0x8a`. Character's `ai_state_visible`
property is a third field, `+0x13e4`, and is not the enabled byte.

The actual two `_prim_Monster_SURPRISE_01/02` XML blocks in
`data/3d/modules/crypt/mgp/crypt_straight_c_ns_01.mgp` use
`ai_state="Limbus"`, `auto_spawn="0"`, `ai_state_visible="0"`, an empty
`char_template`, and no `visible`, `activate_cond`, `deactivate_cond`,
`min_difficulty`, tutorial, or demo override. A fresh native owner whose
enabled state is initialized by the source constructor can use enabled=1
for this bounded input. Checkpoint/network deserialization, enable/disable
conditions, difficulty/tutorial/demo policies, and later SetEnable producers
must remain separate supported services or explicit unsupported boundaries;
this audit does not claim that no original runtime could change the byte.

## Role-3 group producer

After `Revive`, Limbus Blur reads Character role `+0x400` and runs its group
branch only for role 3. It captures vector length, skips the owner, and
queries every other member's `SM_IsInLimbus()` without short circuit. Any
true answer writes GroupInfo `+0x24=2`; otherwise it writes 0. Empty or
self-only vectors write 0. This is neither an active-peer predicate nor the
all-members predicate in `GroupInfo::CanRespawn`.

The original ELF SHA-256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Supporting ranges and exact byte hashes are recorded in the adjacent manifest.
