// Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CBlender_glitch_c-0:
// glitch::collada::animation_track::CBlender<glitch::core::quaternion, 1, glitch::core::quaternion>.
// Declarations are in glitch-scene-collada-gui/tb/glitch_collada_animation_track_CBlender_quaternion.hpp.

#include <cstdint>
#include <cstring>

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CBlender_quaternion.hpp"

namespace glitch::collada::animation_track {

namespace {

using glitch::core::quaternion;

// Key k of the quaternion array: four floats (x, y, z, w) per key.
using QuatKeys = const float (*)[4];

quaternion readKey(QuatKeys keys, int index) {
    return quaternion{keys[index][0], keys[index][1], keys[index][2], keys[index][3]};
}

void writeQuaternion(float* out, const quaternion& q) {
    out[0] = q.X;
    out[1] = q.Y;
    out[2] = q.Z;
    out[3] = q.W;
}

// Bit pattern of a float, as the original reads it when a float is returned as a 32-bit word.
std::uint32_t floatBits(float value) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof bits);
    return bits;
}

}  // namespace

// 0x6130d4 _ZN6glitch7collada15animation_track8CBlenderINS_4core10quaternionELi1ES4_E17getBlendedValueExEPvPfiS6_
// reconstructed: the first nonzero weight seeds the result, later nonzero weights are slerped in,
// and zero weights are skipped. The return value reproduces the Hex-Rays _BOOL4 result: 1 on the
// exact-weight early return; on the blended path the word of the x component, which the original's
// tail leaves in r0 (LODWORD(v29)). Callers that test the result as a bool see nonzero on that path.
std::uint32_t CBlender<quaternion, 1, quaternion>::getBlendedValueEx(void* values, float* weights,
                                                                      int count, void* out) {
    const QuatKeys keys = static_cast<QuatKeys>(values);
    float* result = static_cast<float*>(out);

    // v29..v32: running result, identity (0, 0, 0, 1) on entry.
    quaternion acc{0.0f, 0.0f, 0.0f, 1.0f};
    // v12: running weight total.
    float total = 0.0f;

    if (count <= 0) {
        // Original: v11 = 1 and v12 = 0, so the blend loop is skipped; the identity is written.
        writeQuaternion(result, acc);
        return floatBits(acc.X);
    }

    // First key with a nonzero weight. Key 0 is used when its weight is nonzero.
    int first = 0;
    if (weights[0] == 0.0f) {
        first = -1;
        for (int i = 1; i < count; ++i) {
            if (weights[i] != 0.0f) {
                first = i;
                break;
            }
        }
        if (first < 0) {
            // All weights are zero: the original jumps to the epilogue with v12 = 0 and writes the identity.
            writeQuaternion(result, acc);
            return floatBits(acc.X);
        }
    }

    total = weights[first];
    acc = readKey(keys, first);
    if (total == 1.0f) {
        // The first nonzero weight is exactly 1.0: the key is copied unchanged.
        writeQuaternion(result, acc);
        return 1;
    }

    for (int i = first + 1; i < count; ++i) {
        const float weight = weights[i];
        if (weight == 0.0f) {
            continue;
        }
        total = total + weight;
        acc = quaternion::slerp(acc, readKey(keys, i), weight / total);
    }

    writeQuaternion(result, acc);
    return floatBits(acc.X);
}

// 0x613378 _ZN6glitch7collada15animation_track8CBlenderINS_4core10quaternionELi1ES4_E15getAddedValueExEPvPfiS6_
// reconstructed: the running product starts at the identity. Each key contributes slerp(identity,
// key, w) for w > 0, and slerp(identity, key with x, y, z negated, -w) for w < 0. Zero weights and
// the accumulator are as the Hex-Rays listing has them: the accumulator is never updated, so
// every slerp starts from the identity. NaN weights take the positive branch, as the original's
// `w <= 0.0` test does.
void* CBlender<quaternion, 1, quaternion>::getAddedValueEx(void* values, float* weights, int count,
                                                            void* out) {
    const QuatKeys keys = static_cast<QuatKeys>(values);
    float* result = static_cast<float*>(out);

    // v52/v53: the slerp source. Only memset to zero plus w = 1.0 in the original; never written after.
    const quaternion identity{0.0f, 0.0f, 0.0f, 1.0f};
    // v48..v51: the running product, identity on entry.
    quaternion product{0.0f, 0.0f, 0.0f, 1.0f};

    for (int i = 0; i < count; ++i) {
        const float weight = weights[i];
        quaternion factor;
        if (weight <= 0.0f) {
            if (weight >= 0.0f) {
                // Zero weight: skipped.
                continue;
            }
            // Negative weight: conjugate the key (x, y, z negated, w kept) and slerp with -weight.
            const quaternion key = readKey(keys, i);
            const quaternion conjugate{-key.X, -key.Y, -key.Z, key.W};
            factor = quaternion::slerp(identity, conjugate, -weight);
        } else {
            // Positive weight (includes NaN, as in the original).
            factor = quaternion::slerp(identity, readKey(keys, i), weight);
        }
        product = product * factor;
    }

    writeQuaternion(result, product);
    return out;
}

}  // namespace glitch::collada::animation_track
