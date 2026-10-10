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
