# Physical startup audit and bounded stage telemetry V41

The Xiaomi Android16 capture proves that the Crypt and authored HUD render after
parent's cold-start ordering fix. It also contains long initial HWUI frames.
This work traces that remaining startup path and prepares measurement hooks; it
does **not** claim responsive loading or a displayed-FPS improvement.

## Physical evidence and source path

`audit.json` retains the physical log hash, source hashes/line anchors and the
numeric HWUI fields. The two captured frames report933ms and944ms. The first has
891.305ms from PerformTraversalsStart to DrawStart; the second has923.869ms from
IntendedVsync to Vsync. Their HWUI IssueDrawCommandsStart-to-FrameCompleted spans
are3.865ms and3.052ms. These belong to the Android window/HWUI timeline, **not a
measurement of the separate game GL workload**. The log also reports111 skipped
frames. Brief logcat lacks per-line timestamps and TIDs, so no individual world,
bitmap or HUD function duration can be assigned from this capture.

Current MainActivity calls loadSelected synchronously on the GLSurfaceView GL
thread inside onSurfaceChanged, after the now-valid resize. That method performs
asset/provenance InputStream reads, ByteArrayOutputStream buffering and toByteArray
copies, then JNI copies the descriptor into a native vector and calls the complete
world loader. A front-menu launch likewise consumes its actual pending request
inside NativeBridge.draw and performs full world loading before that draw callback
returns. Subsequent first gameplay drawing lazily calls OriginalUiSession.load
through prepare_player_frame: cache verification, constants, localization, native
SWF/font services, actual shared/HUD movie parsing/advance and bitmap/GPU resources
occur synchronously before first HUD submission. Front UI has its own analogous
source loader/GPU owner and must be measured independently.

The existing SDK37 GLSurfaceView source corroborates surfaceChanged forwarding
to onWindowResize and a main-thread wait for render completion while the GL thread
calls renderer.onSurfaceChanged and onDrawFrame. The physical log contains the
same !readyToDraw/waiting-for-draw warning. This supports a resize-handshake stall
mechanism, but SDK37 is not the exact vendor Android16 framework. The amount
attributable to each app stage remains unmeasured and is the purpose of V41.

Two shader GPU probe/world-ready sequences, with the second retaining11NPCs,
also show graphics/Activity recreation work in the capture. The earlier valid-
surface fix prevents constructing the camera with the wrong dimensions; it does
not eliminate the resource reconstruction or first-HUD cost. Native warm CPU120
phase samples are elapsed native callback timing, not display cadence/FPS acceptance.

## Prepared measurement system

New owned files provide34 typed stages, a portable native recorder, its independent
JNI adapter and an unreferenced Java companion. Recording is explicit per actual
request/context generation, with native TIDs, input/output byte counts, failures,
nested inclusive/exclusive wall durations and first/last timestamps. It uses64
fixed active slots,128 history slots (first32 plus rolling last96) and fixed stage
totals. Totals account for all completed events even when history rolls over;
drop/invalid/open/nesting/overflow conditions remain visible. Stale tickets cannot
publish into a replacement generation. There are no Level, Save, GS, source-menu
state changes, progress percentages, fake readiness or implicit logging.

The Java companion performs no library loading or scheduling. Caller must load
the existing native library, begin a real request, explicitly mark successful
Java outputs, and finish/cancel capture. C++ RAII notices exception unwinding;
nonthrowing false/error returns must explicitly fail their scope. The output is
bounded JSON returned only on request; its serialization should occur outside
the first critical GL frame. Java origin time is retained separately instead of
assuming its clock aligns with native steady_clock. Actual presentation/swap is
outside these app scope callbacks. No live hook or measurement was installed by
this agent.

`ROOT-INTEGRATION.md` gives exact source-stage hook locations and a concrete
extraction contract: immutable source/decoded-byte preparation with owned APK/
AssetManager lifetime, resource accounting and cancellation identity; bounded
GL commits on the actual context thread; genuine original Lua/native/UI ownership
and publication retained. Moving the whole current load_world to a worker is
unsafe because it mixes CPU decoding, GL operations and live source graph mutation.
Queueing the same monolith after resize alone is not proof of a visible responsive
loading frame. The first coherent extraction should be bounded raw-data prefetch
and independently measured GL commits.

The actual menu_Loading lifecycle producer still belongs to the loader contract:
the live demo retains real C1 with phase130=0 and does not publish a current GS.
Whole Level.Init/LoadProcess/Unload/destruction and reached providers remain
required. V41 stage totals must never substitute for those original fields.

## Verification and remaining acceptance

- Strict `-Wall -Wextra -Werror` **four native compiles passed**, ARM64 and
  x86_64 core/JNI, with no warning suppression (`compile.json`).
- Java companion `javac -Xlint:all -Werror` passed. Android app/DEX integration
  remains parent-owned.
- **10,148 UBSan host assertions passed** under128MiB/5CPU-second runtime bounds:
  nested wall/bytes/parent attribution,10,000 completed events with bounded
  history,64active-slot cap, stale/cancelled/replaced generations, invalid/time
  reversal/nesting, exception and explicit failures, saturation and2,000
  concurrent thread events. JSON parsed successfully (`host-receipt.json`).
- Physical HWUI/source audit is accepted as the captured evidence and path,
  with the timing/SDK caveats above. **Deployed stage timings, first meaningful
  presentation, loading responsiveness and displayed FPS are pending.**

No shared startup/CMake/renderer/UI file edits, device/ADB operations, graphics
fixture, emulator or downloads were performed. Parent owns phone reconnection,
hook integration and the next real request/context-stage measurement.
