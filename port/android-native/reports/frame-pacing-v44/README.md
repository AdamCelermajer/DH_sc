# V44 explicit debug-only vsync60 experiment

This is a **staged experiment**, not an accepted FPS fix or a presentation
backend. Apply the surgical `integration.patch` to MainActivity. New controller
and view sources already exist in the shared checkout. There are no native,
CMake, game-clock, asset-loading, shader, resolution or EGL changes.

Default `RENDERMODE_CONTINUOUSLY` stays active unless this exact activity launch
has `frame_pacing=vsync60` **and** ApplicationInfo.FLAG_DEBUGGABLE is set. Unknown
values and non-debug apps retain the continuous default. Runtime intent changes
do not switch modes; relaunch explicitly to compare. New GLSurfaceView subclass
delegates to its normal implementation when the experiment is disabled.

## How it works

Android's [Choreographer](https://developer.android.com/reference/android/view/Choreographer)
posts callbacks on the UI looper. The bridge uses actual callback timestamps;
it does not assume every display callback represents a 60 Hz tick. The scheduler
keeps an absolute 16,666,666 ns phase, skips missed deadlines arithmetically,
and issues at most one pending request. On 120 Hz this normally selects alternate
vsyncs; 90/144 Hz require nonuniform tick patterns for a roughly 60 Hz average.
On a physical display below 60 Hz, 60 presented FPS is impossible; metadata logs
identify that case or unavailable display metadata. The cadence follows actual
timestamps across display changes without assuming the reported rate is exact.

One callback object per lifecycle epoch is reused for all vsyncs. Warm controller
paths allocate nothing in host evidence. The Android bridge similarly reuses its
callback wrapper. This does not claim Android/Choreographer itself allocates zero
bytes. There is no polling timer, loop catching up missed frames, per-vsync log,
native advance, presentation counter, eglSwapInterval(0) or glFinish.

[GLSurfaceView](https://developer.android.com/reference/android/opengl/GLSurfaceView)
continues to own its existing GL thread, surface, context and EGL swap. Native
draw still executes once for each actual onDrawFrame callback. Renderer completion
acknowledges that callback, **not** swap, GPU completion or display presentation.
An EGL-blocked GL thread can still have one request waiting. Framework-mandated
initial/resize frames are allowed to execute even if the scheduler did not request
them; they are identified separately in explicit diagnostic snapshots.

## Lifecycle and UI guarantees

Scheduling requires resume + attachment + visible window + focus + real surface
dimensions + renderer context readiness. Pause, focus loss, hiding, detachment,
surface destruction and close remove posted callbacks and retire their epochs.
Context recreation invalidates pending acknowledgement identities on the GL
thread, then posts one coalesced main-thread reconciliation task. A stale callback
cannot remove/rearm a newer one, and an old GL completion cannot clear a newly
issued request. GLSurfaceView.onPause still owns stopping the GL thread; an
already submitted request cannot be retracted with its public API. This helper
does not skip NativeBridge.draw to imitate cancellation.

All existing explicit requestRender sites in MainActivity use a coalescing update
hook in the experiment, and their original requestRender call in default mode.
UI/queued GL commands remain on their original threads and receive a subsequent
target-cadence draw while active. Loading remains in the real surface-changed
callback after dimensions are known, so first loading does not depend on a
Choreographer draw to run. Renderer and source wall clocks are untouched. The
actual model renderer still uses unsigned milliseconds from steady_clock, updates
last_frame from real time and performs its original >2000 ms Application skip;
no Choreographer timestamp is passed as dt, capped or substituted.

Request-scheduler failure stops this explicit experiment and logs DH2Pacing;
relaunch without the opt-in to restore the default. API 24+ is already the app's
minimum. No SDK classes were mirrored in tests: host tests run the real portable
controller through its injected driver, and the thin Android bridge/full staged
app compile against the installed real API 37 android.jar.

## Component evidence

`java-receipt.json`: 81 checks, cadence at 30/60/90/120/144/240 Hz and irregular
timestamps, main/GL ownership, pending backpressure, missed deadlines, stale
epochs/completions/timestamps, pause/resume, window focus/visibility, attachment,
surface destruction/recreation, renderer readiness, close and scheduler failure.
100,000 warmed actual-controller callbacks allocated **0 bytes** on the JBR host.
Controller/test and controller/Android bridge each pass -Xlint:all -Werror;
full app Java plus the staged activity compiles with the project's existing
legacy deprecations. `native-wall-clock-contract.json` captures the unchanged
production timing block and exact one NativeBridge.draw call. `patch-check.json`
proves surgical applicability; all component receipts bind exact source hashes.

These are scheduler/lifecycle/component checks, not Android runtime proof,
loading-speed proof or an FPS improvement. No emulator, device, APK install,
EGL/driver execution or root-owned performance capture was performed here.

## Parent comparison and acceptance

1. Build one candidate containing this patch and the same accepted native source.
   Keep actual source/APK/device/display/refresh/guard identity and settings fixed.
   Run default continuous mode first, with the existing bounded frame_trace
   diagnostics, warm identical scene and established external presentation trace.
2. Relaunch the same candidate with `--es frame_pacing vsync60`. Example launch
   identity is `com.example.dh2/.MainActivity`; optional Crypt selection is
   `--es world crypt01.dwld`. Capture the same scene/warm interval/input sequence.
   Root uses its existing guarded device/emulator tools; this handoff launches
   nothing and supplies no bypass of the OS/memory guard.
3. Compare actual presentation cadence/tails and trace-consistent callback/native
   CPU/swap intervals, not request counts. Check 60 and a real higher-refresh mode;
   90/144 Hz patterns cannot be perfectly uniform without choosing a different
   display mode, which this experiment does not change. Report refresh metadata,
   long main-thread delays and compositor/virtual-display restrictions explicitly.
4. Exercise main menu -> game -> profile -> items/skills -> game, initial load,
   resize/orientation, focus loss/return, Home/resume, >2-second pause, genuine
   context recreation and close. Compare native timing/AI/script/HUD/cast outcomes
   with the same source clocks. Repeat lethal skill tests only after root's Kill
   provider bug is fixed; this scheduler is not a workaround for that failure.
5. Accept only if actual cadence/stability improves and loading/input/lifecycle
   regressions are absent. Keep default continuous unchanged until that decision.
   A callback that returns quickly plus a long swap is insufficient evidence of
   a GPU cause, and this component makes no such claim.
