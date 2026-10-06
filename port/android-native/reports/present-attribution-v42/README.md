# V42 frame/present attribution: source frozen, runtime capture pending

The prepared system measures the existing Java renderer callback and native
call, captures Android scheduling/graphics evidence, and keeps unknown swap,
GPU and presentation timing unknown. It does not replace GLSurfaceView, add a
render loop, schedule frames, synchronize the GPU or change original callbacks,
effects, geometry, source progress or resources.

Accepted source evidence in `compile-test-receipt.json`: strict arm64-v8a and
x86_64 compilation of the new native companion, Java11 `-Xlint:all -Werror`,
and10 host collector/parser acceptance tests. No device calls, real GPU timing,
pixels, presentation samples, FPS improvement or responsive-loading proof were
produced by this handoff. Root owns shared integration and runtime acceptance.

## Run only under root's already accepted emulator safety policy

```text
python port/android-native/tools/collect_present_trace_v42.py
python port/android-native/tools/collect_present_trace_v42.py --capture
python port/android-native/tools/analyze_frame_boundary_v42.py FRAME_REPORT.json --output ATTRIBUTION.json
```

The first command is a no-device dry run. Capture accepts only emulator-5554,
checks an existing5037 daemon, existing emulator and single game process, and
never starts/resets/installs anything. It records process identity before/after;
changed/disappeared PID or start time invalidates attribution. Failure creates
its unique receipt and does not retry. It does not launch the app or recover a
black scene: root first restores a stable, stationary Crypt workload and uses
the explicit `frame_trace` debug intent when launching the coherent candidate.

Unique output goes to `.local-inputs/present-attribution-v42/TIMESTAMP-ID/`:
`capture.pftrace` and `receipt.json`. Status CAPTURED_NOT_ANALYZED means bytes
were captured, not that timing or source availability has been accepted.
Unsupported sources, trace loss and incomplete slices must be examined first.
No automatic remote-file removal or global tracing-service kill is issued.

The config limits its ring/file to8MiB and requests20s capture. The ftrace
256KiB buffer is **per CPU**, additional to that ring;8MiB is not a process-tree
memory cap. Duration excludes suspend; the collector's35s timeout limits its
local client, while service-side duration/file limits remain in force. Root's
emulator job/watchdog/headroom policy is a separate prerequisite.
[Trace configuration](https://perfetto.dev/docs/reference/trace-config-proto).

Perfetto accepts the prepared text configuration through stdin. The gfx/view,
scheduler/process/memory sources provide observations without an app framework
mirror. FrameTimeline is an optional Android12+ source; distinguish the game's
SurfaceView layer from Activity/HWUI layers before using its events.
[Capture CLI](https://perfetto.dev/docs/reference/perfetto-cli),
[FrameTimeline](https://perfetto.dev/docs/data-sources/frametimeline).

## Attribution contract

`FrameBoundaryV42` records at most512 callbacks or20s after the first accepted
trace-enabled callback. Explicit configuration and `Trace.isEnabled()` are
required. API26–28 stay disabled because that public query requires API29.
The callback, NativeBridge.draw, and unchanged Java post-native work have
separate nested spans. The fixed arrays do not allocate per frame; reporting
creates JSON once after finish on the owning GL TID, outside the draw callback.
Transfer that immutable String for persistence on another thread.

The host parser validates row/order/count invariants and reports wall medians,
p95/p99/max for callback, native, pre/post-native, callback cadence and the
unattributed intercallback gap. Guest thread CPU is separate; unavailable raw
CPU values are excluded. It explicitly returns null for actual EGL swap, GPU
duration, presented FPS and host QEMU paging. Cadence is not presented FPS.

Open `capture.pftrace` in an existing Perfetto UI or trace processor. Run the
queries in `attribution.sql`, inspecting actual source/table availability:

| Evidence | Interpretation |
| --- | --- |
| DH2 spans on the helper's actual GL TID | Native and Java callback wall boundaries |
| Actual EGL/dequeue/queue slices on that TID | Only the specific observed driver/framework spans |
| Running versus Runnable/preempted intervals | Guest CPU execution versus guest scheduling delay |
| Sleeping/blocked intervals and available blocked function | Wait evidence; not automatically GPU work |
| Actual SurfaceView FrameTimeline | Presentation/jank evidence for that named layer |
| Guest process faults / vmstat / thread counter deltas | Guest evidence; not host QEMU memory/paging |
| Source mask/primitive/resource counters | Source workload proxies; not raster duration or overdraw measurement |

Absent swap/driver slices are unavailable evidence, not zero cost. The stock
renderer callback ends before framework swap. Correlate real same-TID trace
events before attributing its gap to swap, scheduler, driver or VM waits.
Guest scheduling also does not establish host QEMU scheduling: root may align
its separately owned Windows job metrics with external time anchors, retaining
clock uncertainty. Do not sum nested inclusive spans into total latency.

## Optional capability discovery is not a GPU sampler

`present_capabilities_v42.cpp` discovers only extensions actually advertised by
the current EGL/GL context, checks corresponding entry points and records
elapsed-query bits/disjoint state and present-timestamp support. It records
`gpu_or_present_samples_collected:false`. It allocates no timer query and calls
no glFinish/glFlush/fence. It does not consume glGetError/eglGetError. A separate
explicit current-context discovery call is required after context recreation.

If evidence warrants a future GPU query sampler, use a bounded context-owned
pool, never reuse pending queries, poll availability without blocking, reject
disjoint results and abandon names on context loss. No such sampler is deployed
here. Real EGL frame-ID sampling around swap needs an actual supported boundary;
public GLSurfaceView.Renderer hooks do not provide that boundary. Trace first.

## Startup extraction boundary

See `STARTUP-EXTRACTION.md` and the V41 `ROOT-INTEGRATION.md`. V42 capture does
not make loading staged or responsive. Keep real source loading/Save/C1/module
ownership and GL affinity until an independently measured extraction is ready.
