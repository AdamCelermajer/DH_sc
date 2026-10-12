# v21.3 independent package review

**Verdict: READY for package content and test-roster verification only.** This does not accept the release; root's active bug gates and combined production review remain separate.

## Package evidence

- Candidate: `.local-inputs/windows-source-clock-v21.3`; base: `.local-inputs/windows-source-clock-v21.2`.
- Candidate manifest contains 4,420 unique entries. Every listed file exists and matches its recorded byte count and SHA-256. There are no unlisted files, missing paths, userdata entries (`.save`, `.gsave`, or `dh2_settings.tutorials`), symlinks, or reparse points.
- Candidate and source frozen executable both hash to `276C83146351F41E879F296143F8F59F74A0D3552F224D9015AEF87C3037779A`. Receipt source commit is `51c4ac5c469b01969b39d6af1be45a68e5eb2c57`.
- Candidate and v21.2 manifests have the same 4,420 paths. Only `dh-foundation.exe`, `README.txt`, `release-notes.json`, and `package-receipt.json` differ. Every unchanged path has matching baseline bytes and SHA-256; the v21.2 package itself also passes all 4,420 original manifest checks.
- `startup.args` and `swamp.args` use relative `assets` and `audio-assets` roots; `startup.args` also names relative `menu-assets` and audio-table files. No absolute external asset references were found in package config. The main-menu config has two save outputs (`--game-save gameplay.save` and `--save character.save`), both absent from the frozen manifest. The only package text files with absolute workspace paths are receipt references to the CTest log and pending final review report.

## Test and metadata evidence

- The raw log at `.local-inputs/v21.3/integration/ctest-177-51c4ac5c469b01969b39d6af1be45a68e5eb2c57.log` has all 177 numbered entries, all 177 test names unique, 177 `Test Passed.` outcomes and zero `Test Failed.` outcomes. It is the complete roster claimed by the receipt.
- Controls in the package README match `Play.cmd`, `Play-swamp.cmd`, and the two startup argument files.
- The README and release notes retain the accepted fire-pattern caveat, water-caustics and fog/lighting differences, repeated-F9 targeting-flag caveat, and partial broader content caveat. Audio language correctly distinguishes real output-path/device PCM evidence from unmeasured human audibility and acoustic parity; the detailed caveats remain in `coordination/v21.3/AUDIO-REVIEW.md`.
- Receipt status accurately remains “Frozen candidate; independent verification pending.” Its `verification.report` points to `coordination/v21.3/FINAL-REVIEW.md`, which is not present yet and remains pending root's combined review.

## Scope limit

I did not launch the packaged executable. The requested audit scope allows writes only to this report; a startup run requires isolated save/config/log/capture outputs. No candidate package file, baseline file, save, worker seed, or user window was changed.

Independent package roster verdict: **READY**, exact EXE SHA-256 `276C83146351F41E879F296143F8F59F74A0D3552F224D9015AEF87C3037779A`.

