# Retained live effect texture adapter V5

New `renderer_effect_texture_v5.inc` replaces the old method named `effect_texture_v4`; callers and retained `EffectGpuTextureV4` type do not change. It is intentionally named V5 while retaining the old callable name to minimize parent integration edits.

Root integration in `renderer_effect_scene_v4.inc`:

1. Remove old `effect_texture_v4` body, old forward declaration, and unused `effect_driver_unpack_alignment_v4` global.
2. Include `renderer_effect_texture_v5.inc` immediately after existing texture/resource maps, shader/options globals and before `clear_combat_fx_gpu`.
3. Add engine header includes `texture_binding_owner_v1.hpp`, `texture_mipmap_v1.hpp`, `texture_unbind_v1.hpp` before the implementation-class includes. Add `texture_unbind_v1.cpp` to the engine target; the existing binding/mipmap/image sources remain required.
4. Replace the raw texture-delete loop in `clear_combat_fx_gpu` with:

```cpp
for(auto& pair:effect_gpu_textures_v4){
 std::string error;
 if(!release_effect_texture_v5(*pair.second,discard_context,false,error))
  throw std::runtime_error(error);
}
```

The loop must run for both context-live and context-discard cases, before clearing the maps. GPU resources may still hold shared image owners while buffers/programs are released; this is intentional. Lost-context path clears the nonowning receiver grid and invalidates the image receipt without issuing GL calls into a replacement context.

The dedicated effect driver uses actual `GL_MAX_TEXTURE_IMAGE_UNITS` (source enum8872, unsigned clamp8), actual active texture and unpack alignment, source grid/counter and retained image bytes. The fresh bind prefix clears error10 before actual GL generation, directly publishes the receiver without incrementing counter84, binds actual target/name, then publishes online8 before parameter/updateData. Virtual20 is the whole source mipmap helper over this SAME grid/receiver/cache; it is not a raw glGenerateMipmap substitute.

`EffectTextureStateGuardV5` saves all actual 2D bindings affected by this source driver and restores generic renderer state. Before restoring external GLuints it clears source grid entries through the original NULL branch. Generic objects are never claimed as source CTexture receivers; after the boundary the source grid is empty and its active/alignment cache reflects actual GL. The existing effect draw guard may continue temporarily binding the already-ready texture for sampling; it neither changes texture parameters nor inserts canonical grid entries. This is an explicit modern renderer boundary rather than a full generic source-driver adoption.

Whole reached 2D unbind5b28dc clears matching grid entries before deletion, clears name/online/error flags, applies source dirty mask and reconstructs actual pending bits (base bit for auto mips, all source pending words for explicit mips). A source updateData error10 runs this cleanup and then restores error10, as whole bindImpl does. Missing-provider/C++ failure also releases the allocated lifetime and preserves the original exception. Unsupported formats/non-2D branches remain rejected.

Validation: 16 original ARM whole unbind cases, 4 original bindImpl→real unbind error tails; strict ARM64 and x86_64 Android compilation of the new adapter in a renderer-shaped class. Native fixture `tests/texture_unbind_v1_test.cpp` checks32 ordered prefix/poststate assertions; source list: test + `texture_unbind_v1.cpp` + `texture_binding_owner_v1.cpp`. Compile-only adapter test is `tests/renderer_effect_texture_v5_compile.cpp` with include paths engine-textures and native app cpp. Source GL callbacks in oracle/native tests are fixtures; live GLES draw acceptance remains parent verification.
