# Complete CharAI OnUpdate coordinator

`character_ai_update.hpp/.cpp` reconstructs the complete instruction body of
`CharAI::OnUpdate`0x3d1050. This is an additive coordinator; the actual active
AIS, Lua, zoning/world lists, GameObject position and visual services remain
explicit synchronous dependencies. The earlier `character_script_update`
module remains unchanged and retains its selected AISDefault body/prefix scope.

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The manifests and assembly here capture the complete wrapper, state predicates,
virtual implementations, zoning/visibility/position bodies, outer update gates,
and initial-position producers. `producers/xrefs.json` binds raw vtable bytes
and targeted initial-position instruction references. Its candidate vtable list
is broader than the two GameObject/Character tables used below; no inheritance
or runtime use is inferred from unrelated table names.

## Exact branch and reload order

1. Read AI active script+0x1c. If present, invoke its virtual+0x18 at0x3d1074.
   An absent active script still reaches the remaining owner work.
2. Reload AI owner+4. Query its embedded state machine+0x4fc using
   SM_IsIdle(false)0x3c0260. The actual SM_GetState0x3c01ac returns-1 if the
   machine's selected state pointer+0x20 is absent; otherwise it reads that
   state's first word. Idle(false) accepts states3,13 and18.
3. A nonidle owner also queries SM_IsAwaitingToSpawn0x3c0230, accepting17.
   Every other state takes DisableZoning, regardless of absent targets.
   Idle/awaiting owners also take that branch if owner+0x408 or+0x418 is nonnull.
4. The disabling branch captures owner visual+0x2d8 in r4 at0x3d10a4 **before**
   calling GameObject::DisableZoning0x38c600. After that synchronous call it
   invokes VisualObject::SyncVisibility0x4713d0 on the captured visual if
   nonnull. A callback replacing owner or visual does not change this subject.
5. Idle/awaiting with both targets absent returns if owner zoned byte+0x2ee
   is nonzero. Otherwise it invokes owner virtual+0xc4, IsZonable, at0x3d1118.
   A zero result returns.
6. After a nonzero result it reloads AI owner+4 and calls EnableZoning0x38c790.
   It reloads owner again and calls virtual+0x34, IsDead. A nonzero result
   returns. If false, reload owner and inspect updating byte+0x85. Nonzero
   returns; zero continues.
7. Pass the current owner's initial position+0x1450 by reference to
   GameObject::SetPosition0x393db4 with update-previous argument1. After that
   call reload owner, visual+0x2d8 and its node+8. If both exist, reload the
   current owner's three initial-position words and invoke node virtual+0xa4
   with a stack vector copy. The wrapper then reaches its epilogue.

There is no CharAnimator::Update call in this OnUpdate body. Scene/timeline
sampling and the later Character animator update are distinct frame producers.
The native coordinator preserves all pointer capture/reload points and copies
position words without arithmetic, retaining signed zero/NaN/infinity payloads.
The request's vector is the borrowed service's entry snapshot; internal helper
interleavings are not reconstructed by copying this command.

## Live producers and backend requirements

Character vtable0x965f30, address point+8, resolves virtual+0xc4 to
Character::IsZonable0x3a36e4 and+0x34 to IsDead0x3a2ed4. IsDead reads
Character+0x1449. IsZonable invokes actual IsPlayer virtual+0x28, then rejects
faerie type3 through IsFaerie0x3a3094; otherwise it returns MeetCondition0x38ab60
(the captured base implementation returns1).

Character::IsPlayer0x3a49f0 queries GetCharType0x3a3054. That reads resolved
AI row+0x38 through GetCharAI0x3a3024, whose row stride is0x44. Type1 is player;
type0 instead uses the original interned-name comparison; other types are not
player. The separately cache-bound `character-script-selection/authored-inputs.json`
shows selected row44 Player has Type1 and script `__player__`. Thus genuine
Prince IsZonable is false. This is a recovered virtual producer, not a generic
always-false zoning service: other native characters require their own actual
row/type/name/condition producers. Twenty-seven focused cases execute the actual
Character virtual bodies against supplied resolved AI rows1/2/3.

GameObject EnableZoning/DisableZoning are captured but not reimplemented here.
They manage zone/world registration, zoned+0x2ee, in-zone+0x2f0, visual visibility
and virtual setUpdating+0x3c. ObjectBase::setUpdating0x33dcf0 writes byte+0x85.
The callback must refresh `zoned` and `flag85` after those genuine operations;
forcing either value would bypass the subsequent source restore gate.
SyncVisibility reads its own bound owner, enabled+0x80, IsZonable, zoned and
in-zone state, and invokes actual visual SetVisible. It is not an unconditional
visible toggle.

GameObject::SetPosition0x393db4 updates relative/absolute bounds, position,
physical body transform, visual sync and, when argument1, destination. The
service must bind that complete actual native operation; this module does not
replace it with only a scene-node setter. The final explicit node setter is a
separate call and occurs only after the owner/visual/node reloads.

Initial position is not an invented zero/current-frame cache:
Character::SetInitialPosition0x3a58f4 stores the supplied XYZ at1450/1454/1458,
then calls PFWorld::GetFloorHeightAt0x525508. A successful floor query replaces
initial Z. InitPost invokes this producer with current owner position+0x160 at
0x3b53d4, then SetPosition(initial,true)0x3b53e8. InitSpawned supplies the spawn
payload to the same producer0x3b37c0 and invokes SetPosition(initial,true)
0x3b37d4. The manifest/xrefs also identify constructor, serialization/network
and state-consumer references; those are not all recovered initialization APIs.

Outer CharAI::Update0x3cfbf4 remains a separate source boundary. Before calling
virtual OnUpdate+0x18 at0x3cfd18 it skips when paused+0x18 is nonzero. A nonzero
owner controller+0x378 byte+9 proceeds directly; otherwise a nonzero debug byte
or controller byte+8 skips the update. It then checks owner+0x520 bit0x100 and
IsZonable/zoned/in-zone gating. It then marks owner+0x88 and executes target/
master/loading steps0x3cb908,0x3cc5a4,0x3cf3f0. These services and gates must
not be skipped merely because this OnUpdate coordinator is available. Character
Update's established frame order remains timers→AI→FSM→animator→GameObject,
after scene animation and physical Step, with its earlier eligibility gates.

## Stable native API and contracts

`AIUpdateState80` projects active/owner/target408/target418/visual/visual_node
identities, selected state and machine-present bit, zoned/updating bytes, and
initial XYZ. Services that replace the owner must refresh this projection
synchronously. The selected AIS may use the existing source helper; it must
retain its timer/controller and any Lua/dependency services.

Eight `AIUpdateRequest32` variants name active update, disable zoning, sync
visibility, IsZonable, enable zoning, IsDead, SetPosition and node SetPosition.
`AIUpdateServices24` has a service-availability mask and callback status separate
from its full32-bit query result. Missing services return2; callback failures
return3 and retain completed external effects. They never complete as no-ops.

`dh2_character_ai_update(out,state,services)` returns0 after the source epilogue.
Malformed top-level inputs return1 atomically before writes/calls. The bounded
caller contract requires valid nonoverlapping aligned objects, owner identity,
coherent post-callback projections, byte fields0..255, machine-present0/1,
zero reserves and only eight known mask bits. `AIUpdateResult16` reports attempted
phase(service+1), last service and entered callback count; completed phase is9.

## Verification and invocation

`character-ai-update-arm64-differential.json`:4,100 actual complete original
OnUpdate versus optimized O2 ARM64 cases,8,924 ordered service-entry snapshots,
27 genuine Character virtual-producer cases,14 atomic malformed rejections and
eight explicit unavailable-service checks; zero mismatches. Cases cover active
absence/presence, all state gates, absent machine, both targets, byte0/1/255,
visual/node nulls, full32-bit query results and synchronous owner/visual/target/
state/updating/saved-position mutations. IEEE position words compare exactly.

Gold SHA256:
`d3e4bd4e5577798e6896d81f18ce9a66e04711d003c6f335b37bd50fa1586065`.
`character-ai-update-host-audit.json` replays that gold with ASan/UBSan and binds
the actual CMake audit executable and production world DSO. Both wrapper and
selected script kernel resolve by dladdr to `libdh2_level_world.so`. A composed
case executes actual source-built AISDefault update, its real pause stores and
timer→owner reload→Stop order, then verifies that the wrapper uses the changed
owner/state for its visibility branch. Timers/Stop/zoning/position remain named
fixtures. Eight runtime failure-prefix cases verify explicit status and retained
callback effects. There are zero sanitizer findings.

Parent-owned CMake target: `character_ai_update_audit`, linked to
`dh2_level_world`; production library includes `character_ai_update.cpp`.
No CMake, renderer or existing script module was edited by this worker.

```
cmake --build /home/adampalace/dh2-world-build --target character_ai_update_audit
ASAN_OPTIONS=detect_leaks=1:abort_on_error=1 UBSAN_OPTIONS=halt_on_error=1 /home/adampalace/dh2-world-build/character_ai_update_audit port/level-world/reference/character-ai-update/update-fixtures.bin
python port/level-world/tests/character_ai_update_host.py --main-linked
```

The Python host runner's `--main-linked` mode executes the already-built CMake
target, records compiler commands and sanitizer configuration, and binds source,
gold, report, executable and actual DSO hashes. Its standalone mode builds only
a new audit DSO and still imports the selected kernel from the actual world DSO.
The original differential uses the cached Unicorn/ELF dependencies and
`tools/build_character_ai_update_oracle.ps1` for its NDK29 O2 ARM64 oracle.
No Android build, emulator action, full AI/FSM/Lua/world backend, complete
original frame, GPU or APK parity is claimed by this component proof.

Production source is stable:
header SHA256`35efb90d8a55675e48a602d21dcce1ddbdb1aa211a6fca0acddcaa66d77e2758`;
cpp SHA256`ba39216c5ab26eb3a3f04a165c57d7644b819548fb387332c8e8076f68662e39`.
