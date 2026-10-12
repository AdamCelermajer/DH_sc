// Track B batch e2-glitch-scene-collada-gui-glitch_collada_SAnimationAccessor-a4d950-0.
// glitch::collada::SAnimationAccessor key search (findKeyFrameNo / findKeyFrameNoEx), 14 functions.
// Declarations are in glitch-scene-collada-gui/layouts/animation_track.hpp. The 4-arg
// findKeyFrameNoEx<int,1000> (0x66a1f0) is defined and explicitly instantiated in the sibling
// batch file tb_scene-collada-gui-glitch_collada_SAnimationAccessor-0.cpp; the explicit
// specializations here call it.
//
// Key vectors are glitch::res::vector<int> objects in guest memory (count word, data address).
// Their layout is not in the tree, so the count and data address come from the seam below, and
// every key and cache field is read through the guest seam. The control flow follows the
// Hex-Rays pseudocode, including the goto structure of the hint searches (labels keep the
// Hex-Rays names in comments). Every row is partial because the seam is a placeholder.
//
// The 8-bit and 16-bit key forms (uchar, ushort) compare against time / 33.333 and check
// equality against key * 33.333; the int form compares time directly. Both scales are double
// literals in the original (33.333), so they are computed as double and rounded to float.

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

#include "glitch-scene-collada-gui/layouts/animation_track.hpp"

namespace glitch::res {
template <typename T>
class vector;
}  // namespace glitch::res

namespace glitch::collada {

namespace {

constexpr double kKeyTickMs = 33.333;  // scale of the 8-bit and 16-bit key forms (double literal)

// Offsets in the key-search cache record (SAnimationAccessor +0x08), layouts/animation_evidence.hpp.
constexpr std::uint32_t kCacheFraction = 0x00;  // float, written as *a6
constexpr std::uint32_t kCacheKeyId = 0x04;     // compared with the time argument
constexpr std::uint32_t kCacheIndex = 0x08;     // written as *a5
constexpr std::uint32_t kCacheFound = 0x0c;     // result byte
constexpr std::uint32_t kCacheEnabled = 0x0d;   // cache is used only when nonzero

// Guest memory seam. Placeholder until the guest image map exists: reads return 0, writes are dropped.
std::uint32_t guestU32(std::uint32_t address) {
    (void)address;
    return 0;  // placeholder: guest word read
}

std::uint16_t guestU16(std::uint32_t address) {
    (void)address;
    return 0;  // placeholder: guest halfword read
}

std::uint8_t guestU8(std::uint32_t address) {
    (void)address;
    return 0;  // placeholder: guest byte read
}

float guestF32(std::uint32_t address) {
    const std::uint32_t bits = guestU32(address);
    float value = 0.0f;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}

void guestWriteU32(std::uint32_t address, std::uint32_t value) {
    (void)address;
    (void)value;  // placeholder: guest word write is dropped
}

void guestWriteU8(std::uint32_t address, std::uint8_t value) {
    (void)address;
    (void)value;  // placeholder: guest byte write is dropped
}

void guestWriteF32(std::uint32_t address, float value) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof bits);
    guestWriteU32(address, bits);
}

// glitch::res::vector<int> seam. The layout is not in the tree: the element count is the word
// before the data pointer, and the data pointer is the word at +0x04. Placeholders return 0.
int keysCount(const glitch::res::vector<int>& keys) {
    (void)keys;
    return 0;  // placeholder: element count (vector layout not mapped)
}

std::uint32_t keysData(const glitch::res::vector<int>& keys) {
    (void)keys;
    return 0;  // placeholder: guest data address (vector layout not mapped)
}

// Element index of the key data, read at the element width of T: int 4 bytes, std::uint8_t 1 byte,
// std::uint16_t 2 bytes. The original indexes the data pointer with the element size, not a byte offset.
template <typename T>
T keyAt(std::uint32_t data, int index) {
    const std::uint32_t address = data + static_cast<std::uint32_t>(index) * static_cast<std::uint32_t>(sizeof(T));
    if constexpr (std::is_same_v<T, int>) {
        return static_cast<int>(guestU32(address));
    } else if constexpr (std::is_same_v<T, std::uint8_t>) {
        return guestU8(address);
    } else {
        static_assert(std::is_same_v<T, std::uint16_t>, "key element width is 4, 1 or 2 bytes");
        return guestU16(address);
    }
}

// (float)key, the value compared against the threshold.
template <typename T>
float keyCompare(std::uint32_t data, int index) {
    return static_cast<float>(keyAt<T>(data, index));
}

// The equality value: (float)key for int, (float)((float)key * 33.333) for the 8-bit and 16-bit forms.
template <typename T>
float keyEquality(std::uint32_t data, int index) {
    if constexpr (std::is_same_v<T, int>) {
        return keyCompare<T>(data, index);
    } else {
        return static_cast<float>(static_cast<double>(keyCompare<T>(data, index)) * kKeyTickMs);
    }
}

// Threshold for the comparisons: (float)time for int, (float)((float)time / 33.333) for the 8-bit and 16-bit forms.
template <typename T>
float compareThreshold(int time) {
    if constexpr (std::is_same_v<T, int>) {
        return static_cast<float>(time);
    } else {
        return static_cast<float>(static_cast<double>(static_cast<float>(time)) / kKeyTickMs);
    }
}

// The time as the equality float, for every width.
float equalityTime(int time) {
    return static_cast<float>(time);
}

// (int)(float) of the key on the millisecond scale. Used by the fraction.
template <typename T>
int keyMilliseconds(std::uint32_t data, int index) {
    return static_cast<int>(keyEquality<T>(data, index));
}

// Fraction of time between key index and index + 1. Called only after a key was found, so index + 1 is
// a valid element (the search returns false at the last key).
template <typename T>
float keyFraction(std::uint32_t data, int time, int index) {
    const int start = keyMilliseconds<T>(data, index);
    const int end = keyMilliseconds<T>(data, index + 1);
    return static_cast<float>(time - start) / static_cast<float>(end - start);
}

// The original stores the fraction as bits after clamping: below 0 gives 0, at or above 1 gives 1,
// and NaN gives 0 (the comparison fails).
float clampUnit(float value) {
    if (value >= 0.0f) {
        return value >= 1.0f ? 1.0f : value;
    }
    return 0.0f;
}

template <typename T>
void storeFraction(std::uint32_t data, int time, int index, float& fraction) {
    fraction = clampUnit(keyFraction<T>(data, time, index));
}

// Binary search with no hint (uchar 0x66a6d0, ushort 0x66abb8). The result index is always written.
template <typename T>
bool searchKeyFrame(const glitch::res::vector<int>& keys, int time, int& outIndex) {
    const std::uint32_t data = keysData(keys);
    const int last = keysCount(keys) - 1;
    const float threshold = compareThreshold<T>(time);
    int high = last;
    if (high > 0) {
        int low = 1;
        do {
            const int mid = (low + high) >> 1;
            if (keyCompare<T>(data, mid) <= threshold) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        } while (low <= high);
    }
    outIndex = high;
    return equalityTime(time) != keyEquality<T>(data, high) && high != last;
}

// 0x66a368 (int, hint). Hex-Rays labels are kept as comments. The binary search over the whole key
// vector runs inline here (label9), unlike the 8-bit and 16-bit forms, which fall back to searchKeyFrame.
bool searchKeyFrameIntHint(const glitch::res::vector<int>& keys, int time, int& outIndex, int hint) {
    const std::uint32_t data = keysData(keys);
    const int last = keysCount(keys) - 1;
    const float timeF = static_cast<float>(time);
    int high = last;
    int v9 = hint < 0 ? 0 : hint;  // a5 & ~(a5 >> 31)
    if (v9 >= last) {
        v9 = last;
    }
    bool v11 = keyCompare<int>(data, v9) > timeF;
    int v12 = 0;
    int v14 = 0;
    float v19 = 0.0f;

    if (v11 && v9 > 0) {
        v12 = v9 - 1;
        goto label7;
    }
    if (high <= v9) {
        v14 = v9;
        goto label25;
    }
    v14 = v9 + 1;
    v19 = keyCompare<int>(data, v14);
    if (v19 >= timeF) {
        v14 = v9;
        goto label9;
    }
    if (high <= v14) {
        goto label25;
    }
    v12 = v9 + 2;
    if (keyCompare<int>(data, v14 + 1) >= timeF) {
        v11 = timeF < v19;
        goto label9;
    }
label7:
    if (v12 >= high) {
        goto label27;
    }
    v11 = timeF < keyCompare<int>(data, v12);
    v14 = v12;
label9:
    if (v11 || keyCompare<int>(data, v14 + 1) < timeF) {
        if (high > 0) {
            int low = 1;
            do {
                const int mid = (low + high) >> 1;
                if (timeF >= keyCompare<int>(data, mid)) {
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            } while (low <= high);
        }
        outIndex = high;
        return timeF != keyCompare<int>(data, high) && last != high;
    }
label25:
    v12 = v14;  // LABEL_25 and LABEL_26
label27:
    outIndex = v12;
    return timeF != keyCompare<int>(data, v12) && last != v12;
}

// 0x66a894 (uchar, hint) and 0x66b130 (ushort, hint). The two are the same control flow over a
// 1-byte or 2-byte key. The result index (v11) and the equality index (v12) differ only on the
// fall-through to the no-hint search (label9), which the original reaches through findKeyFrameNo.
template <typename T>
bool searchKeyFrameNarrowHint(const glitch::res::vector<int>& keys, int time, int& outIndex, int hint) {
    const std::uint32_t data = keysData(keys);
    const int last = keysCount(keys) - 1;
    const float threshold = compareThreshold<T>(time);
    int v8 = hint < 0 ? 0 : hint;
    if (v8 >= last) {
        v8 = last;
    }
    bool v10 = keyCompare<T>(data, v8) > threshold;
    int v11 = 0;
    int v12 = 0;
    int v13 = 0;
    float v16 = 0.0f;

    if (v10 && v8 > 0) {
        v11 = v8 - 1;
        goto label7;
    }
    if (last <= v8) {
        v13 = v8;
        goto label13;
    }
    v13 = v8 + 1;
    v16 = keyCompare<T>(data, v13);
    v12 = v8 + 1;
    if (v16 >= threshold) {
        v12 = v8;
        v13 = v8;
        goto label9;
    }
    if (last <= v13) {
        goto label13;
    }
    v11 = v8 + 2;
    if (keyCompare<T>(data, v13 + 1) >= threshold) {
        v10 = threshold < v16;
        goto label9;
    }
label7:
    if (v11 >= last) {
        v12 = v11;
        goto label14;
    }
    v12 = v11;
    v10 = threshold < keyCompare<T>(data, v11);
    v13 = v11;
label9:
    if (v10 || keyCompare<T>(data, v13 + 1) < threshold) {
        return searchKeyFrame<T>(keys, time, outIndex);
    }
    v11 = v13;
    goto label14;
label13:
    v12 = v13;
    v11 = v13;
label14:
    outIndex = v11;
    return equalityTime(time) != keyEquality<T>(data, v12) && v11 != last;
}

}  // namespace

// 0x66a6d0 _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoIhLi30EEEbRKNS_3res6vectorIiEEiRi
// partial: binary search over the 8-bit keys (guest seam). Control flow matches Hex-Rays.
template <>
bool SAnimationAccessor::findKeyFrameNo<std::uint8_t, 30>(const glitch::res::vector<int>& keys, int time,
                                                          int& outIndex) const {
    return searchKeyFrame<std::uint8_t>(keys, time, outIndex);
}

// 0x66abb8 _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoItLi30EEEbRKNS_3res6vectorIiEEiRi
// partial: binary search over the 16-bit keys (guest seam). Control flow matches Hex-Rays.
template <>
bool SAnimationAccessor::findKeyFrameNo<std::uint16_t, 30>(const glitch::res::vector<int>& keys, int time,
                                                           int& outIndex) const {
    return searchKeyFrame<std::uint16_t>(keys, time, outIndex);
}

// 0x66a368 _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoIiLi1000EEEbRKNS_3res6vectorIiEEiRii
// partial: hinted search over the int keys (guest seam). The control flow follows the gotos of Hex-Rays.
template <>
bool SAnimationAccessor::findKeyFrameNo<int, 1000>(const glitch::res::vector<int>& keys, int time,
                                                   int& outIndex, int hint) const {
    return searchKeyFrameIntHint(keys, time, outIndex, hint);
}

// 0x66a894 _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoIhLi30EEEbRKNS_3res6vectorIiEEiRii
// partial: hinted search over the 8-bit keys (guest seam). Falls back to the no-hint search.
template <>
bool SAnimationAccessor::findKeyFrameNo<std::uint8_t, 30>(const glitch::res::vector<int>& keys, int time,
                                                          int& outIndex, int hint) const {
    return searchKeyFrameNarrowHint<std::uint8_t>(keys, time, outIndex, hint);
}

// 0x66b130 _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoItLi30EEEbRKNS_3res6vectorIiEEiRii
// partial: hinted search over the 16-bit keys (guest seam). Falls back to the no-hint search.
template <>
bool SAnimationAccessor::findKeyFrameNo<std::uint16_t, 30>(const glitch::res::vector<int>& keys, int time,
                                                           int& outIndex, int hint) const {
    return searchKeyFrameNarrowHint<std::uint16_t>(keys, time, outIndex, hint);
}

// 0x66a788 _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExIhLi30EEEbiRKNS_3res6vectorIiEEiRi
// partial: no-hint search, then getInterpolationType. Result is the AND of the two.
template <>
bool SAnimationAccessor::findKeyFrameNoEx<std::uint8_t, 30>(int index, const glitch::res::vector<int>& keys,
                                                            int time, int& outIndex) const {
    const bool keyFrame = findKeyFrameNo<std::uint8_t, 30>(keys, time, outIndex);
    return getInterpolationType(index) != 0 && keyFrame;
}

// 0x66ac78 _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExItLi30EEEbiRKNS_3res6vectorIiEEiRi
// partial: no-hint search, then getInterpolationType. Result is the AND of the two.
template <>
bool SAnimationAccessor::findKeyFrameNoEx<std::uint16_t, 30>(int index, const glitch::res::vector<int>& keys,
                                                             int time, int& outIndex) const {
    const bool keyFrame = findKeyFrameNo<std::uint16_t, 30>(keys, time, outIndex);
    return getInterpolationType(index) != 0 && keyFrame;
}

// 0x66a2ac _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExIiLi1000EEEbiRKNS_3res6vectorIiEEiRiRf
// partial: the 4-arg search is the sibling batch's findKeyFrameNoEx<int,1000>. Fraction is clamped to [0,1].
template <>
bool SAnimationAccessor::findKeyFrameNoEx<int, 1000>(int index, const glitch::res::vector<int>& keys, int time,
                                                     int& outIndex, float& fraction) const {
    const bool keyFrame = findKeyFrameNoEx<int, 1000>(index, keys, time, outIndex);
    if (keyFrame) {
        storeFraction<int>(keysData(keys), time, outIndex, fraction);
    }
    return keyFrame;
}

// 0x66a7c4 _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExIhLi30EEEbiRKNS_3res6vectorIiEEiRiRf
// partial: 8-bit key fraction (scaled by 33.333), clamped to [0,1].
template <>
bool SAnimationAccessor::findKeyFrameNoEx<std::uint8_t, 30>(int index, const glitch::res::vector<int>& keys,
                                                            int time, int& outIndex, float& fraction) const {
    const bool keyFrame = findKeyFrameNoEx<std::uint8_t, 30>(index, keys, time, outIndex);
    if (keyFrame) {
        storeFraction<std::uint8_t>(keysData(keys), time, outIndex, fraction);
    }
    return keyFrame;
}

// 0x66acb4 _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExItLi30EEEbiRKNS_3res6vectorIiEEiRiRf
// partial: 16-bit key fraction (scaled by 33.333), clamped to [0,1].
template <>
bool SAnimationAccessor::findKeyFrameNoEx<std::uint16_t, 30>(int index, const glitch::res::vector<int>& keys,
                                                             int time, int& outIndex, float& fraction) const {
    const bool keyFrame = findKeyFrameNoEx<std::uint16_t, 30>(index, keys, time, outIndex);
    if (keyFrame) {
        storeFraction<std::uint16_t>(keysData(keys), time, outIndex, fraction);
    }
    return keyFrame;
}

// 0x66a5e0 _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExIiLi1000EEEbiRKNS_3res6vectorIiEEiRiRfi
// partial: hinted search, then the interpolation-type gate, then the clamped fraction.
template <>
bool SAnimationAccessor::findKeyFrameNoEx<int, 1000>(int index, const glitch::res::vector<int>& keys, int time,
                                                     int& outIndex, float& fraction, int hint) const {
    const bool keyFrame = findKeyFrameNo<int, 1000>(keys, time, outIndex, hint);
    const bool found = getInterpolationType(index) != 0 && keyFrame;
    if (found) {
        storeFraction<int>(keysData(keys), time, outIndex, fraction);
    }
    return found;
}

// 0x66aab4 _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExIhLi30EEEbiRKNS_3res6vectorIiEEiRiRfi
// partial: hinted 8-bit search, then the interpolation-type gate, then the clamped fraction.
template <>
bool SAnimationAccessor::findKeyFrameNoEx<std::uint8_t, 30>(int index, const glitch::res::vector<int>& keys,
                                                            int time, int& outIndex, float& fraction,
                                                            int hint) const {
    const bool keyFrame = findKeyFrameNo<std::uint8_t, 30>(keys, time, outIndex, hint);
    const bool found = getInterpolationType(index) != 0 && keyFrame;
    if (found) {
        storeFraction<std::uint8_t>(keysData(keys), time, outIndex, fraction);
    }
    return found;
}

// 0x66b550 _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExItLi30EEEbiRKNS_3res6vectorIiEEiRiRfi
// partial: hinted 16-bit search, then the interpolation-type gate, then the clamped fraction.
template <>
bool SAnimationAccessor::findKeyFrameNoEx<std::uint16_t, 30>(int index, const glitch::res::vector<int>& keys,
                                                             int time, int& outIndex, float& fraction,
                                                             int hint) const {
    const bool keyFrame = findKeyFrameNo<std::uint16_t, 30>(keys, time, outIndex, hint);
    const bool found = getInterpolationType(index) != 0 && keyFrame;
    if (found) {
        storeFraction<std::uint16_t>(keysData(keys), time, outIndex, fraction);
    }
    return found;
}

// 0x66b65c _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoEiRKNS_3res6vectorIiEEiRiRfi
// partial: dispatch on the time-internal type of channel 0, through the key-search cache at +0x08.
// When the cache is enabled (byte +0x0d) and its key id (+0x04) differs from time, the cache is refreshed
// with the search for the type (3 -> 16-bit, 4 -> int, 1 -> 8-bit); the cache fields are then read back.
// The cache is guest memory, so the reads and writes go through the seam (placeholder: reads return 0).
bool SAnimationAccessor::findKeyFrameNo(int index, const glitch::res::vector<int>& keys, int time,
                                        int& outIndex, float& fraction, int hint) const {
    const std::uint32_t cache = this->cache;
    if (guestU8(cache + kCacheEnabled) == 0) {
        // Cache disabled: the byte at +0x0d is 0 here, so the type is read at index 0.
        switch (getTimeInternalType(0)) {
            case 3:
                return findKeyFrameNoEx<std::uint16_t, 30>(index, keys, time, outIndex, fraction, hint);
            case 4:
                return findKeyFrameNoEx<int, 1000>(index, keys, time, outIndex, fraction, hint);
            case 1:
                return findKeyFrameNoEx<std::uint8_t, 30>(index, keys, time, outIndex, fraction, hint);
            default:
                return false;
        }
    }

    if (static_cast<int>(guestU32(cache + kCacheKeyId)) != time) {
        guestWriteU32(cache + kCacheKeyId, static_cast<std::uint32_t>(time));
        const int type = getTimeInternalType(0);
        if (type == 3 || type == 4 || type == 1) {
            int cachedIndex = static_cast<int>(guestU32(cache + kCacheIndex));
            float cachedFraction = guestF32(cache + kCacheFraction);
            bool found = false;
            if (type == 3) {
                found = findKeyFrameNoEx<std::uint16_t, 30>(index, keys, time, cachedIndex, cachedFraction, hint);
            } else if (type == 4) {
                found = findKeyFrameNoEx<int, 1000>(index, keys, time, cachedIndex, cachedFraction, hint);
            } else {
                found = findKeyFrameNoEx<std::uint8_t, 30>(index, keys, time, cachedIndex, cachedFraction, hint);
            }
            guestWriteU32(cache + kCacheIndex, static_cast<std::uint32_t>(cachedIndex));
            if (found) {
                guestWriteF32(cache + kCacheFraction, cachedFraction);
            }
            guestWriteU8(cache + kCacheFound, found ? 1 : 0);
        }
    }

    fraction = guestF32(cache + kCacheFraction);
    outIndex = static_cast<int>(guestU32(cache + kCacheIndex));
    return guestU8(cache + kCacheFound) != 0;
}

// 0x66b814 _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoEiiRiRfi
// partial: getKeyTime(index) yields the guest address of the key-time vector. A host
// glitch::res::vector<int> cannot be formed from that address (no guest memory map), so this
// returns not-found and leaves the outputs alone. The dispatcher (0x66b65c) is complete in form.
bool SAnimationAccessor::findKeyFrameNo(int index, int time, int& outIndex, float& fraction, int hint) const {
    const std::uint32_t keyTime = getKeyTime(index);
    (void)keyTime;  // placeholder: the key-time vector at this guest address is not mapped
    (void)time;
    (void)outIndex;
    (void)fraction;
    (void)hint;
    return false;
}

}  // namespace glitch::collada
