# Equipment adapter signed-vitals regression

## Evidence and expected behavior

The frozen `source-panes/rogue-unequip-auto-equip.log` capture (frozen build
SHA `1FBA9F…581214`) records the Rogue's authored starter set at equipment
revision 6: five owned bindings, including `Dagger01` in source slots 1 and 2.
After removing slot 1, revision 7 retains four bindings and the offhand dagger;
the next menu-frame path reports `health must be finite and nonnegative` before
Auto-equip. This is the reported source-pane regression; the log's separate
diagnostic line is retained verbatim in the frozen capture rather than treated
as an independent balance value.

The source property model keeps signed 8.8 cells. `equipment_adapter.cpp`
rebuilds gear and recalculates through `recalc_properties_with_class`, then
reads current/max HP and MP from resolved cells 36, 38, 41, and 43. Those raw
cells can carry negative/unavailable sentinels. The source action and menu
projection must leave them unchanged. The semantic `CharacterState` and
`ActorState` values follow the already-used animation-only projection in
`tests/combat_session_tests.cpp`: `max(0, original_signed256(cell))`. That keeps
save/menu validators valid without writing a guessed value back into source
base, saved, or resolved property sheets.

The focused test uses the actual original Rogue fresh profile and ItemTable
definitions (`StartingSuitRogue`, `StartingBootsRogue`, `StartingGlovesRogue`,
and two `Dagger01` instances) with the source slot mapping observed in the
capture. It removes slot 1, validates the same `CharacterState` as the next
menu frame would, auto-equips that same instance, then checks the resulting
bindings and the semantic projection of all four vitals after both mutations.

## Verification and limits

The focused test passes with the refreshed coherent archives and staged
original-cache asset root. It was linked privately as
`.local-inputs/private-equipment-vitals-regression/equipment_adapter_tests.exe`
with LLVM-MinGW C++17, `-Wall -Wextra -Werror`, the adapter source, and the
read-only `libfoundation_runtime_equipment_menu.a`, `libfoundation_data.a`,
`librecovered_content.a`, `libcontent_xml.a`, and `libdh2_freetype237.a` archives.
The runner output includes `Rogue unequip/frame/auto-equip vitals`; the test
validated the same character immediately after unequip and after auto-equip.
The executable SHA-256 is
`FD4EBB8DDD23E2D9A893CF4DBFD536025CECC83F88A25BA35B10608E359B1BB2` and the
equipment-menu archive SHA-256 is
`E7E1DEF5F50442A5F46077E53BE543CD8DF4F79DB7F6A55EA7CBA57837246E73`.
No shared archive was rebuilt.

This test exercises the feature adapter and original property/item tables. It
does not claim campaign-save parity or independently verify the root UI's
rendered next frame; the frozen source-pane capture is the supplied runtime
symptom evidence.
