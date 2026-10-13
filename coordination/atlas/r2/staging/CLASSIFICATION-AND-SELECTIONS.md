# Package B classification and staging selections

Package B has classified **33,378/33,378 Act/reference pairs**. The current inventory represents **27,430 unique canonical reference IDs**, of which **5,948** occur in both act labels. Raw kind shards remain available in `scope/` and `classification/`; aggregate families follow.

## Scope and evidence

The immutable Step 0 baseline is **28,555 pairs / 22,607 unique IDs**. Package C's current authored-source closure is `r2/act2/scope-current.json` (39 scene rows, 121 source edges, 496 module-source variants). It adds **4,823 reference pairs**; `scope-expansion.json` retains the prior IDs/counts and the added rows' initial `TODO` state. The two Act 1 backlink scenes and `act_2_inferred` membership remain explicitly labeled in the source shards; this is not a claim that every reached scene is narratively Act 2.

The source audit is in `STAGING-RULES.md`. The content resolver hash is `B5C1D517CFC194987D4E912A5535B0FE37CC184AE5F7AE4698EAF5B7ADEEAEEB`; the frozen staging script hash used for preflight is `10326BFE350D8A6620A797AEBBB0B0C431008AEAF655F0E66F004F78C185767F`. The source resolver is evidence for the port's candidate rules, not proof that an untyped serialized string is executed. Top unresolved rows preserve recorded mob, level, skill, or source-file chains in `top50-blocking-references.json`.

## Per-act family scope

| Act label | Anim | Sound | Effect | Projectile | Model | Script | Other/nonresource |
|---|---:|---:|---:|---:|---:|---:|---:|
| `act_1` | 472 | 279 | 152 | 21 | 3732 | 145 | 3034 |
| `act_2_inferred` | 743 | 729 | 451 | 46 | 13920 | 622 | 9032 |

## Source-resolution and staging metrics

“Before unresolved” is the atlas source row's original `UNRESOLVED` status. “After unresolved” includes Package B `UNRESOLVED`, `AMBIGUOUS`, `PORT_RULE_ABSENT`, and `ABSENT_ALL_CORPORA`. These metrics count act/reference pairs. “Ready to stage” counts pairs whose exact original regular file is absent from staged2 and included in a selection; “already staged” counts pairs whose destination is present in the manifest. Shared pairs can refer to the same selected path.

| Act label | Before unresolved | After unresolved | Ready to stage | Already staged |
|---|---:|---:|---:|---:|
| `act_1` | 1787 | 1976 | 82 | 499 |
| `act_2_inferred` | 6527 | 4571 | 236 | 845 |

Terminal classification totals: **RESOLVED_STAGED=1344, ORIGINAL_EXISTS_NOT_STAGED=318, PORT_RULE_ABSENT=0, ABSENT_ALL_CORPORA=0, AMBIGUOUS=0, UNRESOLVED=6547, NOT_NEEDED=25169**. No resource row is called `ABSENT_ALL_CORPORA` unless an exact rule and concrete target support that conclusion. Unresolved native/dynamic, serialized, packed-only, or embedded-script cases stay unresolved with the evidence limit stated on each row.

## Selections and preflight

- `r2/staging/selections/asset-selection-r2-act_1-assets-extra-android-data-assets-data.json`: 75 source paths; `assets-extra/android/data` → `assets/data`.
- `r2/staging/selections/asset-selection-r2-act_2_inferred-assets-extra-android-data-assets-data.json`: 101 source paths; `assets-extra/android/data` → `assets/data`.

The selections contain **176 unique physical source paths** total; this is lower than the number of ready-to-stage pairs because several rows share one file.

The selected paths have no duplicate source path across files/groups and no destination already in the base manifest. The preflight executed `stage_assets_v21_8.py`'s original `main` through base-manifest hashing, selection validation, source checks, and duplicate/destination checks, stopping before `target.mkdir`. **2/2** checks passed; `preflight.json` records `mutated_staged_or_base=false`. This validates copy inputs only; it does not create an output package. Packed iOS `.dat` resources are not offered as ready-to-stage regular files; materialization or extraction remains an explicit blocker when relevant.

## Known-answer preservation and remaining limits

`known-answers.json` reproduces the atlas evidence that **45** distinct `sounds.xml` filenames have no file in the combined original source corpus and that `sfx_faerie_ice.wav` is absent. These global catalog checks are separate from the scoped reference-pair metrics. The ledger does not invent filename aliases or infer execution from matching serialized text. The source parse repairs that expanded Package C's closure are recorded in `scope-expansion.json`; new rows remain traceable to their source sites and confidence.
