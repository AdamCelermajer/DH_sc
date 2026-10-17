# Ambush to Character Spawn composition slice

This host-only composition connects the existing Infected Village trigger and
actor registry modules to `actor-spawn-runtime`. The C++ driver imports the
supplied level/MGP/script cache, initializes the 26-record Character registry,
feeds outside/inside caller-provided player AABBs through the existing
approximate trigger adapter, and advances the checked Ambush scheduler. It
serializes the five actual scheduler-to-registry projection records. Python
then passes those exact ordered names to the existing
`apply_spawn_character_request()` logic and validates source Character
templates against the cached `Charater_Templates` table.

## Verified behavior

- The decoded source script projects five `SpawnCharacter` requests in this
  order: `_prim_tmp_infected05`, `_prim_tmp_infected07`,
  `_prim_tmp_infected17`, `_prim_tmp_infected06`, `_prim_tmp_infected16`.
- The four present source Characters are MGP records 6, 8, 7, and 19 from the
  module 01 record. Each carries `char_template="InfectedVillage_CommonType1"`,
  `char_template_pydata="Charater_Templates"`,
  `_templateName="MonsterCommonType1"`, `ai_state="Limbus"`, and
  `auto_spawn="0"`.
- The cached template's full alternative list is decoded and every property ID
  resolves to the source CharacterTable. The harness does not choose a random
  alternative or claim which model variant is active.
- Each Character constructor registers its state IDs 0–19 in its own
  `CharStateMachine` map. The cached IDs are `Limbus=0` and `Spawn=1`.
  The Ambush command resolves the existing Character and `SM_SetSpawnState`
  directly requests state 1; `_SetState` finds the per-Character mapping and
  calls `CSSpawn::OnFocus`. This projection reports a direct Limbus-to-Spawn
  state change for the four resolved static Characters.
- `_prim_tmp_infected17` has no source Character record, remains absent from
  the loaded-name list, and returns a no-state-change lookup miss in both the
  existing actor registry and Spawn-state projection.
- This is a host projection of the native command and state-map path. It still
  allocates no native Character or render object and does not execute native
  focus/blur callbacks, Spawn animation playback, timers, AI, or combat.

Trigger bounds/contact remain the existing approximate host proxy: the
recovered Zone local-box arithmetic is used, but final native world bounds and
`Zone::IsInside` behavior are still unresolved. The module 02 Ambush trigger
is outside this slice.

## Run

From the repository root, with the supplied `../cache/files` and a C++17
compiler available as `c++` (or set `CXX`):

```powershell
python -m unittest discover -s port/ambush-spawn-runtime/tests -v
python port/ambush-spawn-runtime/tests/run_host.py
```

The host runner compiles against the existing `trigger-contact`,
`script-runtime`, `actor-runtime`, world importer, and actor-spawn-runtime
decoder. It writes the generated validation evidence under this module's
ignored `build/` folder. Tests use no emulator or external service, modify no
existing component source, and do not copy the supplied cache into the repo.
