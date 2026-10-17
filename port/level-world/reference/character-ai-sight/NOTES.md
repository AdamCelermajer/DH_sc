# Original AI sight callers

## Reuse and scope

This change reconstructs **two bounded caller orchestrations**, scalar
`AI_IsInSight(float)` (`0x3d4ea0`, 56 bytes) and object
`AI_IsInSight(GameObject const*)` (`0x3d4ed8`, 192 bytes). The five verified
original ranges are evidence, not five newly implemented functions. Ownership,
GetCharAI and GetTargetPosition producers remain explicit borrowed services.
No native/CMake wiring or current actor behavior is claimed.

Existing `dh2_ai_range` is a snapshot API, performs extra melee arithmetic and
squares Z before adding X+Y. It cannot supply this owner/point orchestration
or its exact source operation order. It remains unchanged. New numerical code
performs only the required separately rounded binary32 sight operations.

## Source ordering

The scalar overload captures current owner (`0x3d4ea4`), calls genuine
GetCharAI (`0x3d4eac -> 0x3a3024`), then reads returned row radius `+0x3c`
(`0x3d4eb0`). It squares that raw float once and returns canonical bool for
**square strictly greater than supplied squared distance**. No finite/radius
clamp is added. Negative radius squares normally; both zero signs, NaN and
infinity retain IEEE comparison behavior. Supplied distance may itself be
negative or NaN; the source does not reinterpret it as a coordinate distance.

The object overload captures its explicit target or, if null, AI `+0x40`
(`0x3d4f84`). If absent it returns false with no provider calls. Otherwise:

1. Capture current owner and call GetTargetPosition (`0x3d4ee8..0x3d4ef0`).
2. Call fixed target GetTargetPosition (`0x3d4ef4..0x3d4efc`).
3. Only then read the two returned live points for X, Y and Z subtraction
   (`0x3d4f00..0x3d4f28`). Neither point is copied before the target callback.
4. Square X; square Y; add X+Y; square Z; add Z (`0x3d4f2c..0x3d4f70`).
   Each operation separately rounds to binary32; no fused multiply-add.
5. Tail-call the scalar overload (`0x3d4f74..0x3d4f80`), which reloads owner.

The owner used for the original point query may differ from the owner used for
the scalar radius query after either position callback. Returned point pointers
remain fixed even if current point selection changes. Mutating their retained
coordinate storage during target callback affects subsequent source reads.

The actual GetTargetPosition leaf (`0x3935dc`, 36 bytes) selects object `+0x184`
only if word `+0x180` and visible byte `+0x80` are both nonzero; otherwise it
returns object `+0x160`. GetCharAI (`0x3a3024`, 48 bytes) captures the global
table before GetCharAIId and returns a `0x44`-stride row. GetCharAIId
(`0x3a2fec`, 56 bytes) owns fallback8. These owning producers are not duplicated
in this port module; their actual instructions execute in the component oracle.

## Lifetime and errors

Point/radius values are explicit projections, not original memory overlays.
One owning thread retains state, each owner/candidate, context and all returned
point/row storage through synchronous return, including retired storage after
pointer/table replacement. Providers may refresh owner/target and mutate stored
values; independent nested outputs are allowed. They may not overwrite this
Result/service table or destroy borrowed views during a call.

Providers return zero on successful invocation. Missing/error/throwing services
fail explicitly and preserve completed mutations. No rollback or cleanup is
invented. Top-level alignment/range/aliases reject before effects. Returned
points are validated together only after the second callback; invalid views
then fail explicitly. Result values/numeric diagnostics are meaningful on
completion; counts record attempted provider calls.

## Verification and soft-float boundary

Host tests cover strict boundary, negative radius/distance, zeros, NaN and
infinity, null fallback and skipped services, retained live point mutation,
fresh scalar owner, captured/replaced props row, and failures/alias/alignment.

The ARM oracle executes both original caller bodies and real GetTargetPosition,
GetCharAI/GetCharAIId leaf/table instructions. External `fsub`, `fmul`, `fadd`
and `fcmpgt` PLT/library dependencies are explicitly modeled with separately
rounded IEEE binary32 arithmetic; the original library implementations are not
available in the ELF and are not claimed executed. It records original operand
and operation order, comparing computed non-NaN words exactly and NaN outputs
by classification. **NaN payload/sign propagation is excluded.**

```powershell
python port/level-world/tests/run_character_ai_sight_host.py `
  --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe `
  --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so `
  --output port/level-world/build/character-ai-sight-review/host.exe `
  --report port/level-world/build/character-ai-sight-review/validation.json
```
