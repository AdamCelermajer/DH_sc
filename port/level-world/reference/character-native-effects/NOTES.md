# Native pending effects and Stunned/Scared methods

Original ELF SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The captured nineteen routines and eight service dependencies are hash-bound
under this directory. Frozen FSM/state/owner sources, production CMake, renderer
and APKs were not edited. This source stage is separate from tested APK22835.

## SetStun and SetScare

`SM_SetStunState` **3c5ffc** and `SM_SetScareState` **3c6144**:

1. Query owner `Character.IsBoss` **3a3158**. CharAI row+14 mask0x4 (bit2) rejects
   both effects, even when force is true.
2. `GetCharAnimTableId` **3a3228** reads signed Character+1000. An authored ID
   outside the current table range falls back to17. The effect then rejects an
   ID outside the table count. The native callback supplies the authored word;
   the native wrapper performs this recovered getter fallback.
3. If machine+2c pending bit2/4 is absent, start a timer on Character+3b4 with
   supplied uint32 duration, repeat0, event2b/2c and null user ref. The timer
   response is ignored by the original void caller, including UINT_MAX. Reload
   mask and OR the pending bit **after** that callback. An existing mask skips
   timer restart; repeated effects do not replace the remaining duration.
4. Read the current global row array; capture signed Stunned+8c/Scared+7c from
   the 160-byte source CharAnim record. Query exact constants
   `('AnimStancedAnim','SL__LIST_IPHONE')`. The original ANDS retains only
   bit200/100 for the branch: when absent, modifier is zero; when present, call
   `GetAnimStance` **3a53e0** and use its signed result. Add wrapping32 to the
   captured base, then store machine+28/Character+524 animation override.
5. Force requests `_SetState` **3c1938** with ID9/8, eventc35c/c35d and supplied
   payload; normal mode calls `RaiseStateEvent` **3c5684** with that event.
6. If requested mode is true, reload flags and OR800/400 **after** the transition
   callback. A transition may have overwritten the prior flags.

`NativeEffects32` borrows actual FSM/Character identity and projected authored
Stunned/Scared row arrays. Its providers must supply genuine AI flags, authored
table word, constants, stance, timer and registered-state operations. A nonzero
native provider status stops at the delivered prefix, without pretending a
missing state transition succeeded. A delivered original timer UINT_MAX is
distinct from a native TimerStore capacity/malformed diagnostic; a native bridge
must report those diagnostic failures explicitly through its status.

## Full recovered state-method bodies

The adapter includes these actual methods, with their deeper effects provided
synchronously rather than accepted as invented no-ops:

- Stunned Focus **3c3cc0** sets flags2202, captures the source row-array pointer
  before querying the table ID, recomputes the Stunned sequence using the same
  stance bit, and calls `CharAnimator.ANIM_Set` **3cacb0**. It queries the actual
  Character virtual+28 player predicate; a nonzero result writes controller+8
  lock1. Then `CancelSneaking` **3bc6b8**, reload physical pointer+2dc, unpin.
- Scared Focus **3c45c4** sets flags2240, recomputes the Scared sequence, sets
  it, produces a random heading, cancels sneaking, reloads body and unpins.
- The random point uses four ordered calls to `Random.GetRandom.clone1`
  **3c26a0**: bounds9998,9998,100,100. Each first result is signed-int→float32,
  multiplied by float bits38d1b717 then separately added to3951b717. Sign bits
  flip when the respective later signed result <=49. XYZ is (x,y,+0). The point
  goes to `v2Controller.Cmd_HeadTowards(Point)` **405374**. No normalization or
  float-fused shortcut is introduced.
- Scared OnEvent **3c2b28**, event23, regenerates that heading. All other events
  have an empty body. Stunned OnEvent **3c0030** is an actual `bx lr`.
- Stunned Update **3c549c**, after pending bit2 clears, calls
  `SM_SetIdleState(false)` **3c1a00**: writes machine+3c idle_suppressed0 and
  directly requests state3/event-1/null payload. It does not raise eventc352.
- Scared Update **3c4788**, after pending bit4 clears, calls animator+49c
  `ANIM_StopLoop(false)` **3c948c**. It does not directly enter Idle.
- Stunned Blur **3c3b4c** clears controller+8 lock and pins a present body.
  Scared Blur **3c4834** clears the controller heading-object target through
  **4053d0**, then reloads and pins a present body. Logging is outside these
  gameplay service observations.

The native `effect_focus`, `effect_event`, and `effect_body` functions preserve
these method bodies. They do not advance a world, dt, animator clock or timer.
Animation selection, controller commands, CancelSneaking, registered state
transition and physical pin/unpin are real receiver services. Their owner
identities, lifetimes and implementation must remain valid for synchronous
reentry; the borrowed FSM/State binding itself stays stable.

## StateInfo ownership gap and correction

**Correction to the earlier frozen native-FSM read-only prose:** the factories
for these classes do not allocate a private state class. Original executed
`GetNewState<CSScared>` **3c0534** returns the same symbol
`Singleton<CSScared>::s_inst` **9a2ad0** through GOT9996c4; Stunned **3c0550**
returns `Singleton<CSStunned>::s_inst` **9a2ad8** through GOT9991b0. The two
behaviors are shared singleton objects. `behavior_probe.py` repeats the actual
factory calls and binds their symbols. Its raw ELF word is zero before global
constructors; this probe does not claim startup/vtable initialization ran.

Per-machine StateInfo/map ownership and event/predicate registration are
separate. Original OnInit captures prove Stunned **3c8730** registers deathc358
→12, injuredc35a→11, knockbackc35b→10; Scared **3c861c** additionally registers
expiry2c→3, animation22→3, and stunc35c→9. Conditional member-function identity
and the actual StateInfo registry/state methods for other destinations remain
required dependencies. This module does not own that registry or register
fake callbacks, and the existing four-state module cannot accept IDs8/9.

Consequently the complete producer and recovered Focus/Update/Blur/Event bodies
are available, but full native effect entry, timer-expiry/event-map closure and
Crypt monster/rene gameplay are not claimed. Animation-row loading, real stance
and AI row providers also remain explicit live integration responsibilities.

## Evidence and integration

- Original producer probe: 2048 cases plus24 table gates, actual IsBoss bit
  extraction/GetCharAnimTableId and surrounding producer instructions.
- Original simple methods: 576 Update/Blur cases including body/lock bytes and
  heading callback removing the body before the subsequent pin check.
- Original Focus/event: 1152 Focus and24 event cases, actual separate float32
  arithmetic and sign flip, RNG-result/animation/controller/sneaking fixtures.
- O2 ARM64: 2648 producer/simple-body and1176 Focus/event comparisons; 6606+8943
  ordered services, zero mismatch. The two reports bind the same current helper
  library. Caller malformed guards are six and five.
- ASan/UBSan host: 125845 core checks and77273 Focus/event checks, zero findings.
  Real linked TimerStore verifies first start/partial elapsed/no restart before
  an explicitly rejected missing state owner. Real world/native body verifies
  Blur pin and two Focus unpins, retaining local center. Focus/event composite
  calls the actual linked source RNG eight times. Animation/controller/sneaking
  callbacks remain explicit fixtures in that host composition.

Production source addition is `character_native_effects.cpp`; parent owns CMake.
Suggested targets `character_native_effects_audit` and
`character_native_effects_focus_audit` use their same-named C++ test files,
actual world/Box2D public headers and dl; the latter also links game-data.
Treat recovered Box2D headers as system headers for the legacy unused-value
macro. Arguments are `effect-fixtures.bin` and `focus-fixtures.bin` here.

Reproduce with the direct Python interpreter and dependency PYTHONPATH used by
the previous native FSM audit: run the three original `*_probe.py` files, the
behavior probe, build `tools/build_character_native_effects_oracle.ps1`, then
run both `tests/character_native_effects*_differential.py --library
.local-inputs/character-native-effects/oracle.so` and both `*_host.py` runners.
After central integration the host runners accept `--main-linked`, producing
new dedicated main-linked reports rather than overwriting isolated proofs.
