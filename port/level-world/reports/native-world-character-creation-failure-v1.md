# NativeWorld character creation failure safety v1

## Evidence and expected behavior

This is internal infrastructure, so it has no direct original visual counterpart. Its original consumer is character body allocation: `OriginalActorPhysical::initialize` calls `NativeWorld::create_character` before assigning the returned pointer into its owned `NativeBody`. If a required collision-filter provider fails in that call, the caller has no body pointer to release. The visible/gameplay invariant is that startup either produces a complete physical actor or fails without leaving an orphan body or dangling `WorldObject` identity that can be reached by later world callbacks.

The local Box2D 2.0.1 source establishes the callback order: `b2Body::CreateShape` creates and links the shape, then `b2Shape::CreateProxy` inserts the proxy. Broadphase pair commit reaches `b2ContactManager::PairAdded`, which calls the world's `ShouldCollide` synchronously. This occurs before `CreateShape` returns, while `OriginalActorPhysical::initialize` still has not received a body pointer. `NativeWorld::ShouldCollide` calls the required provider through `dh2_physical_world_should_collide`; a thrown provider error must not unwind through Box2D's internal mutation.

Before this fix, `NativeWorld::create_character` let a non-Step provider exception escape from `ShouldCollide`. The partially-created body remained linked into the world, with `b2BodyDef::userData` still pointing at the owner's `WorldObject`. The caller's failed initialization had no body handle to clean up. Existing Step behavior already uses cooperative failure delivery: latch the first callback error during `Step`, finish the locked operation, then report it. The implementation applies that principle narrowly to character creation: latch only while `create_character` is building a body, finish Box2D's synchronous operation, destroy that operation's partial body while the owner identity remains alive, then report the original provider failure. A normal provider result of false remains a normal denied collision and does not fail creation.

## Implementation

`physical_world.cpp` now uses a `.cpp`-local, thread-local, nestable creation scope keyed by `NativeWorld*`; no public header or object layout changes. `ShouldCollide` and contact delivery catch provider exceptions only while Step or a matching character-creation scope is active. Creation-time failures are latched separately from `step_error_`. `create_character` finishes shape/mass/pin work, destroys only its partial body, exits the callback scope, and rethrows the first original error. Other callback exceptions outside these cooperative scopes retain their existing throw behavior. If partial-body destruction itself fails, the surfaced error reports both the original failure and the cleanup failure.

## Focused verification

The private runner compiles the current `physical_world.cpp` and a fresh focused test against a hash-checked private copy of the immutable Box2D archive. The test creates a live overlapping actor, forces a second actor's filter provider to throw from proxy pairing, and checks the original error, body count, zero failed-owner bodies, idle callback depths, and preservation of the first actor. It also checks that a provider returning false permits body creation and denies collision, then repairs the provider and retries successfully. Finally it injects a contact-provider exception during `Step`, checks post-unlock reporting and no same-call replay, repairs that provider, and verifies the next Step succeeds.

Run with:

```powershell
powershell -ExecutionPolicy Bypass -File port/level-world/tools/run_native_world_character_creation_failure_v1.ps1
```

## Results and limits

The focused private runner passed with output:

```text
{"creation_callback_failure_cleaned":true,"denied_collision_remained_success":true,"provider_retry_succeeded":true,"step_failure_recovery":true,"orphan_bodies":0}
```

It observed the exact original error prefix `intentional construction filter provider failure`. After failure, the backend retained exactly the ground body plus the original live actor; iteration found zero bodies owned by the failed actor, one body for the live actor, and `cleanup_delivery_idle_v106()` was true. A provider returning false still created its body and rejected collision. Repairing the provider allowed an overlapping retry while preserving both owner identities. The Step test observed one failed callback invocation in the failed update, confirmed idle delivery state, and succeeded on the next update after repairing the callback.

The existing current-source actor-physics runner also passed from a fresh private build directory: `PASS same-session Mage/Lizard physics frame, real contacts, once-per-frame Step, source sleeping/pin distinction, same-actor position publication, physical Stop and failure-prefix guard`. The new standalone executable SHA-256 is `5345A161F83AAA457E20B6C022A0E2E26EC62068B3663F2DC5467CE207AD7151`; the private Box2D archive copy is `1DEDF142B571CF67E03BC3D0E8EEBD9614A915D3E92A2BAB104B97392BAB374B`.

This verifies the implementation in an isolated native fixture and current-source actor-physics regression, not full production GUI startup or integrated-session behavior. The production pool's register-before-initialize startup fix is owned separately by root and remains an integration check.
