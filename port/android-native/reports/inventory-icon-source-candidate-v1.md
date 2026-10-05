# Original inventory icon source result

Original ELF ItemInstance::GetIconName (`0x3f9e94`) reads instance item ID
at +4, indexes the actual global Items table with stride0xa4, and returns
row's string pointer at +8. It has no fallback or class/type/model branch.
NativeInvGetItemDetails writes ItemIcon from that same original table field,
as already reconstructed by CharacterMenuQueriesOwnerV1. Blank starter rows
therefore remain blank. No substitute item icon was added.

The original character SWF has genuine equipment category frames. Those are
slot/category presentation rather than evidence of an item-icon fallback.
Existing menu_icon_pixels supports their exact authored shape bitmap crops.
New equipment_slot_icon_name(binding,slot,name,error) resolves the borrowed
player's actual EquipmentSlots constants and returns the matching authored
frame label. Pass that label to menu_icon_pixels for source slot artwork.

Actual loot_table_pycst.bin constants, recorded separately in
equipment-slot-original-constants-v1.json:

| slot | original constant and SWF label |
|---|---|
|0|Torso|
|1|RightHand|
|2|LeftHand|
|3|Feet|
|4|HandArmor|
|5|RightHandRingFinger|
|6|LeftHandRingFinger|
|7|Waist|
|8|Head|

Count9, none=-1. Strict ARM64Android24 syntax compilation passed for
gameplay_icons.cpp and gameplay_hud.cpp with -Wall -Wextra -Werror.
The existing original atlas Android decoding/crop validation applies to these
already-catalogued frames. No additional fabricated crop or arbitrary item
fallback was introduced. No source consumer fallback was established by the
decompressed shipped SWF string scan; absence of such strings alone does not
prove all source consumers lack fallback behavior.

HUD follow-up: source algorithm failure now preserves reached writes rather
than forcibly erasing Active. Same live Gear potion count is independently
reported if an earlier missing skill query stops the algorithm; this is
explicitly outside the reached authored write prefix. Appended vector index25
is source status, index26 last failing service operation (-1 on success).
No unavailable skill usability or spell result is fabricated.
