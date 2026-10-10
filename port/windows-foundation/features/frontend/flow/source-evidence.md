# Recovered desktop frontend navigation

`menu_flow.hpp` is an independent C++17 navigation projection in namespace
`dh::foundation::frontend::flow`. Link `menu_flow.cpp`; the test executable links
`menu_flow_tests.cpp`. It has no gameswf, Android, renderer, network or save-store
dependency. `Services` loans actual save creation, assignment, level start, and
optional synchronous stack presentation. Absent owners fail explicitly.

## Original evidence

IDA input directory: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so`.
Addresses below identify its `pseudocode/<first-four-hex>/<eight-hex>.c` files.

| Address | Recovered operation |
| --- | --- |
| 0x43b1b4 NativePushMenu | Converts arg0 using to_xstring, ignores extra args, pushes named MenuManager state. |
| 0x43b158 NativePopMenu | No args invokes virtual top-pop; args convert arg0 then named-pop. |
| 0x439dd8 NativePopAllMenus | Virtual pop with all=true. |
| 0x43ac28 NativePopAllAbove | Exactly one STRING/OBJECT arg, to_xstring then named-pop with above=true. |
| 0x43ae9c NativeGoToMainMenu | Application::GoToMainMenu(event0). |
| 0x431924 MenuManager::PushMenu(name) | Resolve registry name, only push if found. |
| 0x438278 MultiMenuManager::PushMenu | Original menu roster includes MainMenu, StartGame, EnterName and SelectClass; authored lifecycle is more extensive than this projection. |
| 0x439270 MultiMenuManager::PopMenu(name,above) | Named removal and distinct above removal; lifecycle invokes OnHide, hide animation and state removal. |
| 0x42c62c MenuMainMenu::Show | MainMenuError AS callback and exact error-symbol mapping for events1-6/8/9; events0/7 have no error string. |
| 0x43f630 NativeCreateSaveSlot | Name/class args; playable CharacterTable class required; allocate actual next free slot, generate seeds/date, save; returns that slot. |
| 0x43cf50 NativeAssignSaveSlotToPlayer | Slot/player arguments must be nonnegative; calls actual assignment. |
| 0x43e0d0 NativeStartGame | Uses real save difficulty/progression and offline level loading; online handling requires separate real owners. |

`authored_navigation_evidence.json` contains actual decoded DoAction rows from
the unmodified `port/android-native/app/src/main/assets/original-cache/data/menus/dqmenus_droid.swf`,
SHA256 `c114c7d39b7fe0351a93a515c78f9457dda715d78aed6e3bcc3f4895120de421`.
`capture_evidence.py` reproduces it using the repository's original-bytecode decoder.
Offsets refer to decompressed SWF byte offsets, not native addresses.

| Original sprite / frame / offset | Actual path and behavior |
| --- | --- |
| root/sprite510 frame0, onRelease function291293 | Main `btn_MENU_SINGLE_PLAYER`: tests FirstTimeClicked and NativeStartFromGCInvite; sets IsMultiplayer=false. Empty mySaveSlot.InUse pushes menu_EnterName (push291424), occupied pushes menu_StartGame. |
| root/sprite93 frame0, trim35975 / isValidName36337 | Trim checks only codes9/10/13/32. Accept if trimmed name is nonempty and raw length>0. Punctuation and other whitespace are not disallowed. Validation does not replace raw name. |
| root/sprite93 frame0, isFullString36470 | Authored keyboard considers length>=8 full. Input owner must enforce keyboard behavior separately; accept_name does not invent an additional cap for externally committed text. |
| root/sprite93 frame0, btn_Accept function37249 | isValidName -> copy raw field to PlayerName -> NativePushMenu(menu_SelectClass). Original sequence is name THEN class. |
| root/sprite428 frame0 | Class chooser btn_left/btn_right onRelease, btn_Confirm.on_clicked, back NativePopMenu. ClassNumber/PlayerClass are retained source variables. |
| root/sprite428 frame29 | Confirmation executes NativeCreateSaveSlot(PlayerName,PlayerClass), NativeAssignSaveSlotToPlayer(SlotID,0), sets current_slot, NativePopAllAbove(menu_MainMenu), then NativeStartFromGCInvite when InvitePending or otherwise NativePushMenu(menu_StartGame). |
| root/sprite471 frame0, single-player function272952 | StartGame submenu single-player button calls NativeAssignSaveSlotToPlayer(SlotID,0), then NativeStartGame(CurrentDiff). |

The class roster KnightPlayerBase/RoguePlayerBase/MagePlayerBase is also already
recovered in `port/android-native/app/src/main/cpp/front_ui_session_v87.cpp`
`current_class`/class-selection callback code. Desktop selector bounds are0-2.

## Host hookup

- Main single-player authored release: `single_player(SlotFact{id,in_use},error)`;
  slot facts must come from the save owner, including an explicitly selected empty slot.
- Name Accept: `accept_name(raw,error)`; name Back: `back(error)`.
- Class arrows: `select_class(index,error)`; Confirm: `confirm_class(error)`;
  class Back: `back(error)`.
- StartGame single-player release: `start_game(difficulty,error)`.
- Main multiplayer release: `multiplayer(error)` gives
  `MENU_MULTIPLAYER_NO_CONNECTION` with an explicit unavailable status. A subsequent
  single-player release restores `offline_single_player` mode.
- Options/info/help screens use `push(source_name,error)` and `back(error)`.
- `navigation_callback` receives strings already converted by the actual AS bridge.
  For NativePopAllAbove the bridge must also perform the original type guard.

`present_stack(previous,next,error)` must implement the real renderer/movie
lifecycle if rendering is claimed. The model commits only on presentation acceptance.
It does not execute OnShow/OnHide bytecode, transitions, audio, focus or avatar
cameras itself. Simple tests omit this owner and claim navigation state only.

Successful create and assignment prefixes are remembered on a failure so retries
cannot create another profile. This is a declared desktop retry policy; APK retry
semantics are not inferred. current_slot is published after successful assignment.
The launch callback accepts a request and does not claim the level rendered.

## Missing callback / feature list

Not implemented here: NativeStartFromGCInvite, NativeOnlineSanityCheck,
NativeIsMultiplayerEnabled, NativeConectionType (original spelling),
NativeIsBluetoothWifiEnabled, NativeGCAutoMatch, NativeDoWeHaveInternet,
NativeEnableGameCenter, NativeGLXPlayerLoggedIn, NativeGLLive,
NativeGCLeaderboards, NativeGCAchievements, NativeLaunchIGP,
NativeHasPushNotification, NativeSetSaveSlotIDToMainMenu (avatar owner),
NativeGetSaveSlotDetails, NativeEraseSaveSlot, NativeFindFreeSaveSlot,
NativePlaySoundFX/NativePlayMusic, NativeGetStringFromSymbol/NativeGetParsedString,
ClassNumber animation/frame execution, NativePushState/FS_SwitchMenu,
and actual keyboard/QueryString dispatch. These require their authoritative owners;
unknown callbacks fail and no online response is fabricated.

The desktop pure-state roster is intentionally limited to recovered frontend
states. Unknown names and duplicate active states fail; the original whole
multi-movie registry and animation lifetime are outside this adapter.

## Verification

Compiled with `g++ -std=c++17 -Wall -Wextra -Werror`; 64 assertions pass.
Checks cover original name/class order, punctuation/whitespace, empty and occupied
slots, missing owners, partial create/assign/presentation failures, idempotent
completion, start assignment/load failures, online unavailable mode, native
navigation arguments and error-symbol mapping. Fixtures are explicit providers;
this is not a packaged-game test.

## Runtime presentation projection

Link `presentation.cpp`. `presentation(menu_name,PresentationFacts)` returns
actual AS label selections, hidden authored paths, dynamic/localized text, and
main/class/saved-avatar scene policy. Art must resolve string labels to actual
sprite frames; projecting timeline frame0 produces the wrong icons and dialogs.
Main source onShow SWF290280 sets show/Idle, BrownBG=false and TitleGraphic/
RenderedBG=true. Icon label source290038 selects MenuSingle, MenuOptions, GLogo,
MenuI. Source getSlot292352 branches on actual InUse/slot index, hides empty
profile details/delete/difficulty arrows and emits GAMEPLAYMENUS_EMPTY; profile
names/classes/levels/location/date must be loaned from actual save owners.

Name onPush36839 sets MenuBack, TitleGraphic=false, RenderedBG=false; onShow37010
shows BrownBG and Accept. Class native Update at0x428540/0x42859c, plus current
production update_class implementation, selects MENU_CLASS_00/01/02 and
MENU_KNIGHT_DESC/ROGUE_DESC/MAGE_DESC with boundary arrows index0..2, no wrap.

The initial keyboard active uppercase layer is a reference-informed recovery:
sprite93 initializes isCaps=false at36820, but does not initialize child layer
visibility. The decoded droid SWF has no DoAction tags for keyboard sprites89,
91 or92. Both placements appear in static extraction; renderer must select the
actual visible layer. Presentation accepts `upper_keyboard_visible` separately
from source isCaps. Shift40715/44160 toggles isCaps then sets UpperCase=isCaps,
LowerCase=!isCaps; source first shift from false can retain UpperCase. This
specific initial visibility cannot be claimed as a recovered AVM statement.

## Profile chooser regression audit (2026-10-09)

Visual evidence inspected: `C:/Users/adamc/AppData/Local/Temp/codex-clipboard-b847e6e5-6c9c-4e2d-912b-3ce0f3bb7c93.png` (the supplied still; capture time/frame is unavailable). It shows the StartGame profile page for `BOBGRATT`, a Warrior portrait, and repeated `WARRIOR` values across the profile panel. This is direct evidence of bad selected-profile text projection; it does not prove the saved class or any runtime action. A separate live GUI check is still required to verify the fix.

Logic evidence is the recovered `root/sprite510` action set in `authored_navigation_evidence.json`, decoded from the original droid SWF. `getSlot` at offset292352 calls `NativeGetSaveSlotDetails(current_slot)` and populates SlotText, PlayerInfos, class, location, act, save date, level and difficulty fields; its empty-slot branch hides PlayerInfos, PlayerRender and Delete. The paired slot handlers at offsets293732/293909 bound movement to slots0..3, call `getSlot` for the new index and pass the returned SlotID to `NativeSetSaveSlotIDToMainMenu`. Delete opens `Confirmation` with `Show` at294191. Refuse hides it and calls `NativeHUDInteract("no")` at294371. The accepted erase path at294508 calls `NativeEraseSaveSlot`, refreshes the same `current_slot` through `getSlot`, then updates Main's selected SlotID. The original title uses `root/sprite510` frame0; static screenshot gives no animation timestamp.

The helper contract now carries the exact selected `SlotFact{id,in_use,save_path}` through inspection and selection. Removal receives that exact fact only after explicit confirmation; after the host operation, it must re-inspect the same slot and return its fresh empty-slot path. The generic Load route receives the same fact before assignment. If both the fact and slot resolver provide paths, the adapter rejects any mismatch without loading, mutating shared state or assigning. Repeated `single_player` entry clears name/class/start state; the class input owner synchronizes the new class row when it enters SelectClass. After a new profile is saved, the selected slot fact is refreshed before assignment.

Saved-profile projection may populate only name, source class label, level and fields that the actual selected-file metadata provider confirms (act, localized location, difficulty and save date). Missing specialization labels and other unavailable metadata are omitted. The runtime rebases those projected fields to the authored `menu_StartGame/PlayerInfos/...` receivers; without a projection it hides the static Warrior placeholder panel. The preview evidence in `preview/EVIDENCE.md` confirms Knight, Rogue and Mage all use the source `models/prince_modular.bdae`; their idle/selection clips and starting attachments differ. The generic class preview does not own a loaded saved-profile Character/Gear avatar, so it cannot establish saved Rogue appearance.

Focused verification after the helper edits: MinGW feature targets `menu_flow_tests`, `screen_interaction_tests`, `runtime_creation_flow_adapter_v1_tests`, `creation_adapter_tests`, and `frontend_runtime_v1_tests` passed CTest 5/5; `frontend_runtime_v1` built. After adding the final explicit upper-slot boundary assertion, standalone MinGW C++17 runs passed `menu_flow` (80 assertions) and original input/slot/remove/repeat-flow behavior. A new full Ninja relink is currently blocked by an unrelated shared-header mismatch in `playable_actor_world.hpp:74` versus `combat_system.hpp:52` (`resolve_damage_with_outcomes`, seven vs eight parameters); no frontend source is named by that diagnostic. Tests cover slot bounds/selection path, remove refusal/accept and exact path, fresh create state/name/class row, exact selected-file Load and resolver mismatch refusal, metadata omission, and Main-to-StartGame receiver rebasing. This is helper-level verification only. Main callback wiring for the selected-file projector and recoverable same-directory rename, plus an isolated rendered GUI check, remain pending lead/root integration and a reserved window.

## Original class scene lifetime and button release proof

Original IDA MenuCharacterSelect::Show0x428f38 begins by destroying the Main
scene and avatar camera, transfers its save slot, marks Main destroyed and
resets its slot to-1. It then binds `class_select` to RenderClassSelectPane and
constructs `CLASS_SELECTION.bdae`. Hide0x4283c0 removes class scene nodes, flushes
objects/animation sets, restores Main camera/slot/scene/character. The original
Main scene cannot render underneath the class pane after Show. Its former
RenderCharacterPane0x42bb44 is an empty exported body.

RenderClassSelectPane0x428b74 obtains the actual `class_select` absolute rectangle,
divides its coordinates by original InvPixelScaleX/Y, casts to integer viewport,
calls scene-manager virtual+60 with0, then restores prior viewport. The meaning
of that virtual argument/clear behavior belongs to the scene renderer owner;
this adapter makes no invented color-clear, ground-coverage or viewport claim.
Background RenderedBG=false is inherited from actual Name onPush along this
supported route; original class AS onShow257898 is empty. A direct unsupported
Main->Class entry must not pretend the class AS independently hides RenderedBG.

New `component_init_evidence.json` and `clip_actions_evidence.json` decode all8
original generic DoInitAction blocks and all4 PlaceObject2 clip events using
reproducible capture scripts. These do not contain button Idle initialization
or KeyboardResize/_xscale/_yscale sizing logic. Name Confirm is generic sprite69
under buttons70, nested Accept68. Its actual labels are lowercase idle/pressed/
released/focus_in/focus_out and Appear/Gone. Native RenderFX::UpdateCursor0x7ac924
plays pressed on capture and released on release (clicked fallback). The
presentation uses idle for unpressed Accept (actual source idle Stop4 has zero
glow alpha; released Stop13 still has orange glow), released for Shift (its
source Stop13 has zero glow alpha), and pressed
when actual input supplies pressed_button_path. This is a source-native input
state projection; an AS onLoad Idle initialization is not claimed.

## StartGame sparse-profile and third-button regression (2026-10-10)

Visual evidence before the fix: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/v19-frontend-hotfix/current-gui/rogue-start-metadata.png` (1280x720, StartGame after selecting the genuine Rogue profile). The rendered page showed the correct QA name, Rogue class and Level 1, but six unrelated fields still read `WARRIOR` and the third button read `BtnText`. This proves the SWF timeline's static text survived sparse profile projection; it does not establish what values the original save contained. The screenshot was inspected before editing.

Logic evidence: original `root/sprite510` `getSlot` (offset292352) reads `NativeGetSaveSlotDetails(current_slot)` and projects profile fields into `PlayerInfos`. The current portable save schema does not provide source facts for act, location, save date, or difficulty, so those exact text receivers (`Hud_Act`, `Hud_Location`, `Last_Save`, `Last_Save_Infos`, `DifficultyTitle`, `Difficulty`) must be explicitly cleared rather than inherit authored Warrior placeholders. Original `root/sprite471` onShow localizes `StartMenuButtons.btn_MENU_Leader.text.htmlText` with `MENU_LEADERBOARDS` (offsets272223–272251), `btn_MENU_SINGLE_PLAYER.text` with `MENU_SINGLE_PLAYER` (272259–272287), `btn_MENU_MULTIPLAYER.text` with `MENU_MULTIPLAYER` (272288–272323), and `btn_Achievements.text` with `MENU_ACHIEVEMENTS` (272324–272358). The base authored StartGame frame in `art/original_art_data.cpp` contains the exact `menu_StartGame/StartMenuButtons/btn_Achievements/text` field; that is the third visible row in the captured base layout. `btn_MENU_Leader` is an alternate source frame. Therefore the rendered base third row must resolve `MENU_ACHIEVEMENTS`; the Leaderboards symbol is also projected for the alternate frame.

Expected behavior: project only selected-profile source facts; render the six unknown fields blank, preserve known name/class/level, and localize each source button receiver. In the captured base frame the third row is `btn_Achievements` and resolves `MENU_ACHIEVEMENTS`; an alternate Leaderboards frame resolves `MENU_LEADERBOARDS`. The focused runtime smoke also checks that refusal keeps the exact selected save bytes, accepted removal preserves them in one same-directory recovery sibling, arrows retain the canonical slot path when it becomes empty, and the class-selection transition is allowed to finish before input is accepted.

Uncertainty: the screenshot is a single frame and does not prove the source movie's full animation/focus lifecycle. No post-edit image has been captured yet. The camera/viewport issue is separate: source `RenderClassSelectPane428b74` derives a viewport from the absolute `class_select` SWF rect divided by `InvPixelScaleX/Y`, then restores the previous viewport; the current host still draws to the full Window viewport. Exact pixel-scale semantics and temporal alignment with the supplied 0:54 class-selection video remain unverified, so no camera correction is inferred here.

Verification plan: compile the current runtime with the production C++17 target, run focused flow/input/text tests, then use the lead-reserved isolated main window and genuine source-created Rogue sandbox profile. Capture the unknown-field blanks and localized third label along with slot arrows, refusal, accepted recoverable rename, create-again, and exact-path Load. Until those checks run, this change is source-edited only; helper assertions are not rendered-runtime verification.

Intermediate rendered evidence from the lead: `.local-inputs/v19-frontend-hotfix/profile-slots-main/rogue-metadata-fixed.png` showed the unknown profile placeholders had been cleared, but the visible third row still showed `BtnText`. This output was reported before the source-button mapping correction. Inspection of the source-art projection showed the base-layout receiver is `menu_StartGame/StartMenuButtons/btn_Achievements/text`; the prior Leaderboards-only assertion checked an alternate-frame receiver and did not assert the actual base third-row field. The current patch adds the source MENU_ACHIEVEMENTS binding and a static composed-field assertion for that exact receiver. It is not considered visually fixed until the next reserved current-main capture passes.

First integrated slot-smoke failure (lead's isolated own-window run, result `C6C5993C8FF12025028C53C30D1DC1674987DA3536E82AA72887A64FC2FE3A85`): captures for phases00–04 reportedly showed the empty/occupied arrows, refusal dialog and byte-preservation check passing; the original selected Rogue file remained exactly 1,011 bytes. The process then exited with Windows status3221225477 when accepting removal. Source review identified the harness's wrapped `remove_selected_slot` callback capturing a local `std::function` by reference after the setup block ended. The accept/rename result was not reached, so this run does not verify recovery rename or later create/load phases. The patch captures the original callback by value; source-owned harness lambdas that outlive setup are being checked for the same lifetime error before rerun.

Second integrated run (`.local-inputs/v19-frontend-hotfix/profile-slots-main/slots.log`, result `73E72A…7C41`) completed the accepted production rename and preserved the original bytes in the recoverable sibling `.removed-1791580344`, then failed in `Navigator::resolve_remove_selected` at `flow/menu_flow.cpp:98`. The production check incorrectly required an empty refreshed slot to have a different path. The actual Main callback correctly returns the same canonical slot path with `in_use=false`; retaining that path is necessary so later slot selection identifies the same slot, while its former bytes now live under the recovery sibling. The stale occupied fact after this bad rejection caused the later metadata projection to try loading the removed pathname. The original file is recoverable and the test seed must be copied to a fresh sandbox path before retry; do not overwrite the recovery sibling or an existing created profile.

Integrated verification after the fixes: lead confirms the frozen executable SHA begins `DB0E4987` and ends `6482E615`; its build included `flow/menu_flow.cpp` SHA256 `24055D1AF0A2ECBC4101780811B810447492B5FBF0CFEB70AB6DDAE8821409C8`, with the object built before that executable. The isolated current-main window exited0 and emitted `profile_slot_smoke: PASS` in `.local-inputs/v19-frontend-hotfix/profile-slots-main-2/slots.log` at 2026-10-09 21:14:55.163Z. All expected phase captures are present in `profile-slots-main-2/captures/`: 00–06 cover initial slot0, genuine Rogue slot1, confirmation, refusal bytes, accepted confirmation, empty slot1 after rename and arrows; 09–11 show Knight→Rogue→Knight; 12 shows the one created Knight profile after two Confirm releases; 14 and16 verify post-create empty/occupied arrows and exact selected-profile page before Load. The log reports 2 Confirm releases, created slot1, exact loaded path `character-slot-1.save`, recoverable sibling `character-slot-1.save.removed-1791580485`, and generic Start delivered. The recovered sibling SHA256 `AF04B723A32CF7B07F78CB102F429AD85C690FFC3443D85F4294CA9E12E5058A` matches the seed bytes. I inspected phase12 and phase16: sparse metadata fields are blank, class/name/level remain, and the base third row reads ACHIEVEMENTS; no WARRIOR or BtnText placeholders remain. This verifies the isolated menu/profile sequence, not live gameplay. The class-scene camera/ground-coverage issue remains separate and uncorrected; these StartGame captures do not measure 1280x720 or 1920x1080 class viewport coverage.
