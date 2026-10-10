# B061 - equipped helm (and other body parts) not shown on the in-game hero

## Evidence
- Screenshots: user-shots/b061-helm-in-menu.png (helm on the equipment avatar) vs b061-no-helm-ingame.png (hair, no helm).
- Reference video: NOT inspected in this session (no frames extracted); the original draws equipped modular parts on the live actor
  (PlayerGearEffectsV5::update_skin categories, already recovered in source_equipment_appearance.cpp). Direct video confirmation is a remaining gap.
## Root cause
The equipment page draws its own VisualSkinOwnerV6 (modular plan: MC_Torso/Feet/Hands/Head per slot). The gameplay body is the CharacterVisual
built once at game start with a fixed controller list: MC_Head__naked plus equipped Torso/Feet/Hands (main.cpp start_same_state). Head items were never
included and later equips never touched the gameplay meshes (only the menu skin owner and weapon attachments).
## Change
- CharacterVisual::reselect_controllers (original_character.cpp/hpp): rebuilds drawable parts on the SAME retained Scene/skeleton, redeforms, atomic on failure
  (refactored the controller->mesh loop into Impl::build_parts).
- RuntimeEquipmentBindingV1::body_controller_ids: modular controller ids from the same slot-to-part plan the menu uses.
- main.cpp syncPlayerBodyParts: called after the equipment binding is created and after every equipment render change; rebinds textures.
## Tests
- modular_defaults ctest: part swap keeps Scene, changes head part, unknown controller leaves visual untouched, restore works. ctest: only
  session_skill_binding (known) and winmm_pump_priority_v1 (0xc0000135 missing DLL, environment, audio) fail.
## Before/after (real EXE, hidden desktop)
B061-before-after-sheet.png: pairs before|after for base, helm, torso, boots, gloves, all (PlateHelm/Armor/Boots/Gloves). Log: "Player body parts: MC_Torso_Plate_03 ... MC_Head_Plate_03".
Mid-game equip: AutoEquip ALL path calls the same sync (logged) but the test bags offer no equippable upgrade, so no mid-game visual was captured.
## Remaining gaps
- Weapons/shield: unchanged code path (attachments); only mc_rweapon_longsword_01 exists in the package assets, so other weapons/shields could not be captured.
- Other classes (mage/rogue) not captured; code is class-generic.
## Package files required
data/3d/characters/prince/weapons/* (other weapon and shield models) are missing from the Preview 15 package; equipping them makes the equipment binding fail.
