# Lane 15 handoff: skill and faery menus

## Source state

The lane 15 implementation is already present in the assigned baseline; this handoff records its concrete interfaces for integration. No gameplay readiness or menu behavior is claimed as verified in the current APK.

- `port/engine-ui/character_menu_skill_authority_v1.hpp` borrows one existing V3/V6 player skills owner. It retains the real owner identity and forwards readiness, script session, `UpdateAllSkills`, skill info, and errors without copying Save, VM, timers, buffs, or skill state.
- The existing `CharacterMenuActionsOwnerV1` is the menu action owner used by root wiring. It resolves the current class skill-list index from actual skill tables, maps menu rows to table records, checks saved level/slot availability, writes equipped slot rows to the real Save, and trains through the retained `IncSkill` service. Keep its current source and connect the authored callbacks; it is outside this lane's write set.
- `faery_gameplay_v1.*` reads the selected faery and difficulty from the same `PlayerGameplayBinding`, validates unlocked state in the actual Save, reads original spell info/usability through the retained script owner, and applies selection through Save → skill update → actual Character+0x420 faery visual/anim set → current Level placement/follower service.
- `faery_cast_owner_v2.*` uses the same live Character/StateOwner/skills/Save/property graph for controller gates, usable checks, CharAnimTable spell animation, state transition, state callbacks, animator `do_spell`, step timing and deferred `StopLoop`. It does not manufacture a companion, spell slot, Save unlock or alternate FSM.
- `source_campaign_faery_v109.*` enrolls the faery resource providers under the retained campaign candidate, verifies campaign/character lifetimes, resolves actual faery model rows, and exposes the menu and Level placement services. `renderer_character_faery_v8.inc` supplements the existing character panel graph with those providers. Primary1 enrollment exists in `native_process_primary1_binding_v98.inc`; root owns shared wiring and build integration.

## IDA source anchors

Use the IDA export and original assembly only. Relevant routines in `libDungeonHunter2.so`:

- `NativeSkillsTrainSkill` `0x43d548` calls `Character::IncSkill` `0x3bcc58`, then reads skill points and returns through the original callbacks.
- `NativeEquipSkill` `0x43d63c` resolves its three arguments and delegates to `SG_SetSkillInSlot` `0x3bbe54`.
- `NativeHUDSetActiveFaery` `0x43ee40` validates its callback shape, checks unlock, calls `Character::ChangeFaery` `0x3ae99c`, then performs the HUD/faery retarget/effect continuation.
- `Character::ChangeFaery` `0x3ae99c` stores the selected faery for the current difficulty, calls `CharAI::UpdateAllSkills`, then refreshes the companion visual and animation set when Character+0x420 is non-null.
- `Character::GetCharFaery` `0x3aeac0` resolves `GetCharFaeryListId`, checks the source faery-count and row `Type == selected index`, and returns the actual table row.
- `Character::_GetCurrentSpellInfo` `0x3b6e30` resolves the selected faery row before reading its saved level.

The selected-faery table provider in `renderer_player_cast_v2.inc` checks the original faery row type/index and returns its authored spell type. The cast owner must remain wired to the actual `CharAnimTable` `Spells` list; the earlier correction from CharacterModel is recorded in `port/android-native/reports/renderer-faery-timers-v2-integration.md`.

## Root wiring and limits

Connect the existing menu callbacks to `CharacterMenuActionsOwnerV1` and `CharacterMenuFaeryActionsV1` on the same character graph, and preserve `PlayerSkillsRuntime::bind_cast` and the existing faery panel provider enrollment. Root owns `native_app.cpp`, shared renderer/native_app wiring, CMake, APK build, emulator and ADB.

Source/menu owners and earlier isolated semantic checks do not establish current gameplay. Verify menu navigation, a real saved skill point/slot mapping, unlocked faery selection, cast begin/event/end and persistence together on the integrated target. Online spell transport still fails explicitly when no actual network producer is supplied; no Save unlock or placeholder network success should be added. Actual runtime wiring and newly packaged gameplay remain unverified here.

## Startup array descriptor review (read-only support to lane 01)

IDA and actual cache inspection found two omitted nested vectors in the current registration descriptors:

- `FaeryListTable`: change shape `ii` → `[i]`. `Arrays::FaeryListTable::read` `0x4bc12c` reads 12-byte rows; `Structs::FaeryList::read` `0x4eab9c` reads an unsigned count then that many int32 values. The actual `faeries_pyarray.bin` first section is four lists in 100 bytes (0..0x63), followed by FaeryTable count 16 at 0x64. Lists are `[2,4,5,6,3]` and three copies of `[1,13,14,15,7]`. With `ii`, the startup parser consumes only 36 bytes before beginning FaeryTable inside list data, accounting for the downstream false string length 234881024 at offset 0x55.
- `FootstepEffect`: change `iSiiii` → `iS[i][i]`. `Structs::FootstepEffect::read` `0x506660` reads an int, a counted string, then two independent count+int32-vector payloads.

`FaeryTable` `iiiiSii` is correct: `Structs::Faery::read` `0x50637c` reads four ints, a counted string, and two ints. Its FaeryTable section spans offsets 0x64..0x2ba (599 bytes including the 4-byte count); the full combined stream is 699 bytes. In the Effects family, `AnimatedEffectTable` `bi[ibibbiiibbiS]i` correctly models packed bool bytes and nested `AnimFX` rows (`0x4ed73c`, `0x506990`); `CharEffect` `iiiib` correctly models four ints plus one packed bool (`0x4ed5a8`).

No source edit was made; lane 01 owns the registration header. These descriptor corrections are recommendations to apply there. Root should pick up lane 01's updated header before the integrated startup attempt.

