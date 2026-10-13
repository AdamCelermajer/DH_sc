# Atlas method and verification ownership

Scope is read-only investigation of original data and frozen port source. No game
behavior, game tests, CMake, packages or saves are changed by this branch.

The original consumers are Lua native bindings, PyData table readers, the level
loader, BRES event tracks and the audio source binding layer. The visible invariant
is that an authored animation, effect, projectile or sound reference can be traced
through its source rule to the exact original file or a recorded missing asset.

Existing visual evidence is reused from `coordination/v21.7/B155-B156-HANDOFF.md`
section 5: Part 1 18:12–20:48 (Mud Lord); witch sequence 966–1000 seconds. Those
notes distinguish observations from player/faery effect inferences. This atlas
does not implement animations or claim to have replayed those sequences. New
gameplay work must verify behavior in its production session before acceptance.

Logic evidence for the extra-site extractor: `port/engine-animation/event_track.cpp:6`
loads the root+0x2c serialized event track; the record stores type, count, time
array, and eight-byte groups with string pointer lists. `port/engine-resources/resources.cpp:159`
validates BRES serialized offsets. `port/windows-foundation/features/audio/canonical_source_audio_adapter.cpp:23`
strips exactly the first `sfx_` prefix when submitting an event's named sound.
The Python reader retains each key/group index and offset separately, including
duplicate names. Non-event binary file strings retain LIKELY consumer confidence.

Before implementation, verification was defined as: enumerate the source corpus,
record every extracted source-site ID, regenerate deterministically, independently
re-extract to detect omitted sites, reject missing statuses and malformed unresolved
rows, check original known answers, and exercise checker failure branches by deleting
or corrupting generated rows. A helper/source match cannot establish playability.

Root owns inventory, portable runtime, orchestration, checker, lookup and report.
The script, reference and act categories have separate owners and count files;
root aggregates their counts into PROGRESS at every commit. No concurrent writer
owns PROGRESS. No push is authorized. Source files and evidence retain paths and
hashes; original assets remain untracked outside the atlas worktree.

Uncertainties include incomplete supplied caches, platform differences, dynamic
Lua values, imported helpers, and provisional act boundaries. Terminal UNRESOLVED
is investigation coverage, not reference resolution or gameplay completion.
