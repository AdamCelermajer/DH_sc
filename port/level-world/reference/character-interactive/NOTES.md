# Character interaction predicate

The new maintained module reconstructs the complete original
`Character::IsInteractive(GameObject*) const`,192B at0x3a4870. It is a source
caller, not a complete object/property/state/relationship implementation or
native binding. Existing target/relationship/search kernels require this actual
virtual+0x88 predicate and had no maintained equivalent body.

## Source sequence

The Character identity and interacting GameObject argument are captured once.
First query the Character's virtual+0x34 `IsDead`. A truthy dead result and a
nonnull interacting object query `AI_IsFriend` on this Character's embedded
CharAI+0x3c8 with that fixed object. If friend is truthy, query `IsMonster`. A
false Monster result returns canonical1 immediately, before deleted/enabled,
state flags, further death checks, or byte415. Dead friendly nonMonsters can
therefore be interactive even when the ordinary branch would reject them.

Every other branch reads current deleted byte+0x81 and, if zero, enabled
byte+0x8a. Deleted!=0 or enabled==0 returns0. Next query IsFaerie then IsSummoned,
stopping on truthy. Next query virtual IsDead again, stopping on truthy. The
last two source loads require current flags+0x520 bit0x2000 then return the RAW
byte+0x415. This tail result is not normalized to0/1. No current visibility byte
0x80 or distance calculation is consumed by this caller.

The relationship callee is `AI_IsFriend`0x3d511c, not `AI_IsEnemy`0x3d574c.
The predicate at0x3a30ac is `IsSummoned`, not `IsInvisibleMan`0x3a30dc.
IsMonster/IsFaerie/IsSummoned respectively compare fresh GetCharType to4/3/5.
GetCharType invokes GetCharAI and reads AIProps word+0x38. Native providers must
use actual current property identity and must not infer these facts from actor
distance or model name. The direct Ghost's source row68 Type4 can support these
classifications only while that row and Character AIProps identity remain valid.

ObjectBase::Delete0x33ddb4 sets byte81=1; enabled8a is separate from current
visibility and is changed by SetEnable. Character flags520 belong to its state
machine; Idle flags2380 has bit2000, whereas Spawn241 and Limbus0 do not. The
authors of these facts remain outside this bounded caller.

Byte415 physically aliases embedded CharAI+0x4d: both CharAI constructors
initialize it to1. Character::_SetIsTargetable0x3b7a64/104B converts a valid
single Lua boolean and stores it at0x3b7ac4. It is targetability, not AIEnabled.
CancelSneaking0x3bc6b8/204B additionally sets it to1 on its player branch at
0x3bc774. The direct Ghost remains nonPlayer on its source AIProps Type4 path,
so that player-only write is not an initial Ghost fact. Later targetability
producers and callback writes must retain their source timing.

## Borrowing, failure and verification

The module owns no game objects. One owning thread retains the Character,
embedded AI, fixed interactor, context/services and retired backing through
return. Callback queries may change live fields; direct field reads happen at
their original phase, including after the second IsDead. Stable identities may
not change. Same Character/output reentry, control aliasing/destruction and
misaligned control views are rejected or forbidden; independent objects can
nest. Provider failures/exceptions preserve completed effects and stop without
rollback or extra callbacks. These errors are port boundaries, not recovered
original error-return behavior.

Host tests cover the exceptional dead-friendly branch, Type3/4/5/9 distinction,
the second death query, field mutations, raw byte415, both query paths, callback
failure, exceptions, missing services, aliases/alignment and independent nested
calls. The original comparison executes the entire192B caller with genuine
Character vtable+0x34, actual12B IsDead, actual24B classification leaves and
actual16B GetCharType. AI_IsFriend and fresh GetCharAI table capture remain
explicit named fixture dependencies. No dead/classification result is returned
by a numeric oracle hook. Exact original symbols/ranges and translated PT_LOAD
hashes are pinned in the adjacent manifest. No floating-point import is modeled.
