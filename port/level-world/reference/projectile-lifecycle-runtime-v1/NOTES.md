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
- `Projectile::Update` `0x3e51b0`: pending impact/target gate, destination,
  GameObject update, motion/time/range, floor/path and room tests; expiry calls
  OnExpire. Physics and complete update remain unimplemented here.
- `Projectile::OnCollision` `0x3e55b4`: active/exit and target filters, stores
  target/point, optional callback, then handles callback result.
- `Projectile::OnExpire` `0x3e5078`: optional row+52 floor correction, clear
  +0x3cc, set +0x3d1, GetTargetPosition, HandleImpactFX. It does not despawn.
- `ProjectileManager::DeSpawn` `0x3e61b4`: SetUpdating(false), Stop, clear
  active byte, then release the source pool slot; the ObjectManager object stays.

The selected port implements fresh/reused Spawn choreography, a typed pair of
ProjectileManager pools over the shared ObjectManager owner, the post-GameObject
Update lifetime/speed/range gates, OnExpire ordering, DeSpawn, and OnCollision
filter/store/callback dispatch. Collision detection and source relation/callback
bodies remain supplied by the caller; row+4's owner-target predicate is injected.
Native product construction/vtables, PreCache shells, physics, floor/room Update
tests, and production animation-event binding remain open.

`projectile_factory_owner_v1` now validates the source type map, reserves the
exact generated name/handle in the borrowed canonical ObjectManager, then
accepts a single factory-produced object into the lifecycle kernel. Serial and
ProjectileManager identity remain caller inputs. The unbound production
callback is the source `ObjectManager::GetNewObject` factory provider for
`Projectile`/`LaserTypeProjectile`: it must construct the real class, initialize
its inherited/source fields and vtable, and return its lifetime-owned product.
This typed bridge has host coverage only; Android animation dispatch is still
not connected and no live projectile behavior is claimed.

| Needed provider | Retained source contract | Port gap |
|---|---|---|
| Factory product | `GetNewObject` `0x34b520` -> Add `0x34b270`; `GetNewInstance<Projectile>` `0x340d14` allocates 980 bytes with GO_ID 9 and calls C1 `0x3e5e40`; Laser thunk `0x340cf0` allocates 996 bytes with GO_ID 10 and calls C1 `0x3e4b6c`. | Map is metadata only; no constructor/provider execution or object lifetime owner. ARM32 sizes/GO_IDs cannot be used as a 64-bit C++ allocation/layout. |
| State/vtable | Projectile C1 calls GameObject C2 and initializes source fields: `+0x374=-1`, `+0x378/+0x380/+0x384=0`, `+0x388..+0x39c=0`, `+0x3a4=0`, `+0x3b8=0`, `+0x3c4/+0x3c8/+0x3cc=0`, `+0x3d0/+0x3d1=0`, active `+0x85=1`. It installs three Projectile vtable address points at object `+0/+4/+0x24`; Laser C1 calls Projectile C2, initializes `+0x3d4/+0x3d8/+0x3dc=0`, `+0x3e0=-1`, and installs its three address points. | No semantic 64-bit state owner/default-init adapter or real virtual provider. Vtable symbols are `_ZTV10Projectile` `0x966f90` and `_ZTV19LaserTypeProjectile` `0x966e90` (each 256 bytes); binary pointers are not callable in the port. |
| SetInfo/motion | Bool `SetInfo` `0x3e58cc` stores projectile id `+0x374`, owner `+0x380`, target restriction `+0x384`, callbacks `+0x3b8/+0x3bc`, user data `+0x3c0`, and gate bytes `+0x3d0/+0x3d1`; resolves owner/target/node positions and visual node, then sets start/velocity/destination state. It maps row Model through ProjectileDict and calls `GameObject::SetVisualObject(path,nullptr,0)`; `VisualObject` C1 `0x472a0c` calls `AssetManager::loadSceneNode(path,empty,0,255)`, refreshes root bounds, finds `_colbox_`, then runs `CalcMeshBox`/`ApplyMeshBox`. Float overload `0x3e57cc` dispatches the vtable `+0xc8` virtual to bool SetInfo (Laser override `0x3e4570`), then derives look-at/target positions, calls `SetDestination`, and optionally syncs visual rotation. | Animation event supplies only template id and actor state. No source-equivalent owner/target resolver, SceneManager/AssetManager loadSceneNode provider, visual-node owner, manager, or target/destination virtual providers are bound. The portable path and BDAE bytes alone do not produce source scene-node state. |
| Physical/collision | Bool `SetInfo` allocates 40 bytes for `PhysicalObject` and calls ctor `0x46f2f0` as `(world, GameObject, 0,1,1,1,0,32,0x51f,0)`, installs `POProjectile` vptr, then `SetPhysicalObject(...,0)` `0x394bf8`. With the second bool set, the ctor builds a circle of radius `max(AABB width,height)*0.005`, using GameObject floats `+88/+89` scaled by `0.01` as shape offset; the definition's density is `0.1`. `_init` `0x46edf0` calls `PhysicalWorld::createBody`, stores the PhysicalObject as body user data, then creates/adds the shape. SetPhysicalObject stores the physical owner and calls UpdatePFObject; it only pins on a true third argument, which Projectile SetInfo does not pass. `POProjectile::onCollisionTest` `0x470520` runs base filtering, then accepts source IDs 9/10; `onCollisionBegins` `0x4704b4` accepts ID 9 and dispatches projectile vtable `+0xd0` -> `OnCollision` `0x3e55b4`. The latter gates `+0x3d0/+0x3d1`, applies owner/target relation filters, stores target/point and dispatches callbacks. | `physical::NativeWorld` has Box2D body/contact services and the port's character component, but neither owns a projectile `PhysicalObject` semantic or POProjectile contact adapter. Exact body radius/offset can be calculated only after source scene bounds and GameObject placement exist. No callback maps `WorldObject` contacts to ObjectManager target handles or the source relation/callbacks. Do not reuse the character physical component as a projectile owner. |
| Expire/effects | `OnExpire` `0x3e5078` optionally corrects floor height from projectile row+52 via PFWorld/PFRoom/PFFloor, clears target `+0x3cc`, sets expiry `+0x3d1`, gets target position and calls `HandleImpactFX`. `HandleImpactFX` `0x3e4eec` dispatches character/non-character FX; visual routines include `0x3e4db0`. | Kernel's injected hooks preserve ordering but are not bound to floor/world, target-position, VisualFX config or renderer owners. `OnCollision` callback return 1 rejects after storing; 2 invokes optional hit callback then impact FX; 3 runs type-1 OnExpire; default follows pending-expiry path. |
| Update/despawn | `Update` `0x3e51b0` calls GameObject update, then source dt, motion/range, PFWorld floor/path and room tests; calls OnExpire on terminal conditions. Manager `DeSpawn` `0x3e61b4` disables updating, stops, clears active and releases its pool slot while ObjectManager retains the object. | Portable pool release is tested, but no native Projectile product, manager callsite, frame-update owner, or PFWorld floor/path/room provider is connected. Keep the lifecycle kernel out of frame/pool ownership. |

Source launch callers are `CharAISkillScript::SpawnProjectile` and
`CharAI::_OnAnimEvent` (`attack_ranged`, `attack_mainhand`, `do_skill`). The
animation path gates on `CanRangeAttack` and calls the float Spawn overload with
the Character GameObject, null target, `_HandleProjectile` in callback slot
`+0x3b8`, null hit callback, null userdata, and angle 0. The skill path passes
its script-owned owner/callback/userdata. The native pool owner accepts these
requests; neither caller is yet bound to it in production.

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

This caller is not yet bindable in the port. Player skill VM registers the
`SpawnSkillProjectile__` key, but `native_player_skills.cpp` has no dispatch for
`character_spawn_skill_projectile`, so it fails closed. The renderer's ranged
animation action only reports the template ID and currently returns before
projectile creation. Neither path owns `CharAISkillScript`/its callback userdata
or the singleton manager's pool counters, and the existing ObjectManager
reservation requires that exact serial. Retained Player preparation exposes
ordered skill-slot identities; each resolves to a portable constructor State
holding identity, Character owner, script name and index. This can validate a
userdata index/owner, but is not the allocated source class/vtable or callable
`_ProjectileCheck`/`_ProjectileHit` object. Generic Box2D `NativeWorld` has no
projectile pool/product owner; actor and item bodies have distinct owners. Do
not synthesize a request from the animation event. Next required provider is
one canonical projectile owner binding the retained skill identity to manager
pool/serial and product lifetime. SetInfo visual/physics and downstream
collision/update/effects remain separate blockers.
