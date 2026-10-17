# Recovered combat calculations in the Android 17 source preview

The tested source APK is **840,611 bytes**, SHA-256
`122af9795982829d6f15e7c45a417824b465461d6d9f9cfd660cbb3bbe55c795`.
Target SDK 37, minimum 26; source ARM64/x86_64 libraries have checked 16 KiB
ELF/ZIP alignment. APK signatures v2/v3 verify. No original engine or ARM
translator is packaged. Full source gameplay remains unfinished.

## Import and use

The app now has **Import script constants**, with a 4 MiB file limit and an
owned atomic integer constant merge. Import these owner-cache files:

1. **Import character properties** — `data/pydata/character_properties_pyarray.bin`.
2. **Import item data** — `data/pydata/loot_table_pyarray.bin`.
3. **Import script constants**, twice — `data/pydata/ai_pycst.bin` and
   `data/pydata/design_pycst.bin`.
4. **Import script source** — a diagnostic plaintext Lua chunk.

Class/power imports and gear update methods remain available. Combat formulas
are already initialized from the unchanged packaged recovered source.

```lua
local a = DH2CreatePropertyState(2)
local d = DH2CreatePropertyState(3)
a:SetCombatContext(0, 0, "attacker")
d:SetCombatContext(0, 0, "defender")
DH2SeedRandom(0)
CF_ClearCombatants()
CF_SetCombatants(a, d, GetPyCst("Elemental", "none"), false, false)
local damage = CF_CalcDamage(0, GetPyCst("CombatAttackTypes", "Melee"))
assert(type(damage) == "number")
CF_ClearCombatants()
```

The import status reports execution success; this does not damage a rendered
character or start gameplay. Seeding/context setters are authored controls.

## Exact installed package checks

Both Android 17 x86_64 emulators (4 KiB and 16 KiB pages) pass:

- Installed APK pulled back with the exact build hash; packaged combat source
  hash matches the 36,004-byte original.
- 26 imports in the same process: actual AI/design constants, two property/item
  generations and 80 selected real-record damage cases plus all 216 controlled
  combat cases. Assertion chunks match separately checked host files.
- Stunned/sneak/combo damage, block/critical state, elemental resistance, leech,
  DoT, skill and chance/status thresholds, both hands and minimum damage.
- Retained owned actor context/name; malformed and oversized constant rejection;
  replacement with retained other groups; restoring original constants; recovery.
- Source warrior mesh/texture/25-track walk; eleven visible import/play buttons,
  with no clipping. Both ready screenshots were inspected.
- Same APK two-motion import, midpoint seek, paused 0/50/100% poses, advancing
  playback and stable pause. All six paused mix screenshots were inspected.
- No fatal signal or exception in either run; process retained across imports.

Reports: `combat-{4k,16k}-runtime-validation.json` and
`combat-animation-runtime-validation.json`. The complete runner corpora cover
3,568 real cases / 26,760 checked values/counts and 216 controlled cases / 1,896
checks on host, strict sanitizers and both Android page sizes; see
[binding scope](../lua-character/COMBAT-BINDING.md) and
[random instruction comparison](../random/README.md).

The exact original dodge wrapper references a missing helper when combatants
are set. The checks expect that error; the original file remains unchanged.
Original Lua/game equivalence, Character construction, damage/result application,
buff/state/combo updates, events, AI/navigation, streaming, progression, audio,
saves and full source gameplay remain unfinished. ARM64 hardware and Fold7 were
not tested. The playable Test11 packages still use the original ARM32 engine
through translation; they are a separate compatibility checkpoint.
