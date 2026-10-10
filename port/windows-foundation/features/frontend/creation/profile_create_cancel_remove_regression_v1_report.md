# Headless creation/profile return regression (2026-10-10)

## Source and expected route

Reused the existing source audit at `../flow/source-evidence.md` and
`../flow/authored_navigation_evidence.json`; no new behavior was inferred from
the test harness. The original droid SWF is identified there by SHA256
`c114c7d39b7fe0351a93a515c78f9457dda715d78aed6e3bcc3f4895120de421`.

- `root/sprite510`, frame 0, action `onRelease` at decompressed offset 291293
  tests the selected profile's `InUse`: empty enters `menu_EnterName`, occupied
  enters `menu_StartGame`. The source arrow range is slots 0–3.
- `root/sprite93`, `btn_Accept` action at 37249 validates the authored name,
  stores it, and pushes `menu_SelectClass`. Back uses `NativePopMenu`.
- `root/sprite428`, frame 29, Confirm calls `NativeCreateSaveSlot`, assigns the
  new slot to local player 0, pops above `menu_MainMenu`, then pushes
  `menu_StartGame`. `root/sprite471` StartGame single-player release assigns
  `SlotID` and calls `NativeStartGame`.
- `root/sprite510` opens the erase confirmation; refusal calls the source “no”
  path and refreshes the selected slot. Original acceptance calls
  `NativeEraseSaveSlot`. The desktop provider preserves the same user bytes via
  a same-directory recovery rename, then re-inspects the same canonical slot
  path. That rename is an intentional recoverability policy, not a claim that
  the native function renames files.

Visual evidence is reused from `../flow/source-evidence.md`: the supplied
StartGame still shows the selected-profile panel, and the prior reserved
isolated main run recorded phase captures for refusal, accepted removal, repeat
creation, and selected-profile Load. Those captures are UI evidence; this new
test is headless and does not claim a new rendered-runtime result. The source
asset is the recovered v1.0.2 SWF; the repository’s separate reference video is
v1.0.3.

## Test and result

New `profile_create_cancel_remove_regression_v1_tests.cpp` loads the actual
13-input `RuntimeCreationSourceV1` snapshot and exercises the existing
`GenericCreationFrontendHostV1`, `RuntimeCreationFlowAdapterV1`, and
`flow::Navigator`. Its filesystem slot callbacks use a unique OS temp
directory, the authored four-slot range, exact canonical slot paths, and a
recoverable rename sibling. It verifies:

1. Empty-slot EnterName → SelectClass → Back → Back leaves every slot empty,
   the caller’s `CharacterState` unchanged, and create/assignment/start counts
   at zero.
2. First Confirm persists/reloads into the same state owner, creates only once
   on repeated Confirm, and reaches StartGame before the separate start action.
3. StartGame Back returns to Main with the same selected slot. Removal refusal
   preserves exact bytes and path. Accepted removal keeps the canonical slot
   path empty and saves byte-identical contents to a recovery sibling; slots
   1–3 remain untouched.
4. Repeat creation reuses that selected slot path, updates the same shared
   state in place, then returns a valid same-owner/slot generic start receipt.

The test also runs the existing pause source hit-test/router for authored Main
Menu → confirmation Yes and asserts `return_to_main_menu`. That only tests the
feature route result; it does not execute main's game-session teardown.

Run command:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File port/windows-foundation/features/frontend/creation/run_profile_create_cancel_remove_regression_v1.ps1
```

Result: PASS using the current `.local-inputs/frontend-feature-build`
`libfrontend_components.a` and `.local-inputs/windows-foundation-build` archives
with LLVM-MinGW, `-std=c++17 -Wall -Wextra -Werror`. The only disabled warning
is `-Wmissing-field-initializers` for the existing public runtime config's
`flow::SlotFact selected_slot{0, false}` aggregate default. Output confirmed
first save size 1,025 bytes; refusal equality; recoverable rename; two source
creates, four authored assignment calls, two same-state starts. The runner
removes its test executable and the test removes only its unique temp directory.

## Death/return branch boundary and main integration

Read-only source review found no `pauseMainMenuRequested` identifier in the
current repository or frontend feature. The current root-owned implementation
uses `returnToFrontend` in `port/windows-foundation/main.cpp`: the pause route
sets it after confirmation Yes (around line 2123); the break path around 2230
attempts active-session checkpoint validation and saves the local character;
the outer loop around 3007 resets to `startMode="menu"` while retaining the
selected save slot. Source recovery for original `Application::GoToMainMenu`
`0x32c1f4` shows, for active level type 38, QuickSave and `SG_SaveLocalPlayer`,
then MenuManager reset, `GSFlashMenu` switch, player removal, and status-bar
transition. `NativeGoToMainMenu` `0x43ae9c` calls it with event 0.

The source pause projection already exposes a pure test seam:
`source_pause_ui_frame_v1` → `source_pause_ui_hit_test_v1` →
`source_pause_ui_route_v1`; it emits `return_to_main_menu` only for the
confirmation Yes button. No frontend service currently consumes that result as
a death-triggered return request. The inspected `PlayerManager::AllPlayersDead`
`0x36e764` and caller `ScriptManager::StartScript` `0x4605c0` provide an
all-dead script admission gate, not evidence that death itself enters the
pause-confirmation/Single Player route. The causal death → frontend transition
remains unverified and should not be inferred from pause behavior.

The existing production registration is already at root-owned
`port/windows-foundation/main.cpp` around lines 475–650: it binds
`flowServices.select_new_profile`, `selected_save_path`,
`assign_selected_slot`, and `start_same_state`, installs exact slot
`inspect_slot` / `select_slot` / recoverable `remove_selected_slot`, creates
`GenericCreationFrontendHostV1`, and calls `run_frontend_v1` using the same
Window/Renderer. Therefore no main/CMake registration is needed for this
headless helper. The remaining integration/acceptance test for the death path
must name its genuine source trigger and main-owned return policy; feature code
must not translate generic death into `NativeGoToMainMenu` by assumption.

## B007 focused route guards (2026-10-10)

Visual evidence was re-inspected from the supplied original gameplay-video screenshots saved unchanged at `../verification/original-name.png` and `../verification/original-class.png`. The player overlay shows `1:06 / 9:47` on the Enter Name screen: back arrow at upper left, Confirm above the keyboard. At `1:20 / 9:47`, the class chooser shows the same back arrow, left/right class arrows, selected Rogue and Confirm. These are stills, so they establish visible controls and selected content only; they do not establish transition timing or delete-confirm behavior. I also inspected the local Criss82 v1.0.3 video at 01:05, 01:06, 01:19 and 01:20: the first pair stays on an occupied Warrior main menu and the latter pair is a gameplay cinematic, so it does not show the empty-slot creation route. The supplied creation screenshots are from the separate RadiaGamer7 v1.0.3 video; the recovered menu SWF is v1.0.2. This leaves original create/back/remove transition visuals and their timing unobserved.

Logic evidence is reused from `../flow/authored_navigation_evidence.json` and `../flow/source-evidence.md`, then checked against the actual recovered callers. `root/sprite93` action at decompressed offset 37249 validates and stores the raw name before pushing `menu_SelectClass`; Back calls `NativePopMenu`. `root/sprite428` frame 29 calls `NativeCreateSaveSlot`, then `NativeAssignSaveSlotToPlayer(SlotID, 0)`, publishes `current_slot`, pops above Main, and pushes StartGame. Native `0x43f630` validates the class through `CharacterTable`/`IsPlayableClassID`, asks `PlayerSavegame::SG_GetNextFreeSlot`, saves, and returns that slot; `SG_GetNextFreeSlot` is `0x4660c8`. The Main slot arrows at `root/sprite510` offsets 293732/293909 constrain the selected slot to authored indices 0–3. The erase caller opens `Confirmation` at 294191 and only invokes `NativeEraseSaveSlot` at 294508 after accept; refuse at 294371 leaves the selected save untouched. StartGame's caller at `root/sprite471` offset 272952 assigns the selected slot to player 0 before calling StartGame. No outcome is inferred from a failed provider callback; provider-missing retry visibility and invalid desktop owner return are port-side guards.

Expected behavior: retain the authored name→class→confirm route, create only into one of four selected slots, assign player 0 before presenting StartGame, and preserve the selected profile on refusal. The minimal route correction is to reject a returned slot outside 0–3 before assignment/navigation and to leave an accepted erase confirmation visible when its required provider is unavailable, allowing retry or explicit refusal. This does not claim that the original app has a missing removal provider; it makes the desktop adapter's failure state recoverable.

Focused verification was defined before editing: feed a create owner that returns slot 4 and assert no assignment or menu advance; feed an occupied selected slot with no remove owners, accept once, verify the confirmation remains visible, then refuse and verify it closes. The strict feature CMake target rebuilt from the changed `menu_flow.cpp`; `ctest -R '^menu_flow_tests$'` passed 1/1, including both branches. The existing actual-source 13-input create/back/refuse/remove/recreate/same-slot/start regression then passed with the refreshed `libfrontend_components.a`: cancel had zero slot/state mutation; refusal preserved exact bytes; accepted remove retained recoverable bytes; repeat creation reused slot 0; two source creates, four assignments and two same-state starts. This is isolated focused evidence, not a new normal executable run. Production `main.cpp` callsites were inspected (creation provider at 475–500, exact slot inspection/arrow/removal at 590–635, runtime invocation at 650); they use the same Navigator. Integrated proof of the new guards on a frozen executable remains with the lead.

## B007 invalid create-result retry guard (2026-10-10)

Visual evidence is reused from the source-backed creation screenshots above: the supplied original v1.0.3 stills at 1:06 (Enter Name) and 1:20 (class chooser) show the visible back/confirm and class-arrow controls. They do not show a full-slot race or an invalid CreateSaveSlot result; that visual branch remains unobserved. The recovered movie is v1.0.2, so this port-side error guard is not claimed as an original visible response.

Logic evidence: original `root/sprite428` frame29 calls `NativeCreateSaveSlot(PlayerName, PlayerClass)`, then assigns its returned `SlotID` to player0 and navigates to StartGame. IDA `NativeCreateSaveSlot` at `0x43f630` calls `PlayerSavegame::SG_GetNextFreeSlot` at `0x4660c8`, constructs and saves the profile, and only then returns the slot value. `SG_GetNextFreeSlot` scans consecutive existing slot indices and returns the first gap; when indices0–3 are occupied it can return4. Thus a success callback followed by slot4 is an already-performed save with no valid authored Main slot identity. The old adapter rejected assignment/navigation but left `CreationStage::editing`, so a second Confirm replayed the non-idempotent save callback.

Expected behavior: valid source creation remains name→class→create→assign player0→StartGame and repeated successful Confirm stays idempotent. If the host reports a successful create with a slot outside0–3, reject assignment, retain the class screen and mark that Navigator's creation outcome unmapped; do not invoke CreateSaveSlot again until the host reconstructs/reconciles profile storage. This fail-closed policy is a desktop safety adaptation; the source does not define recovery for this malformed/stale-slot case.

Focused verification was defined before the patch: a provider increments a create counter and returns success with slot4; first Confirm must reject without assignment or navigation, and repeated Confirm must not increment the counter. The strict C++17 focused `menu_flow_tests` rebuilt from `menu_flow.cpp`, `presentation.cpp`, creation text/adapter owners and `character_state.cpp` passes **91 assertions**, including the invalid-result repeat case including a second empty slot, refusal with missing removal providers, ordinary idempotent Confirm and authored `MENU_ACHIEVEMENTS` receiver. The existing real 13-input create/remove/recreate runner was also attempted against the current prebuilt `libfrontend_components.a`; the process terminated with Windows status `0xC0000005` before reporting a result. That archive does not include this uncommitted state-machine edit and is not a coherent rebuilt integration target, so this attempt is not a pass or production-path result. The lead must rebuild the feature archive and rerun the isolated full route before integration acceptance.
