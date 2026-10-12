# Shared layouts (copies, owned by the integrator)

Copies of the dependency layouts that glitch-scene and the glitch-core-io and glitch-video categories
both need. The category copies stay where they are for now; nothing here is built by CMake.

- `include/glitch-shared/sid_collection.hpp`: SIDedCollection layout, confirmed on the ITexture
  instances from the binary (see glitch-core-io NOTES, "SIDedCollection layout (dependency pass)").
- `include/glitch-shared/glitch_core.hpp`: core math stand-in (vector3d, plane3d, triangle3d,
  aabbox3d, CMatrix4 members). Copy of glitch-core-io deps/glitch_core.hpp.
- `include/glitch-shared/gl_function_table.hpp`: GL ES 2.0 function table entries (glitch-video,
  PLT and GOT evidence in libDungeonHunter2.so).

Not copied yet: `CMatrix4::operator*` (glitch-core-io core/matrix4_operators.hpp) depends on
core/matrix4_base.hpp.
