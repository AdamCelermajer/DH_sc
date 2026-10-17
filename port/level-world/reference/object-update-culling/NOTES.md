# ObjectBase update culling and remote predicate

Original ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The accompanying manifest pins the exact symbols, ranges and bytes.

## Complete maintained bodies

| Original | Source implementation | Boundary |
|---|---|---|
| `ObjectBase::TestCullingBeforeUpdate(aabb<float> const&)`, `0x33de90/432` | `object_update_culling::evaluate` | Complete 424 instruction bytes plus 8 literal bytes; online, remote virtual, current-Level and camera-frustum queries remain mandatory reached services |
| `ObjectBase::IsRemotelyUpdated() const`, `0x33dd10/20` | `object_update_culling::is_remotely_updated` | Complete five-instruction leaf: word `+0x110 != 0xffffffff` returns 1; otherwise return the raw byte `+0x118` |

The older controller/acquisition modules expose the remote query as a provider;
they do not contain this leaf implementation. No culling/frustum implementation
with equivalent ownership and ordered field reads existed in the maintained
source. Arithmetic uses separate binary32 operations, without fast math or FMA.
Irrlicht's native renderer types are not overlaid onto these logical views.

## Source order and numerical predicate

1. Capture the Object and AABB pointers. Call `GetOnline` (`0x7fd794`), then
   read its byte `+5`. If nonzero, call Object virtual `+0x54` on the captured
   Object. **Its returned word is discarded**; provider changes to Object
   fields remain visible to the subsequent phase read.
2. Fresh byte `+0x86 == 0` writes 1 and returns true. A byte other than 1 returns
   true without a store or camera access. Only phase 1 takes the geometry path.
3. Capture AABB words before `Application::GetCurrentLevel` (`0x31f594`) in
   source order: maxZ, minX, minY, minZ, maxX, maxY. Provider mutation of the
   live AABB afterward does not replace the captured geometry. A null Level
   writes Object `+0x86 = 2` and returns true.
4. A nonnull Level reads `+0x128`, then that camera owner's `+8`, then calls
   the captured camera node's virtual `+0x144`. Keep its returned frustum live
   across the entire six-plane loop. The caller does not reread the Level or
   camera pointer after this virtual call.
5. Plane `i` is returned-frustum `+0x0c + i*0x10`: normal XYZ and D. A normal
   component `>= +0` chooses the captured **minimum** face; otherwise it chooses
   the maximum. Signed negative zero also chooses minimum; NaN chooses maximum.
   Evaluate `nx*x`, `ny*y`, their sum, `nz*z`, its sum, then add D, rounding each
   operation to binary32. A distance **strictly greater than zero** returns
   false immediately. Equality and unordered NaN do not reject.
6. All six planes passing writes Object `+0x86 = 2` and returns true. Rejection
   does not write this byte, including a value changed by a preceding provider.

The sign is verified by executing the actual relocated import stubs before
numeric modeling: `0x30e4b4 = __aeabi_fcmpge`, `0x30ed6c = __aeabi_fmul`,
`0x30eba4 = __aeabi_fadd`, **`0x30e2f8 = __aeabi_fcmpgt`**. An earlier integration
draft assumed a less-than comparison from the branch shape. That assumption
was corrected before this source kernel or comparison proof was published.

ObjectBase constructor field producers are separately pinned: C1
`0x33f15c/436` sets byte `+0x86 = 0` at `0x33f264`, word `+0x110 = -1` at
`0x33f2d8`, and byte `+0x118 = 0` at `0x33f2c8`; C2 `0x33f310/436` has the
same writes at `0x33f418`, `0x33f48c`, and `0x33f47c`. Their zero/minus-one
register producers are included in the manifest. ObjectManager's offline
StartWork arm clears the live `+0x86` at `0x34aa24` before virtual `+0x24`.
These are external lifecycle producers, not extra functions reconstructed by
this module. The runtime must retain their subsequent live writes; it must not
reset these fields to constructor defaults inside an unrelated synchronization.

## Camera dependency and vtable address point

The camera call is not an ordinary ObjectBase vtable lookup. The scene camera
constructor at `0x583734/324` installs its primary address point with
`add r1,r3,#0x1c` at `0x5837dc`, then `str r1,[r4]` at `0x5837e4`.
Its vtable GOT entry `0x9958c0` points to `_ZTVN6glitch5scene16CCameraSceneNodeE`
at `0x975400`. The address point is `0x97541c`; slot `+0x144` at `0x975560`
contains `CCameraSceneNode::getViewFrustum` (`0x582138/8`). The collada camera
address point `0x98b5fc` has the same slot at `0x98b740`. The getter returns
`this + 0x168`; the culling caller reads planes at its return `+0x0c`.

These are dependency and pointer-producer facts. The camera constructor,
frustum calculation, renderer camera publication and getter are not added to
the new complete-body count. `CameraVisual` in the header is a logical camera
owner view, not a claim that the source field is a `VisualObject`.
Character::CanUpdate's captured Character Visual/root is owned by the separate
eligibility caller; this culling helper reaches the Level camera chain.

## Ownership, failures and port guards

Object, source AABB, global projection, Level, camera, node, frustum and their
backing storage remain alive on one owning thread through synchronous return.
Metadata identities and addresses remain stable; providers may mutate live
fields and camera/root pointers at the original query boundaries. The service
table is copied so changing the caller's table does not revoke callbacks during
the current invocation. There is no camera-distance approximation, substituted
constant acceptance, synthetic phase reset or invented frustum.

Null Level is a real source branch. A taken null/malformed camera, root or
frustum is an explicit invalid-source-fact error; unavailable, nonzero-returning
or throwing providers fail after retaining prior effects. There is no rollback
or extra cleanup. Port alignment/range/control-alias checks run before typed
reads and output writes. Known optional AABB/global aliases are rejected before
output initialization; null optional inputs are accepted until phase 1 needs
them. Reached camera facts are validated before dereferencing. Same-Object
reentry is rejected; independent Object/output evaluation is allowed. These
checks are port protocol, not claimed original ARM branches.

## Reproduction and scope

Run `tests/run_object_update_culling_host.py --original-elf <verified ELF>
--compiler <C++17 compiler>`. The runner verifies exact range hashes, camera
vtable bytes and constructor address-point instructions, and actual relocated
PLT names before modeling soft-float arithmetic. It compares complete original
ARM returns, phase writes, service order/receivers, captured AABB and final
plane arithmetic against the maintained source. Finite arithmetic bits are
exact; arithmetic NaN payload/sign are compared as unordered NaN class.
Coverage is collected per instruction, excluding the eight literal bytes.

The host proof includes all plane positions, all component sign combinations,
raw online/remote values, null Level, signed zero, NaN/infinities/subnormals,
provider mutation, and pointer/alias/reentry/partial-failure checks. Reference
fixtures provide actual six plane words at the source camera boundary, not a
claim of complete camera ownership. The validation report records compiled
sources, replay cases and executable hashes. Native eligibility/camera binding,
graphics lifecycle and whole-engine/gameplay parity remain pending.
