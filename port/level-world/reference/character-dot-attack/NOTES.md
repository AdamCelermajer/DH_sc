# Self/self DoT calculation and application coordinator

This reconstruction is source-only. Original ELF SHA-256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` and the accompanying assembly preserve the captured
routine bytes. The new native API does not allocate a Character, combat owner,
timer, second DoT property sheet, or hidden Application singleton.

## Calculation entries and timer composition

`Character::F_DotAttack` at `0x3b2e68` performs an attack tracing load/query,
then `_F_CalculateResult` at `0x3b2638`, with mask `0x20080000`, category `-1`,
the supplied element and raw amount. It does **not** apply damage itself.

`dh2_character_dot_calculate` reproduces that standalone wrapper.
`dh2_character_dot_calculate_result` enters at CalculateResult for the frozen
TimerEffect `effect_dot_calculate` service. TimerEffect has already delivered
the wrapper's first tracing pair; using the standalone entry there would add
two calls that the original does not make.

CalculateResult orders profile begin, AttackResult reset, mask/category/element
writes, CF_SetCombatants (`0x3b059c`), a second tracing pair, direct damage, and
profile end. Both profile calls use `Character::_F_CalculateResult` at original
`0x8c3d38`. The source reset (`0x3af3b0`) sets amount and all three DoT words to
`-1`, HP/MP leech and outcome bits to zero. The two source level19 getters see
the same retained cached sheet in this self/self domain, so both deltas are
zero; the supplied shared DotCombatContext32 retains both actor identities,
element and zero offhand/magic/blocked/critical fields.

Mask bit19 selects CF__CalcDamage (`0x3b1fb8`) type3. It copies the **raw**
amount. There is no positive DoT rate, elapsed-time, repeat, duration or tick
scaling, no RNG draw, and no equipment lookup in this branch. The implementation
executes existing genuine CombatResult/Damage kernels, not a replacement
constant result. TimerEffect rereads its owner before ApplyResult; application
can therefore receive a different retained actor than calculation used.

## ApplyResult's bounded source domain

`dh2_character_dot_apply` executes self/self `F_ApplyResult` (`0x3b10b4`) for
the unmodified direct DoT result, offline nonplayer actors and party count<=1.
Profiles, Application/player/session queries and Character/AI/FX services are
explicit synchronous providers. Missing providers fail at the reached prefix.
The native contract requires stable receiver/backing lifetimes and no callback
mutation of the input AttackResult. Actor byte/halfword fields and property
values stay live, including the network ID reread after AddAggro.

1. Query online state (`0x7fd794`); unsupported online branch is reported here.
2. Increment attacker combo halfword `Character+0x14d0`, wrapping at65536.
3. Load/query `NoDamages` (literal `0x8c3ca8`). If false, load/query `GOD`
   (`0x8c3cb8`), then query Application's same key if the debug value is false.
   True GOD queries IsPlayer before the next byte check. The actor's **raw
   +0x14f0 byte independently suppresses damage even if both GOD queries were
   false** (`0x3b152c..1538`). NoDamages skips all of those later checks.
4. Positive permitted amount reads actual Game party count+0x6c4. Count>1
   reaches an explicit unsupported co-op scaling boundary; no default factor.
5. GetEffectiveThreatPerDamage (`0x3bd394`) reads cached property204. Execute
   separate f32 `float(property204)/256`, `float(amount)/256`, then multiply.
   Deliver actual AI_AddAggro (`0x3d7c68`) with the self identity and threat.
   A positive returned float triggers load/query `isTracingThreatChange`
   (`0x8c3ce8`); NaN is not positive.
6. Clear push-death byte+0x53b. **HitFor runs only when live network_id+0x110
   equals -1**, after another attack tracing pair. Other network IDs skip the
   hit but continue the later dead/regen/FX calls. No implicit remote damage.
7. Query live IsDead, call RegenHP(0), RegenMP(0), then query IsDead again.
   The second query still happens if the positive damage block was skipped.
   A living target queries IsPlayer; its native state-reaction branch is an
   explicit unsupported boundary. Negative/zero amounts still execute the tail.
8. Always call CancelSneaking (`0x3bc6b8`), scrolling combat text (`0x3af77c`),
   and combat sound (`0x3afee0`). Bit29 suppresses the two AI result callbacks,
   **not** these services. Then query target IsPlayer and attacker IsPlayer
   separately; either player stat/achievement branch remains unsupported.

There is no self-identity rejection in captured AI_SetAggro (`0x3d79ec`). Its
player/dead gates, outgoing and reciprocal incoming maps and first-insertion
OnAggro callback remain genuine AI ownership dependencies, not a no-op rule.

## Proof and remaining backend ownership

The original/O2 ARM64 audit has1800 comparisons, split900 wrapper and900 inner
entry,34228 ordered requests,264 reached HitFor service requests,8 native atomic
guards, and175 explicit original-prefix-only unsupported cases. CalculateResult,
reset, SetCombatants, direct damage and cached getters run original instructions.
Full supported ApplyResult control flow runs; its required external bodies are
labelled fixtures. There are no skipped ApplyResult blocks in supported cases.
All result words, shared context fields, actor fields, sheets and requests are
compared. Signed extremes, combo wrap, raw GOD bytes, zero/negative damage,
network rereads and negative/zero/infinite/NaN returned aggro values are covered.

The sanitizer DSO audit replays the same gold,361946 checks/8 atomic guards/689
failure-prefix cases. Separate compositions use the actual central debug map
with real fopen ENOENT, actual self/self aggro tables seeded with an existing
entry (so no missing OnAggro callback is accepted), actual health/property
kernels and actual TimerEffect. Three genuine health prefixes include lethal
and surviving hits; the callback deliberately fails after the health prefix
because the remainder of HitFor is unbound. One real timer34 composition
preserves the HP write and fails at Apply, with no manufactured successful tick.
Runtime HP is correctly stored in the saved sheet with source default base HP;
the test does not put runtime HP into both base and saved sheets.

Actual full HitFor continuation/kill/rewards/AI/hit FX, CancelSneaking skill/buff
ownership, scrolling text manager, combat sound and full online/player/co-op
branches remain required backends. Neither the original service fixture corpus
nor the genuine prefix tests establish complete native damage delivery or a
live monster timer integration. No APK or renderer change was made.

## Central integration

Add `character_dot_attack.cpp` to the parent-owned world DSO. The optional
`character_dot_attack_audit` executable uses `tests/character_dot_attack.cpp`,
world/game-data libraries and `dl`; sanitizer flags match the central project.
No executable interposition or libm wrappers are needed. Parent can reproduce
the actual main-library replay with:

```
python port/level-world/tests/character_dot_attack_host.py --main-linked
```

Without that flag the runner builds only an isolated module/test in owned
scratch, borrowing the actual central DSOs. It binds their hashes before/after.
`build_character_dot_attack_oracle.ps1` plus
`tests/character_dot_attack_differential.py` reproduce optimized ARM64 evidence.
The API uses phase=last attempted service+1, calls=attempted deliveries and
hit_called=completed HitFor delivery. Return1 completes, -1 malformed atomic,
-2 required provider failure, -3 the reached unsupported source branch.
