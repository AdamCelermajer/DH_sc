# Inventory binding prerequisites from Adam 791e961

Pinned upstream: `791e961b12233100b303038c961666834f4beb9d`, read through Git blobs without changing the audit checkout. The import manifest attributes each exact text dependency and frozen gold fixture. The original ELF SHA is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`; the ELF, APK, canonical text/cache files and private inputs are not imported.

## Delivered code and ownership

`ItemTextOwnerV5` borrows the sole inventory ItemTable, retained CharacterTable, one `HudTextV1` StringManager cache and explicit localization/Application providers. Its ClassName service reads actual Character row word5. It uses the distinct source integer/string varargs formatter for item name/stats/requirements and `parseEx` for power properties. Missing providers and negative localized description IDs reject; an already-appended power or output prefix remains. Floating/dollar directives in the varargs service remain explicit unsupported continuations.

The scoped `port/engine-ui/inventory_text_v1.cmake` defines only shared `dh2_inventory_text_v1` with five source TUs, linked to the existing game-data DSO. Root selected the helper in native app CMake; Android uses16KiB maximum page size and `--no-undefined`. Runtime text providers remain unbound. It imports no menu, GameSWF or renderer. `localization.cpp` also retains its source `Localization` implementation; the integration must instantiate only its one chosen StringManager cache (`HudTextV1` is needed here), then share that cache with later HUD/menu work.

`PlayerEquipmentQueriesLiveV1` borrows the existing V4 and exact property sheet. It reads active set slots1/2 afresh, follows actual Item metadata and cached source field203. Main/off categories are word37; offhand type22=6 is shield, other present offhand is dual; raw two-hander is main word26=-4. Effective two-hander additionally follows types4/5 or zero field203. Combat uses raw two-hander, preserves caller state/combo and sets the same resolved sheet. This module has no inventory/property/save/RNG/renderer/VM/timer/frame owner. The existing equipment requirements adapter is unchanged.

## Actual original order

The scoped construction capture contains `ItemInventory::ItemInventory` (0x3ff200), `Character::InitPost` (0x3b4d60), and `_InitEquipment` (0x3b395c).

* Constructor: gold0, gold limit `INT32_MAX`, capacity byte0xff (-1), selected set0, flags0, two nine-slot null sets. The V4 constructor's capacity input needs this exact constructor/profile projection, not an arbitrary number.
* `InitPost`: `SG_Load(4)` at3b513c. Recognized player-record branch at3b54e0 calls `_InitEquipment` before level/class/full recalculation at3b5188/5190/519c. The later recognized branch at3b54d4 calls `_InitSkillsSlots`. Then3b51c4 writes gold limit1,000,000,000 and3b51e0 stores property194's integer result, negative clamped0, low byte retained. IncSkill can write capacity earlier if that real branch runs. Actual save/profile/player-record outcomes are required.
* `_InitEquipment`: fresh online query; if online, actual record byte66c must be1. Existing inventory or gold takes Skin. Otherwise original loot property9 feeds `AddLoot(loot,0,0,-1,0)`; captured new count is scanned with IsEquippable then virtual CharacterAutoEquip. No fixed starter IDs or invented skill points/levels. Selected V4 `add_fixed_loot` supports its recovered fixed, unpowered domain; genuine random/subloot/powered/gold/difficulty continuations remain required when authored data reaches them.

## Required native closure before activation

1. Retain the actual CharacterTable and immutable item/loot/power resources. Construct one V4 inventory alongside the existing Player save/properties, borrowing the established RNG stream. Bind gear/grants to the skill runtime's exact buff-aware PropertyView and rebind class-row pointers after catalogue reload.
2. Supply actual common_text three buffers, common_text/font constants, selected language pack and `text/<sheet>` provider with genuine leases/close/Debug. Application title/version are required when reached. No default pack/language/formatter/ClassName dictionary is supplied by these adapters.
3. Bind actual Skin. Adam `VisualSkinResourcesV6` loads the complete modular prince BRES; `VisualSkinOwnerV6` borrows the same stable animated Scene and genuine weapon resources/Debug. Renderer must consume its real `draw_parts` geometry/materials. Detach before GL context teardown, preserve logical inventory/text/presentation/RNG, and rebind after Scene recreation. Native import/renderer integration remains separate.
4. Bind genuine online/current-player/difficulty/player-count/profile facts and ordered source grants. Fullness, gold/HUD notifications and AddLoot continuations must be real services when reached.
5. Retire actual ItemPresentation entries before explicit storage destruction, failed temporary creation/split exits and final teardown. Catch typed equipment lifetime failures around direct V4 mutations, retaining reached prefixes. The frozen equipment adapter does not cover every implicit temporary destructor or final inventory teardown.
6. Feed these owned weapon facts into combat/stance and source animation selection. Compilation and the host gate do not activate native inventory/equipment/loot.

## Gate scope

The wrapper uses root's central game-data selection, including the new query TU exactly once with no staging, and the same five-TU shared text helper. PE imports verify each test calls the text DSO and that text imports the existing game-data DSO; both DSO binaries are hashed. It replays 1,000 original weapon cases, 1,087 original parseEx cases and355 original integer/string varargs cases. The cache composition creates every actual1322 Item through V4's source text order and formats936 actual powers; the one negative description preserves its appended IDs/record before rejecting. Nine source class rows, both equipment sets, combat state preservation, no property/RNG mutation, explicit file/constant/Debug/close failures and loaded-sheet prefixes are checked. English pack0 and Debug transport are declared host fixtures. Actual translations/constants/file bytes are canonical local inputs, hashed in the report. No native renderer/profile/grant/loot activation is claimed.
