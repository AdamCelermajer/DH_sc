Gameplay HUD and potion integration candidate

The frozen PlayerEquipmentRenderOwnerV1 predecessor remains recorded by
port/level-world/reference/player-equipment-render-owner-v1/freeze-manifest.json.
Its header hash is 38b35d2f2b5ee8edb28460e917998d049f6ec0d65170721b075b1983bb699365;
its implementation hash is ec1819702bd80bbace8a88e571f1965d09850131300e2843be33c57412e3eedf.
That historical manifest was not changed. The current candidate extends that
owner API with remove_one_potion, project_potion_capacity, and the initial-slot
raw inventory equipment-selection toggle. It is not byte
identical to the frozen predecessor and needs a new composed checkpoint.

remove_one_potion lends the same retained mutable V4 inventory to the existing
CharacterMenuInventoryMutationV1. Quantity above one uses its original signed
quantity operation; quantity one deletes the actual owned slot and potion
identity. It creates no inventory mirror. gameplay_use_potion borrows the same
PropertyView used by combat, rejects dead/inactive/no-potion/full-vitals requests,
increments source player property219, and runs frozen RegenHP/RegenMP(-1).
Actual DebugSwitches/file services are mandatory when regeneration reaches
the original statistics debug lookup. Source sound and the >99 local-player
trophy continuation remain unconnected; the latter reports a consumed prefix.
The recovered controller byte8/byte9/global input-block predicates still need
the actual controller/input authority before full original input parity.

Original evidence: NativeUsePotion 0x43d2c8; Character::Ctrl_UsePotion 0x3ad6f0;
ItemInventory::RemoveOnePotion 0x3fe878; v2Controller::Cmd_UsePotion 0x4056b0.
No potion cooldown was found in these recovered bodies. Full HP and full MP
returns before potion consumption in NativeUsePotion.

HUD snapshots invoke the frozen 17-member HUD player-info algorithm, retaining
the actual saved skill slot rows, SkillV6 usability and timer-info calls, real
inventory count, real design LowHealthPercentage constant, and the visible
Activity's actual movement pad selection. Equipped ids/levels and saved faery
availability are appended for Android presentation. Fresh save faeries stay
locked. Unlocked spell info/usable/cast producers remain required.

GameplayIcons decodes unmodified original-cache textures through the actual
texture kernel. Its 147-label catalog comes from original character-menu SWF
frame labels, shape bounds and bitmap fill matrices. This exports atlas bitmap
artwork and does not claim original vector clip/button rendering parity.

Validation: compileDebugJavaWithJavac passed; Android24 ARM64 Clang syntax
checks passed with Wall/Wextra/Werror for GameplayHud helper, GameplayIcons
helper and extended Gear owner. The Android x86_64 standalone runtime test on
emulator-5554 decoded the original 1024x1024 atlas through the real texture
kernel and validated all 147 authored crops had in-bounds visible artwork.
Its cropped RGBA hash is 8667b986333d9b16. Whole Android link and live gameplay are owned
by the root composed build and have not been claimed here.
