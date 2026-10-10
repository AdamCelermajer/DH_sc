# Mage Staff01 projectile contact source evidence

This note records the evidence used by `RuntimeSessionProjectileContactBodiesV1`.
It does not claim a normal `main.cpp` projectile caller or a complete source hit
result tail.

* `CharAI::_OnAnimEvent` at `0x3d4434` reaches the state-5 ranged event only
  after the actual range query, then spawns the selected projectile row with
  `CharAI::_HandleProjectile` at `0x3cc280` as the hit callback. The actual Mage
  Staff01 equipment selects ProjectileTable row20 / FireWandProjectile and the
  authored projectile BDAE URI `data/3D/projectiles/elemental_bolt_fire.bdae`.
* `Projectile::SetInfo` at `0x3e58cc` loads the model and creates the projectile
  `PhysicalObject` using constructor `0x46f2f0`. The recovered modern
  `CanonicalProjectilePhysicalV112::initialize` reproduces its live shape from
  that same GameObject's absolute AABB: circle radius is
  `0.01 * 0.5 * max(bounds[3]-bounds[0], bounds[4]-bounds[1])`; center is
  `position160.xy * 0.01`. It is a bullet sensor with category `32`, mask
  `0x51f`, and group zero unless source `MP_NoCollisions` selects `-666`.
  The authored ProjectileTable row contains no collision-radius field.
* `Projectile::GetSpeed` at `0x3e4da8` returns row speed unchanged. Row20 speed
  `20` is written directly as Box2D XY velocity; Box2D positions publish in
  game units multiplied by 100. Source `Level::Update` steps
  `PhysicalWorld::update` at `0x34bd08` before ObjectManager projectile update,
  so the prior frame's velocity advances before `Projectile::Update` at
  `0x3e51b0` writes the next velocity and decrements timers.
* The physical filter calls source `POProjectile::onCollisionTest` at
  `0x470520` (also represented by the peer-owner/filter checks in
  `CanonicalProjectilePhysicalV112::test`). The callback receives a resolved
  peer and the projectile's current source position; projectile filtering
  rejects physical response after delivering source collision handling. The
  ordinary `Projectile::OnCollision` body at `0x3e55b4` rejects dead, pending,
  self and null peers, then applies dedicated-target and FriendlyFire / actual
  `AI_IsEnemy` gates before recording the peer and point for its check callback.
* Source `Projectile::Update` at `0x3e51b0` consumes its queued hit callback
  before normal projectile update. `_HandleProjectile` at `0x3cc280` first
  notifies owner `CharAI::OnProjectileHit` through vtable `+0xac`, then for
  Character peers runs `F_MeleeAttack` (`0x3b3368`) followed by
  `F_ApplyResult` (`0x3b10b4`). In `ranged-attack-v41/source.json`, the first
  call passes the captured peer GameObject as the `OnProjectileHit` argument;
  the result calls pass `offhand=false`, the projectile's byte `+0x37c` as
  critical, and `F_ApplyResult`'s final flag as false. The hit callback's low
  two result bits decide projectile continuation. The runtime now requires a
  typed `source_owner_on_projectile_hit` provider and invokes it before the
  `source_hit_semantics`/F_MeleeAttack provider. Missing owner callback fails
  before any Session result/RNG prefix. The resulting impact packet carries the
  exact `OriginalMeleeResolution` borrowed from the just-produced same-Session
  pending resolution plus the false F_ApplyResult flag for a result-tail
  consumer.

The modern body bridge receives the live decoded BDAE vertex bounds and same
Session ActorId/body resolver from its caller. Its native projectile body is
registered in the caller's existing `NativeWorld`; the feature does not create
a second world or synthesize contact from distance. Call order is: register
body after accepted state-5 launch; run the single existing NativeWorld step;
let the native shape filter queue the typed contact; synchronize the runtime
record's XY from the native body's post-step position; then call the existing
projectile runtime update, which drains contact before writing next-frame
velocity. Expiry, explicit unregister, Session lease renewal, or Session death
removes body registration and queued contact before renderer publication.
The focused test verifies a real same-NativeWorld projectile/registered
ActorState overlap, owner callback order/failure behavior, exact source result
payload, source-unit velocity, timer expiry, and stale body/token cleanup on
binding renewal.

Unknown at this feature boundary: complete owner `OnProjectileHit` behavior,
complete `F_ApplyResult` status/aggro/kill/presentation effects, exact runtime
`main.cpp` enrollment, and GPU pixels. These remain explicit caller/provider
responsibilities.
