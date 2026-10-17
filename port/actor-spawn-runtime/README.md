# Character-template and Spawn-state slice

This host-only slice decodes the cached `Charater_Templates` array, preserves
each template's ordered `CharInfo` alternatives, and maps an explicitly chosen
alternative to the matching `CharacterTable` name. The caller supplies the
alternative index; this code does not reproduce the native RNG or pick a
variant on its own.

It also models the observed `SpawnCharacter` command path for one exact,
already-loaded Character name. A successful lookup requests the `Spawn` state
from the cached `AIStates` constants. A missing or ambiguous name leaves the
state unchanged. This is the state-setter boundary, not actor creation or a
complete state machine.

## Evidence

The cache inputs are read in place:

- `character_templates_pyarray.bin` and its `pyarraynames` sidecar.
- `character_properties_pyarraynames.bin` for validating and naming each
  `CharInfo` value. This file concatenates three tables; its first table is the
  `CharacterTable` name list, followed by the stat scheme and stat list names.
- `ai_pycst.bin` for the numeric `AIStates` IDs.

The recovered ARM32 reader evidence is pinned by source symbols and checked by
the host runner. ELF addresses below are the preserved assembly addresses;
decompiler listings add `0x10000` to them:

- `Arrays::Charater_Templates::read`, ELF `0x4b3f5c`, reads an array count and
  invokes each 12-byte array record's reader.
- `Structs::CharTemplate::read`, ELF `0x4ccd38`, reads `CharInfoSize`, allocates
  that many 8-byte entries, and invokes each entry reader.
- `Structs::CharInfoName::read`, ELF `0x4f1c90`, reads one signed 32-bit value
  at record offset `+4`.
- `Character::SafeGetCharPropsTemplateId`, ELF `0x3b36ec`, resolves the
  template's name against `Charater_Templates`.
- `Character::SafeGetCharPropsId`, ELF `0x3b3d38`, uses the native random
  generator to pick a `CharInfo` entry, stores its value as the Character
  properties ID, and asserts the choice is below `CharInfoSize`.
- `Script_SpawnCharacter::Execute`, ELF `0x45f400`, looks up the named loaded
  Character and calls `SM_SetSpawnState(false, false)` only when found.
- `CharStateMachine::SM_SetSpawnState`, ELF `0x3c2734`, takes the direct
  `_SetState(1, ...)` branch for the false argument case. `AIStates.Spawn` in
  the supplied cache is also ID 1. The Character constructor at ELF
  `0x3a9340` registers IDs 0–19 in that Character's embedded state machine
  through `RegisterState` at `0x3c7318`. The factory table returns shared
  singleton state objects; each Character keeps its own ID-to-state mapping.
  `_SetState` checks that mapping, blurs the old state if present, stores the
  target, calls its `OnFocus`, then raises state-change event `0x1d`. For a
  normal Character, state 1 is registered and the Ambush request reaches
  `CSSpawn::OnFocus`. If a target state is absent, the setter clears current
  state, skips target `OnFocus`, and raises the event; it does not assert.
  Assembly checks in the host runner pin the per-Character 0–19 registration
  loop and the registered-state factory lookup.

The loaded-actor list and caller-provided template alternative index are
explicit inputs to this projection. It does not emulate the name-based native
object manager, actual callback execution, animation playback, timers, AI
decisions, rendering, or the original game loop. The assembly and cache
evidence does not prove which random template alternative is selected for a
specific actor.

## Run

From the repository root with Python 3:

```powershell
python -m unittest discover -s port/actor-spawn-runtime/tests -v
python port/actor-spawn-runtime/tests/run_host.py --cache ../cache/files
```

The runner checks the supplied template count and every property ID against
the table-name count, checks all variants of
`InfectedVillage_CommonType1`, reads `AIStates`, verifies the native
`Limbus -> Spawn` request and miss/repeat/malformed cases, and rejects truncated
or trailing cache data. It writes a provenance report under the ignored
`build/` directory. No device, emulator, Drive, or external service is used.
