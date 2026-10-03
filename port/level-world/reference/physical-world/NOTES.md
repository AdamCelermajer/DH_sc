Original engine SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

PhysicalWorld load creates Box2D with supplied physics-unit bounds, zero gravity
and sleeping enabled, then installs the four listeners. Its update converts
the global unsigned millisecond interval through `__aeabi_ui2f`, multiplies by
float bits `0x3a83126f`, and calls `Step(dt,10)`.

ShouldCollide calls both object policies in order when both shape user pointers
exist. Otherwise it delegates to the original default Box2D contact filter.
Add/Persist/Remove calculate the instigator from shape1 mass, shape1 body
position, contact point and object1's virtual velocity. When mass1 is not
positive, the instigator is true unless mass2 is positive. Both recipients get
the same point and inverse instigator bits. Result sends fixed true/false
without point data. Boundary Violation has debug logging only; both destruction
callbacks are empty.

The differential supplies nonmutating filter/velocity/contact observers. It
omits the contact methods' debug logger/string and canary blocks but executes
their dispatch and complete instigator instructions. Step is an argument
observer for this corpus; the physics-backend corpus executes real simulation.
The NativeWorld character factory binds already-verified character definitions
to the recovered Box2D source; it does not implement game object allocation,
attachment ownership or pathfinder registration. Callers must keep WorldObject
services alive through body destruction and must discard body handles on clear.
