# Physical pin/unpin and original Box2D 2.0.1 mass lifecycle

The adjacent manifest/disassembly binds PhysicalObject::pin (0x46eb20), unpin
(0x46eae0), b2Body::SetMass (0x7e1b28) and SetMassFromShapes (0x7e1818) to the
owner-supplied ELF SHA256. `physical_lifecycle.cpp` reconstructs those routines
using a native logical LifecycleBody view, a TransformBody and original shape
services. It is not a private binary overlay or historical source text.

pin first sets PhysicalObject+0x27 to1, then calls SetMass with mass0, inertia0
and the existing body local center. If already pinned it does nothing. unpin
first clears that byte, then calls SetMassFromShapes and wakes the body, clearing
sleep bit8 and sleep time. If already unpinned it does nothing. Locked worlds
make either mass call a no-op, but the pin byte still changes, and active unpin
still wakes. Neither routine adjusts velocities, force or torque.

SetMass resets inverse mass, inertia and inverse inertia, copies supplied mass
and computes its inverse only if mass>0. Fixed-rotation bit0x40 forces inertia0;
otherwise supplied inertia is copied and inverted only when positive. Unlike
SetMassFromShapes, it does not subtract a center offset. There is no modern
minimum mass or velocity correction. SetMassFromShapes sums each shape's mass,
mass-weighted center and inertia in original list order. Positive total mass
normalizes center by inverse mass. Positive inertia with nonfixed rotation
subtracts mass*(centerX^2+centerY^2) and inverts that result without a new guard;
otherwise inertia and inverse inertia become zero.

Both mass routines rebuild world center from the current origin/rotation and
new local center, copy it to previous center, and invoke UpdateSweepRadius for
every shape. They then set body type0 when both inverses compare equal to zero,
type1 otherwise. In this 2.0.1 binary these are static0 and dynamic1. A type
change invokes RefilterProxy for all shapes after all sweep-radius calls. Shape
ComputeMass, UpdateSweepRadius and RefilterProxy are explicit services in this
bounded reconstruction; the separately recovered genuine 2.0.1 backend can
provide them. Their geometry/broadphase implementation is not claimed here.

The 4,846-case original-instruction versus ARM64 audit includes repeated carried
pin/unpin sequences, all pin/lock/type/fixed-rotation gates, signed zeros,
subnormals, infinities, NaNs and seeded raw float words. It compares all logical
fields, 3,545 shape mass calls, 6,991 sweep-radius calls and 4,617 refilter calls,
including the current pin/type at every callback. There are2,270 type changes
and385 locked cases. All finite words match; arithmetic NaNs compare by class.
Velocity/force/torque words stay bitwise unchanged. Four malformed caller checks
reject before mutation/services. The same original-derived gold corpus passes
native ASan/UBSan replay, not implementation-derived expectations.

Caller captures under `callers/` identify genuine character integration:

- POCharacter constructor0x3b4010 calls pin at0x3b4070 after its base constructor.
- CSMove::OnFocus0x3c3bf8 writes flags+0x520=0x23c1, clears+0x53c, calls0x3c0f18,
  then unpins the physical object if present at0x3c3c8c.
- CSMove::OnBlur0x3c3aa4 calls GameObject::Stop at0x3c3b08, then pins the present
  physical object at0x3c3b18.
- GameObject::SetPhysicalObject0x394bf8 optionally pins an attached body when
  its supplied boolean is true, then updates subobjects via0x393ea0.
- CharStateMachine::RaiseStateEvent0x3c5684 treats event48 specially: if its
  original idle query succeeds and a body exists, it pins before dispatching.

A scan of direct ARM B/BL xrefs also finds matching focus-unpin/blur-pin in
CSStunned, CSScared, CSKnockedBack, CSSkill and CSLiftingMove; CSAttack has
conditional pin/unpin calls in focus/event and pin on blur. LiftableObject has
additional lifecycle calls. These observations identify integration boundaries;
the full state machine, attack condition producers and liftable logic are not
reconstructed here. In particular CSMove's0x23c1 does not set bit1 tested by the
physical-position policy getter, so assuming every move-state Stop requests
physical resets would misrepresent the original flags.
