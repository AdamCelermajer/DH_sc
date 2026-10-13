# Original Data Atlas round 2 progress

Phase: **STEP 0 ENUMERATED; ALL ROWS TODO**.

Processed: **0/29,290 (0.00%)**.

Processed means DONE, UNRESOLVED(reason), or NOT-NEEDED(reason). It does not mean that unknown contracts are resolved or that content runs in the port.

| Package | Rows | Processed | Percent | States |
| --- | --- | --- | --- | --- |
| A | 271 | 0 | 0.00% | {"DONE": 0, "NOT-NEEDED": 0, "TODO": 271, "UNRESOLVED": 0} |
| B | 28555 | 0 | 0.00% | {"TODO": 28555} |
| C | 457 | 0 | 0.00% | {"TODO": 457} |
| Open questions | 7 | 0 | 0.00% | {"TODO": 7} |

Package B counts act/reference pairs; its package ledger also records unique source-site coverage. Shared player resources can belong to both act scopes.

Act boundaries are INFERENCE. Follow the source transition frontier and package scope files rather than assuming the entire reachable closure is narrative Act 2.

Resume with TODO rows first, then the explicit unresolved reasons. Root owns integration, validation, commits and follow-up investigation. Package output and source evidence remain separate from gameplay implementation and runtime/visual verification.

Regenerate from original sources: `./tools/atlas/regen.ps1 --round2`. Check: `./tools/atlas/atlas.ps1 check --round2`. Source regeneration builds the round-1 dependency atlas in an ignored scratch directory and writes only round-2 results to tracked output.
