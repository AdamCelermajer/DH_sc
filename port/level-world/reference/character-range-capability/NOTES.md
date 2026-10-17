# Live ranged capability and parameter providers

`character_range_capability.{hpp,cpp}` maintains complete original caller bodies:

- Character::CanRangeAttack(int&,int&,int&) const, 0x3a4cd0/100B.
- ItemInventory::CanRangeAttack(int&,int&,int&) const, 0x3ffebc/116B.
- ItemInventory::HasRangedWeapon() const, 0x3fffa4/112B.
- Character::CanRangeAttack() const, 0x3a4d3c/32B.

The manifest pins nine exact function ranges and two translated PT_LOAD vtable
prefixes against ELF SHA256
36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80.
GetCurrentEquipSet/GetItem, tail delegation and thunks support this composition;
they are not added to the four complete caller count. No native wiring, inventory
construction, equipment mutation policy, full property resolution or whole-AI
claim is made.

## Existing maintained data reuse

Existing `character_attack_geometry.cpp` projects range parameters over immutable
inventory/rows into one contiguous output[3], committing all3 with memcpy. It
cannot preserve genuine output aliases or source scalar mutation after each
store. Existing `character_combat_queries.cpp` projects no-output capability but
intentionally removes repeated global GetItem calls over immutable inputs.

The new live module reuses their CombatProperties896, CombatInventory16,
CombatEquipSet8, CombatItemInstance4 and CombatItemRecord164 types without editing
those units. Typed wrappers keep actual Character/Inventory identities and
captured original virtual bindings separate from those data views. Only current
global ItemTable capture is a service. No supplied capability boolean, guessed
weapon name or synthesized radius is used.

## Character output ordering

The fixed Character's property32 (+1078) is tested against -1. Every other word,
including zero and other negative values, selects the property branch. Sequential
stores are property30 (+1070) signed ASR8 to output1, property31 (+1074) signed
ASR8 to output2, then fresh property32 to output3; return true. The existing
property schema names these RangeMinDistance, RangeMaxDistance, RangeProjectileID.

Output pointers may be exactly aliased and may point to writable scalar property
or item-row words. Thus output1 can change the value later read for output2 or
output3. There is no batch snapshot or memcpy. Source kind/type limits, positive
distance tests or projectile validation are not added. -1 takes the fixed
Character's embedded inventory with the original three output pointers.

## Genuine inventory virtual dispatch

Inventory116B calls its vtable address-point+8 before reading equip table. Base
ItemInventory vtable 0x967760 has address point0x967768; +8 is0x400014, whose 4B
tail reaches HasRangedWeapon. Character's secondary ItemInventory address point
is0x966124 (primary table starts0x965f30); +8 is0x3a4d34, the 8B -37c thunk into
the no-output Character caller0x3a4d3c. That caller freshly tests property32 and
then nonvirtually delegates to inventory400014 if -1. The two supported original
bindings are explicit metadata, not a fabricated runtime predicate. Unknown
derived vtables return unsupported_virtual.

False capability returns without writing ANY outputs. True capability then
performs a NEW GetCurrentEquipSet(1) read, fresh inventory+14 table/reference/
instance lookup, fresh GetItem row capture and sequential stores row+98, +9c,
+a0 (words38/39/40), returning true. A Character override can grant capability
even if the selected equipment row is not type4/5; the source does not recheck
that classification in the output path.

## Repeated inventory/row reads

HasRangedWeapon112B captures GetCurrentEquipSet(1)'s signed byte+2e index ONCE.
The first lookup uses the inventory's live table and selected main-hand reference.
Absent reference returns false. Present reference dereferences the ItemInstance
and performs GetItem. Type4 returns true immediately. Otherwise the SAME cached
index selects a SECOND lookup using fresh inventory table/reference/instance and
a SECOND GetItem call; that fresh row must be type5. Changed current_set does not
change the cached HasRangedWeapon index; it does affect the output overload's
later independent getter.

GetItem44B captures the ItemInstance ID before global ItemTable pointer loading,
then selects stride164. Every GetItem captures current rows independently. The
service receives the captured ID; changing the instance inside the table callback
does not change that call's row, but a subsequent GetItem sees the mutation.
Returned rows are live through their following field reads. Global/retired table
backing remains alive until the whole caller completes.

## Guards, ownership and errors

Native alignment/count/index/lifetime boundaries are explicit additions, outside
the original unchecked invalid-memory parity domain. Source signed byte2e is
represented by current_set in[-128,127]; valid selected indices must fit the
borrowed count1..128. Item rows have count1..65536 and captured valid unsigned IDs.
The argument1 getter domain is implemented here; other getter arguments are not
claimed. Unused inventory/table services are allowed on property short circuit.

Outputs can alias each other and writable property/item words, while wrapper,
binding, pointer view/reference/instance metadata, Services and Result remain
disjoint. Errors preserve earlier stores and provider effects and stop; no
rollback or automatic cleanup is added. One owning thread retains all wrappers,
props, equipment/ref/instances, rows, retired backing, service context and outputs.
Metadata/identity/bindings cannot change; global capture may change live scalar,
equipment or table facts. Same-wrapper reentry is forbidden. Independent owners
and other source callers can nest; focused tests compose both frozen CloseRange
and Range with the actual new Character provider and their exact output aliases.

## Verification

The host test covers actual output aliases, writable property/row aliases,
property short circuits, type4/type5 repeated reads, base/Character virtual
routes, global/instance/ref/table/current-set mutation, errors/throws, malformed
views and nested live range composition.

The runner executes original ARM Character100B/32B, Inventory116B/112B/4B,
GetCurrentEquipSet32B (argument1), GetItem44B and the Character8B virtual thunk.
Genuine original vtables and concrete pointer graphs are used. No capability
value, equip getter or GetItem return is mocked. The global table capture point
at0x3f9e18 has controlled fixture mutations AFTER the ID instruction and BEFORE
the actual pointer/stride instructions. Branches, ASR, aliases, stores and fresh
loads execute directly. Every valid source case compares result, all three local
output words, property/row mutations, current index, instance ID and ordered
captured-item queries to compiled C++. Invalid original pointers/indices are
excluded from ARM execution and tested as port failure bounds.
