# Existing frontend window focus-resume evidence (B006)

## Expected behavior and scope

Returning from gameplay to the frontend reuses the caller-owned `Window` and
`Renderer`. The menu must render while that window is in the background. Mouse,
keyboard, and text events are admitted only when the same window has actual OS
focus. A focus-loss edge cancels held keys and pointer captures; regaining focus
binds native input if needed and resumes fresh input. This is the deliberate PC
host adaptation. It does not force foreground focus and does not revive an HP-0
character.

## Visual evidence

The original reference is Dungeon Hunter 2 v1.0.3, [gameplay video](https://www.youtube.com/watch?v=z_Zky7qQdYs).
I inspected the existing sparse frame sheet at
`.local-inputs/v19-frontend-hotfix/death-menu-restart/video-scan-30s.png`, sampled
at 30-second intervals through 22:00. It contains authored name-entry, story,
and gameplay screens; it does not show the reported death-to-menu sequence or a
window focus transition. The source game is Android and provides no direct visual
counterpart for this Windows host-focus policy, so no original focus behavior is
claimed.

The current-port consumer capture
`.local-inputs/v19-frontend-hotfix/death-menu-restart/death-menu-preview.png`
shows the returned frontend menu still rendered with the selected Rogue profile
and Start Game / Options / Info controls. The runtime log records the upstream
Pause → Main Menu save at frame 120 with HP 0. This is port reproduction evidence,
not an original-game image or proof that the second Single Player action works.

## Recovered logic and current caller

The B006 recovered-source note is
`port/windows-foundation/features/persistence/death_restart_v1_evidence.md`.
Its IDA export is APK v1.0.2 and the video is v1.0.3. `Application::GoToMainMenu`
at `0x32c1f4`, reached via `NativeGoToMainMenu` at `0x43ae9c`, quick-saves the
active Level/player, resets the menu manager, switches to Flash, and removes
players. The recovered function has no host-window focus gate. The separate
offline death-screen callback is `NativeReviveAllPlayers` at `0x43b1e4`; the
pause-menu return is not that revive path.

In this port, `Window::focused()` in `port/windows-foundation/platform_win32.cpp`
is the actual `GetFocus() == hwnd` check. `FrontendRuntimeV1::attach` previously
rejected `!window.focused()` before it could bind the menu services. The native
host's `attach_focused_window` in `features/frontend/native_host.cpp` obtains
`GetFocus()`, verifies the current process, and subclasses that HWND; SDL uses
`SDL_GetKeyboardFocus()` and watches focus-loss events in
`features/frontend/native_host_sdl.cpp`. The presentation consumer polls the
same runtime, takes native host events, and routes keys/pointers/text through
`ScreenInteraction`; its focus-loss handler clears captures and pressed paths.

The exact failure is preserved in
`.local-inputs/v19-frontend-hotfix/death-menu-restart/same-process-death-restart.log`:
real Rogue/Lizard hit 14, death `End34` serial 83 and body removal, then Pause
Yes/save at frame 120, followed by `Existing frontend Window is not focused`
before the next Single Player click. The matching args file is
`same-process-death-restart.args`.

## Implementation and focused verification

The reusable contract is in `frontend_runtime_v1.{hpp,cpp}`. Attaching the
rendering runtime no longer requires focus. Native input binding is deferred
until a poll observes actual focus. `input_focused()` reports the live host
state, and `take_input_events()` drains/discards physical events while
unfocused, collapses a focus-loss edge to one cancellation event, and resumes
delivery after focus returns. The presentation caller must consume events via
`runtime.take_input_events()` and clear its interaction/pressed state on the
returned `focus_lost` event. Programmatic diagnostic `action_script` remains an
explicit source-command path rather than simulated physical input.

Before implementation, the focused checks were set to cover: initially
unfocused attachment policy; no host binding or physical event admission until
real focus; focus regain; one focus-loss cancellation edge; stale queued input
suppression; keyboard and pointer capture cancellation; and fresh input after
focus returns. The feature tests are `frontend_runtime_v1_tests` and
`frontend_input_tests` in the frontend CMake test list. On the current terminal
sources, both tests pass (`frontend_runtime_v1_tests`: 28 contract checks;
`frontend_input_tests`: capture canceled across focus loss and fresh input
accepted after regain). The actual `frontend_runtime_v1` library also compiles
and links with the updated runtime implementation. These checks exercise the
portable focus policy and input adapter; they do not instantiate a native
Window or exercise the presentation caller.

This report covers source and isolated focus-policy verification only. The lead
owns the presentation callsite and the normal same-process executable proof;
B006 remains open until that production path returns to Single Player on the
same profile and the independent reviewer confirms the no-duplicate/no-save-
corruption conditions in `docs/BUGS-AND-IMPLEMENTATION.md`.
