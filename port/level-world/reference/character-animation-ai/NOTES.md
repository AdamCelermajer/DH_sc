# Character AI animation consumers

The reconstructed module executes the five original consumer bodies and two
dispatchers. Controller, scheduler, target, AIS and nested Character event
bodies are synchronous borrowed services. This is a consumer-kernel proof,
not a claim that the complete AI ownership or game-world integration exists.

Original ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The three function manifests and assembly captures identify every audited
consumer, helper and named virtual implementation. The separate constructors
capture resolves the fresh animator closed-state question.

## Exact entry points and projection

| Function | Address | Selection or effect |
|---|---:|---|
| CharAI::_OnAnimStepBegin | `0x3d4204` | Live state 4 Move, 5 Attack, 6/7 SkillSpell |
| CharAI::_OnAnimStepEnd | `0x3d3ff8` | Live state 5 Attack, 6/7 SkillSpell |
| _OnAnimStepBegin_Move | `0x3d4120` | Seeking/radius gate then controller MoveTo |
| _OnAnimStepBegin_Attack | `0x3d4044` | Depth-dependent flags, LookAt, pre-attack, event 1a |
| _OnAnimStepEnd_Attack | `0x3d3e44` | Combo/depth gate, continuation, target, scheduler and events |
| _OnAnimStepBegin_SkillSpell | `0x3d3dd4` | Set started byte and optional StopLoop(true) |
| _OnAnimStepEnd_SkillSpell | `0x3d3d68` | Optional StopLoop(true) |

`AnimationAIState96` has borrowed 64-bit owner/controller/target/look-target
identities. It projects animator depth from owner `+0x4c8` (animator `+0x2c`),
owner flags `+0x520`, target `AI+0x40`, seeking/sticky bytes `+0x4a/+0x4b`,
attack index `+0x74`, continuation/last/finisher bytes `+0x78/+0x79/+0x7a`,
skill bytes `+0xd0/+0xd1`, owner XYZ `+0x1a8/+0x1ac/+0x1b0`, and the raw
signed owner byte `+0x14a8`. The higher-level producer of `+0x14a8` remains
unresolved; the module deliberately retains the offset name.

The API is `dh2_character_animation_ai(state, operation, services)`, with
operations for each dispatcher or direct consumer. Requests use
`AnimationAIRequest32`, response words/XYZ use `AnimationAIResponse16`, and
`AnimationAIServices16::invoke` runs synchronously. Callbacks may mutate the
live projection. Locals captured by the original remain captured locally.
The return value is the actual source acceptance (1), after its real branch
effects; malformed native input returns -1 before mutation or callbacks.

## Ordered behavior and callback reentry

Move first checks nonnull target, nonzero seeking, and owner flag `0x1000`
clear (`AI_IsTargetSeeking`, `0x3d49d0`). It fetches target XYZ, then reads live
owner XYZ and captures the three differences. It queries melee radius squared
and computes `(x*x+y*y)+z*z`, with source single-precision arithmetic. The
comparison at imported `0x30e9ac` is `__aeabi_fcmple`: MoveTo is issued when
**radius squared <= distance squared**, including equality. Its target is
reloaded after the radius callback. NaNs, infinity and overflow were tested.

Attack Begin captures depth before querying step index and count. At depth 0
it stores the step as attack index, then emits event `0x1a` with that step word
as payload. At depth 1, step 0 writes `AI+0x79=1`, calls controller LookAt with
owner `+0x408`, writes `AI+0x7a=0`, then calls AI virtual `+0xa4` using the
**live** attack index after LookAt. Other steps compare against count-1, write
`+0x79`, call LookAt, then write `+0x7a` from the captured comparison. Other
depths perform no branch effects and accept.

Attack End queries HasComboAttack before capturing depth and querying step
index/count. If false, it accepts immediately. At depth 0 it reads and clears
continuation `+0x78`, retaining exact `old_byte XOR 1` (not boolean negation).
For the last step, a nonzero result invokes ClearNonStickyTarget; zero invokes
SetStep(0). Both then emit `0x1b`, followed by `0x1c`, with null payloads.
For a nonlast step, a nonzero result queries owner virtual `+0x124`; if false,
it clears the target then SetStep(count). Every nonlast branch emits `0x1b`.
Count subtraction follows 32-bit wraparound.

At depth 1, a nonnull target receives virtual `+0x34` and the target is then
reloaded. If it became null, the raw signed owner byte `+0x14a8 == 8` is tested.
Only the second-last step can SkipNextStep. It clears continuation when the
remaining target reports dead or the absent-target byte is 8; otherwise it
skips the next step. Other steps clear continuation. Callback target mutation
and retained query return were compared against the original instructions.

Both SkillSpell functions capture depth, query index, and query count even
though count is unused. At depth 1/step 0, Begin writes started `+0xd0=1`;
both Begin and End issue StopLoop(true) when `+0xd1` is nonzero.

## Service implementations and remaining producers

* Step index `0x3c932c` returns UINT_MAX when animator closed byte `+0x48` is
  nonzero. Step count `0x3c934c` returns 0 when closed.
* SetStep wrapper `0x3c9484` loads current depth and tails `0x3c946c`; it checks
  closed and only writes the frame's raw step word. It does not select a clip,
  replay, or raise event 26. SkipNextStep `0x3c9464 -> 0x3c9444` similarly only
  increments the step word. StopLoop `0x3c948c` clears the current loop count;
  true sets animator skip flag `+0x4a=1`, false preserves it. These are borrowed
  services here and the scheduler worker owns the native adapters.
* ClearNonStickyTarget `0x3d8d70` returns if sticky byte `+0x4b` is nonzero.
  Otherwise it calls the deep AI_SetTarget(null,false) `0x3d6890`, then
  AI_SyncLastTarget `0x3d49c4` copies the live target to `+0x44`. This full helper
  is an explicit service; target ownership/notification policy is not replaced.
* Target virtual `+0x34` is Character::IsDead `0x3a2ed4` (owner byte `+0x1449`);
  base GameObject implementation `0x3400ac` returns 0. Owner virtual `+0x124`
  is Character::CanRangeAttack `0x3a4d3c`: resolved property index 32 at
  `+0x1078 != -1` accepts, otherwise EquipSet::HasRangeWeapon `0x400014`.
* AI virtual `+0xa4` is CharAI::OnPreAttack `0x3d0ed4`. It first calls
  AI_CanAttack(null) `0x3d67f4`; only a true result with active AIS `+0x1c`
  calls AIS virtual `+0xa4(index)`. CanAttack selects the live target, requires
  AI_IsEnemy, then checks melee equipment/range or the owner's range-attack
  capability and AI_IsInRange. It is not a target getter or unconditional call.
* AI_GetMeleeRadius `0x3d4c34` queries EquipSet::GetMeleeRadius `0x3fff30` into
  an integer, converts it to float, adds the selected 68-byte AIProps row's
  float `+0x20`, and `0x3d4c9c` squares it. Equipment/AIProps production remains
  the explicit radius query service; no guessed weapon radius is supplied.
* CharAI::OnEndOfAnim `0x3d0ce8` forwards to active AIS `+0x1c`, virtual `+0x98`,
  when nonnull. AISDefault `0x3dbeec` is empty; AISExternal `0x3dccd0` forwards.
  Parent separately verified the script chooser: row44 `__player__` selects
  AISPlayerIPhone, whose end virtual inherits that empty default. Pending AIS
  `+0x20` and active AIS `+0x1c` are distinct; native allocation/ownership remains
  a separate boundary. This consumer module does not invent that binding.

HasComboAttack `0x3a346c` uses GetCharAnimTableId `0x3a3228`, **not animation
stance**. Resolved property index 2 at owner `+0x1000` selects a valid table;
invalid indices use 17. The table stride is 160, moving Attack sequence is
field `+4`; sequence stride is 20 and type at `+0x10` must equal 1. The two
pure exports reconstruct the lookup predicates. The authored bank48 probe
finds Attack248/type1 (children467-469) and AttackStatic243/type1
(children470-472). Current renderer sets animation_table from resolved[2]
after its KnightPlayerBase property recalculation; bare stance selection does
not determine this combo predicate. `authored-player-bank.json` binds the
actual animation stream and records the separate property-production boundary.

Fresh CharAnimator constructors C1 `0x3c906c` and C2 `0x3c8ff4` both set closed
`+0x48=1`, depth `+0x2c=0`, pending `+0x49=0`, pending sequence `+0x50=-1`, and
speed factors `+0x34/+0x40=1`. C1 writes closed at `0x3c90a0`; C2 at
`0x3c9028`. A first pre-selection event26 can therefore observe closed getters;
the source getter is not to be initialized as an open empty sequence.

## Verification

Original-instruction execution versus optimized standalone ARM64 passed
2,197 consumer cases, 4,530 ordered service-entry snapshots, 90 original pure
lookup cases and 17 atomic malformed-input guards, with zero mismatches.
The same corpus passed host ASan/UBSan. Cases include callback changes to
depth/index/target/flags, raw byte values 0/1/2/255, boundary counts, closed
scheduler outputs, float exceptional values, and identities above 4 GiB.

Gold corpus SHA-256:
`80e0863da67e4ebe38d80679899510792a3a915c2309c1b0b0024d8e4b3ae1b6`.
The `character-animation-ai-arm64-differential.json` and
`character-animation-ai-host-audit.json` reports bind original, corpus,
current source/test and reference hashes. No APK or live-controller proof is
claimed by these standalone reports. Production integration must bind the
named synchronous services and refresh live projections after their effects.

## Main-library and Android source integration

`CMakeLists.txt` now compiles the consumer source into `dh2_level_world`.
The `character_animation_ai_audit` target links that shared library rather than
compiling a separate consumer copy. `tests/character_animation_ai_linked_host.py`
builds between stable dependency-source snapshots, resolves and hashes the five
shared dependencies and executable, checks exported consumer symbols, then
replays all 2,197/90/4,530/17 cases under ASan/UBSan. The corresponding report is
`reports/character-animation-ai-main-library-host-audit.json`, with zero findings.

Both actual Android CMake configurations (`arm64-v8a`, `x86_64`) build the new
world library successfully. `tests/character_animation_ai_android_library.py`
replays that original-bound gold through the actual NDK Debug ARM64 world ELF:
all 2,197 consumers, 90 scalar cases, 4,530 requests and 17 guards pass. Its
ELF64 AArch64 LOAD segments have16KiB alignment; world ELF SHA256 is
`2167e70c50cc2c8d7093a852a5d1d3e00229ed382f130efb5aa4379cbeb44bf2`.
`reports/character-animation-ai-android-library.json` binds this execution.
Android checked memcpy executes through an explicit libc service fixture
(718 calls); TLS canary and borrowed gameplay services remain fixtures.

The built world/animation/game-data/skinning libraries for both ABIs are
preserved in `.local-inputs/animation-source-library-capture/`, with hashes in
`libraries.json`. These builds do not change the saved timing APK. The replay
does not execute unrelated library methods, GPU rendering or live AIS ownership;
physical ARM64 and the full observer/controller integration remain pending.
