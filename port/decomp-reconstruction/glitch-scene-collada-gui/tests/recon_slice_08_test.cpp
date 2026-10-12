// Slice 08 test. The accessor, applicator and reference-count collaborators are not
// reconstructed yet, so this file supplies minimal definitions for them.
#include "glitch-scene-collada-gui/recon/slice_08.hpp"

#include <cstdio>

namespace glitch::collada {
struct SAnimationAccessor {
    recon_08::SAnimationOutput* output;
};
}  // namespace glitch::collada

namespace glitch::collada::recon_08 {
SAnimationOutput* getOutput(const SAnimationAccessor& accessor, int) { return accessor.output; }
float applyValueEx_SLightColor(void* applicator, const std::uint8_t* value) {
    *static_cast<float*>(applicator) = static_cast<float>(value[0]);
    return 42.0f;
}
void grabReference(glitch::IReferenceCounted*) {}
void dropReference(glitch::IReferenceCounted*) {}
void attachSkin(CRootSceneNode*, glitch::IReferenceCounted*) {}
glitch::IReferenceCounted* createSkinFromFactory(const void*, glitch::video::IVideoDriver*,
                                                 SController*, CRootSceneNode*) {
    return reinterpret_cast<glitch::IReferenceCounted*>(static_cast<std::intptr_t>(0x10));
}
SController* lookupControllerByOwner(const glitch::video::IVideoDriver*, const CRootSceneNode*) {
    return nullptr;
}
glitch::IReferenceCounted** constructControllerFromLookup(glitch::IReferenceCounted** out,
                                                          glitch::video::IVideoDriver*, int,
                                                          SController*, CRootSceneNode*) {
    return out;
}
SImage* lookupImage(const void*, int) { return nullptr; }
glitch::IReferenceCounted* newImage(const void*, const SImage*) { return nullptr; }
}  // namespace glitch::collada::recon_08

using namespace glitch::collada;
using namespace glitch::collada::recon_08;

// Placeholder applicator tag; the real applicator layouts are not mapped yet.
struct ProbeApplicator {};

static int failures = 0;
#define CHECK(c)                                                              \
    do {                                                                      \
        if (!(c)) {                                                           \
            std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c);           \
            ++failures;                                                       \
        }                                                                     \
    } while (0)

int main() {
    // getInstance: the same function-local instance on every call.
    CHECK(getInstance_CVirtualEx<ProbeApplicator>() == getInstance_CVirtualEx<ProbeApplicator>());

    // Position keys (vector3d, stride 12): element 1 is {4,5,6}.
    Key3f positionKeys[2] = {{{1.f, 2.f, 3.f}}, {{4.f, 5.f, 6.f}}};
    SAnimationOutput output{0, reinterpret_cast<std::uint8_t*>(positionKeys)};
    SAnimationAccessor accessor{&output};

    Key3f copied{};
    int handle = getKeyBasedValue_61225c(accessor, 1, &copied);
    CHECK(copied.v[0] == 4.f && copied.v[1] == 5.f && copied.v[2] == 6.f);
    CHECK(handle == static_cast<int>(reinterpret_cast<std::intptr_t>(&output)));

    // Difference to[1] - from[0] = {3,3,3}, last component 3.
    Key3f diff{};
    float last = getKeyBasedValue_6122a4(accessor, 0, 1, &diff);
    CHECK(diff.v[0] == 3.f && diff.v[2] == 3.f && last == 3.f);

    // Interpolation: sub = key 0, from = key 0, to = key 1, t = 0.5.
    Key3f lerped{};
    float lerpLast = getKeyBasedValueEx_6122f4(accessor, 0, 0, 1, 0.5f, &lerped);
    CHECK(lerped.v[0] == 2.5f - 1.f && lerpLast == lerped.v[2]);

    // float[4] keys (stride 16) through the forwarder.
    Key4f quat[2] = {{{0.f, 0.f, 0.f, 0.f}}, {{2.f, 4.f, 6.f, 8.f}}};
    SAnimationOutput quatOutput{0, reinterpret_cast<std::uint8_t*>(quat)};
    SAnimationAccessor quatAccessor{&quatOutput};
    Key4f lerp4{};
    float forwarded = getKeyBasedValue_612238(quatAccessor, 0, 0, 1, 0.5f, &lerp4);
    CHECK(lerp4.v[0] == 1.f && lerp4.v[3] == 4.f && forwarded == 4.f);

    // CLightColor keys (three bytes each).
    KeyRgb rgb[2] = {{{1, 2, 3}}, {{7, 8, 9}}};
    SAnimationOutput rgbOutput{0, reinterpret_cast<std::uint8_t*>(rgb)};
    SAnimationAccessor rgbAccessor{&rgbOutput};
    KeyRgb rgbCopy{};
    getKeyBasedValue_612684(rgbAccessor, 1, &rgbCopy);
    CHECK(rgbCopy.v[0] == 7 && rgbCopy.v[2] == 9);
    float target = 0.0f;
    float applied = applyKeyBasedValue_612670(rgbAccessor, 1, &target, nullptr);
    CHECK(applied == 42.0f && target == 7.0f);

    // constructImage with no image stores a null result.
    glitch::IReferenceCounted* slot = reinterpret_cast<glitch::IReferenceCounted*>(static_cast<std::intptr_t>(0x99));
    glitch::IReferenceCounted** outImage = &slot;
    constructImage_60fcc0(outImage, nullptr, 0);
    CHECK(*outImage == nullptr);

    // constructSkin stores the factory result.
    glitch::IReferenceCounted* skinSlot = nullptr;
    glitch::IReferenceCounted** outSkin = &skinSlot;
    constructSkin_60f924(outSkin, nullptr, nullptr, nullptr, nullptr);
    CHECK(skinSlot == reinterpret_cast<glitch::IReferenceCounted*>(static_cast<std::intptr_t>(0x10)));

    std::printf("%s\n", failures ? "recon_slice_08: FAILED" : "recon_slice_08: ok");
    return failures != 0;
}
