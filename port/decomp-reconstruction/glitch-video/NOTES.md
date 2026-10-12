# glitch-video notes

Ledger: 315 rows (221 reconstructed, 10 partial, 84 skipped). Queue total 1345, about 1030 still open.

## Layouts inferred

- CFPSCounter (+0x00 fps=60, +0x04 primitives of last frame, +0x08 last reset time,
  +0x0c frames since reset, +0x10 primitive sum since reset, +0x14 primitive average,
  +0x18 primitive total, +0x1c draw calls, +0x20 2D draw calls, +0x24 texture bindings).
  registerFrame recomputes fps and average once `now - last > 0x5db` (1499 ms), using
  float reciprocal and f2d/d2uiz conversions (helpers at 0x30e514 / 0x30e4cc, read as
  __aeabi_f2d and __aeabi_d2uiz from the call pattern; not confirmed against the helpers).
- CVideoModeList (vtable +0x00, +0x04 = 1 unidentified, +0x08 begin, +0x0c end,
  +0x10 capacity end, +0x14 desktop dimension, +0x1c desktop depth). Elements are 12 bytes:
  resolution (w,h) then depth. Port uses std::vector and keeps the offsets as comments.
- CImage: +0x10 width, +0x14 height (only these are reconstructed).
- IReadFile: read() at vtable +0x0c. Upstream Irrlicht has read() at slot 2, so the glitch
  base class carries one extra virtual before it. The stand-in in deps/glitch_io.hpp keeps
  a placeholder slot.
- CImageLoaderBMP/TGA, CImageWriterTGA: vtable and +0x04 = 1 in the constructors.
- CNullDriver: the four 2D/colour setters are empty bodies.

## Decisions

- Mangled-name C1/C2 (and D1/D0) pairs share one C++ definition. The deleting D0 variants
  are compiler generated and are ledgered as `skipped`.
- Extension checks treat the original's "return pointer or 1" as a bool.
- CVideoModeList getVideoModeResolution/Depth: original uses `index <= count`. Port uses
  `index < count`, so it does not read one element past the end. Ledgered `partial`.
- The port adds two test helpers, not in the original: CVideoModeList(vector, ...) and
  CImage(w, h). They are marked in the headers.

## Blockers and open items

- pixel_format::detail::PFDTable (1600 bytes at 0x8e36c0 in .rodata, 40 rows of 0x28) is
  not in the repo. The table bytes are absent from recovered/native/assembly and symbols.
  getPackedType, getFormat, computePitch, computeSizeInBytes, computeMipmapSizeInBytes,
  swapBytes, convert, and every converter depend on it. Blocked until the table is dumped.
- SColor::getInterpolated, SColorHSL::toRGB/toRGB1, SColorf::equals: decompiler output is
  garbled (soft-float calls read as tail calls). Need a careful pass over the assembly
  listings before translating.
- CMaterial::getHashCode / equals / operator<, CVertexAttributeMap, CMaterialVertexAttributeMap,
  CVertexStreams, CTextureManager, shader and material parameter classes: not started. Most
  depend on boost::intrusive_ptr, GL function pointers or shared-memory block headers.
- Remaining image loaders (PNG, JPG, DDS, PVR, ATC) and writers (JPG, PNG) are not started.

## Batch 2 (this session, continuation)

- CNullDriver: full method set from the asm (126 entries). Bodies were taken from the
  listings: empty, constant returns, out-parameter writes. Nested CBuffer, CTexture and
  CFramebuffer are included. getScissor and getTransform are `partial`: they read driver
  state that is not reconstructed.
- IVideoDriver (video_driver.hpp): member offsets +0x44 (max texture size), +0x50 (CFPSCounter),
  +0xf4 (shader pointer cleared by onShaderDestroyed). Base classes are not modelled.
- CCommonGLDriverBase (ccommon_gl_driver_base.hpp): state getters. Member offsets are from
  the asm, and member names come from the accessor names.
- Texture loaders (texture_loaders.hpp): constant probes and CDataInfo getters. The base
  classes are shown as simple inheritance; the original base layout is not confirmed.
- Destructors: deleting thunks (bl operator delete) and empty destructor bodies are ledgered
  `skipped` as compiler generated, with the reason in notes.
- deps/intrusive_ptr.hpp: boost::intrusive_ptr-compatible pointer without reference counting.
- deps/gl_function_table.hpp: minimal GL function table, not yet used by reconstructed code.
- Symbols in comments were copied from queue.csv (mangled), not guessed.

## Still open

- SColor::getInterpolated, SColorHSL::toRGB/toRGB1, SColorf::equals: soft-float bodies. Helper
  mapping (0x30e70c fcmp, 0x30e3ac fsub, 0x30ed6c fmul, 0x30eba4 fadd, 0x30e2e0 ui2f,
  0x30e4cc d2uiz, 0x30e514 f2d) is inferred from call patterns and not yet verified.
- IMaterialParameters templates and setParameterAt/getParameterAt conversions: need the asm
  for the helper calls before translation.
- CProgrammableGLDriver / CCommonGLDriver<...> template instances: the template is not
  modelled; getters and shader-manager accessors are still open.
- getStringsInternal (string tables) and the pixel_format PFD table: data not in repo.
