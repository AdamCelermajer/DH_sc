# Combat formula dependencies in owned script actors

The unchanged recovered `data/scripts/level/combat_formulas.luac` now runs with
the reconstructed offline `Rand`, existing property/equipment methods and three
additional actor projections: `GetState`, `GetHitCount`, `GetName`.

`SetCombatContext(state,hitCount,name)` is an authored control. It requires an
integral representable signed 32-bit state, an unsigned 16-bit hit count and a
1–255-byte string without NUL. All validation occurs before mutation; the name
is owned by the object's Lua environment. New diagnostic objects use state -1,
hit count zero and name `source character`. This is not an original Character
constructor, state machine, combo update or script callback.

The original state getter returns -1 without a current node, otherwise the
node's first integer. Hit count reads an unsigned 16-bit field. Name reads a
stored string pointer. Actual original fixture outputs are recorded in
`../random/differential-validation.json`; 147 original-emission versus owned
host Lua checks cover supported offline random and context outputs.

## Exact recovered formula execution

`../lua-runtime/tests/combat_corpus.py` loads all 26 validated integer constant
files and the exact 36,004-byte combat source (SHA-256
`f83639a12c5c1910f9a23fff494f4013330943ad4ef8f4d91c33fe1eb975565f`).
Expected results use a separate authored Python float32/integer calculation.

| Corpus | Cases | Checked values/counts |
| --- | ---: | ---: |
| Controlled damage/chance fixtures | 216 | 1,896 |
| All 446 real data records, both hands, melee/range/magic/DoT | 3,568 | 26,760 |

Both corpora pass normal host, strict ASan/UBSan/float-cast host, Android 17
x86_64 with 4 KiB pages and Android 17 x86_64 with 16 KiB pages. Every staged
file/list/runner hash is checked on each emulator: 39 controlled and 144 real
inputs. The rebuilt runner's existing property, class, item and gear selftests
also pass on host, strict host and both emulators.

Controlled cases cover stunned/sneak/combo damage, block/critical state, physical
and magical armor, minimum damage, elemental resistance/clamping, leech,
physical/skill DoT, both hands, skills and status/chance thresholds. They use
synthetic type-8 data to exercise reconstructed `SetProp`. Real-record cases use
empty gear/buffs without derived class application. They do not prove that
original Character construction produces the same aggregate combat state.

The original `CF_CalcDodge` live path references missing `CF__CalcDodge`.
The tests assert its error and subsequent recovery. The preserved original is
unchanged; no unverified replacement formula is installed.

Reports are `combat-{controlled,real}-{host,strict-host,android-4k,android-16k}-validation.json`.
Full original interpreter equivalence, combat dispatch/result application,
damage events, hit/state updates, buffs, AI and a game loop remain unfinished.
