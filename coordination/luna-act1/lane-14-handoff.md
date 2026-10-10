# Lane 14 handoff: Character and inventory menus

## Delivered source change

`character_panel_session_v1.cpp` now routes native panel actions through the same live `CharacterMenuActionsOwnerV1` graph used by the authored movie. The fallback adapter still uses the current campaign Gear for automatic equip, while unequip and equipment-set swap use the action owner. The swap path therefore runs the existing `DisplayRightHud` and `FillActionIcon` callbacks supplied by the bound campaign menu. Before actions, supplemental providers must preserve this panel's Gear, skills and Save identities. Invalid item/slot inputs are rejected at the adapter.

The authored movie, query dispatch, item/power/stat presentation, skill/stat mutations and live inventory preview were already implemented in the assigned source. Preview uses the current campaign Character's retained visual and same Gear draw views; it does not create a second avatar. Root integration already includes these implementations and binds the retained Character panel runtime.

## Static reference checked

Only IDA export plus original assembly was used. `Character::EquipItemAuto` at `0x3a9fa8` receives the inventory index in `R1`, calls `ItemInventory::EquipItemAuto` on the inventory subobject at `this+0x37c`, then updates gear properties, skin, and HP/MP. `Character::EquipItemToSlot` at `0x3a9f30` receives slot/item in `R1`/`R2`, calls the inventory subobject, then updates properties, checks item requirements, updates skin, and validates HP/MP. `Character::EquipSlotAuto` at `0x3a9ee8` follows the corresponding requirements/skin/vitals sequence. These caller sequences support routing through the retained Gear owner; the IDA pseudocode is inference, with ARM register/offset/call order confirmed in `assembly-functions.asm`.

## Coordinator integration

No additional shared wiring requested. `model_renderer.cpp` already calls `bind_player_character_panel_v3`, binds `CharacterPanelRuntimeV3::bind` for ReloadSkills, adds the campaign action/query providers, and `native_character_menu_v4.inc` installs the preview on `_root.menu_InventorySheetMain.avatarpane`. Root remains owner of these shared files and integrated runtime acceptance.

Source delivery only. No build, tests, APK, emulator or gameplay verification was run in this lane.
