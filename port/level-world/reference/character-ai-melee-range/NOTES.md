# Live melee range and melee radius

`character_ai_melee_range.{hpp,cpp}` maintains two original caller
orchestrations: `AI_IsInMeleeRange(GameObject const*)` at `0x3d6188` (592 bytes)
and `AI_GetMeleeRadius()` at `0x3d4c34` (104 bytes). Owning handle, inventory,
interaction, debug and property getter dependencies are explicit services.
No complete CharAI/actor integration or new full dependency body is claimed.
Original symbol/range hashes are pinned in `original-functions.json`.

## Reuse without snapshot substitution

State and borrowed Point reuse `character_ai_sight` views. AiRow/AiTable reuse
the frozen enemy-retention views (68-byte original AI row). Those are header
types only; the new production cpp has no link dependency on those cpp files.
Existing `dh2_ai_range`, `dh2_attack_melee_distance` and
`dh2_attack_melee_radius` snapshot APIs remain unchanged. They cannot preserve
this caller's live pointer reads, repeated owner loads and property-table
capture across callbacks. The new radius API can bind the existing verified
inventory parameter helper through its actual owning inventory views.

## Melee source order and branch boundary

Null argument captures current target+40; absent target returns false. A
nonnull candidate remains fixed even if a callback changes target+40.
Const ObjectBase::GetHandle (`0x33dd70`) then ObjectHandle::GetObject(false)
(`0x33ff8c`) resolves an object. This is not the nonconst handle overload,
Character conversion, or RTTI. A null result or resolved word+f4 nonzero
delegates `AI_IsInInteractionRange(original AI, original candidate)`.
Otherwise resolved virtual+90 InteractionType with **fresh owner** must equal8;
other values take the same exact named fallback. Its owning/math body remains
external here; a missing callback reports unavailable instead of approximating
interaction reach or assuming an unresolved object is out of range.

The eligible path captures a fresh owner's GetTargetPosition, then the
**original candidate's** GetTargetPosition. Both borrowed coordinate backings
are read only after the second callback. The distance computes owner-minus-target
x/y/z, xx, yy, xx+yy, zz, then sum+zz with separately rounded binary32 operations.
It is captured before either radius query. Original AI GetMeleeRadius comes
first; the **resolved Character's embedded AI** GetMeleeRadius comes second.
The port service supplies that owning AI by the resolved Character identity;
it must not add a native layout offset to an arbitrary scene ID.

The sum of those two radii is cached before debug services. First diagnostic
sequence loads the current global DebugSwitches, calls Load, constructs
`IsTracingCharAITarget`, calls GetSwitch and destroys/frees the temporary string
as its source allocation requires. A truthy query performs another fresh global
load/Load/query for `isTracingCharAITarget` and destroys that string. The second
return is ignored. These complete sequences are service keys1/2; the source
query is not removed because its result is used only for tracing. Finally the
cached radius sum is squared and compared strictly greater than cached distance.
Negative/zero/infinite/NaN values are not clamped. There is no vertical cutoff.

## Radius source order

First fresh owner supplies ItemInventory::CanMeleeAttack(int&), with the local
integer initialized to0. Its boolean return is discarded. A ranged weapon can
leave that output0; the borrowed inventory adapter must preserve that behavior.
After this service, capture the next fresh owner **before** loading/capturing
the global AIProps row base. GetCharAIId is then called on that captured owner,
with the original getter's fallback8 semantics. Capture the selected row; convert
the inventory output as signed32 integer to binary32, then read row+20 and add
with separate binary32 rounding. Replacing the global base after capture cannot
replace that row, but changing its backing value before the late read is seen.
Count/index/alias guards are port bounds; no modulo or guessed AI ID is introduced.

## Lifetime and proof scope

One thread retains original AI, original candidate, resolved object/Character,
every owner, borrowed points/tables/rows and context storage through completion,
including replaced backing. Identity keys are immutable. Providers may change
live bindings and values but cannot destroy borrowed objects, overwrite output
or services, or reenter the same State. Independent calls may nest. Missing,
failed or throwing services preserve completed effects. Known top and provider
view aliases are rejected; no rollback or extra game callback is invented.

Host tests execute both new APIs, including an actual radius composition for
both melee radius requests. Original comparison executes both complete caller
instruction ranges and the real GetTargetPosition leaf. Const-handle resolution,
InteractionType, radius calls inside the melee caller, diagnostic/string handling
and InteractionRange are explicitly modeled boundaries. The radius comparison
executes actual table capture/row arithmetic while inventory/GetCharAIId remain
observed services. Original external fsub/fmul/fadd/fcmpgt/i2f library bodies are
not in the executable; they are modeled IEEE binary32 imports. Finite outputs
compare bitwise and NaN outputs compare unordered classification, not payload/sign.
This is component proof, not an Android playtest or a native wiring claim.
