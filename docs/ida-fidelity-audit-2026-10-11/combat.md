# Combat batch fidelity audit (2026-10-11)

## Progress

- last_address_done: 0x52a504 (all 150 batch rows triaged; deep-read: ObjectSearcher search/filter/sort, CSAttack/CSSkill/CSCast, Projectile/LaserTypeProjectile/ProjectileManager, Structs::Projectile::read)
- groups_done: ObjectSearcher, CS* states, Projectile*, POProjectile, ProjectileManager, GameObjectDamager/CMsgAttackResult (boilerplate)
- next: nothing mandatory. Optional follow-ups: dump ProjectileTable rows to learn which rows use laser/targetLock/stayOnFloor/timeBetweenHit; trace Windows auto-target (OOI) origin
- range items: see weapon-range.md (not analysed here)
- Method limit: ProjectileTable/AITable binary data was not dumped (recovered/ source-data is absent in this checkout), so "which rows are affected" is unknown.

## Summary

Windows gameplay lives in `port/windows-foundation` (targeting.cpp, target_selection_runtime.cpp, playable_actor_world.cpp, combat_session.cpp, features/effects/runtime_session_projectile_*). The literal ObjectSearcher reconstruction in `port/level-world/character_target_search*.cpp` is used by android-native/level-world tests and is NOT in the Windows path, so Windows targeting is an independent re-implementation.

Counts (approximate, by mapping-table row): faithful 8, partial 14, divergent 1, missing 5, unclear 5, n/a 12 groups, intentional-adaptation 0.

Top risks:
1. Projectile sim implements only the straight timer/speed/damping/maxDistance subset: no homing (TargetLock), no PF floor/wall/room-gated expiry, no StayOnFloor, no DisappearOnHit/TimeBetweenHit, no laser class behavior (F1, F3).
2. Projectile contact with non-character objects (barrels, destructibles) is a hard error instead of a hit (F2).
3. Windows target ordering/filtering differs from ObjectSearcher: distance-only sort (no frontal mode, no characters-first), search origin is feet not TargetPosition, no sneak/detection gate, no dead-friend (resurrect) mode, no rectangle search (F6-F8).
4. level-world IsCharacterValid compares flags to 0x80000001 whereas the ARM `CMN R3,#0x80000001` means 0x7FFFFFFF ("all"); not in Windows path (F9).
5. AttackDelay timer unit/ownership assumed ms from AITable[+4] (F10, unclear); CSSkill state flags unverified (F11).

## Findings

### F1. Projectile homing, StayOnFloor, wall/floor expiry not simulated (high if shipped rows use them; confidence medium)
- IDA 0x3e51b0 Projectile::Update: if ProjectileTable row byte +53 (TargetLock) and a tracked target (this+900) exists, destination = that target's GetTargetPosition every frame; otherwise destination = spawn heading*1000. After GameObject::Update: `speed -= damping*dt/1000`, `timerMs -= dt`; OnExpire(1) when timer<=0 or speed<=0; OnExpire(1) when distSq from spawn >= maxDistance^2 (if maxDistance>=0). Then floor test via PFWorld::GetFloorHeightAt(pos): floor not CanPathOn and not flag 0x2000000 => OnExpire(2) (wall impact); floor ok but height-lock (this+932/928) and floor Z above lock => OnExpire(2); no floor and no room => OnExpire(0) (silent); room flag&1 => OnExpire(2). OnExpire (0x3e5078) with row byte +52 (StayOnFloor) snaps Z to floor height first.
- Port: `port/windows-foundation/features/effects/runtime_session_projectile_v1.cpp` update() (about lines 470-545) advances along a fixed `forward`, applies the same speed/damping/timer/maxDistance rule (matches IDA; damping*seconds == damping*dt_ms/1000), but never reads `target_lock`, `stay_on_floor`, `max_rotation` (parsed in runtime_source_projectile_v1.cpp:47-62 only). Wall/floor expiry is delegated to physics-body contacts (runtime_session_projectile_contacts_v1.cpp, category 32 mask 0x51f), not the PF floor/room test. Expiry impact is always type 1 (expire_fx/sound); the type-2/3 object/wall FX ints (row +44/+48 with fallback +20/+24, per HandleImpactFX 0x3e4db0) are never chosen.
- Consequence: homing projectiles fly straight and miss; wall/room-edge explosions and their FX/sound differ.
- Fix: implement TargetLock destination tracking and the three-way floor/room expiry from level PF data; map expiry type to FX/sound as HandleImpactFX does.

### F2. Non-character projectile contact is an error, not a hit (medium-high; confidence high)
- IDA 0x3e55b4 Projectile::OnCollision: for non-Character GameObjects (when no homing target is set) falls through to the hit-record path (pending flag 977, callback this+952), so projectiles hit breakables/objects; FX via HandleImpactFX(go,...) 0x3e4eec: character => type 4, else type 3.
- Port: runtime_session_projectile_v1.cpp contact() returns `fail("Non-character projectile interaction needs its source object activator")` when `!character_peer`.
- Consequence: a ranged shot at a destructible hits the failure path instead of damaging/triggering it with object-impact FX.
- Fix: route non-character peers to the object interaction owner and emit impact type 3.

### F3. DisappearOnHit / TimeBetweenHit / Laser class ignored (medium; depends on data)
- IDA ProjectileManager::Spawn 0x3e701c picks the class by row byte +28 (laser). LaserTypeProjectile::Update 0x3e3fcc: beam persists; this+992 countdown reloaded from row+56 (TimeBetweenHit) after each damage callback (this+956); row byte +5 (DisappearOnHit) or timer<=0 ends it (DeSpawn type 1). LaserTypeProjectile::OnCollision 0x3e3e2c latches the first Character target (+900) and plays row+20 FX at it; beam endpoints are updated every frame (vt+164) from owner/projectile nodes.
- Port: runtime_source_projectile_v1.cpp parses `laser`, `time_between_hit_ms`, `disappear_on_hit`, but `launch_state5` does not gate on `laser`; a laser row would run as an ordinary one-shot projectile. After a hit the port sets `live=false` unless `source_outcomes&3` (continue). In IDA the continue/despawn choice is the callback result (0 => destroy, 1 => continue), with DisappearOnHit only in the laser path.
- Consequence: beam attacks (if any shipped row is a laser) behave as single projectiles; piercing may differ.
- Confidence: low-medium (laser rows not enumerated).
- Fix: fail loudly for laser rows or implement the interval; verify the continue rule.

### F4. Projectile Update ordering (faithful for straight-line subset)
- IDA: pending-hit callback (977/956) first, clear dead tracked target, set destination, GameObject::Update (moves with the previous speed), then damp/timer/range. Port update(): process_pending first, move with last frame velocity, write new velocity, damp, expire checks. Order matches.

### F5. Spawn origin and spread overload (unclear)
- IDA Projectile::SetInfo 0x3e58cc: spawn = owner TargetPosition, overridden by the visual node "projectile_node" absolute position when present; heading = owner look vector (sin(rot), -cos(rot), 0); non-homing destination = heading*1000 from TargetPosition (with the bool a8 and a node present, heading is node-to-owner XY direction). Float overload 0x3e57cc rotates the heading by a8*0.017453 (spread).
- Port: origin/forward come from the caller-supplied `request.pose` loan. Not verified that the caller uses projectile_node and Z=0 heading, and no spread-angle equivalent was found in this runtime.

### F6. Target sort modes (medium-low)
- IDA TargetSorter::_sortClosest 0x38d570: character flag first, then smaller distance (distance = |center-origin| - targetRadius - owner AI melee radius). _sortFrontal 0x38d5b4: angle instead of distance. Resort 0x4a35b4 switches mode.
- Port targeting.cpp candidates() sorts only by edgeDistance with actor-id tiebreak; no frontal mode. Only characters are candidates, so the characters-first rule is moot. Skills that use frontal ordering (closest to facing) pick by distance instead.

### F7. Search origin and degenerate angle (medium-low)
- IDA TargetList::Search 0x4a2f34 and overloads 0x4a3428/0x4a33c8: origin = reference object's GetTargetPosition (this+388 if flagged, else position this+352; 0x3935dc); accept if dist<=range and (cone>=3.1416 or cone>=|angle|); angle = acos(dot/(|a||b|)) in 3D against look vector. Zero delta gives NaN, which fails the cone test.
- Port targeting.cpp eligible(): origin = actor feet position, target point = position+targetOffset; zero distance accepted (`distance>0` guard). Cone formula/forward vector (sin,-cos,0) and `edge = dist - radius - sourceRadius` match.
- Consequence: small distance bias when searcher and target have different target-position offsets (big bosses/flyers).
- Fix: use the searcher TargetPosition as origin.

### F8. Filter gates absent in Windows targeting (medium-low; confidence medium)
- IDA search loop requires: candidate != self, visible byte (+138), zone gate (+750/+752), IsInteractive(vt+136) then `_IsCharacterValid` 0x4a1ab8: stealth gate (ref+4884 detection < cand+4880 sneak => reject), mask bits 0x80 player, 0x10 merchant, 0x20 cleaner, 0x40 talk flag, 1 enemy (not dead, AI_IsEnemy, not both players), 2 neutral (not dead), 4 friend, 8 DEAD friend (resurrect; owner state !=15, target state !=16). Non-characters use _IsGameObjectValid 0x4a1950 (interaction type ==8 or !=-1).
- Port: eligible() requires target alive; relation mask + traits (visible/targetable/inAllowedZone) + capabilities. No sneak/detection compare in the player path (an enemy-side `search_sneak_gate` exists at features/enemy_ai/monster_decisions.cpp:13, not audited here); no dead-friend mode; non-character object searches and SearchRect (0x4a2858, width-bounded search) have no Windows equivalent found.
- Consequence: player auto-target ignores enemy stealth; no resurrect/dead-ally targeting; line/rect skills cannot use the original width search.

### F9. level-world IsCharacterValid constant misread (low; not in Windows path)
- ARM 0x4a1af4 `CMN R3,#0x80000001` => flags == 0x7FFFFFFF (matches decompile). `port/level-world/character_target_search.cpp:50` tests `flags==0x80000001u`. Callers pass masks such as 128/89, so "all" never fires. Fix: compare to 0x7FFFFFFF.

### F10. AttackDelay ownership/units (unclear)
- IDA CSAttack::OnBlur 0x3c3f74: if Character::GetAttackDelay() (AITable[charAIId] +4, 0x3a3...) != 0, CharTimers::TMR_Start(delay, 0, id 42) on leaving the attack state; OnFocus sets flag 330|=1 when delay != 0.
- Port combat_session.cpp:1382-1383 `cooldown_seconds = ai->attack_delay*0.001`, timing attack_departure (combat_system.cpp:221), gating `request()` at combat_session.cpp:891. Departure timing matches; ms unit and that timer 42 expiry (not an AI decision) re-enables swings are assumptions.

### F11. CSSkill/CSCast state flags and events (partial)
- IDA CSSkill::OnFocus 0x3c4480: flags(+1312)=25409; clears 0x140 in +1320; byte+1364 sets 0x100; non-boss, non-miniboss monsters get 0x10000 (interruptible); OnBlur clears 0x10000 for them, syncs last target, stops movement, raises event 31, starts timer 48 (10 ms) when 0x100. CSSkill::OnEvent: anim event 40 "is_stoppable" sets 0x8000. CSCast: flags 25345, events 32/33, speed 1.0.
- Port: is_stoppable at port/level-world/character_skill_state_v4.cpp:52 (Windows linkage not confirmed). The 0x10000 monster-interrupt flag and 0x100/timer-48 logic were not found in features/generic_skills.

### F12. CSAttack facing and speed sync (faithful)
- IDA CSAttack::OnUpdate 0x3c14f8: every frame LookAt(target) and anim speed = PROPS_GetAttackSpeed (resync if |delta|>=0.0001). OnEvent 26: LookAt target position (ranged without target: SetHeadingDirection). Port: combat_session.cpp:442, 788 request LookAt on admission and active update; PROPS_AttackSpeed48 multiplier at :1326. Hit timing is animation-marker driven (original_actor_animation_events.cpp). Judged faithful, not exhaustively traced.

## Mapping table

| IDA address | IDA function (or group) | port location(s) | status | note |
|---|---|---|---|---|
| 0x31f444,0x31f44c,0x3200ac,0x3242d0,0x327a40,0x3af330 | CMsgAttackResult (GetDataPtr/Size, dtors, SetProperties, Create) | none | n/a | multiplayer message; no single-player consequence |
| 0x38aabc,0x38b2cc,0x38c900,0x38d18c,0x38d608,0x38d60c,0x38d730,0x38d764,0x4a18a4,0x4a19c4,0x4a1fcc | ObjectSearcher list dtors | n/a | n/a | boilerplate |
| 0x38d4a4-0x38d55c,0x38d7bc,0x38d7d4,0x4a17fc-0x4a1898,0x4a18a8-0x4a18fc,0x4a19f8,0x4a1cc8,0x4a1cfc,0x4a20f0 | Character/Player/Room/Backup list iterators | Windows uses TargetRegistry vector | n/a | iteration plumbing |
| 0x38d568 | TargetSorter::_sortNoSort | targeting.cpp | n/a | |
| 0x38d570 | TargetSorter::_sortClosest | targeting.cpp:25-28 | partial | F6 |
| 0x38d5b4 | TargetSorter::_sortFrontal | none in Windows | missing | F6 |
| 0x4a1ab8 | TargetList::_IsCharacterValid | targeting.cpp eligible; target_selection_runtime.cpp relation; level-world/character_target_search.cpp character_valid | partial | F8 (Windows), F9 (level-world constant divergent) |
| 0x4a1950 | TargetList::_IsGameObjectValid | none | missing | non-character target types not searchable |
| 0x4a191c | TargetList::SetRefObject | target_selection_runtime.cpp bind | faithful | |
| 0x4a2f34,0x4a33c8,0x4a3428,0x3d0020 | TargetList::Search (range+cone) | targeting.cpp eligible | partial | F7 |
| 0x4a2858,0x4a2e70,0x4a2ed0 | TargetList::SearchRect | none | missing | F8 |
| 0x4a2730,0x4a348c | TargetList ctor | target_selection_runtime.cpp | n/a | |
| 0x4a35b4,0x3d015c | Resort / SetSortType | none | missing | F6 |
| 0x4a36b0,0x4a1a38,0x38f53c,0x38f72c,0x52a504 | BackupResults/FlushBackup/GetResultsBackup/SearchFailCache | features/combat target retention | unclear | not compared |
| 0x3c0010,0x3c0014,0x3c001c,0x3c0928,0x3c095c,0x3c0990,0x3c0018,0x3c0020,0x3c0024 | CS* dtors/stubs | n/a | n/a | |
| 0x3c1288 | CSAttack::OnEvent | combat_session.cpp; original_actor_animation_events.cpp | faithful | F12 |
| 0x3c14f8 | CSAttack::OnUpdate | combat_session.cpp:788, :1326 | faithful | F12 |
| 0x3c3f74 | CSAttack::OnBlur | combat_session.cpp:913-935; combat_system.cpp:221 | partial | F10 |
| 0x3c404c | CSAttack::OnFocus | combat_session.cpp; original_attack_sequence.cpp | faithful | anim selection not re-derived |
| 0x3c8284 | CSAttack::OnInit | actor lifecycle transitions | unclear | event->state registrations not compared |
| 0x3c0ac8 | CSSkill::OnEvent | level-world/character_skill_state_v4.cpp:52 | partial | F11 |
| 0x3c434c,0x3c4480,0x3c8438 | CSSkill OnBlur/OnFocus/OnInit | features/generic_skills/* | partial | F11 |
| 0x3c3934,0x3c39d0,0x3c85b8 | CSCast OnBlur/OnFocus/OnInit | features/generic_skills/runtime_skill_cast_* | unclear | |
| 0x3e3df4-0x3e3e1c | Projectile::Is* getters | n/a | n/a | |
| 0x3e3e24,0x3e4da8 | GetSpeed | runtime_session_projectile_v1.cpp (speed=row+64) | faithful | |
| 0x3e51b0 | Projectile::Update | runtime_session_projectile_v1.cpp update | partial | F1, F4 |
| 0x3e3fcc | LaserTypeProjectile::Update | none | missing | F3 |
| 0x3e3e2c | LaserTypeProjectile::OnCollision | none | missing | F3 |
| 0x3e55b4 | Projectile::OnCollision | runtime_session_projectile_v1.cpp contact | partial | dedicated(+4)/friendlyFire(+16) faithful; non-character divergent (F2) |
| 0x3e5078 | Projectile::OnExpire | update() expiry push | divergent | F1 |
| 0x3e4db0,0x3e4eec | HandleImpactFX | runtime_session_projectile_render_v1; impacts packet | partial | type 3 object FX absent |
| 0x3e58cc,0x3e57cc,0x3e4570,0x3e449c | Projectile/Laser SetInfo | launch_state5 | partial | F5 |
| 0x3e4fe4,0x3e5d94-0x3e5edc,0x3e4ac0-0x3e4bc8 | SetManager/ctors/dtors/thunks | n/a | n/a | |
| 0x3e60fc,0x3e6158,0x3e68b0,0x3e6920,0x3e715c,0x3e63c4,0x3e6418,0x3e65e0,0x3e67d8,0x3e646c | ProjectileManager ctor/dtor/vector helpers/Flush | n/a | n/a | pooling |
| 0x3e61b4 | ProjectileManager::DeSpawn | update() erase | faithful | |
| 0x3e693c | ProjectileManager::_Create | n/a | n/a | pool; LTProjectile vs Projectile |
| 0x3e6edc,0x3e701c | ProjectileManager::Spawn | launch_state5 | partial | row range check yes; laser class ignored (F3) |
| 0x3e71cc | ProjectileManager::PreCache | runtime_session_projectile_render_v1 | unclear | no gameplay effect expected |
| 0x47046c,0x470480,0x4704b4,0x470520,0x4706ec | POProjectile | runtime_session_projectile_contacts_v1.cpp | partial | filter bits faithful; non-char peers F2 |
| 0x4a7b34,0x4a7bd0,0x4b6494,0x4b662c,0x4bb978 | Arrays::GameObjectDamager | level-loader/game_object_arrays_owner_v81.cpp | n/a | table loader |
| 0x4c6c48,0x4c6c4c,0x4c6c50,0x4ce17c | Structs::Projectile dtor/finalize | n/a | n/a | |
| 0x4edbb8 | Structs::Projectile::read | runtime_source_projectile_v1.cpp:35-62 | faithful | field order/types match (+4 dedicated, +16 friendlyFire, +32 maxDistance, +60 timer, +64 velocity, +68 damping) |
