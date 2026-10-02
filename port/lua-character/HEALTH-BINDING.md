# Health and mana on owned script actors

`DH2CreatePropertyState(row)` now supports the original-matched health/mana
projection. Each actor retains its property dataset generation after import
replacement. These are owned diagnostic actors; original Character creation,
buff ownership, combat/death/events, AI and the game loop are unfinished.

## Recovered script methods

| Method | Returns / amount |
| --- | --- |
| `GetHP(...)` | Three values: whole current HP, whole maximum HP, integer percentage; arguments ignored |
| `RegenHP(raw, ...)` | No result; first number truncated to raw signed fixed amount |
| `RegenMP(raw, ...)` | Same for mana |
| `HasMana(raw, ...)` | Boolean for signed raw cost versus current raw mana |
| `UseMana(raw, ...)` | Boolean; successful normal cost subtracts through original property routing |

The original five callback bodies generate expected values and complete final
sheets for the checked script corpus. No original interpreter executes.
Numeric input must be finite and in the signed 32-bit conversion range;
undefined original conversions reject. Missing/non-numeric first arguments on
regen/mana methods yield no result. At most 32 supplied arguments are accepted.
The bridge uses offline ordinary mana policy, without exemptions.

## Additional source controls

`SetHP(whole)`, `SetMP(whole)`, `ValidateHPMP()`, `GetMP()`, `GetTotalHP()`,
`GetTotalMP()`, `GetHPFraction()` and `GetMPFraction()` expose reconstructed
native bodies through authored Lua method registrations. The names/registration
are not claimed recovered script API. Set accepts a finite signed number,
truncates to whole integer, then shifts eight bits with wrapping. Validation
caps above maxima, retaining negative values. Fraction getters return a ratio.

```lua
local actor = DH2CreatePropertyState(2)
-- Use appropriate real property data for maxima, or a controlled fixture.
actor:SetHP(20)
local hp, maximum, percent = actor:GetHP()
actor:RegenHP(256) -- one raw fixed-point HP unit
if actor:HasMana(256) then
    assert(actor:UseMana(256))
end
```

The controlled corpus checks 520 actors / 116,480 final fields. Real corpus
checks all 446 character rows / 99,904 final fields, with original setters,
validation, regen, mana calls and getters. Host, strict sanitizers and both
Android 17 page sizes execute the same assertion content. See the health
validation reports and [native scope](../character-health/README.md).

Regen negative amount requests filling to maximum, with original signed wrap
and comparison behavior. Negative mana costs preserve the original increase
behavior. `GetHP` returns three zeros for raw maximum zero; unsafe shifted-zero
or overflow division rejects with actor state retained. Native fractions use
IEEE infinity/NaN classes for zero maxima. This adds no automatic damage
dispatch, health bar, resurrection or save integration.
