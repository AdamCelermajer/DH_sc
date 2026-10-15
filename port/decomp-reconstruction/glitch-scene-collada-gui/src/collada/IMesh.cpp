// Bodies of glitch::collada::IMesh, CMesh and CMorphingMesh (canonical classes: collada/IMesh.hpp). This file replaces
// src/tb_scene-collada-gui-glitch_collada_CMesh-0.cpp, src/tb_scene-collada-gui-glitch_collada_CMesh_SBuffer-0.cpp and the
// CMesh / CMorphingMesh bodies of src/collada_scene_accessors.cpp. With the counting intrusive_ptr the reference moves that
// those files could not express are done by the handles, so the getters and setMaterial are no longer partial.
#include "glitch-scene-collada-gui/collada/IMesh.hpp"

// CMaterial and CMaterialVertexAttributeMap complete (the counting handles need the reference-counting overloads).
#include "glitch-video/material.hpp"
#include "glitch-video/material_vertex_attribute_map.hpp"
#include "glitch-video/video_driver.hpp"
#include "glitch-scene-collada-gui/collada/CRootSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/CMeshBuffer.hpp"

namespace glitch::collada {


// ---------------------------------------------------------------------------------------------------------------------
// IMesh
// ---------------------------------------------------------------------------------------------------------------------

// 0x667704 _ZN6glitch7collada5IMeshD1Ev (D0 0x667738 skipped; the ledger has two rows with this name)
// Restores the vtable and destroys the CColladaDatabase at +0x0c, which drops the record's use (dropDatabase).
IMesh::~IMesh() {
    dropDatabase(database);
}

// 0x6676b4 _ZN6glitch7collada5IMesh9onAnimateEj (empty)
void IMesh::onAnimate(std::uint32_t /*timeMs*/) {}

// 0x6676b8 _ZN6glitch7collada5IMesh28onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverEj
// The pseudocode body is `return 16`.
std::int32_t IMesh::onPrepareBufferForRendering(E_PREPARE_BUFFER_STEP /*step*/, video::IVideoDriver* /*driver*/,
                                                std::uint32_t /*index*/) {
    return 16;
}

// 0x6676c0 _ZN6glitch7collada5IMesh20releaseProcessBufferEPNS_5video12IVideoDriverEj (empty)
void IMesh::releaseProcessBuffer(video::IVideoDriver* /*driver*/, std::uint32_t /*index*/) {}

// 0x6676c4 _ZN6glitch7collada5IMesh4initEPNS_5video12IVideoDriverEb (empty)
void IMesh::init(video::IVideoDriver* /*driver*/, bool /*flag*/) {}

// 0x6676c8 _ZN6glitch7collada5IMesh12setTransformEPNS_5video12IVideoDriverERKNS_4core8CMatrix4IfEE
// Calls the driver's vtable slot +0x6c (setTransform) with the state 1 and the matrix.
void IMesh::setTransform(video::IVideoDriver* driver, const core::CMatrix4<float>& matrix) {
    driver->setTransform(1, matrix);
}

// ---------------------------------------------------------------------------------------------------------------------
// CMesh
// ---------------------------------------------------------------------------------------------------------------------

CMesh::SBuffer::SBuffer() = default;
CMesh::SBuffer::SBuffer(const SBuffer& other) = default;
CMesh::SBuffer& CMesh::SBuffer::operator=(const SBuffer& other) = default;

// 0x644c88 _ZN6glitch7collada5CMesh7SBufferD1Ev
// Drops the vertex-attribute map, the material and then the buffer (the members are destroyed in reverse declaration order, which
// is that order). The ledger row was partial because the old intrusive_ptr did not count.
CMesh::SBuffer::~SBuffer() = default;

// Not in the binary (see the header): the reference count starts at 0 like every CMesh constructor; the members keep their defaults.
CMesh::CMesh() {
    referenceCounter = 0;
}

// 0x645588 _ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESB_b
// (C2 0x645b68)
// PARTIAL. Written: the reference count 0, the database copied with a use on its record, the uid (the geometry's name), and the box left at
// the unit default. Not written: the loop over the geometry's mesh buffers (SGeometry +0x0c is the record with the buffer count at
// +0x0c, the buffer array at +0x10 with 56-byte entries, the box at +0x14..+0x28, and the on-demand index and vertex data), which
// builds one scene::CMeshBuffer per entry through CMeshBuffer(driver, mesh, index, vertexConfig, indexConfig, onDemand) and reuses the
// buffer the entry caches at +0x34 when `onDemand` is set. SGeometry does not map that record, and the CMeshBuffer constructor is partial.
CMesh::CMesh(const CColladaDatabase& db, video::IVideoDriver* /*driver*/, const SGeometry& geometry, const SBufferConfig& /*vertexConfig*/,
             const SBufferConfig& /*indexConfig*/, bool /*onDemand*/) {
    referenceCounter = 0;
    copyDatabase(database, db);
    uid = geometry.name;
    bufferFlags = 0;
}

// 0x644eb8 _ZN6glitch7collada5CMeshC1ERKS1_ (C2 0x644fc8)
// The reference count starts at 0, the uid is the empty-string constant (not copied), the database is copied with a use on its
// record, word +0x14 is cleared, the buffer vector is copied (each entry takes its three uses) and the box words are copied.
CMesh::CMesh(const CMesh& other) {
    referenceCounter = 0;
    uid = "";
    copyDatabase(database, other.database);
    bufferFlags = 0;
    buffers = other.buffers;
    boundingBox = other.boundingBox;
}

// 0x644cfc _ZN6glitch7collada5CMeshD1Ev (D0 0x644d68 and D2 0x644d4c skipped)
// Destroys the buffer vector (each entry drops its three uses), then the IMesh part.
CMesh::~CMesh() = default;

// 0x644f88 _ZNK6glitch7collada5CMesh5cloneEv
// A new mesh (operator new(0x3c)) built by the copy constructor; the result holds the only use.
boost::intrusive_ptr<scene::IMesh> CMesh::clone() const {
    auto* copy = new CMesh(*this);
    return boost::intrusive_ptr<scene::IMesh>(copy, true);
}

// 0x644a44 _ZNK6glitch7collada5CMesh18getMeshBufferCountEv
std::uint32_t CMesh::getMeshBufferCount() const {
    return static_cast<std::uint32_t>(buffers.size());
}

// 0x644a6c _ZNK6glitch7collada5CMesh13getMeshBufferEj
// An index at or past the end gives a null handle; otherwise the handle is copied (a use is added).
boost::intrusive_ptr<scene::CMeshBuffer> CMesh::getMeshBuffer(std::uint32_t index) const {
    if (index >= buffers.size()) {
        return boost::intrusive_ptr<scene::CMeshBuffer>();
    }
    return buffers[index].buffer;
}

// 0x644ac8 _ZNK6glitch7collada5CMesh11getMaterialEj
boost::intrusive_ptr<video::CMaterial> CMesh::getMaterial(std::uint32_t index) const {
    if (index >= buffers.size()) {
        return boost::intrusive_ptr<video::CMaterial>();
    }
    return buffers[index].material;
}

// 0x644b24 _ZNK6glitch7collada5CMesh24getMaterialVertexAttributeMapEj
boost::intrusive_ptr<video::CMaterialVertexAttributeMap> CMesh::getMaterialVertexAttributeMap(std::uint32_t index) const {
    if (index >= buffers.size()) {
        return boost::intrusive_ptr<video::CMaterialVertexAttributeMap>();
    }
    return buffers[index].vertexAttributeMap;
}

// 0x644bdc _ZN6glitch7collada5CMesh11setMaterialEjRKN5boost13intrusive_ptrINS_5video9CMaterialEEERKNS4_INS5_27CMaterialVertexAttributeMapEEE
// An index past the end is ignored. The material is stored first, then the map; each takes a use and drops the old handle.
void CMesh::setMaterial(std::uint32_t index, const boost::intrusive_ptr<video::CMaterial>& material,
                        const boost::intrusive_ptr<video::CMaterialVertexAttributeMap>& vertexAttributeMap) {
    if (index >= buffers.size()) {
        return;
    }
    SBuffer& slot = buffers[index];
    slot.material = material;                      // +0x04
    slot.vertexAttributeMap = vertexAttributeMap;  // +0x08
}

// 0x644b80 _ZNK6glitch7collada5CMesh14getBoundingBoxEv
const core::aabbox3d<float>& CMesh::getBoundingBox() const {
    return boundingBox;
}

// 0x644b88 _ZN6glitch7collada5CMesh14setBoundingBoxERKNS_4core8aabbox3dIfEE
// Copies the six words of the box into +0x24 .. +0x3b.
void CMesh::setBoundingBox(const core::aabbox3d<float>& box) {
    boundingBox = box;
}

// 0x644a3c _ZNK6glitch7collada5CMesh7getTypeEv (returns 0)
std::uint32_t CMesh::getType() const {
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
// CMorphingMesh
// ---------------------------------------------------------------------------------------------------------------------

CMorphingMesh::SBuffer::SBuffer() = default;
CMorphingMesh::SBuffer::SBuffer(const SBuffer& other) = default;
CMorphingMesh::SBuffer& CMorphingMesh::SBuffer::operator=(const SBuffer& other) = default;
CMorphingMesh::SBuffer::~SBuffer() = default;

// Not in the binary (see the header): the reference count starts at 0; the members keep their defaults.
CMorphingMesh::CMorphingMesh() {
    referenceCounter = 0;
}

// 0x64b9c8 _ZN6glitch7collada13CMorphingMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERNS0_11SControllerEPNS0_14CRootSceneNodeE
// 0x64baa0 (C2)
// PARTIAL. Written: the reference count 0, the database copy with a use on its record, the vectors cleared, the controller name as
// the uid, the root node and the index -1. Not written: the controller's morph record (SController +0x08 is a 32-bit pointer into
// the file data, so `morphData` stays null) and the final instanciateMesh call, which builds the morph targets from the database.
CMorphingMesh::CMorphingMesh(const CColladaDatabase& db, video::IVideoDriver* /*driver*/, SController& controller,
                             CRootSceneNode* root) {
    referenceCounter = 0;
    copyDatabase(database, db);
    uid = controller.name;
    rootNode = root;
    lastIndex = -1;
}

// 0x64a1b8 _ZN6glitch7collada13CMorphingMeshD1Ev (D2 0x64a248, D0 0x64a264); the destructor's only statement before the
// member destruction is `CRootSceneNode::removeMorphingMesh(rootNode, this)` (+0x38 is the root node: the constructor
// writes the root argument there, STR R2,[R4,#0x38] at 0x64ba70). The buffers and the targets are then destroyed, which
// the member destructors do. One host deviation, named: the root node is tested for null, because the port's test-only
// default constructor leaves +0x38 null where the original's constructor always stores a root. The real constructor
// stores a root, so the deviation only covers the not-in-the-binary empty mesh.
CMorphingMesh::~CMorphingMesh() {
    if (rootNode != nullptr) {
        rootNode->removeMorphingMesh(this);
    }
}

// 0x64990c _ZNK6glitch7collada13CMorphingMesh5cloneEv
// Returns a null handle (the original stores 0 into the result slot).
boost::intrusive_ptr<scene::IMesh> CMorphingMesh::clone() const {
    return boost::intrusive_ptr<scene::IMesh>();
}

// 0x649918 _ZNK6glitch7collada13CMorphingMesh18getMeshBufferCountEv
// Forwards to the first morph target (vtable +0x10). The original does not test for an empty target list.
std::uint32_t CMorphingMesh::getMeshBufferCount() const {
    if (targets.empty() || targets.front().mesh.get() == nullptr) {
        return 0;
    }
    return targets.front().mesh->getMeshBufferCount();
}

// 0x649938 _ZNK6glitch7collada13CMorphingMesh13getMeshBufferEj
// Reads the own buffer vector at the index and takes a use. The original does not test the index; out of range gives null here.
boost::intrusive_ptr<scene::CMeshBuffer> CMorphingMesh::getMeshBuffer(std::uint32_t index) const {
    if (index >= buffers.size()) {
        return boost::intrusive_ptr<scene::CMeshBuffer>();
    }
    return buffers[index].buffer;
}

// 0x649960 _ZNK6glitch7collada13CMorphingMesh11getMaterialEj
boost::intrusive_ptr<video::CMaterial> CMorphingMesh::getMaterial(std::uint32_t index) const {
    if (index >= buffers.size()) {
        return boost::intrusive_ptr<video::CMaterial>();
    }
    return buffers[index].material;
}

// 0x649988 _ZNK6glitch7collada13CMorphingMesh24getMaterialVertexAttributeMapEj
boost::intrusive_ptr<video::CMaterialVertexAttributeMap> CMorphingMesh::getMaterialVertexAttributeMap(std::uint32_t index) const {
    if (index >= buffers.size()) {
        return boost::intrusive_ptr<video::CMaterialVertexAttributeMap>();
    }
    return buffers[index].vertexAttributeMap;
}

// 0x649a88 _ZN6glitch7collada13CMorphingMesh11setMaterialEjRKN5boost13intrusive_ptrINS_5video9CMaterialEEERKNS4_INS5_27CMaterialVertexAttributeMapEEE
// Forwards to the first morph target (vtable +0x20), then stores the material and the map in the own buffer entry. The original
// does not test the target list or the index.
void CMorphingMesh::setMaterial(std::uint32_t index, const boost::intrusive_ptr<video::CMaterial>& material,
                                const boost::intrusive_ptr<video::CMaterialVertexAttributeMap>& vertexAttributeMap) {
    if (!targets.empty() && targets.front().mesh.get() != nullptr) {
        targets.front().mesh->setMaterial(index, material, vertexAttributeMap);
    }
    if (index < buffers.size()) {
        buffers[index].material = material;                      // +0x04
        buffers[index].vertexAttributeMap = vertexAttributeMap;  // +0x08
    }
}

// 0x6499b0 _ZNK6glitch7collada13CMorphingMesh14getBoundingBoxEv
// Forwards to the first morph target (vtable +0x24). The original does not test the target list; an empty list gives an empty box.
const core::aabbox3d<float>& CMorphingMesh::getBoundingBox() const {
    static const core::aabbox3d<float> kEmpty{};
    if (targets.empty() || targets.front().mesh.get() == nullptr) {
        return kEmpty;
    }
    return targets.front().mesh->getBoundingBox();
}

// 0x6499d0 _ZN6glitch7collada13CMorphingMesh14setBoundingBoxERKNS_4core8aabbox3dIfEE (empty)
void CMorphingMesh::setBoundingBox(const core::aabbox3d<float>& /*box*/) {}

// 0x649904 _ZNK6glitch7collada13CMorphingMesh7getTypeEv (returns 1)
std::uint32_t CMorphingMesh::getType() const {
    return 1;
}

// 0x64afec _ZN6glitch7collada13CMorphingMesh28onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverEj
// PARTIAL: the 456-byte body (the per-step vertex stream preparation of the morph buffers) is not reconstructed; the base value
// is returned.
std::int32_t CMorphingMesh::onPrepareBufferForRendering(E_PREPARE_BUFFER_STEP step, video::IVideoDriver* driver,
                                                        std::uint32_t index) {
    return IMesh::onPrepareBufferForRendering(step, driver, index);
}

// 0x64a2f4 _ZN6glitch7collada13CMorphingMesh20releaseProcessBufferEPNS_5video12IVideoDriverEj
// PARTIAL: the 192-byte body (it drops the process buffers of the morph buffers) is not reconstructed.
void CMorphingMesh::releaseProcessBuffer(video::IVideoDriver* /*driver*/, std::uint32_t /*index*/) {}

// 0x64a3b4 _ZN6glitch7collada13CMorphingMesh4initEPNS_5video12IVideoDriverEb
// PARTIAL: the 808-byte body (it creates the morph buffers from the first target) is not reconstructed.
void CMorphingMesh::init(video::IVideoDriver* /*driver*/, bool /*flag*/) {}

// 0x64a6e4 _ZN6glitch7collada13CMorphingMesh5morphEj. PARTIAL: the blend over the targets is not reconstructed; does nothing.
// (The stand-in recon_20::CMorphingMesh_morph returned 0 and was retired with the slice_20 stand-ins.)
void CMorphingMesh::morph(std::uint32_t /*timeMs*/) {}

// 0x64b700 _ZN6glitch7collada13CMorphingMesh15instanciateMeshEPNS_5video12IVideoDriverEPNS0_14CRootSceneNodeE. PARTIAL: the targets
// are not built from the database; does nothing.
void CMorphingMesh::instanciateMesh(video::IVideoDriver* /*driver*/, CRootSceneNode* /*root*/) {}

}  // namespace glitch::collada
