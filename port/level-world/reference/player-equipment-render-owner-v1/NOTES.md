# PlayerEquipmentRenderOwnerV1

This portable owner connects one actual Character identity and one shared
PropertyState to the authoritative V4 inventory, V5 item/property/vitals effects,
real localized item text, and the frozen V6 scene/module/weapon graph. It has no
GL dependency. The same inventory, property state and borrowed RNG survive a
visual detach/rebind. The scene and every service context must remain alive
until detach or owner destruction. Retained draw-parts retain their resource
backing after the visual owner is destroyed.

`initialize` invokes the already proved native `_InitEquipment` producer. It
queries actual online/current-record services, current item count and gold,
reads resolved property 9, delivers genuine fixed loot and calls original
IsEquippable before CharacterAutoEquip. `IsEquippable` at 0x3f9e68 tests item
record word26 != -1. The three actual starter lists end in a potion. The prior
GEV5 source fixture attempts AutoEquip on that potion; the initial-equipment
caller skips it. Consequently its correct initial state is the GEV5 snapshot
after the preceding equippable item (`sheets[count-1]`), rather than the potion
attempt which performs another source ValidateHPMP. No HP tolerance is used.

The original Character EquipItemToSlot 0x3a9f30 and UnEquipItemFromSlot
0x3a9eb0 run inventory mutation, UpdateGearsProperties, CheckItemsRequirements,
UpdateSkin and ValidateHPMP in that order. NativeSwapEquipment 0x442198 uses
the same prefix after SwapEquipmentSet 0x3fc6c8. Its subsequent ActionScript
DisplayRightHud and FillActionIcon calls are caller UI obligations; the portable
`swap` method explicitly ends at the inventory/Skin/vitals prefix.

INV_CheckItemsRequirements 0x3a9d10 iterates the nine owned source slots and
uses INV_DoesMeetRequirements 0x3a4930. The latter queries online before even
testing a null item; online remotely-updated Characters accept immediately.
Otherwise it compares item words29..33 to cached resolved properties19 and
149..152 with signed ARM ASR8. Failed gear is genuinely unequipped/merged;
changed gear recalculates properties then repeats the requirements pass before
Skin/vitals. These cached fields are Character+0x1044 and +0x124c..0x1258,
relative to the sheet at +0xff8. They are not comparisons against raw fixed
values. Recursive pruning has an explicit unsupported budget of 18 levels;
the ordinary finite nine-slot domain passes. No failure rollback is invented:
the source prefix remains observable.

The queries kernel executes original HasMainHand 0x3ffe8c, HasBow 0x400080,
HasStaff 0x4000c8, IsDualWielding 0x40019c/HasOffHandWeapon 0x400158,
HasShield 0x400110 and HasTwoHander 0x4001a0. Damage category is record word37;
shield is offhand type word22 == 6. A raw two-hander has word26 == -4. The
effective stance predicate additionally accepts type22 4/5 or cached property
203 == 0. Combat uses the original raw `HasTwoHander(true)` result; stance uses
`HasTwoHander(false)`. The kernel preserves raw truthy online/remote values,
full native identities and exact signed arithmetic. Its 2,000 original/optimized
ARM64 cases execute 8,000 source predicates. Online singleton and remote virtual
returns are explicit caller services; actual item/current-set/table accessors
and predicate instructions execute. The same corpus replays in the SAN host.

Inputs load exact caller-provided logical cache paths for loot_table,
item_powers and common_text. The owner holds all decoded tables and text.
Language, inventory potion capacity, RNG seed, selected Character, current
difficulty, online state and player count are genuine caller projections;
there are no fallback values. The host test explicitly supplies seed1,
capacity12, language0, selected identity0x100000001, offline/difficulty0/count1.
This proves native composition, not campaign/clock/world-manager provenance.
Real localization filesystem leases and genuine DebugSwitches are used.
Unimplemented reached notifications/power/fullness continuations reject unless
the caller supplies a real provider. Existing cached V6 resources may satisfy
a rebind without invoking asset reads; absent Borrow is tested as atomic
rebind failure, and an explicit table-provider rejection is tested before
inventory mutation. Destructive callback reentry rejects. Scene lifecycle
detach is a quiescent caller operation, not callable during a service callback.

The isolated owner audit builds its two new production TUs plus frozen V6 TUs
against seven copied coherent central151 dependencies. Their hashes are bound
individually. The old binaries are not rebound to later central source bytes.
Frozen V4/V5/V6 reports remain unchanged. This is not a packaged instruction,
GPU, full campaign, item-menu notification, or resource-factory parity claim.

Integration selects `player_equipment_render_owner_v1.cpp` and
`player_equipment_queries_v1.cpp` in the world DSO exactly once. The host target
uses `tests/player_equipment_render_owner_v1.cpp` and links the central world,
data, UI, skinning, animation, scene-materials and script-runtime targets; do
not compile V6 again when it is already in the central skinning DSO. Eight
arguments are recorded in the host receipt. The query corpus is read from
`reference/player-equipment-render-owner-v1/query-fixtures.bin` relative to the
repository working directory.
