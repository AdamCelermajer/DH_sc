# Character template selection and caller-owned RNG state

This small adapter models the uncached NPC template-slot draw in
`Character::SafeGetCharPropsId`. The engine's RNG kernel already exists at
[`port/random/random.c`](../../../random/random.c), so this source does not copy
the LCG. It calls that kernel with the ordinary-stream selector and passes the
resulting slot index back to the separate template resolver.

## Source path and ownership

In original `Character::SafeGetCharPropsId` (ELF `0x3b3d38`), the non-player
template branch first checks whether a property ID is already cached. If it is
uncached, it resolves the named template. Only when `CharInfoSize > 0` does it
advance `Random::s_seed` using the original recurrence, take the remainder by
the alternative count, and increment `s_debugRandomCounters[0]`. The resulting
index is then used to read that ordered `CharInfo` slot. An empty template does
not draw or increment the counter. The routine's player/savegame path is outside
this adapter.

The same ordinary-seed LCG is reconstructed and differentially checked in
`port/random`; its second `bool` parameter selects the independent synchronized
seed/counter pair. `SafeGetCharPropsId` uses the ordinary pair directly. The new
adapter therefore calls `dh2_random_next(streams, slot_count, 0)`: this advances
only `seeds[0]` and `counters[0]`, once for each positive count. It leaves the
synchronized pair untouched. The existing `port/game-data/CombatRandom` is a
separate input-owned combat calculation stream and is not used here.

Seed ownership stays with the caller. The original startup state machine
`GSInit::Update` and `Level::Unload` assign `Random::s_seed` from
`glitch::os::Timer::getRealTime()` and set `Random::s_syncedSeed` to zero. The
static initializer for `Random.cpp` resets the two diagnostic call counters;
it does not select a game seed. The exact timer value and all intervening draws
before each Character constructor are runtime order facts. This module neither
calls the clock nor creates a process-global RNG state.

Because gameplay systems share the ordinary stream, a caller seed by itself is
not enough to claim which of the four template-based Crypt ambushers resolves to
`Crypt_Ghost_RE`. Their exact variants also depend on earlier ordinary-stream
draws and construction order. The test runner requires an explicit fixture
seed and labels it test input; its chosen slot is deterministic test output,
not an inferred original-game variant.

## Implementation and test

[`character_template_random.hpp`](../../character_template_random.hpp) and
[`character_template_random.cpp`](../../character_template_random.cpp) expose
`select_uncached_slot`. It rejects null/overlapping state and output and
negative counts before mutation, and reports no alternatives without drawing
for a zero count. The caller owns stream initialization and must call only for
an uncached template selection. The slot resolver remains responsible for
mapping that index to the selected `CharInfoName` property ID.

The C++ host fixture is
[`character_template_random.cpp`](../../tests/character_template_random.cpp).
The runner cross-checks the real Crypt cache template and all four MGP runtime
keys, compiles this adapter with the already reconstructed RNG kernel, and runs
source-recurrence, ordinary/synchronized-stream isolation, zero-slot, counter
wrap, null, alias, and invalid-count cases:

```powershell
python port/level-world/tests/run_character_template_random_host.py --cache ../cache/files --seed 123456789
```

The seed argument is required and only supplies a reproducible fixture state.
The runner also pins the existing original-vs-port evidence for
`Random::GetRandom` (`port/random/differential-validation.json`: 768 core
comparisons, zero mismatches). It does not invoke the Android application or
reproduce the original per-actor draw order.
