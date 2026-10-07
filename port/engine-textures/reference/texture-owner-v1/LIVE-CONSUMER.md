# Blood texture live consumer

Production files are frozen after the initial-bind addition. The renderer
must retain one `BloodTextureImageOwnerV1` per actual material image and one
`TextureBindingOwnerV1` / `TextureGlCacheFieldsV1` per actual GL driver context.
The grid borrows the same resource identity and source fields; do not copy
sampler flags, dirty bits, image bytes or generated GL names into a second
receiver.

The reached producer order is:

```cpp
Gles2TextureSamplerFeaturesV1 features;
texture_gles2_sampler_features_v1(actual_extensions, actual_float_query,
                                 features, error);
image.initialize(actual_material_image_bytes, features, same_manager,
                 &same_driver_options.flags88(), actual_format_diagnostic,
                 error);

// Retain this reference before delivery. Later fields() access invalidates
// the parameter receipt; do not reacquire it during the mipmap callback.
auto& fields = image.parameters().fields();
TextureBindingReceiverV1 receiver{same_resource_identity,
                                 &fields.flags3f, &fields.dirty40,
                                 &actual_gl_name54};
TextureBindingDriverBorrowV1 driver{&actual_texture_units4c,
                                   &same_gl_cache.active_unit268};
binding.initial_bind_impl_prefix(receiver, fields, actual_gl_name54,
                                 driver, actual_initial_gl_services, error);
image.parameters().update(image.driver(), actual_parameter_services, error);
TextureUpload2dBorrowV1 data;
image.upload_borrow(&same_gl_cache.unpack_alignment26c, data, error);

// actual_upload_services.generate_mipmaps calls this reached whole source
// virtual20 successor. Its bind service calls SAME binding.set_texture with
// receiver, unit, kind, driver and actual active/bind/update services.
actual_upload_services.generate_mipmaps = [&](std::string& e) {
    return image.parameters().generate_mipmaps(
        same_resource_identity,
        {&actual_texture_units4c, &same_gl_cache.active_unit268},
        actual_mipmap_services, e);
};
image.parameters().upload(data, actual_upload_services, error);
// Check every returned bool above. Submit the material only on ready().
```

`actual_texture_units4c` is produced by actual `glGetIntegerv(0x8872)`
(`GL_MAX_TEXTURE_IMAGE_UNITS`) and source unsigned clamp to8, then stored at
source5b3c18. Reject unusable zero before the bounded last-unit kernel.
Sampler cap4 is SET unconditionally at source5b3c1c; ctor9c=40 is not the
initialized driver value. Only the exact consumed capabilities/extension
subset is represented by the feature projection; it does not masquerade as
a whole COpenGLES2Driver.

Actual source extension names/indices, original lookup tested:

| Property | Actual source extension |
|---|---|
| Keep compressed format27 | GL_IMG_texture_compression_pvrtc,414 |
| RGBA14 sized internal8058 | GL_OES_rgb8_rgba8,392 or GL_ARM_rgba8,437 |
| Otherwise RGBA14 internal/external | 1908 /1908, pixel type1401 |
| Anisotropy cap20000/query84ff | GL_EXT_texture_filter_anisotropic,197 |
| Duplicate wrap-T capability80 | GL_EXT_texture3D,87 or GL_OES_texture_3D,396 |
| MaxLOD extension flag80000 | GL_APPLE_texture_max_level,435 |

`initExtensions6dd734` only commits space-terminated tokens. The feature
projection deliberately preserves the original last-token discard. Pass
the actual extension string unchanged; adding a trailing space changes the
source result.

Initial GL name54 and special byte58 are source-zero from CTextureBase C1/C2
6dde98/6dded8. Fresh bindImpl5b5610 clears error10 before actual glGenTextures;
name0 sets10 and cannot publish online8. A nonzero name selects the actual
last unit unless the current grid already contains the same receiver;
directly publishes the borrowed grid pointer without incrementing source
counter84, calls glBindTexture, then publishes flags3f bit8 before
CTexture.update(true). Source format27 compressed-auto and constructor
format14 auto branches are the supported domain of the prefix method.

The current initial method intentionally stops before update(true). It does
not claim the whole bindImpl error/unbind/data-release tail. Root delivers
the actual same-owner update/upload above. If an upload sets error10,
original5b5864 calls unbindImpl then re-sets error10; this continuation must
remain explicit if it is not composed. A reached missing GL/mipmap/diagnostic
provider must not be replaced with success.

Destroy/reload: driver grid is non-owning. Original unbindImpl5b28dc clears all
slots referencing this receiver via setTexture(unit,NULL,kind), deletes its
GL name, clears name54/online8/error10, clears dirty2 and marks parameter/data
dirty; auto-mip data marks base pending bit again. Clear the grid before
destroying retained image/field storage. Actual context loss invalidates
parameter/upload receipts; a new actual GL context needs a fresh cache/grid
and driver-feature query. Other original map/readback/unbind continuations
are not silently supplied by this bounded upload owner.

Native test sources for root's standalone runner:

* `tests/blood_texture_image_v1_test.cpp`, `blood_texture_image_v1.cpp`,
  `texture_owner_v1.cpp`, `texture_driver_fields_v1.cpp`, `texture_mipmap_v1.cpp`,
  `textures.cpp`, `pvrtc.cpp`.
* `tests/texture_binding_owner_v1_test.cpp`, `texture_binding_owner_v1.cpp`,
  `texture_mipmap_v1.cpp`.

Original receipts: 193 sampler cases,96 mipmap cases,36 binding cases,
12 initial-bind cases, exact format27→14 nonzero payload conversion and both
source mip-offset arrays. These are original ARM executions with explicit
GL receiver boundaries, not GPU execution. Actual full atlas parsing and
decoding are included in native image fixtures; live GLES acceptance belongs
to root's renderer checkpoint.
