# Bounded JobObject child-exit reconciliation

Root diagnosed its previous guard stop at13:28:39: ProcessGone38784 during the
second/final membership metrics pass aborted collect_snapshot before owner
telemetry was published. Previous complete telemetry showed a living owner and
job/headroom counters below limits. The incomplete snapshot then also reported
launcher_missing, a misleading secondary diagnosis. This report fixes that
source race; it does not retrospectively infer current process state.

The only production edit is collect_snapshot in `tools/emulator_watchdog_v36.py`.
Root owns the current already authorized guard; no launch/restart/device actions
were performed. Python loads this code when the guard starts, so the correction
is for the next root-authorized launch, not a hot patch to a running process.

## Exact acceptance rules

1. Read/publish actual owner metrics first within selected-process sampling.
   Later child metric errors preserve that observation. A genuinely exited owner
   still produces launcher_missing; no synthetic alive owner is created.
2. Refresh actual named-job membership. Sample every current member, then query
   membership again. At most3 sampling passes and5 logical membership queries
   occur per collection. QueryInformationJobObject's existing internal bounded
   buffer-capacity retry remains unchanged.
3. ProcessGone is the only tolerated child-sampling exception, and only when a
   fresh membership query excludes that PID and a complete stable member pass
   succeeds. A repeatedly listed gone PID remains incomplete and terminates.
4. A PID returning live after ProcessGone or changing creation FILETIME fails
   closed. Access errors and unavailable live metrics are never reclassified as
   process exits. Unstable membership exhausts the bound and fails closed.
5. Final job metrics match the actual final list. Newly joined measured QEMU
   members are included in aggregate counters even if absent from initial global
   enumeration. Proven gone processes are omitted; unrelated QEMU remains
   observed. Creation ordering still protects the inferred launcher tree.

The change does not alter Limits, evaluate, manifest bytes/identity/namespace,
timeout, headroom, explicit physical-pressure policy, bootstrap verification,
ready handshake, telemetry bounds or verified named-job-only termination. It
does not weaken hard Windows JobObject process-tree enforcement.

## Evidence

`race-test-receipt.json`:13 focused fake-API tests PASS, including the exact new
final child exit, initially exited child, delayed stale membership, PID reuse,
access/live errors, bounded churn, newly joined process metrics, unchanged job
and aggregate limits and changed manifest rejection.

The existing watchdog suite also passes16 tests, covering readiness/clean
completion, only verified-job termination, unavailable telemetry, unverified
target refusal, private commit versus working set, thresholds, owner death/reuse,
timeout, explicit paging policy, manifest validation and bounded telemetry.
`existing-test-receipt.json` records the identical frozen source hash.

All tests used injected APIs and bounded temporary files. No WindowsAPI was
created and no actual job/process/emulator/ADB/device was touched. Live acceptance
remains for a subsequent root-authorized guard launch; reproducing the Windows
child-exit timing is not required or claimed as part of fixture evidence.

```text
python port/android-native/tools/tests/test_watchdog_child_exit_v42.py
python port/android-native/tools/tests/test_emulator_watchdog_v36.py
```
