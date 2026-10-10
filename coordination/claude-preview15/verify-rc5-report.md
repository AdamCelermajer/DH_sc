# Preview 15 rc5 verification (verifier rc5)

Status: STOPPED by root before the verdict (the user accepted rc5 as Preview 15 and will test by hand). Partial results: main batch 60/70 exit 0 (the 10 are menu-mode jobs lacking --skip-boot, as in rc4); B028 hurt cue confirmed (uid=284); reload results match rc4; B039 audio runs not completed.

EXE under test: `.local-inputs/windows-source-clock-v19-preview-15-rc5/dh-foundation.exe`, SHA256 `2B70D38C18B92B3924913C7CB3FD795B7E9A375FBADB0AEE397594D584440E2B` (matches brief).
Baseline for audio A/B: rc4 EXE `.local-inputs/windows-source-clock-v19-preview-15-rc4/dh-foundation.exe`.

## Results

| # | Item | Result | Evidence | Verdict |
|---|---|---|---|---|
| 1 | Main batch vs rc4 table (boot flow, B046-B052, DROP, I025, regression) | NOT-RUN yet | | |
| 2 | B039 audio cadence, serial 35 s runs (quiet) | NOT-RUN yet | | |
| 2b | B039 under CPU-burner load, rc5 vs rc4 | NOT-RUN yet | | |
| 3 | B028 lizard hurt cue (combat quiet run) | NOT-RUN yet | | |

## Verdict

Pending.
