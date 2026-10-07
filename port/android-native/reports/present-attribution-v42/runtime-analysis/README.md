# Actual saved V42 trace attribution — incomplete kernel trace

**Observed:** the game GL callback is short, and complete surviving same-thread
EGL swap wall spans occupy the corresponding intercallback gaps. **Unresolved:**
the cause inside those swaps, actual GPU duration, host scheduling/page faults,
and representative whole-trace percentages. Kernel trace loss prevents full
causal accounting. No device or local trace-server calls were made here.

## Evidence identities

- Captured trace: `.local-inputs/present-attribution-v42/20261006-163251-bc0a7b7b/capture.pftrace`.
- Bytes7,959,444; SHA256 `9e58325cb4527ab8f7b50b6606f64d4c98d9119dd86d77401b9dd42bc07c7049`.
- Capture receipt: UTC13:32:51–13:33:12; stable PID3319 and start_ticks7050.
- Java boundary report: `reports/emulator-v41/frame-boundary-v42.json`;
 512 actual callback rows, GL TID3338. Root's separately generated
 `frame-attribution-v42.json` records APK hash and workload.

The independent Java timestamp dataset gives callback median2.370ms,
native2.289ms, Java post-native0.046ms, gap16.891ms and gap p9536.926ms.
Guest-thread CPU median2.323ms is a separate quantity. Its largest gap is1.016s;
that is wall time, not GPU computation. Root separately reports a subsequent
untraced stationary SurfaceFlinger sample:486 intervals/10.41s,46.68FPS,
p9537.68ms/p9944.4ms/max47.08ms and zero intervals above50ms. That later
sample is not this tracing population or proof of a specific optimization's
causal contribution.

Actual GL capability report: timer extension advertised **false** despite
entry points being present. EGL frame-timestamp extension/functions advertised,
but actual display-present support **false**. No GPU or EGL presentation samples
were collected. Function pointers alone do not authorize unsupported sampling.

## Trace loss is confirmed, not hypothetical

The bounded offline decoder found38 `lost_events` bundle flags. Final per-CPU
kernel overrun counters are98,952 (CPU0) and80,651 (CPU1), initially0. Kernel
loss occurred before events reached the8MiB consumer buffer; staying below the
file cap does not prevent per-CPU ring overruns. The last service stats observed
explicit invalid_packets0 and chunks_discarded0, but this decoder is not a full
trace-processor import and does not claim all service/import losses absent.

Loss ranges use the previous serialized per-CPU bundle timestamp and next
bundle's first retained event. Their conservative cross-CPU union excludes
9.007s. Pending synchronous stacks reset at each gap; events inside gaps and
scheduler spans crossing them are discarded. No duration is joined across those
gaps. Missing sources/events do not mean zero time.
[Bundle/loss schema](https://raw.githubusercontent.com/google/perfetto/main/protos/perfetto/trace/ftrace/ftrace_event_bundle.proto),
[kernel overrun statistics](https://raw.githubusercontent.com/google/perfetto/main/protos/perfetto/trace/ftrace/ftrace_stats.proto).

## Complete surviving observations

262 complete `DH2:onDrawFrame` spans independently match262 actual Java rows,
with0 mismatches under0.5ms start/end tolerances. All are on TID3338. The clock
snapshots show boot-minus-monotonic offsets between−223 and−52ns; observed
marker-call overhead is much larger and included in the matching tolerance.

251 corresponding complete, loss-free intercallback gaps contain exactly one
fully bounded same-TID `eglSwapBuffers` span. In **this surviving subset only**:

| Wall quantity | Median | p95 | Samples |
| --- | ---: | ---: | ---: |
| Intercallback gap |17.148ms |36.499ms |251 |
| Observed EGL swap span |16.928ms |36.403ms |251 |
| Gap outside that swap span |0.097ms |0.227ms |251 |

These are selected complete-span observations, **not** representative workload
percentiles, percentages or a GPU-bottleneck conclusion. The larger native512-row
timestamp dataset remains independent; do not substitute the partial trace
distribution for it.

The same GL thread includes actual `dequeueBuffer`, `queueBuffer`,
`waitForBufferRelease` and buffer-release callback spans. Loss-free complete
`waitForBufferRelease` spans exist (375 samples, observed subset median5.618ms).
They establish that real buffer/queue wait code is reached; they do not isolate
GPU execution from buffer pacing, compositor activity or guest/host scheduling.
Nested queue/release spans must not be added to their inclusive EGL swap parent.

Actual sched_switch/waking/wakeup events permit complete island running/off-CPU
observations. Initial/ending states and loss gaps leave unknown boundaries. Raw
switch-out states include0,1,2,128,256; this minimal decoder does not equate all
nonzero states to sleeping or make a whole-trace CPU allocation. Its JSON keeps
unresolved off-CPU spans separate. No blocked-reason source was requested, so a
blocked function or IO reason cannot be supplied. Guest process snapshots show
minor-fault delta22/major0 across the capture; CLK_TCK is unobserved, so CPU ticks
are not converted to seconds. These are guest counters, not host QEMU paging.
[Scheduling schema](https://raw.githubusercontent.com/google/perfetto/main/protos/perfetto/trace/ftrace/sched.proto).

## Tooling and reproducibility

The two installed Studio tools are gRPC services, not the conventional standalone
trace_processor_shell CLI. Root's --help run returned no matching flags.
Static scoped binary inspection found the service protocol, but the active Python
runtime has no grpc/protobuf modules. No binary was launched, no loopback/public
listener created and no dependency downloaded. The full SQL/FrameTimeline import
remains pending an available client. The fallback decodes bounded known official
wire fields directly; it rejects unsupported compression/non-boot ftrace clocks
and malformed data.8 small synthetic parser/loss/scheduler tests PASS. It is not
a substitute for full Perfetto import/error validation.
[Packet schema](https://raw.githubusercontent.com/google/perfetto/main/protos/perfetto/trace/trace_packet.proto).

```text
python port/android-native/tools/parse_saved_perfetto_v42.py TRACE.pftrace --tid 3338 --pid 3319 --output OUTPUT_DIRECTORY
python .local-inputs/summarize_saved_trace_v42.py
python port/android-native/tools/test_saved_perfetto_v42.py
```

The first invocation decodes only the existing file. `offline-ftrace.json` retains counters/loss diagnostics;
`offline-ftrace-detail.json` retains actual slices/events;
`callback-trace-correlation.json` records every accepted match/gap.

## Calibrated next capture — prepared, not run

Root may run `collect_present_trace_calibrated_v43.py --capture` once after its
fresh genuine main-menu→Crypt flow and original profile ownership are restored.
No-arg dry-run is verified. All collector daemon/device/PID/file/time guardrails
remain. Config1024KiB **per CPU** and100ms drain replaces256KiB/1000ms;8MiB
consumer ring/file and20s duration remain. With the observed2 CPUs, kernel
buffers add2MiB, separate from consumer/process memory. Root's existing hard
process-tree cap and headroom policy still apply. CPU frequency/sys_stats are
omitted from this focused scheduling/graphics calibration; genuine app markers,
gfx/view, scheduler, process metadata and FrameTimeline remain.

Success means a new receipt plus inspected zero/no-growing kernel losses and
accepted import stats, not merely a nonempty file. If the8MiB file cap closes
capture early, retain the actual interval and lost/open-slice diagnostics;
do not raise caps or infer20s coverage. Before optimizing, attribute actual
swap-internal waits using the usable trace and external host evidence. No GPU
finish, reduced effects, geometry shortcuts or framework mirror is proposed.
