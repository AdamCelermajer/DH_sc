# Authored in-game character panel V2

This extends the existing Android `CharacterPanelSessionV1`. It owns one
`CharacterMenuMovieV1` (actual shared + character SWFs) and one source
`MenuStackOwnerV1`; it never creates a player, Gear, Save, skill VM or scratch.
Main-menu/character selection is outside this owner.

## Root integration

Keep the existing `native_app.cpp` sole panel instance. Link:

- engine-ui: `authored_character_panel_v2.cpp`, `character_menu_movie_v1.cpp`,
  `menu_stack_owner_v1.cpp`, `menu_stack_actions_v1.cpp`, `menu_stack_v1.cpp`,
  `authored_character_menu_bridge_v1.cpp`, `authored_character_menu_session_v1.cpp`,
  `authored_menu_lifecycle_v1.cpp`, `authored_menu_localization_v1.cpp`,
  `character_menu_reload_action_v1.cpp`, `character_menu_reload_v1.cpp`,
  `menu_rollover_input_v1.cpp` if these are not already linked.
- android-native: `character_panel_authored_v2.cpp` beside existing panel TUs.

On the GL thread, obtain the current `player_gameplay_binding()` for each call:

1. `panel.bind_gameplay_services(CharacterPanelGameplayServicesV2)` supplies
   reached supplemental source item/Save/faery/HUD services described below.
2. `panel.initialize_authored(binding, AuthoredCharacterPanelServicesV2, error)`.
   Pass `OriginalUiSession::movie_services()` with its retained actual texture,
   source text/font platform, localization and native application providers.
   The movie executes **NativeReloadSkills during startup**, so it must already
   have a genuine backend. Initialization failure preserves the prior owner.
3. `panel.authored_viewport(binding, actualViewportSeed, actualDriver,error)`.
   Seed `movie_rect` from the actual character movie definition. Supply source
   orientation/dimensions and real viewport/bounds; do not reuse HUD artwork
   dimensions as an inferred character-movie rectangle.
4. `panel.authored_open(binding,error)` executes native PushMenu of
   `menu_CharacterMenu`. Its original onPush opens the remembered/default tab.
5. Per frame `authored_frame(binding,seconds,error)` then
   `authored_display(binding,x,y,width,height,error)`.
6. `authored_geometry(binding,actualPath,screenX,screenY,out,error)` uses the
   retained source viewport, actual matrices and GameSWF shape traversal.
   On a source-selected release call `authored_release(binding,path,error)`.
   Named transport `authored_tab(binding,0..3,error)` and `authored_back` invokes
   the same actual handlers; it is useful for accessibility and regression.

Tab paths under `_root.menu_CharacterMenu.CharacterMenuTabs` are
`btnCharacterSheet`, `btnInventoryTab`, `btnSkillTreeTab`, `btnFaeriesTab`,
`btnBack`. Inventory row/skill paths are authored runtime display children;
do not replace them with a static Android icon grid. This version provides
named scoped release and actual shape geometry. Full drag/scroll/focus event
orchestration is a remaining input-provider boundary, not an accepted no-op.

Every movie operation installs a transient current-profile borrow, including
nested native callbacks. `CharacterPanelSessionV1::dispatch` invokes existing
source native queries/actions against the same Gear/Skill/Save/temp. Supplemental
hooks cannot replace those identities. The JSON snapshot is only the existing
development widget fallback; authored AS object writes do not use JSON.

## Required actual platform graph

`AuthoredCharacterPanelServicesV2` requires real existing HUD/base RenderFX
identities/root projections, source MenuStack globals, source panel renderflags,
and `character` projection of actual live weak receivers/focus fields. Maximum
occurrences is caller-owned storage capacity, not an authored menu policy.

Source PostLoad42f284..42f2a4 allocates discovered `menu_*` entries as MenuBase,
then RegisterMenu42ee94 calls RegisterState7adf50 before valid7c. Original
MenuBase Create41b3e4/GotFocus41b3e8/LostFocus41b3ec are `bx lr`; these exact
branches are implemented. A caller can supply a genuine concrete Create override.
No other lifecycle method is silently accepted.

Whole Show/Hide uses `authored_menu_show_v1/hide_v1`. Caller must deliver reached
Debug.Load/GetSwitch, rollover-event enable store, actual Application+ec store,
deadzone registration, source manager+60 clear, listener unregister and any
reached drag reset. Localization uses this session's actual common_text cache,
fresh source constants/player/name/file services, and source RenderFX context
publication. Remaining stack services include original debug/focus controller,
touch reset/process, render reset, input/listener and reached license methods.

Native application routing preserves original AS arguments/results. Required
reached callbacks include `NativePlaySoundFX`, `NativeChangeRolloverInputBehavior`,
`NativeBackToHud`, startup `NativeLoadSettings`/`NativeIsMultiplayerEnabled`,
localization and tutorial/device services. Bind original source owners; absence
fails with the original reached diagnostic. Native navigation stays inside the
same retained stack and may reenter it synchronously from onPush/onRelease.

## Same-profile native providers

Already connected by existing session: full stats query, actual inventory lists,
equipped/item details and powers, real current gold; source equip/unequip/auto-equip,
stat assignment, train skill, equip skill, saved skill IDs/points. These use live
property/class/skill-list/Save/capacity/debug providers and source AS member writes.

`CharacterPanelGameplayServicesV2.actions(binding,actionGraph,error)` can bind
the **actual** swap-HUD owner (`DisplayRightHud`/`FillActionIcon`) and full
increment source services if needed. `queries(binding,actions,queryGraph,error)`
can bind retained `CharacterMenuItemActionsV1`, `CharacterMenuSaveActionsV1`,
`CharacterMenuFaeryActionsV1`, source faery offset and CanIncSkill queries.
Pointers supplied by this callback must remain alive through nested dispatch.
These callback owners are retained, but actors are freshly borrowed.

Still mandatory when reached:

- Drop/transmute: same mutable inventory + property view and source inventory
  services, exact transmute multiplier/local-player/online/achievement/Skin;
  genuine empty drop container + actual source DropInventory world delivery.
- Confirm-save: whole same-character SG_Save, script constants, actual player
  classification/achievement. Closing after pending stat/skill changes reaches
  original confirm2 and Save; do not dismiss it with a successful placeholder.
- Faery mutation: source Save/difficulty + actual Character+420, source movement
  and effect, current HUD receiver and faery interface. Fresh locked faery state
  remains locked and cannot be invented.
- NativeReloadSkills: use `CharacterMenuReloadActionV1` with fresh selected
  Character and exact coercion, then `dh2_character_menu_reload_v1` services in
  source order: RemoveBuffs, Save.Load(mask8), InitSkills, UpdateSkills, recalc,
  CheckItems, saved level/class, actual MenuFX lookup, conditional spec prompt.
  V6 `native_reload_skills` alone is not this whole nine-phase coordinator.
  A development actor lacking a canonical save-file producer remains a genuine
  startup blocker; neither empty-file success nor skipping Reload is supported.

## Verification and limits

`tools/run_authored_character_panel_v2.py` stages hash-pinned actual SWFs,
common_text array/constants, English/symbol sheets and original TTF files. It
uses current APK libraries with missing menu owner TUs compiled explicitly,
then runs only `/data/local/tmp/...` on emulator5554. No APK install, game
lifecycle, force-stop or touch. Receipt includes APK/lib/source/binary hashes.

PASS: actual20screen catalog; default Stats source handler, Skills, Inventory,
Back and source stack transitions; actual source-font edit text/localization;
real GameSWF shape centre hit, scoped shape-selected release; nested mandatory
native sound failure is preserved. Profile/application/driver/GPU delivery
hooks in this test are explicitly fixtures (74 query calls, 83 lifecycle calls
in the initial successful run). This is movie/navigation/input acceptance,
**not live profile mutations, real GPU artwork or complete Android menu parity**.

Strict `-Werror` compile passes all three panel/new TUs for ARM64 and x86_64.
The old Linux frozen host dependency snapshot is incompatible with current
source edit-text/localization APIs; its host runner records no sanitizer pass.
Use the current-APK-linked receipt rather than claiming that older host run.
