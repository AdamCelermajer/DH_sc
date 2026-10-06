# Source texture owner V1

`texture_owner_v1.hpp/.cpp` and `texture_driver_fields_v1.hpp/.cpp` are new
portable modules; no renderer, APK, or CMake changes are included.

## Source facts

* TextureManager ctor stores flags74 = 0x43. IVideoDriver ctor stores options88
  = 0x10. `createTextureFromImage` forces mipmapped when driver88 bit10 is set,
  independently of the image28 authored-mipmap byte.
* Manager bits20/10 select descriptor **usage+0c**, not layout+8. The exposed
  descriptor prefix implements the actual E_TEXTURE_LAYOUT=0 domain.
* ITexture ctor produces sampler dirty1ffd, anisotropy1, lod_bias0,
  mip_count = floorlog2(max dimension)+1 when mipmapped, max_lod=mip_count−1,
  min filter index3 for mipmapped / index1 otherwise, mag filter index1.
* updateParameters uses the exact original target/filter/wrap tables and
  preserves its duplicate wrap-T write for capability9c bit80. Format-property
  bit8 with flags3f bit2 emits the required diagnostic and changes min filter
  to index0 (GL_NEAREST). Anisotropy and max-LOD branches require actual driver
  fields and callbacks when reached.
* updateData5afff0 uploads only base mip when auto-generation bit3f:2 is set.
  It discards an initial GL error, checks after each upload, retains flag10
  on error, clears data dirtiness before the mip tail, diagnoses compressed
  auto-mips and skips generation. Uncompressed auto-mips require source
  virtual20 only when capability9c bit4 is set.
* Actual atlas member is
  `data/3d/textures/atlas_fx_particles_001.tga`, 131132 bytes,
  SHA256 `2256f485a94b00e8a8e5cbf93f0ff2038eff8d289ad73386683459ce01767b1b`.
  Its original PVR descriptor is 512×512, format27 (PVRTC4 RGBA), no authored
  mipmaps, base payload131072. The retained material URI is the producer of
  the requested atlas; do not choose this filename independently of material.

## Integration order

1. Retain the actual image bytes/metadata and SAME driver options owner.
2. `texture_image_descriptor_v1` gives the source layout0 descriptor prefix.
3. Deliver genuine Driver.createTexture/createTextureImpl. In particular,
   createTextureImpl selects native format from the actual driver mapping
   row at driver+4b0+format*20. This can change format27. Preserve its required
   conversion/diagnostic before constructing `TextureParameterOwnerV1`.
4. `construct(actual_selected_descriptor)`, then `set_data(generate_mips)`.
   `set_data` here is the bounded non-null data/invalidation scalar domain;
   caller retains the sole payload and source offsets/pending bitset.
5. `update(actual_driver_fields, parameter_services)` delivers the source
   sampler writes to the actually bound GL texture.
6. `upload(actual_payload/offsets/pending_bits/format_mapping, upload_services)`
   delivers source 2D image calls, unpack alignment, error checks, and the
   reached diagnostic or virtual20 generation continuation.
7. Draw only when `ready()` is true. There is no upload-completed setter.
   Context loss invalidates delivery receipts. A borrowed mutable sampler
   accessor invalidates the parameter receipt and requires another update.

`TextureUpload2dServicesV1::image` is an actual GL upload boundary. It receives
target, level, width, height, actual internal format, exact retained payload
range, compressed flag, actual pixel format and pixel type. `generate_mipmaps`
must deliver whole actual virtual20 (binding/active texture/filter preservation
plus generation), not a successful empty callback. Unsupported cube/3D/null
data and texture creation layouts remain explicit required continuations.

## Shader owner

`TextureDriverOptionsOwnerV1` owns constructor-backed flags88 and whole
setOption. Disabling a mask containing100 clears flags first, then requires
actual virtual1fc. Shader masks400/800/1000 read this SAME authority; GPU
capabilities9c and extension flags7ec are different fields.

`ShaderAdditionalConfigOwnerV1` owns config7c/length80. First initialization
reads exact `glsl.config` from the actual shader resource provider, misses
retain−1 and retry, found text changes `^` to LF. The actual pack member has
zero bytes: `config()` must be a PRESENT empty string after successful read.
Pass that borrowed pointer to existing `shader_source_plan`, alongside the
SAME options88. Constructor options are proven; final historical option call
history is not claimed by this bounded owner.

## Verification

`tools/probe_texture_parameters_v1.py` executes 193 original ARM cases
(192 target/filter/wrap combinations plus actual format27 compressed
downgrade) and captures exact calls/fields. `parameters-original-oracle.json`
contains the original ELF hash and source format27 row bytes.

`tests/texture_owner_v1_test.cpp` covers the native counterparts, exact actual
cache parser/decode, source ctor fields/mipmap decision, shader empty/miss
lifecycle and ordered upload receipts. The original/cache payload fixture
is prepared by `tools/prepare_blood_atlas_fixture_v1.py` with a pinned hash.
Standalone native runtime execution is requested from root; both ABI strict
syntax checks passed. GL recording callbacks are fixtures, not GPU execution.

Remaining production boundary: actual context-derived capabilities and
format mapping/conversion/create/bind lifetime must be composed by root's
real GL texture provider. This handoff does not replace that boundary with
fixed GL_LINEAR, a fabricated format, or a constant source_texture_ready.
