# Linux x86-64 preview port

## Scope and expected behavior

This is a small host/backend port of the existing Windows foundation. It keeps
the shared game, frontend, asset readers, mixer, saves, and gameplay sources;
Linux selects SDL2 for its window, OpenGL context, keyboard/event host, and audio
device. The visible invariant is that the existing native frontend can display
the same packaged artwork and UI on a Linux desktop while the renderer still
rejects calls made from a foreign context or thread. The focus gate remains
real: the frontend must attach to an SDL-focused window.

There is no direct original-game visual counterpart to OS window, thread, or
audio-queue adapters. Their consumers are the existing `Window` and
`Renderer` interfaces, `FrontendRuntimeV1::attach`, and the V34 mixer output
consumer. The V34 audio handoff documents the original cue-admission/mixer
boundary; SDL only transports the already-mixed output and introduces no cue
selection or gameplay rule.

## Implementation

- `platform_linux.cpp` creates an SDL2 window and OpenGL compatibility context,
  translates SDL window state and keys into the existing `Window` API, and
  pumps focus events before publishing initial focus. A real unfocused window
  remains unfocused.
- `features/frontend/native_host_sdl.cpp` adapts SDL key, text, mouse, and
  focus events to the current frontend input contract.
- `platform_context_identity.cpp` exposes the current SDL context/window and
  Linux `gettid` identity to the existing renderer-affinity check without
  changing renderer layout.
- `platform_key_codes.hpp` and `platform_sleep.hpp` preserve the existing
  virtual-key callsites and Windows branches; Linux supplies stable key values
  and the monotonic 32-bit uptime compatibility helper used by existing code.
- `features/audio/linux_sdl2_output.*` sends the unchanged V34 mixer output
  through SDL2 queued audio. Its device clock is an estimate based on the SDL
  application queue, not a hardware timestamp.
- CMake selects Win32/WGL/WinMM or SDL2/OpenGL by platform and keeps
  Win32-only GUI probes on Windows. Linux has its own window/renderer smoke
  and SDL audio lifecycle tests.

## Verification

The Linux production target built in a private Ubuntu 22.04 x86-64 chroot with
GCC 11.4, SDL2 2.0.20, and OpenGL. The `linux_platform_smoke` test passed under
Xvfb/Mesa llvmpipe. It checks window dimensions and close events, renderer
initialization, full/reduced/invalid visual-quality selection, and rejection
from a non-owner thread. All three `linux_sdl2_*` tests passed, covering dummy
output, unavailable-driver failure, lifecycle focus pause/resume, output clock,
and close/reopen behavior.

The packaged Linux executable ran the production menu path for 12 frames and
exited with `menu_MainMenu`, `interactive:true`. The captured screenshot was
visually inspected at
`.local-inputs/linux-menu-smoke.png`; it shows the packaged swamp background,
character art, and Start Game/Options/Info UI. The SDL window had to receive
focus from the test window manager before the frontend's existing focus gate
was satisfied. This was a controlled Xvfb window, not the user's live desktop.

A separate run of the packaged ELF exercised the production `--start-mode swamp`
path in a private Ubuntu 22.04 chroot and Xvfb/Mesa llvmpipe display. It loaded
the Swamp level (24,738 triangles, 377 instances, 392 ranges), loaded 33
population visuals, rendered 30 frames, captured a 1280x800 frame, and exited
cleanly. The inspected capture is
`.local-inputs/linux-full-swamp-smoke.png`; it shows the Swamp scene, HUD,
player, priest, and environment. This verifies a rendered production Swamp
session on software rendering, not the full campaign/quest logic: the level
loader reports that module gameplay objects, quests, lights, skybox, and
campaign predicates are not instantiated.

The same run submitted the existing diagnostic walk input. The Knight began at
`(1090.75,-212.202,255)`; the first valid movement admission was
`(-0.270329,-1.32634,0)`, and after 30 frames the final actor position was
`(985.634,-198.633,255)`. This verifies the diagnostic movement path changed
the actor position in the rendered session. The configured attack window
produced no `Damage frame=` event, so combat damage is not verified by this
run. No native Fedora host or friend hardware was available.

The ELF is x86-64 and dynamically links SDL2/OpenGL and system audio/window
libraries. `readelf` reports maximum required symbol versions `GLIBC_2.35`
and `GLIBCXX_3.4.30`; Fedora's exact version is unknown. The archive requires
system SDL2/OpenGL drivers and audio libraries and bundles no system DLLs or
drivers. No native Fedora machine, friend's hardware, physical GPU, or audible
hardware path was available for verification.

This report records Linux backend evidence, not release acceptance or a claim
that all current gameplay gates are resolved. Root owns acceptance of the
candidate alongside the Windows release and remaining gameplay regression
gates.
