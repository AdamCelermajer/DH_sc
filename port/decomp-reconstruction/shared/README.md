# Shared layouts (owned by the integrator)

The one definition of each type that more than one glitch category needs. Every category's CMake target
adds `shared/include/`, so the headers here are included as `glitch-shared/<name>.hpp`.

- `include/glitch-shared/glitch_core.hpp`: the glitch::core value types (vector2d/3d/4d, position2d,
  dimension2d, rect, line2d/3d, aabbox3d, plane3d, triangle3d, quaternion, CMatrix4) and the
  glitch::video colour words (SColor, SColorf). glitch-video and glitch-scene-collada-gui include it
  through their deps/ headers. Members are zero-initialised. The rect members are UpperLeftCorner and
  LowerRightCorner.
- `include/glitch-shared/sid_collection.hpp`: glitch::core::detail::SIDedCollection layout, confirmed on the
  ITexture instances. glitch-core-io's core/sid_collection.hpp forwards here.
- `include/glitch-shared/gl_function_table.hpp`: glitch::video::deps::GLFunctionTable (GL ES 2.0 entries).
  glitch-video's deps/gl_function_table.hpp forwards here.

Not shared, and kept in their category on purpose:

- glitch-core-io's deps/glitch_core.hpp: its own copy, with the core-io stubs (IReferenceCounted, the video
  and scene classes) and the rect members X1, Y1, X2, Y2. Its 33 uses of X1..Y2 stop it from moving here
  until core-io renames them.
- The intrusive_ptr forms (core-io counts references; video and scene do not). Unifying them changes runtime
  behaviour, so the owning threads decide.
- core-io's sided_collection.hpp: a second SIDedCollection implementation, not identical to the shared copy.
- core-platform's deps/glitch_core.hpp: its own copy.
