# Close and ranged reach callers

`character_ai_ranged_range.{hpp,cpp}` maintains the complete original callers
`CharAI::AI_IsInCloseRange(GameObject const*) const` (556B, 0x3d63d8) and
`CharAI::AI_IsInRange(GameObject const*) const` (496B, 0x3d6604). Original ELF
identity, exact function ranges and translated PT_LOAD hashes are pinned in
`original-functions.json`. Supporting dependencies are evidence, not additional
full-body reconstruction claims. This module is not native wired or a complete AI.

## Existing code and interfaces

The snapshot `port/game-data/ai.cpp` receives already computed close/range bits.
The maintained target/master caller kernels query provider services for these
two predicates. Neither implements these live bodies. Shared State/Point and
ResolvedObject types come from the frozen sight/melee headers; no old source is
edited. Close can compose the frozen complete interaction caller through its
named fallback. Sight and melee are separate queries, not substitute distances.

New APIs are `evaluate_close` and `evaluate_ranged`. Typed borrowed services own
const-handle resolution, InteractionType, the actual virtual CanRangeAttack
overload, TargetPosition, diagnostic sequences and the interaction fallback.
Unknown/unavailable services stop explicitly, without guessed weapon properties.

## Exact close ordering

Null argument selects the original AI+40 once; null after fallback returns false.
Const ObjectBase::GetHandle (0x33dd70) / ObjectHandle::GetObject(false) (0x33ff8c)
resolve a potentially distinct object. Null resolution, resolved word+f4 nonzero,
or resolved virtual+90 InteractionType(fresh owner) !=8 tail to the original
interaction caller using the original AI and original candidate. A resolved
Character does not replace that candidate for position queries.

Eligible close calls the fresh owner's virtual+128 overload with r1 pointing to
the lower-limit local and r2/r3 pointing to the SAME second local. A false result
returns false before positions, diagnostics or local reads. A true result then
queries fresh owner TargetPosition followed by fixed candidate TargetPosition.
Both returned points are borrowed; both values are read only after the second
callback. Owner-minus-target x/y/z subtraction, x-square, y-square, xx+yy,
z-square and xy+zz each round separately to binary32.

After diagnostics the lower integer local is squared with ARM MUL modulo2^32.
The resulting word is interpreted as signed32 and converted to binary32 with
i2f. Close returns this limit STRICTLY greater than squared distance. It does not
sum either object's radius, clamp signs, take absolute limits or square a float.

## Exact ranged ordering and bounds

The null/fallback gate is identical. Range immediately invokes fresh owner
virtual+128 with THREE DISTINCT output locals. There is no handle resolution,
f4 test, InteractionType or generic interaction fallback in this caller.
False capability stops before positions. True capability uses the same live
point arithmetic and diagnostic order as close.

Lower integer square/wrapped signed conversion must be LESS THAN OR EQUAL to
squared distance. Only on success is the upper integer square computed and
converted; upper limit must be GREATER THAN OR EQUAL to squared distance.
Unordered comparisons are false. Integer overflow is preserved, including
46341^2 becoming negative and 65536^2 wrapping to zero. No nonnegative/finite
validation or radius approximation is invented.

## Capability and inventory dependency proof

The primary Character vtable begins 0x965f30, with address point 0x965f38.
Address-point+128 contains 0x3a4cd0, the 100B
`Character::CanRangeAttack(int&,int&,int&) const` overload; +124 is the different
no-output overload used by the target/master capability gate.

The overload reads word+1078. If not -1 it writes, in order, signed ASR8 of
word+1070 into output1, signed ASR8 of word+1074 into output2, then the fresh
word+1078 into output3 and returns true. Thus close's max/projectile alias receives
the projectile value last. This ordering must survive any port service implementation.
The existing property schema names IDs30/31/32 RangeMinDistance,
RangeMaxDistance and RangeProjectileID. The output pointer is named projectile
accordingly; its raw value is not a fabricated attack-type enum.
If +1078 is -1 it tail calls ItemInventory::CanRangeAttack at 0x3ffebc with the
captured Character's inventory+37c and the original output pointers.

The 116B inventory overload calls its virtual+8 ranged-weapon predicate; false
returns without writing outputs. True resolves slot1 through 0x3fc6a8, traverses
the inventory's +14 table of 12B entries to ItemInstance, queries ItemProps via
0x3f9e08 and writes Item record+98, +9c, +a0 in order. Full inventory/table/item
ownership and the virtual predicate are external dependencies. The new port does
not claim their bodies. Port local zero initialization is storage hygiene; it is
not an original initial value or a fallback property. A true provider must write
all three outputs with their actual aliases; false outputs are never consumed.

## Diagnostic object lifetime

The global DebugSwitches object is captured ONCE after distance is cached. Both
source Load/GetSwitch/string construction/destruction sequences use that SAME
object, including the optional second Load. The first key is
`IsTracingCharAITarget`, the second `isTracingCharAITarget`; original strings are
0x8c56e0/0x8c56f8. Truthiness of key1 selects key2; key2's result is ignored.
This differs from the melee caller's fresh second global capture. Changing a
global binding cannot replace the captured object or cached squared distance.
Capability stack words are read after both sequences, retaining source timing.

## Verification and boundaries

`tests/character_ai_ranged_range.cpp` tests gating, fresh owner reads, fixed
candidate, borrowed point alias/mutation, output alias write order, both debug
queries, independently nested source interaction fallback, service errors and
exceptions, control alignment/alias and invalid borrowed views. Errors preserve
prior provider effects and stop; no rollback or extra cleanup is added.

`tests/run_character_ai_ranged_range_host.py` executes the complete original
556/496B caller instructions, actual Character 100B capability overload,
inventory 116B overload and TargetPosition 36B leaf against the compiled C++.
Before numerical modeling, `tests/elf_import_identity.py` executes every actual
relocated PLT stub and verifies the imported symbol. In particular, 0x30e9ac
resolves to `__aeabi_fcmple`; it does not resolve to `__aeabi_fcmplt`. The earlier
fixture guessed the latter name and wrongly made the lower range bound strict.
The corrected caller and equality fixture accept the ranged lower boundary;
the existing `dh2_attack_ranged_distance` inclusive lower bound was correct.

External soft-float imports are explicitly modeled separately rounded IEEE
binary32. Original integer MUL/ASR, branch decisions and aliased output stores
execute directly. Const handle/GetObject(false), InteractionType, inventory
virtual8/slot1/item-row lookup, debug sequences and interaction fallback are
named fixture providers; one borrowed-same-point case models TargetPosition.
Finite result words compare exactly; NaN compares unordered class, without
original imported soft-float NaN payload/sign propagation claims.

One owning thread retains the original AI, fixed candidate, resolved object,
fresh owners, live points, debug object, provider/context and retired backing
through return. Providers may mutate live facts and bindings, but must not
destroy borrowed backing, change stable identity keys, overwrite controls or
reenter the same State. Independent calls can nest. Output pointers are valid
only through caller return. No assets, model renderer, candidate discovery,
CMake or native production binding is changed by this reconstruction.
