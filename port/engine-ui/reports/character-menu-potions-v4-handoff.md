# Original Inventory potion callbacks V4

Bind `character_menu_potions_call_v4` before the application's unhandled-native
failure in its existing typed `CharacterMenuAsBridgeV1` dispatcher. It uses
manual `CharacterMenuValueV1` fields and does not reference World-defined
numeric/string factories, avoiding the previous UI/World link cycle.

`CharacterMenuPotionServicesV4` pins the actual application owner. Supply:

- `player(index,false)` for source NativeGetPlayerChar43c388.
- `local_player(0,true)` for source PlayerManager.GetLocalPlayer36e478→660.
- `count(character)` from the same live Gear inventory's `num_potions()`.
- `capacity(character)` from its `potion_capacity_v4()` signed byte getter.
- `localized_format` forwarding `character_menu_potion_localized_format_v4`
  over the retained OriginalUi HudText and its actual environment.
- `parse_integer` forwarding `character_menu_potion_integer_text_v4` over
  that same text owner/environment.

Both player callbacks must use the real source manager lookup, validate the
same character authority and preserve genuine null; they are distinct source
queries. The inventory getter is an immutable borrow of the existing sole
private byte, not an additional capacity snapshot. `num_potions()` already
matches ItemInventory.GetNumPotions3fc690: inventory+24 potion pointer absent
means0, otherwise read signed16 ItemInstance+50 quantity. Inventory+2c capacity
is Character+3a8 and is read signed8.

Whole NativeGetStringNumPotions446978 receives `(receiver, playerIndex)`,
converts actual argument1 to_number/f2iz, resolves that player with remote=false
and returns without mutation if absent. It calls GetPyCst
`StrID/GAMEPLAYMENUS_POTIONS`, gets the actual localized format, reads the live
count, parses the one signed integer via StringManager.parse508ef4, then writes
the actual receiver's `StrNumPotions`. It does not replace the function result.
The concrete text helpers use the same HudText `integer_string` and existing
`item_text_varargs_v5`, including actual localized numeric defaults/current
pack and explicit unsupported-directive failures. Null format is the source
empty parser branch, distinguished from a missing service.

Whole NativeGetNumPotions44996c receives the actual receiver but always queries
local0,true. For a nonnull character it writes numeric `NumPotions`, then
rereads live signed capacity and writes `MaxNumPotions`. Reading capacity after
the first synchronous setter preserves watcher/reentry effects. Results remain
unchanged. Missing providers retain reached member-write prefixes.

Actual packaged `gameplaymenus.symbols` selects row394 for this symbol.
English authored format is `Potions: ^d`; all eight language resources contain
the same signed integer directive. Their actual bytes/hashes are recorded in
`reference/character-menu-application-v4/potions-actual-text.json`; labels are
not manufactured or language-corrected by this adapter.

Validation: strict ARM64 syntax checks passed. The original ARM proof29 cases
executes both whole wrappers plus actual GetNumPotions3fc690, extreme signed
quantities/capacities, fresh capacity after a setter mutation and both genuine
null-character paths. AS/CString/player/constant/getString/parse boundaries are
explicit proof providers, not a complete text cache/live UI claim.

Native isolated typed callback fixture passed emulator5554, binary SHA256
`0b1ce40be07e7f6be57653a59ac3dc498b2defa289826c8fd787a5ec36eb4f82`.
It verifies source call order, same identities, no result replacement, null
character behavior and partial failure prefixes. Root owns linked APK and live
original Inventory/preview acceptance.
