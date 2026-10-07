# V48 bounded Windows job-exit notification drain

Root's actual 180939 receipt now proves PID30940 returned ProcessGone and
remained in all three immediate Job PID queries, while the four stable members
had fresh metrics. V45's subset correction was insufficient for this distinct
kernel-notification lag. Listed missing members must not be silently dropped.

The owned live watchdog now yields **only** when every unmeasured final member
has actually returned ProcessGone. It retains the same three sampling passes
and five membership-query bound. At most two 75 ms delays are requested, with
a 150 ms monotonic planned-wait budget. A delayed OS wake is recorded and never
earns more wait time; it cannot be guaranteed that an overloaded scheduler wakes
within the requested duration. No extra retry occurs after pass three.

After each yield the same actual metric sampling and authoritative refreshed
membership proof is required. A gone PID must disappear from that real final
job list, or monitoring still fails closed. New unsampled arrivals do not qualify
for the exit drain. Access failure, live metric failure, FILETIME reuse, identity
contradiction, permanent ghost, manifest/owner mismatch and limits all retain
their fail-closed behavior. Memory/headroom/physical/time limits, launcher,
JobObject APIs, verified-job-only termination and external cleanup scope are
unchanged. No fake zero-byte member or parallel readiness flag was introduced.

ProcessGone diagnostics now distinguish OpenProcess ERROR_INVALID_PARAMETER
(not creation or exit proof) from GetExitCodeProcess reporting exit. For the
latter, the same still-open query HANDLE supplies creation/exit FILETIME and
the real exit code before it closes. An exited creation-time contradiction
against an earlier sampled identity fails closed. This diagnostic is **not**
used to admit a PID still listed without metrics; complete job-list reconciliation
remains mandatory. No closed-handle/PID inference substitutes for membership.

49 injected tests PASS: 8 new drain/transient/persistent/identity/oversleep cases,
12 V45 membership cases, 13 previous race cases, 16 existing threshold/owner/
manifest/ready/verified-only-termination cases. Sleep/clock in new tests are
injected; no API method is mirrored and no emulator is involved.

`native-benign-job-receipt.json` adds a meaningful actual Windows check using
exactly one benign Python child in a fresh hard-limited 64 MiB Job. It verifies
suspended/live membership, same-HANDLE creation/exit fields, parent outside the
owned job, actual termination and authoritative empty final membership. On this
run, real delayed job notification **was reproduced**: the first reconciliation
listed the exited PID, one bounded yield let it disappear, and the collector
completed without invented metrics. Only the freshly created test job could be
stopped; no emulator, ADB or device operation occurred. This is not a sustained
QEMU stability/FPS acceptance test.

The source and all receipts are frozen by SHA256. Root owns the next authorized
guarded emulator launch and actual APK validation. If a real listed PID remains
unmeasured after this bounded drain, the watchdog intentionally still stops;
that case requires further source proof, not disabling the guard or raising RAM
limits.
