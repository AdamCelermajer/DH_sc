# Independent emulator memory watchdog V36

`tools/emulator_watchdog_v36.py` is an independent Windows guard. It never
launches an emulator, invokes ADB, or kills a process by PID/image name. The
only termination primitive is a previously verified, pinned named Job Object.
Root's launch guard owns the kernel hard limit, fresh job creation, atomic
cross-session reservation and target launch. This watchdog owns independent
memory admission and continuous monitoring.

## Launcher contract

Run the launcher/watchdog outside the tool AppContainer. AppContainer tokens
can expose global memory while hiding most host processes; the watchdog
explicitly refuses that token rather than accepting a false zero-QEMU census.

The launcher creates a fresh `Local\DH2-Emulator-{UUID}` job, assigns a
suspended target, and atomically writes an immutable manifest with:

```
job_name
launcher_pid
launcher_create_time
target_pid
target_create_time
```

Creation times are exact unsigned FILETIME integers from GetProcessTimes.
The launcher stays outside the target job and remains alive throughout the
run. Invoke the watchdog with required `--manifest`, `--telemetry`, `--receipt`
and `--ready` paths. All four must be distinct. The launch guard waits for
the matching ready nonce/manifest SHA, verified=true and live watchdog process
before ResumeThread. Missing or failing readiness leaves the target suspended
and requires launch-guard cleanup.

The watchdog verifies the manifest namespace, launcher and target creation
identities, target job membership and launcher non-membership. It then takes
a complete passing initial memory snapshot before atomically publishing ready.
The target frontend may subsequently exit while actual job children remain;
that is allowed. Launcher death/reuse, manifest drift, unavailable metrics,
telemetry/monitoring failure or breached limits terminate only the pinned job.
An unverified job is never terminated by this component; the launcher must
clean up its own suspended target if bootstrap fails.

## Limits and APIs

Defaults, with explicit CLI overrides:

| Argument | Default |
|---|---:|
| --soft-job-gib | 5 GiB owned job private commit |
| --aggregate-qemu-gib | 12 GiB all visible host QEMU/emulator private commit |
| --min-commit-headroom-gib | 16 GiB system commit reserve |
| --min-physical-available-gib | 6 GiB available physical memory |
| --timeout-seconds | 1200 seconds |
| --poll-seconds | 2 seconds |
| --telemetry-limit-bytes | 8 MiB per JSONL file |

The telemetry log retains one bounded previous file (`.1`), at most 16 MiB
combined with defaults. A diagnostic JSON receipt records limits, readiness,
last snapshot/reasons and whether job termination succeeded. Logs contain
process image basenames and identities, not command lines.

`system_qemu_snapshot(api=None)` is the stable read-only launcher preflight
API. It returns `system`, `qemu`, `gone_pids` and
`aggregate_qemu_private_bytes`. System counters include commit total/limit,
physical available/total and page size in bytes. Process records include PID,
parent PID, exact creation FILETIME, image basename, PrivateUsage, working set
and handles. `evaluate(snapshot, Limits())` is pure and separable. Actual job
membership wins over parent-PID guesses; tree lineage also checks creation
ordering to exclude stale PID ancestors.

JobObjectBasicProcessIdList is information class **3**; class 9 is extended
limits. The PID query has a bounded 4096-member flexible ULONG_PTR array.
Job handle rights are QUERY(4)|TERMINATE(8). GetPerformanceInfo page counters
are multiplied by the actual PageSize; process budgets use PrivateUsage,
not resident working set. API references:
[job query](https://learn.microsoft.com/en-us/windows/win32/api/jobapi2/nf-jobapi2-queryinformationjobobject),
[PID structure](https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-jobobject_basic_process_id_list),
[job ownership](https://learn.microsoft.com/en-us/windows/win32/procthread/job-objects).

## Verification and limits of the evidence

Fixture tests: 15 passed, covering private vs working-set accounting,
independent aggregate thresholds, exact threshold boundaries, reserve floors,
timeout, missing metrics, launcher death/reuse, invalid manifest/job identity,
ready handshake, bounded log rotation, telemetry failure and verified-job-only
termination. Unrelated QEMU identities are observed but never cleanup targets.

Read-only Windows API probe passed with native 64-bit structures:
PerformanceInfo104, MemoryCountersEX80, ProcessEntry568 bytes. Full-host
probe saw 377 processes and no active QEMU; the earlier restricted probe saw
only four processes. That difference motivated explicit AppContainer refusal.
The restricted-token negative test is separately recorded. No emulator was
launched, resumed or terminated during these component tests.

Root still owns a benign suspended-job end-to-end handshake/termination test
before emulator use. This component does not prove emulator leak resolution,
frame-rate improvements, or safety of a new unmanaged emulator launched by an
external program. Root's one-emulator reservation/preflight policy is required.
