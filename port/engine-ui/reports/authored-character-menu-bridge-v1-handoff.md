# Authored character menu transport V1

This is an authored SWF integration adapter. The Android CharacterPanel is only
a debug fallback; its arrangement is not the recovered original UI.

## Actual movie and ownership

`CharacterMenuMovieV1` loads `dqshared_droid.swf` then
`dqcharmenu_droid.swf` into one actual GameSWF player/global namespace, executes
startup, and verifies all twenty source root placements. The complete Stats,
Inventory, Skills and Faeries artwork/widgets/actions already exist in that
movie. Its retained receivers include `menu_CharacterMenu`,
`menu_CharacterSheetNew`, its five stat subsection receivers,
`menu_InventorySheetMain`, `menu_InventorySheetDetails`,
`menu_SkillTreeSheetNew`, and `menu_FaerySheet`. It does not provide a native
MenuBase lifecycle by itself.

`AuthoredCharacterMenuBridgeV1` now routes original native calls, preserving
actual AS object/array/result semantics through `CharacterMenuAsBridgeV1`:

* The existing 23 query/action callbacks go to the same
  `CharacterMenuQueriesOwnerV1`. Its underlying actions must borrow the retained
  player/Gear/Save/V6 authority already used by gameplay.
* `NativeReloadSkills` goes to the actual `CharacterMenuReloadActionV1`.
* Push/Pop/PopAllAbove/PopAllMenus go to the actual `MenuStackActionsV1`.
* ChangeRolloverInputBehavior goes to `MenuRolloverInputV1`, using the same
  mutable four RenderFX projections as navigation/input.
* Other reached callbacks require the retained application's dispatcher. They
  are not successful no-ops or inferred offline/multiplayer values.

Install `native_actions()` and `dispatch(name, fn_call, error)` in the real
SwfServices provider BEFORE movie load. Keep the bridge and every borrowed
owner alive through movie teardown. The caller's resources, GPU, localization
and retained SwfTextFontPlatformV1 services must remain actual production ones.
The bridge owns no movie or gameplay state, avoiding a movie/provider cycle.

## Reached startup requirements

Original startup executes NativeReloadSkills once. Its genuine coordinator
requires remove buffs, reload saved skills, reload skill instances,
UpdateAllSkills, property recalculation, item check, saved level/class and menu
FX lookup; specialization prompt is conditional. The original root also reaches
skill details, skill point count, equipped skill IDs and rollover input.
Historical CharacterMenuMovie host tests explicitly used boundary fixtures for
these and do not prove a complete live lifecycle. Do not import those fixtures.

## Activation and input requirements

Register actual root/screen identities in the existing MenuStackOwnerV1 and
push `menu_CharacterMenu` through its source manager operation. Display the
original movie, advance it once per application tick, and deliver native
cursor/button policy to the same movie's SwfInputConnectionV2 inside its Scope.
Use its real viewport for draw, cursor conversion and resize. There is no
Android ACTION-to-source-button synthesis in this adapter.

Stack callbacks have two distinct AS layers: stack Push/Pop invokes `OnShow` /
`OnHide`; MenuBase Show/Hide invokes `onPush` / `onPop`. Calling only one layer
does not reproduce initialization or action refresh.

## Exact remaining lifecycle providers

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Source captures are in `reference/menu-stack-v1/captures/lifecycle`.

* IsValidMenu 41b3f0 reads MenuBase byte7c. GotFocus 41b3e8 and LostFocus
  41b3ec are genuine `bx lr` bodies.
* Show 425450 (728 bytes) is not a visibility setter: valid gate, actual Debug
  load/GetSwitch `isTracingMenuBase`, ProcessLocalization virtual +40 when byte75 is
  zero, SetVisible 4223bc, weak current-context validation, RenderFX callback
  `onPush`, global/menu-name side effects, ClearLoadingScreen, cached
  `option_Custom` character +9b IsLevelRunning assignment (Options only),
  then RegisterDeadZones 42331c.
* Hide 424af4 (620 bytes) includes valid/debug prefix, optional DragAndDrop
  ResetPositions, listener/menu/global state transitions, SetVisible(false),
  actual UnRegisterListener and `onPop`. Language-specific settings branches
  are separate and must remain conditional rather than be generalized.
* RegisterDeadZones 42331c actually finds authored `deadzone_` characters and
  calls GetAbsoluteBoundingRect 416a7c. Do not substitute guessed rectangles.

`authored_menu_lifecycle_v1.hpp/.cpp` now implements the whole Show/Hide
coordinator. `AuthoredMenuFieldsV1` is the retained source field projection;
constructor visible74/localized75/counter78/valid7c/drag5c are zero.
`authored_menu_show_v1` / `authored_menu_hide_v1` call required synchronous
`AuthoredMenuLifecycleServicesV1::invoke`, retaining reached writes and rejecting
the first missing endpoint. GotFocus/LostFocus need no endpoint because the
actual source bodies return immediately.

Registration is a separate producer: original RegisterMenu42ee94 scans four
RenderFX, resolves the actual named screen, appends the menu, calls
MenuFX::AddState7adf50(menu,false), then writes valid7c=1 at42ef28. Do not set it
from world readiness or simply because a SWF file loaded.

Production endpoint binding requirements:

* `localize` is whole ProcessLocalization422d10 and must publish localized75
  via its actual source setter/continuation. It is not a generic Create callback.
* `set_visible` receives0/1, freshly resolves the actual weak current-context
  character and writes its source +9b visible byte. The coordinator repeats the
  source valid gate and publishes MenuBase visible74 only after this endpoint.
* `invoke_as` delivers actual `onPush`/`onPop` using this menu's RenderFX and
  current weak context, zero args, inside the same actual graph Scope.
* `store_rollover_event_enabled` writes the retained sole source static
  MenuBase::m_isRolloverEventEnabled (ELF9a487c); Show always requests1.
* `store_igm_opened` writes the retained sole MenuBase::s_igmOpened
  (ELF9a4863), only for Ingame/playlist/Merchant, Show1/Hide0.
* `store_application_ec` writes actual Application singleton+ec. Show sets
  whether the menu is VerificationLoading; Hide writes0 ONLY for that menu.
* `option_custom_level_running` (Options only) resolves actual authored
  `option_Custom`, then assigns its +9b from actual Application IsLevelRunning.
* `register_deadzones` finds actual authored `deadzone_` children and stores
  source absolute rectangles. `reset_drag_positions` is reached only when
  real drag5c is nonnull.
* `clear_manager_60` is reached for CharacterMenu/Merchant. Listener removal
  is required on every valid Hide. Language get/reset0/save is reached only
  for language menu and unsigned language>7.

These resource/input/localization/deadzone/listener/application endpoint
implementations must now bind actual retained application owners. The module
does not claim live original-menu activation is completed before those bindings.

## Validation

New bridge/lifecycle pass strict aarch64 Android24 C++17 syntax compile.
Actual original ARM Show/Hide and SetVisible instructions generated960 gold
calls across15 names, valid/localized/drag gates and languages0/7/8/−1.
O2 strict host C++ matches all ordered service requests and retained
visible/localized/counter fields. Required localization failure rejects with its
prefix retained. Localization/GPU/debug/application/weak live character/
listener/deadzone endpoints remain declared oracle fixtures; no actual native
Show/Hide instruction was replaced. Reproduction:
`tests/authored_menu_lifecycle_original_v1.py`, then compile lifecyclecpp+
`tests/authored_menu_lifecycle_v1.cpp` and run with
`reference/authored-menu-lifecycle-v1/original-gold.txt`.
No packaging/install or shared renderer/UI/CMake edits were made.
