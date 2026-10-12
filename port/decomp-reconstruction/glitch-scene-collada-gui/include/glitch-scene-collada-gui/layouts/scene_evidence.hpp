// Evidence-based member layouts for glitch::scene classes, recovered from the
// Hex-Rays bodies in the decomp export (32-bit ARM target).
//
// Every named member carries: +0xNN offset, access width, and src=0xADDR, the
// body address that proves the access. Gaps are pad arrays marked "not
// confirmed". A name is used only where the body shows the access; method names
// alone were not used. Irrlicht naming was not applied: no Irrlicht header was
// checked against these offsets.
//
// Hex-Rays prints `this + N` as bytes when `this` is an int, and as word
// indexes when it is a _DWORD pointer. All offsets below are byte offsets from
// the object start, already converted.
//
// Virtual-slot calls are listed per class in the comment block as
// "slot N (+OFF) at SRC": N is the vtable word index (OFF / 4).

#ifndef GLITCH_SCENE_COLLADA_GUI_LAYOUTS_SCENE_EVIDENCE_HPP
#define GLITCH_SCENE_COLLADA_GUI_LAYOUTS_SCENE_EVIDENCE_HPP

#include <cstddef>
#include <cstdint>

namespace glitch::scene::evidence {

// ---------------------------------------------------------------------------
// CBatchMesh (complete type, no base region). Size 0x7C: operator new(0x7C) in
// the CBatchSceneNode ctor at 0x57f9c0.
// Vector: begin +0x20 and end +0x24 (count = (end - begin) / 20).
// Batch entry stride is 20 bytes (getMeshBuffer and getMaterial index 20 * i).
// Batch entry: buffer ptr +0x00 (ref at +4 of the buffer), material ptr +0x04
// (ref at +0 of the material). Source: 0x578c50 and 0x578ccc.
// Virtual-slot calls: none in the bodies listed.
// ---------------------------------------------------------------------------
struct CBatchMesh {
    std::uint32_t vptr;                 // +0x00 width=4 src=0x5789e4 (ctor stores off_974818; dtor 0x57a59c rewrites it)
    std::uint32_t ref_count;            // +0x04 width=4 src=0x57f9c0 (++v8[1] after new CBatchMesh); src=0x57e594 (getMesh ++*(v+4))
    std::uint32_t owned_block_0x08;     // +0x08 width=4 src=0x57a59c (GlitchFree in dtor); zeroed by ctor 0x5789e4
    std::uint8_t pad_0x0c[0x04];        // not confirmed: zeroed by ctor 0x5789e4, no read in the listed bodies
    std::uint8_t pad_0x10[0x04];        // not confirmed: zeroed by ctor 0x5789e4, no read in the listed bodies
    std::uint32_t owned_block_0x14;     // +0x14 width=4 src=0x57a59c (GlitchFree in dtor)
    std::uint8_t pad_0x18[0x08];        // not confirmed: zeroed by ctor 0x5789e4
    std::uint32_t batch_begin;          // +0x20 width=4 src=0x578c28 (count), src=0x578c50 (getMeshBuffer reads begin); vector<SBatch> dtor at 0x57a59c
    std::uint32_t batch_end;            // +0x24 width=4 src=0x578c28 (count = (end - begin) / 20)
    std::uint8_t pad_0x28[0x04];        // not confirmed: vector capacity word, not read in the listed bodies
    std::uint32_t owned_block_0x2c;     // +0x2c width=4 src=0x57a59c (GlitchFree in dtor, read as this+11 words)
    std::uint8_t pad_0x30[0x08];        // not confirmed: zeroed by ctor 0x5789e4
    std::uint8_t pad_0x38[0x18];        // not confirmed: ctor 0x5789e4 writes -1.0f x3 at +0x38 and +1.0f x3 at +0x44; no read seen
    float bbox_min[3];                  // +0x50 width=4 float src=0x578c78 (setBoundingBox stores a2[0..2]); src=0x57aa8c (getBoundingBox returns this+80)
    float bbox_max[3];                  // +0x5c width=4 float src=0x578c78 (setBoundingBox stores a2[3..5])
    std::uint32_t bbox_stamp_valid;     // +0x68 width=4 src=0x57aa8c (nonzero test before the TickCount compare); zeroed by ctor 0x5789e4
    std::uint32_t bbox_tick;            // +0x6c width=4 src=0x578c78 (setBoundingBox stores TickCount); ctor 0x5789e4 writes -1
    std::uint32_t ctor_uint_arg;        // +0x70 width=4 src=0x578aa4 (ctor(unsigned) stores a2); ctor 0x5789e4 default 44
    std::uint8_t bbox_dirty;            // +0x74 width=1 src=0x57aa8c (forces updateBoundingBox); ctor 0x5789e4 sets 1
    std::uint8_t pad_0x75[0x03];        // not confirmed
    std::uint8_t pad_0x78[0x04];        // not confirmed: zeroed by ctor 0x5789e4
};
static_assert(offsetof(CBatchMesh, vptr) == 0x00);
static_assert(offsetof(CBatchMesh, ref_count) == 0x04);
static_assert(offsetof(CBatchMesh, owned_block_0x08) == 0x08);
static_assert(offsetof(CBatchMesh, owned_block_0x14) == 0x14);
static_assert(offsetof(CBatchMesh, batch_begin) == 0x20);
static_assert(offsetof(CBatchMesh, batch_end) == 0x24);
static_assert(offsetof(CBatchMesh, owned_block_0x2c) == 0x2c);
static_assert(offsetof(CBatchMesh, bbox_min) == 0x50);
static_assert(offsetof(CBatchMesh, bbox_max) == 0x5c);
static_assert(offsetof(CBatchMesh, bbox_stamp_valid) == 0x68);
static_assert(offsetof(CBatchMesh, bbox_tick) == 0x6c);
static_assert(offsetof(CBatchMesh, ctor_uint_arg) == 0x70);
static_assert(offsetof(CBatchMesh, bbox_dirty) == 0x74);
static_assert(sizeof(CBatchMesh) == 0x7C);

// ---------------------------------------------------------------------------
// CBatchSceneNode. Base region (ISceneNode and the virtual bases, reached
// through vbase-offset writes in the ctor) is below +0x130 and is not mapped
// here. Derived members start at +0x130.
// Virtual-slot calls, all on the mesh object at +0x130 (typed IMesh):
//   slot 4 (+0x10) getMaterialCount   at 0x57e554
//   slot 6 (+0x18) getMaterial        at 0x57e570
//   slot 9 (+0x24) getBoundingBox     at 0x57e538
// ---------------------------------------------------------------------------
struct CBatchSceneNode {
    std::uint8_t pad_0x00[0x130];       // not confirmed: base region (ISceneNode and virtual bases)
    std::uint32_t mesh;                 // +0x130 width=4 src=0x57f9c0 (ctor stores new CBatchMesh, drops old); src=0x57e594 (getMesh ++ref); src=0x57e538/0x57e554/0x57e570 (vcalls)
    std::uint8_t pad_0x134[0x04];       // not confirmed: zeroed by ctor 0x57f9c0
    std::uint8_t pad_0x138[0x04];       // not confirmed: ctor 0x57f9c0 writes 2
    std::uint8_t pad_0x13c[0x04];       // not confirmed: zeroed by ctor 0x57f9c0
    std::uint8_t pad_0x140[0x04];       // not confirmed: zeroed by ctor 0x57f9c0
    std::uint8_t pad_0x144[0x04];       // not confirmed: ctor 0x57f9c0 writes -1
    std::uint8_t pad_0x148[0x04];       // not confirmed: ctor 0x57f9c0 writes byte 1 at +0x148
    std::uint32_t cache_ref;            // +0x14c width=4 src=0x57f3ec (dtor drops it); zeroed by ctor 0x57f9c0
    std::uint8_t visible_cache_on;      // +0x150 width=1 src=0x57ed0c (setVisibleIndexCache reads and writes it); ctor 0x57f9c0 sets 1
    std::uint8_t pad_0x151[0x03];       // not confirmed
    std::uint32_t visible_cache_buf;    // +0x154 width=4 src=0x57ed0c (operator delete[] when the cache turns off, then zeroed); zeroed by ctor 0x57f9c0
    std::uint32_t visible_entries;      // +0x158 width=4 src=0x57f3ec (dtor delete[]); src=0x57e650 (updateInfo, 20-byte entries: count +0x00, match +0x04, tick +0x08, flags +0x0c)
    std::uint8_t pad_0x15c[0x04];       // not confirmed: ctor 0x57f9c0 writes -1
    std::uint32_t vptr_secondary;       // +0x160 width=4 src=0x57f3ec (dtor writes off_97499C)
};
static_assert(offsetof(CBatchSceneNode, mesh) == 0x130);
static_assert(offsetof(CBatchSceneNode, cache_ref) == 0x14c);
static_assert(offsetof(CBatchSceneNode, visible_cache_on) == 0x150);
static_assert(offsetof(CBatchSceneNode, visible_cache_buf) == 0x154);
static_assert(offsetof(CBatchSceneNode, visible_entries) == 0x158);
static_assert(offsetof(CBatchSceneNode, vptr_secondary) == 0x160);

// ---------------------------------------------------------------------------
// CBillboardSceneNode. Base region (IBillboardSceneNode and ISceneNode) is
// mapped only by its vptr at +0x00. The ctor writes +0x04 from a sub-object
// table and that is not confirmed.
// Colors: setColor (src 0x580b44) writes the four color words; getColor (src
// 0x580bec) returns +0x178 and +0x19c. The ctor writes a8 to 0x178 and 0x1c0,
// and a7 to 0x19c and 0x1e4. Pairing into named colors is not confirmed.
// Virtual-slot calls:
//   slot 30 (+0x78) on the IVideoDriver arg at 0x581c80 (vertex stream allocate)
//   slot 9  (+0x24) on the object at +0x114 at 0x580a40 (onRegisterSceneNode);
//                   the object's type is not confirmed
// ---------------------------------------------------------------------------
struct CBillboardSceneNode {
    std::uint32_t vptr;                 // +0x00 width=4 src=0x581c80 (ctor stores *a2)
    std::uint8_t pad_0x04[0x130];       // not confirmed: +0x04 sub-object word (src 0x581c80) and base region
    float size_w;                       // +0x134 width=4 float src=0x580aa0 (setSize); read as dimension2d by getSize src=0x580b3c
    float size_h;                       // +0x138 width=4 float src=0x580aa0 (setSize; zero replaced by 1.0)
    float bbox_min[3];                  // +0x13c width=4 float src=0x580aa0 (setSize stores -(w+h)/6 with the sign bit flipped)
    float bbox_max[3];                  // +0x148 width=4 float src=0x580aa0 (setSize stores +(w+h)/6)
    std::uint32_t material;             // +0x154 width=4 src=0x580e68 (setMaterial swaps in a ref); read src=0x580a40
    std::uint8_t pad_0x158[0x0C];       // not confirmed: zeroed by ctor 0x581c80 (part of the 144-byte block passed to slot 30)
    std::uint8_t pad_0x164[0x08];       // not confirmed: ctor 0x581c80 writes 1.0f at +0x164 and +0x168
    std::uint8_t pad_0x16c[0x0C];       // not confirmed
    std::uint32_t color_0x178;          // +0x178 width=4 src=0x580b44 (setColor), src=0x580bec (getColor), ctor 0x581c80 writes a8
    std::uint8_t pad_0x17c[0x20];       // not confirmed: ctor writes 1.0f at +0x188 and 0 at +0x18c
    std::uint32_t color_0x19c;          // +0x19c width=4 src=0x580b44 (setColor), src=0x580bec (getColor), ctor 0x581c80 writes a7
    std::uint8_t pad_0x1a0[0x14];       // not confirmed: ctor writes 0 at +0x1ac and 1.0f at +0x1b0
    std::uint8_t pad_0x1b4[0x0C];       // not confirmed
    std::uint32_t color_0x1c0;          // +0x1c0 width=4 src=0x580b44 (setColor), ctor 0x581c80 writes a8 (bytes 448..451)
    std::uint8_t pad_0x1c4[0x0C];       // not confirmed: ctor writes 0 at +0x1d0 and +0x1d4
    std::uint8_t pad_0x1d0[0x14];       // not confirmed
    std::uint32_t color_0x1e4;          // +0x1e4 width=4 src=0x580b44 (setColor), ctor 0x581c80 writes a7 (bytes 484..487)
    std::uint32_t vertex_streams;       // +0x1e8 width=4 src=0x581c80 (CVertexStreams::allocate result, ref-counted and swapped in)
};
static_assert(offsetof(CBillboardSceneNode, vptr) == 0x00);
static_assert(offsetof(CBillboardSceneNode, size_w) == 0x134);
static_assert(offsetof(CBillboardSceneNode, size_h) == 0x138);
static_assert(offsetof(CBillboardSceneNode, bbox_min) == 0x13c);
static_assert(offsetof(CBillboardSceneNode, bbox_max) == 0x148);
static_assert(offsetof(CBillboardSceneNode, material) == 0x154);
static_assert(offsetof(CBillboardSceneNode, color_0x178) == 0x178);
static_assert(offsetof(CBillboardSceneNode, color_0x19c) == 0x19c);
static_assert(offsetof(CBillboardSceneNode, color_0x1c0) == 0x1c0);
static_assert(offsetof(CBillboardSceneNode, color_0x1e4) == 0x1e4);
static_assert(offsetof(CBillboardSceneNode, vertex_streams) == 0x1e8);

// ---------------------------------------------------------------------------
// CCameraSceneNode. ICameraSceneNode and CCameraSceneNode share the +0x130
// vptr slot (ICameraSceneNode writes off_95FC28, CCameraSceneNode writes
// off_975580). Base region is below +0x130.
// Virtual-slot calls (on the animator/receiver elements of the list at +0xfc):
//   slot 7 (+0x1c) and slot 8 (+0x20) at 0x581f68 (onEvent)
// ---------------------------------------------------------------------------
struct CCameraSceneNode {
    std::uint8_t pad_0x00[0x130];       // not confirmed: base region (ISceneNode, ICameraSceneNode base words)
    std::uint32_t vptr;                 // +0x130 width=4 src=0x583878 (CCameraSceneNode ctor stores off_975580); src=0x582b18 (ICameraSceneNode ctor)
    std::uint8_t icam_flag;             // +0x134 width=1 src=0x582b18 (ICameraSceneNode ctor writes 0)
    std::uint8_t pad_0x135[0x03];       // not confirmed
    float copied_vec3_0x138[3];         // +0x138 width=4 float src=0x583878 (ctor arg a5 copied as three words to +0x138..+0x140); role not confirmed
    std::uint8_t pad_0x144[0x04];       // not confirmed: ctor writes 0
    std::uint8_t pad_0x148[0x04];       // not confirmed: ctor writes 1.0f
    std::uint8_t pad_0x14c[0x04];       // not confirmed: ctor writes 0
    std::uint8_t pad_0x150[0x04];       // not confirmed: ctor writes a float (bit pattern 1067506044)
    std::uint8_t pad_0x154[0x04];       // not confirmed: ctor writes a float (bit pattern 1068149419)
    std::uint8_t pad_0x158[0x04];       // not confirmed: ctor writes 1.0f
    std::uint8_t pad_0x15c[0x04];       // not confirmed: ctor writes a float (bit pattern 1161527296)
    std::uint8_t pad_0x160[0x04];       // not confirmed
    std::uint8_t ctor_flag_0x164;       // +0x164 width=1 src=0x583878 (ctor arg a6 stored)
    std::uint8_t input_receiver_enabled; // +0x165 width=1 src=0x581f40 (setter writes); src=0x581f48 (getter reads); src=0x581f68 (onEvent returns 0 when clear)
    std::uint8_t pad_0x166[0x02];       // not confirmed
    // SViewFrustum starts at +0x168 (ctor call at 0x583878). Its size is not confirmed,
    // so the struct ends here.
};
static_assert(offsetof(CCameraSceneNode, vptr) == 0x130);
static_assert(offsetof(CCameraSceneNode, icam_flag) == 0x134);
static_assert(offsetof(CCameraSceneNode, copied_vec3_0x138) == 0x138);
static_assert(offsetof(CCameraSceneNode, ctor_flag_0x164) == 0x164);
static_assert(offsetof(CCameraSceneNode, input_receiver_enabled) == 0x165);

// ---------------------------------------------------------------------------
// CEmptySceneNode. The ctor (0x5839d8) writes the two vptrs and a box.
// Two vptrs: offset 0x00 (final value off_9758EC) and offset 0x148 (final value
// off_9759F0, after the ISceneNode base ctor).
// ---------------------------------------------------------------------------
struct CEmptySceneNode {
    std::uint32_t vptr;                 // +0x00 width=4 src=0x5839d8 (ctor writes off_975B5C, then off_9758EC)
    std::uint8_t pad_0x04[0x12C];       // not confirmed: base region (ISceneNode)
    float bbox_min[3];                  // +0x130 width=4 float src=0x5839d8 (ctor writes -1.0f x3)
    float bbox_max[3];                  // +0x13c width=4 float src=0x5839d8 (ctor writes +1.0f x3)
    std::uint32_t vptr_secondary;       // +0x148 width=4 src=0x5839d8 (ctor writes off_95B720, then off_9759F0)
    std::uint8_t pad_0x14c[0x04];       // not confirmed: ctor writes int 1 (a flag)
};
static_assert(offsetof(CEmptySceneNode, vptr) == 0x00);
static_assert(offsetof(CEmptySceneNode, bbox_min) == 0x130);
static_assert(offsetof(CEmptySceneNode, bbox_max) == 0x13c);
static_assert(offsetof(CEmptySceneNode, vptr_secondary) == 0x148);

// ---------------------------------------------------------------------------
// CDummyTransformationSceneNode. Base region is below +0x130.
// The ctor (0x6bb940) clears 0x40 bytes at +0x130 and writes 1.0f/-1.0f floats
// around the flag. These float values are not named here.
// ---------------------------------------------------------------------------
struct CDummyTransformationSceneNode {
    std::uint8_t pad_0x00[0x130];       // not confirmed: base region (IDummyTransformationSceneNode, ISceneNode)
    std::uint8_t zero_block_0x130[0x40]; // not confirmed: cleared by ctor 0x6bb940 (memset 0x40 bytes at +0x130), then float writes 1.0f and -1.0f
    std::uint8_t setup_flag_0x170;      // +0x170 width=1 src=0x6bb940 (set 0 before the memset, set 1 after)
};
static_assert(offsetof(CDummyTransformationSceneNode, setup_flag_0x170) == 0x170);

}  // namespace glitch::scene::evidence

#endif  // GLITCH_SCENE_COLLADA_GUI_LAYOUTS_SCENE_EVIDENCE_HPP
