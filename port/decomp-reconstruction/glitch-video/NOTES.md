Ledger: 581 rows (349 reconstructed, 70 partial, 162 skipped). Queue total 1345, 764 still open.

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

## Blockers and open items (earlier waves; see Wave 5 below)

- pixel_format::detail::PFDTable (1600 bytes at 0x8e36c0 in .rodata, 40 rows of 0x28) is
  not in the repo. Wave 5 declares the row layout and a zero-filled table; see Wave 5. The table bytes are absent from recovered/native/assembly and symbols.
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

## Wave 5

Ledger after this wave: 581 rows (349 reconstructed, 70 partial, 162 skipped).

Class layouts added:

- CVertexStreams (vertex_streams.hpp): +0x04 m_attributeMask, +0x0e m_flags (bit 0 kFlagHomogeneous,
  bit 1 kFlagHasData), +0x10..+0x24 is the stream array, here a std::vector. SVertexStreamData is the
  setStream input; setStream copies +0x08 to the entry's +0x0a, and not the attribute at +0x08.
- CVertexAttributeMap (vertex_attribute_map.hpp): +0x00 word zeroed by the constructors and not copied
  by operator=; +0x04..+0x21 is a 30-byte slot-to-stream-index map, 0xff when unset.
- CCommonGLDriverBase: m_colorMask (+0x1ec), m_blendColor (+0x204) and m_clearColor (+0x208) moved here
  from CCommonGLDriver, because the getters are base methods. Blend src/dst are uint8 at +0x200/+0x201.
- CCommonGLDriver (ccommon_gl_driver.hpp): render-state setters over the GL function table; m_frontFaceFlip
  (+0x4a0), m_scissorFlag (+0x23c). flush(), presentHook() and stateHook() are virtual no-op defaults.
- CProgrammableGLDriver (programmable_gl_driver.hpp): queryMaxLightCount 8, getMaxUserClipPlanes 6,
  getShaderManager (+0xd8), getDriverType (0x78), commitShader records m_currentShader (+0xf4).
- IShader m_glProgram (+0x4c); CGLSLShader (gl_small.hpp) stores the glCreateProgram result there.
- glitch::core::vector4d and glitch::video::SColorf added to deps/glitch_core.hpp.
- IMaterialParameters<Owner, kTrackDirty> (material_parameters.hpp): dirty words at +0x0c/+0x10 only when
  kTrackDirty is true (CMaterial). Explicit instances: CGlobalMaterialParameterManager (false), CMaterial
  (true), CMaterialRenderer (false).
- Image loaders and writers: m_refCount on IImageLoader, m_unknown04 = 1 on the JPG loader and the JPG and PNG
  writers (texture_loaders.hpp, image_loaders.hpp, src/image_factories.cpp).
- pixel_format (pixel_format.hpp): SFormatEntry is the 0x28-byte PFDTable row. The table is zero-filled here,
  so the size helpers are reconstructed but partial until the table data is supplied.

Decisions:

- Thunks (4-byte `b #target`): ledgered reconstructed only when the target is already in the port and the
  body forwards to it (0x5a0e24 to updateStatesInternal). Otherwise skipped, with the target from the ARM
  listing. Every target was checked against recovered/native/assembly.
- Compiler clones (`.clone.N`) with a constant argument: only the generic body (0x7d46fc, setStream with a
  flag) is implemented. The clones are skipped and name the constant. Clones with identical bodies are
  also skipped, so no body is written twice.
- Ghidra shows some mip-level helpers as void (0x5edbcc, 0x5edbec, 0x5edc80). Their ARM tail-branches into
  computeSizeInBytes, so they return the size.
- ARM register shifts of 32 or more give 0. pixel_format uses lsrOrZero to match, since C++ shifts that
  large are undefined.
- CVertexAttributeMap::set ignores slots past the 30-byte map. The original writes without a check.
- deps/intrusive_ptr.hpp does not count references. setStream and the CVertexStreams destructor are partial
  because the original drops references there.
- Matrix overloads of IMaterialParameters (CMatrix4 is not in deps) are skipped, along with the thunks that
  forward to them.
- Destructor chains (D1/D0 with base destructor calls) are skipped as compiler generated. The port's
  implicit destructors cover them.

Still open:

- getStringsInternal (about 24 functions): the E_* types are int32 aliases, so the overloads collide, and the
  string table contents are not in the repo.
- IMaterialParameters E_SHADER_PARAMETER_VALUE_TYPE overloads (316-byte switch bodies), grabParameters and
  dropParameter (the template needs the matrix and value switches).
- CMaterial hash code, equals, init and reset: the CMaterial layout beyond the first members is not in the port.
- CGlobalMaterialParameterManager, CMaterialRendererManager, C2DDriver, CTextureManager, CGLSLShaderManager,
  CLight, CImage: most small bodies depend on classes that are only forward-declared.
- IVideoDriver: vtable slots +0x1f8 (binding), +0x0c (render buffer release) and the setMaterial overloads are
  not identified.
- Image writer encode paths and the libjpeg/libpng callbacks; IBuffer::map and copy-to-buffer paths.
