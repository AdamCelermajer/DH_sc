# Retained native input/frame session V1

This additive application adapter composes the frozen source input/frame batch with the real `SwfMovie` facade. It is outside the tested Android checkpoint. It neither substitutes stock mouse advance nor attributes original Application/Android input policy to the adapter.

## Ownership and startup

`SwfInputSessionV1` owns a generation containing Provider, Movie, then InputConnectionV2. Reverse destruction releases input strong slots and its exact graph lease before Movie teardown; Provider survives both. Impl retains Provider, which owns history/frame observers and only a weak generation. Caller backend owners must remain independent of the session and strong graph values. An outer caller can retain a graph value across reload; it is rejected by the fresh graph and delays destruction of its original graph safely.

The detached candidate installs typed native callbacks through the unchanged facade, then `graph_start` binds `SwfInputHistory` and `SwfFrameConnection` before any shared/root `load_file` or character construction. The original caller graph hook runs afterward. It must not install a competing history/frame receiver. The original read/texture/image/draw/native/stencil/glyph providers and their owner are forwarded without synthetic success. Shared files retain the caller's supplied order.

After loading, one real ActionScript Scope obtains a pinned root value, resolves the supplied context path, and binds source input against that exact root/player graph. The source camera update runs on an owned copy of the caller's initial seed; `camera_state()` exposes its resulting writes. Later `camera_update()` uses the same owned camera storage so its prefix is observable to synchronous reentry, copies results back even on failure, and updates the same viewport used by input and display.

Failed load/read/startup/context/seed/provider delivery preserves the old generation. Explicit outer release during candidate startup cancels publication. Releasing or destroying the outer session during a native event does not close the executing graph: the batch pins its control, generation, facade Scope and source receiver through the complete delivered prefix. Supported in-scope input reentry operates on that generation directly; it does not open a forbidden second facade Scope. Reload attempted from an executing Scope still fails the facade's busy guard.

## Source pipeline and viewport

The adapter calls the unchanged source kernels recovered at RenderFX `UpdateCursor 0x7ac924`, `UpdateInput 0x7ac4bc`, SetFocus `0x7ac228`, ResetFocus `0x7ac410`, and Update `0x7ad68c`. Update takes signed milliseconds once, performs the source float conversion/division, invokes original root/sprite scheduling, then the all-four pending-event tail. It receives an explicit caller advance flag. The captured MenuFX Update `0x7ad88c` forwards that bool; the original Android/Application producer remains unrecovered.

V2 is byte-identical to V1 after name/include normalization before its four appended viewport methods. Its sole addition exposes camera update, viewport snapshot, screen mapping and display rectangle on the existing private viewport. No slot reset, focus rebind or drag reconstruction occurs on resize. Display submits that exact source rectangle in the same retained Scope using the installed real render handler, root background and requested actual clip. It does not create an independent facade viewport or aspect-fit projection.

The caller supplies genuine driver dimensions/orientation, authored rectangle/initial bounds, camera fields, source context, flags, selection, native receiver and event policy. Android ACTION mapping, pointer-ID-to-four-slot routing, button/mask meaning, touch consumption, application pause clock and advance flag are explicit application integration responsibilities. Config's zero-initialized C++ fields are not evidence of original values. Frame advance is supplied by the owned source `SwfFrameConnection`; a caller-provided substitute advance or 3D scene-mouse callback rejects this 2D session at load.

Current inspected app wiring has a status-only `OriginalUiSession`: it advances zero at load and updates status/display, with no source elapsed-frame or touch entry. MainActivity's surface listener consumes HUD/world motion; other assets use orbit. The coherent next connection must route actual GL-thread pointer policy to this outer session, call Update with an explicit app clock/advance policy, and bind real HUD/native receiver services. The fixture's permissive acceptance is labeled and must never become an application default. Full AS VM, audiovisual, GPU, 3D mouse and original Android policy parity are not claimed.

## Last-player safety overlay

An actual sanitizer destruction/recreate test exposed upstream `gameswf_player.cpp` lines94–101: `clear_standard_method_map()` deletes each process-global builtin map without clearing the pointer. On the next player constructor `action_init()` dereferences those freed maps. The failing ASan log is `.local-inputs/swf-input-session-v1-final.log`.

NEW `gameswf_player_lifetime_overlay_v1.cmake` replaces only the player TU with `overlays/player-lifetime-v1/gameswf_player.cpp`, adding `s_standard_method_map[i] = NULL` immediately after delete. The vendor is untouched. This is an explicitly modern lifecycle safety correction, not original ARM32 destructor parity. The final sanitizer test destroys the last player, constructs another, then destroys the outer session synchronously inside a real native event; no hidden persistent player anchor keeps the maps alive.

## Integration and reproduction

Production additions: `swf_input_connection_v2.cpp`, `swf_input_session_v1.cpp`, plus the frozen source input/frame set: `swf_cursor_input.cpp`, `swf_input_geometry.cpp`, `swf_input_policy.cpp`, `swf_input_history.cpp`, `swf_input_connection.cpp` (legacy ABI/tests), `swf_event_dispatch.cpp`, `swf_event_core.cpp`, `swf_frame_schedule.cpp`, `swf_frame_connection.cpp`, `swf_drag_values.cpp`. Existing facade, ActionScript, viewport and HUD-timeline dependencies remain linked once.

After `gameswf_sources.cmake`, opt into `gameswf_input_overlay_v1.cmake`, `gameswf_frame_overlay_v1.cmake` and `gameswf_player_lifetime_overlay_v1.cmake`. Font and text overlays replace separate TUs and compose without changing these three recipes. Preserve the pinned vendor headers and identical `TU_CONFIG_LINK_TO_JPEGLIB/LIBPNG/FREETYPE/THREAD=0` definitions for core and clients. Exactly one implementation of each core TU and one set of GameSWF globals must be linked. Every player/movie using the opted-in core must bind observers before construction; a plain unobserved legacy `SwfMovie` is an explicit failure, not a stock fallback. Central facade/default-graph migration remains parent-owned.

The isolated host target is `session_audit`, linked to actual `libsession_core.so`. The runner also executes input/frame core regressions and all 10,800 original-derived gold cases:

```sh
python3 port/engine-ui/tools/swf_input_session_host_v1.py \
  --build-directory .local-inputs/swf-input-session-v1-host \
  --report port/engine-ui/reports/NEW-session-proof.json
```

On Windows invoke through WSL from the repository. The runner refuses report overwrite and verifies compiler/source inputs did not change during its run. ASan/UBSan/LSan are enabled; the upstream disabled-JPEG RTTI boundary retains `-fno-sanitize=vptr`. The one expected source stderr line for deliberately missing fixture resource is checked exactly; all sanitizer/other stderr rejects. The actual session fixture reads real SWF bytes through the facade, runs shared frame ActionScript/native callbacks, constructs real core shapes/sprites, and reaches source setters/hit testing/drag/actions/native events. Its SWF/resources/acceptance/draw sink are controlled host fixtures, not an authored full-HUD or GPU acceptance test.

Final host proof: `reports/swf-input-session-v1-host-audit-final.json`. Android receipt: `reports/swf-input-session-v1-android-compile.json` records exact NDK arguments/compiler/source/object hashes for ARM64 and x86_64 O2 compilation of session, V2 connection and lifetime TU. This is compilation only; no Android link, APK, ADB or GPU operation was performed. `connection-baseline.json` binds V2 baseline equality, unchanged frozen V1 sources and the one-line vendor-overlay correction. The freeze manifest binds these artifacts separately from the previous immutable batch.
