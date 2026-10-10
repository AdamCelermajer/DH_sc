# B009 Quest/Faery route handoff

## Source evidence

The original menu movie is `port/android-native/app/src/main/assets/original-cache/data/menus/dqcharmenu_droid.swf`, SHA-256 `43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0`. This is a static authored menu, so animation continuity is not part of this page action. Its Faery display list is `menu_FaerySheet` / sprite 547 at matrix `[1,0,0,1,3585,1092]`; the retained parsed source in `source_layout.json` records the five Faery slot contours, button visual frames, selected-Faery artwork, and four editable text fields. The original Quest page is `menu_QuestLogSheetNEW` / sprite 599 at `[1,0,0,1,170,2126]`; the corresponding exact art and hit contour facts are in `features/quests/ROOT_INTEGRATION.md` and `source_quest_page_v1.hpp`. There is no original runtime screenshot/capture for either page in this workspace, so this evidence is the authored SWF display list and geometry, not a claim of live pixel acceptance.

The source ActionScript dump is `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`. The Faery tab is selected in the root `btnFaeriesTab` route (`0x0000e323`); the pushed page is the exact `menu_FaerySheet` symbol. The five slot `onRelease` functions at `0x00024321` onward call `NativeHUDSetActiveFaery(0..4, player=0)`, then update the selected Faery image and localized details. The `onShow` action first resolves all five unlock frames, then applies `Focused` to the saved active slot; it does not make a locked-looking slot immune to the authored click handler. Quest Log is a different route: `btnQuestLogTab` sets the tab and pushes exact `menu_QuestLogSheetNEW` after `NativePopAllAbove(1)` (route evidence at `0x0000ed38`, detailed in `coordination/integration-lead/character-menu-pages.md`). It uses row and Activate release callbacks from its Quest provider.

IDA evidence is retained in `port/level-world/reference/character-menu-faery-connection-v8/original-functions.asm` and `integration.md`. `Character::ChangeFaery` at `0x003ae99c` obtains the live difficulty, validates the source Faery count, writes the selected Save slot, calls `CharAI::UpdateAllSkills` (`0x003d8894`), reads the current Faery association at Character `+0x420`, then, when present, resolves its source model, sets that visual with the authored force flag, and calls `CharAnimator::ANIM_AddSetToRenderObject` (`0x003c99a0`). `NativeHUDSetActiveFaery`'s separate outer Level placement continuation is documented in the same handoff. The feature cannot claim this continuation happened from a Save-byte update alone.

Quest authority and route evidence is already recorded in `features/quests/ROOT_INTEGRATION.md`: the Quest page provider binds exact symbol `menu_QuestLogSheetNEW`, retains the same CharacterState/source owner, appends the authored sprite599 page and routes authored-stage row/Activate releases. Existing provider tests are the source of its focused evidence; this handoff does not repeat those page helpers.

## Expected route and implementation

Root keeps one `SourceCompositionV1` for the selected CharacterState owner. Register the Faery tab provider at `Tab::faery` and the Quest provider under its exact pushed-menu symbol. Quest input must arrive in the authored 480x320 SWF stage after root viewport conversion. Faery tab hit testing uses the source 480x320 coordinates from the same menu stage. A selected Faery release runs same-Session ChangeFaery state publication, UpdateAllSkills, then actual visual placement, in that order; it resolves the selected slot against the same profile's five-row source FaeryList and the coordinator's already-loaded Cast bank. A missing ChangeFaery, UpdateAllSkills, placement, Session actor lease, source table row, or bank root rejects before a new selection is written. After a successful ChangeFaery store, a later continuation failure reports that source prefix and does not fabricate rollback of the original action.

The callable Faery adapter is `bind_session_faery_page_provider_v1` in `session_faery_page_v1.hpp/.cpp`. It adapts the existing authored CharacterState presenter but always supplies the strict same-Session `activate_slot` action, so production registration cannot fall through to the fixture-only single-cell store. The original Quest factory remains `bind_source_quest_menu_page_provider_v1` in `features/quests/source_quest_menu_page_provider_v1.hpp`.

## Integration lead call contract

Use the already selected CharacterState, menu owner token, source FaeryTables borrow, and current preloaded `RuntimeSkillAnimationBankV1`; do not construct another profile or Session. The host callbacks are its real same-Session equivalents of ChangeFaery, UpdateAllSkills, and visual placement.

```cpp
faery_menu::CharacterStateFaeryBindingsV1 faery_page = /* same selected profile */;
faery_menu::SessionFaeryActivationV1 faery_action;
faery_action.owner = selected_character_owner;
faery_action.character = &character_state;
faery_action.session = &combat_session;
faery_action.tables = source_faery_tables;
faery_action.animation_bank = &runtime_skill_animation_bank;
faery_action.difficulty = current_source_difficulty;
faery_action.profile_faery_list_id = actual_character_table_faery_list_id;
faery_action.validate_same_session = /* current controlled player identity check */;
faery_action.change_current = /* same-Session source ChangeFaery store */;
faery_action.update_all_skills = /* actual derived skill continuation */;
faery_action.place_selected_visual = /* actual visual placement continuation */;
character_menu::SourcePageProviderV1 faery_provider;
if (!faery_menu::bind_session_faery_page_provider_v1(
        faery_page, std::move(faery_action), faery_provider, error) ||
    !source_composition.register_page(character_menu::Tab::faery,
                                      std::move(faery_provider), error)) {
    return fail_menu_route(error);
}

character_menu::SourcePageProviderV1 quest_provider;
if (!bind_source_quest_menu_page_provider_v1(
        quest_binding, same_character_state, selected_character_owner,
        quest_provider, error) ||
    !source_composition.register_source_menu_page(
        SourceQuestMenuPageProviderV1::source_menu_symbol,
        std::move(quest_provider), error)) {
    return fail_menu_route(error);
}
```

Build integration must add `session_faery_page_v1.cpp` to the feature library that already owns the Faery page and link its existing generic-skills animation-bank implementation. Main/menu-stack/viewport and CMake changes remain with the integration lead.

## Focused verification and limits

The strict connected test is added to `tests/hotty_session_cast_v1_tests.cpp` and run by `run_hotty_session_cast_v1_tests.ps1`. On a real initialized Mage Session with the original FaeryTables and the bank's Celest slot0 / Hotty slot4 Cast roots, it proves: missing placement and missing UpdateAllSkills reject before changing the selected slot; a clicked slot with no preloaded root rejects before source callbacks; the source contour calls ChangeFaery → UpdateAllSkills → visual placement once per release, including a repeated authored click; an outside click is consumed without a callback; existing Hotty/Celest cast tests still pass after the fixture returns to slot4. Result: PASS, `run_hotty_session_cast_v1_tests.ps1`.

### Session lifetime guard follow-up

This infrastructure correction has no separate original visual counterpart; it preserves the existing visible invariant that a Faery release only operates on the current live profile/Session. The original behavior and authored page evidence above remain applicable: the slot release routes to `NativeHUDSetActiveFaery`, whose source ChangeFaery/UpdateAllSkills/visual continuation is separately documented. Source ownership evidence is explicit in `port/windows-foundation/combat_session.hpp`: `CombatSession::lifetime_lease()` says to capture the witness once and inspect it before dereferencing a borrowed Session reference; `CombatSession::~CombatSession()` marks the witness dead (`combat_session.cpp`), and the weak witness does not extend Session lifetime. The previous provider violated this contract by obtaining that witness from the raw pointer inside every click callback.

The focused observable test is provider bind and successful repeated slot action on the real initialized Mage Session, then destruction of that same Session followed by another in-contour release. The latter must reject before touching the raw Session or invoking any ChangeFaery/UpdateAllSkills/visual callback. Same-profile CharacterState, actor-binding lease, actual five-slot source list/rows, and preloaded Cast-bank checks remain in force. No gameplay timing or visuals are changed; the lifetime witness is host bookkeeping. A racing concurrent destructor is outside the existing synchronous menu callback ownership contract and remains an explicit uncertainty.

Follow-up result: `run_hotty_session_cast_v1_tests.ps1` PASS, including the destroyed-Session callback case. The test confirms the in-contour post-destruction release rejects with the Session-lifetime diagnostic and does not append another source continuation. The preexisting connected Hotty/Celest result checks also pass in the same runner.

This is a connected feature/provider test with declared same-Session host continuation callbacks, not production `Level::PlaceFaeryAndFollowers`, normal menu-stack registration, or live GUI evidence. Root still owns mapping the real UpdateAllSkills and visual placement continuation, registering both routes, viewport mapping for Quest input, and visual verification on the normal executable. The lack of original runtime captures remains an evidence gap for screenshot-level comparison.
