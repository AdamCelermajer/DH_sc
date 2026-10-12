// Member declarations for classes that partial rows in ledger.csv need before
// their bodies can be rebuilt (rows noted "needs member declaration in
// layouts/animation_track.hpp"). Declarations only: no methods, no bodies.
//
// Offsets come from the constructor, destructor and accessor pseudocode of the
// 32-bit ARM build (tools/decomp_extract.py). Pointers are stored as 32-bit guest
// words (std::uint32_t). Each comment gives the real qualified name of the class.
// Several classes below are in glitch::collada, not glitch::collada::animation_track;
// they are declared here in the namespace the recovery plan asked for.
//
// Unnamed gaps are pads marked "not confirmed". Byte offsets, not word indexes:
// Hex-Rays "*((_DWORD *)this + N)" is +0x(4*N).
#ifndef GLITCH_SCENE_COLLADA_GUI_LAYOUTS_ANIMATION_TRACK_MEMBERS_HPP
#define GLITCH_SCENE_COLLADA_GUI_LAYOUTS_ANIMATION_TRACK_MEMBERS_HPP

#include <cstddef>
#include <cstdint>

namespace glitch::collada::animation_track {

// glitch::collada::IAnimationSetTemplate. The destructor (0x667588, 0x66762c)
// writes the vtable at +0x00, walks the vector at +0x04..+0x08 and frees the
// buffer at +0x04 with releaseProcessBuffer.
struct IAnimationSetTemplate {
    std::uint32_t vptr;             // +0x00 guest vtable pointer (dtor writes it)
    std::uint32_t targets_begin;    // +0x04 vector begin. Each element is a pointer to a record whose +0x0c buffer is freed by the dtor
    std::uint32_t targets_end;      // +0x08 vector end. The dtor sets it back to begin when it clears the vector
};

// glitch::collada::ISceneNodeAnimator. Destructor (0x669738, 0x6697c4) writes
// vptrs at +0x00, +0x04 and +0x24 and drops the reference-counted object at +0x18.
// onBind (0x669538) and forceUnbindAll (0x667e40) use the bound node at +0x10.
struct ISceneNodeAnimator {
    std::uint32_t vptr;             // +0x00 guest vtable pointer
    std::uint32_t vptr_0x04;        // +0x04 secondary base vtable pointer (dtor writes it)
    std::uint8_t pad_0x08[8];       // +0x08..+0x0f not confirmed
    std::uint32_t bound_node;       // +0x10 scene node pointer (onBind stores it, forceUnbindAll clears it)
    std::uint8_t pad_0x14[4];       // +0x14..+0x17 not confirmed
    std::uint32_t ref_counted;      // +0x18 IReferenceCounted pointer, dropped by the dtor
    std::uint8_t pad_0x1c[8];       // +0x1c..+0x23 not confirmed
    std::uint32_t vptr_0x24;        // +0x24 secondary base vtable pointer (dtor writes it)
};

// glitch::collada::CAnimationSetTransformationTemplate. Both constructors (0x6e264c,
// 0x6e2830) allocate 0x10 bytes. The vector at +0x04..+0x0c holds 80-byte targets.
struct CAnimationSetTransformationTemplate {
    std::uint32_t vptr;             // +0x00 guest vtable pointer (ctor writes off_98B228)
    std::uint32_t targets_begin;    // +0x04 vector begin (ctor zeroes it)
    std::uint32_t targets_end;      // +0x08 vector end (ctor zeroes it)
    std::uint32_t targets_cap;      // +0x0c vector capacity (ctor zeroes it)
};

// glitch::collada::CAnimationSet. Destructor (0x65f2ac) writes the vtable at +0x00,
// calls remAnimationTemplate, frees the buffers at +0x0c, +0x18, +0x30, +0x40,
// +0x4c and +0x58, and destroys the vector<CColladaDatabase> at +0x24.
// addAnimationTemplate (0x6600a4, 0x6600d0) stores the new template at +0x64.
struct CAnimationSet {
    std::uint32_t vptr;             // +0x00 guest vtable pointer
    std::uint8_t pad_0x04[8];       // +0x04..+0x0b not confirmed
    std::uint32_t buffer_0x0c;      // +0x0c buffer freed by the dtor, type not confirmed
    std::uint8_t pad_0x10[8];       // +0x10..+0x17 not confirmed
    std::uint32_t buffer_0x18;      // +0x18 buffer freed by the dtor, type not confirmed
    std::uint8_t pad_0x1c[8];       // +0x1c..+0x23 not confirmed
    std::uint32_t databases_begin;  // +0x24 vector<glitch::collada::CColladaDatabase> begin
    std::uint32_t databases_end;    // +0x28 vector<glitch::collada::CColladaDatabase> end
    std::uint32_t databases_cap;    // +0x2c vector<glitch::collada::CColladaDatabase> capacity
    std::uint32_t buffer_0x30;      // +0x30 buffer freed by the dtor, type not confirmed
    std::uint8_t pad_0x34[12];      // +0x34..+0x3f not confirmed
    std::uint32_t buffer_0x40;      // +0x40 buffer freed by the dtor, type not confirmed
    std::uint8_t pad_0x44[8];       // +0x44..+0x4b not confirmed
    std::uint32_t buffer_0x4c;      // +0x4c buffer freed by the dtor, type not confirmed
    std::uint8_t pad_0x50[8];       // +0x50..+0x57 not confirmed
    std::uint32_t buffer_0x58;      // +0x58 buffer freed by the dtor, type not confirmed
    std::uint8_t pad_0x5c[8];       // +0x5c..+0x63 not confirmed
    std::uint32_t transform_template; // +0x64 CAnimationSetTransformationTemplate pointer (addAnimationTemplate stores it)
};

// glitch::collada::CSceneNodeAnimatorSet. Constructors write vtables at +0x00, +0x04,
// +0x58 and +0x84 and store self at +0x5c. The destructor (0x65fa08, 0x65fb2c)
// drops the animation set at +0x24, frees the buffers at +0x28, +0x40 and the
// vector at +0x34..+0x38, and releases the block at +0x54.
// The +0x24 slot is also the secondary base vtable of ISceneNodeAnimator (see the
// dtor); this conflict is not resolved here.
// The +0x60..+0x84 fields are written by the base constructor and used by
// CSceneNodeAnimatorSynchronizedSet (its destructor destroys +0x60 and +0x70).
struct CSceneNodeAnimatorSet {
    std::uint32_t vptr;             // +0x00 guest vtable pointer
    std::uint32_t vptr_0x04;        // +0x04 secondary base vtable pointer
    std::uint8_t pad_0x08[0x1c];    // +0x08..+0x23 ISceneNodeAnimator base area, not confirmed
    std::uint32_t animation_set;    // +0x24 intrusive pointer to glitch::collada::CAnimationSet. getBindURI, getAnimationData and getAnimationVisualSceneID read it; the dtor drops it
    std::uint32_t buffer_0x28;      // +0x28 buffer freed by the dtor, type not confirmed
    std::uint8_t pad_0x2c[8];       // +0x2c..+0x33 not confirmed
    std::uint32_t nodes_begin;      // +0x34 vector begin. Elements are pointers whose vtable +4 is called by the dtor
    std::uint32_t nodes_end;        // +0x38 vector end
    std::uint8_t pad_0x3c[4];       // +0x3c vector capacity, not confirmed
    std::uint32_t buffer_0x40;      // +0x40 buffer freed by the dtor, type not confirmed
    std::uint8_t pad_0x44[12];      // +0x44..+0x4f not confirmed
    std::int32_t current_animation; // +0x50 index passed to CAnimationSet::getDatabase
    std::uint32_t animation_block;  // +0x54 intrusive pointer to glitch::collada::CAnimationBlock (CAnimationStreamingManager result). Released by the dtor
    std::uint32_t vptr_0x58;        // +0x58 secondary base vtable pointer (ctor writes it)
    std::uint32_t self_ptr;         // +0x5c ctor stores this here
    std::uint32_t sync_data_begin;  // +0x60 vector<CSceneNodeAnimatorSynchronizedSet::SynchronizationData>, 32-byte entries (int +0x00, int +0x04, string +0x08). Destroyed by the synchronized set's dtor
    std::uint8_t pad_0x64[8];       // +0x64..+0x6b vector end and capacity, not confirmed
    std::uint32_t animation_count;  // +0x6c number of animation names (ctor: byte size of the name vector / 24)
    std::uint32_t names_begin;      // +0x70 vector of 24-byte entries (a const char* name at +0x14). Destroyed by the synchronized set's dtor
    std::uint8_t pad_0x74[8];       // +0x74..+0x7b vector end and capacity, not confirmed
    float sync_time;                // +0x7c synchronized time. computeSynchronizedAnimationValues writes it
    std::uint8_t wrapped_flag;      // +0x80 set to 1 by computeSynchronizedAnimationValues when time passes the length
    std::uint8_t pad_0x81[3];       // +0x81..+0x83 not confirmed
    std::uint32_t vptr_0x84;        // +0x84 secondary base vtable pointer (ctor writes it)
};

// glitch::collada::CSceneNodeAnimatorSynchronizedSet. Derives from CSceneNodeAnimatorSet
// (its constructor calls CSceneNodeAnimatorSet::CSceneNodeAnimatorSet). The fields it
// adds are the ones listed in the base above at +0x60..+0x84.
struct CSceneNodeAnimatorSynchronizedSet : CSceneNodeAnimatorSet {};

// glitch::collada::CSceneNodeAnimatorSynchronizedBlender. Constructor (0x661b74, 0x661c78)
// and destructor (0x66186c, 0x661a08) zero or free the members below. Its
// ISceneNodeAnimator base area (+0x08..+0x23) is not declared here.
struct CSceneNodeAnimatorSynchronizedBlender {
    std::uint32_t vptr;             // +0x00 guest vtable pointer (ctor writes off_98415C)
    std::uint32_t vptr_0x04;        // +0x04 secondary base vtable pointer (ctor writes off_98422C)
    std::uint8_t pad_0x08[0x1c];    // +0x08..+0x23 ISceneNodeAnimator base area, not confirmed
    std::uint32_t vptr_0x24;        // +0x24 secondary base vtable pointer (ctor writes off_984240)
    std::uint32_t self_ptr;         // +0x28 ctor stores this here (getLength and others read through it)
    std::uint8_t pad_0x2c[4];       // +0x2c..+0x2f not confirmed
    std::uint32_t targets_begin;    // +0x30 vector begin. Elements are pointers to records whose +0x04 is a reference-counted animator (dtor drops it)
    std::uint32_t targets_end;      // +0x34 vector end
    std::uint32_t targets_cap;      // +0x38 vector capacity (ctor zeroes it)
    std::uint32_t weights;          // +0x3c float array of per-target weights (getLength reads it; dtor frees it)
    std::uint8_t pad_0x40[8];       // +0x40..+0x47 zeroed by ctor and not freed by dtor, not confirmed
    std::uint32_t buffer_0x48;      // +0x48 buffer freed by the dtor, type not confirmed
    std::uint8_t pad_0x4c[8];       // +0x4c..+0x53 not confirmed
    std::uint32_t buffer_0x54;      // +0x54 buffer freed by the dtor, type not confirmed
    std::uint8_t pad_0x58[8];       // +0x58..+0x5f not confirmed
    std::uint32_t values_0x60;      // +0x60 int array written by setTarget at index a2 and freed by the dtor
    std::uint8_t pad_0x64[8];       // +0x64..+0x6b not confirmed
    std::uint32_t clones;           // +0x6c pointer array. setTarget stores the clone (vtable +8) of the applicator and releases the old entry (vtable +4)
    std::uint8_t pad_0x70[8];       // +0x70..+0x77 not confirmed
    std::uint32_t owned_object;     // +0x78 object freed through its vtable +8 by the dtor
    std::uint32_t sync_data_begin;  // +0x7c vector<vector<SynchronizationData>> begin (dtor destroys it)
    std::uint8_t pad_0x80[8];       // +0x80..+0x87 vector end and capacity, not confirmed
    std::uint32_t buffer_0x88;      // +0x88 buffer freed by the dtor, type not confirmed
    std::uint8_t pad_0x8c[8];       // +0x8c..+0x93 not confirmed
    std::uint32_t buffer_0x94;      // +0x94 buffer freed by the dtor, type not confirmed
    std::uint8_t pad_0x98[8];       // +0x98..+0x9f not confirmed
    std::uint32_t buffer_0xa0;      // +0xa0 buffer freed by the dtor, type not confirmed
    std::uint8_t pad_0xa4[12];      // +0xa4..+0xaf not confirmed
    std::uint8_t synchronized_flag; // +0xb0 zeroed by ctor
    std::uint8_t word_0xb1[2];      // +0xb1 16-bit value set to 1 by ctor (unaligned store)
    std::uint8_t pad_0xb3[1];       // +0xb3 not confirmed
    std::uint32_t vptr_0xb4;        // +0xb4 embedded base vtable pointer (ctor writes off_95B720, then off_984294)
    std::uint32_t count_0xb8;       // +0xb8 counter initialised to 1 by ctor
};

// glitch::collada::animation_track::CFloatEx. The vtable pointer at +0x00 is the only
// member read through this (applyValue calls vtable slot +8). The Ex bodies also pass
// r0 as a SAnimationAccessor, so no data member offsets from this are confirmed.
struct CFloatEx {
    std::uint32_t vptr;             // +0x00 guest vtable pointer
};

// glitch::collada::animation_track::CTextureTransformEx. Same status as CFloatEx: only
// the vtable pointer is read through this.
struct CTextureTransformEx {
    std::uint32_t vptr;             // +0x00 guest vtable pointer
};

// glitch::collada::animation_track::CVector3dEx. Same status as CFloatEx.
struct CVector3dEx {
    std::uint32_t vptr;             // +0x00 guest vtable pointer
};

// glitch::collada::animation_track::CVisibilityEx. Same status as CFloatEx.
struct CVisibilityEx {
    std::uint32_t vptr;             // +0x00 guest vtable pointer
};

// glitch::collada::animation_track::CWeightEx. Same status as CFloatEx.
struct CWeightEx {
    std::uint32_t vptr;             // +0x00 guest vtable pointer
};

// Layout checks. They fail the build if any named member drifts from its offset.
static_assert(offsetof(IAnimationSetTemplate, targets_begin) == 0x04, "IAnimationSetTemplate::targets_begin");
static_assert(offsetof(IAnimationSetTemplate, targets_end) == 0x08, "IAnimationSetTemplate::targets_end");

static_assert(offsetof(ISceneNodeAnimator, vptr_0x04) == 0x04, "ISceneNodeAnimator::vptr_0x04");
static_assert(offsetof(ISceneNodeAnimator, bound_node) == 0x10, "ISceneNodeAnimator::bound_node");
static_assert(offsetof(ISceneNodeAnimator, ref_counted) == 0x18, "ISceneNodeAnimator::ref_counted");
static_assert(offsetof(ISceneNodeAnimator, vptr_0x24) == 0x24, "ISceneNodeAnimator::vptr_0x24");

static_assert(offsetof(CAnimationSetTransformationTemplate, targets_begin) == 0x04, "CAnimationSetTransformationTemplate::targets_begin");
static_assert(offsetof(CAnimationSetTransformationTemplate, targets_end) == 0x08, "CAnimationSetTransformationTemplate::targets_end");
static_assert(offsetof(CAnimationSetTransformationTemplate, targets_cap) == 0x0c, "CAnimationSetTransformationTemplate::targets_cap");
static_assert(sizeof(CAnimationSetTransformationTemplate) == 0x10, "CAnimationSetTransformationTemplate is 0x10 bytes (ctor allocation)");

static_assert(offsetof(CAnimationSet, buffer_0x0c) == 0x0c, "CAnimationSet::buffer_0x0c");
static_assert(offsetof(CAnimationSet, buffer_0x18) == 0x18, "CAnimationSet::buffer_0x18");
static_assert(offsetof(CAnimationSet, databases_begin) == 0x24, "CAnimationSet::databases_begin");
static_assert(offsetof(CAnimationSet, databases_end) == 0x28, "CAnimationSet::databases_end");
static_assert(offsetof(CAnimationSet, databases_cap) == 0x2c, "CAnimationSet::databases_cap");
static_assert(offsetof(CAnimationSet, buffer_0x30) == 0x30, "CAnimationSet::buffer_0x30");
static_assert(offsetof(CAnimationSet, buffer_0x40) == 0x40, "CAnimationSet::buffer_0x40");
static_assert(offsetof(CAnimationSet, buffer_0x4c) == 0x4c, "CAnimationSet::buffer_0x4c");
static_assert(offsetof(CAnimationSet, buffer_0x58) == 0x58, "CAnimationSet::buffer_0x58");
static_assert(offsetof(CAnimationSet, transform_template) == 0x64, "CAnimationSet::transform_template");

static_assert(offsetof(CSceneNodeAnimatorSet, vptr_0x04) == 0x04, "CSceneNodeAnimatorSet::vptr_0x04");
static_assert(offsetof(CSceneNodeAnimatorSet, animation_set) == 0x24, "CSceneNodeAnimatorSet::animation_set");
static_assert(offsetof(CSceneNodeAnimatorSet, buffer_0x28) == 0x28, "CSceneNodeAnimatorSet::buffer_0x28");
static_assert(offsetof(CSceneNodeAnimatorSet, nodes_begin) == 0x34, "CSceneNodeAnimatorSet::nodes_begin");
static_assert(offsetof(CSceneNodeAnimatorSet, nodes_end) == 0x38, "CSceneNodeAnimatorSet::nodes_end");
static_assert(offsetof(CSceneNodeAnimatorSet, buffer_0x40) == 0x40, "CSceneNodeAnimatorSet::buffer_0x40");
static_assert(offsetof(CSceneNodeAnimatorSet, current_animation) == 0x50, "CSceneNodeAnimatorSet::current_animation");
static_assert(offsetof(CSceneNodeAnimatorSet, animation_block) == 0x54, "CSceneNodeAnimatorSet::animation_block");
static_assert(offsetof(CSceneNodeAnimatorSet, vptr_0x58) == 0x58, "CSceneNodeAnimatorSet::vptr_0x58");
static_assert(offsetof(CSceneNodeAnimatorSet, self_ptr) == 0x5c, "CSceneNodeAnimatorSet::self_ptr");
static_assert(offsetof(CSceneNodeAnimatorSet, sync_data_begin) == 0x60, "CSceneNodeAnimatorSet::sync_data_begin");
static_assert(offsetof(CSceneNodeAnimatorSet, animation_count) == 0x6c, "CSceneNodeAnimatorSet::animation_count");
static_assert(offsetof(CSceneNodeAnimatorSet, names_begin) == 0x70, "CSceneNodeAnimatorSet::names_begin");
static_assert(offsetof(CSceneNodeAnimatorSet, sync_time) == 0x7c, "CSceneNodeAnimatorSet::sync_time");
static_assert(offsetof(CSceneNodeAnimatorSet, wrapped_flag) == 0x80, "CSceneNodeAnimatorSet::wrapped_flag");
static_assert(offsetof(CSceneNodeAnimatorSet, vptr_0x84) == 0x84, "CSceneNodeAnimatorSet::vptr_0x84");

static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, vptr_0x04) == 0x04, "Blender::vptr_0x04");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, vptr_0x24) == 0x24, "Blender::vptr_0x24");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, self_ptr) == 0x28, "Blender::self_ptr");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, targets_begin) == 0x30, "Blender::targets_begin");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, targets_end) == 0x34, "Blender::targets_end");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, targets_cap) == 0x38, "Blender::targets_cap");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, weights) == 0x3c, "Blender::weights");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, buffer_0x48) == 0x48, "Blender::buffer_0x48");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, buffer_0x54) == 0x54, "Blender::buffer_0x54");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, values_0x60) == 0x60, "Blender::values_0x60");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, clones) == 0x6c, "Blender::clones");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, owned_object) == 0x78, "Blender::owned_object");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, sync_data_begin) == 0x7c, "Blender::sync_data_begin");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, buffer_0x88) == 0x88, "Blender::buffer_0x88");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, buffer_0x94) == 0x94, "Blender::buffer_0x94");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, buffer_0xa0) == 0xa0, "Blender::buffer_0xa0");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, synchronized_flag) == 0xb0, "Blender::synchronized_flag");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, vptr_0xb4) == 0xb4, "Blender::vptr_0xb4");
static_assert(offsetof(CSceneNodeAnimatorSynchronizedBlender, count_0xb8) == 0xb8, "Blender::count_0xb8");

static_assert(sizeof(CFloatEx) == 4, "CFloatEx holds only the vtable pointer");
static_assert(sizeof(CTextureTransformEx) == 4, "CTextureTransformEx holds only the vtable pointer");
static_assert(sizeof(CVector3dEx) == 4, "CVector3dEx holds only the vtable pointer");
static_assert(sizeof(CVisibilityEx) == 4, "CVisibilityEx holds only the vtable pointer");
static_assert(sizeof(CWeightEx) == 4, "CWeightEx holds only the vtable pointer");

}  // namespace glitch::collada::animation_track

#endif  // GLITCH_SCENE_COLLADA_GUI_LAYOUTS_ANIMATION_TRACK_MEMBERS_HPP
