// Slice 10 reconstruction (glitch::collada::animation_track key-based value paths):
// CInterpreter, CInterpreterQuaternionAngle, CVirtualEx forwarders, the color delta,
// the quaternion applicator and the identity fill. Free functions for the rebuilt bodies,
// the templates they instantiate and the stand-in types they need. Existing headers are
// not edited; the member declarations these bodies belong to do not exist in the tree yet.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace glitch::collada::recon_10 {

// Stand-in for a key table. The original keeps the key array pointer at +0x04.
struct SKeyTable {
    std::uint32_t unmapped = 0;  // +0x00 not mapped
    const void* keys = nullptr;  // +0x04 key array; the element type is the track key type
};

// Stand-in for CInputReader<T, float, 1>, the three words the reader fills.
struct SInputReader {
    const SKeyTable* table = nullptr;  // +0x00 (table->keys at +0x04)
    const float* scale = nullptr;      // +0x04
    const float* offset = nullptr;     // +0x08
};

// Stand-in for SAnimationAccessor. Only the members the bodies read are modelled.
struct SAnimationAccessor {
    SInputReader reader;
    const std::uint32_t* defaultValue = nullptr;  // getDefaultValue(); non-null when hasDefaultValue()
    const SKeyTable* output = nullptr;            // getOutput(0)

    bool hasDefaultValue() const { return defaultValue != nullptr; }
    const std::uint32_t* getDefaultValue() const { return defaultValue; }
    const SKeyTable* getOutput(int) const { return output; }
};

// Stand-in for CApplicatorInfo. The original calls the object's vtable slot +0x9c with
// the computed value; the slot index is not mapped, so the call is a named virtual here.
struct CApplicatorInfo {
    virtual ~CApplicatorInfo() = default;
    virtual void applyValue(const float* value) = 0;
};

// Quaternion helpers of CO::quaternion (core-platform). Declared only; the bodies are
// not reconstructed in this slice.
float* quaternionFromAngleAxis(float* out, float angle, const float* axis);
void quaternionSlerp(float* out, const float* from, const float* to, float t);
void quaternionMultiply(float* out, const float* lhs, const float* rhs);

namespace detail {

inline std::uint32_t toWord(float value) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof bits);
    return bits;
}

// Reads key[index] of the reader's table as float, then applies scale and offset.
template <typename Key>
inline float keyScaled(const SInputReader& reader, int index) {
    const float key = static_cast<float>(static_cast<const Key*>(reader.table->keys)[index]);
    return key * *reader.scale + *reader.offset;
}

// Stores a value into a Dim-word buffer. With a default value the Dim words are copied
// and word Component receives the value; without one only word 0 receives it, as the
// decompiled non-default path does. Returns the default value pointer or null.
template <std::size_t Dim, std::size_t Component>
inline const std::uint32_t* storeKeyedValue(const SAnimationAccessor& acc, void* out, float value) {
    static_assert(Component < Dim, "the component word must lie inside the value");
    // Words are written with memcpy so the caller can read them back as floats
    // without violating strict aliasing.
    auto* bytes = static_cast<unsigned char*>(out);
    const std::uint32_t* defaults = acc.getDefaultValue();
    if (defaults != nullptr) {
        for (std::size_t i = 0; i < Dim; ++i) {
            std::memcpy(bytes + i * sizeof(std::uint32_t), &defaults[i], sizeof(std::uint32_t));
        }
        const std::uint32_t bits = toWord(value);
        std::memcpy(bytes + Component * sizeof(std::uint32_t), &bits, sizeof bits);
    } else {
        const std::uint32_t bits = toWord(value);
        std::memcpy(bytes, &bits, sizeof bits);
    }
    return defaults;
}

inline void conjugateXyz(float* q) {
    q[0] = -q[0];
    q[1] = -q[1];
    q[2] = -q[2];
}

inline void copyQuaternion(float* dst, const float* src) {
    for (std::size_t i = 0; i < 4; ++i) {
        dst[i] = src[i];
    }
}

}  // namespace detail

// CInterpreter<Mixin, float, Dim, SUseDefaultValues<Component, Key>>::getKeyBasedValueEx.
template <typename Key, std::size_t Dim, std::size_t Component>
struct CInterpreterKeyed {
    // (acc, key, out): the value of one key.
    static const std::uint32_t* getKeyBasedValueEx(const SAnimationAccessor& acc, int key, void* out) {
        return detail::storeKeyedValue<Dim, Component>(acc, out, detail::keyScaled<Key>(acc.reader, key));
    }

    // (acc, from, to, out): the difference between two keys.
    static const std::uint32_t* getKeyBasedValueEx(const SAnimationAccessor& acc, int from, int to, void* out) {
        const float delta = detail::keyScaled<Key>(acc.reader, to) - detail::keyScaled<Key>(acc.reader, from);
        return detail::storeKeyedValue<Dim, Component>(acc, out, delta);
    }

    // (acc, from, to, t, out): linear blend between two keys.
    static float getKeyBasedValueEx(const SAnimationAccessor& acc, int from, int to, float t, void* out) {
        const float begin = detail::keyScaled<Key>(acc.reader, from);
        const float value = t * (detail::keyScaled<Key>(acc.reader, to) - begin) + begin;
        detail::storeKeyedValue<Dim, Component>(acc, out, value);
        return value;
    }

    // (acc, base, from, to, t, out): blend of two key deltas measured from the base key.
    static float getKeyBasedValueEx(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out) {
        const float origin = detail::keyScaled<Key>(acc.reader, base);
        const float begin = detail::keyScaled<Key>(acc.reader, from) - origin;
        const float end = detail::keyScaled<Key>(acc.reader, to) - origin;
        const float value = begin + t * (end - begin);
        detail::storeKeyedValue<Dim, Component>(acc, out, value);
        return value;
    }
};

// CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNode...ComponentMixin<..., Component, Key>>>:
// each wrapper forwards to the CInterpreter instance with the same key, dimension and component.
template <typename Key, std::size_t Dim, std::size_t Component>
struct CVirtualExKeyed {
    using Interp = CInterpreterKeyed<Key, Dim, Component>;

    static const std::uint32_t* getKeyBasedValue(const SAnimationAccessor& acc, int key, void* out) {
        return Interp::getKeyBasedValueEx(acc, key, out);
    }
    static const std::uint32_t* getKeyBasedValue(const SAnimationAccessor& acc, int from, int to, void* out) {
        return Interp::getKeyBasedValueEx(acc, from, to, out);
    }
    static float getKeyBasedValue(const SAnimationAccessor& acc, int from, int to, float t, void* out) {
        return Interp::getKeyBasedValueEx(acc, from, to, t, out);
    }
    static float getKeyBasedValue(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out) {
        return Interp::getKeyBasedValueEx(acc, base, from, to, t, out);
    }
};

// CInterpreterQuaternionAngle<CSceneNodeQuaternionAngleMixin<Key>, Key>: the value is an
// axis-angle pair read as four words (axis x, y, z then angle at word 3), converted to a
// quaternion with fromAngleAxis. Blends are taken on the axis-angle words first (QA2) or
// on the converted quaternions (QA3 product, QA4 slerp).
template <typename Key>
struct CInterpreterQuaternionAngleKeyed {
    using Interp = CInterpreterKeyed<Key, 4, 3>;

    // (acc, key, out) -> out
    static float* getKeyBasedValueEx(const SAnimationAccessor& acc, int key, void* out) {
        float value[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        Interp::getKeyBasedValueEx(acc, key, value);
        return quaternionFromAngleAxis(static_cast<float*>(out), value[3], value);
    }

    // (acc, from, to, t, out) -> out
    static float* getKeyBasedValueEx(const SAnimationAccessor& acc, int from, int to, float t, void* out) {
        float value[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        Interp::getKeyBasedValueEx(acc, from, to, t, value);
        return quaternionFromAngleAxis(static_cast<float*>(out), value[3], value);
    }

    // (acc, from, to, out) -> product[0]; out receives conjugate(from) * to.
    static float getKeyBasedValueEx(const SAnimationAccessor& acc, int from, int to, void* out) {
        float toValue[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        float fromValue[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        Interp::getKeyBasedValueEx(acc, to, toValue);
        Interp::getKeyBasedValueEx(acc, from, fromValue);
        float toQuaternion[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        float fromQuaternion[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        float product[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        quaternionFromAngleAxis(toQuaternion, toValue[3], toValue);
        quaternionFromAngleAxis(fromQuaternion, fromValue[3], fromValue);
        detail::conjugateXyz(fromQuaternion);
        quaternionMultiply(product, fromQuaternion, toQuaternion);
        detail::copyQuaternion(static_cast<float*>(out), product);
        return product[0];
    }

    // (acc, base, from, to, t, out) -> product[0]; out receives conjugate(base) * slerp(from, to, t).
    static float getKeyBasedValueEx(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out) {
        float fromValue[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        float toValue[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        float baseValue[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        Interp::getKeyBasedValueEx(acc, from, fromValue);
        Interp::getKeyBasedValueEx(acc, to, toValue);
        Interp::getKeyBasedValueEx(acc, base, baseValue);
        float fromQuaternion[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        float toQuaternion[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        float blended[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        float baseQuaternion[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        float product[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        quaternionFromAngleAxis(fromQuaternion, fromValue[3], fromValue);
        quaternionFromAngleAxis(toQuaternion, toValue[3], toValue);
        quaternionSlerp(blended, fromQuaternion, toQuaternion, t);
        quaternionFromAngleAxis(baseQuaternion, baseValue[3], baseValue);
        detail::conjugateXyz(baseQuaternion);
        quaternionMultiply(product, baseQuaternion, blended);
        detail::copyQuaternion(static_cast<float*>(out), product);
        return product[0];
    }
};

// CVirtualEx<CApplyValueEx<quaternion, CSceneNodeQuaternionAngleMixin<Key>>> wrappers.
template <typename Key>
struct CVirtualExQuaternionAngleKeyed {
    using Interp = CInterpreterQuaternionAngleKeyed<Key>;

    static float* getKeyBasedValue(const SAnimationAccessor& acc, int key, void* out) {
        return Interp::getKeyBasedValueEx(acc, key, out);
    }
    static float* getKeyBasedValue(const SAnimationAccessor& acc, int from, int to, float t, void* out) {
        return Interp::getKeyBasedValueEx(acc, from, to, t, out);
    }
    static float getKeyBasedValue(const SAnimationAccessor& acc, int from, int to, void* out) {
        return Interp::getKeyBasedValueEx(acc, from, to, out);
    }
    static float getKeyBasedValue(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out) {
        return Interp::getKeyBasedValueEx(acc, base, from, to, t, out);
    }
};

// CApplyValueEx<quaternion, CSceneNodeQuaternionAngleMixin<Key>>::applyKeyBasedValueEx.
// The local value starts as (0, 0, 0, 1), is filled by the blend, then handed to the
// applicator. The value argument of the demangled signature is not read by the body.
template <typename Key>
struct CApplyQuaternionAngleKeyed {
    static void applyKeyBasedValueEx(const SAnimationAccessor& acc, int from, int to, float t, void* value,
                                     CApplicatorInfo* applicator) {
        (void)value;
        float quaternion[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        CInterpreterQuaternionAngleKeyed<Key>::getKeyBasedValueEx(acc, from, to, t, quaternion);
        applicator->applyValue(quaternion);
    }
};

// CVirtualEx<CApplyValueEx<quaternion, ...>>::applyKeyBasedValue forwards to the apply body.
template <typename Key>
struct CVirtualExApplyQuaternionAngleKeyed {
    static void applyKeyBasedValue(const SAnimationAccessor& acc, int from, int to, float t, void* value,
                                   CApplicatorInfo* applicator) {
        CApplyQuaternionAngleKeyed<Key>::applyKeyBasedValueEx(acc, from, to, t, value, applicator);
    }
};

// CInterpreter<CMixin<unsigned char, 4, SMaterialSetParam<...>, 3, unsigned char>, ...>::
// getKeyBasedValueEx(acc, from, to, out): the byte delta of the output track, truncated to
// char, goes to byte 3. With a default value bytes 0 to 2 are copied from it.
inline const std::uint32_t* colorDeltaKeyed(const SAnimationAccessor& acc, int from, int to, void* out) {
    const auto* keys = static_cast<const std::uint8_t*>(acc.getOutput(0)->keys);
    const char delta = static_cast<char>(keys[to] - keys[from]);
    auto* bytes = static_cast<std::uint8_t*>(out);
    const std::uint32_t* defaults = acc.getDefaultValue();
    if (defaults != nullptr) {
        const auto* defaultBytes = reinterpret_cast<const std::uint8_t*>(defaults);
        bytes[0] = defaultBytes[0];
        bytes[1] = defaultBytes[1];
        bytes[2] = defaultBytes[2];
        bytes[3] = static_cast<std::uint8_t>(delta);
    } else {
        bytes[0] = static_cast<std::uint8_t>(delta);
    }
    return defaults;
}

// CVirtualEx<CApplyValueEx<float [4], CMixin<float, 4, ...>>>::getIdentityValue: zeroes
// the four words of the value.
inline void identityValue16(void* value) {
    std::memset(value, 0, 16);
}

// Free functions for the rebuilt bodies (one per queued address).

// 0x6157ec _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float CVirtualEx_PosY_short_getKeyBasedValue_6157ec(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x615808 _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionYExIsEEfLi3ENS1_17SUseDefaultValuesILi1EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CInterpreter_PosY_short_getKeyBasedValueEx_615808(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x6158d8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CVirtualEx_PosY_short_getKeyBasedValue_6158d8(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x6158ec _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionYExIsEEfLi3ENS1_17SUseDefaultValuesILi1EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreter_PosY_short_getKeyBasedValueEx_6158ec(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x615a38 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_PosY_short_getKeyBasedValue_615a38(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x615a5c _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionYExIcEEfLi3ENS1_17SUseDefaultValuesILi1EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CInterpreter_PosY_char_getKeyBasedValueEx_615a5c(const SAnimationAccessor& acc, int key, void* out);

// 0x615af8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CVirtualEx_PosY_char_getKeyBasedValue_615af8(const SAnimationAccessor& acc, int key, void* out);

// 0x615b08 _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionYExIcEEfLi3ENS1_17SUseDefaultValuesILi1EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float CInterpreter_PosY_char_getKeyBasedValueEx_615b08(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x615c10 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float CVirtualEx_PosY_char_getKeyBasedValue_615c10(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x615c2c _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionYExIcEEfLi3ENS1_17SUseDefaultValuesILi1EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CInterpreter_PosY_char_getKeyBasedValueEx_615c2c(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x615cf4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CVirtualEx_PosY_char_getKeyBasedValue_615cf4(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x615d08 _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionYExIcEEfLi3ENS1_17SUseDefaultValuesILi1EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreter_PosY_char_getKeyBasedValueEx_615d08(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x615e48 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_PosY_char_getKeyBasedValue_615e48(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x615ed8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CVirtualEx_PosZ_float_getKeyBasedValue_615ed8(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x615eec _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionZExIsEEfLi3ENS1_17SUseDefaultValuesILi2EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CInterpreter_PosZ_short_getKeyBasedValueEx_615eec(const SAnimationAccessor& acc, int key, void* out);

// 0x615f8c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CVirtualEx_PosZ_short_getKeyBasedValue_615f8c(const SAnimationAccessor& acc, int key, void* out);

// 0x615f9c _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionZExIsEEfLi3ENS1_17SUseDefaultValuesILi2EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float CInterpreter_PosZ_short_getKeyBasedValueEx_615f9c(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x6160a8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float CVirtualEx_PosZ_short_getKeyBasedValue_6160a8(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x6160c4 _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionZExIsEEfLi3ENS1_17SUseDefaultValuesILi2EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CInterpreter_PosZ_short_getKeyBasedValueEx_6160c4(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x616194 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CVirtualEx_PosZ_short_getKeyBasedValue_616194(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x6161a8 _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionZExIsEEfLi3ENS1_17SUseDefaultValuesILi2EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreter_PosZ_short_getKeyBasedValueEx_6161a8(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x6162f4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_PosZ_short_getKeyBasedValue_6162f4(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x616318 _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionZExIcEEfLi3ENS1_17SUseDefaultValuesILi2EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CInterpreter_PosZ_char_getKeyBasedValueEx_616318(const SAnimationAccessor& acc, int key, void* out);

// 0x6163b4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CVirtualEx_PosZ_char_getKeyBasedValue_6163b4(const SAnimationAccessor& acc, int key, void* out);

// 0x6163c4 _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionZExIcEEfLi3ENS1_17SUseDefaultValuesILi2EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float CInterpreter_PosZ_char_getKeyBasedValueEx_6163c4(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x6164c8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float CVirtualEx_PosZ_char_getKeyBasedValue_6164c8(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x6164e4 _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionZExIcEEfLi3ENS1_17SUseDefaultValuesILi2EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CInterpreter_PosZ_char_getKeyBasedValueEx_6164e4(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x6165ac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CVirtualEx_PosZ_char_getKeyBasedValue_6165ac(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x6165c0 _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionZExIcEEfLi3ENS1_17SUseDefaultValuesILi2EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreter_PosZ_char_getKeyBasedValueEx_6165c0(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x616700 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_PosZ_char_getKeyBasedValue_616700(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x616724 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleXExIsEEfLi3ENS1_17SUseDefaultValuesILi0EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CInterpreter_ScaleX_short_getKeyBasedValueEx_616724(const SAnimationAccessor& acc, int key, void* out);

// 0x6167c4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CVirtualEx_ScaleX_short_getKeyBasedValue_6167c4(const SAnimationAccessor& acc, int key, void* out);

// 0x6167d4 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleXExIsEEfLi3ENS1_17SUseDefaultValuesILi0EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float CInterpreter_ScaleX_short_getKeyBasedValueEx_6167d4(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x6168e4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float CVirtualEx_ScaleX_short_getKeyBasedValue_6168e4(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x616900 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleXExIsEEfLi3ENS1_17SUseDefaultValuesILi0EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CInterpreter_ScaleX_short_getKeyBasedValueEx_616900(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x6169d0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CVirtualEx_ScaleX_short_getKeyBasedValue_6169d0(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x6169e4 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleXExIsEEfLi3ENS1_17SUseDefaultValuesILi0EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreter_ScaleX_short_getKeyBasedValueEx_6169e4(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x616b30 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_ScaleX_short_getKeyBasedValue_616b30(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x616b54 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleXExIcEEfLi3ENS1_17SUseDefaultValuesILi0EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CInterpreter_ScaleX_char_getKeyBasedValueEx_616b54(const SAnimationAccessor& acc, int key, void* out);

// 0x616bf0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CVirtualEx_ScaleX_char_getKeyBasedValue_616bf0(const SAnimationAccessor& acc, int key, void* out);

// 0x616c00 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleXExIcEEfLi3ENS1_17SUseDefaultValuesILi0EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float CInterpreter_ScaleX_char_getKeyBasedValueEx_616c00(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x616d08 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float CVirtualEx_ScaleX_char_getKeyBasedValue_616d08(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x616d24 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleXExIcEEfLi3ENS1_17SUseDefaultValuesILi0EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CInterpreter_ScaleX_char_getKeyBasedValueEx_616d24(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x616dec _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CVirtualEx_ScaleX_char_getKeyBasedValue_616dec(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x616e00 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleXExIcEEfLi3ENS1_17SUseDefaultValuesILi0EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreter_ScaleX_char_getKeyBasedValueEx_616e00(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x616f40 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_ScaleX_char_getKeyBasedValue_616f40(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x616f64 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIsEEfLi3ENS1_17SUseDefaultValuesILi1EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CInterpreter_ScaleY_short_getKeyBasedValueEx_616f64(const SAnimationAccessor& acc, int key, void* out);

// 0x617004 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CVirtualEx_ScaleY_short_getKeyBasedValue_617004(const SAnimationAccessor& acc, int key, void* out);

// 0x617014 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIsEEfLi3ENS1_17SUseDefaultValuesILi1EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float CInterpreter_ScaleY_short_getKeyBasedValueEx_617014(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x617124 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float CVirtualEx_ScaleY_short_getKeyBasedValue_617124(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x617140 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIsEEfLi3ENS1_17SUseDefaultValuesILi1EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CInterpreter_ScaleY_short_getKeyBasedValueEx_617140(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x617210 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CVirtualEx_ScaleY_short_getKeyBasedValue_617210(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x617224 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIsEEfLi3ENS1_17SUseDefaultValuesILi1EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreter_ScaleY_short_getKeyBasedValueEx_617224(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x617370 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_ScaleY_short_getKeyBasedValue_617370(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x617394 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIcEEfLi3ENS1_17SUseDefaultValuesILi1EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CInterpreter_ScaleY_char_getKeyBasedValueEx_617394(const SAnimationAccessor& acc, int key, void* out);

// 0x617430 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CVirtualEx_ScaleY_char_getKeyBasedValue_617430(const SAnimationAccessor& acc, int key, void* out);

// 0x617440 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIcEEfLi3ENS1_17SUseDefaultValuesILi1EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float CInterpreter_ScaleY_char_getKeyBasedValueEx_617440(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x617548 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float CVirtualEx_ScaleY_char_getKeyBasedValue_617548(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x617564 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIcEEfLi3ENS1_17SUseDefaultValuesILi1EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CInterpreter_ScaleY_char_getKeyBasedValueEx_617564(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x61762c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CVirtualEx_ScaleY_char_getKeyBasedValue_61762c(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x617640 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIcEEfLi3ENS1_17SUseDefaultValuesILi1EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreter_ScaleY_char_getKeyBasedValueEx_617640(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x617780 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_ScaleY_char_getKeyBasedValue_617780(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x6177a4 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleZExIsEEfLi3ENS1_17SUseDefaultValuesILi2EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CInterpreter_ScaleZ_short_getKeyBasedValueEx_6177a4(const SAnimationAccessor& acc, int key, void* out);

// 0x617844 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CVirtualEx_ScaleZ_short_getKeyBasedValue_617844(const SAnimationAccessor& acc, int key, void* out);

// 0x617854 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleZExIsEEfLi3ENS1_17SUseDefaultValuesILi2EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float CInterpreter_ScaleZ_short_getKeyBasedValueEx_617854(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x617960 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float CVirtualEx_ScaleZ_short_getKeyBasedValue_617960(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x61797c _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleZExIsEEfLi3ENS1_17SUseDefaultValuesILi2EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CInterpreter_ScaleZ_short_getKeyBasedValueEx_61797c(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x617a4c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CVirtualEx_ScaleZ_short_getKeyBasedValue_617a4c(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x617a60 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleZExIsEEfLi3ENS1_17SUseDefaultValuesILi2EsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreter_ScaleZ_short_getKeyBasedValueEx_617a60(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x617bac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_ScaleZ_short_getKeyBasedValue_617bac(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x617bd0 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleZExIcEEfLi3ENS1_17SUseDefaultValuesILi2EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CInterpreter_ScaleZ_char_getKeyBasedValueEx_617bd0(const SAnimationAccessor& acc, int key, void* out);

// 0x617c6c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const std::uint32_t* CVirtualEx_ScaleZ_char_getKeyBasedValue_617c6c(const SAnimationAccessor& acc, int key, void* out);

// 0x617c7c _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleZExIcEEfLi3ENS1_17SUseDefaultValuesILi2EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float CInterpreter_ScaleZ_char_getKeyBasedValueEx_617c7c(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x617d80 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float CVirtualEx_ScaleZ_char_getKeyBasedValue_617d80(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x617d9c _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleZExIcEEfLi3ENS1_17SUseDefaultValuesILi2EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CInterpreter_ScaleZ_char_getKeyBasedValueEx_617d9c(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x617e64 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CVirtualEx_ScaleZ_char_getKeyBasedValue_617e64(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x617e78 _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleZExIcEEfLi3ENS1_17SUseDefaultValuesILi2EcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreter_ScaleZ_char_getKeyBasedValueEx_617e78(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x617fb8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_ScaleZ_char_getKeyBasedValue_617fb8(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x617fdc _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi3EhEEhLi4ENS1_17SUseDefaultValuesILi3EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CInterpreter_Color_uchar_getKeyBasedValueEx_617fdc(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x618050 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const std::uint32_t* CVirtualEx_Color_uchar_getKeyBasedValue_618050(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x618064 _ZN6glitch7collada15animation_track27CInterpreterQuaternionAngleINS1_30CSceneNodeQuaternionAngleMixinIsEEsE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
float* CInterpreterQuaternionAngle_QAngle_short_getKeyBasedValueEx_618064(const SAnimationAccessor& acc, int key, void* out);

// 0x6180a4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
float* CVirtualEx_QAngle_short_getKeyBasedValue_6180a4(const SAnimationAccessor& acc, int key, void* out);

// 0x6180b4 _ZN6glitch7collada15animation_track27CInterpreterQuaternionAngleINS1_30CSceneNodeQuaternionAngleMixinIsEEsE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float* CInterpreterQuaternionAngle_QAngle_short_getKeyBasedValueEx_6180b4(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x6180f0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float* CVirtualEx_QAngle_short_getKeyBasedValue_6180f0(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x61810c _ZN6glitch7collada15animation_track27CInterpreterQuaternionAngleINS1_30CSceneNodeQuaternionAngleMixinIsEEsE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float CInterpreterQuaternionAngle_QAngle_short_getKeyBasedValueEx_61810c(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x618208 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float CVirtualEx_QAngle_short_getKeyBasedValue_618208(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x61821c _ZN6glitch7collada15animation_track27CInterpreterQuaternionAngleINS1_30CSceneNodeQuaternionAngleMixinIsEEsE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreterQuaternionAngle_QAngle_short_getKeyBasedValueEx_61821c(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x61839c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_QAngle_short_getKeyBasedValue_61839c(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x6183c0 _ZN6glitch7collada15animation_track27CInterpreterQuaternionAngleINS1_30CSceneNodeQuaternionAngleMixinIcEEcE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
float* CInterpreterQuaternionAngle_QAngle_char_getKeyBasedValueEx_6183c0(const SAnimationAccessor& acc, int key, void* out);

// 0x618400 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
float* CVirtualEx_QAngle_char_getKeyBasedValue_618400(const SAnimationAccessor& acc, int key, void* out);

// 0x618410 _ZN6glitch7collada15animation_track27CInterpreterQuaternionAngleINS1_30CSceneNodeQuaternionAngleMixinIcEEcE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float* CInterpreterQuaternionAngle_QAngle_char_getKeyBasedValueEx_618410(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x61844c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
void CApplyValueEx_QAngle_char_applyKeyBasedValueEx_61844c(const SAnimationAccessor& acc, int from, int to, float t, void* value, CApplicatorInfo* applicator);

// 0x618498 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
void CVirtualEx_QAngle_char_applyKeyBasedValue_618498(const SAnimationAccessor& acc, int from, int to, float t, void* value, CApplicatorInfo* applicator);

// 0x6184bc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float* CVirtualEx_QAngle_char_getKeyBasedValue_6184bc(const SAnimationAccessor& acc, int from, int to, float t, void* out);

// 0x6184d8 _ZN6glitch7collada15animation_track27CInterpreterQuaternionAngleINS1_30CSceneNodeQuaternionAngleMixinIcEEcE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float CInterpreterQuaternionAngle_QAngle_char_getKeyBasedValueEx_6184d8(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x6185d4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float CVirtualEx_QAngle_char_getKeyBasedValue_6185d4(const SAnimationAccessor& acc, int from, int to, void* out);

// 0x6185e8 _ZN6glitch7collada15animation_track27CInterpreterQuaternionAngleINS1_30CSceneNodeQuaternionAngleMixinIcEEcE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreterQuaternionAngle_QAngle_char_getKeyBasedValueEx_6185e8(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x618768 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_QAngle_char_getKeyBasedValue_618768(const SAnimationAccessor& acc, int base, int from, int to, float t, void* out);

// 0x618e68 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getIdentityValueEPv
void CVirtualEx_Float4_c0_getIdentityValue_618e68(void* value);

// 0x618e78 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE16getIdentityValueEPv
void CVirtualEx_Float4_c1_getIdentityValue_618e78(void* value);

}  // namespace glitch::collada::recon_10
