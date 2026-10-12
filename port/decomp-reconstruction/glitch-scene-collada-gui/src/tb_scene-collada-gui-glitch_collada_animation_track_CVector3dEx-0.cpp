#include "glitch-scene-collada-gui/vector3d_ex.hpp"

namespace glitch::collada::animation_track {

// 0x6e3fb0 _ZN6glitch7collada15animation_track11CVector3dEx17getBlendedValueExEPvPfiS3_
// Partial: keys is the key block the original reads from `this` (component c of key i at
// keys[3*i + c], byte offset +12*i +4*c). Layout request: map the key block on CVector3dEx.
// Other behaviour: count == 2 blends key 0 and key 1 by weights[1] / (weights[0] + weights[1]),
// writes out = (x, y, z) and returns x; count == 1 writes key 0 and returns the this bits in
// the original (not reproduced, 0.0f here); count >= 3 accumulates keys 1..count-1 with a running
// weight sum, writes out and returns z; count <= 0 writes nothing (return as count == 1).
float CVector3dEx_getBlendedValueEx(const float* keys, const float* weights, int count, float* out) {
    float result = 0.0f;
    if (count <= 2) {
        if (count == 2) {
            // +0x04 is a2[1], +0x00 is a2[0] in the original
            const float t = weights[1] / (weights[1] + weights[0]);
            // key 1 at +0x0c (x), +0x10 (y), +0x14 (z)
            const float y = keys[1] + (t * (keys[4] - keys[1]));
            const float x = keys[0] + (t * (keys[3] - keys[0]));
            out[2] = keys[2] + (t * (keys[5] - keys[2]));
            out[0] = x;
            out[1] = y;
            result = x;
        } else if (count == 1) {
            out[0] = keys[0];
            out[1] = keys[1];
            out[2] = keys[2];
            // original returns the this-pointer bits here; not reproduced
        }
        return result;
    }

    // count >= 3: running blend over keys 1..count-1
    float vx = keys[0];   // v5
    float vy = keys[1];   // v6
    float vz = keys[2];   // v7
    float wsum = weights[0];  // v8
    float z = vz;
    for (int i = 1; i != count; ++i) {
        wsum = wsum + weights[i];
        const float k = weights[i] / wsum;  // v11
        const float* key = keys + 3 * i;    // +12*i in the original
        vx = vx + (k * (key[0] - vx));
        vy = vy + (k * (key[1] - vy));
        z = vz + (k * (key[2] - vz));
        vz = z;
    }
    out[2] = z;
    out[0] = vx;
    out[1] = vy;
    return z;
}

// 0x6e41ec _ZN6glitch7collada15animation_track11CVector3dEx17getBlendedValueExEPvPfiS3_f
// Partial: inherits the key-block layout gap from the 4-argument overload (keys passed explicitly).
// Blends the 4-argument result into out with the factor: out = (1 - f) * out + f * blended, for x, y, z.
float CVector3dEx_getBlendedValueEx(const float* keys, const float* weights, int count, float* out, float factor) {
    float blended[3] = {0.0f, 0.0f, 0.0f};  // v10 (x), v11 (y), v12 (z)
    CVector3dEx_getBlendedValueEx(keys, weights, count, blended);
    const float y = ((1.0f - factor) * out[1]) + (factor * blended[1]);
    const float x = ((1.0f - factor) * out[0]) + (factor * blended[0]);
    out[2] = ((1.0f - factor) * out[2]) + (factor * blended[2]);
    out[0] = x;
    out[1] = y;
    return x;
}

}  // namespace glitch::collada::animation_track
