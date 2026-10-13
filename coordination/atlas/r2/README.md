# Original Data Atlas round 2

Source contracts, a staging ledger, and per-scene reconstruction dossiers. Original Android and iOS inputs are complementary partial corpora. There is no iPhone binary or verified iOS native ID mapping. Native contracts use the Android library; platform/version uncertainty remains explicit.

| Package | Work rows | Processed | States |
| --- | --- | --- | --- |
| A | 271 | 170 | {"DONE": 153, "NOT-NEEDED": 4, "TODO": 101, "UNRESOLVED": 13} |
| B | 33378 | 33378 | {"NOT_NEEDED": 25169, "ORIGINAL_EXISTS_NOT_STAGED": 318, "RESOLVED_STAGED": 1344, "UNRESOLVED": 6547} |
| C | 671 | 632 | {"DONE": 617, "NOT-NEEDED": 11, "TODO": 39, "UNRESOLVED": 4} |

Processed includes explicit UNRESOLVED and NOT-NEEDED dispositions. TODO means investigation remains. Contract completion does not establish Windows gameplay parity.

[Native contract index](natives/INDEX.md) · [Staging classifications and selections](staging/CLASSIFICATION-AND-SELECTIONS.md) · [Level dossiers](act2/dossier.md) · [Progress](PROGRESS-R2.md)

Package B covers 33,378 act/reference pairs, 27,430 distinct source IDs, and 5,948 IDs shared across act labels. Typed non-file keys retain a staging-only disposition; their table/resource dependencies must still be followed.

| Act label | Before source unresolved | After source unresolved | Ready reference pairs | Already staged pairs |
| --- | --- | --- | --- | --- |
| act_1 | 1787 | 1976 | 82 | 499 |
| act_2_inferred | 6527 | 4571 | 236 | 845 |

Before/after counts concern source classification, not successfully loaded game content. Selected physical paths are deduplicated across groups and base destinations.

| Selection JSON | Physical files |
| --- | --- |
| [asset-selection-r2-act_1-assets-extra-android-data-assets-data.json](staging/selections/asset-selection-r2-act_1-assets-extra-android-data-assets-data.json) | 75 |
| [asset-selection-r2-act_2_inferred-assets-extra-android-data-assets-data.json](staging/selections/asset-selection-r2-act_2_inferred-assets-extra-android-data-assets-data.json) | 101 |

Selections were checked by the original staging script, stopping before its first target-directory creation. Packed iOS .dat candidates need materialization/version review and are not offered as ready-to-copy files. [Preflight receipt](staging/preflight.json).

| Unresolved reason | Reference pairs |
| --- | --- |
| OTHER | 4782 |
| Serialized/file-like text is present, but its consumer role is not proven; matching files are diagnostic candidates only. | 832 |
| NAME_NO_FILE | 795 |
| No exact, proven file mapping or typed non-resource rule applies; the source site remains unresolved. | 70 |
| FILE_MISSING | 68 |

Detailed per-kind metrics and referrer chains: [metrics](staging/before-after-metrics.json), [top 50 references](staging/top50-blocking-references.json). The global audio catalog checks preserve 45 absent sounds.xml filenames and absent sfx_faerie_ice.wav; these differ from scoped pair counts.

## Seven source questions

1. **Player and defender combat-result delivery** — DONE. Character::F_ApplyResult 0x3b10b4 dispatches first to attacker CharAI at Character+968, then defender CharAI at the same offset, both virtual slot +180 with the same (attacker, defender, result). Result mask 0x20000000 suppresses both. Online authority can return before this tail.

2. **SnS target-chance units and comparisons** — DONE. The original _F_CalculateResult 0x3b2638 requests Random(100), then shifts the random integer left by 8 before the chance helper. Random uses seed modulo the positive bound, so the comparison domain is integers 0..99 multiplied by 256.

3. **Class formula types 5 and 6** — DONE. Type5 dispatches to _ScaleWithPercentage 0x3e2c78: destination S[d] becomes arithmetic_shift_right_8(low32(S[a]*S[d])). Type6 dispatches to _ScaleWithPercentageForBuff 0x3e2c10: scale S[a] multiplies S[d], or owner resolved[d] when the caller enables buff mode.

4. **Buff capacity, reuse and per-property combination** — DONE. PROPS_AddBuff 0x3e232c maps capacity<=0 to128. Replacement/reuse scans run only when current count equals requested capacity. Lower unsigned strength entries qualify and their strength is immediately rewritten; equal-strength permanent entries qualify; timed ties compare elapsed time through TMR_TimeLeft. This is not a general FIFO eviction policy.

5. **RemoveBuff receiver and foreign handles** — DONE. Character::_RemoveBuff 0x3b842c calls PROPS_DelBuff on the bound receiver at Character+1376. The optional pointer selects an instance inside that receiver's class declaration; it never changes the receiver to the pointer's owner.

6. **CharAnimTable stun lookup and admission** — DONE. SM_SetStunState 0x3c5ffc rejects bosses first. It obtains GetCharAnimTableId 0x3a3228 (invalid authored ID falls back to17), then validates the returned row against CharAnimTable size. It reads the Stunned field at byte140 of the160-byte native row, not an arbitrary hardcoded animation.

7. **ApplyBuff binding and pointer branch** — DONE. Character::createBindings 0x3b56bc explicitly registers ApplyBuff both as global bound function and object method to Character::_ApplyPropClass 0x3babdc. The atlas alias is correct; this is not a guessed similarly named routine.

[Full answers, source addresses, evidence and behavioral cases](natives/QUESTIONS.md).

## Readiness and remaining gaps

The source-transition closure contains 39 scenes and 121 edges. It includes later content and Act 1 backlinks; narrative act boundaries are INFERENCE. 217 distinct character rows have source-linked dossiers. Repairs to malformed source rule text are logged; original bytes were not changed.

| Highest-reaching blocker | Affected scenes |
| --- | --- |
| Windows_production_runtime_verification | 39 |
| integrated_visual_runtime_verification | 39 |
| native:AddToVFTable | 39 |
| native:ClearTarget | 39 |
| native:GetPyCst | 39 |
| native:GetState | 39 |
| native:MoveTo | 39 |
| native:Stop | 39 |
| package_A_native_contract_cross_join | 39 |
| package_B_resource_staging_cross_join | 39 |
| resource:loot | 39 |
| native:HasMaster | 38 |

The conservative playable-support metric is {'denominator': 234, 'numerator': 0, 'percent': 0.0}. Its denominator counts required per-level capabilities. Zero verified support here means this static investigation supplied no integrated runtime/visual proof; it does not assert every existing port feature fails.

[Coverage and denominator](act2/coverage.md) · [Implementation ranking](act2/blocker-reach.md) · [Scope expansions and retained frontier records](act2/scope-expansion.json)

## Rebuild and verification

Run `./tools/atlas/regen.ps1 --round2` from the atlas2 worktree. It rebuilds source indexes in ignored scratch storage, then regenerates round-2 artifacts. Run `./tools/atlas/atlas.ps1 check --round2` for scope, contract, classification, staging, evidence and original known-answer checks. A remaining TODO makes the final check fail.

[Check receipt](check.json) · [Immutable initial scope](STEP0.json) · [Tracked-file-size recommendation](TRACKED-FILE-SIZES.md)

All new generated files must stay below 40,000,000 bytes. Existing oversized round-1 outputs are preserved. A separate cleanup should untrack or shard regenerable snapshots and address historical Git blobs before a future push. This round does not push or rewrite history.
