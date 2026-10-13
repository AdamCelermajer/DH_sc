# Mud Lord reference-gap correction

Reported issue: **ATLAS-GAP-001**, 2026-10-13. Integration owner: root.
Implementation owners: atlas_lua_gap_resolver, atlas_file_gap_resolver and root
(actor skill slots). Independent source reviewer: atlas_gap_review.

## Investigation

The user observed eight misleading entries under Recorded reachable gaps in
the Mud Lord browser dossier. The source ledger retained both nested literals
and enclosing Lua expressions; several enclosing expressions already had
proven derived targets while their canonical status stayed unresolved. Callback
keys, schema names, enum values and scalar audio parameters were also treated
as unresolved resources. Exact AI script and BDAE texture URI joins were absent.

Original consumers and admission conditions are recorded in
[Lua semantics](lua-semantics-report.md), [file paths](file-semantics-report.md),
[skill slots](skill-slot-links-report.md), and [resolution rules](RULES.md).
Existing B155/B156 Mud Lord gameplay evidence is reused. This is a static
development-index change with no original visual gameplay counterpart; it does
not change animation timing, callback dispatch, skill admission or audio output.

## Expected and implemented result

- `swampking_core` reaches the exact `data/scripts/ai/swampking_core.luac` URI
  and retains both Android and iPad receipts.
- `GetPyOID("AnimDict", "swampking_emerge")` reaches dictionary row 1321.
  Nested mentions of `swampking_fall` and `swampking_intimidate` are retained as
  classified duplicate source sites instead of additional missing resources.
- `OnDied`, `OnEndOfAnim` and four further exact Mud Lord event keys retain
  native callback evidence with a non-resource disposition.
- `AIStates.Skill=6`, target-list filter/sorting constants and registered
  CharacterProperties fields retain their own semantic namespaces.
- Literal sound control parameters are classified only at proven native API
  positions. Sound member names reach their exact source rows; missing downstream
  audio files remain independent gaps.
- All 12 literal Mud Lord `DoSkill` calls map through its own SkillTree list to
  rows 106–109. Shared scripts with different consumers remain ambiguous.
- Exact texture URIs retain platform receipts and a LIKELY producer association;
  raw BDAE strings alone do not establish runtime material use.

Every extracted reference ID remains in the ledger. Non-resource sites use
`NOT-NEEDED` with a meaningful reason and original-consumer evidence. Both partial
source caches remain eligible for exact file availability. No iPhone binary or
cross-platform native ID equivalence is assumed.

## Verification and remaining gaps

[Focused verification](data/semantic-verification.json) covers the user examples,
constant values, callbacks, source joins, cache-union fallback, idempotence,
missing rows/files, ambiguous shared-script slots, dynamic operands, object
receivers and iOS isolation. The current original-source audit and graph/mob
checks verify coverage and relationship endpoints. Independent regeneration
compares the generated artifacts. The previously passed generic 22-case checker
rejection report is retained; this change has additional focused failure cases.

The refreshed browser dossier is inspected after generation, including resource
navigation and the remaining Gaps tab. Gap labels identify Android versus iPad
sources: the same literal in an unverified iPad contract is a separate source
site, rather than evidence that the resolved Android resource is absent.
Dynamic expressions, unsupported native
contracts, missing source files and runtime conditions remain recorded gaps;
the correction does not suppress those unknowns.
