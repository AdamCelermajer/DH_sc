# Original frame ordering and clock handoff

This is a read-only source/instruction trace of ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. Captured routine manifests, assembly, annotated calls, clock cross-references and vtable evidence are in `.local-inputs/frame-order-discovery`. The trace establishes ordered call sites and conditional boundaries; it is not an end-to-end frame execution oracle or a reconstruction of the complete application/state/input lifecycle.

## Normal loaded-level frame

Application::Update `0x32ccc4` calls ComputeDt at`0x32cda0`, Application::_Update(GetDt) at`0x32cde4`, then _Draw at`0x32cdec`. The actual _Update argument is App+0x8c. TouchScreen::ProcessEvents occurs earlier at`0x32cd98`.

Within Application::_Update `0x32c438`, the guarded scene update runs at`0x32c918`. It then branches back through`0x32c928 -> 0x32c518`, resets the CharAI step allowance, updates PlayerManager at`0x32c544`, calls EventManager::Update at`0x32c564`, and calls StateMachine::Update at`0x32c574`. Therefore scene animation is before game-state/Level simulation in this path. Later touch/accelerometer/InputManager updates are at`0x32c5a4`, `0x32c5bc`, `0x32c5e0`; this trace does not flatten those separate input stages into one assumed input sample.

The scene call is virtual slot0x60 on the scene manager. Both CSceneManager and game SceneManager constructors establish primary vtable address points at their vtable symbol+0x1c, not+8. SceneManager ctor`0x352cc4` writes that address point; actual slot0x60 resolves to CSceneManager::update `0x58b9f0`. Its normal input is float-converted _Update milliseconds and its bool argument is0. It accumulates the absolute scene clock, then calls the scene root's onAnimate slot0x14 at`0x58baa4`. The scene root can be the generic manager root; its child traversal reaches game RootSceneNode instances. This is not an assumption that the manager root itself is a game character RootSceneNode.

StateMachine::Update `0x33a7f4` processes its pending state work, then invokes the current top state's slot0x18 at`0x33a840`. The original GSLevel vtable slot0x18 resolves to GSLevel::Update `0x386630`. Its loaded/gameplay branch calls Level::Update(false) at`0x38670c`; loading branches call it at`0x386798` or Level::Load separately. Level::Update first routes incomplete load stage+0x130 through _LoadProcess rather than gameplay. The normal stage value is0x26.

The verified gameplay ordering in Level::Update `0x3f82d8` is:

| Call site | Actual operation |
|---|---|
| `0x3f8478` | GameEventManager::Update |
| `0x3f84c8` | PhysicalWorld::update -> actual Box2D Step |
| `0x3f84d8` | SpawnGroupManager::update |
| `0x3f84dc` | CharAI::HandleGroups |
| `0x3f84e8` | ObjectManager::Update(float1) |
| `0x3f84ec` | CharAI::IncUpdateQueue |
| `0x3f84f4` | Level::UpdateCameraZoom |
| `0x3f8500` | VisualFXManager::Update |

Scripts/player-manager services and ambient-light setup occur before the Step. In particular ObjectManager's argument is literal float1; its argument is not the physics dt producer. PhysicalWorld gets the application dt independently.

ObjectManager::Update `0x34a620` updates room/network/start-update work before traversing its active objects. It invokes eligible ObjectBase slot0x2c at`0x34a84c`. The Character vtable slot0x2c resolves to Character::Update `0x3abe98`. Enabled/active/deletion bytes, online/remote policy, and current-level gates can skip an object. The trace establishes the ordering of calls that execute; it does not assert that every object updates every frame.

Character first calls virtual CanUpdate at`0x3abf60`; false skips the main update. After its preceding state/online/interaction branches, the normal update sequence is optional TimerUtil::Update at`0x3ac024`, CharTimers::Update at`0x3ac02c`, CharAI::Update at`0x3ac034`, CharStateMachine::Update at`0x3ac03c`, CharAnimator::Update at`0x3ac048`, then GameObject::Update at`0x3ac054`. The CharAnimator call handles scheduling/completion state; scene/timeline pose animation has already run earlier in the frame.

GameObject::Update `0x38cbe8` stores prior position/rotation and then executes:

| Call site | Actual operation |
|---|---|
| `0x38ccc4` | UpdatePath |
| `0x38cccc` | UpdateRotation |
| `0x38ccd4` | UpdateSubObjects |
| `0x38ccdc` | UpdateTargetPosition |
| `0x38cce4` | RequireOnlineUpdate |

An optional pending virtual action at`0x38cc84` precedes these calls; idle sound can follow them. UpdatePath can therefore execute complete Stop before the same actor's UpdateSubObjects. Native body velocity/transforms issued by actor updates occur after this frame's physics Step and affect the subsequent physics integration, while the already sampled visual/root displacement can be synchronized by the same frame's UpdateSubObjects.

## Root animation and timeline

Generic ISceneNode::onAnimate `0x596d6c` checks node flags, animates each attached animator via slot0x10 (`0x596db8`), updates its transform via slot0xb8 (`0x596ddc`), then recursively invokes each child's onAnimate slot0x14 (`0x596e04`). Flags0x400/1 and0x200 can skip this work.

The game RootSceneNode::onAnimate `0x35d168` performs visibility/frustum policy. Its normal branch animates attached animators at`0x35d3a8`, handles displacement at`0x35d434`, then animates children at`0x35d404`. Its culled branch advances root time through virtual slot0x18 at`0x35d2b8`, applies applicator animation at`0x35d474`, and handles enabled displacement at`0x35d48c`. Other visibility and transform calls are interleaved by those branches; do not replace them with unconditional normal traversal.

The collada CRootSceneNode::onAnimate `0x65af10` calls generic node animation when mode+0x1ac==1; otherwise it updates the absolute transform. Both branches write the supplied timestamp to root+0x1b0. The game RootSceneNode overrides that dispatch and has the explicit displacement behavior above. The already reconstructed RootNewAnim can also synchronously call onAnimate during a clip restart; consequently there can be actor-phase animation work in addition to the ordinary early scene phase.

CTimelineController::update `0x667104` receives signed absolute milliseconds, converts to float seconds by division by1000. First update after initialization establishes the last time and does not advance. Later updates apply `(now_seconds-last_seconds)*scale`; completion callback occurs after clamp/wrap but before current integer milliseconds are recomputed. These details and same-clip restart/overshoot are verified separately by the root/timeline worker; this trace does not impose a new relative-dt animation clock.

## Dt and absolute clocks

Application::ComputeDt `0x320da4` calls real time `0x60b0cc`, computes wrapping32-bit `now-App+0x88`, stores now to+0x88, and calculates:

1. Convert the unsigned difference to float; multiply by App+0x9c; truncate using signed float-to-int; store App+0x8c.
2. Convert that stored word as unsigned to float; multiply by App+0x98; add fractional remainder App+0x94; truncate using signed float-to-int; store App+0x90.
3. Store `untruncated_scaled_value-float(unsigned_word_at_0x90)` to App+0x94.

Imports are confirmed: PLT`0x30e2e0`=__aeabi_ui2f, `0x30e964`=__aeabi_i2f, `0x30e4cc`=__aeabi_f2iz. GetDt `0x31f66c` returns+0x8c; GetDtScaled `0x31f674` returns+0x90; SetTimeScale `0x31f67c` writes+0x98. No clamp appears in ComputeDt. Invalid/overflowing float-to-int domains are not given invented portable semantics here. A direct ARM BL scan found46 GetDt call sites including physical Step, GameObject rotation, Character FSM/timers and controller/camera services, and no direct GetDtScaled calls; inlined field access and indirect calls are outside that scan.

PhysicalWorld::update `0x34bd08` calls GetDt at`0x34bd30`, unsigned-word-to-float conversion at`0x34bd34`, multiplies by float bits0x3a83126f (.001) at`0x34bd40`, then calls b2World::Step at`0x34bd54` with iteration count10. It performs one Step per invocation. No accumulator, fixed-step subdivision or additional dt clamp appears in that wrapper.

CSceneManager::update accumulates float milliseconds at+0x254 and converts the accumulated value to unsigned integer time for onAnimate. Its special sentinel bits0xc7f12000 instead obtains glitch Timer::getTime `0x60aee4`, replacing the accumulated clock. Normal Application dispatch supplies its frame dt, not that sentinel. Timer::getRealTime `0x60b0cc` uses imported gettimeofday-style timeval data and computes seconds*1000+microseconds/1000 in32-bit arithmetic. The glitch virtual timer separately has stop/start/speed state; it is not interchangeable with the normal accumulated scene time.

## Pause/load and remaining boundaries

Application::_Update checks IsLevelPaused(true) before deciding to dispatch scene animation. It also checks loaded level, level transition, level+0x144 and+0x1a8, and debug/online global branches. A paused single-player path skips the scene call, but proceeds to StateMachine::Update; the current top state determines whether GSLevel/Level gameplay executes. Pause is not implemented by writing GetDt=0 in this trace. Application::Update still recomputes dt on its ordinary frames. Level gameplay can also return early for its debug freeze switches; incomplete loading follows _LoadProcess.

Application::Update can skip the complete frame for app+0xa4, and has a separate device-time gap greater than2000 branch (`0x32cd54`) that calls ComputeDt then returns without _Update/_Draw. These gates must not be silently converted into the renderer's existing arbitrary dt clamp.

This establishes the integration order **scene/timeline/root displacement -> current game state -> physics Step -> eligible actor timers/AI/FSM/animation scheduler -> path -> rotation -> subobjects**, with the explicit player/event/spawn/group services shown above. It does not establish every pause/debug/online setting's value, every object/node's eligibility, all virtual animator implementations, or gameplay collision callback policy. Real Box2D contact callbacks happen within Step; complete gameplay listener consequences remain owned by the physical-world integration. No production renderer, CMake or tests were changed for this trace.

## Rotation handoff

UpdateRotation `0x393710`, called between path and subobjects, computes current GameObject Z+0x174 from heading target+0x178 and source rotation speed using unsigned GetDt milliseconds. It then requests VisualObject::SyncRotation only when the visual exists and the actual Character visual-with-rotation policy is true. Negative speed (Character flag0x20 produces-1) snaps immediately without reading dt; ordinary speed uses float4pi*speed and float.001*dt, a single heading-delta correction by2pi, and a strict snap boundary. The pure coordinator, field mapping and5,590 original-instruction/host replay cases are documented in `../actor-rotation/NOTES.md`. Move policy0x23c1 leaves visual/physics rotation-source bits clear while enabling visual-with-rotation, so it preserves this actor-produced angle during subobject synchronization.

## Prior state and target-node cache

Before UpdatePath, GameObject::Update snapshots the current position XYZ+0x160/+0x164/+0x168 into previous position+0x190/+0x194/+0x198 and current Euler XYZ+0x16c/+0x170/+0x174 into previous Euler+0x19c/+0x1a0/+0x1a4. The actual reads/writes are`0x38cc90..0x38ccbc`. These precede path, rotation and subobjects and follow the optional pending virtual action. The snapshots therefore retain the state at this precise point, rather than the final synchronized state after the frame.

After UpdateSubObjects, GameObject::UpdateTargetPosition `0x393d74` checks the scene-node pointer at GameObject+0x180 (`0x393d78`). If null it leaves target-position cache+0x184/+0x188/+0x18c unchanged. Otherwise it calls ISceneNode::getAbsolutePosition `0x597180` at`0x393d90`, then copies the returned XYZ into that cache at`0x393da0/0x393da4/0x393da8`. The getter directly reads node+0x54/+0x58/+0x5c into its returned vector; it does not recalculate or update the scene transform. This routine reads the target node's current cached absolute position. It does not copy actor position or destination into path-controller fields, and does not write visual target state. Its call follows subobject synchronization at`0x38ccdc`; RequireOnlineUpdate follows at`0x38cce4`.

The two-routine manifest and full assembly are preserved under `.local-inputs/frame-order-discovery/target-position`. Target-node binding producers and scene absolute-transform freshness are separate boundaries; this read-only trace does not infer a new binding or recalculate a target node after the early scene phase.
