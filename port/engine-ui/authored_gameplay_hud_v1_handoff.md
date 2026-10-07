# Authored Android gameplay HUD V1

Borrow the `SwfMovie` already retained by `OriginalUiSession`. This owner creates
neither a second movie nor another player/settings/skill authority.

```cpp
std::unique_ptr<dh2::ui::AuthoredGameplayHudV1> gameplay;
// After the genuine shared + HUD movie load and first advance:
gameplay = std::make_unique<dh2::ui::AuthoredGameplayHudV1>(*movie);
gameplay->bind(actual_settings.option("HUDStyle"), error);
gameplay->activate(error);
// Once per frame, use the same native HudInfos producer's successful 17 words:
gameplay->update(infos, actual_class_id, actual_settings.saved_option("DPad") != 0, error);
gameplay->display(error);
```

Retain the existing selected-root `PlayerStatusHud`/enemy HUD producers and
HurtCorners submission. Replace the partial HealthBars submission with this
whole `HUDelements` submission. Do not submit HealthBars twice. The authored
movie includes potion counter, all buttons, bars, joystick, portrait, borders,
shadows, fonts and atlas artwork. No Android icon circles are needed.

The actual Android topology is **0/1 rolling list, 2/3 fixed three skills**.
0/2 have the left joystick; 1/3 have the right joystick. This differs from the
existing `hud_manager.cpp` fixed/list selection branch; V1 does not change that
frozen kernel or pretend that HUD0 is HUD2. Select actual saved HUDStyle2 to
match the supplied left-joystick/fixed-buttons reference.

`bind` validates all nine real controls and selects the genuine root. `activate`
invokes original `onPush` then `onShow`. `refresh_skills` invokes actual
`setSkillsButtons` after a real equipment/slot mutation. No substitute icon
lookup is performed. Register required natives before **shared/root loading**:

* `NativeSkillGetEquipedSkillsIDs(arrayReceiver, 0)` writes the real equipped
  list to the SAME AS array. Original actions31219..31234.
* `NativeGetSkillDetails(skillID, objectReceiver, 0)` fills original details;
  `SkillIcon` and `SkillAssignedToSlot` are consumed by authored button setup.
  Original actions312a3..312c6. Reuse the existing character query adapter.
* `NativeHUDGetActiveFaery(0)` returns actual active faery ID; `onPush` selects
  authored `btimg` frameID+1. Original320d4..3211b.
* `NativeHUDSkill(slot)` slots0/1/2 and `NativeHUDSpell()` no arguments: actual
  source handlers; route to retained native gameplay actions and preserve
  failures. `NativeUsePotion(0)` and `NativeSwapEquipment(0)` likewise.
* Menu handlers require `NativePushState("menu_Ingame"/"menu_CharacterMenu")`,
  `NativeChangeRolloverInputBehavior(3,true)`, `NativePauseAllSounds`,
  `NativePlaySoundFX("MenuBack")`, `NativeUpdateOrientation`,
  `NativeAwayFromHud`. These require real application/audio/navigation owners.

`geometry(control, screenX, screenY, out, error)` converts coordinates through
the SAME source viewport, borrows actual character, traverses GameSWF shapes,
and returns actual world twip bounds plus control-local pointer coordinates.
It checks ancestor visibility. This provides Java/native transport with actual
authored placement under rotation/aspect changes, without circle approximations.
Use retained `SwfInputConnectionV2` for original press/roll/release focus and
timeline delivery when that input owner is connected. `release` is a narrow
explicit AS release transport, not a replacement for that whole input owner;
do not deliver both paths for one release.

Attack artwork is `controls.controls.btn_interact` ID374, not `btn_attack`.
The joystick's `stick.onRelease` action13042 is empty. No native movement/attack
command appears in these clips. `release(attack/joystick)` therefore rejects
explicitly; transport their actual shape/local coordinates to the existing
source gameplay controller. Faery AS has only release→NativeHUDSpell: original
hold/cancel uses the retained native faery controller, not invented AS handlers.

`update` uses original portrait class ranges290..292→sourceframe2,
325..327→1, else0, availability/joystick visibility, actual cooldown percentages
minus1 (lower-bound0), and real usable flags. AS numeric `gotoAndStop` receives
source zero-based frame+1. Rolling buttons resolve their actual `SlotId`.
Health/MP/XP arithmetic stays in the existing source status/native
providers, so incomplete gameplay cannot become successful through this owner.
The owner additionally writes the actual potion count to the exact original
`HealthBars.btn_potion.cnt.value.text` setter. The asset has no local-player
level text field; no substitute level label is introduced.

`joystick_stick_offset(xTwips,yTwips)` adds caller-provided controller visual
displacement to the actual retained authored `Joystick.stick` translation;
`joystick_stick_reset` restores that exact transform. `_x`/`_y` setters receive
SWF pixels (twips/20). This is placement transport, not a new input policy:
the original joystick radius/deadzone remains unrecovered. The actual shape
hit and pointer-local coordinates are available for the existing controller.

The source `_initSettings`46e47c is a valid separate lifecycle entry: clear actual
option map, in false mode copy all actual GameOption descriptor/default rows,
then set14 tutorial bytes true. It does NOT load a file, switch language, clear
the file buffer, or mark loaded/new. Exposing this original entry allows real
settings selection without claiming full `loadSettings` completion.

Host report `reports/authored-gameplay-hud-v1-host.json`: actual bundled SWF,
all four layouts, portrait/cooldown frames, viewport shape domain, original
whole HUD drawing2552 vertices, positive and negative shape hits, stick offset
and rest reset, and explicit missing native input rejection;
404 checks each O1/O2 ASAN/UBSAN, including whole action-icon mapping and actual
root FillActionIcon delivery. Both new HUD and settings modules compile
with current production header order/flags for ARM64 and x86_64.
Texture/localization callbacks are fixtures;
production native action/gesture/audio/menu completion is not claimed.
