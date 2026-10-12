// Slice 08 reconstruction. Declarations and the unattached collaborators are in
// include/glitch-scene-collada-gui/recon/slice_08.hpp.
#include "glitch-scene-collada-gui/recon/slice_08.hpp"

#include <cstddef>

namespace glitch::collada::recon_08 {
namespace {

// Interpolation over one key element of N floats. Matches the original arithmetic:
// out[i] = (from[i] + t * (to[i] - from[i])) - sub[i], the last component returned.
template <typename K>
float interpolateKey(const K& sub, const K& from, const K& to, float t, K& out) {
    constexpr std::size_t n = sizeof(K::v) / sizeof(float);
    float result = 0.0f;
    for (std::size_t i = 0; i < n; ++i) {
        result = (from.v[i] + t * (to.v[i] - from.v[i])) - sub.v[i];
        out.v[i] = result;
    }
    return result;
}

// out[i] = to[i] - from[i], the last component returned.
template <typename K>
float differenceKey(const K& from, const K& to, K& out) {
    constexpr std::size_t n = sizeof(K::v) / sizeof(float);
    float result = 0.0f;
    for (std::size_t i = 0; i < n; ++i) {
        result = to.v[i] - from.v[i];
        out.v[i] = result;
    }
    return result;
}

}  // namespace

// ---------------------------------------------------------------------------
// float[4] mixin
// ---------------------------------------------------------------------------

// 0x612128
int getKeyBasedValue_612128(const SAnimationAccessor& accessor, int index, void* out) {
    const SAnimationOutput* output = getOutput(accessor, 0);
    *static_cast<Key4f*>(out) = reinterpret_cast<const Key4f*>(output->keys)[index];
    return outputHandle(output);
}

// 0x612174
float getKeyBasedValue_612174(const SAnimationAccessor& accessor, int from, int to, void* out) {
    const SAnimationOutput* output = getOutput(accessor, 0);
    const auto* keys = reinterpret_cast<const Key4f*>(output->keys);
    return differenceKey(keys[from], keys[to], *static_cast<Key4f*>(out));
}

// 0x6121c0
float getKeyBasedValueEx_6121c0(const SAnimationAccessor& accessor, int sub, int from, int to,
                                float t, void* out) {
    const SAnimationOutput* output = getOutput(accessor, 0);
    const auto* keys = reinterpret_cast<const Key4f*>(output->keys);
    return interpolateKey(keys[sub], keys[from], keys[to], t, *static_cast<Key4f*>(out));
}

// 0x612238
float getKeyBasedValue_612238(const SAnimationAccessor& accessor, int sub, int from, int to,
                              float t, void* out) {
    return getKeyBasedValueEx_6121c0(accessor, sub, from, to, t, out);
}

// ---------------------------------------------------------------------------
// vector3d<float>, CSceneNodePositionMixin<float>
// ---------------------------------------------------------------------------

// 0x61225c
int getKeyBasedValue_61225c(const SAnimationAccessor& accessor, int index, void* out) {
    const SAnimationOutput* output = getOutput(accessor, 0);
    *static_cast<Key3f*>(out) = reinterpret_cast<const Key3f*>(output->keys)[index];
    return outputHandle(output);
}

// 0x6122a4
float getKeyBasedValue_6122a4(const SAnimationAccessor& accessor, int from, int to, void* out) {
    const SAnimationOutput* output = getOutput(accessor, 0);
    const auto* keys = reinterpret_cast<const Key3f*>(output->keys);
    return differenceKey(keys[from], keys[to], *static_cast<Key3f*>(out));
}

// 0x6122f4
float getKeyBasedValueEx_6122f4(const SAnimationAccessor& accessor, int sub, int from, int to,
                                float t, void* out) {
    const SAnimationOutput* output = getOutput(accessor, 0);
    const auto* keys = reinterpret_cast<const Key3f*>(output->keys);
    return interpolateKey(keys[sub], keys[from], keys[to], t, *static_cast<Key3f*>(out));
}

// 0x612370
float getKeyBasedValue_612370(const SAnimationAccessor& accessor, int sub, int from, int to,
                              float t, void* out) {
    return getKeyBasedValueEx_6122f4(accessor, sub, from, to, t, out);
}

// ---------------------------------------------------------------------------
// vector3d<float>, CSceneNodeScaleMixin<float>. Same arithmetic as the position paths.
// ---------------------------------------------------------------------------

// 0x612394
int getKeyBasedValue_612394(const SAnimationAccessor& accessor, int index, void* out) {
    const SAnimationOutput* output = getOutput(accessor, 0);
    *static_cast<Key3f*>(out) = reinterpret_cast<const Key3f*>(output->keys)[index];
    return outputHandle(output);
}

// 0x6123dc
float getKeyBasedValue_6123dc(const SAnimationAccessor& accessor, int from, int to, void* out) {
    const SAnimationOutput* output = getOutput(accessor, 0);
    const auto* keys = reinterpret_cast<const Key3f*>(output->keys);
    return differenceKey(keys[from], keys[to], *static_cast<Key3f*>(out));
}

// 0x61242c
float getKeyBasedValueEx_61242c(const SAnimationAccessor& accessor, int sub, int from, int to,
                                float t, void* out) {
    const SAnimationOutput* output = getOutput(accessor, 0);
    const auto* keys = reinterpret_cast<const Key3f*>(output->keys);
    return interpolateKey(keys[sub], keys[from], keys[to], t, *static_cast<Key3f*>(out));
}

// 0x6124a8
float getKeyBasedValue_6124a8(const SAnimationAccessor& accessor, int sub, int from, int to,
                              float t, void* out) {
    return getKeyBasedValueEx_61242c(accessor, sub, from, to, t, out);
}

// ---------------------------------------------------------------------------
// float[3], CMixin<float,3,SMaterialSetParam<...>,-1,float>. Same arithmetic as the
// vector3d paths; the element is three floats.
// ---------------------------------------------------------------------------

// 0x6124cc
int getKeyBasedValue_6124cc(const SAnimationAccessor& accessor, int index, void* out) {
    const SAnimationOutput* output = getOutput(accessor, 0);
    *static_cast<Key3f*>(out) = reinterpret_cast<const Key3f*>(output->keys)[index];
    return outputHandle(output);
}

// 0x612514
float getKeyBasedValue_612514(const SAnimationAccessor& accessor, int from, int to, void* out) {
    const SAnimationOutput* output = getOutput(accessor, 0);
    const auto* keys = reinterpret_cast<const Key3f*>(output->keys);
    return differenceKey(keys[from], keys[to], *static_cast<Key3f*>(out));
}

// 0x612564
float getKeyBasedValueEx_612564(const SAnimationAccessor& accessor, int sub, int from, int to,
                                float t, void* out) {
    const SAnimationOutput* output = getOutput(accessor, 0);
    const auto* keys = reinterpret_cast<const Key3f*>(output->keys);
    return interpolateKey(keys[sub], keys[from], keys[to], t, *static_cast<Key3f*>(out));
}

// 0x6125e0
float getKeyBasedValue_6125e0(const SAnimationAccessor& accessor, int sub, int from, int to,
                              float t, void* out) {
    return getKeyBasedValueEx_612564(accessor, sub, from, to, t, out);
}

// ---------------------------------------------------------------------------
// CLightColorMixin<unsigned char>: three-byte keys.
// ---------------------------------------------------------------------------

// 0x612604
float applyKeyBasedValueEx_612604(const SAnimationAccessor& accessor, int index, void* applicator) {
    const SAnimationOutput* output = getOutput(accessor, 0);
    const KeyRgb value = reinterpret_cast<const KeyRgb*>(output->keys)[index];
    return applyValueEx_SLightColor(applicator, value.v);
}

// 0x612670
float applyKeyBasedValue_612670(const SAnimationAccessor& accessor, int index, void* applicator,
                                animation_track::CApplicatorInfo* info) {
    static_cast<void>(info);  // the original forwards only the first three arguments
    return applyKeyBasedValueEx_612604(accessor, index, applicator);
}

// 0x612684
int getKeyBasedValue_612684(const SAnimationAccessor& accessor, int index, void* out) {
    const SAnimationOutput* output = getOutput(accessor, 0);
    *static_cast<KeyRgb*>(out) = reinterpret_cast<const KeyRgb*>(output->keys)[index];
    return outputHandle(output);
}

// ---------------------------------------------------------------------------
// CColladaDatabase construct helpers
// ---------------------------------------------------------------------------

// 0x60f924
glitch::IReferenceCounted** constructSkin_60f924(glitch::IReferenceCounted** out,
                                                 const void* database, glitch::video::IVideoDriver* driver,
                                                 SController* controller, CRootSceneNode* root) {
    glitch::IReferenceCounted* skin = createSkinFromFactory(database, driver, controller, root);
    attachSkin(root, skin);
    *out = skin;
    if (skin != nullptr) {
        grabReference(skin);
        dropReference(skin);
    }
    return out;
}

// 0x60fa70
glitch::IReferenceCounted** constructController_60fa70(glitch::IReferenceCounted** out,
                                                       glitch::video::IVideoDriver* driver, int index,
                                                       CRootSceneNode* root,
                                                       CRootSceneNode* owner) {
    static_cast<void>(index);
    SController* controller = lookupControllerByOwner(driver, root);
    return constructControllerFromLookup(out, driver, index, controller, owner);
}

// 0x60fc70
glitch::IReferenceCounted** constructImage_60fc70(glitch::IReferenceCounted** out,
                                                  const void* root, const SImage* image) {
    if (image == nullptr) {
        *out = nullptr;
        return out;
    }
    glitch::IReferenceCounted* created = newImage(root, image);
    *out = created;
    if (created != nullptr) {
        grabReference(created);
    }
    return out;
}

// 0x60fcc0
glitch::IReferenceCounted** constructImage_60fcc0(glitch::IReferenceCounted** out,
                                                  const void* database, int index) {
    const SImage* image = lookupImage(database, index);
    return constructImage_60fc70(out, database, image);
}

}  // namespace glitch::collada::recon_08
