# GameObject heading reconstruction

`navigation_heading.cpp` reconstructs `GameObject::LookTowards` (0x393b1c,
204 bytes) and `GameObject::SetHeadingDirection` (0x393be8, 260 bytes).
The adjacent manifest binds those ranges to the owner-supplied original ELF.
These are reconstructed implementations, not recovered historical source text.

LookTowards ignores Z. A zero XY vector retains the previous angle. On the
horizontal axis it writes the original float words for pi/2 or 3pi/2. Otherwise
it computes atan(x/-y), adding positive pi when y>0 and x>0, negative pi when
y>0 and x<=0. Replacing this with atan2 would change the stored angle in some
quadrants even where the visible orientation is equivalent.

SetHeadingDirection copies XY, clears Z, and activates the heading only when
the squared XY magnitude is strictly greater than float word 0x38d1b717. It
normalizes only when that magnitude exceeds one. Products, sums and division
retain individual float rounding. Rotation reads the input pointer after the
heading write: a pointer to the object's own direction therefore observes the
normalized vector. Both separate-input and self-alias calls are compared.

The native API rejects malformed inputs, overflowing squared magnitude,
nonzero reserved storage and nonboolean rotation before changing state. Direct
LookTowards output must not overlap the direction; SetHeadingDirection permits
its documented self-direction alias only. These are native caller contracts,
not claims about the original engine's handling of malformed memory.

The original-instruction corpus contains 4,476 calls, including 2,171 aliases,
zero/sign boundaries, activation and normalization thresholds, seeded vectors
and 128 circular directions. The standalone ARM64 library and both APK ARM64
libraries match every state word. Their imported atan/sqrt services are modeled
consistently; this does not prove bitwise device-libm identity. The host sanitizer
audit permits at most two ULP in nonzero angles, compares all other words exactly,
and checks six atomic rejections. Its current replay has zero libm variants.

Android's touch movement calls SetHeadingDirection after a successful supported
floor movement. Player melee calls LookTowards on the selected target delta
before starting an attack. World smoke checks actual touch-facing logs for the
stairs and east boundary; combat smoke checks every selected attack direction
and retains its original damage/death comparisons. These are live facing calls,
not a startup-only heading probe.

Position advancement still uses the development supported-floor adapter. The
full original UpdatePath, physical body update, root motion and pursuing enemies
remain pending. Disassembly establishes that UpdatePath at 0x3940c0 coordinates
MovePath, destination tests, heading, AvoidObstacles and ValidateDirection;
UpdateSubObjects at 0x3943cc applies physical/visual position updates. Those
coordinators and their producers must be reconstructed before the navigation
helpers can faithfully drive the complete character controller.
