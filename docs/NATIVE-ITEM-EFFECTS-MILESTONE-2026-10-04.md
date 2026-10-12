# Native equipment, item text and power effects

The shared native build now includes the original equipment effects and item
presentation subsystem. Previously the retained V4 inventory provided genuine
item ownership and equipment slots, but delegated gear effects and presentation
to external services. V5 supplies the actual gear/property/class/vitals and
localized text services. Visual resource construction is still required.

## Integrated behavior

- Four production modules in `dh2_game_data`: `item_gear_properties_v5`,
  `item_power_tables_v5`, `item_presentation_v5`, `player_gear_effects_v5`.
- Two production modules in `dh2_engine_ui`: `item_text_varargs_v5`,
  `item_text_owner_v5`. UI links the same game-data library; host tests do not
  compile private copies of these production modules.
- Gear effects borrow the actual V4 inventory and its shared Character
  PropertyState. A different PropertyView is rejected. Gear reset, ordered
  stat/power additions, real class recalculation and live buffs all operate on
  that state. HP is clamped and resolved before MP is read and clamped.
- Only weapon slots 1/2 use the selected equipment set. Other equipped slots
  use set 0, matching the original getter. Left-hand offsets, wrapping integer
  additions and the source type-9 power assignment are preserved.
- Immutable ItemPower backing owns all 937 definitions and 1,224 ordered
  entries. Borrowed backing survives facade destruction; reload while borrowed
  and malformed input reject without replacing a valid snapshot.
- Actual Item names, stats and requirements use the original Item/Character
  tables, current HudText pack, constants, localization files and DebugSwitches
  provider. Modular mesh names and localized display names remain distinct.
- Item Power records stay keyed to the actual owned item. Their ID vector and
  descriptions follow the original append/sort behavior, including the source
  swap that leaves positional SortingOrder words in place. The caller must
  forget presentation records before destroying their item.
- The distinct source varargs formatter preserves exact signed32 integer and
  string arguments; it is not replaced by float conversions or parseEx.

## Current verification

`port/level-world/reports/native-item-effects-main-linked-host-audit-v5.json`
records **146 passing suites with zero sanitizer findings** against the actual
central CMake libraries. All previous 140 suites were rerun. The six new suites
cover:

| Suite | Evidence |
| --- | --- |
| Gear and Power tables | 4,037 gear cases; all 937 power rows and 1,224 entries |
| Item presentation | 4,031 source caller cases and 1,119 Power instance cases |
| Same inventory effects | Knight/Mage/Rogue, both equipment sets, 6 cases/112 operations |
| Genuine cache composition | Same inventory/property/vitals graph, 128 localized item deliveries, 936 power descriptions, 4 real localization file leases |
| Varargs formatter | 355 source cases and required-failure guards |
| Skin caller | 24 original cases with ordered factory/debug/visual replacement services |

The original/O2 differential evidence is bound by the independently checked
`port/game-data/reference/player-item-effects-v5/freeze-manifest.json`:
10,503 comparisons across six original-code proofs. The final isolated
`player-item-effects-v5-host-audit-v3.json` has 66,068 checks and zero
ASan/UBSan/LSan findings. The new central receipt establishes current shared
library linkage instead of attributing the isolated dependency snapshot to the
current build.

Both actual Android libraries build for ARM64 and x86_64.
`port/android-native/reports/native-item-effects-library-build-v5.json` verifies
one compiler selection per production module, its owning DSO, required exports,
the UI-to-game-data dependency, ELF64 architecture and at least 16 KiB LOAD
alignment. Its source/receipt hashes are recorded. This is compilation and ELF
evidence, not live player or physical-device evidence.

## Remaining connection work

Skin's original caller is implemented and tested, but its non-null VisualObject
path still requires actual module/weapon resource factories and the retained
skin graph. The next visual-resource batch is developing that owner; no dummy
factory acceptance is used.

Arbitrary powered-loot creation/valuation, floating varargs continuations and
the original null Power-description continuation remain explicit required
boundaries. Definition 0's null description is accounted for separately; it is
not counted among the 936 successful descriptions.

The gameplay agent has reported a private complete-cache V3 bootstrap pass for
all three classes, including actual Faery scripts, passive Buffs and the same
session's cooldown timer/AI/script expiry. Its full skill AI coordinator is
still being completed and has not been centrally frozen or integrated. The
older V2 complete-cache failure remains preserved as evidence of the previous
missing callback.

The separate **Inspect app launch and menus** session owns launch/loading/main
menu composition in an isolated snapshot and emulator. Our text/font engine
work continues independently; its menu patch will need review against the
current source facade before integration.

No new APK was packaged or installed for this library milestone. The visible
gameplay emulator remains on the verified
`dh2-native-source-player-ui-136e924a.apk` checkpoint. Live item/skill ownership,
new menus and physical ARM64 testing are not claimed by these results.
