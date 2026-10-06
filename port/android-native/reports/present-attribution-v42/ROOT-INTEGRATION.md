# Root-owned minimal integration

Owned frozen source files:

- `app/src/main/java/com/example/dh2/FrameBoundaryV42.java`
- `app/src/main/cpp/present_capabilities_v42.cpp`

Root adds only the native companion to existing `dh2_native`, retaining the
existing EGL/GLES linkage and loaded-library ownership. Configure the Java
helper once on the actual GL thread outside a callback, gated by the app's
debuggable flag and explicit `frame_trace` intent. No NativeBridge API change
or framework copy is required.

```java
FrameBoundaryV42.configure(debuggable && frameTraceRequested);
// Around the complete existing onDrawFrame, retaining early-return branches:
long ticket = FrameBoundaryV42.beginFrame();
try {
    // Existing pre-native work, including genuine pending-load branch.
    FrameBoundaryV42.beginNative(ticket);
    try {
        NativeBridge.draw();
    } finally {
        FrameBoundaryV42.endNative(ticket);
    }
    // Existing unchanged audio/Java/UI tail stays here.
} finally {
    FrameBoundaryV42.endFrame(ticket);
}
```

Root reports integration of this wrapper plus a protected debug report receiver;
that shared edit is outside this handoff's source/compile acceptance. Capture
must run against the root's coherent rebuilt APK and actual foreground Crypt.

After capture, queue to the existing GL owner: call finish, capability discovery
and the guest-thread counter sample, then report. Persist returned immutable
Strings on a separate IO thread, bounded to the existing app-private debug
file. No perframe Log or report allocation is required. A single after-capture
thread-counter snapshot has no delta by itself; use before/after same-TID
snapshots if measuring changes, or rely on captured scheduler intervals.

Early callbacks that return before NativeBridge.draw may have zero native
boundaries; the parser deliberately rejects them instead of claiming valid
render frames. Preserve raw reports to diagnose them. The helper is diagnostics
only: a completed callback/report is not source Level.Init or meaningful pixels.

Use `analyze_frame_boundary_v42.py` for the saved boundary JSON. Use the SQL
separately against a genuine captured trace, first checking stats/loss and exact
TID/layer names. Optional FrameTimeline query requires its actual table/source.
The SQL was source-reviewed but has not executed against an actual trace here.

Optional V41 startup stage scopes retain its genuine generation and exact
current phase outcome. A cold startup callback belongs to the startup report,
while the normal workload trace measures stable rendering. Do not merge those
populations or turn phase counts into original loading percentages.
