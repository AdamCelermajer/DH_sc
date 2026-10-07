# Whole retained in-game menu lifecycle: source plan

This is a read-only source map and proposed next implementation contract, not a
completed native lifecycle or live-menu proof. The full bodies are captured in
`full-bodies`, further RenderFX bodies in `render-dependencies`. Original ELF:
SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Keep frozen menu-stack/action modules and the authoritative root AS graph intact.

## Important source distinctions

- MenuBase.Show `425450` is NOT the stack's OnShow callback. It calls lowercase
  `onPush`; MenuBase.Hide `424af4` calls lowercase `onPop`. Both are additional
  source operations on the same actual character.
- IsValidMenu `41b3f0` reads byte+7c. SetVisible `4223bc` tests that virtual getter
  again, obtains the live weak menu character, writes character+9b and menu+74.
  Valid menu with absent/expired character can reach a null write in original;
  native implementation must stop at that invalid continuation, not accept it.
- Base GotFocus `41b3e8` and LostFocus `41b3ec` are genuinely empty. Concrete
  overrides must dispatch by recovered class, not presumed base behavior.
- Global MenuBase rollover-event flag `9a487c` and IGM-opened `9a4863` are distinct
  from frozen stack `isInGameMenu 9f6400`.
- The Application byte+ec write is preserved as its raw source field until its
  producer/consumer meaning is established; do not rename it World paused.
- Hide's language branch calls **SavegameManager** getLanguage `46d514`,
  setLanguage `46d104`, and saveSettings `46cb34`. Those are not Level pause
  methods. Earlier broad dependency notes must not be read as semantic proof.

## Complete Show/Hide order from static instructions

Show:

1. Virtual IsValidMenu; invalid returns.
2. Debug.Load then GetSwitch("isTracingMenuBase").
3. Write global rollover flag=1.
4. If menu+75 is zero, virtual ProcessLocalization `422d10`.
5. SetVisible(true), then fresh weak menu character/render.
6. RenderFX.InvokeASCallback(character,"onPush",null,0) `7abe0c`.
7. Write menu+78 word=0.
8. Application+ec = (menu name == menu_VerificationLoading).
9. menu_language or menu_splash calls GSInit.ClearLoadingScreen `384ef0`.
10. menu_Ingame, menu_playlist, or menu_Merchant sets global s_igmOpened=1.
11. menu_Options refreshes source cached `option_Custom`, reads
    Application.IsLevelRunning `320f5c`, then sets that actual character visible.
12. RegisterDeadZones `42331c` for every valid Show after the above branch.

Hide:

1. Virtual IsValidMenu; invalid returns.
2. Debug.Load then GetSwitch("isTracingMenuBase").
3. If menu+5c drag object exists, DragAndDrop.ResetPositions `412aa8`.
4. menu_CharacterMenu or menu_Merchant clears MenuManager+60 via fresh instance.
5. menu_language queries saved language; if unsigned result>7, sets language0
   then saves settings, preserving call order.
6. menu_Ingame, menu_playlist, or menu_Merchant clears s_igmOpened=0.
7. menu_VerificationLoading clears Application+ec=0.
8. SetVisible(false), fresh MenuManager instance, UnRegisterListener.
9. Fresh weak menu character/render, InvokeASCallback(character,"onPop",null,0).

All receiver/field rereads and synchronous mutation must be compared against
original execution before implementation freeze. No provider above may be
replaced by an accepted empty callback merely because it is not exercised by a
particular local test. In-game callers do not reach language/menu selection
branches normally, but those branches still need explicit required services.

## Localization and dead-zone ownership

ProcessLocalization `422d10` requires the live menu character; a missing one
returns without setting localized+75. Its loop handles source StringManager
sections **19,20,28**, re-fetches current pack/counts and exact symbols/text,
sets RenderFX context, finds the actual authored target by ordered name formats,
checks target type0x20, and sets its text property. It sets localized+75=1 only at
the completed source tail. Existing HudText/Localization native ownership should
be reused for rows/text, while exact target-name and retained edit-text setters
must be implemented, not localized=true without work.

RegisterDeadZones `42331c` checks byte+b4 and initial live menu character. It sets
registered+b4=1 BEFORE debug/search, invokes actual FindCharacters `7a8c08`, then
for each live result calls source character-box producer `416a7c` and appends a
float4 rectangle to this menu's owned vector. Source geometry/search/viewport
producers and exact search literal are next capture targets. An empty real search
is a valid completed state; an unavailable search is a required failure. No
invented full-screen rectangle or accepted empty list.

## RenderFX linkage

PlayAnim `7aba04` is an 8-byte tail wrapper to GotoFrame `7ab924` with play=true.
GotoFrame rejects null/non-source-type2; calls character virtual+9c labeled goto;
only on success calls virtual+94 with !play (0 here), then returns true. Missing
label returns false and must preserve source focus_in→show fallback ordering.

SetFocusDefault `7ac444` performs actual FindCharacters under render.context,
takes first result and SetFocus `7ac228` or calls ResetFocus `7ac410` if empty.
SetFocus captures previous focus, checks flags0x40 and prior source type2/focus
enabled, plays focused-out animation and sends events, assigns the owned smart
focus reference, invokes concrete event controller for incoming focus, clears
focus if rejected, otherwise plays focused-in animation and sends events.
ResetFocus calls SetFocus(null,index) then clears the secondary smart reference.
Existing source event-dispatch/core and retained-frame APIs should be composed;
not reduced to setting an invented Boolean member.

## Proposed native ownership/API

A NEW MenuLifecycleOwnerV1 should borrow the published MenuStackOwnerV1 and SAME
SwfAsGraph/lease. It owns only genuine MenuBase adjunct state (localized flag,
word+78, dead-zone registration/vector, concrete source-class metadata), with a
retained binding from each stack record to its actual movie character. Direct
projection writes synchronize with that actual core object. No new AS player,
cloned menu graph, or parallel compatibility menu model.

Core operations should run inside the root facade's existing Scope: actual
character visibility, labeled goto/play state, native AS invocation, retained
focus/event controller, localization edits and geometry/search. The adapter
services supplying debug, drag position resets, loading state, real settings,
Application fields and manager/listener ownership remain mandatory. Required
world/HUD/input policy belongs to its actual captured call sites rather than a
guessed generic Show pause hook.

Before code: capture source MenuBase constructor `4269dc`, source Enable/load
producer and exact class selection/weak character binding; capture FindCharacters
recursive producer and character-box conversion; then compare original Show,
Hide, localization, dead-zone and focus/goto compositions with mutation/reentry.
Root supplies retained movie lifetime and AS bridge; this module supplies source
lifecycle state/order and concrete core operations without touching root files.
