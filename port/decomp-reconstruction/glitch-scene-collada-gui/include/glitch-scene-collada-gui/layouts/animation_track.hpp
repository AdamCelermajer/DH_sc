// Member layouts for glitch::collada::SAnimationAccessor and the
// glitch::collada::animation_track applicator classes, from the Hex-Rays
// pseudocode in recovered/native/hexrays-all (32-bit ARM offsets).
// Declarations only: no function bodies. Pointers are stored as 32-bit guest
// words so the host layout matches the 32-bit ARM offsets. Offsets are confirmed by the
// accessor, clone and applicator bodies cited next to each member. Anything
// not cited is a pad or marked "type not confirmed".
#ifndef GLITCH_SCENE_COLLADA_GUI_LAYOUTS_ANIMATION_TRACK_HPP
#define GLITCH_SCENE_COLLADA_GUI_LAYOUTS_ANIMATION_TRACK_HPP

#include <cstddef>
#include <cstdint>

namespace glitch::res {
template <typename T>
class vector;
}  // namespace glitch::res

namespace glitch::collada {

// Pointee of SAnimationAccessor +0x00. The name is not confirmed. Its fields are
// read by SAnimationAccessor::getAnimator (0x66a004), getChannel (0x669e44),
// getOutput (0x669e24).
struct SAnimationAccessorTrack {
    std::uint8_t pad_0x00[8];       // +0x00..+0x07 not read by the accessor helpers
    std::uint32_t keyframes;        // +0x08 (guest 32-bit pointer) table of 28-byte entries. The uint32 at entry+0x18 is an output index (getOutput)
    std::uint8_t pad_0x0c[4];       // +0x0c not read by the accessor helpers
    std::uint32_t channels;         // +0x10 (guest 32-bit pointer) array of 16-byte entries (getChannel returns base + 16 * i)
    std::uint32_t animator;         // +0x14 (guest 32-bit pointer) animator object, type not confirmed. getAnimator returns it. Its vtable is called at byte offsets 108 and 128 (SAnimationAccessor::applyValue 0x66a010 and 0x66a0c0)
};

// glitch::collada::SAnimationAccessor
struct SAnimationAccessor {
    std::uint32_t track;            // +0x00 guest pointer to SAnimationAccessorTrack (dereferenced by every accessor helper)
    std::uint32_t outputs;          // +0x04 (guest 32-bit pointer) base of the output table. getOutput(i) returns outputs + 8 * keyframes[i].outputIndex + 4 (0x669e24)

    std::uint32_t cache;            // +0x08 (guest 32-bit pointer) key-search cache record: float fraction +0x00, key id +0x04, index +0x08, found byte +0x0c, enabled byte +0x0d (src 0x66ad8c, used by findKeyFrameNo 0x66b65c). Same offset as the evidence view in layouts/animation_evidence.hpp

    // Accessor methods. Bodies are in src/tb_scene-collada-gui-glitch_collada_SAnimationAccessor-0.cpp.
    // Guest words are read through the guest memory seam there, so every body is partial.
    // Guest addresses are std::uint32_t; plain values are int, following the Hex-Rays return types.
    int getTarget() const;                                              // 0x669e00
    int getType(int index) const;                                       // 0x669e10
    std::uint32_t getOutput(int index) const;                           // 0x669e24
    std::uint32_t getChannel(int index) const;                          // 0x669e44
    bool hasDefaultValue() const;                                       // 0x669e54
    int getDefaultValue() const;                                        // 0x669e68
    int getChannelsCount() const;                                       // 0x669e78
    int getTimeInternalType(int index) const;                           // 0x669e84
    int getInterpolationType(int index) const;                          // 0x669e9c
    std::uint32_t getOffsets() const;                                   // 0x669eb4
    std::uint32_t getScales() const;                                    // 0x669ec4
    int getOffsetScaleType() const;                                     // 0x669ed4
    std::uint32_t getKeyTime(int index) const;                          // 0x669eec
    int getKeyTime(int index, int key) const;                           // 0x669f0c
    int getStart(int index) const;                                      // 0x669fac
    int getEnd(int index) const;                                        // 0x669fb4
    int getLength(int index) const;                                     // 0x669fdc
    std::uint32_t getAnimator() const;                                  // 0x66a004
    int getValue(int index, void* out, int& outValue, bool flag) const; // 0x66a1a8
    template <typename T, int N>
    bool findKeyFrameNoEx(int index, const glitch::res::vector<int>& keys, int time, int& outIndex) const;  // 0x66a1f0 (int); uchar 0x66a788, ushort 0x66ac78
    // Key-search family. The key vector is glitch::res::vector<int> in every symbol, whatever the
    // element width (the width is T). Bodies are in the accessor source (explicit specializations
    // there, and explicit instantiation for <int,1000> of the 4-arg findKeyFrameNoEx in its sibling file).
    template <typename T, int N>
    bool findKeyFrameNo(const glitch::res::vector<int>& keys, int time, int& outIndex) const;                       // 0x66a6d0 (uchar), 0x66abb8 (ushort)
    template <typename T, int N>
    bool findKeyFrameNo(const glitch::res::vector<int>& keys, int time, int& outIndex, int hint) const;            // 0x66a368 (int), 0x66a894 (uchar), 0x66b130 (ushort)
    template <typename T, int N>
    bool findKeyFrameNoEx(int index, const glitch::res::vector<int>& keys, int time, int& outIndex, float& fraction) const;          // 0x66a2ac (int), 0x66a7c4 (uchar), 0x66acb4 (ushort)
    template <typename T, int N>
    bool findKeyFrameNoEx(int index, const glitch::res::vector<int>& keys, int time, int& outIndex, float& fraction, int hint) const; // 0x66a5e0 (int), 0x66aab4 (uchar), 0x66b550 (ushort)
    bool findKeyFrameNo(int index, int time, int& outIndex, float& fraction, int hint) const;                     // 0x66b814
    bool findKeyFrameNo(int index, const glitch::res::vector<int>& keys, int time, int& outIndex, float& fraction, int hint) const; // 0x66b65c (dispatcher on the cache at +0x08)
};

namespace animation_track {

// glitch::collada::animation_track::CApplicatorInfo. The destructor (0x667cf0)
// frees the object without touching members. setTarget (0x660df8) calls the
// vtable slot at +8 on the applicator and stores the returned object, which
// matches clone. The 16-bit
// field at +0x08 is read by CTextureTransformEx::applyValueEx (0x6e38c4) and passed
// as the parameter id.
struct CApplicatorInfo {
    std::uint32_t vptr;             // +0x00 guest vtable pointer (clone writes it)
    std::uint32_t word_0x04;        // +0x04 type not confirmed. Copied by CMaterialApplicatorInfo::clone (0x667ed0)
    std::uint16_t parameterId;      // +0x08 read as uint16 by CTextureTransformEx::applyValueEx (0x6e38c4)
    std::uint8_t pad_0x0a[2];       // +0x0a not read by the pseudocode
};

// glitch::collada::animation_track::CMaterialApplicatorInfo. clone (0x667ed0)
// allocates 0x0c bytes, writes the vtable at +0x00, and copies the words at
// +0x04 and +0x08. Its destructor (0x667d04) frees the object. Inheritance from
// CApplicatorInfo is not confirmed, so this is a separate declaration.
struct CMaterialApplicatorInfo {
    std::uint32_t vptr;             // +0x00 guest vtable pointer (clone writes it)
    std::uint32_t word_0x04;        // +0x04 type not confirmed (copied by clone)
    std::uint32_t word_0x08;        // +0x08 type not confirmed (copied by clone)
};

// glitch::collada::CAnimationTrackEx. Only the vtable pointer is touched. Its
// applyValue overloads (0x6e2a18, 0x6e2ad8, 0x6e2ce8, 0x6e2e90, 0x6e2f3c) dispatch
// through the vtable at byte offsets 64, 72, 88 and 92. No data members are read.
class CAnimationTrackEx {
public:
    std::uint32_t vptr;             // +0x00 guest vtable pointer
};

}  // namespace animation_track

// Layout checks against the offsets above. They fail the build if the members drift.
static_assert(offsetof(SAnimationAccessor, track) == 0x00, "SAnimationAccessor::track");
static_assert(offsetof(SAnimationAccessor, outputs) == 0x04, "SAnimationAccessor::outputs");
static_assert(offsetof(SAnimationAccessor, cache) == 0x08, "SAnimationAccessor::cache");
static_assert(offsetof(SAnimationAccessorTrack, keyframes) == 0x08, "track keyframes");
static_assert(offsetof(SAnimationAccessorTrack, channels) == 0x10, "track channels");
static_assert(offsetof(SAnimationAccessorTrack, animator) == 0x14, "track animator");
static_assert(offsetof(animation_track::CApplicatorInfo, parameterId) == 0x08, "CApplicatorInfo::parameterId");
static_assert(offsetof(animation_track::CMaterialApplicatorInfo, word_0x08) == 0x08, "CMaterialApplicatorInfo::word_0x08");
static_assert(sizeof(animation_track::CMaterialApplicatorInfo) == 0x0c, "CMaterialApplicatorInfo is 0x0c bytes (clone)");

}  // namespace glitch::collada

#endif  // GLITCH_SCENE_COLLADA_GUI_LAYOUTS_ANIMATION_TRACK_HPP
