# UpdatePath physical Stop composition

`controller_physical.cpp` combines the unchanged recovered UpdatePath API with
the native physical controls and SetXForm kernel. These original routines are
identified by the manifests in ../navigation-controller, ../physical-controls
and ../body-transform. The original ARM32 engine only executes in desktop tests;
the Android APK contains rebuilt ARM64/x86_64 source libraries.

The base coordinator preserves its API and emits a physical_stop_requested fact.
The wrapper requires the scene's body-presence view to match the supplied native
binding, validating pointers/reserved fields/callbacks before coordinator writes.
On a physical Stop it zeroes linear/angular velocity through the recovered
setters, requests the game XY scaled by .01 and the current body angle, invokes
the actual native SetXForm kernel, and finally sleeps the body and resets linear
velocity, angular velocity, force, torque and sleep time. Original Stop ignores
SetXForm's bool: locked, frozen or proxy synchronization failure still receives
the final reset sequence. The wrapper preserves this behavior.

The composed corpus executes original UpdatePath, empty DropPath, actual physical
setters and actual original SetXForm. Its 1,186 cases cover policy/body/request
gates, signed zero, finite-position overflow, nonfinite physical angles, locks,
frozen bodies and failing shape callbacks. All controller/navigation/path/body
fields, kernel return and ordered synchronize/destroy/commit events compare.
The existing 1,132 coordinator cases separately cover nonempty waypoint behavior,
avoidance and geometry. The composed corpus deliberately uses empty paths.

Host ASan/UBSan replay covers all 1,186 composed cases and five atomic binding
rejections. Trig imports are identically supplied from the verified original
corpus; production source retains platform sinf/cosf. Arithmetic NaN payload/sign
is not portable. This evidence does not establish device libc/GPU parity.

Shape synchronization, proxy destruction and broadphase commit remain explicit
caller backends. Character definitions do not allocate bodies or calculate mass.
Physics world stepping/contacts, pin/unpin lifecycle, visual/root motion and live
player/enemy movement remain pending. This source milestone is not a completed
physics engine, movement controller or playable full game.
