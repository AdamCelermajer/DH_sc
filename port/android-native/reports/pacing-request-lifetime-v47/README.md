# V47 one queued request versus one in-flight callback

This is a staged lifecycle correction to the explicit V44 experiment. Apply
`integration.patch` surgically to the existing controller/report helper. There
are no MainActivity, native, EGL, clock, quality, loading or default-mode changes.
Root's short same-APK comparison (54.22 continuous /50.76 vsync FPS at reported
60 Hz) showed no improvement and was not a controlled sustained A/B. This packet
is not an FPS fix or a default-mode promotion.

V44 held `pending` through the entire `onDrawFrame` callback. Thus a main vsync
during native/Java callback work could not queue a successor even though
GLSurfaceView can hold one request behind an in-flight callback. This is a
concrete scheduling mechanism, not attribution of that measured difference.

The corrected owner distinguishes the queued request from `activeFrame`:

- Renderer entry consumes the queued request and creates its unique in-flight
  callback ticket. A subsequent vsync may queue exactly one successor.
- Further vsyncs coalesce while that successor is queued, even if the current
  callback is slow. No backlog or catch-up burst is created.
- Completing the current callback retires only its matching ticket. It never
  clears a newer queued request. Stale epoch/context completions remain rejected.
- Pause/window/surface/context/close invalidate both ownerships. Existing actual
  framework first/resize draws remain legitimate and do not skip native work.

The report adds `renderer_callback_in_flight` and explicitly defines pending as
a queued request. It still measures neither EGL swap completion, GPU execution
nor presentation. Existing source wall dt/>2000 ms skip, main/UI scheduling and
GL/context ownership remain unchanged. The continuous default never constructs
the experimental controller.

136 actual portable-controller/export checks PASS: 43 new queued/in-flight and
slow/native-overlap/epoch/context/close/thread completion checks, 81 previous
cadence/lifecycle/first-load checks and 12 one-shot export checks. The prior
100,000 warmed callbacks still allocate0 bytes on JBR. Strict real API37 bridge
and full current app Java compile PASS; no SDK mirror, device, APK or actual EGL
operation was performed. Root must compare actual presentation and lifecycle
flow again before any performance acceptance or default change.
