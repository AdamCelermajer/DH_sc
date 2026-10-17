# Authored encounter persistence

This saves the source development encounter, not the original game's `.savegame`
format, campaign, inventory, rewards or arbitrary script state. Original cache
and saves are neither read nor overwritten.

## Ownership and lifecycle

`GameplayActivity` keeps simulation and Lua checkpoint creation on its GL owner
thread. Each successful visible frame publishes an immutable checkpoint under
`stateGuard`. Pause gates new steps and hands the last completed checkpoint to a
process-wide save writer; it performs no Lua calls or disk writes on the UI
thread. Resume resets the simulation clock on its owner thread. A native session
token rejects callbacks belonging to a replaced Activity session.

Activity/process recreation creates a fresh runtime and restores a complete
candidate before stepping. EGL recreation in the same Activity rebuilds graphics
resources and keeps the CPU session. Assets are retained privately by each native
session so candidate restore/reset never depends on expired JNI array pointers.

## File format

Files are under private app storage `files/source-encounter-saves/`, in two slots:
`encounter-a.dh2s` and `encounter-b.dh2s`. All fields are explicitly little endian.
The maximum payload is 8192 bytes; the present two-enemy payload is 3096 bytes.

The Java envelope is 32 bytes:

| Offset | Field |
| --- | --- |
| 0 | `DHF1` |
| 4 | Format version, uint32 = 1 |
| 8 | Monotonic positive sequence, int64 |
| 16 | Payload length, uint32 |
| 20 | IEEE CRC32 of the complete native payload |
| 24 | Reserved uint32 = 0 |
| 28 | IEEE CRC32 of the preceding 28 header bytes |

The native `DH2S` payload begins with a 64-byte header:

| Offset | Field |
| --- | --- |
| 0 | `DH2S` |
| 4 | Native schema version, uint32 = 1 |
| 8 | Total payload length, uint32 |
| 12 | IEEE CRC32 of the body after byte 64 |
| 16 | 32-byte SHA-256 content generation supplied by the Activity |
| 48 | Fixed area identifier `0x43525331`, development cross junction |
| 52 | Authored definition revision, uint32 = 1 |
| 56 | Enemy count, uint32, 1..3 |
| 60 | Reserved uint32 = 0 |

The SHA-256 covers the explicit encounter revision string and ordered,
length-delimited room, texture, hero, motion, property, class, loot, power, quest,
constant and original combat-script bytes. Changes to orchestration/balance must
advance the authored revision string and native definition revision.

The body contains 20 bytes of `DHE1` binding metadata, one 980-byte named actor
record per player/enemy, then a 72-byte quest/RNG/counter tail. Actor records store
zero-padded 32-byte IDs (`player`, `sentry_a`, `sentry_b`, `sentry_c`), float32
x/y/heading/remaining cooldown, and the 932-byte exact actor state. Record order
does not define identity; restore resolves each name and rejects duplicates.
Identity is scoped by the fixed area and exact asset/definition generation.
The original selected quest row/objective indices are consistency metadata
within that generation, not an asserted original stable quest ID or a migration
mechanism.

`DHA1` actor state stores 224 raw int32 saved property values, combat state and hit
count, and exact death metadata. Candidate actors reload their profile/base and
recompose final properties. Schema 1 permits only saved HP to differ from the
fresh encounter definition: equipment, class changes and other gameplay need an
explicit new schema/validation policy. Names/profile metadata are reconstructed
from the definition. `DHQ1` stores exact quest progress and record context;
`DHR1` stores both uint32 RNG seeds and both counters. No integer passes through
the float32 Lua numeric bridge during checkpoint transport.

No input-held flags, elapsed wall clock, renderer handles, pointers, transient
attack/hit effects or derived HUD cache are serialized. Gameplay cooldowns are
preserved; time spent outside the foreground does not advance the simulation.

## Validation and atomic replacement

Native restore validates version, length, checksum, generation, area, spawn IDs,
duplicates, finite transforms, floor membership, cooldowns, properties, death
identity and quest/event consistency. It creates a separate source runtime and
imports actor/quest/RNG state without replaying kill events or completion/loot
requests. Only a fully valid candidate replaces the running session. Reset uses
the same candidate construction rule. A failed step/checkpoint is never saved.

One executor coalesces position writes to roughly one per second and makes combat,
reset and lifecycle checkpoints urgent. It syncs the temporary file then uses a
same-directory atomic rename; unsupported atomic replacement fails visibly. The
other slot remains available. Slot selection uses validated sequences; corrupted
or semantically rejected files are retained as `.rejected-<uuid>` when overwritten.
An owner/run epoch prevents queued earlier snapshots replacing an acknowledged
reset or a newer Activity session.

Loading tries the newest complete compatible checkpoint, then the preceding slot.
If none can restore, the fresh preview disables autosaving and asks the user to
use Reset to begin a new saved run, preserving existing files. `Progress saved.`
is shown only after the writer acknowledges the current checkpoint; errors show
`Progress not saved:`. Sudden power-loss behavior has not been physically tested.

## Focused checks

`port/android-app/tests/EncounterSaveStoreTest.java` runs against the actual host filesystem:
checksums/truncation, atomic slot writes, fallback, damaged-file retention, stale
owners, reset/write queue ordering, and write failure preserving the prior slot.

`port/android-app/tests/gameplay_persistence.cpp` plus
`port/android-app/tests/run_gameplay_persistence.py` execute
the actual native session with original gameplay inputs on an Android x86_64
runner. They cover exact mid-combat restore, shuffled spawn records, every-byte
corruption/truncation, semantic rejection preserving the complete live session,
continued HP/quest/RNG/cooldown equivalence, victory, reset and defeat. Its floor
predicate is deliberately simplified; APK/emulator UI checks must separately
exercise the real original room geometry, lifecycle and cold restart.
