# Path update coordinator

`navigation_controller.cpp` reconstructs GameObject::UpdatePath (0x3940c0),
IsAtDestination (0x39361c), SetDestination (0x393600) and the logical path/heading
part of Stop (0x3938f8). The manifest binds those routines and two Character
virtual policy getters to the supplied original ELF. This is reconstructed
source; the original studio source text is unavailable.

The original first copies GameObject position into its embedded PFObject even
when the path-update policy is disabled. Otherwise it consumes at most one
waypoint, sets the GameObject destination, and tests strict squared XY distance
below 6400. A nonempty path uses the final PF target for this test; an empty path
uses the GameObject destination. Z is ignored. Arriving with a requested but
empty path clears the request and heading and resets the GameObject destination
to its position. Heading angle is retained.

When not arrived, the coordinator sets heading from destination minus position,
optionally applies AvoidObstacles, then sets PF object flag 2 from the active
heading. Enabled boundary validation runs afterward unless its original debug
preference suppresses it. ValidateDirection's bool is diagnostic; the original
always reapplies SetHeadingDirection to the resulting self-aliased direction.
Consequently the PF active flag can remain set even if this final heading call
deactivates the heading. Native code preserves that order.

Avoidance sees PF's desired target and, when present, the first remaining path
edge target. These are distinct from GameObject's current destination. Native
scratch copies provide these views without changing other actors. Path segments,
actor views and retained floor-map keys use bounded caller-owned scratch. Invalid
requests or insufficient storage preserve controller/path/object/map/result
state, while scratch and collision-query workspaces may change.

Internal heading arithmetic accepts original intermediate NaNs and infinities,
including a finite-position subtraction that overflows only in Z. Public heading
APIs keep their existing finite-caller contract. The original-instruction corpus
compares 1,132 requests: 97 destination queries and 1,035 complete updates. It
includes 384 successive-state ticks, actual Crypt floor queries, waypoint
ownership, policy gates, stop requests, boundary vertices/misses, activation,
coincident forces and overflow branches. Both packaged ARM64 libraries and the
standalone library are compared. NaN arithmetic is compared by classification;
ordinary words, state and query order are exact under modeled IEEE/libm imports.

The desktop sanitizer audit replays the same corpus through the actual Crypt
asset loader, checks eight atomic rejections, and permits at most two ULP in
nonzero libm angles. Nine existing navigation/heading corpora are also replayed.
The API 37 emulator world regression checks the app's existing movement, facing,
boundaries, rendering, rotation and pause/resume. That runtime still uses the
development position adapter, not this coordinator.

Character's actual path/physics policy getters execute in the ARM32 oracle;
avoidance policy and debug preference are fixtures. Original Stop's calls to
physical velocity/angular velocity/position setters are service observations;
native output requests that work rather than implementing the physical body
backend. The final direct Box2D reset fields are outside the compared projection.
Native GameObject/Character construction, physical body/shape producers,
UpdateSubObjects, original input/root motion and live pursuit remain pending.
This source milestone is not a completed movement controller or full game.
