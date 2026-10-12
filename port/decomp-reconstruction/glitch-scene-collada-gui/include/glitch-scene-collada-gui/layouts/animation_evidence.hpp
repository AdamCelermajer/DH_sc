// Layout evidence for glitch::collada animation classes.
//
// Source: Hex-Rays export (recovered/native/hexrays-all), ARM32 bodies.
// Every member carries the body address that proves it. Offsets are from the
// start of the object named by the struct. Where a base class is not mapped
// (CSceneNodeAnimatorSet, ISceneNodeAnimator), the struct covers only the
// offsets written by the most-derived ctor/dtor and makes no claim about the
// base size. Gaps are padded and marked "not confirmed".
//
// No Irrlicht naming is used: none of these offsets were checked against
// Irrlicht layouts.
//
// Classes with no usable member evidence (no struct emitted):
//   - CInterpreter<...>::getKeyBasedValueEx (0x6121c0, 0x6122f4): stateless;
//     reads only the accessor and args.
//   - CVirtualEx<CApplyValueEx<...>> thunks (0x60ed10, 0x60eea0, 0x60ef6c):
//     empty dtors, constant getValueSize, retrieveValue with no member access.
//   - CApplicatorInfo dtor (0x667cf0): free only.
//   - CAnimationStreamingManager::cache (0x60bc7c) and the Blender accessors
//     were not read; the streaming manager tail is padded.

#ifndef GLITCH_COLLADA_GUI_ANIMATION_EVIDENCE_HPP
#define GLITCH_COLLADA_GUI_ANIMATION_EVIDENCE_HPP

#include <cstddef>
#include <cstdint>

namespace glitch::collada::evidence {

// ---------------------------------------------------------------------------
// CAnimationBlock. Ctor variants 0x60c05c, 0x60c610, 0x60c7a8, 0x60c940 all
// write the same field set. Dtor 0x60bbd8 and drop 0x60bb1c read it.
// Total size is 0x20: the last named member is at +0x1c.
// ---------------------------------------------------------------------------
struct CAnimationBlock {
    std::uint32_t refcount;     // +0x00 width=4 src=0x60bb1c (drop: decremented; free when it reaches 1)
    std::uint32_t db_record;    // +0x04 width=4 src=0x60c05c (copy of CColladaDatabase word 0; its +0x04 refcount is bumped)
    std::uint32_t db_factory;   // +0x08 width=4 src=0x60c05c (copy of CColladaDatabase word 1)
    std::uint32_t range_ptr;    // +0x0c width=4 src=0x60c610 (ctor arg a3; when 0 it becomes db clip +24)
    std::uint32_t segment_ptr;  // +0x10 width=4 src=0x60c05c (getAnimationSegment result; segment +0x00 count, +0x04 index, +0x08 users; src 0x60c31c, 0x60c414)
    std::uint32_t data_holder;  // +0x14 width=4 src=0x60bbd8 (SAnimationDataHolder*; ctor assigns it at src 0x60c05c)
    std::uint32_t next_block;   // +0x18 width=4 src=0x60c31c (prepareNextBlock: next block, or self when there is none)
    std::uint32_t prev_block;   // +0x1c width=4 src=0x60c31c (back link; the dtor clears the peer's +0x18 at src 0x60bbd8)
};

static_assert(offsetof(CAnimationBlock, refcount) == 0x00);
static_assert(offsetof(CAnimationBlock, db_record) == 0x04);
static_assert(offsetof(CAnimationBlock, db_factory) == 0x08);
static_assert(offsetof(CAnimationBlock, range_ptr) == 0x0c);
static_assert(offsetof(CAnimationBlock, segment_ptr) == 0x10);
static_assert(offsetof(CAnimationBlock, data_holder) == 0x14);
static_assert(offsetof(CAnimationBlock, next_block) == 0x18);
static_assert(offsetof(CAnimationBlock, prev_block) == 0x1c);

// Refcounted buffer held at CAnimationBlock +0x14. Released in src 0x60bbd8.
struct SAnimationDataHolder {
    std::uint32_t refcount;     // +0x00 width=4 src=0x60bbd8 (decremented; buffer freed at 1)
    std::uint8_t pad_0x04[8];   // not confirmed
    std::uint32_t buffer;       // +0x0c width=4 src=0x60bbd8 (operator delete[] target)
};

static_assert(offsetof(SAnimationDataHolder, refcount) == 0x00);
static_assert(offsetof(SAnimationDataHolder, buffer) == 0x0c);

// ---------------------------------------------------------------------------
// CAnimationStreamingManager. Singleton stored in CAnimationStreamingManager::Instance
// by ctor 0x60b3cc (which zeroes words 0x00..0x1c). Blocks are kept in a sorted
// vector of 4-byte CAnimationBlock* (lower_bound with CAnimationBlockSearchCompare).
// ---------------------------------------------------------------------------
struct CAnimationStreamingManager {
    std::uint32_t blocks_begin; // +0x00 width=4 src=0x60c414 (vector<CAnimationBlock*> begin)
    std::uint32_t blocks_end;   // +0x04 width=4 src=0x60c414 (vector end)
    std::uint32_t blocks_cap;   // +0x08 width=4 src=0x60c414 (vector capacity end; growth is 2x)
    std::uint8_t pad_0x0c[0x14]; // not confirmed (zeroed by ctor 0x60b3cc through +0x1c; cache list not read)
};

static_assert(offsetof(CAnimationStreamingManager, blocks_begin) == 0x00);
static_assert(offsetof(CAnimationStreamingManager, blocks_end) == 0x04);
static_assert(offsetof(CAnimationStreamingManager, blocks_cap) == 0x08);

// ---------------------------------------------------------------------------
// COnDemandReader. Dtor 0x60b2b4 is empty and 0x60b644 frees the object.
// read() 0x60b2b8 dispatches through the object at +0x04.
// ---------------------------------------------------------------------------
struct COnDemandReader {
    std::uint8_t pad_0x00[4];   // not confirmed (no vptr store in the bodies read)
    std::uint32_t source;       // +0x04 width=4 src=0x60b2b8 (stream object; its vtable slots 3 and 6 are called)
};

static_assert(offsetof(COnDemandReader, source) == 0x04);

// ---------------------------------------------------------------------------
// SAnimationAccessor. This is the accessor passed by value to the track calls.
// ---------------------------------------------------------------------------
struct SAnimationAccessor {
    std::uint32_t channel;      // +0x00 width=4 src=0x66a004 (channel object; getAnimator reads channel +0x14)
    std::uint32_t key_base;     // +0x04 width=4 src=0x669e24 (base of 8-byte key/output records: base + 8*index + 4)
    std::uint32_t cache;        // +0x08 width=4 src=0x66ad8c (SAnimationKeyCache*; used when its enabled byte is nonzero)
};

static_assert(offsetof(SAnimationAccessor, channel) == 0x00);
static_assert(offsetof(SAnimationAccessor, key_base) == 0x04);
static_assert(offsetof(SAnimationAccessor, cache) == 0x08);

// Channel object reached through SAnimationAccessor +0x00.
struct SAnimationChannelRef {
    std::uint8_t pad_0x00[8];   // not confirmed
    std::uint32_t samplers;     // +0x08 width=4 src=0x669e84 (SSamplerEntry table, 28-byte stride)
    std::uint8_t pad_0x0c[8];   // not confirmed
    std::uint32_t animator;     // +0x14 width=4 src=0x66a004 (receiver of the vtable calls in CAnimationTrackEx's accessor helpers)
};

static_assert(offsetof(SAnimationChannelRef, samplers) == 0x08);
static_assert(offsetof(SAnimationChannelRef, animator) == 0x14);

// Sampler entry, 28-byte stride, at samplers + 28 * i (src 0x669e84, 0x669eec, 0x669e24).
// time_type selects the key search: 1 -> <unsigned char,30>, 3 -> <unsigned short,30>,
// 4 -> <int,1000> (src 0x66ad8c).
struct SSamplerEntry {
    std::uint8_t pad_0x00[4];   // not confirmed
    std::uint32_t time_type;    // +0x04 width=4 src=0x669e84 (values 1, 3, 4 seen in src 0x66ad8c)
    std::uint8_t pad_0x08[4];   // not confirmed
    std::uint32_t key_index;    // +0x0c width=4 src=0x669eec (index into SAnimationAccessor key_base records)
    std::uint8_t pad_0x10[8];   // not confirmed
    std::uint32_t output_index; // +0x18 width=4 src=0x669e24 (index into key_base records for output values)
};

static_assert(offsetof(SSamplerEntry, time_type) == 0x04);
static_assert(offsetof(SSamplerEntry, key_index) == 0x0c);
static_assert(offsetof(SSamplerEntry, output_index) == 0x18);

// Key-search cache at SAnimationAccessor +0x08 (src 0x66ad8c).
struct SAnimationKeyCache {
    float time;                 // +0x00 width=4 src=0x66ad8c (returned as *a6)
    std::uint32_t key_id;       // +0x04 width=4 src=0x66ad8c (compared with a4; recomputed when it differs)
    std::uint32_t index;        // +0x08 width=4 src=0x66ad8c (returned as *a5)
    std::uint8_t found;         // +0x0c width=1 src=0x66ad8c (return value of findKeyFrameNoEx)
    std::uint8_t enabled;       // +0x0d width=1 src=0x66ad8c (cache is used only when nonzero)
};

static_assert(offsetof(SAnimationKeyCache, time) == 0x00);
static_assert(offsetof(SAnimationKeyCache, key_id) == 0x04);
static_assert(offsetof(SAnimationKeyCache, index) == 0x08);
static_assert(offsetof(SAnimationKeyCache, found) == 0x0c);
static_assert(offsetof(SAnimationKeyCache, enabled) == 0x0d);

// ---------------------------------------------------------------------------
// CAnimationTrackEx. Only the vptr is read directly; every other access goes
// through vtable slots (see kVtableSlotCalls).
// ---------------------------------------------------------------------------
struct CAnimationTrackEx {
    std::uint32_t vptr;         // +0x00 width=4 src=0x6e2a18 (every body loads *this to dispatch)
};

static_assert(offsetof(CAnimationTrackEx, vptr) == 0x00);

// ---------------------------------------------------------------------------
// CApplicator<SLightColor>::applyValueEx (0x60dfc0). Writes four floats into the
// output object. Source bytes a2[0..2] are divided by 255. comp3 is the constant 1.0f.
// ---------------------------------------------------------------------------
struct CApplicatorLightColorOut {
    std::uint8_t pad_0x00[0x14]; // not confirmed
    float comp0;                 // +0x14 width=4 src=0x60dfc0 (a2[0] / 255)
    float comp1;                 // +0x18 width=4 src=0x60dfc0 (a2[1] / 255)
    float comp2;                 // +0x1c width=4 src=0x60dfc0 (a2[2] / 255)
    float comp3;                 // +0x20 width=4 src=0x60dfc0 (constant 1.0f, bits 0x3f800000)
};

static_assert(offsetof(CApplicatorLightColorOut, comp0) == 0x14);
static_assert(offsetof(CApplicatorLightColorOut, comp1) == 0x18);
static_assert(offsetof(CApplicatorLightColorOut, comp2) == 0x1c);
static_assert(offsetof(CApplicatorLightColorOut, comp3) == 0x20);

// ---------------------------------------------------------------------------
// CSceneNodeAnimatorSynchronizedSet. Ctor 0x662d10, dtor 0x662b60. Base
// CSceneNodeAnimatorSet is not mapped. The offsets below are those written by
// the ctor/dtor, measured from the most-derived object.
// ---------------------------------------------------------------------------
struct CSceneNodeAnimatorSynchronizedSet {
    std::uint32_t vptr_primary;     // +0x00 width=4 src=0x662d10 (dtor 0x662b60 restores it)
    std::uint32_t vptr_base_1;      // +0x04 width=4 src=0x662d10
    std::uint8_t pad_0x08[0x50];    // not confirmed
    std::uint32_t vptr_base_2;      // +0x58 width=4 src=0x662d10 (dtor 0x662b60 resets it to a second value at the end)
    std::uint32_t self_ptr;         // +0x5c width=4 src=0x662d10 (stores the object's own address)
    std::uint32_t sync_begin;       // +0x60 width=4 src=0x662d10 (zeroed; dtor 0x662b60 destroys the vector here)
    std::uint32_t sync_end;         // +0x64 width=4 src=0x662d10
    std::uint32_t sync_cap;         // +0x68 width=4 src=0x662d10
    std::uint32_t name_count;       // +0x6c width=4 src=0x662d10 ((end-begin)/24 of the vector<std::string> argument)
    std::uint32_t names_begin;      // +0x70 width=4 src=0x662d10 (vector<std::string> copy; dtor 0x662b60 destroys it)
    std::uint32_t names_end;        // +0x74 width=4 src=0x662d10
    std::uint32_t names_cap;        // +0x78 width=4 src=0x662d10
    std::uint8_t pad_0x7c[4];       // not confirmed (zeroed by ctor 0x662d10)
    std::uint8_t flag_0x80;         // +0x80 width=1 src=0x662d10 (written 0; meaning not mapped)
    std::uint8_t pad_0x81[3];       // not confirmed
    std::uint32_t vptr_base_3;      // +0x84 width=4 src=0x662d10 (dtor 0x662b60 restores it)
    std::uint32_t init_one_0x88;    // +0x88 width=4 src=0x662d10 (written 1 before the base ctor call; base may overwrite)
};

static_assert(offsetof(CSceneNodeAnimatorSynchronizedSet, vptr_primary) == 0x00);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedSet, vptr_base_1) == 0x04);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedSet, vptr_base_2) == 0x58);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedSet, self_ptr) == 0x5c);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedSet, sync_begin) == 0x60);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedSet, sync_end) == 0x64);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedSet, sync_cap) == 0x68);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedSet, name_count) == 0x6c);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedSet, names_begin) == 0x70);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedSet, names_end) == 0x74);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedSet, names_cap) == 0x78);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedSet, flag_0x80) == 0x80);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedSet, vptr_base_3) == 0x84);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedSet, init_one_0x88) == 0x88);

// ---------------------------------------------------------------------------
// CSceneNodeAnimatorSynchronizedBlender. Ctor 0x661b74 (zeroes most words and
// stores vptr-like words), dtor 0x66186c (frees the buffers below). Base
// ISceneNodeAnimator is not mapped. Offsets are measured from the most-derived object.
// ---------------------------------------------------------------------------
struct CSceneNodeAnimatorSynchronizedBlender {
    std::uint32_t vptr_primary;     // +0x00 width=4 src=0x661b74 (dtor 0x66186c restores it)
    std::uint32_t vptr_base_1;      // +0x04 width=4 src=0x661b74 (dtor 0x66186c restores it)
    std::uint8_t pad_0x08[0x1c];    // not confirmed
    std::uint32_t vptr_base_2;      // +0x24 width=4 src=0x661b74 (dtor 0x66186c restores it)
    std::uint32_t self_ptr;         // +0x28 width=4 src=0x661b74 (stores the object's own address)
    std::uint8_t pad_0x2c[4];       // not confirmed
    std::uint32_t seg_begin;        // +0x30 width=4 src=0x66186c (word array begin; freed in dtor; elements dropped through element+4)
    std::uint32_t seg_end;          // +0x34 width=4 src=0x66186c (word array end)
    std::uint8_t pad_0x38[4];       // not confirmed (zeroed by ctor 0x661b74)
    std::uint32_t buf_0x3c;         // +0x3c width=4 src=0x66186c (zeroed in ctor; GlitchFree in dtor)
    std::uint8_t pad_0x40[8];       // not confirmed
    std::uint32_t buf_0x48;         // +0x48 width=4 src=0x66186c (GlitchFree in dtor)
    std::uint8_t pad_0x4c[8];       // not confirmed
    std::uint32_t buf_0x54;         // +0x54 width=4 src=0x66186c (GlitchFree in dtor)
    std::uint8_t pad_0x58[8];       // not confirmed
    std::uint32_t buf_0x60;         // +0x60 width=4 src=0x66186c (GlitchFree in dtor)
    std::uint8_t pad_0x64[8];       // not confirmed
    std::uint32_t buf_0x6c;         // +0x6c width=4 src=0x66186c (GlitchFree in dtor)
    std::uint8_t pad_0x70[8];       // not confirmed
    std::uint32_t owned_0x78;       // +0x78 width=4 src=0x66186c (object deleted through vtable slot 2; NOT written by ctor 0x661b74)
    std::uint32_t vecvec_begin;     // +0x7c width=4 src=0x66186c (vector<vector<SynchronizationData>> begin; dtor ~vector at this+124)
    std::uint32_t vecvec_end;       // +0x80 width=4 src=0x66186c
    std::uint32_t vecvec_cap;       // +0x84 width=4 src=0x66186c
    std::uint32_t buf_0x88;         // +0x88 width=4 src=0x66186c (GlitchFree in dtor)
    std::uint8_t pad_0x8c[8];       // not confirmed
    std::uint32_t buf_0x94;         // +0x94 width=4 src=0x66186c (GlitchFree in dtor)
    std::uint8_t pad_0x98[8];       // not confirmed
    std::uint32_t buf_0xa0;         // +0xa0 width=4 src=0x66186c (GlitchFree in dtor)
    std::uint8_t pad_0xa4[12];      // not confirmed (zeroed by ctor 0x661b74 through +0xa8)
    std::uint8_t flag_0xb0;         // +0xb0 width=1 src=0x661b74 (written 0)
    std::uint8_t pad_0xb1[2];       // not confirmed (a 16-bit 1 is written at +0xb1, src 0x661b74)
    std::uint8_t pad_0xb3[1];       // not confirmed
    std::uint32_t vptr_base_3;      // +0xb4 width=4 src=0x661b74 (first set to a generic vtable, then to the final value; dtor restores)
    std::uint32_t init_one_0xb8;    // +0xb8 width=4 src=0x661b74 (written 1 before the base ctor call)
};

static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, vptr_primary) == 0x00);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, vptr_base_1) == 0x04);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, vptr_base_2) == 0x24);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, self_ptr) == 0x28);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, seg_begin) == 0x30);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, seg_end) == 0x34);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, buf_0x3c) == 0x3c);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, buf_0x48) == 0x48);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, buf_0x54) == 0x54);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, buf_0x60) == 0x60);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, buf_0x6c) == 0x6c);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, owned_0x78) == 0x78);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, vecvec_begin) == 0x7c);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, vecvec_end) == 0x80);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, vecvec_cap) == 0x84);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, buf_0x88) == 0x88);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, buf_0x94) == 0x94);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, buf_0xa0) == 0xa0);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, flag_0xb0) == 0xb0);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, vptr_base_3) == 0xb4);
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, init_one_0xb8) == 0xb8);

// ---------------------------------------------------------------------------
// Vtable slot calls. The slot is the vptr byte offset divided by 4.
// "receiver" is the object whose vtable is dispatched through.
// ---------------------------------------------------------------------------
struct VtableSlotCall {
    const char* caller;         // body that makes the call (or the body that defines the slot)
    std::uint32_t slot;         // vtable index (byte offset / 4)
    std::uint32_t src;          // body address that proves the call
    const char* note;           // receiver, arguments, meaning
};

inline constexpr VtableSlotCall kVtableSlotCalls[] = {
    {"CAnimationTrackEx::retrieveValue", 2, 0x6e29d4,
     "receiver *this (+0x08); return value is the byte count passed to memcpy"},
    {"CAnimationTrackEx::getValue (variant A)", 14, 0x6e2b8c,
     "receiver *this (+0x38); keyed path with accessor, key, value out"},
    {"CAnimationTrackEx::getValue (variant A)", 15, 0x6e2b8c,
     "receiver *this (+0x3c); non-keyed path"},
    {"CAnimationTrackEx::applyValue (variant B)", 16, 0x6e2ad8,
     "receiver *this (+0x40); keyed path, args (accessor, keyIdx, keyIdx+1, time, out, applicator, scale)"},
    {"CAnimationTrackEx::applyValue (variant B)", 18, 0x6e2ad8,
     "receiver *this (+0x48); non-keyed path"},
    {"CAnimationTrackEx::applyValue (variant C)", 17, 0x6e2ce8,
     "receiver *this (+0x44); keyed path"},
    {"CAnimationTrackEx::applyValue (variant C)", 19, 0x6e2ce8,
     "receiver *this (+0x4c); non-keyed path"},
    {"CAnimationTrackEx::applyValue (keyed)", 22, 0x6e2a18,
     "receiver *this (+0x58); args (accessor, keyIdx, keyIdx+1, time, out, applicator, scale)"},
    {"CAnimationTrackEx::applyValue (non-keyed)", 23, 0x6e2a18,
     "receiver *this (+0x5c); args (accessor, keyIdx, out, applicator, scale)"},
    {"SAnimationAccessor::applyValue", 32, 0x66a010,
     "receiver is the animator at SAnimationChannelRef +0x14 (+0x80 on that animator)"},
    {"SAnimationAccessor::getValue", 29, 0x66a15c,
     "receiver is the animator at SAnimationChannelRef +0x14 (+0x74 on that animator)"},
    {"COnDemandReader::read(int,int,void*)", 3, 0x60b2b8,
     "receiver COnDemandReader +0x04 (+0x0c); args (source, a4, a2)"},
    {"COnDemandReader::read(int,int,void*)", 6, 0x60b2b8,
     "receiver COnDemandReader +0x04 (+0x18); args (source, a3, 0)"},
    {"CVirtualEx<CApplyValueEx<vector3d<float>,CSceneNodeScaleMixin<float>>>::retrieveValue", 36, 0x60ef6c,
     "receiver argument a2 (+0x90); returns a float[3] pointer, which is copied as 12 bytes"},
    {"CVirtualEx<CApplyValueEx<vector3d<float>,CLightColorMixin<uchar>>>::getValueSize", 2, 0x60eea0,
     "defines a constant 3; slot number not verified for this vtable"},
    {"CSceneNodeAnimatorSynchronizedBlender::~CSceneNodeAnimatorSynchronizedBlender", 2, 0x66186c,
     "receiver is owned_0x78 (+0x08), deleted through its vtable"},
};

}  // namespace glitch::collada::evidence

#endif  // GLITCH_COLLADA_GUI_ANIMATION_EVIDENCE_HPP
