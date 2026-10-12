// Track B batch e1-glitch-scene-collada-gui-glitch_collada_SAnimationAccessor-0.
// glitch::collada::SAnimationAccessor accessors (part 1 of 2), from the Hex-Rays pseudocode.
// Declarations are in glitch-scene-collada-gui/layouts/animation_track.hpp.
//
// Every body reads guest 32-bit words (track record, channel, keyframe and output tables, the
// animator). The image is not mapped in this tree yet, so those reads go through the guest
// seam below, which returns zero. Every row is therefore partial: the control flow matches the
// pseudocode, and the missing piece is the guest memory map. findKeyFrameNoEx also needs the
// glitch::res::vector<int> layout, which is not in the tree (only forward-declared).

#include <cstdint>

#include "glitch-scene-collada-gui/layouts/animation_track.hpp"

namespace glitch::res {
template <typename T>
class vector;
}  // namespace glitch::res

namespace glitch::collada {

namespace {

// Guest memory seam. Placeholder until the guest image map exists. Returns zero.
std::uint32_t guestU32(std::uint32_t address) {
    (void)address;
    return 0;  // placeholder: guest word read (no guest memory map in tree)
}

std::uint16_t guestU16(std::uint32_t address) {
    (void)address;
    return 0;  // placeholder: guest halfword read
}

std::uint8_t guestU8(std::uint32_t address) {
    (void)address;
    return 0;  // placeholder: guest byte read
}

// glitch::res::vector<int> element access. The layout is not in the tree. Placeholder.
template <typename T>
int vectorCount(const glitch::res::vector<T>& keys) {
    (void)keys;
    return 0;  // placeholder: element count (vector layout not mapped)
}

template <typename T>
T vectorAt(const glitch::res::vector<T>& keys, int index) {
    (void)keys;
    (void)index;
    return T{};  // placeholder: element read (vector layout not mapped)
}

inline std::uint32_t unsignedIndex(int index) {
    return static_cast<std::uint32_t>(index);
}

}  // namespace

// 0x669e00 _ZNK6glitch7collada18SAnimationAccessor9getTargetEv
// partial: reads the channel table pointer at track +0x10, then the word at channel +0x04.
int SAnimationAccessor::getTarget() const {
    const std::uint32_t channels = guestU32(track + 0x10);
    return static_cast<int>(guestU32(channels + 4));
}

// 0x669e10 _ZNK6glitch7collada18SAnimationAccessor7getTypeEi
// partial: channel table at track +0x10, 16-byte entries, word at entry +0x08.
int SAnimationAccessor::getType(int index) const {
    const std::uint32_t channels = guestU32(track + 0x10);
    return static_cast<int>(guestU32(channels + 16u * unsignedIndex(index) + 8u));
}

// 0x669e24 _ZNK6glitch7collada18SAnimationAccessor9getOutputEi
// partial: outputs + 8 * keyframe[index].outputIndex + 4. The keyframe table is at track +0x08,
// 28-byte entries, output index at entry +0x18.
std::uint32_t SAnimationAccessor::getOutput(int index) const {
    const std::uint32_t keyframes = guestU32(track + 0x08);
    const std::uint32_t outputIndex = guestU32(keyframes + 28u * unsignedIndex(index) + 24u);
    return outputs + 8u * outputIndex + 4u;
}

// 0x669e44 _ZNK6glitch7collada18SAnimationAccessor10getChannelEi
// partial: channel table at track +0x10 plus 16 * index. Returns the guest address.
std::uint32_t SAnimationAccessor::getChannel(int index) const {
    const std::uint32_t channels = guestU32(track + 0x10);
    return channels + 16u * unsignedIndex(index);
}

// 0x669e54 _ZNK6glitch7collada18SAnimationAccessor15hasDefaultValueEv
// partial: non-zero default-value pointer at track +0x18.
bool SAnimationAccessor::hasDefaultValue() const {
    return guestU32(track + 0x18) != 0;
}

// 0x669e68 _ZNK6glitch7collada18SAnimationAccessor15getDefaultValueEv
// partial: word at +0x08 of the default-value record at track +0x18.
int SAnimationAccessor::getDefaultValue() const {
    const std::uint32_t defaultValue = guestU32(track + 0x18);
    return static_cast<int>(guestU32(defaultValue + 8));
}

// 0x669e78 _ZNK6glitch7collada18SAnimationAccessor16getChannelsCountEv
// partial: channel count word at track +0x0c.
int SAnimationAccessor::getChannelsCount() const {
    return static_cast<int>(guestU32(track + 0x0c));
}

// 0x669e84 _ZNK6glitch7collada18SAnimationAccessor19getTimeInternalTypeEi
// partial: keyframe table at track +0x08, time type word at entry +0x04 (28-byte entries).
int SAnimationAccessor::getTimeInternalType(int index) const {
    const std::uint32_t keyframes = guestU32(track + 0x08);
    return static_cast<int>(guestU32(keyframes + 28u * unsignedIndex(index) + 4u));
}

// 0x669e9c _ZNK6glitch7collada18SAnimationAccessor20getInterpolationTypeEi
// partial: interpolation type word at keyframe entry +0x00.
int SAnimationAccessor::getInterpolationType(int index) const {
    const std::uint32_t keyframes = guestU32(track + 0x08);
    return static_cast<int>(guestU32(keyframes + 28u * unsignedIndex(index)));
}

// 0x669eb4 _ZNK6glitch7collada18SAnimationAccessor10getOffsetsEv
// partial: word at +0x08 of the scale/offset record at track +0x1c. Returned as a guest word.
std::uint32_t SAnimationAccessor::getOffsets() const {
    const std::uint32_t record = guestU32(track + 0x1c);
    return guestU32(record + 8);
}

// 0x669ec4 _ZNK6glitch7collada18SAnimationAccessor9getScalesEv
// partial: word at +0x04 of the scale/offset record at track +0x1c. Returned as a guest word.
std::uint32_t SAnimationAccessor::getScales() const {
    const std::uint32_t record = guestU32(track + 0x1c);
    return guestU32(record + 4);
}

// 0x669ed4 _ZNK6glitch7collada18SAnimationAccessor18getOffsetScaleTypeEv
// partial: when the record at track +0x1c is set, returns the word at its +0x00; otherwise 2.
int SAnimationAccessor::getOffsetScaleType() const {
    const std::uint32_t record = guestU32(track + 0x1c);
    if (record != 0) {
        return static_cast<int>(guestU32(record));
    }
    return 2;
}

// 0x669eec _ZNK6glitch7collada18SAnimationAccessor10getKeyTimeEi
// partial: outputs + 8 * keyframe[index].timeIndex + 4. Returns the guest address of the key-time
// record. The keyframe time index is at entry +0x0c.
std::uint32_t SAnimationAccessor::getKeyTime(int index) const {
    const std::uint32_t keyframes = guestU32(track + 0x08);
    const std::uint32_t timeIndex = guestU32(keyframes + 28u * unsignedIndex(index) + 12u);
    return outputs + 8u * timeIndex + 4u;
}

// 0x669f0c _ZNK6glitch7collada18SAnimationAccessor10getKeyTimeEii
// partial: time type comes from getTimeInternalType(0). Type 3 reads a 16-bit key and type 1 an
// 8-bit key, both scaled by 33.333332. Type 4 reads a 32-bit key unscaled. Other types return 0.
int SAnimationAccessor::getKeyTime(int index, int key) const {
    const int timeInternalType = getTimeInternalType(0);
    if (timeInternalType == 3) {
        const std::uint32_t data = guestU32(getKeyTime(index) + 4);
        const int value = static_cast<int>(guestU16(data + 2u * unsignedIndex(key)));
        return static_cast<int>(static_cast<double>(value) * 33.333332);
    }
    if (timeInternalType != 4) {
        if (timeInternalType != 1) {
            return 0;
        }
        const std::uint32_t data = guestU32(getKeyTime(index) + 4);
        const int value = static_cast<int>(guestU8(data + unsignedIndex(key)));
        return static_cast<int>(static_cast<double>(value) * 33.333332);
    }
    const std::uint32_t data = guestU32(getKeyTime(index) + 4);
    return static_cast<int>(guestU32(data + 4u * unsignedIndex(key)));
}

// 0x669fac _ZNK6glitch7collada18SAnimationAccessor8getStartEi
// partial: getKeyTime(index, 0). Inherits the guest-read dependency.
int SAnimationAccessor::getStart(int index) const {
    return getKeyTime(index, 0);
}

// 0x669fb4 _ZNK6glitch7collada18SAnimationAccessor6getEndEi
// partial: the last key index is the count word at the key-time record minus one.
int SAnimationAccessor::getEnd(int index) const {
    const int count = static_cast<int>(guestU32(getKeyTime(index)));
    return getKeyTime(index, count - 1);
}

// 0x669fdc _ZNK6glitch7collada18SAnimationAccessor9getLengthEi
// partial: getEnd(index) - getStart(index).
int SAnimationAccessor::getLength(int index) const {
    const int end = getEnd(index);
    return end - getStart(index);
}

// 0x66a004 _ZNK6glitch7collada18SAnimationAccessor11getAnimatorEv
// partial: animator word at track +0x14. Returned as a guest address.
std::uint32_t SAnimationAccessor::getAnimator() const {
    return guestU32(track + 0x14);
}

// 0x66a1a8 _ZNK6glitch7collada18SAnimationAccessor8getValueEiPvRib
// partial: Hex-Rays calls the guest vtable slot at +104 of the animator, passing the accessor,
// the index, out, the out value and the flag. A guest function cannot be called from this tree.
// The body returns 0 as a placeholder for that call.
int SAnimationAccessor::getValue(int index, void* out, int& outValue, bool flag) const {
    (void)index;
    (void)out;
    (void)outValue;
    (void)flag;
    return 0;  // placeholder for the guest vtable slot +104 call on the animator
}

// 0x66a1f0 _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoExIiLi1000EEEbiRKNS_3res6vectorIiEEiRi
// partial: binary search over the key vector for the time (compared as float). The index is
// written to outIndex. The result is true when the channel interpolation type is non-zero and the
// time is strictly inside the vector. The vector layout is not mapped (see the seam above).
template <typename T, int N>
bool SAnimationAccessor::findKeyFrameNoEx(int index, const glitch::res::vector<T>& keys, int time,
                                          int& outIndex) const {
    const float timeValue = static_cast<float>(time);
    const int count = vectorCount(keys);
    int high = count - 1;
    if (high > 0) {
        int low = 1;
        do {
            const int mid = (low + high) >> 1;
            if (timeValue >= static_cast<float>(vectorAt(keys, mid))) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        } while (low <= high);
    }
    outIndex = high;
    const bool notAtEdge = timeValue != static_cast<float>(vectorAt(keys, high)) && high != count - 1;
    return getInterpolationType(index) != 0 && notAtEdge;
}

// Explicit instantiation for the only instantiation in this batch (findKeyFrameNoEx<int, 1000>).
template bool SAnimationAccessor::findKeyFrameNoEx<int, 1000>(int, const glitch::res::vector<int>&,
                                                              int, int&) const;

}  // namespace glitch::collada
