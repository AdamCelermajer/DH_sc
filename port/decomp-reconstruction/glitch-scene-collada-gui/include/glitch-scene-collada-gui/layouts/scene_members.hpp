// Member declarations for glitch::scene classes that partial ledger rows need
// (ledger note: "needs member declaration in layouts/scene.hpp").
//
// Offsets come from the Hex-Rays pseudocode of the rows' bodies (ctor stores,
// dtor drops, and field reads). Hex-Rays prints `this + N` in decimal for
// word-indexed accesses, so those were converted to bytes before writing here.
// Pointer slots are 32-bit (the ARM target); they are declared as std::uint32_t
// so the offsets hold on any host.
//
// Gaps are pad arrays. Gaps inside a base class region are "not confirmed".
// Classes whose layout could not be determined from the bodies are not declared
// here (see the report that accompanies this file).

#ifndef GLITCH_SCENE_COLLADA_GUI_LAYOUTS_SCENE_MEMBERS_HPP
#define GLITCH_SCENE_COLLADA_GUI_LAYOUTS_SCENE_MEMBERS_HPP

#include <cstddef>
#include <cstdint>

namespace glitch::scene {

// ---------------------------------------------------------------------------
// CSceneNodeAnimatorDelete
// ---------------------------------------------------------------------------
struct CSceneNodeAnimatorDelete {
    std::uint8_t pad_0x00[0x0C];      // not confirmed: ISceneNodeAnimator base region
    std::uint32_t ctor_arg_0x0c;      // +0x0C: ctor arg (uint), passed back to createClone
    std::uint32_t scene_manager;      // +0x10: CSceneManager* (ctor arg), passed to createClone
};
static_assert(offsetof(CSceneNodeAnimatorDelete, ctor_arg_0x0c) == 0x0C);
static_assert(offsetof(CSceneNodeAnimatorDelete, scene_manager) == 0x10);

// ---------------------------------------------------------------------------
// CSceneNodeAnimatorRotation
// ---------------------------------------------------------------------------
struct CSceneNodeAnimatorRotation {
    std::uint8_t pad_0x00[0x0C];      // not confirmed: ISceneNodeAnimator base region
    float rotation[4];                // +0x0C: quaternion copied from the ctor arg (4 words)
    std::uint32_t start_time;         // +0x1C: ctor arg (uint), subtracted from the animate time
};
static_assert(offsetof(CSceneNodeAnimatorRotation, rotation) == 0x0C);
static_assert(offsetof(CSceneNodeAnimatorRotation, start_time) == 0x1C);

// ---------------------------------------------------------------------------
// CSceneNodeAnimatorTexture
// ---------------------------------------------------------------------------
struct CSceneNodeAnimatorTexture {
    std::uint8_t pad_0x00[0x0C];      // not confirmed: ISceneNodeAnimator base region
    std::uint32_t textures_begin;     // +0x0C: std::vector<intrusive_ptr<ITexture>> begin
    std::uint32_t textures_end;       // +0x10: std::vector<intrusive_ptr<ITexture>> end
    std::uint8_t pad_0x14[0x04];      // not confirmed: vector capacity word (dtor path)
    std::uint32_t period;             // +0x18: divisor in animateNode (time / period)
    std::uint32_t start_time;         // +0x1C: subtracted from the animate time
    std::uint8_t pad_0x20[0x04];      // not confirmed
    std::uint8_t loop;                // +0x24: bool flag read in animateNode and createClone
};
static_assert(offsetof(CSceneNodeAnimatorTexture, textures_begin) == 0x0C);
static_assert(offsetof(CSceneNodeAnimatorTexture, textures_end) == 0x10);
static_assert(offsetof(CSceneNodeAnimatorTexture, period) == 0x18);
static_assert(offsetof(CSceneNodeAnimatorTexture, start_time) == 0x1C);
static_assert(offsetof(CSceneNodeAnimatorTexture, loop) == 0x24);

// ---------------------------------------------------------------------------
// CSceneNodeAnimatorFlyCircle
// ---------------------------------------------------------------------------
struct CSceneNodeAnimatorFlyCircle {
    std::uint8_t pad_0x00[0x0C];      // not confirmed: ISceneNodeAnimator base region
    float center[3];                  // +0x0C: vector3d ctor arg (a3)
    float axis[3];                    // +0x18: vector3d ctor arg (a6), normalized in init()
    float derived_0x24[3];            // +0x24: vector3d written by init()
    float derived_0x30[3];            // +0x30: vector3d written by init()
    float ctor_arg_0x3c;              // +0x3C: float ctor arg (a4)
    float ctor_arg_0x40;              // +0x40: float ctor arg (a5), multiplies the time delta
    std::uint32_t start_time;         // +0x44: uint ctor arg (a2), subtracted from the animate time
};
static_assert(offsetof(CSceneNodeAnimatorFlyCircle, center) == 0x0C);
static_assert(offsetof(CSceneNodeAnimatorFlyCircle, axis) == 0x18);
static_assert(offsetof(CSceneNodeAnimatorFlyCircle, derived_0x24) == 0x24);
static_assert(offsetof(CSceneNodeAnimatorFlyCircle, derived_0x30) == 0x30);
static_assert(offsetof(CSceneNodeAnimatorFlyCircle, ctor_arg_0x3c) == 0x3C);
static_assert(offsetof(CSceneNodeAnimatorFlyCircle, ctor_arg_0x40) == 0x40);
static_assert(offsetof(CSceneNodeAnimatorFlyCircle, start_time) == 0x44);

// ---------------------------------------------------------------------------
// CSceneNodeAnimatorFlyStraight
// ---------------------------------------------------------------------------
struct CSceneNodeAnimatorFlyStraight {
    std::uint8_t pad_0x00[0x0C];      // not confirmed: ISceneNodeAnimator base region
    float start[3];                   // +0x0C: vector3d ctor arg (a2)
    float end[3];                     // +0x18: vector3d ctor arg (a3); third word computed in ctor
    std::uint8_t pad_0x24[0x14];      // +0x24..+0x37: derived values, zeroed in ctor (not confirmed)
    std::uint32_t start_time;         // +0x38: uint ctor arg (a6), subtracted from the animate time
    std::uint32_t ctor_arg_0x3c;      // +0x3C: uint ctor arg (a4)
    std::uint8_t loop;                // +0x40: bool ctor arg (a5)
};
static_assert(offsetof(CSceneNodeAnimatorFlyStraight, start) == 0x0C);
static_assert(offsetof(CSceneNodeAnimatorFlyStraight, end) == 0x18);
static_assert(offsetof(CSceneNodeAnimatorFlyStraight, start_time) == 0x38);
static_assert(offsetof(CSceneNodeAnimatorFlyStraight, ctor_arg_0x3c) == 0x3C);
static_assert(offsetof(CSceneNodeAnimatorFlyStraight, loop) == 0x40);

// ---------------------------------------------------------------------------
// CSceneNodeAnimatorFollowSpline
// ---------------------------------------------------------------------------
struct CSceneNodeAnimatorFollowSpline {
    std::uint8_t pad_0x00[0x0C];      // not confirmed: ISceneNodeAnimator base region
    std::uint32_t waypoints_begin;    // +0x0C: std::vector<vector3d<float>> begin (12-byte elements)
    std::uint32_t waypoints_end;      // +0x10: std::vector<vector3d<float>> end
    std::uint8_t pad_0x14[0x04];      // not confirmed: vector capacity word (dtor frees it)
    float speed;                      // +0x18: float ctor arg (a5), scales the time delta
    std::uint8_t pad_0x1c[0x04];      // not confirmed
    std::uint32_t start_time;         // +0x20: uint ctor arg (a2), subtracted from the animate time
};
static_assert(offsetof(CSceneNodeAnimatorFollowSpline, waypoints_begin) == 0x0C);
static_assert(offsetof(CSceneNodeAnimatorFollowSpline, waypoints_end) == 0x10);
static_assert(offsetof(CSceneNodeAnimatorFollowSpline, speed) == 0x18);
static_assert(offsetof(CSceneNodeAnimatorFollowSpline, start_time) == 0x20);

// ---------------------------------------------------------------------------
// CDefaultSceneNodeAnimatorFactory
// ---------------------------------------------------------------------------
struct CDefaultSceneNodeAnimatorFactory {
    std::uint8_t pad_0x00[0x0C];      // not confirmed: vptr and base region
    std::uint32_t refcounted_ref;     // +0x0C: IReferenceCounted*, dropped in the dtor
};
static_assert(offsetof(CDefaultSceneNodeAnimatorFactory, refcounted_ref) == 0x0C);

// ---------------------------------------------------------------------------
// CDefaultSceneNodeFactory
// ---------------------------------------------------------------------------
// Table entry; stride is 28 bytes (7 words) per the loop in getTypeFromName.
struct CDefaultSceneNodeFactoryEntry {
    std::uint32_t type;               // +0x00: E_SCENE_NODE_TYPE key (compared in getCreateable...)
    std::uint8_t pad_0x04[0x14];      // +0x04..+0x17: name string storage, size not confirmed
    std::uint32_t type_name;          // +0x18: value returned by the lookups
};
static_assert(offsetof(CDefaultSceneNodeFactoryEntry, type) == 0x00);
static_assert(offsetof(CDefaultSceneNodeFactoryEntry, type_name) == 0x18);
static_assert(sizeof(CDefaultSceneNodeFactoryEntry) == 0x1C);

struct CDefaultSceneNodeFactory {
    std::uint8_t pad_0x00[0x08];      // not confirmed: vptr and base region
    std::uint32_t entries_begin;      // +0x08: CDefaultSceneNodeFactoryEntry* begin
    std::uint32_t entries_end;        // +0x0C: CDefaultSceneNodeFactoryEntry* end
};
static_assert(offsetof(CDefaultSceneNodeFactory, entries_begin) == 0x08);
static_assert(offsetof(CDefaultSceneNodeFactory, entries_end) == 0x0C);

// ---------------------------------------------------------------------------
// CDummyTransformationSceneNode
// ---------------------------------------------------------------------------
struct CDummyTransformationSceneNode {
    std::uint8_t pad_0x00[0x130];     // not confirmed: ISceneNode base region
    float relative_transformation[16]; // +0x130: returned by getRelativeTransformationMatrix (CMatrix4, 16 floats assumed)
};
static_assert(offsetof(CDummyTransformationSceneNode, relative_transformation) == 0x130);

// ---------------------------------------------------------------------------
// CColladaBinaryFileLoader
// ---------------------------------------------------------------------------
struct CColladaBinaryFileLoader {
    std::uint8_t pad_0x00[0x0C];      // not confirmed: vptr and base region
    std::uint32_t refcounted_ref;     // +0x0C: IReferenceCounted*, dropped in the dtor
};
static_assert(offsetof(CColladaBinaryFileLoader, refcounted_ref) == 0x0C);

// ---------------------------------------------------------------------------
// CAppendMeshBuffer
// ---------------------------------------------------------------------------
struct CAppendMeshBuffer {
    std::uint8_t pad_0x00[0x14];      // not confirmed: CMeshBuffer base region
    std::uint32_t stream_array;       // +0x14: stream entry array (16-byte entries, see adjustStride)
    std::uint32_t buffer_alias;       // +0x18: pointer set to buffer_b in the ctor
    std::uint8_t pad_0x1c[0x04];      // not confirmed
    std::uint32_t zero_0x20;          // +0x20: zeroed in ctor
    std::uint32_t zero_0x24;          // +0x24: zeroed in ctor
    std::uint32_t zero_0x28;          // +0x28: zeroed in ctor
    std::uint16_t field_0x2c;         // +0x2C: set to 1 in ctor
    std::uint8_t pad_0x2e[0x0A];      // +0x2E..+0x37: not confirmed
    std::uint32_t ctor_arg_0x38;      // +0x38: uint ctor arg (a2)
    std::uint32_t zero_0x3c;          // +0x3C: zeroed in ctor
    std::uint32_t ctor_arg_0x40;      // +0x40: uint ctor arg (a3)
    std::uint8_t pad_0x44[0x04];      // +0x44..+0x47: zeroed in ctor, not confirmed
    std::uint32_t stride;             // +0x48: set to 1 in ctor; adjustStride writes a uint16 here
    std::uint32_t field_0x4c;         // +0x4C: set to 2 in ctor
    std::uint32_t zero_0x50;          // +0x50: zeroed in ctor
    std::uint32_t zero_0x54;          // +0x54: zeroed in ctor
    std::uint32_t buffer_a;           // +0x58: IBuffer* (IReferenceCounted), reset with count a2
    std::uint32_t buffer_b;           // +0x5C: IBuffer* (IReferenceCounted), reset with count a3
    std::uint8_t flag_0x60;           // +0x60: bool, set to 1 in ctor and by endAppend
    std::uint8_t pad_0x61[0x03];      // not confirmed
    std::uint32_t vec_begin;          // +0x64: byte vector begin (configureStream)
    std::uint32_t vec_end;            // +0x68: byte vector end
    std::uint32_t vec_cap;            // +0x6C: byte vector capacity
};
static_assert(offsetof(CAppendMeshBuffer, stream_array) == 0x14);
static_assert(offsetof(CAppendMeshBuffer, buffer_alias) == 0x18);
static_assert(offsetof(CAppendMeshBuffer, field_0x2c) == 0x2C);
static_assert(offsetof(CAppendMeshBuffer, ctor_arg_0x38) == 0x38);
static_assert(offsetof(CAppendMeshBuffer, ctor_arg_0x40) == 0x40);
static_assert(offsetof(CAppendMeshBuffer, stride) == 0x48);
static_assert(offsetof(CAppendMeshBuffer, buffer_a) == 0x58);
static_assert(offsetof(CAppendMeshBuffer, buffer_b) == 0x5C);
static_assert(offsetof(CAppendMeshBuffer, flag_0x60) == 0x60);
static_assert(offsetof(CAppendMeshBuffer, vec_begin) == 0x64);
static_assert(offsetof(CAppendMeshBuffer, vec_end) == 0x68);
static_assert(offsetof(CAppendMeshBuffer, vec_cap) == 0x6C);

// ---------------------------------------------------------------------------
// CBillboardTextSceneNode
// ---------------------------------------------------------------------------
struct CBillboardTextSceneNode {
    std::uint8_t pad_0x000[0x134];    // not confirmed: ITextSceneNode / ISceneNode base region
    std::uint8_t pad_0x134[0x44];     // +0x134..+0x177: secondary vptr and inline text buffer, not confirmed
    std::uint32_t text_begin;         // +0x178: text buffer begin (set to +0x138 in ctor)
    std::uint32_t text_end;           // +0x17C: text buffer end
    std::uint8_t pad_0x180[0x04];     // not confirmed
    std::uint32_t font;               // +0x184: IGUIFont* (ctor arg); vtable slot 0x48 queried for dimensions
    float dim_x;                      // +0x188: width component of setSize
    float dim_y;                      // +0x18C: height component of setSize
    std::uint8_t pad_0x190[0x18];     // +0x190..+0x1A7: float defaults and flags set in ctor, not confirmed
    std::uint32_t ctor_arg_0x1a8;     // +0x1A8: uint ctor arg (a9)
    std::uint32_t color;              // +0x1AC: SColor ctor arg (a10), written byte-wise in ctor
    std::uint8_t pad_0x1b0[0x0C];     // +0x1B0..+0x1BB: zeroed in ctor, not confirmed
    std::uint32_t material;           // +0x1BC: refcounted holder; getMaterialCount calls its vtable slot 0x10
    std::uint8_t pad_0x1c0[0x08];     // +0x1C0..+0x1C7: secondary vptr and flag (=1), not confirmed
};
static_assert(offsetof(CBillboardTextSceneNode, text_begin) == 0x178);
static_assert(offsetof(CBillboardTextSceneNode, text_end) == 0x17C);
static_assert(offsetof(CBillboardTextSceneNode, font) == 0x184);
static_assert(offsetof(CBillboardTextSceneNode, dim_x) == 0x188);
static_assert(offsetof(CBillboardTextSceneNode, dim_y) == 0x18C);
static_assert(offsetof(CBillboardTextSceneNode, ctor_arg_0x1a8) == 0x1A8);
static_assert(offsetof(CBillboardTextSceneNode, color) == 0x1AC);
static_assert(offsetof(CBillboardTextSceneNode, material) == 0x1BC);

// ---------------------------------------------------------------------------
// CTerrainSceneNode
// ---------------------------------------------------------------------------
struct CTerrainSceneNode {
    std::uint8_t pad_0x000[0x110];    // not confirmed: ISceneNode base region
    std::uint32_t scene_manager;      // +0x110: CSceneManager*; onRegisterSceneNode and render call through it
    std::uint8_t pad_0x114[0x20];     // +0x114..+0x133: not confirmed (+0x130 also read by getHeight)
    float position[3];                // +0x134: vector3d set by setPosition
    float rotation[4];                // +0x140: quaternion set by setRotation (getMatrix in applyTransformation)
    float pivot[3];                   // +0x150: vector3d added to transformed vertices in applyTransformation
    float scale[3];                   // +0x15C: vector3d set by setScale
    std::uint8_t pad_0x168[0x10];     // +0x168..+0x177: not confirmed
    std::uint32_t patch_list_begin;   // +0x178: patch container begin (createPatches, getIndex); element type not confirmed
    std::uint32_t patch_list_end;     // +0x17C: patch container end
    std::uint8_t pad_0x180[0x2C];     // +0x180..+0x1AB: LOD and index state, not confirmed
    std::uint32_t material_holder;    // +0x1AC: refcounted holder; getMaterialCount calls its vtable slot 0x10
};
static_assert(offsetof(CTerrainSceneNode, scene_manager) == 0x110);
static_assert(offsetof(CTerrainSceneNode, position) == 0x134);
static_assert(offsetof(CTerrainSceneNode, rotation) == 0x140);
static_assert(offsetof(CTerrainSceneNode, pivot) == 0x150);
static_assert(offsetof(CTerrainSceneNode, scale) == 0x15C);
static_assert(offsetof(CTerrainSceneNode, patch_list_begin) == 0x178);
static_assert(offsetof(CTerrainSceneNode, patch_list_end) == 0x17C);
static_assert(offsetof(CTerrainSceneNode, material_holder) == 0x1AC);

}  // namespace glitch::scene

#endif  // GLITCH_SCENE_COLLADA_GUI_LAYOUTS_SCENE_MEMBERS_HPP
