# Character health and mana projection

This portable C module reconstructs health and mana operations on the owned
four-sheet property state, with empty buffs. It does not construct an original
Character or connect combat, death, events, AI or gameplay.

## Recovered behavior

| Original ELF entry | Operation | Source behavior |
| --- | --- | --- |
| `3bd0c0`, `3bd0d0` | SetMP, SetHP | Whole signed integer shifted left eight bits with wrapping; original property routing |
| `3bd0e0`–`3bd110` | Total/current MP/HP | Arithmetic right shift eight bits of the final value |
| `3bd140` | ValidateHPMP | Signed upper clamp for HP, then MP; no lower clamp |
| `3bd2dc`, `3bd338` | Percent getters | Float32 raw current divided by raw maximum; ratio, not percentage times 100 |
| `3bdbb8`, `3bdca4` | RegenMP, RegenHP | Raw fixed amount; negative requests maximum; wrapping sum/cap comparison; positive adjusted amount uses PROPS_Add |
| `3b6cd8` | Script GetHP | Three integer returns: current whole value, maximum whole value, wrapped product/division percentage |
| `3bd40c`, `3bdef4` | HasMana, UseMana | Signed raw cost check and wrapping subtraction through PROPS_Add; explicit exemption boundary input |
| `3b7774`, `3b77ec`, `3b7864`, `3b78ec` | Script regen/mana callbacks | First numeric argument is truncated to signed raw integer; missing or wrong-tag first argument returns nothing; trailing arguments ignored within the source limit |

Health is field 36, maximum health 38, mana 41, maximum mana 43. Routing uses
the existing original-matched property state: type 32 writes saved/recalculates,
type 8 writes final, other types do not write. No new zero clamp is introduced
by validation. Negative native mana cost remains capable of increasing mana.

Exemptions represent decisions made outside these property operations. The
original HasMana may exempt an online object through its virtual method;
UseMana additionally checks configuration, debug switches and a Character flag.
The source module takes an explicit boolean decision. The Lua bridge uses
offline normal policy without exemptions. Network/configuration ownership is
not reconstructed by this module.

Invalid arguments, aliases and malformed tables reject without committing
mutation. Script HP division rejects a nonzero raw maximum whose shifted whole
value is zero, and INT_MIN/-1 overflow. Original undefined paths are not claimed
equivalent. Zero-divisor native fractions preserve IEEE infinity/NaN classes;
the NaN payload is not an equivalence claim. Buff-inclusive behavior is outside
this API's current scope.

## Evidence

- `differential-validation.json`: 4,156 actual original ARM32 / compiled source
  ARM64 / host comparisons, zero mismatches. All four sheets, surrounding
  reserved fields, source output guards and stack restoration are checked.
- Actual cache routing and synthetic types -1..63; signed/wrapping shifts,
  validation, regeneration, mana policy decisions, numeric/tag handling and
  captured script return values are exercised.
- `host-build-validation.json`: 12,000 safety iterations with address,
  undefined behavior and float-cast-overflow sanitizers; recovery disabled.
- `android-build-validation.json`: source ARM64 library with 16 KiB ELF layout.
- `../lua-runtime/health-*-validation.json`: original-derived script assertions
  on 520 controlled cases and all 446 real character rows, checking 216,384 final
  field queries per host/strict host/Android 17 4 KiB/16 KiB run.

Original property readers and mutation bodies execute. Stream allocation/copy,
debug Load/string lifetime, online/config/virtual decisions and ReturnValues
pushes are explicit fixtures. Imported float32 arithmetic, finite conversions
and signed division use stated arithmetic models. Original Lua VM and Character
construction do not execute. Real row initialization uses the previously
checked owned property loading/composition, followed by original health calls.

## Reproduce

Build on Linux/WSL:

```sh
python3 port/character-health/build.py --host \
  --report port/character-health/host-build-validation.json
```

Build ARM64 with the Windows NDK:

```powershell
python port/character-health/build.py --ndk PATH_TO_NDK --report port/character-health/android-build-validation.json
```

Run the comparison under the Unicorn/pyelftools test environment:

```sh
python port/character-health/tests/differential.py \
  --original PRIVATE_ORIGINAL_SO --oracle port/skin-payloads/build/oracle.so \
  --cache PRIVATE_CACHE_ROOT --host port/character-health/build/health-host.so \
  --arm64 port/character-health/build/health-arm64.so \
  --report port/character-health/differential-validation.json
```

The private original/cache inputs are supplied separately. See
[script binding](../lua-character/HEALTH-BINDING.md) for ownership and method scope.
