// Track B batch p1-glitch-scene-collada-gui-pack0-837d16-0 (platform pass): the two scalar deleting
// destructors (D0) of glitch::CAndroidOSDevice and of its nested CCursorControl. Both are skipped.
//
// The batch was queued for the platform pass, which brings back JNI and Android glue. These two are
// not JNI functions. A D0 is the compiler-generated `delete p` path: it runs the complete-object
// destructor, then frees the storage through the original's allocator (LC_API_FREE_0). README.md
// lists a compiler-generated thunk as `skipped`, and TRACK-B.md rule 4 says thunks are not
// reimplemented. The C++ delete expression produces the same shape, as in
// glitch-video/include/glitch-video/material.hpp (`if (p->m_refCount-- == 1) delete p;`).
//
// The shapes below were read from the assembly (py -3 tools/decomp_asm.py --address 0x...).
// The class declarations are in glitch-scene-collada-gui/include/glitch-scene-collada-gui/platform_stubs.hpp.

// 0x6a04ec _ZN6glitch16CAndroidOSDevice14CCursorControlD0Ev (skipped)
//   PUSH {R4,LR}; BL LC_API_FREE_0(this); returns this. The complete destructor is empty
//   (0x6a0350 D1, reconstructed as `~CCursorControl() = default`), so the compiler does not call it.
//   The equivalent C++ is `delete cursorControl;`.

// 0x6a05b0 _ZN6glitch16CAndroidOSDeviceD0Ev (skipped)
//   PUSH {R4,LR}; BL _ZN6glitch16CAndroidOSDeviceD1Ev (complete destructor at 0x6a0548, a partial
//   row: its member teardown layout is not mapped); BL LC_API_FREE_0(this); returns this.
//   The equivalent C++ is `delete device;`.
