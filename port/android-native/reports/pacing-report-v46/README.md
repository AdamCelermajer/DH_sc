# V46 one-shot pacing diagnostic export

Apply `integration.patch` surgically to root's current MainActivity; it adds one
`pacing` JSON field next to the preserved `request_id` in the existing protected
DEBUG_FRAME_REPORT GL-thread capture. New `FramePacingReportV46.java` already
exists. It copies one immutable V44 snapshot and converts it to plain data;
it schedules no frame, changes no clock/state, performs no IO and exports no FPS.
Default continuous mode explicitly reports that no scheduler snapshot exists.

The existing debug-build check, android.permission.DUMP receiver gate, current
world/readiness gate, bounded frame capture and report persistence are unchanged.
Therefore this does not add an initial-loading report endpoint or bypass those
guards. Root can include it in its next comparative build without another CMake,
native or pacing-lifecycle change.

12 pure export checks PASS; full staged Java app compiles against the real API
37 android.jar, and git apply --check passes. No SDK classes were mirrored and
no APK/device/GL execution was performed. The data identifies pending/cadence
skips, stale lifecycle deliveries, actual renderer callbacks and update requests.
Scheduler request counts are explicitly not presentation/FPS measurements.

Use existing `boundary.rows` + `guestThread` for callback/native CPU and actual
external presentation/Perfetto trace for main-vsync/compositor/swap attribution.
This small export has no per-vsync timestamp series and cannot establish a GPU
cause, loading-tail improvement or presented cadence on its own.
