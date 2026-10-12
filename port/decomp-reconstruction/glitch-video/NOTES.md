Ledger: 812 rows (369 reconstructed, 91 partial, 352 skipped). Queue total 1345, 533 still open.
Wave 7 added 231 rows (see the Wave 7 section at the end).

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
  (IBuffer::map for E_BUFFER_MAP_ACCESS is done in wave 7; the read overload is still open.)

## Wave 7

Ledger after this wave: 812 rows (369 reconstructed, 91 partial, 352 skipped). Wave 7 added 231 rows: 20 reconstructed,
21 partial, 190 skipped. Of the skipped rows, 50 are queue entries outside glitch::video (see Decisions).

Source: the Hex-Rays export (recovered/native/hexrays-all, commit 9476235b) is the primary pseudocode source. Bodies
written before the switch to Hex-Rays were checked against the same addresses there. The ARM listings were used
for thunk and tail-call questions only.

Class layouts added or confirmed (offsets from the original, names from the call pattern):

- IBuffer (buffer.hpp): +0x04 m_refCount, +0x08 m_data, +0x0c m_size, +0x10 m_type, +0x11 m_usage, +0x12 m_flags12
  (bit 0 data not owned, bit 1 has data, bit 5 unmap callback), +0x13 m_mapCount13 (low 5 bits count, bits 5-6 access).
  Virtual slots used by map(): index 5 (+0x14) mapImpl, index 6 (+0x18) unmap callback, index 7 (+0x1c) getMappedData.
  The indices come from the call pattern, not from a vtable dump, so the slot names are inferred.
- CCommonGLDriverBase::CBufferBase: derives from IBuffer; +0x14 driver, +0x18 word not identified, +0x1c mapped pointer.
- CCommonGLDriverBase::CTextureBase: +0x54 word and +0x58 byte zeroed by the constructor (names not identified).
- CCommonGLDriverBase::CRenderTargetBase: +0x4c and +0x54 attachment handles, +0x58 colour format byte, +0x59 target
  count byte. getTargetCount reads them by attachment type (0 count, 1 and 2 handle set, 3 handles equal).
- CCommonGLDriverBase::SCommonShadowState: twelve words; word +0x18 starts at 4, the others at 0.
- CNullDriver::CTexture: constructor arguments are not stored (ITexture base not ported).
- CGLSLShaderCode: +0x2c driver pointer; +0x30 word and +0x34 byte not identified.
- CGLSLShader: +0x44 and +0x48 words and +0x50 byte not identified; +0x4c is IShader::m_glProgram.
- CMaterialRendererManager (material_renderer_manager.hpp, new): +0x90 flag word; getTechnique and getParameterDef
  take an STemporaryID (one word) and return it when the flag is set and the id is non-zero.
- ITextureDataLoading (texture_loaders.hpp): m_file +0x04, m_source +0x08, m_desc +0x0c, m_info +0x10,
  m_sourceStep +0x1c, flags +0x20 (read failed), +0x21 (set by the constructors), +0x22 (pitch known).
- STextureLoadInfo: format +0x04, layoutType +0x08, width +0x10, height +0x14, depth +0x18.
  STextureHeaderBlock: width +0x24, height +0x28. STextureDataInfo: header pointer +0x00, swapBytes +0x0d.
- SDDSSurfaceFormatHeader: 0x7c bytes; pixelFormatSize +0x48 (0x20 expected).
- CPrimitiveStream: m_count +0x08, m_primitiveType +0x16 (uint16).
- ITexture: m_refCount +0x04, m_width +0x20, m_data +0x2c, m_levelSizes +0x30 (per-mip sizes, then the dirty bitmap),
  m_flags38 +0x38, m_mipCount3e +0x3e, m_flags3f +0x3f, m_dirtyFlags40 +0x40, m_lockCount42 +0x42, m_lockFlags43 +0x43.
- CTextureManager: m_placeHolders +0x48, m_textures vector at +0x68.

Decisions:

- Compiler clones, deleting destructors and 4-byte thunks: none are left in the queue. Earlier clones and destructor
  chains keep their wave-5 policy.
- Destructor teardowns (D1/D0) that have no port body are ledgered skipped, not partial. The port's implicit
  destructors cover them; the notes say where a member is not freed (ITexture m_data).
- Partial constructors: when the base constructor is not ported (ITexture, IShader, IShaderCode base, IVideoDriver),
  the body keeps only the fields that follow the base call. Such rows are partial, not reconstructed.
- Out-of-video queue entries: 50 functions in glitch-video/queue.csv belong to glitch::collada (22), glitch::res (10),
  glitch::scene (9), glitch::gui (6) and glitch::ps (3). They are outside glitch::video. They are ledgered as skipped with
  source "(outside glitch-video)" so the queue is consistent. They should be reassigned or confirmed by the coordinator.
- Hex-Rays header sizes do not match queue.csv body_bytes (for example 5b93ec: header 496, queue 64). The queue is used
  for addresses and symbols, and the bodies follow the pseudocode.
- IBuffer::map has one overload in the port. E_BUFFER_READ_MAP_ACCESS is an int32 alias here, so 0x5a1adc would
  collide with it and is not ported.
- Data tables are still absent from the repo: the PFD table (pixel_format), ValueTypeSize (SVertexAttributeTypeInspection)
  and the string tables. Anything that needs them stays open.
- libpng and libjpeg callbacks are still skipped (wave 5).

Still open (533 queue entries, all inside glitch::video):

- CCommonGLDriver<CProgrammableGLDriver<CGLSLShaderHandler>, CProgrammableGLFunctionPointerSet> and its nested
  classes, about 98 entries. Bodies use the GL function table, driver state and vtable slots that are not identified.
- IMaterialParameters templates for CMaterial, CMaterialRenderer and CGlobalMaterialParameterManager, about 63 entries.
  The value switches and the ISharedMemoryBlockHeader layout (count +0x0e, base +0x20) are not in the port.
- CMaterialRendererManager (about 52 entries, minus the two getters written here). It has no class in the port beyond
  the lookups.
- CVertexStreams and CMaterialVertexAttributeMap, about 46 entries. setupStreams needs the GL attribute table.
- IVideoDriver, about 40 entries. Includes the constructors (956 bytes) and the draw dispatch, which needs
  CDriverBinding.
- Shader classes (IShader, IShaderManager, CGLSLShader link path, CGLSLShaderManager, CShaderCodeManager), about 38
  entries. Most use SIDedCollection; linkProgram (0x6de9f8) is 1432 bytes.
- CMaterial, about 28 entries. The heap block, hash words and parameter blocks are not in the port.
- CTextureManager, about 27 entries. Its texture collection is an SIDedCollection, which is not in the port.
- CImage, about 23 entries. Pixel access and copy paths need the PFD table.
- SColor and pixel_format, about 21 entries (needs the PFD table; SColor interpolation not rechecked against Hex-Rays).
- C2DDriver (15), CNullDriver (9), ITexture (7), IBuffer::reset (2473 bytes) and IBuffer::map for E_BUFFER_READ_MAP_ACCESS.
- Image loaders for BMP, ATC, DDS, PVR, JPG, PNG and TGA: decode paths are not reconstructed in this wave.
- Smaller groups: CLight, CBatchDriver, COpenGLES2Driver constructors, CGenericBaker, the anonymous-namespace blit and
  preprocess helpers, IMaterialTechniqueMapsReader, the clustering helpers, and the SShaderParameterDef and
  SShaderVertexAttributeDef serializers.

## glitch-video-2 slice (78 rows)

- Written: buffer map (registerBufferMap, unregisterBufferMap, clearBufferMap) and createBuffer in ccommon_gl_driver.cpp; std::map member at +0x800 with BufferInfo (field names not identified).
- beginScene/endScene written as partial: IVideoDriver base call not reachable from CCommonGLDriverBase.
- Remaining open rows (CBatchDriver, detail soft-draw helpers, genericDriverInit, particle bakers, blits, loadImage BMP) are ledgered partial, not written this pass. Each needs its class layout or template instance first.
- Existing partial bodies for setBlend*/setStencil*/setCullFace/setDepthFunc/setFrontFace left unchanged; GL enum tables are still missing.

## glitch-video-1 slice (worker findings)

- SPackedRGBtoLuminanceAlphaConverter layout from PFDTable row (0x5edca4): weights 0.3/0.59/0.11 at +0x00,
  channel masks +0x0c, channel shifts +0x18, alpha shift +0x1b, alpha mask +0x1c, alpha scale +0x20, table
  alpha max +0x24 (0 when row flags bit 0 is set). Row bytes read through SFormatEntry::unknown18.
- ITextureDataLoading::getFilePitch (0x6077c4) returns the computePitch value in the original; the port is void.
- IMaterialParameters accessors: the matrix overloads read the owner block through +0x04 (CMaterial) or self
  (CMaterialRenderer), count at +0x0e, entries at +0x20, 16-byte entries with type byte +0x06 (11 = matrix).
  Dirty words are written at +0x0c and +0x10 by the matrix setters. Helpers getMatrixParameter and
  setMatrixParameter are not recovered.
- CImageLoaderDDS::loadTextureHeader needs a real STextureDesc layout (+0 type, +4 format, +16 width, +20 height,
  +24 depth, +28 flag); FourCC map: DXT1 (0x31545844) -> 18, DXT2/DXT3 (0x32/0x33545844) -> 19, DXT4/DXT5 (0x34/0x35545844) -> 20.

## glitch-video-3 (slice: IVideoDriver, IBuffer, CGLSLShader, CGLSLShaderManager, texture loaders)

- IBuffer m_flags12 bit 0 (0x01) is kFlagOwnsData: the data is a private copy that reset frees.
  The older note above says "data not owned"; the copy() and reset() bodies and the existing
  test (flag 1 keeps the data) show the opposite. Bit 2 (0x04) is kFlagDirty (read-back pending).
- IBuffer::copy returns bool and sets kFlagOwnsData. IBuffer::reset(size, data, keepOwned) is
  reconstructed from the control flow of 0x5a1cb4.
- CGLSLShader +0x44 and +0x48 hold the two shader code references (first and second argument of
  the 6df1f0 constructor). +0x4c is m_glProgram. releaseShaderCode and releaseDriverSpecificResources
  are reconstructed; GLFunctionTable gained deleteProgram.
- 64 of 78 rows are skipped: they need core math types (CMatrix4, triangle3d, aabbox3d, vector3d),
  CMaterial or CMaterialRenderer layouts, CGLSLShaderManager, or viewport and batch state that the
  port does not have. Each skipped row names its missing dependency in the ledger notes.

## glitch-video-6 slice (worker notes)

- SColor::getInterpolated reconstructed (src/s_color.cpp). Blend is t*this + (1-t)*other with t clamped to [0,1]; channel bytes are read with shifts, not casts.
- CMaterial: m_renderer +0x04, m_dirtyParameters +0x0c, m_dirtyHashes +0x10, m_hashCodes +0x18 (array of one word per technique). Renderer technique table at renderer+0x18, 12-byte entries, parameter count byte at entry +4. getTechnique, updateHashCode and compare are declared, not defined.
- IMaterialTechniqueMapsReader: m_manager +0x04, map ids +0x08/+0x0c (-1), flags +0x12/+0x13, error +0x16. The vtable and the virtual hooks (+0x1c, +0x28) are not modelled.
- Blocked on missing infrastructure (see the ledger part notes): pixel_format PFD table, SBlitJob, CMaterial heap blocks, CGlobalMaterialParameterManager and SIDedCollection, IVideoDriver base constructors, CBuffer and IBuffer relationship, CProgrammableGLDriver template.

## glitch-video-8 (slice of 78 rows, CVertexStreams / SVertexStream / pixel_format / programmable driver)

- New: render_state.hpp/.cpp (SRenderState, partial), tests/vertex_streams_build_test.cpp.
- vertex_streams.hpp: CVertexStreams ctor, allocate overloads, clone, setStreams, setupStreams, SVertexStream::SMapBuffer<T>
  (stores the SVertexStream pointer at +0x00 and the mapped pointer at +0x04, as the original does).
- New fields m_field08 (+0x08), m_count0c (+0x0c), m_streamCount0d (+0x0d) are named by offset only.
- Blockers for the remaining skipped rows: ValueTypeSize and Defaults tables of SVertexAttributeTypeInspection (not in repo);
  PFD table (pixel_format); GL function table lacks glCreateShader, glShaderSource, compile and info-log calls and
  glGetIntegerv/glHint; CProgrammableGLDriver layout past +0x284; CMaterialTechniqueMapLoadState not in port;
  IVideoDriver texture manager slot; IBuffer unmap callback (vtable +0x18) not modelled.
- All new vertex-stream bodies stay partial: buffer reference counts are not modelled in deps/intrusive_ptr.hpp.

## Slice glitch-video-4 (CLight, IShader, shader code, material renderer manager, SColorHSL)

- CLight (light.hpp): +0x00 count, +0x04..+0x0c words, +0x10..+0x34 ten floats (default 1.0), +0x40 FLT_MAX,
  +0x48 40.0, +0x4c 2.0, +0x50 STransformationSource slot, +0x54 flag, +0x58 halfword, +0x5a flag. Size 0x5c.
- IShader (shader_attributes.hpp): stage parameter records at +0x28 and +0x30 (pointer, halfword, halfword count);
  +0x24 vertex attribute defs; +0x38 vertex attribute mask; +0x3c count byte; +0x40 shader id halfword.
  Stage 2 has no record in the original reads; getParameterID returns 0xffff for it.
- CMaterialRendererManager: +0x28 driver, +0x90 open creation-state pointer (getTechnique tests it). The
  technique, render-pass and parameter tables are SIDedCollection and SSharedString trees; their bodies are
  ledgered skipped with the note blocked.
- ICodeShaderManager: +0x7c additional config text, +0x80 its length (-1 until loaded).
- Converter SAlphaIndexedPackedConverter (alpha_indexed_converter.hpp): masks at +0x00..+0x08 and +0x1c..+0x30,
  shifts in two groups, LUT pointer +0x3c, LUT mask +0x40; the constructor needs PFDTable (blocked).

## PFDTable data (integrator decision, 2026-10-12)

- The table bytes are in the original ELF: `ida.zip` contains `libDungeonHunter2.so` (15.9 MB). The
  earlier note that the bytes were absent was wrong; they were only missing from the repo's listings.
- `PFDTable` is at file vaddr 0x8e36c0, 1600 bytes, 40 rows of 0x28. The generated source is
  `src/pixel_format_table.cpp`. Fields follow the layout in `include/glitch-video/pixel_format.hpp`.
- Decision: the row layout is taken from the header as written. Bytes +0x18..+0x23 are kept as
  `unknown18` and are not read by the helpers.
- The pixel_format rows that were blocked on the table (getPackedType, getFormat, computePitch,
  computeSizeInBytes, computeMipmapSizeInBytes, swapBytes, convert, converters) can now be rerun.
