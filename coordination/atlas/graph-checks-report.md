# Universal graph reviewer checks

This report is produced by `graph_checks.run(ctx)` and is deliberately separate
from atlas generation. The focused checks load the generated graph once; small
coverage rejection fixtures run in memory and do not copy the large corpus.

Coverage invariants exercised: a valid minimal graph passes; duplicate typed
node IDs, missing source entities, duplicate relationship IDs, dangling
relationship endpoints, missing reference owners/targets, and missing profiles
are rejected. Known paths cover the Mud Lord, Knight skills, Bow01, Bow03 and
Potion0. Every explicit item resource edge with a known indexed asset/model must
appear in the universal graph. Each known root also needs outgoing dependencies
and incoming users. These are data graph reachability checks, not claims about
execution in the game.

## Results

| Check | Passed | Evidence or failure |
| --- | --- | --- |
| `{{CHECK_NAME}}` | `{{PASSED}}` | `{{EVIDENCE}}` |

## Scope and gaps

This validates generated graph structure and discoverability. It does not prove
that authored associations execute at runtime. Any missing path remains an atlas
resolution gap and should be reviewed against its source ledger row.
