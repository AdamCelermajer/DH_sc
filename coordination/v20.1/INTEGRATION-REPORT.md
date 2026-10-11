# v20.1 integration evidence

## Initial earlier-merged batch, 2026-10-11

Source: `v20.1/integrate` at `dfc67d8d`, clean at build time. Build wrapper
`p14_build.ps1 -Name v201int -Jobs 2`: exit 0 (1,042 Ninja steps).
Frozen test executable:
`.local-inputs/v20.1/root/dh-foundation-initial-integration.exe`, SHA256
`18708E183185B68C512807AEA448357A072A6B65BD76B033D06D168014D05CEB`.
Full PowerShell ctest, LLVM-MinGW DLL directory on PATH, timeout 120, jobs 4:
167/168 pass, only allowed worktree `session_skill_binding` fails; 12.33 seconds.
Full log: `.local-inputs/v20.1/root/ctest-initial-integration.log`.

These checks cover the already-merged B079/B080/B081/B082/B083/B085/B087/B088/B089
batch only. They do not establish completion of unmerged work or acceptance.

### Focused normal executable checks

Fixture inputs reuse the recorded bug investigations (original footage/IDA
locations are in their reports). Runtime arguments were normalized to accepted
v20 read-only assets and new isolated per-job working directories, save paths,
logs and captures. No campaign cinematic was replayed. Fixed step 1/60;
`quiet_run.ps1`, hidden desktop, silent output. All four runs exit 0.
Evidence directory: `.local-inputs/v20.1/root/initial-runtime/`.

| Scope | Observed integration result | Limits |
|---|---|---|
| B079 chest | Interaction starts frame 260; authored marker 467 ms opens at 289; container opened and four items delivered once at 303; hero returns from interaction at 332. Frames 250/270/290/310/330/360/390 show the pose, lid change and moving drops. Later Space gives an ordinary swing, no second open/loot. | Reuses source/video investigation; no fresh pixel/timing match to original, gold individual bone approximation unchanged, sound heard by nobody in silent run. |
| B085/B089 vase | Delayed visual run avoids startup quest banner. Frames 250/268/276 show selected vase and weapon swing; 292/320 show broken vase without ring. Marker hit 276, sound submission and opening/loot path occur once. A second Space at 360 does not re-interact or duplicate loot. | First run (40 press) passed logs but its banner obscured the visual result; retained, not counted as swing visual proof. Disappearance at the exact hit frame is not established by the sampled sequence; zero-stage source vase only. |
| B088 NPC | Row365 talk at 100 accepts quest53, NEW QUEST text appears. Icon log changes type3 to type-1 at120; frames300/650 show sword. Second Space at600 raises no second TalkToNPC event. | Campaign scripts are off, so dialogue start and nearby-monster competition remain unverified here. |
| B087 marker asset | Marker draw dispatch is present. | Accepted v20 lacks `quest_marker_blue.bdae`; log explicitly reports missing resource and fallback yellow is visible. Add original blue asset to NEW package and check it there. This is not a completed correct-colour check. |

No root code behavior change was made to produce these results. Original visual
and logic match statements remain those in the individual investigations;
integrated observations above must not be promoted to broader gameplay claims.

### Baseline preservation

All 3,640 accepted v20 manifest entries passed full size/SHA256 and file-roster
verification before these runs. Accepted EXE SHA256 is
`8BA1E537D33D4D4FD5B7DD9822C4CAB5080917FE2427A3903AB7C247827E8CCE`.
Record `.local-inputs/v20.1/root/v20-baseline-full-hash.json`.

## Remaining production and release gates

Per-bug reviews, remaining implementation/runtime checks, integration of reviewed
branches, combined save/death/transition checks, full ctest after changes, final
asset manifest, frozen v20.1 independent verification and corrected release
notes are pending. See CODEX-RUN.md and the shared bug tracker for live ownership.
No active bug has been moved to RESOLVED-BUGS and no package is accepted by this
baseline report.
