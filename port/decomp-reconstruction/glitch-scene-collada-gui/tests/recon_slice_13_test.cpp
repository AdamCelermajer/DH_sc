// Slice 13 checks: interpolation, weighted blends, retrieve copies and parameter forwarding.
// The accessor, parameter setters and target are stubbed here.
#include <cstdint>
#include <cstdio>

#include "glitch-scene-collada-gui/recon/slice_13.hpp"

using namespace glitch::collada::recon_13;

namespace {

float gFloatValues[4] = {1.0f, 2.0f, 3.0f, 4.0f};
std::uint8_t gByteValues[6] = {10, 20, 30, 40, 50, 60};
std::uint32_t gKeyWords[8] = {0, 0, 0, 0, 0x11111111u, 0x22222222u, 0x33333333u, 0x44444444u};
SAnimationOutput gOutputs[1];
int gSetterCalls = 0;
std::uint16_t gLastParameterId = 0;
std::uint8_t gLastColor[4] = {0, 0, 0, 0};
float gLastVec2[2] = {0.0f, 0.0f};
float gLastFloat = 0.0f;

struct StubTarget : IAnimationTarget {
    std::uint32_t received[4] = {0, 0, 0, 0};
    bool called = false;
    int applyValue(const void* value) override {
        const auto* words = static_cast<const std::uint32_t*>(value);
        for (int i = 0; i < 4; ++i) {
            received[i] = words[i];
        }
        called = true;
        return 7;
    }
};

struct StubNode : ISceneNodeTransform {
    std::uint32_t position[3] = {1, 2, 3};
    std::uint32_t scale[3] = {4, 5, 6};
    const std::uint32_t* positionWords() override { return position; }
    const std::uint32_t* scaleWords() override { return scale; }
};

bool nearly(float a, float b) {
    float d = a - b;
    return d < 1e-4f && d > -1e-4f;
}

}  // namespace

// Stubs for the unattached externals used by the tested bodies.
namespace glitch::collada::recon_13 {

SAnimationOutput* getOutput(SAnimationAccessor*, int) {
    return &gOutputs[0];
}

int setParameterCvtFloat(void*, std::uint16_t id, int, const void* value) {
    ++gSetterCalls;
    gLastParameterId = id;
    gLastFloat = *static_cast<const float*>(value);
    return 0;
}

int setParameterCvtColor(void*, std::uint16_t id, int, const std::uint8_t* value) {
    ++gSetterCalls;
    gLastParameterId = id;
    for (int i = 0; i < 4; ++i) {
        gLastColor[i] = value[i];
    }
    return 0;
}

int setParameterCvtVec2(void*, std::uint16_t id, int, const float* value) {
    ++gSetterCalls;
    gLastParameterId = id;
    gLastVec2[0] = value[0];
    gLastVec2[1] = value[1];
    return 0;
}

}  // namespace glitch::collada::recon_13

int main() {
    gOutputs[0].values = gFloatValues;

    // float1 interpolation: key 1 at t = 0.25 gives 2 * 0.75 + 3 * 0.25.
    float out = 0.0f;
    float result = float1_interp_getKeyBasedValueEx(nullptr, 1, 0, 0.25f, &out);
    if (!nearly(result, 2.25f) || !nearly(out, 2.25f)) {
        std::printf("float1 interp failed: %f\n", static_cast<double>(result));
        return 1;
    }

    // float1 weighted sum over two keys, and the single-key copy.
    float weights[2] = {0.5f, 0.25f};
    float sum = 0.0f;
    float1_getAddedValue(nullptr, gFloatValues, weights, 2, &sum);
    if (!nearly(sum, 1.0f * 0.5f + 2.0f * 0.25f)) {
        std::printf("float1 added failed: %f\n", static_cast<double>(sum));
        return 1;
    }
    float1_getAddedValue(nullptr, gFloatValues, weights, 1, &sum);
    if (!nearly(sum, 1.0f)) {
        std::printf("float1 single key failed\n");
        return 1;
    }

    // float1 apply forwards the interpolated value and the binding id at +0x08.
    SParameterBinding binding{};
    binding.parameterId = 42;
    int status = float1_applyKeyBasedValueEx(nullptr, 0, 0, 0.5f, nullptr, &binding);
    if (status != 0 || gLastParameterId != 42 || !nearly(gLastFloat, 1.5f)) {
        std::printf("float1 apply failed\n");
        return 1;
    }

    // Position retrieve copies three words from the position source.
    StubNode node;
    std::uint32_t copied[3] = {0, 0, 0};
    const std::uint32_t* source = posZChar_retrieveValue(nullptr, &node, copied);
    if (source != node.position || copied[0] != 1 || copied[1] != 2 || copied[2] != 3) {
        std::printf("position retrieve failed\n");
        return 1;
    }
    scaleXShort_retrieveValue(nullptr, &node, copied);
    if (copied[0] != 4 || copied[2] != 6) {
        std::printf("scale retrieve failed\n");
        return 1;
    }

    // Quaternion float track: the key record (four words) goes to the target.
    gOutputs[0].values = gKeyWords;
    StubTarget target;
    int slot = quatFloat_applyKeyBasedValueEx(nullptr, 1, &target);
    if (slot != 7 || !target.called || target.received[0] != 0x11111111u || target.received[3] != 0x44444444u) {
        std::printf("quaternion key copy failed\n");
        return 1;
    }
    // Forwarder reaches the same body.
    StubTarget target2;
    quatFloat_applyKeyBasedValue(nullptr, nullptr, 0, &target2);
    gOutputs[0].values = gFloatValues;

    // Byte colour interpolation over keys 0 and 1 at t = 0.5: (10 + 40) / 2, (20 + 50) / 2, (30 + 60) / 2.
    gOutputs[0].values = gByteValues;
    std::uint8_t rgb[3] = {0, 0, 0};
    colorInterpGetKeyBasedValueEx(nullptr, 0, 0, 0.5f, rgb);
    if (rgb[0] != 25 || rgb[1] != 35 || rgb[2] != 45) {
        std::printf("colour interp failed: %d %d %d\n", rgb[0], rgb[1], rgb[2]);
        return 1;
    }

    // Colour apply packs the blended bytes with opaque alpha and sends the binding id.
    SParameterBinding colorBinding{};
    colorBinding.parameterId = 9;
    colorApplyKeyBasedValueEx(nullptr, 0, 0, 0.5f, nullptr, &colorBinding);
    if (gLastParameterId != 9 || gLastColor[0] != 25 || gLastColor[3] != 0xff) {
        std::printf("colour apply failed\n");
        return 1;
    }

    // Two-component weighted blend and count-one copy.
    float vec[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    float vweights[2] = {0.5f, 0.5f};
    float blended[2] = {0.0f, 0.0f};
    vec2GetBlendedValue(nullptr, vec, vweights, 2, blended);
    if (!nearly(blended[0], 2.0f) || !nearly(blended[1], 3.0f)) {
        std::printf("vec2 blend failed\n");
        return 1;
    }
    vec2ApplyBlendedValueEx(vec, vweights, 2, nullptr, &colorBinding);
    if (gLastVec2[0] != 2.0f || gLastVec2[1] != 3.0f) {
        std::printf("vec2 apply failed\n");
        return 1;
    }

    std::printf("recon_slice_13 tests passed (%d setter calls)\n", gSetterCalls);
    return 0;
}
