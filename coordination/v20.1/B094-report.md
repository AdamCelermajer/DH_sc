# B094: brown boss lizard not targetable

The user's correction identifies **the brown boss lizard**, not Celeste/Rene. The old fix094 INTENDED verdict is invalid for this report because its identity premise was wrong. Its companion regression test is unrelated and is not part of this patch.

Implementation and evidence are owned by the combined cinematic session `/root/b094_b091_boss_cinematics`, branch `fix/b091`, worktree `DH_wt/fix091`. See [B091-report.md](B091-report.md) for original video sequences, IDA callers/gates, implementation, tests, frozen executable, run receipts, and limits.

Observed reproduction: accepted v20 EXE SHA256 `8BA1E537D33D4D4FD5B7DD9822C4CAB5080917FE2427A3903AB7C247827E8CCE`, normal compiled-bank LizCaptain_Intro authored contact, SKIP80, then walk toward boss: brown mace lizard beside hero without boss ring/plate, HP260, lifecycle Spawn1, physical0 through1800. Screenshot shot13 and P1 462-478 s identify the Bogwomp Chief/model/room. Exact Type2/3 instance from a still remains an inference.

Fixed private frozen EXE SHA256 `964133243B31BCC7B2E17F3B4A43B07D287F291F53AD6662718AC8CA99C680A2`: captain played, early skip80, late skip230 all show boss ring/plate, deal and receive legitimate hits, die and disappear after the source corpse delay. Hidden/silent isolated fixtures, no command injection/enemy HP0. Private player HP1000 is a duration fixture; reward XP not claimed. Build exit0; ctest167/168, known session_skill_binding only.

**Matches video:** boss is a hostile target and fights after the scene; bounded death/disappearance. **Matches IDA:** skipped Idle exits Spawn through physical admission; kind45 returns to gameplay only when its current scripted gate clears (+5248=0, +1345=1), even before final duration. The final captain trace defers201/252, Unsets301 and releases once302. **Not verified:** exact user save/full route and continuous non-skipped passivity (baseline played boss fights when approached); original SKIP video comparison is absent. Root review/integration remains before release closure. Package assets: none.

Reviewer repair evidence is under `.local-inputs/claude-v20.1/fix091/audit/gate-final/` with frozen receipt at2026-10-11 19:23:52UTC. Full ctest recheck167/168; a first schema_v4 Windows save-replace error5 passed3 focused retries and the full recheck, with the first failure preserved. Prior ungated/interim binaries are historical.
