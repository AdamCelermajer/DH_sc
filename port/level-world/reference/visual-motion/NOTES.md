# Visual root motion and owner position

Captured original desktop-only ARM32 ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The original library is never linked into the native game. The manifest and
assembly capture the source instructions used for this reconstruction.

`VisualObject::Update` (470cf0) is empty. Motion is produced earlier by scene
animation, not by this virtual update and not by a character speed constant.
`ApplyPosition` (47124c) reads the scene root's position, initializes missing
root output to zero, and invokes `GameObject::SetPosition(point,false)`.
Absent owner is a no-op. Absent scene with a present owner sets game XYZ to zero.
The setter shifts an auxiliary object's XYZ by the game position difference,
updates absolute AABB, requests the physical body's XY transform, and invokes
visual SyncPosition. PF cached XYZ (+1e0) is distinct and unchanged.
SyncPosition copies game XYZ (+160) to scene XYZ (+ac), dirties bit8 and calls
the scene's absolute-position backend with recursion false.

RootSceneNode::_HandleDisplacement (35cf48) reads the animated reference's
raw local translation and sums animator applicator deltas in linked-list order.
It scales delta X/Y by the owner root's X/Y scale, explicitly replaces delta Z
with positive zero, rotates through the original conjugate quaternion-vector
operation, and adds to owner XYZ. The ordinary helper node receives the raw
translation with every sign bit flipped, then updates absolute position.
The uncommon no-helper branch clears animated X/Y while retaining its Z, and
subtracts raw XYZ from a distinct secondary root if present. NaNs cause the
returned moved predicate to be true; no finite-value rejection is invented.

AnimApplicator::CalculateDelta (364444) and ResetDelta(point) (3644cc) are
reconstructed. The former returns positive-zero delta on the same timestamp
but still updates previous XYZ; otherwise it subtracts the previous sample.
The timestamp is the actual engine timestamp, distinct from clip time.
GetApplicator's five accepted animator type layouts (11..15) execute in the
oracle. The pure kernel receives the resulting deltas as a caller-owned array.

Original GetAnimRoot(false) tries scene **name** root_camera, then Bip01, Root,
root_character. The scene search is depth-first in child-list order. Track
binding remains by node ID/UID through the existing native animation player.
Shipped Prince chooses root_camera index33/35 nodes; skeleton root_camera
25/28; ghost root_camera41/45; slime root_character1/9. None requires fabricated
root track targets. The skeleton clips have three unbound targets; slime_walk
has one, as reported by the existing MissingTargets::ignore sampler policy.
Those missing targets are a remaining asset/scene binding fidelity gap.

Visual SyncRotation (472948) passes owner Euler XYZ at +16c,+170,+174.
Visual SetRotation (472874) invokes quaternion::set with **(Y,-X,-Z)** radians.
The exported dh2_visual_rotation preserves this axis swap and sign-bit flips,
using the already recovered normalized quaternion-from-Euler math routine.
The third game rotation is not the separate heading field at +178.

## Scene bridge and restart contract

SceneBinding samples the existing native Player, computes the chosen root's
applicator delta, applies the recovered displacement, and rebuilds every graph
world matrix as owner * helper * authored hierarchy. It uses the existing
node matrix and multiply routines. No helper is added to the serialized graph:
Player requires its original node count, names and parent order. Instances
receive corresponding graph world matrices. Skin palette generation must use
these matrices after sample; the renderer must not apply owner placement again.

Caller supplies clip milliseconds, actual timestamp, and an explicit restart
fact. With restart true, the bridge samples clip.start, resets the applicator at
timestamp+1, then samples the requested current clip time at timestamp. This
reproduces the ordinary **nonzero last timestamp** RootNewAnim sequence; a
newly constructed original root with last timestamp zero skips that sequence.
The adapter's explicit restart offers a well-defined initial baseline too.

For characters, _SetAnimStep (3ca984..3ca9a8) passes PlayClip loop=false.
Animation step byte +1c sets controller displacement byte +10, not loop mode.
Timeline therefore clamps at clip end; callback sets CharAnimator pending +49;
Update chooses/replays the next step. Same-clip BlendedPlayClip checks getLoop;
when the current loop policy is false it
jumps to clip.start+applicator extraTime(+10), and calls RootNewAnim. Its reset
to clip.start followed by current overshoot sampling retains overshoot motion.
The bridge does not infer these FSM/extraTime decisions or modulo-wrap clips.
Direct modulo wrapping without the original restart fact produces a negative
stride delta; automatic reset at the overshoot point would discard valid motion.
Animation step +30 is an authored speed factor, copied to CharAnimator+34.
Original controller speed is CharAnimator+40 multiplied by that +34 factor;
the existing player accepts clip milliseconds after this timeline policy.

Full animator blending weights, scene culling, timeline fractional scaling,
callback replay scheduling and complete scene onAnimate ownership are external.
The native bridge supports a sampled single clip. It must not be described as
the full original character animator or blend scheduler.

## Evidence

ARM64 differential executes original CalculateDelta, ResetDelta, ordered
_CalcDelta/GetApplicator, quaternion-vector, _HandleDisplacement, scene setters,
Get/Set/Apply/SyncPosition, owner setter/AABB and VisualSetRotation instructions.
SetXForm and absolute-position services are synchronous observed fixtures.
1,796 base records compare finite words exactly and arithmetic NaNs by class;
20 ordered callbacks also compare payloads. Stable gold:
`kernel-fixtures.bin`, SHA256
`523f24079f703a5df42631010de7d6cc9d7d8094f6efc2daaebb53a4702cb03f`.
Host ASan/UBSan replays the same corpus with nine atomic caller rejection checks.

Real assets: tests/visual_motion_assets.cpp samples all 39 packaged actor clips
every16ms, exact endpoint, duplicate timestamp and explicit restart. 3,567 rows
feed original and ARM64 applicator/displacement kernels independently. The raw
clip sampler itself is the existing native animation reconstruction, not an
independent original-engine scene/clip loader. Reports clearly keep these asset
extras outside the 1,796 base corpus. Host bridge has zero sanitizer errors.
Unit scale/identity quaternion walk displacement over one authored clip:
Prince approximately (-0.000037998,-603.274,0)/800ms;
skeleton (0,-208.875,0)/2033ms; ghost (0,-535.093,0)/1333ms;
slime (0,-291.996,0)/1066ms. These describe serialized motion, not a speed policy.

Build standalone ARM64 kernels with tools/build_visual_motion_oracle.ps1.
Run tests/visual_motion_differential.py with --engine, --library, --report and
--reference-output; optional --asset-samples uses the host probe's VRSA rows.
Host visual_motion_audit takes kernel-fixtures.bin. Host
visual_motion_assets_audit takes asset directory, sample output, JSON report.
