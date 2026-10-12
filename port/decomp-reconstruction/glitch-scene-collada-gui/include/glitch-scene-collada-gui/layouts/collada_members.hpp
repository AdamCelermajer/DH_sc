// Member layouts for glitch::collada::IMesh, glitch::collada::ISkinnedMesh, the
// skin technique hierarchy (glitch::collada::detail::IColladaSkinTechnique and its
// four derived techniques), reconstructed from the Hex-Rays pseudocode of
// libDungeonHunter2.so (32-bit ARM build).
//
// Only data members are declared; no function bodies. Each member carries its
// byte offset in the original object. Every pointer and reference field is stored
// as a 32-bit word (std::uint32_t), the same convention as layouts/scene.hpp, so
// the offsets hold on a 64-bit host too.
//
// This header is standalone. It defines names that collada_db.hpp, collada_misc.hpp
// and collada_database.hpp also define (ISkinnedMesh and the detail technique
// classes), so do not include it together with those headers.
#ifndef GLITCH_SCENE_COLLADA_GUI_LAYOUTS_COLLADA_MEMBERS_HPP
#define GLITCH_SCENE_COLLADA_GUI_LAYOUTS_COLLADA_MEMBERS_HPP

#include <cstddef>
#include <cstdint>

namespace glitch {
class IReferenceCounted;
namespace scene {
class CMeshBuffer;
}
namespace video {
class IVideoDriver;
}
namespace collada {
class SSkinBuffer;
class SSkin;
}  // namespace collada

namespace collada {

// ---------------------------------------------------------------------------
// glitch::collada::IMesh. The destructor sets the vtable (off_984FF0) and then
// destroys the CColladaDatabase member at +0x0c. CColladaDatabase is two words
// (record at +0x00, factory at +0x04; see layouts/collada_db.hpp), so it spans
// +0x0c..+0x13 here. Nothing beyond +0x13 is read by the destructor.
// ---------------------------------------------------------------------------
struct IMesh {
    std::uint32_t vptr_0x00;                // +0x00 vtable pointer (off_984FF0)
    std::uint8_t pad_0x04[0x08];            // +0x04..+0x0b not confirmed
    std::uint32_t database_record_0x0c;     // +0x0c CColladaDatabase::record (destroyed by ~IMesh)
    std::uint32_t database_factory_0x10;    // +0x10 CColladaDatabase::factory (destroyed by ~IMesh)
};

static_assert(offsetof(IMesh, vptr_0x00) == 0x00, "IMesh::vptr_0x00");
static_assert(offsetof(IMesh, database_record_0x0c) == 0x0c, "IMesh::database_record_0x0c");
static_assert(offsetof(IMesh, database_factory_0x10) == 0x10, "IMesh::database_factory_0x10");

// ---------------------------------------------------------------------------
// glitch::collada::ISkinnedMesh. Its destructor is the same code as IMesh's: it
// sets the same vtable (off_984FF0) and destroys the CColladaDatabase at +0x0c.
// skinningEnabled at +0x18 is the field already declared in collada_misc.hpp.
// ---------------------------------------------------------------------------
struct ISkinnedMesh {
    std::uint32_t vptr_0x00;                // +0x00 vtable pointer (off_984FF0)
    std::uint8_t pad_0x04[0x08];            // +0x04..+0x0b not confirmed
    std::uint32_t database_record_0x0c;     // +0x0c CColladaDatabase::record (destroyed by ~ISkinnedMesh)
    std::uint32_t database_factory_0x10;    // +0x10 CColladaDatabase::factory (destroyed by ~ISkinnedMesh)
    std::uint8_t pad_0x14[0x04];            // +0x14..+0x17 not confirmed
    bool skinningEnabled;                   // +0x18 (from collada_misc.hpp)
};

static_assert(offsetof(ISkinnedMesh, vptr_0x00) == 0x00, "ISkinnedMesh::vptr_0x00");
static_assert(offsetof(ISkinnedMesh, database_record_0x0c) == 0x0c, "ISkinnedMesh::database_record_0x0c");
static_assert(offsetof(ISkinnedMesh, database_factory_0x10) == 0x10, "ISkinnedMesh::database_factory_0x10");
static_assert(offsetof(ISkinnedMesh, skinningEnabled) == 0x18, "ISkinnedMesh::skinningEnabled");

namespace detail {

// ---------------------------------------------------------------------------
// glitch::collada::detail::IColladaSkinTechnique, the base of the four skin
// techniques below. Its destructor sets the vtable (off_985600) and releases the
// holder pointer at +0x08: it decrements the count word at the holder's +0x00 and,
// when that reaches zero, deletes the array at the holder's +0x0c. The derived
// classes repeat these fields, so the layout is written out in each of them.
// Nothing past +0x0b is read by the base destructor.
// ---------------------------------------------------------------------------
struct IColladaSkinTechnique {
    std::uint32_t vptr_0x00;                // +0x00 vtable pointer (off_985600)
    std::uint8_t flag_0x04;                 // +0x04 bool, last argument of the derived constructors
    std::uint8_t pad_0x05[0x03];            // +0x05..+0x07 not confirmed
    std::uint32_t sharedBuffer_0x08;        // +0x08 refcounted holder (zeroed by the derived constructors)

    // 0x670ec8 glitch::collada::detail::IColladaSkinTechnique::initProxyBuffer(CMeshBuffer*, SSkinBuffer&, SSkin&, IVideoDriver*)
    // Partial: stub that returns nullptr. See src/tb_scene-collada-gui-glitch_collada_detail_IColladaSkinTechnique-0.cpp.
    glitch::IReferenceCounted* initProxyBuffer(glitch::scene::CMeshBuffer* meshBuffer,
                                               glitch::collada::SSkinBuffer& skinBuffer,
                                               glitch::collada::SSkin& skin,
                                               glitch::video::IVideoDriver* driver);
};

static_assert(offsetof(IColladaSkinTechnique, vptr_0x00) == 0x00, "IColladaSkinTechnique::vptr_0x00");
static_assert(offsetof(IColladaSkinTechnique, flag_0x04) == 0x04, "IColladaSkinTechnique::flag_0x04");
static_assert(offsetof(IColladaSkinTechnique, sharedBuffer_0x08) == 0x08, "IColladaSkinTechnique::sharedBuffer_0x08");

// ---------------------------------------------------------------------------
// glitch::collada::detail::CColladaHardwareMatrixSkinTechnique. Constructed with
// (SSkin&, SSkinCache&, bool). The map member is a std::map keyed by unsigned
// short whose header node sits at +0x18 and whose count is at +0x28. The
// destructor sets the vtable (off_985540), erases the map, then runs the base
// destructor.
// ---------------------------------------------------------------------------
struct CColladaHardwareMatrixSkinTechnique {
    std::uint32_t vptr_0x00;                // +0x00 vtable pointer (off_985540)
    std::uint8_t flag_0x04;                 // +0x04 bool, last constructor argument
    std::uint8_t pad_0x05[0x03];            // +0x05..+0x07 not confirmed
    std::uint32_t sharedBuffer_0x08;        // +0x08 refcounted holder (zeroed by the constructor)
    std::uint32_t skin_0x0c;                // +0x0c SSkin& (reference stored as a 32-bit pointer)
    std::uint32_t skinCache_0x10;           // +0x10 SSkinCache& (reference stored as a 32-bit pointer)
    std::uint32_t attachedNode_0x14;        // +0x14 scene node pointer (zeroed by the constructor, stored by attach())
    std::uint8_t pad_0x18[0x10];            // +0x18..+0x27 map header node (color +0x18, root +0x1c, leftmost +0x20, rightmost +0x24); not confirmed beyond what the constructor and destructor write
    std::uint32_t mapCount_0x28;            // +0x28 element count of the map (zeroed by the constructor, reset by the destructor)
};

static_assert(offsetof(CColladaHardwareMatrixSkinTechnique, vptr_0x00) == 0x00, "CColladaHardwareMatrixSkinTechnique::vptr_0x00");
static_assert(offsetof(CColladaHardwareMatrixSkinTechnique, flag_0x04) == 0x04, "CColladaHardwareMatrixSkinTechnique::flag_0x04");
static_assert(offsetof(CColladaHardwareMatrixSkinTechnique, sharedBuffer_0x08) == 0x08, "CColladaHardwareMatrixSkinTechnique::sharedBuffer_0x08");
static_assert(offsetof(CColladaHardwareMatrixSkinTechnique, skin_0x0c) == 0x0c, "CColladaHardwareMatrixSkinTechnique::skin_0x0c");
static_assert(offsetof(CColladaHardwareMatrixSkinTechnique, skinCache_0x10) == 0x10, "CColladaHardwareMatrixSkinTechnique::skinCache_0x10");
static_assert(offsetof(CColladaHardwareMatrixSkinTechnique, attachedNode_0x14) == 0x14, "CColladaHardwareMatrixSkinTechnique::attachedNode_0x14");
static_assert(offsetof(CColladaHardwareMatrixSkinTechnique, mapCount_0x28) == 0x28, "CColladaHardwareMatrixSkinTechnique::mapCount_0x28");

// ---------------------------------------------------------------------------
// glitch::collada::detail::CColladaHardwareQuatSkinTechnique. Same constructor and
// destructor shape as the matrix technique (vtable off_985570). The map is keyed
// by unsigned int; the header node is at +0x18 and the count at +0x28.
// ---------------------------------------------------------------------------
struct CColladaHardwareQuatSkinTechnique {
    std::uint32_t vptr_0x00;                // +0x00 vtable pointer (off_985570)
    std::uint8_t flag_0x04;                 // +0x04 bool, last constructor argument
    std::uint8_t pad_0x05[0x03];            // +0x05..+0x07 not confirmed
    std::uint32_t sharedBuffer_0x08;        // +0x08 refcounted holder (zeroed by the constructor)
    std::uint32_t skin_0x0c;                // +0x0c SSkin& (reference stored as a 32-bit pointer)
    std::uint32_t skinCache_0x10;           // +0x10 SSkinCache& (reference stored as a 32-bit pointer)
    std::uint32_t attachedNode_0x14;        // +0x14 scene node pointer (zeroed by the constructor, stored by attach())
    std::uint8_t pad_0x18[0x10];            // +0x18..+0x27 map header node (color +0x18, root +0x1c, leftmost +0x20, rightmost +0x24); not confirmed beyond what the constructor and destructor write
    std::uint32_t mapCount_0x28;            // +0x28 element count of the map (zeroed by the constructor, reset by the destructor)
};

static_assert(offsetof(CColladaHardwareQuatSkinTechnique, vptr_0x00) == 0x00, "CColladaHardwareQuatSkinTechnique::vptr_0x00");
static_assert(offsetof(CColladaHardwareQuatSkinTechnique, flag_0x04) == 0x04, "CColladaHardwareQuatSkinTechnique::flag_0x04");
static_assert(offsetof(CColladaHardwareQuatSkinTechnique, sharedBuffer_0x08) == 0x08, "CColladaHardwareQuatSkinTechnique::sharedBuffer_0x08");
static_assert(offsetof(CColladaHardwareQuatSkinTechnique, skin_0x0c) == 0x0c, "CColladaHardwareQuatSkinTechnique::skin_0x0c");
static_assert(offsetof(CColladaHardwareQuatSkinTechnique, skinCache_0x10) == 0x10, "CColladaHardwareQuatSkinTechnique::skinCache_0x10");
static_assert(offsetof(CColladaHardwareQuatSkinTechnique, attachedNode_0x14) == 0x14, "CColladaHardwareQuatSkinTechnique::attachedNode_0x14");
static_assert(offsetof(CColladaHardwareQuatSkinTechnique, mapCount_0x28) == 0x28, "CColladaHardwareQuatSkinTechnique::mapCount_0x28");

// ---------------------------------------------------------------------------
// glitch::collada::detail::CColladaHardwareTextureSkinTechnique. The constructor
// zeroes +0x14, +0x24 and +0x30 (the map count) and sets the map header node at
// +0x20. init() stores its IVideoDriver* argument at +0x18. prepareCache() writes
// a float at +0x1c (1.0f over an int field of the texture it creates) and skin()
// reads it back. The map is keyed by unsigned int and its count is at +0x30.
// ---------------------------------------------------------------------------
struct CColladaHardwareTextureSkinTechnique {
    std::uint32_t vptr_0x00;                // +0x00 vtable pointer (off_9855A0)
    std::uint8_t flag_0x04;                 // +0x04 bool, last constructor argument
    std::uint8_t pad_0x05[0x03];            // +0x05..+0x07 not confirmed
    std::uint32_t sharedBuffer_0x08;        // +0x08 refcounted holder (zeroed by the constructor)
    std::uint32_t skin_0x0c;                // +0x0c SSkin& (reference stored as a 32-bit pointer)
    std::uint32_t skinCache_0x10;           // +0x10 SSkinCache& (reference stored as a 32-bit pointer)
    std::uint32_t attachedNode_0x14;        // +0x14 scene node pointer (zeroed by the constructor, stored by attach())
    std::uint32_t driver_0x18;              // +0x18 IVideoDriver* stored by init(); prepareCache reads its fields
    float invTextureSize_0x1c;              // +0x1c written by prepareCache (1.0f / int field of the texture); read by skin()
    std::uint8_t pad_0x20[0x10];            // +0x20..+0x2f map header node (color +0x20, root +0x24, leftmost +0x28, rightmost +0x2c); not confirmed beyond what the constructor and destructor write
    std::uint32_t mapCount_0x30;            // +0x30 element count of the map (zeroed by the constructor, reset by the destructor)
};

static_assert(offsetof(CColladaHardwareTextureSkinTechnique, vptr_0x00) == 0x00, "CColladaHardwareTextureSkinTechnique::vptr_0x00");
static_assert(offsetof(CColladaHardwareTextureSkinTechnique, flag_0x04) == 0x04, "CColladaHardwareTextureSkinTechnique::flag_0x04");
static_assert(offsetof(CColladaHardwareTextureSkinTechnique, sharedBuffer_0x08) == 0x08, "CColladaHardwareTextureSkinTechnique::sharedBuffer_0x08");
static_assert(offsetof(CColladaHardwareTextureSkinTechnique, skin_0x0c) == 0x0c, "CColladaHardwareTextureSkinTechnique::skin_0x0c");
static_assert(offsetof(CColladaHardwareTextureSkinTechnique, skinCache_0x10) == 0x10, "CColladaHardwareTextureSkinTechnique::skinCache_0x10");
static_assert(offsetof(CColladaHardwareTextureSkinTechnique, attachedNode_0x14) == 0x14, "CColladaHardwareTextureSkinTechnique::attachedNode_0x14");
static_assert(offsetof(CColladaHardwareTextureSkinTechnique, driver_0x18) == 0x18, "CColladaHardwareTextureSkinTechnique::driver_0x18");
static_assert(offsetof(CColladaHardwareTextureSkinTechnique, invTextureSize_0x1c) == 0x1c, "CColladaHardwareTextureSkinTechnique::invTextureSize_0x1c");
static_assert(offsetof(CColladaHardwareTextureSkinTechnique, mapCount_0x30) == 0x30, "CColladaHardwareTextureSkinTechnique::mapCount_0x30");

// ---------------------------------------------------------------------------
// glitch::collada::detail::CColladaSoftwareSkinTechnique. The constructor zeroes
// the byte at +0x18 and sets the map header node at +0x1c; the count is at +0x2c.
// The map is keyed by unsigned int and holds SSoftwareColorGenData values.
// ---------------------------------------------------------------------------
struct CColladaSoftwareSkinTechnique {
    std::uint32_t vptr_0x00;                // +0x00 vtable pointer (off_9855D0)
    std::uint8_t flag_0x04;                 // +0x04 bool, last constructor argument
    std::uint8_t pad_0x05[0x03];            // +0x05..+0x07 not confirmed
    std::uint32_t sharedBuffer_0x08;        // +0x08 refcounted holder (zeroed by the constructor)
    std::uint32_t skin_0x0c;                // +0x0c SSkin& (reference stored as a 32-bit pointer)
    std::uint32_t skinCache_0x10;           // +0x10 SSkinCache& (reference stored as a 32-bit pointer)
    std::uint32_t attachedNode_0x14;        // +0x14 scene node pointer (zeroed by the constructor, stored by attach())
    std::uint8_t pad_0x18[0x04];            // +0x18..+0x1b; the byte at +0x18 is zeroed by the constructor, role not confirmed
    std::uint8_t pad_0x1c[0x10];            // +0x1c..+0x2b map header node (color +0x1c, root +0x20, leftmost +0x24, rightmost +0x28); not confirmed beyond what the constructor and destructor write
    std::uint32_t mapCount_0x2c;            // +0x2c element count of the map (zeroed by the constructor, reset by the destructor)
};

static_assert(offsetof(CColladaSoftwareSkinTechnique, vptr_0x00) == 0x00, "CColladaSoftwareSkinTechnique::vptr_0x00");
static_assert(offsetof(CColladaSoftwareSkinTechnique, flag_0x04) == 0x04, "CColladaSoftwareSkinTechnique::flag_0x04");
static_assert(offsetof(CColladaSoftwareSkinTechnique, sharedBuffer_0x08) == 0x08, "CColladaSoftwareSkinTechnique::sharedBuffer_0x08");
static_assert(offsetof(CColladaSoftwareSkinTechnique, skin_0x0c) == 0x0c, "CColladaSoftwareSkinTechnique::skin_0x0c");
static_assert(offsetof(CColladaSoftwareSkinTechnique, skinCache_0x10) == 0x10, "CColladaSoftwareSkinTechnique::skinCache_0x10");
static_assert(offsetof(CColladaSoftwareSkinTechnique, attachedNode_0x14) == 0x14, "CColladaSoftwareSkinTechnique::attachedNode_0x14");
static_assert(offsetof(CColladaSoftwareSkinTechnique, mapCount_0x2c) == 0x2c, "CColladaSoftwareSkinTechnique::mapCount_0x2c");

}  // namespace detail
}  // namespace collada
}  // namespace glitch

#endif  // GLITCH_SCENE_COLLADA_GUI_LAYOUTS_COLLADA_MEMBERS_HPP
