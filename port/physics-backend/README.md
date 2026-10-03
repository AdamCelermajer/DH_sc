This is the recovered native Box2D backend for DH2, based on the official
Box2D 2.0.1 release from the original project's
[Google Code archive](https://code.google.com/archive/p/box2d/downloads).
It compiles original C++ physics source for real 64-bit bodies, shapes,
broadphase pairs, contact manifolds, constraints, islands and continuous
collision/world stepping. It does not execute the original ARM32 engine.

The source archive's published SHA-1 is
`27a1a0bd08c81bbf661fa008645ee5c538bb2767`; the downloaded archive matches it.
Its SHA-256 is `62857048aa089b558561074154430883cee491eedd71247f75f488cba859e21f`.
The original DH2 ELF's `b2_version` at `0x99e228` reads `{2,0,1}`. Its body,
circle, polygon and world layouts match compiled release source, including
world lock `+0x191d4` and body mass `+0x74`. SetXForm, body constructor,
SetMass/SetMassFromShapes and Step match the inspected source algorithms.

Two source alterations are explicitly marked in the vendor files:

- `Source/Common/b2Settings.h` uses the DH2 fork's binary-recovered 2048 proxies
  and 16384 pairs. Original constructor loop bounds and pair/broadphase offsets
  prove this difference from official defaults of 512/4096.
- `Source/Common/b2Settings.cpp` reserves an aligned native allocation header.
  The release's `malloc(size+4)+4` misaligns objects containing 64-bit pointers.
  The size header and byte accounting are retained with the native header size.

All other 63 vendored files match official source bytes. Original license
notices and `License.txt` remain intact. `reference/provenance.json` records
both upstream and native hashes for every file. Build flags disable fast math
and floating-point contraction. A forced `cstring` include supplies declarations
that old platform headers provided transitively; it does not change arithmetic.

Integrate with `add_subdirectory(../physics-backend ...)`, link `dh2_box2d_201`,
and include `<Box2D.h>`. Public headers retain this historical API: b2ShapeDef,
b2CircleDef/b2PolygonDef, BodyDef massData, CreateShape, SetXForm, SetMass,
SetMassFromShapes and `Step(dt,iterations)`. This is not a modern fixture API.
Callers supply DH2's owner/contact listeners, authored geometry and lifecycle.

`tests/differential.py` executes the original full world constructor, allocator
clients, CreateBody/CreateShape, circle/polygon mass, broadphase, default filter,
contact creation, solver/island/TOI, SetMass, SetXForm, Step and destruction.
It compares these with the complete source-built ARM64 library. The only
fixtures are imported allocation/libc/IEEE arithmetic/trig and contact listener
observers; shape, mass, broadphase, collision, contact and solver services execute
real original/native instructions. The local ELF loader extension resolves
ARM64 exported-global and vtable relocations needed by the complete library.

The audit passed 40 representative worlds, executing 104 original physics
functions, with exact 37-word body snapshots and 8 world/listener words.
Cases include circles/boxes, dynamic/static/sensor/filter/fixed-rotation/bullet
policies, gravity/zero dt, successful and out-of-range SetXForm, zero-mass pin,
high-speed velocity limiting and continuous collision. A host audit passed the
same worlds under ASan/UBSan with real host libc/math: 157 world steps, 68 contact
listener events and balanced allocation accounting. The ARM64 audit models
imported trig identically on both CPUs; host math parity is verified for these
representative fixtures. Arbitrary math inputs can still differ across libm
implementations by one ULP.

The tests cover representative game-relevant physics, not every possible source
branch. Joint dynamics, arbitrary authored polygons, all tuning-boundary cases
and DH2-specific gameplay listeners are not established by this backend audit.
Parent integration independently reconstructs those gameplay listeners and owner
lifecycle; no generic replacement policy should be inferred from this target.

For an isolated sanitizer host audit, configure this directory with
`-DDH2_PHYSICS_BACKEND_BUILD_TESTS=ON` and sanitizer compiler/linker flags, then
run `dh2_backend_audit` with the generated original fixture path. The optional
audit target stays disabled during ordinary parent builds. NDK oracle and
provenance scripts are in `tools/`; the original fixture is
`.local-inputs/physics-backend-discovery/full-world-reference.bin`.
