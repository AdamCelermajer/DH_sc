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

## B008 selected-profile projection audit (2026-10-10)

Visual evidence: the supplied still at `C:/Users/adamc/AppData/Local/Temp/codex-clipboard-b847e6e5-6c9c-4e2d-912b-3ce0f3bb7c93.png` (SHA256 `FBD5FD2FF6A6CA964731629B1E4CB4E2923DA5D83780A9DE51F2DE502F5311B5`) directly shows one profile panel repeating `WARRIOR` in every metadata row; as a still it has no timestamp and cannot prove a class transition or launch. The user-provided MainMenu reference `.local-inputs/dh2-final-mainmenu.png` (SHA256 `6596B570EA1E44AD4D0B354B036D8702F96DE6B88CFE36A77C9A3F4DB7EE700B`) shows a selected Warrior profile and an equipped Warrior standing in front of the Boglands statue. For the current selected Rogue production path, `.local-inputs/v19-frontend-hotfix/profile-slots-main-2/captures/profile-slot-01-occupied-rogue-slot-1.png` is direct evidence of `QA / Rogue / LEVEL 1`, with the statue visible but no saved player model submitted. These are three stills; no continuous Rogue launch→return sequence was observed.

Logic evidence: original SWF `root/sprite510` `getSlot` at offset292352 calls `NativeGetSaveSlotDetails(current_slot)` and writes the selected name, class and level into PlayerInfos. Its empty-slot branch hides PlayerInfos, PlayerRender and Delete. The slot arrows at offsets293732/293909 bound indexes0..3, select a new SlotID and call `NativeSetSaveSlotIDToMainMenu`. Original `MenuMainMenu::SetupCharacter` at ARM `0x42bd08` loads the same occupied slot through `Character::CreatePlayer(newProfile=false)` and `SG_Load(4)`; an empty slot creates a fresh profile. `Character::SafeGetCharPropsId` at ARM `0x3b3d38` reads PCLS from that Character's Save (defaulting only when class is -1); `Character::GetCharModelName` at ARM `0x3a54d4` resolves the model name before VisualObject setup and has equipment/game-state override branches. In the native profile handoff, `SelectedProfileBindingV1` retains the exact selected file/Save and `CanonicalCharacterCandidateRecordV60` carries that same class/Gear into native construction. The source class showcase uses `models/prince_modular.bdae` for all three base classes (`preview/EVIDENCE.md`); Rogue appearance comes from its class properties, animations and the selected profile's Gear/daggers, so choosing a different base filename by label would be wrong.

Expected behavior: each occupied slot displays the name, localized class and level from that exact selected Save, with absent campaign fields cleared, and its visible Character/Gear comes from the same Save. Selecting or launching the Rogue profile must keep its Rogue model/appearance and saved gear through the return to that profile panel. The minimal text guard fails the projection atomically when the source provider supplies no class label; this prevents a successful partial binding from leaving the SWF's static Warrior label visible. The new render seam accepts a distinct immutable generic save snapshot from the exact slot/path, chooses its authored class body and applies its actual saved Gear through the source skin/render owners. It does not treat the fixed class-creation starting kit as saved Gear.

Uncertainty: the user-provided source MainMenu still depicts Warrior only, so it cannot establish original Rogue pose or gear. We have source code and native construction evidence for the selected class/model identity, but no integrated visual capture of the saved Rogue actor or Rogue launch→return continuity. Original package/version differences remain possible between the 1.0.2 recovered assets/source export and the referenced 1.0.3 video.


Saved-actor integration detail: `FrontendSelectedProfileSnapshotV1` in
`frontend_runtime_v1.hpp` carries the exact selected `SlotFact` and a
`shared_ptr<const CharacterState>`. `valid_for` enforces matching occupied slot,
path and nonempty Character identity/class. The Main/StartGame render owner uses
the snapshot's class to pick the source class body configuration, parses the
real `loot_table` ItemTable, maps the same state's equipment with
`prepare_source_equipment_appearance`, and applies it through a retained
`VisualSkinOwnerV6`. Debug Load/Get uses the actual source `DebugSwitches` owner,
stdio file services and AssetCatalog path resolution. The source equipment
bridge supplies exact body/weapon geometry and original COMMON texture/pass
binding; only those packets are submitted. The body uses the original menu
camera and the source `(0,-200,-20)` / `PI * -0.125` / class property scale
placement from `renderer_front_draw_v87.inc`.

Focused verification: the runtime contract test now admits Knight/Rogue/Mage
snapshots and rejects adjacent slot, wrong path, empty selection and missing
Character identity. The updated statically linked Windows test passed 35
assertions, and strict MinGW syntax checks passed for both the presentation TU
and snapshot tests. The normal presentation path now emits a one-time
`profile_actor` record for each resolved Main/Start profile render key, including
menu, slot/path, Character/class, appearance-step and total draw-packet counts;
the production slot smoke also asserts matching snapshot, Character, class and
nonempty retained source render packets on every occupied Main/StartGame
capture. This is focused/source preparation, not integrated visual
verification. The lead must build the coherent executable and capture
isolated selected Knight/Rogue/Mage profiles on MainMenu and StartGame, then
verify Rogue launch/return with unchanged Save/Gear identity. No new-build image
is claimed yet.

Concrete integration fixture handoff: read-only inventory found isolated class
saves at `.local-inputs/b003-production-space/knight-isolated.save`,
`.local-inputs/b003-production-space/current-227f/rogue-no-reassignment/rogue-isolated.save`,
and `.local-inputs/b026-mage-current-candidate/profile.save` (the latter's
serialized identity is `MagePlayerBase`). Copy each fixture into a fresh,
unique per-class sandbox save path before starting the normal executable; never
pass the fixture originals as writable game saves. The host accepts
`--start-mode menu --save <sandbox-save-file> --save-slot 0 --menu-capture-directory <unique-capture-dir>`
and uses the ordinary MainMenu→Single Player transition to reach StartGame.
Capture both menus for each class and retain each process log's `profile_actor`
record: its class, save path and Character id must match the copied Save, and
its source Gear packet count must be nonzero. On the Rogue run, use the ordinary
Start action then the source return-to-menu path; compare the same copied Save
bytes and actor/Gear identity before and after. These fixture paths are test
inputs only.

Gear regression finding from the isolated normal-host capture (2026-10-10):
the frozen `E826D7CC45B1EA507F9E390A70A4CAFE839D9523AD2D1771E1363F9CFA900A34`
executable rendered the right class body on both MainMenu and StartGame for the
copied Knight/Rogue/Mage saves, but each `profile_actor` record reported only
four render packets; direct PNG inspection showed no Rogue daggers, Mage staff,
or the equipped Knight longsword. These are fresh copies under
`.local-inputs/b008-production-profile-matrix`, not fixture originals. This
isolated matrix directly reproduced the remaining model projection failure.
The PNGs are 1280×720. MainMenu images: Rogue `rogue/main/final.png`
SHA-256 `BDE2DA208D88F02AF87F178647FDB911E838812667A43D444805DC559A6E74C0`,
Mage `mage/main/final.png` SHA-256
`7ED1810060A609FFF6D79A993DAA6506F055A0FFA02137DCBA0FEC52BDC7FD03`, and
equipped Knight `knight-equipped/main/final.png` SHA-256
`DCA15C61D5CFF433DD3890BDB8BC600012D792B92FBD906A17A7D2B11BE09433`.
Each capture used three menu frames; the MainMenu logs report source-clock
times of 150 ms (Rogue/Knight) and 167 ms (Mage). Corresponding StartGame
images/logs are preserved in their `start/` folders.

Recovered-source cause: `main.cpp`'s production `bindEquipmentPage` maps each
`EquipmentBinding` with `equipment_set <= 0` from `source_slot` to its actual
saved `slot` name (with only the known legacy `main`/`off` aliases). The source
`prepare_source_equipment_appearance` then resolves equipment by exact
`binding.slot == slots[source_slot]` (`features/equipment/source_equipment_appearance.cpp:19-31`).
The profile renderer had instead supplied generic `slot0`...`slot8`, so the
appearance plan treated all nine slots as empty despite borrowing the correct
same-save Character. The patch now builds nine distinct names per immutable
snapshot from these exact source bindings, rejects duplicate/out-of-range or
unknown active mappings, and passes those names to the existing source
appearance owner; it does not generate equipment or replace the Save. The
frozen E826 matrix above is the before-state only. Post-patch integrated
verification used the normal host executable
`.local-inputs/v19-frontend-hotfix/preview-verified-menu-audio/dh-foundation.exe`,
SHA-256 `F35873F611975FAA52D62EED42A7B3CF642D84EB2B490EF923CCEBAFF56DCDC4`.
Each input Save was copied again into
`.local-inputs/b008-production-profile-matrix/postfreeze-f35873`; no fixture or
user Save was passed as writable input. Three-frame MainMenu and StartGame
runs exited 0 for all classes. The exact same selected Save path and Character
`profile-slot-0` appear in each pair of actor records. Before projection the
same-class baselines were four packets; after projection Knight with
`Longsword01` in source slot 1 reports 5 packets, Rogue with `Dagger01` in
source slots 1 and 2 reports 6, and Mage with `Staff01` in slot 1 reports 5.
Each also retains its saved suit/boots/gloves (six appearance steps total).
All six 1280×720 PNGs were inspected: the MainMenu and StartGame bodies and
weapon silhouettes match the serialized Warrior/Rogue/Mage identity, and text
labels read `WARRIOR`, `ROGUE`, `MAGE` respectively. The filenames are
`knight-equipped/{main,start}/final.png`, `rogue/{main,start}/final.png`, and
`mage/{main,start}/final.png` under that capture root. Their SHA-256 values are:

| Profile | MainMenu PNG | StartGame PNG |
|---|---|---|
| Knight | `112721E12254DF20424B5C24781635F17C4C20C3EF4390AE65802A10225D8DBC` | `8BE89621B51D14AB78EB4707876C432E4FDBF976E006634CDF2715B048AB2D92` |
| Rogue | `45BB0DC94C5371A9888B174830E2BB406AA7EC4D92B7E866D0B59EE1292D0E67` | `F61CB830ACD8BE849514BCFE629A4C4FADD9C3FDA327D0E94CDB1FE129E52598` |
| Mage | `8A2E02338FFBCA559095ABBABDF51A7B5D90548A210E82F0C1333245DFF21821` | `9C15CA7A7BA2ECEE3DD09302586B13DD20854023AFFBEA74CB4A3DD07F711D63` |

The isolated Rogue return route used the same frozen executable and another
copied save at `.local-inputs/b008-production-profile-matrix/rogue-return-f35873/profile.save`.
With source controller policy `development-controller.xml`, the ordinary
MainMenu Single Player → Start Game route entered the gameplay host; the source
pause page was opened at frame 0 and its authored Main Menu then Yes hit
regions were clicked at frames 1 and 2. The normal return route wrote the
profile and reconstructed the frontend. The pre-run and post-run save SHA-256
were both `AF04B723A32CF7B07F78CB102F429AD85C690FFC3443D85F4294CA9E12E5058A`.
After return, `profile_actor` reported `menu_StartGame`, slot 0, the same save
path, Character `profile-slot-0`, class `RoguePlayerBase`, six appearance steps
and six render packets. The returned-menu screenshot was 1280×720,
SHA-256 `80E3F6B15F1CBC6D47C1C15558931661FD1D756B3107F7DEEBA6F9CF4FC0DFE2`;
it directly shows the Rogue body, both saved daggers and `QA / ROGUE`. Its
right-side button artwork was still transitioning after the three-frame return
capture, so the settled six-class menu matrix above remains the visual panel
reference. The isolated route's full stdout/stderr are in
`rogue-return-f35873/return-with-controller.log` and `.err`. An audio attempt
was skipped by the real focus gate because this no-activation CLI process had
no OS window focus; no MenuConfirm playback is claimed.

Together, the six normal MainMenu/StartGame captures and the same-save Rogue
start/return roundtrip verify selected-profile labels, class appearance,
equipped Gear projection and returned Character/Gear identity for the isolated
records. The route intentionally advanced no gameplay update (`updateSerial=0`);
it does not claim combat progression or OS-focused audio delivery.
