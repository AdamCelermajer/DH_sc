# Quiet, parallel EXE runs (MANDATORY for every worker and verifier from now on)

The user must not be disturbed: no windows, no focus stealing, no sound, no long single-threaded waits.

1. **Never start dh-foundation.exe directly** (no Start-Process, no `cmd /c`, no bash `&`). Always go through
   `port/windows-foundation/tools/quiet_run.ps1`. It runs each job on a private hidden Windows desktop (no window is ever shown,
   no console), below-normal priority, with `DH_AUDIO_SILENT=1` so the audio pump runs and counts exactly as normal but
   submits zeros (no sound). Rendering, GL captures (`--capture file.ppm`), logs and exit codes work normally there.
2. **Batch, then analyze.** First write ALL job folders/args for your whole check list (different working dir, save copies,
   args file and capture/log path per job), put them into ONE jobs JSON, run `quiet_run.ps1 -JobsFile jobs.json -Parallel 12`
   (28 logical CPUs; 8-16 parallel is fine; each job is mostly CPU bound), then read the logs/captures. Do not run jobs one by one.
   Jobs JSON: `[{"name":"b037-p12","exe":"<abs exe>","args":["--startup-config","x.args"],"cwd":"<abs dir>","log":"<abs log>","timeoutSec":180}, ...]`.
   Use `-Summary summary.json` for exit codes. Capture paths inside args files must be ABSOLUTE.
3. Real-time audio-cadence checks (WinMM underruns, B039) are timing-sensitive: run those in a separate batch with `-Parallel 1`
   (still silent and hidden), not mixed into the parallel batch. Never pass `-AllowSound`.
4. Only the EXE `rc2` or later contains DH_audio silent support (`DH_AUDIO_SILENT`). For A/B against older EXEs (Preview 12) their audio
   will still play if the job passes `--audio`: for those baseline runs REMOVE the `--audio`, `--audio-assets`, `--audio-table` options from the args
   unless the check is about audio.
5. Never touch/focus/kill other processes. Everything you launch is finished (or killed by the timeout) when the script returns.
6. Convert PPM -> PNG/crops offline (crop.py/ppm2png.py) and LOOK at images afterwards; that never disturbs anyone.
