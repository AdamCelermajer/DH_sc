# DH_sc emulator memory protection V36

Windows Resource-Exhaustion events attribute tens of GiB of committed memory to
qemu processes, including approximately58.8GiB and22.3GiB in one event. This
proves host emulator resource exhaustion; it does not identify a DH2 native-code
leak or prove the exact cause of the Windows bugchecks. Event evidence is retained
in `port/android-native/reports/performance-v35/windows-memory-events-v36.xml`.

All automated emulator launches must use
`port/android-native/tools/launch_guarded_emulator_v36.py`. The root's old direct
startup helper now delegates to it. Old PID-based/unbounded recovery is disabled
with its source preserved; the loader owner disabled its private direct launcher.

The hard bound uses the Windows committed-memory job limit, not a working-set
trim or an Android guest RAM setting. Microsoft documents these as enforceable
commit limits: https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-jobobject_extended_limit_information

## Enforced protection

- Fresh named Windows Job Object with a default6GiB job-wide and per-process committed-memory
  limits. The target is created suspended and cannot execute before assignment,
  OS-limit readback and independent watchdog readiness have passed.
- Descendants automatically inherit the job; no breakaway flag. Last-handle-close
  cleanup is enabled. The watchdog also stops the pinned job if the launcher dies.
- Independent process verifies PID+creation FILETIME, actual named-job membership,
  immutable manifest, monitoring completeness and full-host visibility. It refuses
  an AppContainer census, which would hide other host emulator processes.
- Defaults:5GiB early stop,12GiB aggregate emulator private memory,16GiB system
  commit reserve,6GiB available-physical-memory floor,20-minute maximum test duration.
  A per-test soft threshold below the selected hard cap can be specified.
  The explicit user-requested paging profile below changes admission thresholds.
- One emulator at a time across coordinated sessions, enforced with a named launch
  mutex, immutable lease registry and live owner/job checks. Existing unmanaged
  emulators cause admission refusal. Nothing is automatically killed by name or
  stale PID; cleanup targets only the newly created verified job.
- Receipts include private commit, working set, handles, system headroom, job
  members, decision and termination result. Telemetry rotates within a bounded size.
  Guest saves and existing artifacts are not deleted; snapshots are not overwritten.

Direct Android Studio/manual launches are outside this launcher’s job limits.
Use Studio with an already guarded device rather than creating another emulator.
Job commitment limits do not cap every GPU-driver/shared/kernel allocation, so the
independent global headroom check remains necessary. No blanket guarantee against
all Windows or driver crashes is claimed.

## Verified protection

- Actual64MiB synthetic job: OS refused further committed allocation, peak stayed
  below cap, suspended-before-assignment behavior and host log capture verified.
- Actual descendant inheritance and last-job-handle close killed only owned test
  processes. Ten hard-job checks passed.
- Actual independent watchdog: readiness while suspended, timeout and launcher
  death stopped the verified job; an unrelated synthetic process stayed alive.
- Sixteen watchdog fixture/protocol tests passed, including the explicit paging
  override, retained commit/identity/job stops, private commit versus
  working set, aggregate pressure, missing metrics, reused identities and telemetry
  failures. Full-host API and AppContainer refusal probes passed.
- Real emulator startup with software graphics stopped at5.03GiB in18seconds.
  Hardware graphics stopped at5.04GiB in14seconds. Neither test launched DH2.
  The5GiB early threshold was below this API37/4GiB guest's startup baseline.
  The6GiB hard cap remained installed and Windows retained substantial headroom.
- The5.5GiB early-stop calibration also stopped at5.53GiB in24seconds. A separate
  GLES-only/hardware calibration stopped at5.51GiB in23seconds. No game launch was
  requested in these startup tests. Neither backend nor disabling guest Vulkan
  resolved admission within these limits. Boot growth alone does not establish a
  leak. These were the conservative default-profile results; the user later
  explicitly requested a higher bounded paging profile.
- Concurrent admission was refused against an actual live64MiB named job lease,
  without launching a second emulator.

## Resource investigation

`gpu-resource-growth-audit-v37.md` identified missing aggregate texture/FX byte
budgets and allocation churn risks. V40 now covers FX GPU buffers and retained
HUD bitmap/texture/framebuffer allocation; model/effect texture admission is
the next integration. The SWF vertex cache is bounded8MiB/1024 entries. World
buffers and FX CPU caches still need admission. These observations are not
causal proof of the host leak.
Continue with short guarded measurements and explicit GPU/CPU resource counters.

Historical software-renderer performance captures remain valid observations of
their recorded builds, but combined final APK acceptance and long-run stability are
pending. No unbounded reproduction is permitted to obtain that evidence.

## Explicit user-requested paging profile, 2026-10-06

The user explicitly requested a15GiB emulator cap and later authorized paging
despite low available physical RAM. The root launch helper now selects15GiB
hard job/process commit,14GiB early stop,16GiB aggregate emulator private-memory
limit and16GiB global system commit reserve. Guest RAM remains4GiB. It runs a
visible emulator, not headless, with one immutable named-job lease.

`--allow-guest-paging --allow-physical-pressure` explicitly disables only the
available-physical-memory admission/stop check. Physical headroom is still
recorded. The Windows job cap, commit reserve, aggregate check, single-instance
mutex, pinned identity/membership, telemetry completeness and lease deadline
remain enforced. Default invocations retain the conservative physical checks.
This authorizes host paging pressure; it does not imply smooth frame pacing.

The first higher-cap profile retained its physical floor and stopped during
startup. With the user's explicit pressure override, the emulator booted and
ran Crypt. Its20-minute lease then stopped at the deadline, not for memory:
job private commit approximately6.72GB, free physical RAM approximately9.0GB,
system commit headroom approximately51.9GB. The replacement lease is bounded
to one hour; the launcher still shuts down its verified job at expiry.

Receipts are under `.local-inputs/emulator-guards-v36/`; the current profile's
manifest is `20261006-155200-0d8b99fb.manifest.json`. This is QA authorization
for this task, not a claim that the emulator or game requires15GiB.
