# AISDefault collision-persist counter producer

Original ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The four-function manifest and instruction capture bind `OnCollisionPersist`
`0x3dbfa0`, `SM_IsMoving` `0x3c029c`, `SM_GetState` `0x3c01ac`, and
`Application::GetDt` `0x31f66c`. The differential executes all four original
bodies. The selected AISPlayerIPhone inherits this AISDefault collision method;
selected-class/vtable evidence is in the sibling character-script lifecycle
and update references. No character collision solver is supplied by this module.

## Exact source control flow

1. Read AIS owner at `+0x98`, call its embedded state machine `+0x4fc`
   `SM_IsMoving(false)`. State4 and state19 return true. State getter reads
   current-state pointer `SM+0x20`; null returns UINT_MAX. All other states return.
2. Cache owner current target `+0x408`. If collided identity equals that
   snapshot, return before virtual services. Otherwise call collided virtual
   `IsCharacter` at vtable `+0x24`.
3. If character, cache owner preferred target `+0x418` **before** first owner
   `IsPlayer` (vtable `+0x28`). If this first player query is false and preferred
   is null or differs from the cached current target, call owner AI `+0x3c8`
   `IsEnemy` (`0x3d574c`). True takes a tail call to `AI_SetTarget`
   (`0x3d6890`), collided argument, flags0, then returns. It does not accumulate
   collision time. The preferred/current values remain snapshots across calls.
4. Otherwise call owner `IsPlayer` again. A true second result followed by true
   `IsEnemy` calls `Character::CancelSneaking` (`0x3bc6b8`). Owner and embedded
   AI identities are read again between services, matching source reentry.
5. Zero persist argument returns. Nonzero persist calls collided `IsCharacter`
   a second time. If false, collided live type `+0xf4` must be2 or21.
6. Read current Application frame word `+0x74`. If equal to AIS stamp `+0xc0`,
   return. Read owner AI-paused byte `+0x3e0`; any nonzero byte returns.
7. Write AIS stamp `+0xc0` first. Capture old AIS collision counter `+0xbc`,
   execute `GetDt` (loads integer word Application `+0x8c`), and write the
   unsigned wrapping sum. It uses integer milliseconds, including high-bit
   values and UINT_MAX. No float seconds or generic frame increment is invented.

Service returns use full-word nonzero truthiness. Both virtual character calls
and both player/enemy queries can return different values in one invocation.
Service callbacks run synchronously; a nested collision can stamp/accumulate
first, making the suspended outer invocation reject the now-current frame.

## Application and counter ownership

`application-producer/` binds the original `_Update` `0x32c438` and outer
`Update` `0x32ccc4` captures. `_Update` reads/increments/stores Application
`+0x74` at `0x32c7e4/0x32c7ec/0x32c7f0`, after gameplay on the common return
path. The preceding branch into `0x32c92c` rejoins this increment. Outer Update
has application byte `+0xa4` and long-time-gap early-return gates; callers must
use the actual Application frame and `GetDt` values rather than assume every
render/host call increments them. This audit executes GetDt but does not execute
the full Application update, time-scale producer, or paused application FSM.
Existing frame-order reference records scene/physics/actor choreography.

AISDefault constructors zero `+0xbc/+0xc0`. Script update uses the same live
collision counter and owner paused byte: unsigned counter>199 triggers clear,
paused1, timer1000/event31 and source controller Stop; timer expiry clears the
paused byte. Those bodies are separately proved in character-script-update.
The collision module deliberately exposes these shared words for the owner to
bridge; copying divergent per-module counters would not represent the original.

## Native API and verification

`character_script_collision.hpp` has borrowed `ScriptCollisionState72`,
`ScriptCollisionObject16`, and synchronous `ScriptCollisionServices16`.
State exposes source identities, live targets, collision counter/stamp, paused
byte, resolved movement state, and exact Application frame/dt. Five named
services preserve virtual/AI argument identities and order: is-character,
owner-is-player, is-enemy, cancel-sneaking, set-target. Their backends remain
explicit; the module does not replace them with a single supplied decision.
Return1 means complete, -1 is an atomic malformed native boundary rejection.

Original ARM32 versus optimized ARM64: **4,096 cases, 6,839 ordered callback
snapshots, 236 nested callback cases, zero mismatches**. Cases cover state
0/4/19/20/null-state sentinel, target/preferred snapshots, type2/21/others,
changing virtual results, byte255 truthiness, persist0/1/255, repeated frame,
paused0/1/255, unsigned wrap, live owner/AI/target/type/frame/dt mutations, and
synchronous reentry at character/cancel/retarget services. Fixture callbacks
are declared boundaries and do not claim original enemy/sneaking behavior.

Host ASan/UBSan replays the identical gold, including callbacks and reentry,
plus **14 atomic native guards and3 sequential counter lifecycle checks**;
zero sanitizer findings. Gold `collision-fixtures.bin` SHA-256:
`9712cce184d8d00504545b7248950b2f4b3ac6a41340917410fcd609877cc6d3`.
Reports bind original/manifest/gold, production/test hashes, exact O2 ARM64
library, sanitizer executable, compiler command and frozen host build inputs.

Build oracle with `tools/build_character_script_collision_oracle.ps1`; run
`tests/character_script_collision_differential.py` with engine/library/manifest/
report/reference-output paths. Run `tests/character_script_collision_host.py
--rebuild` to freeze and audit the isolated source. A parent CMake target can
compile `tests/character_script_collision.cpp`, link the new production source,
and pass `reference/character-script-collision/collision-fixtures.bin` as its
sole argument. The initial isolated milestone changed no existing source,
CMake, renderer, APK, or emulator.

## Production world-library integration

After the isolated proof, parent authorized level-world CMake integration.
`character_script_collision.cpp` is now a production `dh2_level_world` source.
Host target `character_script_collision_audit` compiles the audit alone and
links `dh2_level_world`; it does not compile a private duplicate of the kernel.
`tests/character_script_collision_linked_host.py` builds that actual CMake target,
checks sanitizer configuration and actual `ldd` world-library resolution, then
replays the same original gold. It freezes source/header/CMake hashes across
the dependency tree before build and verifies them after build and replay;
executable/linked production DSO bytes are also stable through replay.

Main-linked ASan/UBSan replay passes **4,096 cases, 6,839 ordered callbacks,
236 nested cases, 14 atomic guards and3 sequential counter checks**, with zero
mismatches/findings. Report:
`reports/character-script-collision-main-linked-host-audit.json`.
World DSO SHA-256 at this integration snapshot:
`0640c225e0355d291bfa4fd2f4e3dbe290ed5f3e2d1dbfa3a2ce65e61c27fa79`.
This is host production-library evidence; no Android/APK/device claim follows.
Only level-world CMake changed during integration; renderer/actor/engine source
ownership stayed with the parent and other assigned workers.
