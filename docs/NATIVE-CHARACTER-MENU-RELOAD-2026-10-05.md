# In-game character-menu reload coordinator

This checkout owns the in-game stats, equipment/inventory and skills screen.
The separate menu chat owns the main menu and character selection. The original
character-menu SWF invokes NativeReloadSkills during its first frame, so a
working character screen needs the entire reload transaction before navigation.

`character_menu_reload_v1` reconstructs the complete original
Character::ReloadSkills caller at 0x3a9db4. It orders removal of buffs, SG_Load
with mask 0x20, AI_ReloadSkills, UpdateAllSkills, property recalculation with
true, equipment requirement checks, a fresh saved-level lookup and conditional
saved-class comparisons. Above level 11, each failed comparison rereads the
class before comparing 263, 325 and 290. It then looks up the current menu
RenderFX and calls `_root.menu_CharacterMenu.IsSpecTime` with one actual boolean.

Native services must borrow the same Character, properties, save and skill
owner. A missing service fails at its reached boundary and preserves the
already delivered prefix; it cannot silently become a successful no-op.
Malformed native entries fail without writing the result. The AS wrapper's
argument conversion/player selection and component implementations remain
separate obligations.

The original ARM caller and native implementations agree on 554 complete cases
and 5,750 ordered component boundaries, including synchronously changing class
getters. Native SAN and O2 runs additionally verify ten failure prefixes and
two atomic entry guards. Both pass ASan/UBSan with leak detection. These tests
explicitly fixture the component services, so they prove the whole coordinator,
not the live Android menu or complete save/skill implementation.

Recovered AI_ReloadSkills destroys each non-null instance in its actual vector
in ascending order, zeros the cell, resets the vector end, then runs
SG_ReloadSkills, SetSkillsAndSpells and UpdateAllSkills. The skill deleting
destructor releases its member at offset 0x0c before deleting the instance.
SG_ReloadSkills reaches the same Save, deletes its old skill allocation,
clears that field, initializes skills, then loads mask 8. A plain update call
cannot replace this lifecycle. SG_Load returns for a null Save; otherwise it
delegates the mask to that actual Save's load operation.

The authored character-menu inspection decodes 527 action blocks, 345 functions
and 563 frame labels. Its original tab navigation identifies stats,
inventory/detail, skill tree/specialization, faery, quest log and map sheets.
This is source evidence; complete menu-stack, layout, input and provider binding
are still needed for a visible character screen.

Evidence: `port/engine-ui/reports/character-menu-reload-v1-host-audit.json`
binds source, original binary/caller, gold and native executables/results.
The installed visible checkpoint remains
`dh2-native-equipped-player-04fb89d1.apk`; this coordinator is not a new APK.

## Original native AS entry point

`CharacterMenuReloadActionV1` now supplies the complete NativeReloadSkills
entry point at 0x43dbb8. Exactly one argument goes through original numeric
conversion and the required external EABI player-index conversion. Every other
argument count selects player zero without coercing its arguments. A fresh
NativeGetPlayerChar(index,false) lookup precedes the complete reload, and a
genuine null player returns without mutation. The prior AS result is retained
on successful, null-player and required-failure paths.

The original caller and native adapter agree over 480 complete cases and 912
ordered original boundaries, with AS/EABI/player/reload boundaries explicitly
fixtured in the original probe. Seven native required guards also pass. The
caller adapter composes the entire previously proven reload coordinator.

Actual GameSWF transport additionally passes SAN/O2: eleven reload callbacks,
ten fresh player selections, five finite EABI fixture conversions and 74
ordered reload services. This verifies genuine string/bool/null/number
conversion, deferred bound-property getters, ignored multi-argument properties,
untouched bound results, null-player guards and required failure prefixes. It
loads shared/HUD movies; the authored character-menu navigation is not covered.
Both production reload translation units also compile with Android NDK Clang
for AArch64. They are not yet linked into a new Android app.

Evidence: `port/engine-ui/reports/character-menu-reload-action-v1-host-audit.json`.
The live external EABI conversion provider remains required: these tests prove
finite representable conversions only. Missing save, skill, menu or conversion
providers reject; this work does not replace those owners with successful stubs.
