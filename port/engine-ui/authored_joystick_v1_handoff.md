# Original authored joystick branch

Retain ONE `AuthoredJoystickStateV1` as the same HUDControls projection:
radiusx/y+C/10,centerx/y+14/18,direction+65c/660/664,magnitude+668,activebyteA.
Original constructor41af1c initializes these zero. Actual initCachedChars
41a048..084 reads `Joystick.bg` virtual128 width and computes trunc(width/20*.5).
Owner `joystick_background_width` delivers actual scoped `bg->get_width()`.

Use `joystick_receiver_geometry(screenX,Y,out,error)` for the actual stick
receiver. `out.local` is inverse-world-transformed pointer in twips;
`local_matrix[2]/[5]` are receiver LOCALtranslation. Source event4 stores
center=trunc(localXY/20). Source event5 uses **PLUS**localtranslation:
dx=trunc((localX+localTX)/20-centerX), same dy. atan2(dy,dx), component clamp
to radii, magnitude=sqrt(clampedX²+clampedY²)/radiusX. Above1, replace position
with trunc(radiusX*cos(angle)),trunc(radiusY*sin(angle)). Deliver original
SetPosition; then CTRLIsAllowed, normalize(1,-1,0), rotateXY by
angle*bitfloat(c2652ee0)+90 at origin0, publish active1.

PLUS is proven original import30eba4 __aeabi_fadd; subtraction30e3ac is fsub.
LOCALmatrix pointerCharacter+4c is loaded/mutated by SetPosition7aa40c;
worldmatrix get753f74 returns+78 instead. The cursor sender uses inverseworld
localXY for both events (swf_input_connection_v2.cpp78). Frozen SwfEvent48's
labels differ: original Event40+12/+16 map to value0 floatbits / x, NOT x/y.

Bind `position_stick` to owner `joystick_stick_position(intPixelsX,Y,error)`
which replaces actual translation, sourcepixels*20. Bind rotate callback to
actual dh2_vec3_normalize then dh2_vec3_rotate_xy. Bind controller predicates,
head and stop to the SAME retained player/controller. Call press/drag/release
kernels with the exact receiver geometry. Release event6/7 resets center,
delivers stick0, publishes active0, then Cmd_Stop when player exists.
Magnitude is retained. Failure preserves reached stores and ordered prefix.

`authored_joystick_update_v1` is source joystick-only Update41a840/978..9d0,
after genuine outer level/readiness/local-player gates. No deadzone: deliver
direction*magnitude to actual Cmd_HeadTowards, including zero vector.
Cmd_HeadTowards/Stop source actor transport is still a separate required owner.

Original1000whole event5 branches through position and CTRL-false exit have
exact binary32/signed-position native replay, O1/O2 ASAN/UBSAN4010checks each.
Additional tests cover zero radius, zero input, scaledheading, required-prefix
failures and release order. Direction/controller are explicit host observers;
whole unrelated HUD event selection/customization/level/attack gates external.
BothABI strict compilation passed. No complete actor movement claimed yet.
