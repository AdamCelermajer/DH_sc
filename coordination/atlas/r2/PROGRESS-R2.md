# Original Data Atlas round 2 progress

Phase: **NATIVE CONTRACT AUDIT AND STAGING REFINEMENT IN PROGRESS**.

Processed: **34,151/34,327 (99.49%)**.

Processed means DONE, UNRESOLVED(reason), or NOT-NEEDED(reason). It does not mean that unknown contracts are resolved or that content runs in the port.

| Package | Rows | Processed | Percent | States |
| --- | --- | --- | --- | --- |
| A | 271 | 134 | 49.45% | {"DONE": 117, "NOT-NEEDED": 4, "TODO": 137, "UNRESOLVED": 13} |
| B | 33378 | 33378 | 100.00% | {"NOT_NEEDED": 28025, "ORIGINAL_EXISTS_NOT_STAGED": 318, "RESOLVED_STAGED": 1344, "UNRESOLVED": 3691} |
| C | 671 | 632 | 94.19% | {"DONE": 617, "NOT-NEEDED": 11, "TODO": 39, "UNRESOLVED": 4} |
| Open questions | 7 | 7 | 100.00% | {"DONE": 7} |

Package B counts act/reference pairs; its package ledger also records unique source-site coverage. Shared player resources can belong to both act scopes.

Act boundaries are INFERENCE. Follow the source transition frontier and package scope files rather than assuming the entire reachable closure is narrative Act 2.

Resume with TODO rows first, then the explicit unresolved reasons. Root owns integration, validation, commits and follow-up investigation. Package output and source evidence remain separate from gameplay implementation and runtime/visual verification.

Regenerate from original sources: `./tools/atlas/regen.ps1 --round2`. Check: `./tools/atlas/atlas.ps1 check --round2`. Source regeneration builds the round-1 dependency atlas in an ignored scratch directory and writes only round-2 results to tracked output.
