# Package B Step 0 scope enumeration

Status: **scope-only; classification is held pending the root's Step 0 release.** Every emitted row is `TODO`. This report describes the audit boundary, not a staging-readiness result.

## Scope boundary and evidence

Act 1 includes the existing Act 1 atlas scope plus the two canonical backlink scenes identified by Package C: `001_swamp.mlx` (`6b2f662bc32e54797884`) and `002_swamp_witchcave.rule.xml` (`f6ed60ebabd20b89aaf1`). Their Act 1 membership is an inference from the v21.8 handoff. The closure root is `003_darkwood.mlx` (`2e1304ff2c8ed98caa94`), labeled `act_2_inferred` because the requested boundary is an inference. The other 34 scenes reached through the transition graph remain `unknown_act_membership`; their references are included in `act_2_inferred` for this broad post-Darkwood package scope, without claiming a narrative act assignment.

Package C's machine-readable evidence is `coordination/atlas/r2/act2/scope.json` (SHA-256: `1a141a0468e36a68afff15b5184e0dd5788b2c8eb0960d07c62e9b5285473b37`). It records 37 scenes, 86 unique authored transition edges (105 provenance/source-variant rows), 306 module-source variants (320 rows), 1,670 static actor candidates, and 14 frontier rows. The closure follows authored `TriggerZoneExitLevel.levelName` declarations in direct and procedural MGP modules, recursively follows nested `Module.mgp` includes, and follows decoded `Script_ChangeLevel` targets. It preserves conditional, duplicate, return, and branch edges. This is evidence of authored declarations, not runtime execution. Malformed or unresolved frontier content and alternate transition mechanisms remain open.

The enumerator joins exact canonical source paths from reached scene rows, all Package C module-source rows, transition edges, campaign/reference tables, actor/resource ownership chains, and all nine Prince skill lists plus faery and item catalogs. Non-resource and dynamic sites are retained for later explicit `NOT-NEEDED(reason)` or `UNRESOLVED` classification. Serialized string candidates are not promoted into executed resources by name similarity.

## Scope counts

There are **28,555 act/reference pairs**, representing **22,607 unique canonical reference IDs**; **5,948** IDs occur in both acts. These are different metrics and are recorded separately in `package-b-progress.json`. Raw kind counts are preserved there under `counts_by_act_kind`; aggregate families are:

| Act label | Anim | Sound | Effect | Projectile | Model | Script | Other/nonresource |
|---|---:|---:|---:|---:|---:|---:|---:|
| `act_1` | 472 | 279 | 152 | 21 | 3,732 | 145 | 3,034 |
| `act_2_inferred` | 637 | 647 | 408 | 45 | 10,379 | 552 | 8,052 |

All 57 kind shards are below 40 MB (largest: 15,351,906 bytes). Progress reports 28,555 `TODO`, 0 processed. No original corpus or staged folder was modified.

## Verification and remaining work

`r2_staging.enumerate_scope(ctx)` regenerated the shards and package progress from the current Package C scope. `r2_staging.check(ctx)` passed its shard existence, unique `(act, reference_id)`, 40 MB limit, and all-TODO checks. Classification, manifest joins, staging selections, stage-script scratch preflight, source-unresolved/readiness deltas, and top blocking-reference analysis remain pending the root's Step 0 commit/release.
