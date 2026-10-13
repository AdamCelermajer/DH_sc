// Bodies of glitch::collada::detail::IColladaSkinTechnique and the four skin techniques (canonical classes:
// collada/IColladaSkinTechnique.hpp). This file replaces the technique stand-ins and bodies of src/collada_misc.cpp and of
// src/tb_scene-collada-gui-glitch_collada_detail_{CColladaHardwareMatrixSkin,CColladaHardwareQuatSkinTe,CColladaHardwareTextureSki,
// CColladaSoftwareSkinTechni,IColladaSkinTechnique}-0.cpp.
//
// Several members are the same code in all techniques (the compiler emitted one copy per class): computeBoundingBox, preparePtrCache,
// onPrepareBufferForRendering (hardware), checkAvailability (hardware) and init (matrix, quaternion). The pseudocode of each copy was
// compared; they are identical, so each class member forwards to one helper below.
#include "glitch-scene-collada-gui/collada/IColladaSkinTechnique.hpp"

#include <limits>

#include "glitch-core-io/core/matrix4_operators.hpp"
#include "glitch-scene-collada-gui/scene/CMeshBuffer.hpp"
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"
#include "glitch-video/buffer.hpp"
#include "glitch-video/material.hpp"
#include "glitch-video/material_renderer.hpp"
#include "glitch-video/shader_attributes.hpp"
#include "glitch-video/video_driver.hpp"

namespace glitch::collada::detail {

namespace {

// The shader of a technique's first render pass.
const video::IShader* firstShader(const video::STechnique& technique) {
    return static_cast<const video::IShader*>(technique.passes[0].shader);
}

// The union of the vertex attribute masks of every pass shader of the technique (the checks require both mask bits 0x30000000).
std::uint32_t passAttributeMasks(const video::STechnique& technique) {
    std::uint32_t mask = 0;
    for (unsigned i = 0; i < technique.passCount; ++i) {
        mask |= static_cast<const video::IShader*>(technique.passes[i].shader)->m_vertexAttributeMask;
    }
    return mask;
}

// glitch::core::operator*<float, glitch::collada::SMatrix> (0x6651a0): the lhs times a 16-value matrix. An identity lhs gives a copy of
// the rhs. Otherwise the 3x3 block and the translation column are multiplied (the last column is 0, 0, 0, 1) and the identity flag is
// clear. The template is defined in glitch-core-io's source file and instantiated for no type, so the scene category repeats the
// arithmetic here (layout request for the core-io owner: instantiate it for collada::SMatrix and use it from here).
core::CMatrix4<float> multiplyBySMatrix(const core::CMatrix4<float>& lhs, const float* rhs) {
    core::CMatrix4<float> out{};
    if (lhs.flag) {
        for (int i = 0; i < 16; ++i) {
            out.M[i] = rhs[i];
        }
        out.flag = false;
        return out;
    }
    const float* a = lhs.M;
    for (int c = 0; c < 3; ++c) {
        for (int r = 0; r < 3; ++r) {
            out.M[r + 4 * c] = ((a[r] * rhs[4 * c]) + (a[r + 4] * rhs[4 * c + 1])) + (a[r + 8] * rhs[4 * c + 2]);
        }
    }
    out.M[3] = 0;
    out.M[7] = 0;
    out.M[11] = 0;
    for (int r = 0; r < 3; ++r) {
        out.M[12 + r] = (((a[r] * rhs[12]) + (a[r + 4] * rhs[13])) + (a[r + 8] * rhs[14])) + a[12 + r];
    }
    out.M[15] = 1;
    out.flag = false;
    return out;
}

// The inlined tail shared by checkAvailability and init: the skin's proxy buffer becomes a static buffer (the usage byte is cleared;
// a driver-mapped buffer is unbound first; a buffer with data is flagged as having data).
void makeProxyStatic(SSkin& skin) {
    video::IBuffer* proxy = skin.proxy;
    if (proxy != nullptr && proxy->m_usage != 0) {
        if ((proxy->m_flags12 & video::IBuffer::kFlagDriverMapped) != 0) {
            proxy->unbindImpl();
        }
        const bool hasData = proxy->m_data != nullptr;
        proxy->m_usage = 0;
        if (hasData) {
            proxy->m_flags12 |= video::IBuffer::kFlagHasData;
        }
    }
}

// 0x66c544 / 0x66df88 / 0x66ee94 / 0x66fab4 preparePtrCache. When the cache's pointer bit (0x10000) is set, the vector of joint matrix
// pointers is sized to the skin's joint count and filled from the attached node's tree (getSceneNodeFromScopeID of each joint's
// scope id, then the node's absolute transformation, K 0x38), null for a missing node; the bit is cleared.
void preparePtrCache(SSkin& skin, SSkinCache& cache, scene::ISceneNode* attachedNode) {
    if ((cache.dirty & 0x10000u) == 0) {
        return;
    }
    cache.jointMatrices.resize(skin.jointCount, nullptr);
    for (std::int32_t i = 0; i < skin.jointCount; ++i) {
        scene::ISceneNode* node = attachedNode->getSceneNodeFromScopeID(skin.jointScopeIds[i]);
        cache.jointMatrices[i] = node != nullptr ? &node->getAbsoluteTransformation() : nullptr;
    }
    cache.dirty &= ~0x10000u;
}

// 0x66c5f4 / 0x66e384 / 0x66ef44 / 0x66fb84 computeBoundingBox (the box is returned through a hidden slot). The pointer cache is
// prepared, the box starts inverted (min +FLT_MAX, max -FLT_MAX), and then: with joint boxes in the skin (+0x8c non-zero) the box of
// each joint (six floats, 24-byte stride) is moved by the joint's matrix and merged; without them the translation of each joint matrix
// is merged as a point. The joint count is the pointer vector's size truncated to 8 bits, as the pseudocode does. Bit 3 of the
// cache's dirty word is cleared.
core::aabbox3d<float> computeSkinBoundingBox(SSkin& skin, SSkinCache& cache, scene::ISceneNode* attachedNode) {
    preparePtrCache(skin, cache, attachedNode);
    constexpr float kMax = std::numeric_limits<float>::max();
    core::aabbox3d<float> box{{kMax, kMax, kMax}, {-kMax, -kMax, -kMax}};
    const unsigned count = static_cast<std::uint8_t>(cache.jointMatrices.size());
    if (skin.boxCount != 0) {
        for (unsigned i = 0; i < count; ++i) {
            const float* source = skin.jointBoxes + 6 * i;
            core::aabbox3d<float> local{{source[0], source[1], source[2]}, {source[3], source[4], source[5]}};
            cache.jointMatrices[i]->transformBox(local);
            box.addInternalBox(local);
        }
    } else {
        for (unsigned i = 0; i < count; ++i) {
            const core::CMatrix4<float>* matrix = cache.jointMatrices[i];
            box.addInternalPoint(core::vector3d<float>{matrix->M[12], matrix->M[13], matrix->M[14]});
        }
    }
    cache.dirty &= ~8u;
    return box;
}

}  // namespace

// ---------------------------------------------------------------------------------------------------------------------
// IColladaSkinTechnique
// ---------------------------------------------------------------------------------------------------------------------

// 0x670dcc _ZN6glitch7collada6detail20IColladaSkinTechniqueD1Ev (0x670e58 is the D2 clone; D0 0x670e3c is skipped)
// Drops the use of the skin-data block (the handle does it: the data is freed with the last use).
IColladaSkinTechnique::~IColladaSkinTechnique() = default;

// 0x670ec8 _ZN6glitch7collada6detail20IColladaSkinTechnique15initProxyBufferEPNS_5scene11CMeshBufferERNS0_11SSkinBufferERNS0_5SSkinEPNS_5video12IVideoDriverE
// PARTIAL: stub. The 1368-byte body builds the proxy buffer of a skinned mesh buffer: it reads the buffer's vertex stream table,
// creates the 0x38-byte proxy mesh buffer object (vtable off_988098) in the SSkinBuffer, copies the masked stream entries, takes the
// skin's IBuffer (skin +0x94) from the driver (driver slot +0x78) when the technique has none, and describes it twice to the proxy
// table with the element size 4 * (weights per vertex + 1). The proxy class and the SSkinBuffer words +0x18..+0x2e are not mapped.
boost::intrusive_ptr<glitch::IReferenceCounted> IColladaSkinTechnique::initProxyBuffer(scene::CMeshBuffer*, SSkinBuffer&, SSkin&,
                                                                                       video::IVideoDriver*) {
    return boost::intrusive_ptr<glitch::IReferenceCounted>();
}

// ---------------------------------------------------------------------------------------------------------------------
// CColladaHardwareTextureSkinTechnique
// ---------------------------------------------------------------------------------------------------------------------

// 0x66e6b8 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechniqueC1ERNS0_5SSkinERNS0_10SSkinCacheEb (C2 0x66e714)
CColladaHardwareTextureSkinTechnique::CColladaHardwareTextureSkinTechnique(SSkin& skin, SSkinCache& skinCache, bool hasSkinData)
    : IColladaSkinTechnique(hasSkinData), skinRecord(&skin), cache(&skinCache) {}

// 0x66ee10 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechniqueD1Ev (D0 0x66ee78 skipped)
// Erases the map (when it has entries), then the base destructor runs.
CColladaHardwareTextureSkinTechnique::~CColladaHardwareTextureSkinTechnique() {
    hardwareData.clear();
}

// 0x66e770 _ZNK6glitch7collada6detail36CColladaHardwareTextureSkinTechnique16needOutputBufferEv
bool CColladaHardwareTextureSkinTechnique::needOutputBuffer() const {
    return false;
}

// 0x66e778 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique6attachEPNS_5scene10ISceneNodeE
void CColladaHardwareTextureSkinTechnique::attach(scene::ISceneNode* node) {
    attachedNode = node;
}

// 0x66e780 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique28onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
// At step 1 the technique skins the buffer (K 0x18); the answer is always 16.
int CColladaHardwareTextureSkinTechnique::onPrepareBufferForRendering(E_PREPARE_BUFFER_STEP step, video::IVideoDriver*, SSkinBuffer& buffer,
                                                                      scene::CMeshBuffer* meshBuffer) {
    if (step == 1) {
        skin(buffer, meshBuffer);
    }
    return 16;
}

// 0x66e7d0 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique20checkAvailabilityStaticERKNS_5video10STechniqueE
// True when the technique's first pass shader has parameters of types 12 and 13 and the passes' vertex attribute masks together hold
// both bits of 0x30000000.
bool CColladaHardwareTextureSkinTechnique::checkAvailabilityStatic(const video::STechnique& technique) {
    const std::uint32_t masks = passAttributeMasks(technique);
    return firstShader(technique)->getParameterID(12, 0, 0) != 0xffff && firstShader(technique)->getParameterID(13, 0, 0) != 0xffff &&
           (masks & 0x30000000u) == 0x30000000u;
}

// 0x66e89c _ZNK6glitch7collada6detail36CColladaHardwareTextureSkinTechnique17checkAvailabilityERKNS_5video10STechniqueE
// The static answer; when it is true the skin's proxy buffer is made static (see makeProxyStatic).
bool CColladaHardwareTextureSkinTechnique::checkAvailability(const video::STechnique& technique) const {
    const bool available = checkAvailabilityStatic(technique);
    if (available) {
        makeProxyStatic(*skinRecord);
    }
    return available;
}

// 0x66ee94 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique15preparePtrCacheEv
void CColladaHardwareTextureSkinTechnique::preparePtrCache() {
    detail::preparePtrCache(*skinRecord, *cache, attachedNode);
}

// 0x66ef44 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique18computeBoundingBoxEv
core::aabbox3d<float> CColladaHardwareTextureSkinTechnique::computeBoundingBox() {
    return computeSkinBoundingBox(*skinRecord, *cache, attachedNode);
}

// 0x66f1f4 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique4initERNS0_11SSkinBufferEPNS_5scene11CMeshBufferEPNS_5video12IVideoDriverEb
// Stores the driver, builds the proxy buffer, makes the skin's proxy buffer static and answers 0 (no output buffer).
int CColladaHardwareTextureSkinTechnique::init(SSkinBuffer& buffer, scene::CMeshBuffer* meshBuffer, video::IVideoDriver* videoDriver,
                                               bool /*flag*/) {
    driver = videoDriver;
    initProxyBuffer(meshBuffer, buffer, *skinRecord, videoDriver);
    makeProxyStatic(*skinRecord);
    return 0;
}

// 0x66f280 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique12prepareCacheEv
// PARTIAL: the 776-byte body fills the cache's palette as a texture of the joint matrices; it multiplies glitch::collada::SMatrix
// values (core's operator*<float, SMatrix> 0x6651a0 is declared but not instantiated for the collada type) and writes the cache's
// buffer through the driver. Not reconstructed.
void CColladaHardwareTextureSkinTechnique::prepareCache() {}

// 0x66ec14 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique4skinERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
// PARTIAL: sets the joint texture parameters of the buffer's material through IMaterialParameters<CMaterial>::setParameter, which
// glitch-video does not expose on CMaterial. Not reconstructed.
int CColladaHardwareTextureSkinTechnique::skin(SSkinBuffer&, scene::CMeshBuffer*) {
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
// CColladaHardwareMatrixSkinTechnique
// ---------------------------------------------------------------------------------------------------------------------

// 0x66b880 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechniqueC1ERNS0_5SSkinERNS0_10SSkinCacheEb (C2 0x66b8dc)
CColladaHardwareMatrixSkinTechnique::CColladaHardwareMatrixSkinTechnique(SSkin& skin, SSkinCache& skinCache, bool hasSkinData)
    : IColladaSkinTechnique(hasSkinData), skinRecord(&skin), cache(&skinCache) {}

// 0x66b9cc _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechniqueD1Ev (D0 0x66ba34 skipped)
CColladaHardwareMatrixSkinTechnique::~CColladaHardwareMatrixSkinTechnique() {
    hardwareData.clear();
}

// 0x66b938 _ZNK6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique16needOutputBufferEv
bool CColladaHardwareMatrixSkinTechnique::needOutputBuffer() const {
    return false;
}

// 0x66b940 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique6attachEPNS_5scene10ISceneNodeE
void CColladaHardwareMatrixSkinTechnique::attach(scene::ISceneNode* node) {
    attachedNode = node;
}

// 0x66b948 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique28onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
int CColladaHardwareMatrixSkinTechnique::onPrepareBufferForRendering(E_PREPARE_BUFFER_STEP step, video::IVideoDriver*, SSkinBuffer& buffer,
                                                                     scene::CMeshBuffer* meshBuffer) {
    if (step == 1) {
        skin(buffer, meshBuffer);
    }
    return 16;
}

// 0x66ba50 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique20checkAvailabilityStaticERKNS_5video10STechniqueE
// True when the first pass shader has a parameter of type 11 and the passes' attribute masks together hold 0x30000000.
bool CColladaHardwareMatrixSkinTechnique::checkAvailabilityStatic(const video::STechnique& technique) {
    const std::uint32_t masks = passAttributeMasks(technique);
    return firstShader(technique)->getParameterID(11, 0, 0) != 0xffff && (masks & 0x30000000u) == 0x30000000u;
}

// 0x66beac _ZNK6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique17checkAvailabilityERKNS_5video10STechniqueE
bool CColladaHardwareMatrixSkinTechnique::checkAvailability(const video::STechnique& technique) const {
    const bool available = checkAvailabilityStatic(technique);
    if (available) {
        makeProxyStatic(*skinRecord);
    }
    return available;
}

// 0x66c544 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique15preparePtrCacheEv
void CColladaHardwareMatrixSkinTechnique::preparePtrCache() {
    detail::preparePtrCache(*skinRecord, *cache, attachedNode);
}

// 0x66c5f4 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique18computeBoundingBoxEv
core::aabbox3d<float> CColladaHardwareMatrixSkinTechnique::computeBoundingBox() {
    return computeSkinBoundingBox(*skinRecord, *cache, attachedNode);
}

// 0x66c8a4 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique4initERNS0_11SSkinBufferEPNS_5scene11CMeshBufferEPNS_5video12IVideoDriverEb
int CColladaHardwareMatrixSkinTechnique::init(SSkinBuffer& buffer, scene::CMeshBuffer* meshBuffer, video::IVideoDriver* driver, bool) {
    initProxyBuffer(meshBuffer, buffer, *skinRecord, driver);
    makeProxyStatic(*skinRecord);
    return 0;
}

// 0x66cd18 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique12prepareCacheEv
// When the palette bit (0x01) of the cache is set: prepares the pointer cache, sizes the palette to the joint count (identity fill),
// then for every joint pointer multiplies the joint's matrix with its inverse bind matrix and the result with the skin's bind shape
// matrix (the products are core's operator*<float, glitch::collada::SMatrix>, see multiplyBySMatrix). Clears the bit.
void CColladaHardwareMatrixSkinTechnique::prepareCache() {
    if ((cache->dirty & 1u) == 0) {
        return;
    }
    preparePtrCache();
    cache->palette.resize(skinRecord->jointCount, core::CMatrix4<float>(core::CMatrix4<float>::EM4CONST_IDENTITY));
    const std::size_t count = cache->jointMatrices.size();
    for (std::size_t i = 0; i < count; ++i) {
        cache->palette[i] = multiplyBySMatrix(*cache->jointMatrices[i], skinRecord->bindMatrices + 16 * i);
        cache->palette[i] = multiplyBySMatrix(cache->palette[i], skinRecord->bindShapeMatrix);
    }
    cache->dirty &= ~1u;
}

// 0x66c278 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique4skinERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
// PARTIAL: prepares the cache, finds (or creates) the parameter ids of the buffer's material renderer in the map, and writes the
// palette and the weight-count selector into the material through IMaterialParameters<CMaterial>::setParameter, which glitch-video
// does not expose on CMaterial. Not reconstructed.
int CColladaHardwareMatrixSkinTechnique::skin(SSkinBuffer&, scene::CMeshBuffer*) {
    prepareCache();
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
// CColladaHardwareQuatSkinTechnique
// ---------------------------------------------------------------------------------------------------------------------

// 0x66cfc0 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechniqueC1ERNS0_5SSkinERNS0_10SSkinCacheEb (C2 0x66d01c)
CColladaHardwareQuatSkinTechnique::CColladaHardwareQuatSkinTechnique(SSkin& skin, SSkinCache& skinCache, bool hasSkinData)
    : IColladaSkinTechnique(hasSkinData), skinRecord(&skin), cache(&skinCache) {}

// 0x66df04 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechniqueD1Ev (D0 0x66df6c skipped)
CColladaHardwareQuatSkinTechnique::~CColladaHardwareQuatSkinTechnique() {
    hardwareData.clear();
}

// 0x66d078 _ZNK6glitch7collada6detail33CColladaHardwareQuatSkinTechnique16needOutputBufferEv
bool CColladaHardwareQuatSkinTechnique::needOutputBuffer() const {
    return false;
}

// 0x66d080 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique6attachEPNS_5scene10ISceneNodeE
void CColladaHardwareQuatSkinTechnique::attach(scene::ISceneNode* node) {
    attachedNode = node;
}

// 0x66d088 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique28onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
int CColladaHardwareQuatSkinTechnique::onPrepareBufferForRendering(E_PREPARE_BUFFER_STEP step, video::IVideoDriver*, SSkinBuffer& buffer,
                                                                   scene::CMeshBuffer* meshBuffer) {
    if (step == 1) {
        skin(buffer, meshBuffer);
    }
    return 16;
}

// 0x66d730 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique20checkAvailabilityStaticERKNS_5video10STechniqueE
// True when the first pass shader has two parameters of type 14 (the second searched from the first one's id + 1) whose definitions differ in
// the byte at +7, and the passes' attribute masks together hold 0x30000000.
bool CColladaHardwareQuatSkinTechnique::checkAvailabilityStatic(const video::STechnique& technique) {
    const std::uint32_t masks = passAttributeMasks(technique);
    const video::IShader* shader = firstShader(technique);
    const std::uint32_t first = shader->getParameterID(14, 0, 0);
    if (first == 0xffff) {
        return false;
    }
    const std::uint32_t second = shader->getParameterID(14, 0, static_cast<std::uint16_t>(first + 1));
    if (second == 0xffff) {
        return false;
    }
    const video::SShaderParameterDef* parameters = shader->m_stageParameters[0].parameters;
    return (parameters[first].field06 >> 8) != (parameters[second].field06 >> 8) && (masks & 0x30000000u) == 0x30000000u;
}

// 0x66d8c8 _ZNK6glitch7collada6detail33CColladaHardwareQuatSkinTechnique17checkAvailabilityERKNS_5video10STechniqueE
bool CColladaHardwareQuatSkinTechnique::checkAvailability(const video::STechnique& technique) const {
    const bool available = checkAvailabilityStatic(technique);
    if (available) {
        makeProxyStatic(*skinRecord);
    }
    return available;
}

// 0x66df88 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique15preparePtrCacheEv
void CColladaHardwareQuatSkinTechnique::preparePtrCache() {
    detail::preparePtrCache(*skinRecord, *cache, attachedNode);
}

// 0x66e384 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique18computeBoundingBoxEv
core::aabbox3d<float> CColladaHardwareQuatSkinTechnique::computeBoundingBox() {
    return computeSkinBoundingBox(*skinRecord, *cache, attachedNode);
}

// 0x66e634 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique4initERNS0_11SSkinBufferEPNS_5scene11CMeshBufferEPNS_5video12IVideoDriverEb
int CColladaHardwareQuatSkinTechnique::init(SSkinBuffer& buffer, scene::CMeshBuffer* meshBuffer, video::IVideoDriver* driver, bool) {
    initProxyBuffer(meshBuffer, buffer, *skinRecord, driver);
    makeProxyStatic(*skinRecord);
    return 0;
}

// 0x66e204 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique12prepareCacheEv
// PARTIAL: when the dual-quaternion bit (0x02) is set, prepares the pointer cache, sizes the dual quaternion vector to the joint
// count (the unit dual quaternion fills new entries) and converts each joint matrix times its bind matrix times the bind shape
// matrix with core::rowMatrixProduct34 and dual_quaternion::operator=(matrix). Neither helper is declared in glitch-core-io's headers.
void CColladaHardwareQuatSkinTechnique::prepareCache() {}

// 0x66dc40 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique4skinERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
// PARTIAL: writes the dual quaternions into the buffer's material through IMaterialParameters<CMaterial>::setParameter (not exposed
// on CMaterial in glitch-video). Not reconstructed.
int CColladaHardwareQuatSkinTechnique::skin(SSkinBuffer&, scene::CMeshBuffer*) {
    prepareCache();
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
// CColladaSoftwareSkinTechnique
// ---------------------------------------------------------------------------------------------------------------------

// 0x66f5f8 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechniqueC1ERNS0_5SSkinERNS0_10SSkinCacheEb (C2 0x66f658)
CColladaSoftwareSkinTechnique::CColladaSoftwareSkinTechnique(SSkin& skin, SSkinCache& skinCache, bool hasSkinData)
    : IColladaSkinTechnique(hasSkinData), skinRecord(&skin), cache(&skinCache) {}

// 0x66f724 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechniqueD1Ev (D0 0x66f78c skipped)
CColladaSoftwareSkinTechnique::~CColladaSoftwareSkinTechnique() {
    colorGenData.clear();
}

// 0x66f6b8 _ZNK6glitch7collada6detail29CColladaSoftwareSkinTechnique16needOutputBufferEv
bool CColladaSoftwareSkinTechnique::needOutputBuffer() const {
    return true;
}

// 0x66f6c0 _ZNK6glitch7collada6detail29CColladaSoftwareSkinTechnique17checkAvailabilityERKNS_5video10STechniqueE
bool CColladaSoftwareSkinTechnique::checkAvailability(const video::STechnique&) const {
    return true;
}

// 0x66f6c8 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique6attachEPNS_5scene10ISceneNodeE
void CColladaSoftwareSkinTechnique::attach(scene::ISceneNode* node) {
    attachedNode = node;
}

// 0x66f7a8 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique28onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
// Asks the driver for the process buffer of the buffer's mesh buffer (type 1 when the step is 0, the attribute mask 0x20001 or, with the
// byte at +0x18 set, 0x60001, masked with the vertex attribute mask of the technique's first pass shader) and, when the answer has
// bit 4, skins the buffer (K 0x18). The answer is returned.
int CColladaSoftwareSkinTechnique::onPrepareBufferForRendering(E_PREPARE_BUFFER_STEP step, video::IVideoDriver* driver, SSkinBuffer& buffer,
                                                               scene::CMeshBuffer* meshBuffer) {
    const std::uint32_t wanted = unk_18 != 0 ? 0x60001u : 0x20001u;
    boost::intrusive_ptr<scene::CMeshBuffer> held = buffer.buffer;
    const video::STechnique& technique = buffer.material->m_renderer->m_techniques[buffer.material->getTechnique()];
    const std::uint32_t flags = wanted & firstShader(technique)->m_vertexAttributeMask;
    const int answer = driver->getProcessBuffer(step == 0 ? 1 : 0, flags, held, false);
    if ((answer & 4) != 0) {
        skin(buffer, meshBuffer);
    }
    return answer;
}

// 0x66fab4 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique15preparePtrCacheEv
void CColladaSoftwareSkinTechnique::preparePtrCache() {
    detail::preparePtrCache(*skinRecord, *cache, attachedNode);
}

// 0x66fb84 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique18computeBoundingBoxEv
core::aabbox3d<float> CColladaSoftwareSkinTechnique::computeBoundingBox() {
    return computeSkinBoundingBox(*skinRecord, *cache, attachedNode);
}

// 0x66f8ec _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique4initERNS0_11SSkinBufferEPNS_5scene11CMeshBufferEPNS_5video12IVideoDriverEb
// PARTIAL: the 456-byte body sets up the output stream table (initProxyBuffer, CVertexStreams::setStream records, the material's
// technique chain and the driver's process buffer). Not reconstructed; the answer is 0.
int CColladaSoftwareSkinTechnique::init(SSkinBuffer&, scene::CMeshBuffer*, video::IVideoDriver*, bool) {
    return 0;
}

// 0x66fe34 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique12prepareCacheEv
// PARTIAL: as the matrix technique's prepareCache (products of joint matrices with core's operator*<float, SMatrix>).
void CColladaSoftwareSkinTechnique::prepareCache() {
    if ((cache->dirty & 1u) == 0) {
        return;
    }
    preparePtrCache();
    cache->palette.resize(skinRecord->jointCount, core::CMatrix4<float>(core::CMatrix4<float>::EM4CONST_IDENTITY));
    cache->dirty &= ~1u;
}

// 0x66ff48 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique4skinERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
// PARTIAL: the 1752-byte vertex skinning loop (positions, normals, tangents through the weighted palette). Not reconstructed.
int CColladaSoftwareSkinTechnique::skin(SSkinBuffer&, scene::CMeshBuffer*) {
    return 0;
}

}  // namespace glitch::collada::detail
