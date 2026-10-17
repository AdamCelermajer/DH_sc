# GameObject Stop caller

`GameObject::Stop()` at `0x3938f8` is reconstructed in the new portable
`game_object_stop` adapter. Its original ELF is
`libDungeonHunter2.so`, SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` binds the Stop body and the three PhysicalObject
setters called by that body to their original symbol ranges and hashes. Stop's
248-byte symbol range contains 236 bytes of ARM instructions followed by a
12-byte literal pool.

The caller order observed in the ARM body is:

1. Call `PFWorld::DropPath` with `this + 0x1c8`.
2. Copy the current XYZ position at `+0x160` to destination `+0x1a8`.
3. Clear byte flags `+0x1b5` and `+0x1b4`, then copy `Vec3f_Origin` to heading
   `+0x1b8`. The runner resolves the global through the original ARM
   instruction's GOT-relative load (`0x9989c4` in this ELF image), follows the
   pointer to `0x99f854`, and verifies its three binary32 words are all zero.
4. If `+0x2dc` is non-null, call the GameObject virtual at vtable offset
   `+0x64`, `IsUpdatingPositionFromPhysics`. A zero result ends the physical
   branch. The base `GameObject` implementation returns 1; the actual
   `Character` override loads `Character+0x520`, extracts bit 1, and returns
  that bit. This is a physics-position update policy, not a generic
  movability test.
   `Character::Character` loads its class vtable and stores the Itanium address
   point `0x965f38`; the `+0x64` entry at `0x965f9c` points to the override at
   `0x3a2e44`. The standalone Ghost source actor is a `Character`, not a
   separate C++ Ghost subclass. Source Idle flags `0x2380` and Move flags
   `0x23c1` both leave bit 1 clear, so Stop skips its physical-reset branch in
   those states. This does not establish every other source state's flag
   policy.
5. For a nonzero result, call `PhysicalObject::setLinearVelocity(0,0)`,
   `setAngularVelocity(0)`, and `setPosition(currentX,currentY)` in that order.
   The source reloads the physical pointer from `+0x2dc` for each call.
6. Reload the physical object and apply the inline body writes through
   `physical + 0x14`: set sleep flag bit 8; clear linear/angular velocity,
   force, torque, and sleep time.

The portable state is a typed 64-bit projection, not an ARM object overlay.
The request protocol keeps object/path/physical identities full-width. All
source calls that own path release, the Character physics-position gate, physical setters,
and transform application are required synchronous services; callback failure
stops at that point and keeps earlier effects. The last inline body projection
is resolved after the setter sequence using the then-current physical identity.

The host differential executes the original Stop, the actual `Character`
override selected through the pinned Character vtable address point, and the
three original setter bodies under Unicorn. It checks the override result for
source flag words, both physical branches, every
instruction in Stop's executable range, copied logical fields, physical body
state, service order, and the `SetXForm` arguments. The SetXForm observer
applies only returned XY to the body snapshot before Stop's inline writes; it
does not implement Box2D transform synchronization, proxies, contacts, or
stepping. The manifest also pins and verifies the complete `DropPath` symbol,
although DropPath itself is intercepted at the Stop boundary. The native host
provider uses the existing `dh2_nav_drop_path` against a nonempty normalized
path, while DropPath's original implementation has its own differential in
`reference/navigation-path`.

`run_game_object_stop_host.py` also checks nonempty and empty native path
projections, service failure prefixes, no physical object, zero and nonzero
virtual results, exact transform requests, and identities above 4 GiB. This is
a reusable source caller kernel; no live Native Ghost/controller wiring is
claimed. `GameObject::LookAt(Point)` is already covered separately by
`character_path_commands.cpp` and remains unchanged.

Reproduce from the repository root:

```powershell
python port/level-world/tests/run_game_object_stop_host.py `
  --original-elf ..\test_strategy\libDungeonHunter2.so
```
