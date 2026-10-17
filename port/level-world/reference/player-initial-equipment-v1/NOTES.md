# Player initial equipment V1

Adapted from Adam Celermajer's `player_initial_grants_v2.cpp` at commit
`791e961b12233100b303038c961666834f4beb9d`. Only its original
`Character::_InitEquipment` caller is reused. The already selected skill
grants, progression, inventory, property, RNG, VM, Save and timers keep their
existing owners.

## Original evidence

The ELF is pinned to SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The complete 276-byte `_InitEquipment` body starts at `0x3b395c` and has
SHA256 `5cd18623a37e0ab1321d45fcfb910761c8aaa4a8808cf43db9c79e647eb4736c`.

At `0x3b39b0` it calls `_GetProperty` with `Character+0x560`, the cached
sheet `Character+0xff4`, and property 9. The actual nested getter executes
its valid-ID branch and reads the static schema offset 36 followed by the
cached word at `Character+0x101c`. It does not call property resolution or
recalculation. Every ARM case gives base/saved/gear values 777/888/999 and a
separately supplied cached Loot. Negative values and both signed extremes
remain unchanged in the full AddLoot arguments `(loot,0,0,-1,0)`.

The 18 cases execute 83 distinct original instruction words with zero import
calls. They cover fresh online byte/record-byte branches, existing item or
gold Skin-only tails, empty AddLoot, captured loop bounds, equippable scan
order and ignored CharacterAutoEquip return values. Online singleton and
record queries, NumItems, AddLoot, IsEquippable, virtual CharacterAutoEquip
and Skin are declared callee fixtures. This is a caller proof, with the
actual nested cached getter, rather than a complete item factory ARM replay.

The manifest also pins the complete 304-byte ItemInventory constructor and
2164-byte InitPost body for static provenance. They are not executed by this
capture. Constructor gold is zero, gold limit is `INT_MAX`, potion capacity
is the signed byte -1, and equipment sets are null. InitPost later stores
`0x3b9ac9ff`, exactly **999,999,999**, at `0x3b51c4`.

## Borrowed composition

`Runtime` borrows the sole `FreshInventoryOwnedV4`, its live `PropertyView`
and the stable existing equipment service descriptor. It reads cached Loot
from `PropertyView::resolved[9]`, scans the captured item count and calls
the existing V4 CharacterAutoEquip. That existing path performs source
gear-property update, Skin and HP/MP validation in order. Existing item/gold
branches invoke only Skin; they insert no extra property or vitals refresh.

Online singleton state, the full-width record identity and its byte66c are
fresh mandatory backend requests when reached. Full AddLoot is mandatory.
Providers act on the supplied sole inventory, factory and retained Item
identities. Missing or failed providers preserve the completed prefix. No
inventory clear, source success fallback, retry, extra owner or grant IDs
are added. The upstream 65,536 native scan budget is retained as an explicit
guard rather than an original ordinary branch.

The selected host gate links the actual centrally selected world and
game-data DSOs and the same five-TU `dh2_inventory_text_v1` library used by
the native application. It compiles this module once, without an executable
staging copy. Tests use all three actual player base/class rows, actual
starter Loot/Item metadata, original class/property kernels, the existing
V4 factory/retained items and actual localized Item text providers. Fixed
AddLoot is used only after checking these starter rows contain no random
entries, sub-loot or powers, at source difficulty zero. Other full AddLoot
continuations remain required. Null source visual Skin is exercised; a
nonnull visual without the genuine factory fails explicitly.

Tests compare the ARM caller results, stale cache protocol, equip effect
order, repeat-call Skin-only behavior, retained item/error prefixes,
reentry, output/error aliases and attached buff-group identity. Compiler
dependencies and configured CMake inputs are discovered, snapshotted and
checked after a clean selected rebuild and execution; actual reached cache
files are also checked. The report records counts and exact hashes.

## Remaining native producer

This module does not implement Character::InitPost. Original InitPost must
reach SG_Load(4) and then a fresh genuine PlayerManager::IsLocalPlayer before
its `_InitEquipment` call at `0x3b54e0`; ResetGearsProperties and optional
quest synchronization follow, and normal base/gears/recalculation continue
later. The real profile/Save section and locality producers remain required.
The adapter is selected and host verified; native profile loading, starter
activation, complete inventory interaction and nonnull gear attachment are
not claimed as live gameplay.

Useful next reuse is Adam's `player_save_load_owner_v1` and
`player_profile_index_v1` for genuine profile/section producers, then
`visual_skin_owner_v6`/`visual_skin_selection_v6` for borrowed actual scene
attachment. His private equipment/property/skills graph must be adapted to
the current sole V4 inventory, grouped PropertyView and same-VM Session.
Current live weapon queries already use the selected inventory façade.
