# Character menu source composition seam

`source_composition.hpp` is an owned, header-only integration seam. It does not add a page owner, art batch, texture, hit contour, or gameplay copy. Root must keep the canonical selected-character graph alive and pass one shared owner token into `SourceCompositionV1`; every provider and the stat action owner must retain that same token.

## Page provider registration

Register each independently owned page with `register_page(Tab, SourcePageProviderV1, error)`. Equipment, Skills and Faery providers supply:

- `ready(error)`: read-only probe against the current live source graph.
- `append(Frame&, error)`: append only that page's native source content to the common menu frame.
- `release(authored_x, authored_y, error)`: route a page action using source 480x320 coordinates.
- `owner`: the same canonical selected-character owner token used by the composition.

`select(Presenter&, tab, error)` checks destination readiness before changing the active tab. On failure it leaves the current tab alone. `install_content(Bindings&, error)` chains an already-present generic root content callback, then invokes only the selected page's provider. The prior callback must not already append Equipment, Skills, or Faery content. It rejects repeated installation to prevent duplicate page batches. Root input routing must use `SourceCompositionV1::release`, which performs `Presenter::hit_test` and guarded selection; calling the older direct `Presenter::release` bypasses readiness checks.

Recommended adapters:

- Equipment `ready` runs its same-owner inventory/slot view probe; `append` calls `equipment_menu::Presenter::frame`. Its release adapter resolves original slot hits; inventory Details and actual equip/unequip dispatch remain with root's inventory/action wiring.
- Skills `ready` runs `skill_ui::Presenter::view` plus an append into a discarded frame; `append` calls `Presenter::append`; `release` calls its authored hit handler. Unsupported source probes/actions must continue to fail closed.
- Faery `ready` calls `faery_menu::present` into a discarded frame; `append` uses `faery_menu::content_callback`; `release` resolves `slot_at` then calls `activate_slot` on the same bindings. Preserve character-menu art3 under the page547 provider, as required by the source stack route.

These callbacks must borrow the live source owners already published by root. They do not instantiate Equipment, Skills, Faery, save, or query owners.

## Stat training route

The original root SWF button handlers in `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt` dispatch Strength/Dexterity/Endurance/Energy indices `0/1/2/3` through `NativeStatsAssignPoint` (for example calls at `0x14a4f`, `0x14c95`, `0x14ee5`, and `0x15125`). The native callback reaches `Character::IncStatStr/Dex/End/Nrg`; the existing portable equivalent is `CharacterMenuActionsOwnerV1::assign_stat` over its genuine `CharacterMenuStatGraphV1`.

Root registers `register_stat_training` only with the genuine actions owner and same owner token. It supplies the exact source button hit resolver and the actual ActionScript pre/post continuations. The pre-action continuation must preserve the authored `AddedStatsThisTurn` / `useSkillPoint` admission and `NativeSaveGame` ordering. The post-action continuation owns the source feedback/update sequence. The seam admits a button only when the complete `equipment->properties()` / `property_view()` graph and all base/saved/gear/resolved groups match and live resolved `Stat_Points` property148 is positive. It then dispatches the source index through the existing actions owner. It does not guess button bounds, save defaults, or make a detached property state.

The selected-tab art's zero-point deactivation uses the same property148 source integer condition. Root still needs to bind the original SWF hit contours and source before/after continuations; these are explicit remaining wiring requirements, not successful completion claims.

## Component verification

`source_composition_tests.cpp` checks same-token registration, failed readiness preserving the current tab, failure for an unbound page, one-time content installation, and chaining of root content before the selected page. It is component verification only; live native owner, page action, save, and visual-reference acceptance remain root integration work.

## B009 navigation evidence (2026-10-10)

**Visual evidence.** The retained source movie is
`port/android-native/app/src/main/assets/original-cache/data/menus/dqcharmenu_droid.swf`
(SHA-256 `43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0`,
480x320 stage). Its parsed page inventory identifies the shared
`menu_CharacterMenu` and separate Faery (`menu_FaerySheet`, sprite 547) and
Quest (`menu_QuestLogSheetNEW`, sprite 599) pages. I inspected the saved menu
capture sequence in `port/android-native/reports/front-game-flow-v1/`:
89 is Stats, 90 is Skills, 91 is Inventory, and 95 is the returned gameplay
world. These are current reconstructed-runtime captures of the source movie,
not original-device footage; they directly show the same page frame and tab
bar across the first three captures and return to the same visible Warrior in
95. The two screenshots do not prove every transition. `reports/feature-faery-menu.json`
records the source Faery page art’s full-stage bounds and 152,403 opaque pixels
out of 153,600 decoded pixels; this is static SWF-art evidence, not a runtime
Faery screenshot. The Quest handoff records that no original Quest screenshot
or continuous capture is present. No direct visual evidence currently proves
the full Stats → Equipment → Skills → Faery → Quest → Back sequence or absence
of gameplay peeking in a fresh normal executable.

**Logic evidence.** The retained ActionScript export
`port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt` shows
`sprite252`’s source callbacks. `btnCharacterSheet.onRelease` at offsets
`0xE825–0xE8B4` switches the selected tab, calls `NativePopAllAbove` with the
root menu name, then pushes `menu_CharacterSheetNew` and
`menu_CharacterSheetStats`. `btnFaeriesTab.onRelease` at `0xEC0E–0xEC81`
switches its source icon state, pops above the character menu, and pushes
`menu_FaerySheet`; `btnQuestLogTab.onRelease` at `0xED38–0xED98` does the same
for `menu_QuestLogSheetNEW`. The native caller gates are preserved in
`native-stack-functions.asm`: `NativePushMenu` (`0x43B1B4`) converts its first
ActionScript argument then calls `MenuManager::PushMenu(const char*)`
(`0x431924`); `NativePopAllAbove` (`0x43AC28`) requires exactly one argument
and a source string value before dispatching through the current HUD root.
Those callbacks operate on the existing root menu stack, so pushed-page
composition must retain the same selected-character owner and pop back to the
existing Character menu. `Presenter::frame` also checks a present persistent
character id against the supplied `CharacterState`; `SourceCompositionV1`
requires each provider to carry the canonical owner token. The production
caller in `port/windows-foundation/main.cpp` currently registers Equipment and
Skills, special-cases Faery clicks without dispatch, and has no Quest stack
registration/release path. The draw loop already paints a black full-window
backdrop before Character-menu art. These are caller observations, not a claim
that the active executable has passed visual acceptance.

**Expected behavior and minimal integration.** Keep one selected-character
lease through every page. Select Stats/Equipment/Skills through the four-tab
Presenter route, with Equipment/Skills readiness checked before changing the
selected tab. Bind the existing Faery provider as the fourth tab and dispatch
its exact source hit contours/actions once. Bind Quest under exact symbol
`menu_QuestLogSheetNEW`; when the source stack reports that symbol, require
provider readiness before appending art or routing releases, and return through
the existing stack to the same Character menu and actor. Keep the existing
full-viewport opaque draw pass behind every page. Root/integration lead owns
the `main.cpp` stack registration, active-symbol input/render dispatch and
normal executable verification; this feature seam does not own those callers.

**Uncertainties and verification.** The retained SWF and native exports come
from the local source package; version differences versus any user’s retail
build are not measured here. Source shows that Faery and Quest are pushed
pages, but this workspace lacks a source gameplay video proving their animated
transitions. Before implementation, the focused test is: in a fresh isolated
normal session, open Stats, Equipment, Skills, Faery and Quest in order; verify
all page labels and same selected actor, hit every Back/return route, and check
that combat/save state stays unchanged and no gameplay pixels show through.
Also verify a missing/unready provider leaves the last valid page intact and
cannot dispatch its action, and a fresh actor/session rebuild binds providers
to the newly selected owner. Existing `source_composition_tests.cpp` and
`source_menu_page_composition_tests.cpp` cover provider owner/readiness,
atomic append, same-stage release coordinates, and page selection only; they
do not replace that normal-runtime capture.
