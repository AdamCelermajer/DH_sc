# Integrated atlas verification

## Original consumer evidence and scope

This change builds a static development index and local viewer. Original game
behavior is unchanged. The source rules and native consumers are recorded in
[resolution rules](RULES.md), [mob sound evidence](mob-sounds-report.md),
[loot evidence](mob-drops-report.md), [campaign links](campaign-links-report.md),
[UI text and script evidence](ui-text-index-report.md), and
[package provenance](package-index-report.md). Existing boss investigations and
the v21.7 Mud Lord handoff remain the gameplay evidence; an atlas lookup does
not certify animation timing, runtime branches, damage, sound playback, or drops.

## Implementation and isolated checks

The universal graph preserves original row IDs, file hashes, source versions,
forward dependencies, reverse consumers, and unresolved source sites. All
8,863 Android inventory table rows have decoded payloads. iPad names and UI
scripts retain their own namespaces; no native iPhone contract is assumed.
Unknown packed payloads, opaque script carriers and missing files remain visible.

The generated [graph checks](data/graph-verification.json) cover Mud Lord,
BashDown, GroundSlam, Charge, Bow01, Bow03, Potion0 and every explicit item
resource join with an indexed target. They also exercise missing entities,
duplicate IDs, dangling links, missing reference owners/targets and profiles.
The [checker rejection tests](data/validation.json) exercise the 22 existing
failure cases against private data copies. The main source audit and all ten
known-answer checks pass, including 45 distinct missing audio filenames.

The local viewer's six protocol checks verify exact WAV bytes, readable original
Lua, the recorded iPad Lua decoder, unknown asset rejection, document-path
boundary rejection and write rejection. Their result is retained under the
ignored runtime directory; run `server_checks.run(ctx)` against `atlas serve`.

## Integrated browser verification

The real local viewer was inspected in the Codex in-app browser. The Mud Lord
shortcut selects CharacterProperties row 358 and displays its source model,
animation sequences, effects/projectile, core/skill scripts, sound records and
loot root. Resource buttons open the corresponding typed records. BashDown
displays its English name Headsplitter and reaches animation 347, effect 164,
sound 190 and its WAV source. Potion0 displays Potion and reaches its audiovisual
row, Android/iPad itemdrop variants, drop/pickup sounds and reverse loot owners.
The WAV page renders audio controls; source protocol checks verify its bytes.
Audibility and gameplay output were not tested.

Search orders exact aliases ahead of incidental mentions and semantic entities
ahead of raw text. Raw references remain available by category. Source links and
reverse usage were verified on the core script and item views. The viewer keeps
raw records collapsed, bounds large result/closure lists, and exposes gaps.
Browser testing caught and repaired incorrect shortcut ranking, an omitted actor
walk, resource buttons missing from the DOM, and a description used as a title.

## Regeneration and remaining limits

`atlas cleancheck` regenerates into an empty private output and compares every
artifact byte for byte. Its final result is retained at
`tools/atlas/.runtime/cleancheck-result.json`; it is not a gameplay test.
Dynamic Lua/native contracts, iPad loader admission, unparsed payload formats,
opaque UI script condition blocks and provisional act boundaries remain explicit
follow-up evidence work. Source indexing is complete for the declared corpus;
runtime meaning is not inferred from file presence or terminal work status.
