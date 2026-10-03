# Prince bank checkpoint validator

`port/android-native/tools/validate_prince_bank_checkpoint.py` reads a frozen
`capture_native_build.py` ZIP and an exact saved repository checkpoint APK. It
does not build, install, run ADB, execute original/native instructions or change
existing reports. It refuses to overwrite its output report.

Required arguments: `--capture ZIP --checkpoint APK --output NEW_REPORT.json`.
Optional proof overrides: `--metadata-proof JSON --bank-host JSON`. Defaults
are the native PAB1 reader host audit and full-Prince-bank integration host
report. Production proof source hashes must agree with captured source hashes;
test/producer files absent from the build capture must still match the bound
proof's current hashes. Rerun a host proof into a new report after relevant
header/source changes rather than changing hashes in an old report.

Optional live inputs: `--movement JSON --lifecycle JSON --combat JSON
--bank-smoke JSON`. `--require-live` requires all four. Each provided report must
pass, bind the installed exact checkpoint SHA, and pass its recorded bounded
checks. The bank smoke additionally binds packaged metadata, observed slot IDs,
Walk/Run, authored trigger/finite closure, frozen clocks/body/pose, Activity
reselection/counter reset, screenshots and raw smoke artifacts. Slot IDs observed
across transitions do not establish simultaneous weights, all17 live engine
indices, both-slot speed or original-pose/GPU parity.

Artifact checks cover the exact canonical PAB1 metadata fields, all116 resource
records/bytes, ordered158 requests, source17 first indices, separate template1111
designation, authored Prince model, repo/Studio asset equality, and the actual
per-ABI compiler source/dependency capture. All14 native libraries must be ELF64
in exactly arm64-v8a/x86_64, with matching machines,16KiB-or-larger LOAD alignment
and congruence, and uncompressed16KiB-aligned APK ZIP entries. Metadata reader and
occurrence-coordinator exports plus live application imports are checked.

`validation=PASS` means supplied artifact/proof checks passed. `build_validation`
and `asset_validation` are separate from `live_validation`, which is NOT_RUN,
PARTIAL_PASS or PASS. The report always preserves
`physical_arm64_verified=false`, `full_game_verified=false` and
`full_bank_original_pose_parity=false`. PAB1 is a documented port metadata format;
its resource-index identity policy does not recover original CCDB addresses.
Host proofs are source-bound evidence and do not become packaged instruction
replays merely because the APK contains matching source exports.

Validator helper verification compared its independent metadata decoder against
the actual native owned dump (12 fields,116 records,158 requests), rejected10
header/count/span mutations, and checked14 ELF/ZIP entries in an existing APK.
Ten mutated ELF-class/machine/LOAD-alignment/ZIP-compression cases also reject.
This helper check is not validation of the upcoming bank checkpoint. Final
checkpoint validation needs the parent's newly frozen build and live reports.
