// glitch::scene member layouts for glitch-scene-collada-gui, derived from the
// Hex-Rays pseudocode (32-bit ARM target). Each member carries its original
// byte offset. Declarations only: no function bodies, no vtables, no methods.
//
// Conventions:
//   * Target pointers and container words are declared as std::uint32_t so
//     that the offsets hold on any host (a 64-bit host would otherwise widen
//     them). Their meaning is given in the comment.
//   * Gaps are unnamed pad_0xNN arrays, with the offset in the name.
//   * Fields whose type is not confirmed by the pseudocode use a sized integer
//     and say "type not confirmed".
#ifndef GLITCH_SCENE_COLLADA_GUI_LAYOUTS_SCENE_HPP
#define GLITCH_SCENE_COLLADA_GUI_LAYOUTS_SCENE_HPP

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"

namespace glitch::video {
class IVideoDriver;
}  // namespace glitch::video

namespace glitch::collada {
struct SMesh;          // tag not confirmed; forward declaration only (CMeshBuffer constructor)
struct SBufferConfig;  // tag not confirmed; forward declaration only (CMeshBuffer constructor)
}  // namespace glitch::collada

namespace glitch::scene {

// glitch::scene::ISceneNode
// Accessors touching these words: getAbsoluteTransformation (+0x24),
// getChildren (+0xf0), getAnimators (+0xfc), getBindedAnimators (+0x104),
// getID (+0x10c), getTriangleSelector (+0x114), setAutomaticCulling (+0x118),
// isDebugObject / setIsDebugObject (+0x11c), getGameData (+0x124),
// getCameraOffset (+0x128), getRenderingLayer (+0x12c).
// Two-word intrusive list head (next, prev). The accessors return the address of the
// embedded object, so the sentinel lives inside ISceneNode rather than behind a pointer.
struct ListSentinel {
    std::uint32_t next;  // +0x00
    std::uint32_t prev;  // +0x04
};
static_assert(sizeof(ListSentinel) == 8, "ListSentinel is two words");

struct ISceneNode {
    std::uint32_t vtable;                 // +0x00 vptr (polymorphic class; not read by the category pseudocode)
    std::uint8_t pad_0x04[0x20];          // +0x04..+0x23 not confirmed
    float absoluteTransform[16];          // +0x24..+0x63 (4x4 matrix, returned by getAbsoluteTransformation)
    std::uint8_t pad_0x64[0x8c];          // +0x64..+0xef not confirmed
    std::uint32_t children[3];            // +0xf0..+0xfb child container words; getChildren returns this+0xf0; +0xf4 is not yet confirmed as a sentinel
    ListSentinel animators;               // +0xfc intrusive list sentinel (getAnimators returns this+0xfc, the object itself)
    ListSentinel bindedAnimators;         // +0x104 intrusive list sentinel (getBindedAnimators returns this+0x104)
    std::int32_t id;                      // +0x10c
    std::uint8_t pad_0x110[4];            // +0x110..+0x113 not confirmed
    std::uint32_t triangleSelector;       // +0x114 ITriangleSelector target pointer
    std::uint32_t automaticCulling;       // +0x118 E_CULLING_TYPE, stored as given
    std::uint32_t flags;                  // +0x11c bit 7 is the debug-object flag (isDebugObject); other bits not confirmed
    std::uint32_t reserved0x120;          // +0x120 not confirmed
    std::uint32_t gameData;               // +0x124 void* target pointer
    float cameraOffset;                   // +0x128
    std::int32_t renderingLayer;          // +0x12c
};

static_assert(offsetof(ISceneNode, absoluteTransform) == 0x24, "ISceneNode::absoluteTransform");
static_assert(offsetof(ISceneNode, children) == 0xf0, "ISceneNode::children");
static_assert(offsetof(ISceneNode, animators) == 0xfc, "ISceneNode::animators");
static_assert(offsetof(ISceneNode, bindedAnimators) == 0x104, "ISceneNode::bindedAnimators");
static_assert(offsetof(ISceneNode, id) == 0x10c, "ISceneNode::id");
static_assert(offsetof(ISceneNode, triangleSelector) == 0x114, "ISceneNode::triangleSelector");
static_assert(offsetof(ISceneNode, automaticCulling) == 0x118, "ISceneNode::automaticCulling");
static_assert(offsetof(ISceneNode, flags) == 0x11c, "ISceneNode::flags");
static_assert(offsetof(ISceneNode, gameData) == 0x124, "ISceneNode::gameData");
static_assert(offsetof(ISceneNode, cameraOffset) == 0x128, "ISceneNode::cameraOffset");
static_assert(offsetof(ISceneNode, renderingLayer) == 0x12c, "ISceneNode::renderingLayer");

// glitch::scene::IMesh. The reference count word at +0x04 is the one that
// CMesh::clone and CMeshBuffer's constructor and clone use.
struct IMesh {
    std::uint32_t vtable;                 // +0x00 vptr
    std::int32_t referenceCount;          // +0x04
};

// glitch::scene::CMesh::SBuffer (12 bytes). The first word is the buffer
// that recalculateBoundingBox reads as a CMeshBuffer (fields +0x14, +0x24,
// +0x28 of that buffer). The other two are refcounted pointers copied by
// SBuffer::operator=.
struct CMeshSBuffer {
    std::uint32_t buffer;                 // +0x00 CMeshBuffer target pointer (refcounted)
    std::uint32_t material;               // +0x04 video::CMaterial target pointer (refcounted)
    std::uint32_t vertexAttributeMap;     // +0x08 video::CMaterialVertexAttributeMap target pointer (refcounted)
};
static_assert(sizeof(CMeshSBuffer) == 12, "CMesh::SBuffer stride is 12 bytes");

// glitch::scene::CMesh (44 bytes, operator new(0x2c) in clone).
struct CMesh {
    std::uint32_t vtable;                 // +0x00 vptr
    std::int32_t referenceCount;          // +0x04
    std::uint32_t buffersBegin;           // +0x08 CMesh::SBuffer array begin (32-bit target pointer)
    std::uint32_t buffersEnd;             // +0x0c CMesh::SBuffer array end
    std::uint32_t buffersCapacity;        // +0x10 type not confirmed (third container word)
    core::aabbox3d<float> boundingBox;    // +0x14..+0x2b; MinEdge at +0x14, MaxEdge at +0x20 (recalculateBoundingBox)
};
static_assert(offsetof(CMesh, boundingBox) == 0x14, "CMesh::boundingBox");
static_assert(sizeof(CMesh) == 0x2c, "CMesh size");

// glitch::scene::CMeshBuffer (56 bytes, operator new(0x38) in clone).
struct CMeshBuffer {
    std::uint32_t vtable;                 // +0x00 vptr
    std::int32_t referenceCount;          // +0x04
    std::uint32_t blockAt0x08;            // +0x08 refcounted block; element storage is at block+0x0c; type not confirmed
    std::uint32_t blockAt0x0c;            // +0x0c refcounted block, same layout as +0x08; type not confirmed
    std::uint32_t blockAt0x10;            // +0x10 refcounted block, same layout as +0x08; type not confirmed
    std::uint32_t vertexStreams;          // +0x14 video::CVertexStreams target pointer (refcounted)
    std::uint32_t primitiveStream;        // +0x18 refcounted object, copied from the first word of CPrimitiveStream
    std::uint32_t primitiveWord1C;        // +0x1c copied from CPrimitiveStream word 1; type not confirmed
    std::uint32_t primitiveWord20;        // +0x20 copied from CPrimitiveStream word 2; type not confirmed
    std::uint32_t primitiveWord24;        // +0x24 copied from CPrimitiveStream word 3; passed to computeBoundingBox
    std::uint32_t primitiveWord28;        // +0x28 copied from CPrimitiveStream word 4; passed to computeBoundingBox
    std::uint16_t primitiveHalf2C;        // +0x2c copied from CPrimitiveStream halfword 10; type not confirmed
    std::uint16_t primitiveHalf2E;        // +0x2e copied from CPrimitiveStream halfword 11; type not confirmed
    std::uint32_t owner;                  // +0x30 refcounted-style target pointer; its vtable slot 8 is called by onDelete
    std::uint8_t flag34;                  // +0x34 set to 1 by the constructor; role not confirmed
    std::uint8_t pad_0x35[3];             // +0x35..+0x37

    // Declared for Track B batch e1-glitch-scene-collada-gui-glitch_scene_CMeshBuffer-0.
    // Definitions: src/tb_scene-collada-gui-glitch_scene_CMeshBuffer-0.cpp (partial, see there).
    CMeshBuffer() = default;
    // 0x6bcf80 glitch::scene::CMeshBuffer::CMeshBuffer(glitch::video::IVideoDriver*, glitch::collada::SMesh&, unsigned int, glitch::collada::SBufferConfig const&, glitch::collada::SBufferConfig const&, bool)
    CMeshBuffer(video::IVideoDriver* driver, collada::SMesh& mesh, unsigned int primitiveIndex,
                const collada::SBufferConfig& vertexConfig, const collada::SBufferConfig& indexConfig, bool onDemand);
    // 0x6bc8c4 glitch::scene::CMeshBuffer::~CMeshBuffer()
    ~CMeshBuffer();
};
static_assert(offsetof(CMeshBuffer, vertexStreams) == 0x14, "CMeshBuffer::vertexStreams");
static_assert(offsetof(CMeshBuffer, primitiveWord24) == 0x24, "CMeshBuffer::primitiveWord24");
static_assert(offsetof(CMeshBuffer, primitiveWord28) == 0x28, "CMeshBuffer::primitiveWord28");
static_assert(offsetof(CMeshBuffer, primitiveHalf2C) == 0x2c, "CMeshBuffer::primitiveHalf2C");
static_assert(offsetof(CMeshBuffer, owner) == 0x30, "CMeshBuffer::owner");
static_assert(sizeof(CMeshBuffer) == 0x38, "CMeshBuffer size");

// glitch::scene::CBatchMesh. The constructor (0x578924) writes the default
// boxes and the stamp words named below.
struct CBatchMesh {
    // glitch::scene::CBatchMesh::SBatch (20 bytes; stride of the batch array).
    struct SBatch {
        std::uint32_t meshBuffer;         // +0x00 target pointer
        std::uint32_t material;           // +0x04 target pointer
        std::uint32_t vertexAttributeMap; // +0x08 target pointer
        std::uint16_t field0C;            // +0x0c zeroed by SBatch(); role not confirmed
        std::uint16_t field0E;            // +0x0e zeroed by SBatch(); role not confirmed
        std::uint16_t field10;            // +0x10 zeroed by SBatch(); role not confirmed
        std::uint8_t pad_0x12[2];         // +0x12..+0x13
    };
    static_assert(sizeof(SBatch) == 0x14, "CBatchMesh::SBatch stride is 20 bytes");

    // glitch::scene::CBatchMesh::SSegment. Only the words named below are
    // confirmed; the total size is not.
    struct SSegment {
        std::uint8_t pad_0x00[0x10];      // +0x00..+0x0f not confirmed
        std::uint32_t indexBegin;         // +0x10 (getIndexRange, first output)
        std::uint32_t indexEnd;           // +0x14 (getIndexRange, second output)
        std::uint8_t pad_0x18[8];         // +0x18..+0x1f not confirmed
        std::uint8_t visible;             // +0x20 read by CBatchSceneNode::isSegmentVisible
    };
    static_assert(offsetof(SSegment, indexBegin) == 0x10, "SSegment::indexBegin");
    static_assert(offsetof(SSegment, visible) == 0x20, "SSegment::visible");

    std::uint32_t vtable;                 // +0x00 vptr
    std::int32_t referenceCount;          // +0x04 zeroed by the constructor
    std::uint8_t pad_0x08[0x18];          // +0x08..+0x1f zeroed by the constructor, not named
    std::uint32_t batchesBegin;           // +0x20 SBatch array begin (32-bit target pointer)
    std::uint32_t batchesEnd;             // +0x24 SBatch array end (count = (end - begin) / 20)
    std::uint32_t batchesCapacity;        // +0x28 zeroed by the constructor; type not confirmed
    std::uint32_t reserved0x2c;           // +0x2c zeroed by the constructor
    std::uint32_t reserved0x30;           // +0x30 zeroed by the constructor
    std::uint32_t reserved0x34;           // +0x34 zeroed by the constructor
    core::aabbox3d<float> localBox;       // +0x38..+0x4f; constructor sets min -1 and max +1; role not confirmed
    core::aabbox3d<float> boundingBox;    // +0x50..+0x67; constructor sets min -1 and max +1; getBoundingBox returns this
    std::uint32_t boundingBoxSource;      // +0x68 checked by getBoundingBox; type not confirmed
    std::uint32_t boundingBoxStamp;       // +0x6c constructor sets -1; setBoundingBox writes the timer tick
    std::uint32_t reserved0x70;           // +0x70 constructor sets 44; role not confirmed
    std::uint8_t boundingBoxDirty;        // +0x74 constructor sets 1; getBoundingBox tests it
    std::uint8_t pad_0x75[3];             // +0x75..+0x77
    std::uint32_t reserved0x78;           // +0x78 zeroed by the constructor; role not confirmed
};
static_assert(offsetof(CBatchMesh, batchesBegin) == 0x20, "CBatchMesh::batchesBegin");
static_assert(offsetof(CBatchMesh, batchesEnd) == 0x24, "CBatchMesh::batchesEnd");
static_assert(offsetof(CBatchMesh, localBox) == 0x38, "CBatchMesh::localBox");
static_assert(offsetof(CBatchMesh, boundingBox) == 0x50, "CBatchMesh::boundingBox");
static_assert(offsetof(CBatchMesh, boundingBoxSource) == 0x68, "CBatchMesh::boundingBoxSource");
static_assert(offsetof(CBatchMesh, boundingBoxStamp) == 0x6c, "CBatchMesh::boundingBoxStamp");
static_assert(offsetof(CBatchMesh, boundingBoxDirty) == 0x74, "CBatchMesh::boundingBoxDirty");
static_assert(offsetof(CBatchMesh, reserved0x78) == 0x78, "CBatchMesh::reserved0x78");

// glitch::scene::CBatchSceneNode. The category pseudocode reads no member of
// this class directly (isSegmentVisible reads CBatchMesh::SSegment). Base
// layout not confirmed.
struct CBatchSceneNode {};

// glitch::scene::IAnimatedMesh. No member is touched by the category
// pseudocode. Base layout not confirmed.
struct IAnimatedMesh {};

// glitch::scene::CSceneManager. Only the two flags written by
// notifyHierarchyChanged and notifyVisibilityChanged are mapped.
struct CSceneManager {
    std::uint8_t pad_0x000[0x288];        // +0x000..+0x287 not confirmed by this category
    std::uint8_t hierarchyChanged;        // +0x288 set by notifyHierarchyChanged
    std::uint8_t visibilityChanged;       // +0x289 set by notifyVisibilityChanged
};
static_assert(offsetof(CSceneManager, hierarchyChanged) == 0x288, "CSceneManager::hierarchyChanged");
static_assert(offsetof(CSceneManager, visibilityChanged) == 0x289, "CSceneManager::visibilityChanged");

}  // namespace glitch::scene

#endif  // GLITCH_SCENE_COLLADA_GUI_LAYOUTS_SCENE_HPP
