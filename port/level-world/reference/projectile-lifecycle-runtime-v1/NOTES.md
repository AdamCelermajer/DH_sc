# Projectile lifecycle

Source: `libDungeonHunter2.so`, SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Factory, constructor, SetInfo, Update and OnExpire details below were checked
against the pinned ELF with ARM disassembly. OnCollision also has retained live
IDA/REA evidence at `tmp/projectile-collision-ida-live.md`; do not interpret
that as live Android or gameplay validation.

- `ProjectileManager::_Create` `0x3e693c` scans separate ordinary/laser pools
  from each cursor, wraps to zero, reuses the first inactive row, or grows
  0→1→2x with per-class serial/name; ObjectManager Add and active-row commit
  precede SetManager. PreCache reserves and creates 10 shells per class, then
  DeSpawns them; the native shell constructor is still unbound.
- `ProjectileManager::Spawn` `0x3e6edc` / `0x3e701c`: id bounds, `_Create`,
  SetUpdating(true), matching float/bool SetInfo overload, then active byte.
- `Projectile::Update` `0x3e51b0`: handles queued `+0x3d1` collision first;
  with stored `+0x3cc` target it calls `_ProjectileHit`, then either clears both
  collision gates when that callback returns 1 or runs impact FX and
  `ProjectileManager::DeSpawn` when it returns 0. Otherwise it updates the
  destination/GameObject, motion/time/range and PFWorld floor/path/room gates;
  terminal conditions call OnExpire followed by DeSpawn.
- `Projectile::OnCollision` `0x3e55b4`: active/exit and target filters, stores
  target/point, optional callback, then handles callback result.
- `Projectile::OnExpire` `0x3e5078`: optional row+52 floor correction, clear
  +0x3cc, set +0x3d1, GetTargetPosition, HandleImpactFX. It does not despawn.
- `ProjectileManager::DeSpawn` `0x3e61b4`: SetUpdating(false), Stop, clear
  active byte, then release the source pool slot; the ObjectManager object stays.

The selected port retains Projectile payloads on the canonical ObjectManager,
uses the shared PhysicalWorld for BDAE-bound circle contacts, and dispatches
accepted contacts through source relation/check callbacks. The pending `+0x3d1`
branch runs from the existing ordered ObjectManager update. Its retained VM
callback invokes `OnDelayedSkill` with collision target and Projectile identity;
source ReturnValues decide resume versus impact FX/DeSpawn. Timer and vertical
speed expiry call OnExpire through the same Level floor owner and AnimatedFX
manager, then release the existing pool slot and destroy the shared-world body.
The selected floor query is the same PFWorld-backed Level collision world.
Update queries that world with include-special=1 only after lifetime/speed
checks pass and the squared-distance gate allows continued flight. A valid
floor is tested against the source projectile capability mask (flying and
swimming, bits 1|2), with the source wall-flag exception. If there is no floor,
`GetRoomAt` scans the same Level's inclusive room bounds and accepts only room
flag bit 0 clear. `PFRoom::PFRoom @0x521540` initializes `+0x24` flags to zero;
the selected path has no call that mutates them. `CollisionRoom::source_flags`
projects that field, and its current floor importer sets zero from this source
constructor fact. Expiry sound and source visual/rotation behavior remain
partial.

`projectile_factory_owner_v1` now validates the source type map, reserves the
exact generated name/handle in the borrowed canonical ObjectManager, then
accepts a single factory-produced object into the lifecycle kernel. Serial and
ProjectileManager identity remain caller inputs. The unbound production
callback is the source `ObjectManager::GetNewObject` factory provider for
`Projectile`/`LaserTypeProjectile`: it must construct the real class, initialize
its inherited/source fields and vtable, and return its lifetime-owned product.
The current app binds a semantic projectile product to the Level ObjectManager,
shared PhysicalWorld and selected script VM. It does not instantiate the original
ARM32 Projectile class or install its binary vtable.

| Needed provider | Retained source contract | Port gap |
|---|---|---|
| Factory product | `GetNewObject` `0x34b520` -> Add `0x34b270`; `GetNewInstance<Projectile>` `0x340d14` allocates 980 bytes with GO_ID 9 and calls C1 `0x3e5e40`; Laser thunk `0x340cf0` allocates 996 bytes with GO_ID 10 and calls C1 `0x3e4b6c`. | Map is metadata only; no constructor/provider execution or object lifetime owner. ARM32 sizes/GO_IDs cannot be used as a 64-bit C++ allocation/layout. |
| State/vtable | Projectile C1 calls GameObject C2 and initializes source fields: `+0x374=-1`, `+0x378/+0x380/+0x384=0`, `+0x388..+0x39c=0`, `+0x3a4=0`, `+0x3b8=0`, `+0x3c4/+0x3c8/+0x3cc=0`, `+0x3d0/+0x3d1=0`, active `+0x85=1`. It installs three Projectile vtable address points at object `+0/+4/+0x24`; Laser C1 calls Projectile C2, initializes `+0x3d4/+0x3d8/+0x3dc=0`, `+0x3e0=-1`, and installs its three address points. | No semantic 64-bit state owner/default-init adapter or real virtual provider. Vtable symbols are `_ZTV10Projectile` `0x966f90` and `_ZTV19LaserTypeProjectile` `0x966e90` (each 256 bytes); binary pointers are not callable in the port. |
| SetInfo/motion | Bool `SetInfo` `0x3e58cc` stores projectile id `+0x374`, owner `+0x380`, target restriction `+0x384`, callbacks `+0x3b8/+0x3bc`, user data `+0x3c0`, and gate bytes `+0x3d0/+0x3d1`; resolves owner/target/node positions and visual node, then sets start/velocity/destination state. It maps row Model through ProjectileDict and calls `GameObject::SetVisualObject(path,nullptr,0)`; `VisualObject` C1 `0x472a0c` calls `AssetManager::loadSceneNode(path,empty,0,255)`, refreshes root bounds, finds `_colbox_`, then runs `CalcMeshBox`/`ApplyMeshBox`. Float overload `0x3e57cc` dispatches the vtable `+0xc8` virtual to bool SetInfo (Laser override `0x3e4570`), then derives look-at/target positions, calls `SetDestination`, and optionally syncs visual rotation. | Animation event supplies only template id and actor state. No source-equivalent owner/target resolver, SceneManager/AssetManager loadSceneNode provider, visual-node owner, manager, or target/destination virtual providers are bound. The portable path and BDAE bytes alone do not produce source scene-node state. |
| Physical/collision | Bool `SetInfo` allocates 40 bytes for `PhysicalObject` and calls ctor `0x46f2f0` as `(world, GameObject, 0,1,1,1,0,32,0x51f,0)`, installs `POProjectile` vptr, then `SetPhysicalObject(...,0)` `0x394bf8`. With the second bool set, the ctor builds a circle of radius `max(AABB width,height)*0.005`, using GameObject floats `+88/+89` scaled by `0.01` as shape offset; the definition's density is `0.1`. `POProjectile::onCollisionTest` `0x470520` filters source IDs 9/10; `onCollisionBegins` `0x4704b4` dispatches source ID 9 to `OnCollision` `0x3e55b4`. | App creates a BDAE-bounded circle in the shared PhysicalWorld and maps character contacts to canonical ObjectManager targets and the recovered relation/callback path. This is a semantic adapter, not the original PhysicalObject/POProjectile instance; source rotation and exact visual-node offset remain partial. |
| Expire/effects | `OnExpire` `0x3e5078` optionally corrects floor height from row+52 through PFWorld/PFRoom/PFFloor, clears collision target `+0x3cc`, sets `+0x3d1`, gets GameObject target position and calls `HandleImpactFX`. Type 1 selects row `expire_fx`/`expire_sfx`; type 4 selects character `impact_fx`/`impact_sfx`. | App uses the selected Level's PFWorld-backed floor query with `include_special=0`; a miss uses source's initialized zero output height. It clears target before dispatching the authored effect through the existing AnimatedFX owner. Expiry sound and cached PFRoom/PFFloor identity remain open. |
| Update/despawn | `Update` `0x3e51b0` first consumes a queued collision: `_ProjectileHit` return 1 clears `+0x3d0/+0x3d1` and resumes; return 0 dispatches character impact FX and DeSpawns. Normal path calls GameObject update, adjusts vertical velocity/timer, checks squared distance and PFWorld floor/path/room, and expires terminal shots. `DeSpawn` `0x3e61b4` disables updating, stops, clears active and releases the pool slot while retaining ObjectManager identity. | App uses the existing ordered ObjectManager callback and shared PhysicalWorld; pending target/Projectile identities, retained script args, same-VM delayed check/hit, impact FX, pool release and body destruction share those owners. Timer/vertical-speed gates, same-Level floor query, `CanPathOn`, wall exception, room bit-0 test and target-position resolution are connected. Source visual/rotation and floor-cache identity remain partial. |

Spawn/Check/Hit caller xrefs are the two `CharAISkillScript::SpawnProjectile` overloads, which store the skill script as callback userdata. Mage Chain Lightning passes `(row, startingTarget, ourTarget)`; the native injects a skill index, so the wrapper sees four Lua values and returns no Projectile pointer (the native Pointer return is used only for at most two values). Its following `SetProjectileTarget(projectile, ourTarget)` therefore receives nil and is a source no-op. The source check then takes its normal 0 result; `OnCollision` queues the hit for the next `Projectile::Update`, which invokes `_ProjectileHit`/`OnDelayedSkill` and reaches the existing SkillCombatRoll/ApplyResult route. These wrappers do not produce OnCollision result 2. Ranged Character animation events use distinct `_HandleProjectile` callback and remain outside this script-delayed path.

Model bounds are source-derived. Projectile row `+40` is a signed
ProjectileDict index; SetInfo resolves it as
`ProjectileDict::members[index * 12 + 8]` and passes that model path to
`GameObject::SetVisualObject`. A negative index clears the visual. The loaded
VisualObject invokes `CalcMeshBox` `0x47211c` then `ApplyMeshBox` `0x470a54`,
which applies its loaded scene-node mesh bounds to GameObject's relative AABB.
For projectile SetInfo's `PhysicalObject` C2 call the exact arguments are
`(world, gameObject, 0,1,1,1,0,32,0x51f,0)`. The `a5=1` constructor path
derives its collision dimensions from GameObject AABB coordinates (scaled by
0.01), including a radius equal to half the larger horizontal extent; it also
reads GameObject floats `+88/+89` for shape placement. Thus the missing values
come from the ProjectileTable row -> ProjectileDict model path -> loaded BDAE
scene-node bounds -> GameObject AABB, not the launch event. The complete local
cache supplied all ProjectileTable/ProjectileDict files and their referenced
models. The Android asset tree now includes the exact six projectile table,
schema, dictionary and constant files, all 26 model BDAEs actually referenced
by the 41 rows, and their 21 texture dependencies. Source/destination hashes
and row->dictionary->model mappings are in
`assets/source-data/projectiles/projectiles-asset-provenance.json`. Dictionary
models 2 and 20 are intentionally not copied because no table row references
them; `AAA_FAKE_DONT_DELETE` has model index -1 and no visual.

IDA's compact `Spawn` trace confirms the product call chain: `Spawn(id, owner,
target, check, hit, userData, float)` bounds-checks id, selects laser via
ProjectileTable row byte `+28`, calls `_Create(1, laserFlag)`, then virtual
`SetUpdating(true)`, the matching float `SetInfo`, and activates byte `+0x85`.
`SetInfo` stores the launch providers and then resolves model/shape as above.
`game-data/projectile_table_v1` now decodes the verified 59-byte rows and
ProjectileDict into portable typed records. `Owner::reserve_from_source`
derives row count and laser class from that table, then keeps the selected
row/model path with the canonical ObjectManager name reservation. This removes
caller-guessable row/type selection while preserving the single ObjectManager
handle and manager-owned serial. It does not construct a native Projectile or
execute source SetInfo: the constructor, vtable and product lifetime remain
open.

`Owner::spawn` now lends that reservation-owned row to the synchronous SetInfo
hook and validates row identity/type before registration; the callback can read
the exact model path and source tuning data without another table cache. The
portable boundary still cannot perform `Projectile::SetVisualObject`: the
current ObjectManager GameObject has no native VisualObject/BDAE scene bounds
provider or renderer-owned projectile actor. Constructor/vtable, collision
physical-body construction, and destructor cleanup remain explicitly unbound.
IDA confirms the Projectile complete/deleting destructors at `0x3e5d9c` and
`0x3e5de4`: the complete destructor restores its three base address points and
calls `GameObject::~GameObject`; the deleting destructor then calls `CustomFree`.
`GameObject::~GameObject` `0x38d378` releases owned VisualObject (`+0x2d8`),
PhysicalObject (`+0x2dc`) and its other ref-counted components before base
destruction. Manager DeSpawn does not destroy; cleanup belongs to canonical
ObjectManager retirement, where the selected Projectile deleting destructor
must run once. `PhysicalObject::~` `0x46eebc` destroys its world body when one
exists. The port has no semantic destructor/retirement owner or source Visual
Object/PhysicalObject refcount object, so a projectile-only cleanup helper
would double-own these resources.

Exact launch edge: `Character::_SpawnSkillProjectile` `0x3b99d0` resolves its
script argument to a live `CharAISkillScript*`, validates the row index, and
calls its float or bool `SpawnProjectile` overload (`0x3da494`/`0x3da510`).
Those overloads pass owner `script+4`, null target, `_ProjectileCheck`,
`_ProjectileHit`, `script` userdata and the overload value to singleton
`ProjectileManager::Spawn`; its `_Create` supplies the source pool serial and
chooses the `ProjectileTable` row's concrete class. Optional userdata arguments
then set start position and destination on the returned object.

`Character::_SpawnSkillProjectile` and `_SetProjectileTarget` are now wired on
the selected Android Level path, including SWAMP as well as Crypt. Player `native_player_skills.cpp`
forwards both function IDs into `player_skill_gameplay_native`; its adapter
uses the retained Player skill-script vector and canonical Character identity.
`NativeMonsterInitialization::script_services()` registers the same two
callbacks on each Monster's existing AIS VM, resolving `skill_index` through
that same retained Monster CharAI skill vector. Neither path creates a VM.
The shared caller kernel preserves the second numeric argument as the
ProjectileTable row ID, optional bool/angle SetInfo variant, source call order
(Spawn, then optional position and destination), and <=2 argument Pointer
return behavior. `_SetProjectileTarget` preserves its non-Pointer/missing
argument no-op and forwards only a source userdata target.

The app projection loads the verified ProjectileTable from the active Level
assets. `projectile_product_owner_v1::Owner` borrows the Level's existing
ObjectManager, routes acquisitions through the existing manager/factory and
lifecycle kernels, retains a per-projectile payload with source row, manager,
owner, target, callback and skill-script userdata identities, and enrolls
updating through the existing ObjectManager staging path. It does not allocate
a native ARM-layout `Projectile`/`LaserTypeProjectile` object or install source
vtables. Optional point userdata resolves positions only when it names a
Character in that same selected Level's canonical ObjectManager and retained
Character owner. Other userdata kinds still fail closed because there is no
generic source userdata position reader. The Player/Monster callback bindings are
host-audited at the Session/kernel boundaries; Android runtime invocation has
not been exercised.

The product-owner host fixture drives accepted-contact filtering and the
source-real `_ProjectileCheck` result 0: it verifies that the collision target
and point are stored and the pending `+0x3d1` gate is set. It then exercises the
retained delayed-hit callback boundary used by the next Projectile::Update,
through the SkillCombatRoll kernel and an ApplyResult provider stand-in. It
does not verify Android physics contact, the app VM adapters, or the full
ordered renderer update.

Live IDA analysis of `_ProjectileCheck` (`0x3daff4`) and `_ProjectileHit`
(`0x3dae7c`) establishes the actual callback ABI: `R0` is the Projectile and
`R1` is its retained CharAISkillScript. `SetSkill` uses the script's embedded
`Arguments` at `script+0x0c`; after that call succeeds, the delayed alias gets
the target userdata stored at Projectile `+0x3cc` and a Pointer to the
Projectile itself. An absent Projectile LuaScript returns 1. Otherwise only
an explicit boolean false from `OnDelayedSkillCheck` maps to native return 1;
errors, no return, nonboolean, and true map to 0. SetSkill ReturnValues are
discarded and do not gate the delayed call. `Projectile::OnCollision`
(`0x3e55b4`) interprets check result 1 as non-consuming contact, 0 as
expiration, and 2 as the path that invokes `_ProjectileHit` plus impact FX.
The concrete `_ProjectileCheck` function returns only 0 or 1, so it does not
itself select the result-2 path. On result 2, `Projectile::OnCollision`
ignores `_ProjectileHit`'s integer return and continues to impact-FX handling.

The current native renderer connects Player and Monster skill projectile
registrations to one Level-owned product owner and the same ObjectManager. It
loads each selected ProjectileDict BDAE through the active Level asset owner,
builds a bounds-derived circle on the existing actor PhysicalWorld, advances
that body in the ordered ObjectManager phase, and synchronizes its position
after the existing shared physics step. Add-contact dispatch runs the shared
source collision filters and invokes `OnDelayedSkillCheck` through the
retained same-VM owner with distinct target and Projectile identities. The
source-0 collision result queues contact for the next ordered update; that
pending route invokes `_ProjectileHit`/`OnDelayedSkill`, and a delayed
SkillCombatRoll reaches the canonical ApplyResult owner. The shared lifecycle
also performs source floor/path/room and expiry checks, target position
resolution, impact FX through the existing AnimatedFX owner, pool release and
body destruction. The source check's result-2 branch is not selected by these
concrete wrappers. The product fixture exercises pending delivery through the
kernel and an ApplyResult provider stand-in; it does not emulate Android
PhysicalWorld or Lua runtime.

Remaining gaps include source VisualObject/rendered projectile models,
rotation/node offsets, cached PFRoom/PFFloor identity, expiry sound and the
separate ranged Character animation-event `_HandleProjectile` caller. The
Windows renderer/NDK build was not rerun after this slice; the desktop build
closure owns full integration compilation. Do not treat the bounded host
fixture as Android runtime validation.
