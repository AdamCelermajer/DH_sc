# PhysicalObject controls and GameObject Stop physical sequence

`physical_controls.cpp` reconstructs the original seven PhysicalObject routines
in the adjacent manifest: setLinearVelocity (0x46e918), addLinearVelocity
(0x46e864), setAngularVelocity (0x46e978), getPosition (0x46e818), getAngle
(0x46e858), getRadius (0x46e750), and setPosition (0x46ea80). The eighth captured
routine is GameObject::Stop (0x3938f8); only its physical portion is implemented
here. These implementations derive from instructions, not historical source.

The 48-byte native BodyState is a logical view, not an original binary overlay.
Its fields correspond to original body flags+0, origin XY+4/+8, angle+0x38,
linear velocity+0x40/+0x44, angular velocity+0x48, force+0x4c/+0x50,
torque+0x54 and sleep time+0x8c; radius comes from PhysicalObject+0xc.
Pointers passed through these native APIs are 64-bit. The original body flags
are loaded/stored as 16-bit values, so widened caller flags above 0xffff reject.

Flag 8 is the sleeping bit in this binary: Stop sets it after clearing velocity,
force, torque and sleep time; nonzero linear requests and every angular request
clear it and reset sleep time. The original body constructor at 0x7e1e4c sets
that same bit from body-definition byte+0x29 (instructions 0x7e1e9c-0x7e1eb0).
This layout is older than the later Box2D awake/bullet flag layout. Naming bit8
as static would contradict these observed sleep/wake operations.

setLinearVelocity writes supplied XY bits directly. Either component unequal
to zero wakes the body; both signed zeros retain its previous sleep state/time.
NaN comparisons are unequal and wake. addLinearVelocity uses the *increment*,
not the resulting sum, for the wake decision, then adds and independently
replaces each component only if its sum exceeds that component's upper cap.
Negative caps are meaningful; there is no lower clamp or magnitude limit.
setAngularVelocity always wakes, including when supplied angular velocity is
zero. Linear and angular setters retain every supplied float bit.

getPosition and getRadius multiply by the original float100 constant.
setPosition multiplies game XY by float word0x3c23d70a (0.01f) and invokes
b2Body::SetXForm with the current body angle. TransformRequest records these
exact arguments and pending1. It does not update the body or any shape proxy.
The caller must execute the recovered Box2D backend separately.

Stop's physical gate requires a physical object and a nonzero virtual physical
movement query. stop_begin performs the exact zero linear/angular setter
sequence and emits the position transform. The caller applies that transform
before stop_finish sets sleeping and clears linear/angular velocity, force,
torque and sleep time. Stop's path release, destination copy, moving/heading
flags and default heading copy belong to the parent controller implementation.

The differential test executes the complete original Stop and all seven original
physical routines, with supplied DropPath and virtual movable query services.
Its SetXForm observer compares exact arguments and records the intermediate
body state. During Stop, a caller fixture copies only emitted XY before the
original direct resets. This does not validate actual b2Body transforms,
shape synchronization, collision contacts, mass/pin behavior or world stepping.
The separate transform reconstruction can compose with these helpers without
changing this bounded audit's claim.

The original-vs-ARM64 audit contains 5,446 calls, including 1,350 float-boundary
cases and 4,096 seeded raw-word cases. All finite state/argument words match;
generated arithmetic NaNs compare by classification, with seven field variants.
Supplied velocity words match exactly even for NaN payloads. All four Stop gate
combinations run, 907 path releases are observed, and 1,122 emitted transforms
are compared, including 215 Stop intermediate states. Unmodeled original body
bytes are checked for preservation. Fourteen malformed native caller checks
reject atomically. Original imported IEEE operations are modeled consistently.

The ASan/UBSan native host replay consumes that instruction-derived corpus;
it passes all 5,446 with zero NaN variants and no sanitizer errors. It does not
mirror the implementation to derive expected answers. The binary reference is
kept under `.local-inputs/physical-controls-reference.bin`; tracked reports bind
it by SHA256. No ARM32 original engine is included in a native runtime.

`controller_physical.cpp` additionally composes UpdatePath's physical Stop
request with stop_begin, the separately recovered b2Body::SetXForm kernel and
stop_finish. A dedicated 1,186-case audit executes original UpdatePath, the
actual original empty-path DropPath, actual setters, and actual SetXForm before
Stop's final resets. It compares full controller/PF/path/body/transform state
and ordered shape/broadphase service arguments. There are 391 original Stops,
294 physical Stops, 54 locked and 64 frozen bodies, 87 synchronization failures,
89 commits, 289 synchronization calls and 238 proxy destruction calls. Signed
zeros, finite positions up to +/-3e38 and nonfinite body angles are covered.
Boundary steering, avoidance and nonempty paths stay in the existing separately
verified core-controller corpus; this corpus adds physical orchestration.

The composed ASan/UBSan host replay passes all 1,186 with five atomic binding
rejections preserving controller, PF/path, physical state, output and callback
counts. Imported cos/sin are the same controlled service as the two instruction
CPUs: the host test supplies the original-verified trig outputs and asserts each
requested angle. It also covers the optimizer-combined sincosf import. Host
glibc can differ by one ULP without these fixtures; no arbitrary body-word
tolerance is permitted. Production transform code retains its native libm.
