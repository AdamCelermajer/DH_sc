# Session RAM audit, 2026-10-06

User request: check all sessions after the RAM crash. No build, emulator, WSL
client, compiler, or gameplay test was started for this audit. The host census
was read-only and executed outside the sandbox because the sandbox's process
namespace does not expose other sessions.

## Current host evidence

At 19:41:02 UTC: 19.41 GiB physical RAM available, 23.24 GiB committed,
385 processes. Host census count matched the global process counter.
No emulator, QEMU, clang, Ninja, CMake, Gradle/Java, Android Studio, wsl.exe,
wslhost.exe, or vmmemWSL image appeared in the census. Windows' wslservice.exe
and protected vmmemCmZygote were present; their presence does not establish
an active game build or WSL test. Protected-process memory queries can fail;
150 process metrics were unavailable and were retained as diagnostics.

Largest measurable individual private allocations were ChatGPT.exe 1.04 GiB,
codex.exe 0.84 GiB, python.exe 0.83 GiB, and opencode.exe 0.59 GiB. These
measurements do not identify the earlier crash initiator. No unrelated process
was terminated.

Full receipt: `current-session-audit-v60.json`.

## Session and agent checks

| Session / worker | Current task | Execution status / evidence |
|---|---|---|
| Main root | Integration and RAM coordination | No owned compiler/build/emulator handle; only completed census/source-read tools during audit |
| combat_animation | Canonical player/Character factory, same constructor inventory/Gear and save ownership | Reports no live children or tools; source only; no measured test peak available |
| level_runtime_integration | Pre-Gear Application PlayerManager and first-local-controller ownership | Reports no live children or tools; source only |
| status_progression | Selected-profile GS/Swamp start and loading-stage integration | Reports no live children or tools; source only |
| Check running sessions safely | Generic loader generated-stream integration; three existing source workers | Parent reports no live execution handles; last guarded compiler terminal PASS, 90.75 MiB peak; source workers only |
| Inspect app launch and menus | Persistent Flash queue and safe HUD unload/reload | Fresh audit confirms no children, execution handles or heavy jobs; eight-file source packet prepared, uncompiled and untested |
| Audio chat | Shared-root V45 audio installation | Frozen source integration; reports no live children, owned processes or pending tool handles; compile awaiting root admission |
| Coordinate Act 1 rebuild sessions | Coordination review | Idle; reports no implementation, children or background jobs launched |

Older publication/backup/diagnosis chats in the app listing were not loaded.
Their titles alone are not evidence of active processes.

## Crash evidence and limits

Existing `incident.json` records a jump from 587 to 3,329 processes in about
five minutes, 77.92 GiB committed memory, 0.96 GiB free physical RAM at the
last retained sample, and a minimum of 0.43 GiB. The owned emulator held
6.98 GiB private memory at the final sample, below the user's 15 GiB cap.
Its guard reported successful termination before the reboot. The recording
lacks the image/parent distribution needed to identify the process surge's
origin; git/conhost crashes do not establish that origin.

## Execution policy

Parallel source work continues. Emulator, full builds, WSL and heavy tests
remain off. Root alone admits one serial Windows NDK syntax leaf inside a
512 MiB process/job limit, at most four job processes, and a 30-second timeout.
Admission requires at least 8 GiB free physical RAM and no more than 900 host
processes. This is a compiler limit, separate from the user's emulator cap.
Do not mark syntax checks as completed gameplay or Act 1 acceptance.
