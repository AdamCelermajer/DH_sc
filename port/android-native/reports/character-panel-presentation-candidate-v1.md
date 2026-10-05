# Character panel source presentation candidate

`CharacterPanelPresentationV1` borrows the same renderer player descriptor,
requires each item pointer to belong to that player's real Gear inventory, and
uses the caller's existing HudText/StringManager. It owns immutable original
cache tables and temporary query descriptions, not gameplay state.

The `powers` callback supplies CharacterMenuQueriesGraphV1's required ID and
description vector. IDs retain the live item order. Description formatting
follows ItemPresentationOwnerV5 AddPower: original Description string, one
variant per serialized attribute with float(raw)/256 and signed raw>>8, then
the existing ParseEx. Sorting metadata is authored row metadata, never a
replacement for positional mutable ItemPower sorting words.

`power_details` additionally exposes exact original palette, special-effect
integer, description ID, ordered Attr/Bonus/Flags triples and source numeric
values. Attr is the distinct 0..48 source enum. Destination property IDs and
names follow original AddPowerProperties jump table 0x3e3308, matching
item_gear_properties_v5.cpp, including separate left-hand mappings. For example
AR_01 Attr29 maps property50; it does not mean CharacterProperties field29.
Unknown enum values have no destinations, as the original kernel ignores them.
Special-effect integers are retained without claiming a guessed FX table name.

`faery` reads the live resolved FaeryList property29 and original fallback0,
then the selected original Faery list/row. It resolves original Name and
Description string IDs through the same text owner and returns original model,
element, script, spell type and type. Save unlocks and levels remain the caller's
same live Save authority. Genuine source fallback identifiers include
Fake_Celest, Fake_Rocky, Fake_Wetty, Fake_Windy and Fake_Hotty.

Validation: actual original ZIP ItemPower/Faery six table files loaded by native
Android x86_64 parsers on emulator5554: 937 powers,1224 ordered attributes,
4 lists,16 faery rows, PASS. Receipt:
character-panel-source-tables-android-v1.json. Helper ARM64 Android24 strict
syntax compilation passed with -Wall -Wextra -Werror. This does not claim a
whole panel UI rendering test or source StringManager runtime oracle.

Integration: retain CharacterPanelPresentationV1(assets), bind query powers to
its powers method with the borrowed gameplay binding, existing HudText and its
text environment; call faery for panel text fields. Add its cpp to Android
CMake. No parent session, Java, native_app or historical manifest was edited.
