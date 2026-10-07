# V45 fully sampled final job membership

Root requested this protective watchdog correction after actual guarded emulator
runs ended on `Named-job membership/metrics did not stabilize within three passes`.
The owned live `emulator_watchdog_v36.py` is updated; launcher, JobObject APIs,
memory thresholds, timeout policy, admission mutex and process cleanup scope are
unchanged. No emulator, ADB, native process job or device operation was executed
by this component. All tests inject an API, never construct WindowsAPI.

## Actual captured evidence and its limits

`captured-receipt-analysis.json` binds the two original receipts by SHA256.
170248 ended at 2411.734 seconds with metrics for all four final member PIDs;
175037 ended at 265.516 seconds with final PID 34312 lacking a metric. Both
decisions were monitoring-unavailable, not memory-limit decisions. Both launcher
FILETIME identities match their manifests. Earlier receipt code did not record
per-pass queried/sampled/refreshed sets, so the exact historical churn sequence
cannot be reconstructed. The equality rejection is demonstrated by independent
fixtures; it is not presented as a complete replay of those two runs.

In particular, 175037's recorded missing final member must **still fail closed**.
This change does not reinterpret or promote that incomplete snapshot as safe.

## Acceptance invariant

For each of at most three passes, sample the queried actual job members, then
refresh actual named-job membership. Accept iff **every refreshed final PID** has
fresh, live, matching-PID/positive-FILETIME metrics from that same pass. Preserve
the existing creation-time comparison with earlier reads and ProcessGone history.
Set equality is unnecessary: a member sampled before it exited may disappear
from the final set without invalidating all surviving members' measurements.

A newly added final PID missing from that current pass requires another sample
pass, even if an older census/pass has a metric for its integer PID. Publication
uses current-pass records only. ProcessGone still listed at the final query,
missing live metrics, access errors, a PID returning live after ProcessGone,
creation-time reuse, invalid live identity and exhausted three-pass rapid churn
still mark the snapshot incomplete; evaluate still terminates only the previously
verified, pinned owned JobObject. No PID-based cleanup, ADB kill, unrelated QEMU
termination, watchdog disable or increased budget was introduced.

Confirmed departing job members do not contribute stale records to current QEMU
aggregate/launcher-tree telemetry; unrelated and newly measured QEMU still count
toward the aggregate private-commit limit. Historical ProcessGone-specific fields
remain distinct from the new `job_reconciled_departed_pids` membership field.

## Bounded diagnosis and fixtures

`job_membership_passes` contains at most three diagnostics. Each PID/identity list
is limited to 64 entries with exact counts, while actual accounting continues to
include every admitted member. Diagnostics show queried members, current sampled
FILETIME identities, ProcessGone results, refreshed members, departed/new/missing
PIDs, acceptance and the PID/identity or access error that stopped sampling.
The existing bounded JSONL rotation remains unchanged.

41 tests pass: 12 new departure/arrival/rapid-churn/current-metric/identity/diagnostic
fixtures; 13 existing child-exit/reuse/access/aggregate race fixtures; 16 existing
threshold, owner, manifest, startup-ready, verified-only-job termination and
telemetry protocol fixtures. The two older race expectations were updated to
require first-pass acceptance of a safe subset and continued fail-closed behavior
for genuine continuously arriving unsampled members. One 80-member fixture proves
diagnostic truncation does not truncate accounting. No large allocations or
stress run were performed.

The ZIP/manifest freezes exact launcher-ready Python source and receipts. Root
must perform the next authorized guarded launch to verify real process churn and
then validate its target APK. Fixtures do not establish sustained host stability,
FPS, loading completion or successful live watchdog operation after this change.
