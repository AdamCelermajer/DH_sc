# v20 release work — 2026-10-11

Requested result: one `windows-source-clock-v20` package, combining Preview 16,
`p16.1/integrate`, completed B068, and `p17/integrate`. No intermediate releases.

## Investigation and ownership

Required handoffs read in order. Main checkout was actually `windows-foundation`
at `b3db5dc0` (the requested `e0f9d23a` is its parent); both are documentation
handoffs beyond the Preview 16 code. Untracked `docs/ida-fidelity-audit-2026-10-11/`
is preserved. `v20/integrate` starts at `2f0030e2`, clean, in `DH_wt/v20int`.

- B068 implementation: fresh `/root/b068_cinematics`, GPT-6.1 Sol medium,
  exclusive `fix068` ownership. All cinematic problems stay in this session.
- Integration: `/root/v20_integration`, GPT-6 Luna high, exclusive `v20int`
  implementation/build ownership. Merge B068 before P17; retain both sides.
- Evidence review: `/root/v20_evidence_review`, GPT-6 Luna high, read-only source
  review with its own report.
- Frozen executable verification: fresh `/root/v20_frozen_verifier`, GPT-6 Luna
  high, waiting for the final package/hash. A/B baseline is accepted Preview 16,
  SHA256 `3ACF50D2A249962156583C3F13CE010443F6AB65A94A7E0DBE423D71CF4D3D1D`.
- Root owns acceptance, package metadata, tracker/history and `publish` push.

## Verification defined before changes

B068 must compare reference frame sequences first, then IDA/recovered logic,
then actual production frames for every authored Act 1 cinematic. Original
timing and animation continuity need sequences. Unknowns remain explicit.

The integration gate is a successful build and full CTest run (only
`session_skill_binding` may fail in a worktree), plus normal runtime tests of
conditions, doors, Space transitions, generated levels and preserved B069-B077
behavior. The frozen package is verified independently against Preview 16 with
isolated saves. Game runs are hidden and silent through `quiet_run.ps1` only.

Package content must include P17 assets required for normal transitions, rather
than relying on scratch overlays. The packager refuses existing destinations
and base packages containing saves; the manifest covers every other shipped
file, including receipts and notes.

## Current status

Investigation and assignments started. B068 completion, merges, build/tests,
integrated runtime/visual verification, frozen package, independent verdict,
tracker resolution and remote push are **pending**. No v20 acceptance is claimed.
Red-door flame may be a known issue by the user's explicit release instruction.

## Root intermediate review (not release verification)

B068 logical step `a4f88744`: root inspected the code diff, runtime
`fix068/audit-opening-before/f1500.png`, `audit-opening-rigid/f1500.png` and
`fix068/v20audit/opening.jpg` reference sequence t85..142 s. The restored authored
rigid mesh visibly adds the missing gold halo; source evidence is VisualObject
constructor `0x472a0c`, with collider ancestry excluded. This supports that bounded
change, not a claim of full cinematic fidelity. Root verified that unqualified
trigger contact returns before modifying runtime edge state, and identified a
remaining stale pre-tick position edge when a script teleports/unmarks the actor;
the B068 owner is addressing it using current post-command actor facts and will
test full played/SKIP openings plus genuine free-play re-entry.

Integration investigation found inherited Swamp source-scope URI stays static
through level transitions, and P17 tests had used external asset overlays.
Integration owns current-level scope derivation and production tests; root will
include finite required asset dependencies in the frozen package. Inherited
`RENE_FOLLOW` forced condition remains under source investigation; deleting it
without a source-backed producer would knowingly disable the requested follower
fix. Any unresolved forced gate must be disclosed as a caveat.
