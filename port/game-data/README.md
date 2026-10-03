# Original character, model and animation tables

This native C++ reader reconstructs the leading CharacterTable section and the complete ModelDict from the supplied game data. It does not run Python or the original ARM32 engine.

The CharacterTable contains 448 named records, each with 224 signed 32-bit fields. Its leading section consumes 401,412 bytes; 196 following subclass bytes remain uninterpreted. The original names and field-schema files also retain later sections. ModelDict contains 116 named model paths. Length-prefixed ASCII strings and counts are bounded, identifiers are unique, and failed reads clear their outputs.

[Original routines](original-functions.json) and [assembly](reference/original-functions.asm) include `CharacterProperties::read` at `0x4f2750`, `CharacterTable::read` at `0x4b4340`, the model dictionary readers and the four-byte integer stream reader. The original character object has a vtable followed by 896 bytes of properties; the new representation owns ordinary C++ arrays and strings.

`Crypt_Skeleton.ModelFile` is 85, linking `data/3D/characters/skeleton/skeleton.bdae`; `CryptSlime.ModelFile` is 89, linking `slime_green_v2.bdae`. These links drive the Android world object's resource selection. Numeric values remain raw original values. The class reader/evaluator below now derives cached base properties; dynamic combat statistics, AI enums and subsequent character subclasses remain pending.

Configure this directory with CMake and ASan/UBSan C++/linker flags, build, then run `data_audit path/to/assets/data`. The [host audit](reports/host-audit.json) verifies all 448 records, 224 fields, 116 models, 439 valid nonnegative model links, and 2,000 mutations/truncations. Original input hashes are retained in the [object checkpoint provenance](../level-world/reports/object-input-provenance.json).

## Class formulas and base properties

[class_tables.cpp](class_tables.cpp) reads all 260 class lists and 1,659 five-word formulas in the original 34,224-byte file. It validates all thirteen schema sections, names, counts, references and property indices; failed loads clear outputs. The [original reader/runtime evidence](reference/classes/original-functions.json) captures ClassTable/ClassFuncList/ClassFunc readers and class dispatch, arithmetic, groups, getters and setters. [Static property layout](reference/classes/property-layout.json) confirms all 224 original offsets equal the serialized field index times four. Numeric IDs such as Level 19 and Max_HP 38 therefore address the native array directly.

The evaluator preserves unsigned 32-bit wrapping and signed arithmetic shifts. Formula 1 computes `base + per_property * (source >> 8)`, with base `-666` meaning the current destination. Group formulas apply up to three referenced classes in order. Clamps, property addition, scaling, constant addition and OID setting follow the original dispatch. Type 3 is ignored, type 8 returns from the current class through an original no-op, and type 6 scales then falls through into addition of its first parameter. That last behavior is preserved even though the parameter is also a property ID. Native execution rejects cycles/depth beyond 32 and excessive work; the owning API applies changes atomically.

[ARM64 comparison](reports/classes-arm64-differential.json) executes the full original class dispatch, arithmetic, groups and sheet getters/setters against compiled native source for 3,120 whole-sheet cases, half using explicit buff snapshots, plus all 448 original character records. All 224 words match in every case. Class arrays and caller sheets are initialized by the fixture; the original cached-sheet identity is selected to avoid dynamic `RecalcProperty`. No formula arithmetic or sheet access is mocked. [Host audit](reports/classes-host-audit.json) verifies exact binary re-encoding, 1,040 class/level applications, 448 character applications, 3,000 mutations/truncations and atomic rejection.

The Crypt records use BaseMonster class 18 and serialized Level 256 (fixed-point 1). Its cached formula produces Max_HP 12,160 (47.5) and Max_MP 5,632 (22), while current HP in that base snapshot stays at its original unresolved `-1`. Android retains these diagnostic base snapshots per monster and logs a checksum across all 224 little-endian words; emulator checks match the original-instruction snapshots for all eleven instances. These cached snapshots do not initialize health, infer dungeon difficulty or produce equipment/buff sheets. The normal owner recalculation and HP/MP initialization path is described below. Level assignment, buff/gear producers and full combat remain pending. `character_classes_pycst.bin` is retained as original enum evidence; the runtime parses the three class list/name/schema files natively.

Reproduce with `classes_audit path/to/class/data path/to/character/data`. `tools/build_class_oracle.ps1` builds the standalone ARM64 comparison library; `tests/classes_differential.py --help` lists its input/report arguments. Original inputs remain ignored by Git.

## Property resolution and mutation

[properties.cpp](properties.cpp) reconstructs `RecalcProperty` and raw/fixed-point setters using the original default/type rows. The original getters' GOT references both identify `Arrays::CharacterTable::members` at `0x9a645c`: defaults are row 0 (`AAA_DEFAULTS_DONT_DELETE`), types are row 1 (`AAA_TYPES_DONT_DELETE`). [Rule evidence](reference/properties/property-rules.json) records all 224 values/types and input hashes; [routine evidence](reference/properties/original-functions.json) and assembly retain the resolver, sheet access, defaults, resetters, mutations and original container traversal.

Property presence means a value differs from that property's default. Type-bit precedence is additive 4, override 2, priority 1, saved contribution 32, base-only 16, then preserving the computed value. Type `-1` becomes 16. Additive properties combine nondefault base, saved, gear and ascending buff groups. Override properties choose the first set base/saved/gear source, then the first ascending group containing a set value. Priority properties inspect descending groups before gear/saved/base. Within a selected buff group, the last nondefault entry wins. Original `AddProperty` replaces a result equal to the default instead of adding to it; an intermediate cancellation/wrap can therefore reset accumulation. Saved contributions use that helper even when the saved value equals the default. Arithmetic wraps explicitly at 32 bits.

`PropertyView` accepts owned sheet arrays and buff groups already ordered as the original map's keys, with insertion order preserved within each group. This API reconstructs resolution, not the production of equipment/buff sheets or their timers. Raw set/add operations write saved properties when type bit 32 is present, or computed values for type bit 8; other types ignore those writes. Set-to-sheet writes the supplied sheet for type bit 4 and recomputes, otherwise writes computed values for bit 8. Integer setters preserve the original wrapping shift by eight. Invalid indices, missing sheets and excessive group sizes are rejected before writes.

The [ARM64 differential report](reports/properties-arm64-differential.json) checks 119,168 individual resolutions and 7,168 mutation cases with all four sheets compared. It exercises actual and synthetic type combinations, empty and multiple buff groups, deque sizes through 40, wrapping extremes and every original character row. The oracle executes original resolver/default/getter/setter logic plus its actual RB traversal and deque distance/advance helpers; no property or traversal function is mocked. Fixtures initialize caller sheets, the original table global and valid ordered containers. Four additional Crypt cases use original cached-class dispatch before resolution. The [ASan/UBSan audit](reports/properties-host-audit.json) compares all 448 native resolved sheets to a binary reference generated by original instructions, and checks default collisions, insertion/group priority, health setters, runtime-only preservation and invalid-input atomicity.

Android owns separate base/saved/gear/resolved arrays per monster. In the property-only checkpoint, it resolves cached class bases with initially empty saved/gear sheets. All eleven checksums match original-instruction snapshots. These Crypt snapshots equal their cached bases and retain HP/MP `-1`; the normal class and health initialization below now follow this phase. Gear/buff producers, live damage, gameplay events and persistent saves remain pending. A faithful property setter alone does not establish playable combat.

Reproduce with `tools/build_properties_oracle.ps1`, then `tests/properties_differential.py --help`. Its required `--row-reference` writes an ignored original-instruction comparison artifact. Run `properties_audit path/to/character/data path/to/row-reference.bin` in the sanitizer build. No extra assets are needed: default/type rows are already bundled in the original character table.

## Normal class recalculation and spawn vitals

`apply_class_uncached` reconstructs the normal base-sheet class path: each linear formula resolves its source through the owner's current base/saved/gear/buff state, then writes the base destination. The `-666` current-destination base is read before resolving the source. Other formulas retain their original raw-sheet behavior. `recalc_properties_with_class` applies the class selected by base ClassID 26, then resolves all 224 properties. Owning calls commit complete state atomically. General class application to other owner sheets and buff creation remain separate work. Cached application retains its previous API and passes a new [3,568-case regression](reports/classes-regression-vitals.json).

[vitals.cpp](vitals.cpp) reconstructs `Character::RegenHP`, `RegenMP` and the HP-then-MP `_InitHpMp` sequence. Regeneration requests are raw fixed-point integers. A negative request uses the maximum; wrapping 32-bit `current + request` is compared to that maximum with a signed comparison, and an excessive request becomes wrapping `maximum - current`. Only positive resulting requests call the original-equivalent property adder. The implementation preserves unusual overflow/default behavior instead of replacing it with a generic saturating health clamp.

[Initialization evidence](reference/vitals/original-functions.json) retains `InitPost`, `Revive`, `_InitHpMp`, regeneration, base-class dispatch and normal linear reads. `InitPost` calls `Revive` at `0x3b542c` and explicitly calls `_InitHpMp` at `0x3b5434`; `Revive` itself calls `_InitHpMp` at `0x3a59f0`. The fresh HP/MP helper consequently invokes the fill sequence twice. For the Crypt rows, the first HP request adds 12,160 to the saved `-1`, producing 12,159; the second adds one and reaches Max_HP 12,160. MP similarly reaches Max_MP 5,632. Full `InitPost`/`Revive`, actor state flags, physics and other lifecycle effects are not reconstructed by this helper.

The [vitals differential report](reports/vitals-arm64-differential.json) matches 2,080 uncached class cases with all four sheets compared, 10,000 regeneration cases including 4,803 positive mutations, all 448 complete original character recalculations, and 448 two-pass HP/MP sequences. Original property/class arithmetic, source resolution and buff traversal run unmocked. For regeneration only, the original debug-switch load/string/query/destructor blocks are skipped: their query result is discarded before stat mutation. The cap decisions, property mutation and resolution execute as original instructions. The full `InitPost`/`Revive` functions are inspected to establish call order, rather than emulated end to end; the report states this limit.

The [ASan/UBSan host audit](reports/vitals-host-audit.json) compares all 896 words of each owner against original-instruction references before and after initialization, checks partial/full/no-op regeneration and rejects invalid/recursive calls atomically. A saved Level increment changes the original class source read and raises BaseMonster Max_HP from 12,160 to 29,440 at level ten. Android performs normal class recalculation and fresh HP/MP initialization per monster; all eleven resulting checksums and both passes' raw requests match the original-instruction fixtures.

Use `tools/build_vitals_oracle.ps1`, then `tests/vitals_differential.py --help` with `uv run --no-project --with unicorn --with pyelftools python`. The ignored reference prefix produces `-uncached.bin` and `-spawn.bin`. Run `vitals_audit path/to/characters path/to/classes path/to/uncached.bin path/to/spawn.bin` in the sanitizer build. The new class oracle build links the property resolver as well as class arithmetic. Timed regeneration, save restoration, dynamic dungeon level selection, equipment/buff producers, damage and real death/revive state transitions remain pending. Selecting `Attack` or `Died` in the development UI still only exercises animations.

## Animation state reconstruction

[animation_tables.cpp](animation_tables.cpp) reads the entire 72,812-byte `animations_pyarray.bin`: 785 animation sequences end at byte 57,952, three camera sets end at 58,028, and 80 character animation sets consume the remainder. The original stream contains 1,348 variable-sized steps, 163 redirects and 1,121 nonnegative character-state links. AnimDict supplies 1,447 named clip paths. The name/schema files contain three/four sections respectively; all sections must match the recovered schema and be consumed exactly. `Interact` and `Spells` are arrays, and step booleans occupy one serialized byte. Runtime C++ vtables and padding are not serialized.

[Reader evidence](reference/animation-readers/original-functions.json) and its assembly capture `AnimStep::read`, `AnimTpl::read`, `CamAnimSet::read`, `CharAnim::read`, array loaders and typed stream reads from the hash-identified original engine. The [native table audit](reports/animation-tables-host-audit.json) re-encodes every parsed record and matches all original bytes, verifies known skeleton/slime/ghost states and dictionary links, and exercises 3,000 mutations/truncations. Failed reads clear outputs; counts, strings, schemas, speeds and character/clip/redirect references are bounded.

[animation_selection.cpp](animation_selection.cpp) reconstructs the initial selection: type 2 selects a random step; other types start at step zero, and redirects enter at most three sequence layers. The supplied random-enabled flag corresponds to bypassing the original debug override. The original helper updates its unsigned seed with `((seed * 59051 + 177149) mod 2^32) mod 14348907`, then reduces it by the step count; every invocation increments the call counter, including count zero. [Selection evidence](reference/selection/original-functions.json) captures the original helper and CharAnimator methods. [ARM64 comparison](reports/animation-random-arm64-differential.json) executes the original helper and compiled ARM64 source for 10,000 bit-exact result/seed/counter comparisons; only its imported unsigned division/remainder is modeled.

Skeleton state 62 links Idle 596, Walk 604, Attack 590 and Died 594. Skeleton idle has two choices; slime idle has six entries referencing two clips; ghost idle uses speed 1.3. Android reads these original links and speeds. All five unique Crypt idle clips pass [every-millisecond pose and skin checks](../level-world/reports/idle-options-host-audit.json).

## Completion and per-actor scheduling

[animation_scheduler.cpp](animation_scheduler.cpp) owns a checked stack of at most three sequence frames. Type 1 advances through steps; other types complete their selected step. Zero loops finishes the current frame, positive loops count additional passes, and negative loops repeat. Repeating type 2 selects again with the original random helper. A completed redirect returns to its parent; completing the root leaves the last clip available for an end-pose hold. Starts and completion changes commit atomically, including RNG state. `stop_loop()` clears the current frame's loop count.

[Completion evidence](reference/completion/original-functions.json) captures `CharAnimator::Update`, the completion callback, `StopLoop`, skip and animation-set entry points. The [3,000-case ARM64 comparison](reports/completion-arm64-differential.json) checks the isolated completion decision, step and loop counters against original instructions. Character event dispatch is a no-op in that oracle; it stops before selection, logging and parent unwind. The [host scheduler audit](reports/animation-scheduler-host-audit.json) exercises 759 original sequence starts, 6,966 completions, nested parent advancement, finite loops, idle re-selection and atomic rejection. Twenty-six empty/event-only starts or later steps remain unsupported by this clip scheduler.

The renderer now gives all eleven monsters separate clocks and scheduler state while sharing decoded resources and clip banks. Twenty-six original clips cover Idle, Walk, Attack and Died for skeleton, slime and ghost. Each attack redirects to three ordered clips; death finishes and holds its final pose. Nonzero clip starts are preserved, including slime attack/recovery and ghost attack/recovery segments. [Host pose audit](../level-world/reports/actor-clips-host-audit.json) samples every millisecond across all 26 clips, totaling 45,218 finite skinned poses.

The global development RNG starts at seed 1. The frame-time clamp and carrying leftover wall time into the next clip are new adapters; full original engine timing and GPU parity are unverified. Blending, queued animation replacement, skip/forced-loop commands, root-motion/event dispatch, FX/audio, AI/combat and persistent actor lifecycles remain pending. Debug state selection demonstrates animation scheduling; it does not instantiate combat.

## Native damage and attack-event decisions

[combat.cpp](combat.cpp) reconstructs `CF__CalcDamage` (`0x3b1fb8`),
`CF_CalcDotDamage` (`0x3b09c4`), the selected weapon-category bonus
(`0x3df8ac`) and combat RNG (`0x3af6d8`). Inputs are resolved 224-word property
sheets, explicit equipment facts and an owned RNG. The output preserves all
seven original damage words: amount, element, HP/MP leech, DoT duration,
DoT amount and DoT element. Physical, magical and direct damage retain the
original fixed-point arithmetic, signed shifts, wrapping, resistance behavior,
critical/block modifiers and unusual default/zero-range behavior. Random range
reduction is **unsigned modulo even when the supplied signed range is negative**.
The API rejects invalid views/flags before changing output or RNG state.

[Original-instruction comparisons](reports/combat-arm64-differential.json)
match 23,792 calls: 10,000 RNG, 3,000 damage, 3,000 DoT, 6,000 bonus and
1,792 damage calculations from all 448 initialized original character sheets.
The original raw-property/inventory getters and RNG execute; only imported
unsigned division/remainder is modeled. Synthetic cases include full signed
word overflow, all sixteen off-hand/magic/block/critical flag combinations,
both weapon categories, combo counters and zero/reversed ranges. Caller
fixtures supply character/equipment state. These helpers consume already
chosen hit/block/critical facts: the hit rolls, target acquisition, damage
application and full combat lifecycle are not implemented by this module.
The [captured evidence](reference/combat/original-functions.json) includes
additional inspected combat functions that remain unported.

[combat_events.cpp](combat_events.cpp) reconstructs only the state-5 branch of
`CharAI::_OnAnimEvent` (`0x3d4434`). Unarmed melee accepts `attack_mainhand` and
`attack_offhand`, preserving the outer sequence step, inner clip step minus
one, and hand selector. Ranged capability accepts `attack_mainhand`,
`attack_ranged` and `do_skill`, carrying the projectile template identifier.
Names remain case-sensitive. The [6,000 original decisions](reports/combat-events-arm64-differential.json)
execute the original state/timeline getters and dispatch; observing caller
fixtures supply capability and attack/projectile backends. Discarded debug
query blocks are skipped and counted. FX/audio/script/skill/spell/interact
handlers, projectile creation and full AI ownership remain pending.

The Android callback now routes each live monster signal into an observable
combat request. Its unarmed capability reads resolved projectile property 32;
the four supplied Crypt character types contain `-1`. The original capability
getter checks that property before inventory fallback, as retained in
[range evidence](reference/combat-events/range-capability/original-functions.asm).
Inventory fallback is not yet integrated. `Actor combat action` logs the
request with `execution pending`; no hit is performed and no target HP is
changed. Frozen pose inspection does not dispatch requests.

[ASan/UBSan verification](reports/combat-host-audit.json) compares all seven
damage words plus RNG for 1,792 original character cases and all 6,000 original
event decisions. Expected outputs are captured from original instructions,
rather than calculated by a second copy of the port. Invalid input atomicity
and zero-range counter wrapping pass, and the linked vitals module still
matches all 448 initialized owners. Reproduce with
`tools/build_combat_oracle.ps1`, then both `tests/combat*_differential.py --help`.
The ignored binary references feed `combat_audit characters-directory
spawn-reference.bin damage-reference.bin events-reference.bin` in the sanitizer
build. Full combat RNG initialization/sharing and game lifecycle remain pending.

## Complete hit and status result calculation

[combat_result.cpp](combat_result.cpp) reconstructs the full
`Character::_F_CalculateResult` (`0x3b2638`), including original result reset,
mask precedence, miss/dodge, block, critical, hurt/push/stun/fear/slow checks,
damage selection and exact random-consumption order. The ten-word result
retains amount, DoT fields, leech, nine outcome bits, input mask, weapon category
and element. A miss or dodge returns before later rolls. Critical and hurt
share a roll; push, stun, fear and slow each consume their own roll. Damage
uses the successful block/critical flags. Physical critical chance has the
original 98-percent upper gate; magical critical chance does not. Low-24-bit
signed level deltas, wrapping fixed-point operations and the slow chance's
two staged float-to-integer conversions are preserved.

`dh2_combat_melee` reconstructs `F_MeleeAttack` request construction from
supplied equipment facts: masks `0x22aab5`/`0x5554a`, selected hand's weapon
category, element `-1`, and off-hand bit 26. The original second boolean is
exposed as `alternate`, without assigning a new gameplay meaning. Its optional
buff removal is not implemented; verification supplies an empty original buff
dictionary and states that limit. Full inventory/buff producers remain separate.

[Original instruction verification](reports/combat-result-arm64-differential.json)
matches 9,792 complete results/RNG states: 8,000 synthetic cases and four
attack masks for every one of 448 initialized source character sheets.
All nine outcomes are observed, with zero through eleven random draws per
synthetic result. A further 2,000 calls execute original `F_MeleeAttack`, its
equipment/category selection and full result calculation against the native
request wrapper. Original property/inventory/helper instructions execute;
imported IEEE soft-float and unsigned division/remainder are modeled.
Profiling routines are original no-ops. Only discarded debug-switch queries
are skipped and counted. This verifies result calculation with supplied
combatant state, not combat-result application or full AI/lifecycle behavior.

The [ASan/UBSan audit](reports/combat-result-host-audit.json) compares all ten
words and RNG state for the same 9,792 original outputs, including overflow
and floating conversion cases. Invalid actor/RNG/hand inputs preserve output.
The ignored host fixture contains the original outputs plus caller sheets and
equipment metadata; expected values are not recomputed by a copy of the port.
Android executes unarmed melee/result probes against a supplied Crypt Skeleton
sheet for all eleven actors at initialization. Their outputs and copied RNG
states match the four original Crypt snapshots. These probes are logged as
`validation only`, never select targets or mutate actor health, and do not
replace the independent animation RNG.

Reproduce with `tools/build_combat_oracle.ps1 -Output
.local-inputs/combat-result-oracle.so`, then
`tests/combat_result_differential.py --help`. Run `combat_result_audit
path/to/combat-result-original-characters.bin` in the sanitizer build. The
[captured controller evidence](reference/combat-result/controller.asm) also
retains `F_ApplyResult` for the next stage. Health/damage application, combo and
aggro ownership, status/death transitions, online/session services, audio/FX
and the normal gameplay target/attack path remain unfinished.

## Health mutation and limited live monster application

[health.cpp](health.cpp) reconstructs `Character::HitFor` subtraction,
property writes, dead/session/debug gates, kill/lifecycle requests and
low-health warning hysteresis. [Original comparison](reports/health-arm64-differential.json)
matches 10,926 cases with all four property sheets and output words compared;
the host repeats these captured outputs under ASan/UBSan. Classification,
session, remoteness and debug facts are supplied. Controller kill is observed
in this isolated helper; audio, tutorials, party credit and achievements are
omitted. The warning rearms at 75 percent inclusive using original float
conversions. Lifecycle write 3 is separate from the actor's dead byte.

[combat_application.cpp](combat_application.cpp) implements the offline,
single-player, nonplayer branch of `F_ApplyResult`. Both actors must use their
direct health owner, the main player is present/living, debug switches are
default, and the gold-damage mask is rejected atomically. This API updates
combo, push-death state, HP/MP and core kill/dead/lifecycle state. It clears
fear/stun/slow result bits after death and regenerates attacker HP before MP.
It returns threat and ordered DoT/dodge/block/hurt/push/stun/fear/slow requests;
their aggro, buff and FSM services are not yet implemented. Remaining kill
rewards/scripts, AI callbacks, FX/audio, multiplayer scaling, indirect health
owners, player-specific application and alternate-mode buff removal are
outside this API's scope.

[Captured application evidence](reference/combat-application/original-functions.json)
includes the original application, health and core kill functions. Original
`Ctrl_Kill` and the kill guard/dead/HP prefix execute before observing the
remaining backend and death event. [Death FSM evidence](reference/combat-application/death-fsm-functions.json)
records the state/controller handoff; the Android queued clip adapter does
not claim to implement that complete FSM.

[ARM64 differential verification](reports/combat-application-arm64-differential.json)
matches 6,914 cases: 6,000 synthetic, 896 initialized source-owner cases and
18 persistent melee hits across three Crypt actor pairs. Each comparison
checks 1,792 property words, ten result words, ten actor-state words, 25
application words and observed service request order. All eight request
types occur. Actual original property/health/regeneration/core-kill and
melee-result instructions execute; omitted services and fixtures are
recorded in the report. [Host verification](reports/combat-application-host-audit.json)
compares the same captured cases under ASan/UBSan and checks invalid calls
leave inputs and outputs unchanged.

Android authored melee events now apply hits for explicitly supplied
development targets. Its three six-hit sequences reduce HP from 12,160 to
zero and queue the original skeleton/slime/ghost death clips. Actor sheets,
combo, death state, scheduler/event cursor and combat RNG survive tested
rotations and background/resume without replaying hits. Frozen inspection
does not apply damage; dead targets are rejected. This remains a limited
combat adapter, with no automatic target acquisition or Prince combat path.
The combat seed and its separation from animation RNG are development
policies rather than recovered original initialization.

The fresh actor regression also exercises outer attack sequence 2. A
separate [6,000-case event fixture](reports/combat-events-application-arm64-differential.json)
includes 24 explicit runtime combinations over sequences 0, 1 and 2,
preserving the historical event fixture. Fresh sanitized damage/event and
health regressions pass. Source and original-reference hashes are bound by
the [Android checkpoint validator](../android-native/reports/build-validation-combat-application.json).

Reproduce with `tools/build_health_oracle.ps1`,
`tools/build_combat_application_oracle.ps1`, and the corresponding
`tests/*_differential.py --help`. Run `combat_application_audit
path/to/combat-application-original.bin` in the sanitizer build. Run
`build_combat_oracle.ps1 -Output .local-inputs/combat-events-application-oracle.so`
and `tests/combat_events_differential.py --runtime-sequences 0 1 2` for the
expanded event evidence. Captured binary fixtures and original assets remain
ignored by Git. The full native game goal remains active.

## Prince attacker application

`dh2_combat_apply_player_to_monster` exposes the verified common application
core for an offline player attacker and a nonplayer defender, with no active
critical-triggered skill. It preserves the same property, combo, health,
regeneration, core kill and status-request behavior as the nonplayer adapter.
Achievements and the remaining aggro/status/reward/FX/audio services stay
external. This entry point covers a player attacker; player defenders are
covered by the separate entry point below.

[Original player comparisons](reports/player-application-arm64-differential.json)
execute the original IsPlayer branch and empty critical-skill lookup. They
compare 2,732 cases: 1,500 synthetic, 896 initialized owner cases, and 336
persistent melee attempts over KnightPlayerBase versus skeleton/slime/ghost.
The original post-core player achievement branch is explicitly omitted.
Each case checks both owners' 1,792 property words, all ten result words,
ten actor-state words, 25 application words and observed service order.
[The host audit](reports/player-application-host-audit.json) repeats the same
captured cases under ASan/UBSan. The existing 6,914 nonplayer cases also pass
against the updated implementation.

[Player initialization references](reference/player-initialization/original-functions.json)
and their assembly preserve `SafeGetCharPropsId`, save/class lookup and
creation/manager call sites. The fallback string is `KnightPlayerBase`, row
263, ClassID 77, animation table 48. The current Android adapter explicitly
uses that fallback and the verified normal base/vitals helpers, producing
HP 42,265 and MP 6,976 in original fixed-point units. This does not recover
the full original save/new-game/equipment initialization pipeline.

Build the ARM64 comparison library with
`tools/build_combat_application_oracle.ps1 -Output .local-inputs/player-application-oracle.so`,
then use `tests/combat_application_differential.py --player-attacker --help`
for its input arguments. Run `player_application_audit
path/to/player-application-original.bin` in the sanitizer build.

## Player defender application

`dh2_combat_apply_monster_to_player` reconstructs the offline nonplayer
attacker / player defender branch, accepting the caller's
`SM_IsIdle(false)` snapshot. It preserves health subtraction, low-health
hysteresis, core kill/death writes, leech ordering and status requests. An
idle player with no existing dodge/block/hurt bit gains hurt on positive
damage or dodge on nonpositive damage. Block, evade and knockdown preserve
the original saved achievement-counter increments (fields 214, 215, 222).
These property writes are implemented even though their threshold-triggered
achievement services and the player stat manager remain external.

[Original comparisons](reports/player-defender-arm64-differential.json)
execute the original `SM_IsIdle(false)` / `SM_GetState`, property mutation,
health and core kill instructions in 2,969 cases: 2,000 synthetic, 896
initialized owner cases, and 73 persistent enemy-to-Knight melee attempts, including one explicit
state-13 moving-player case. All owner/result/state/output words and observed request order match.
The explicit difficulty snapshot is non-tutorial, debug switches are false,
and gold damage, tutorial/party/audio, rewards, full FSM and actual status
services are outside this entry point.

[The host audit](reports/player-defender-host-audit.json) repeats the same
captured cases under ASan/UBSan. Existing [2,732 player-attacker cases](reports/player-defender-player-regression.json)
and [6,914 nonplayer cases](reports/player-defender-monster-regression.json)
remain unchanged against the updated common implementation. Original
support routines are preserved in [the player-defender reference](reference/player-defender/original-functions.json).

Run `tests/combat_application_differential.py --player-defender --help`
with a library built by `tools/build_combat_application_oracle.ps1`.
`player_defender_audit path/to/character/data path/to/player-defender-original.bin`
consumes its captured fixtures in the sanitizer build. Defender records are
14,600 bytes (the existing 14,596-byte record plus one idle snapshot word).

## Native aggression tables

[`aggro.cpp`](aggro.cpp) reconstructs the original Set/Add/Clear aggression
table service and Get/Highest/Count/Has/IsAggroed queries. Caller-owned sorted
64-bit character identities replace the old pointer-keyed ARM32 map layout.
Set updates an outgoing threat and its reciprocal incoming relation. Zero,
negative and NaN threats remain entries; Highest selects strictly positive
threats and keeps the first ascending key on ties. HasAggro uses outgoing count;
IsAggroed uses incoming count. Set rejects player owners and dead actors.
An existing-entry Add returns the actual delta, including negative previous
threat when the underlying Set was rejected.

The service exposes OnAggro/OnDeAggro and target-clear/controller-stop requests.
The caller must dispatch their backends and preserve original character-key
ordering; full AI acquisition, target choice, pursuit, visibility/range/decay,
death cleanup and callback/FSM integration remain pending. Live renderer combat now maintains these relations using recovered threat
outputs and pre-damage actor facts. The callback services, original pre-HitFor
service ordering and full death relation cleanup remain pending; see the
[AI integration notes](reference/ai-target/NOTES.md).

[6,114 original comparisons](reports/aggro-arm64-differential.json) cover
675 IEEE boundary cases, 5,000 persistent relation-network operations,
13 explicit behaviors and 426 captured combat threat requests. Original map
insertion, rebalancing, erase, dead queries and all table queries execute.
Node allocation and actor/callback services are explicit fixtures/observers.
Finite float bits and direct Set values compare exactly; arithmetic NaNs
compare by unordered class without a claim of sign/payload parity.
Both the [repo packaged ARM64 library](reports/aggro-packaged-arm64-differential.json)
and [Studio packaged ARM64 library](reports/aggro-studio-arm64-differential.json)
pass the same records. Packaged compiler TLS canaries execute with supplied
thread storage, and checked memcpy validates its fixture buffer bound.

[ASan/UBSan replay](reports/aggro-host-audit.json) checks all 6,114 records,
reciprocal updates and atomic invalid-input/capacity rejection. See
[the recovered routine notes](reference/aggro/NOTES.md) for layouts, guards,
callback order and the scope of the original instructions.

From the repo root, build the standalone ARM64 oracle with
`port/game-data/tools/build_aggro_oracle.ps1`. Run
`port/game-data/tests/aggro_differential.py --help` for capture arguments,
and `aggro_audit path/to/aggro-original.bin` in the sanitizer build.

## Native character AI checkpoint

[`ai.cpp`](ai.cpp) reads all 76 original AI configurations and 16 faction lists,
reconstructs the character enemy relation, strict full-3D melee/sight comparisons,
and `_UpdateTarget` event decisions with supplied callback/query snapshots.
[Original ARM64 comparisons](reports/ai-arm64-differential.json) verify 23,304
cases: 1,800 geometry cases, 1,024 faction cases and 20,480 target transitions.
The packaged repo and Studio ARM64 libraries pass the same captured records.
ASan/UBSan repeats them and byte-roundtrips every configuration/faction row.

The Crypt renderer now enables automatic standing melee against the Prince by
default. It assigns the first supplied enemy only when targetless, applies the
source melee range, and clears/stops after target death. Its one-candidate spatial
adapter, scheduler/controller mapping and hit-range guard remain explicit
prototype backends. Pursuit, complete acquisition/filtering/timers, full FSM,
attack delay and status/audio services remain pending. See the detailed
[scope and original routine notes](reference/ai-target/NOTES.md).

Bundle inputs with `port/level-world/tools/prepare_ai.py CACHE.zip`; build the
oracle with `port/game-data/tools/build_ai_oracle.ps1`; use
`tests/ai_differential.py --help` and `ai_audit DATA_DIR ai-original.bin`.
