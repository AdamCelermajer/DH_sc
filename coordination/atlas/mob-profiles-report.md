# Joined mob and boss profiles

Status: generated lookup implemented; original behavior semantics remain partial.

The earlier coverage checkpoint classified every inventory row, including unknowns. It did not provide a complete joined dossier for every mob. This batch adds that navigation layer without changing any game behavior.

## Investigation

The original consumers are CharacterProperties model/animation/AI/skill/sound/loot fields, the recovered character-state and animation-step readers in `port/game-data/animation_tables.cpp`, skill and projectile table readers, native Lua table aliases and original script calls. Exact source references and native addresses remain in `RULES.md`, table rows and the reference ledger. CharSounds and loot consumer admission evidence are recorded separately in `mob-sounds-report.md` and `mob-drops-report.md`.

Existing B155/B156 footage notes provide the visible Mud Lord and Witch encounter context; this batch did not replay those sequences or claim new gameplay verification. A static lookup has no standalone original visual behavior. The invariant is that a caller can retrieve authored resources and their source consumers without confusing state sequences, clip dictionaries, effect sets, XML UIDs or table ordinals.

The expected result is one joined lookup per original CharacterProperties row, with exact AI/skill scripts and literal Includes, character animation states through sequence steps, effect/projectile dependencies, available audio from all reached producers, and nested loot possibilities. Missing bytes, dynamic script expressions, unknown schemas and runtime branch conditions remain visible. Shared-script resources are candidate associations, rather than proof that every branch runs for that actor.

## Implementation and verification

`atlas mob <name-or-character-row>` displays the generated actor profile. `--json` expands table records, reference rows, source files, all script calls and native function records. The source-backed alias `Mud Lord` selects SwampKing. All 448 character rows are retained, including NPC/player/default rows; the index distinguishes 59 source-flagged bosses, other mob AI rows and name-only boss candidates. Actor dossiers do not extend the separate per-asset detail-page policy beyond Act 1 and its next act.

Dependency walks use row identities and resolved targets with visited sets, including references inside reached source assets. Every cache variant returned by the common resolver is preserved with source path and SHA256. A sounds section gathers character vectors, animation-step and script-produced audio; Default's empty character vectors do not hide animation audio. Drop graphs retain raw roll/weight/quantity fields, item-list entries, item-power lists, cycles and missing targets, without deriving unconditional probabilities.

Focused checks cover original Default/ArmoredHuman sound vectors and byte boundary; SwampKing/Witch/lizard script and loot joins; iOS serpent attack audio; model texture dependencies; malformed cyclic/dangling loot graphs; and truncated sound bytes. `data/mob-verification.json` stores the results. Checker rejection tests also remove a mob profile, a profile section and a CharSounds reference to prove those omissions fail. Independent review is retained in `mob-join-review.md`.

No game executable, gameplay feature, user window or save was changed or tested. Integrated verification here means the real generated atlas and lookup command, not integrated game behavior.

## Remaining work and ownership

Root owns unresolved profile edges and native/script semantic investigation. Profiles explicitly remain PARTIAL: not all dynamic Lua values are resolved, not all native contracts are reviewed, scene/encounter placement is incomplete, some source payloads are undecoded and source files are absent from both partial caches. The JSON `complete` field is false for every profile; zero profiles are certified as containing every original behavior rule. Continue from the named profile gaps rather than treating the processed inventory percentage as semantic completion.
