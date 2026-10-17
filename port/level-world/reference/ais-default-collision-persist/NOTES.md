# Persistent collision producer for the selected AIS

`ais_default_collision_persist.{hpp,cpp}` maintains the complete original
`AISDefault::OnCollisionPersist(GameObject*,bool)` caller at `0x3dbfa0`
(396 bytes). The manifest pins 14 original function ranges, five primary
vtable prefixes and the existing Application frame-word increment fragment.
One new caller orchestration is claimed; no new complete callee bodies or
native collision routing are claimed. Frozen AIS OnUpdate source is unchanged.

## Actual counter provenance

The caller's qualifying persistent collision path compares AIS+c0 with the
captured Application+74 frame word. On a different frame and fresh owner
byte+3e0 equal to0, it stores that frame to+c0, **then** captures AIS+bc,
queries GetDt on the same captured Application and stores the wrapping32-bit
sum to+bc. Neither delta nor counter is clamped. A normal GetDt callback
changing+bc is overwritten by the captured-counter sum; a callback changing
+c0 is retained. A port GetDt error preserves the already stored+c0.

Application::_Update(int) contains `load+74; add1; store+74` at
`0x32c7e4..0x32c7f4` (16 bytes, last instruction starts at `0x32c7f0`). This
establishes that+74 is a frame word rather than an elapsed clock. This producer
does not reconstruct Application startup or that containing update branch.
AISDefault::OnUpdate reads the **unsigned**+bc counter and, when above199,
clears it before AI_PauseUpdate(1000) and Cmd_Stop. The new State borrows the
same frozen `ais_external_update::State` owner/counter storage and keeps+c0
separately. Initial values and lifetime come from the genuine AIS owner; this
caller does not fabricate construction defaults or increment on every frame.

## Full caller ordering

1. Fresh owner `SM_IsMoving(false)` is queried first. Its 52-byte source helper
   accepts state4 or state19 with argument0. Other states return immediately.
2. Capture fresh owner target+408. A collision with that same target returns
   before peer queries/counter work. A null peer is valid only on a source path
   which returns before dereferencing it; otherwise the port rejects it.
3. Call peer virtual IsCharacter at+24. If true, capture a fresh owner's
   master+418 **before** querying that owner's virtual IsPlayer(+28).
   A false player result plus `(master==null || master!=captured target)`
   queries a fresh owner's AI_IsEnemy(peer). True tail-calls fresh owning
   AI_SetTarget(peer,force=0), returning before counter logic.
4. Otherwise independently query fresh owner IsPlayer again. A truthy result
   queries fresh owner AI_IsEnemy(peer); truthy calls fresh owner CancelSneaking.
   These actions occur even when the input collision flag is false.
5. False collision flag returns. A truthy flag calls peer IsCharacter a
   **second independent time**. True qualifies; false requires directly read
   peer type+f4 equal2 or21. All other types return.
6. Capture Application and frame+74. Equal+c0 returns before owner-flag/GetDt
   reads. Otherwise read fresh owner byte+3e0; truthy returns. The counter
   store sequence then runs as described above.

The master-pointer ARM subtraction is exactly the null/equality predicate
above. It is not an integer object-ID approximation. Raw query words retain
truthiness; the second IsPlayer/IsCharacter query is not replaced by its first
result. Owner replacement by a genuine callback is observed at each later
source load. The selected peer and captured target/master remain retained.

## Views, services and live routing boundary

OwnerFacts, PeerFacts and Application are borrowed adapter views, not native
memory overlays. Identity-to-view/global resolution is **pure metadata**,
not an inserted game callback. It may return a port failure but cannot mutate
game state. Genuine moving/virtual/enemy/target/sneaking/GetDt services are
synchronous. Original-AI layout offsets are not added to arbitrary port IDs;
owning Character providers resolve the actual embedded AI.

CharAI::OnCollisionPersist at `0x3d0e10` (36 bytes) freshly loads selected AIS+1c,
skips null, otherwise forwards peer/flag through AIS virtual+c0. Both AISDefault
and AISExternal primary tables map that slot to `0x3dbfa0`; therefore the plain
`monster` external-script selection inherits this producer. The existing
Character/CharAI event dispatcher routes persistent collision events0x39/0x3a
(true/false) through CharAI+c0 after its controller policy gate. This module
does not bypass that event gate or bind physics callbacks directly.

All vtable offsets use symbol+8 address points. Hashes are translated through
ELF PT_LOAD ranges, including data addresses. AI_IsEnemy/SetTarget,
CancelSneaking, actual collision ownership and Application/frame production
remain genuine owning dependencies; no no-op substitute is introduced.

## Host/original verification and lifetime

The original comparison executes the complete396-byte caller, the actual
52-byte moving helper,20-byte SM_GetState and8-byte GetDt. Concrete virtual
Character/Player, AI_IsEnemy/SetTarget and CancelSneaking are explicit fixture
providers. Mutation hooks test caller read/store order; they do not claim those
original callee bodies perform the injected fixture effects. Pure view traces
match the exact source direct field reads. The meaningful host composition
raises the shared+bc counter above199 through this producer, then calls the
unchanged AIS OnUpdate kernel and verifies its pause/reset path.

One thread retains fixed AIS identity/projection, every owner/peer/Application,
borrowed views and service context through return, including replaced backing.
Callbacks may change live owner/fields but cannot replace State::ais, rewrite
identity keys, destroy borrowed values, overwrite output/services or reenter
the same State. Independent calls may nest. Alignment, known aliases, matching
view identities and byte-width checks are port guards. Missing/error/throwing
providers keep earlier source actions and stores; no rollback or cleanup is
invented. No full AI frame, physics wiring or Android playtest is claimed here.
