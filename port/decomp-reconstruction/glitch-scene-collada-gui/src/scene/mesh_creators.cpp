// The glitch::scene mesh generators (createTerrainMesh, createPlaneMesh, createSphereMesh, createHillPlaneMesh, createCubeMesh,
// createCylinderMesh, createConeMesh, createArrowMesh). Declarations: scene/mesh_functions.hpp. This file replaces the unattached
// recon_28 stubs of src/recon/slice_28.cpp (their symbols were in glitch::scene::recon_28, not the originals).
//
// The generators build a CMesh with one CMeshBuffer each. The buffer comes from the anonymous-namespace helper createMeshBuffer (0x6d6e2c:
// a vertex stream table of the requested attribute mask and a 16-bit index buffer, both created through the driver's buffer factory,
// vptr+0x78) and is filled through mapMeshBuffer (0x6d71d8: SVertexStream::SMapBuffer views of the position, texture coordinate, normal
// and colour streams). glitch-video does not reconstruct IBuffer::map / unmap, CVertexStreams::setupStreams / getStream or the
// SMapBuffer views, so the buffers cannot be built here: the generators other than the terrain one return a null handle (a mesh cache
// that asks for them caches nothing).
#include "glitch-scene-collada-gui/scene/mesh_functions.hpp"

namespace glitch::scene {

// 0x6d6e00 _ZN6glitch5scene17createTerrainMeshERKN5boost13intrusive_ptrINS_5video6CImageEEES7_RKNS_4core11dimension2dIfEEfPNS3_12IVideoDriverERKNS9_IiEEb
// The whole body is the store of a null handle into the result.
boost::intrusive_ptr<IMesh> createTerrainMesh(const boost::intrusive_ptr<video::CImage>& /*texture*/,
                                              const boost::intrusive_ptr<video::CImage>& /*heightMap*/,
                                              const core::dimension2d<float>& /*stretchSize*/, float /*maxHeight*/,
                                              video::IVideoDriver* /*driver*/,
                                              const core::dimension2d<std::int32_t>& /*defaultVertexBlockSize*/, bool /*debugBorders*/) {
    return boost::intrusive_ptr<IMesh>();
}

// 0x6d72d0 _ZN6glitch5scene15createPlaneMeshEjPNS_5video12IVideoDriverEfRKNS1_6SColorE
// PARTIAL: a four vertex quad of the given size (corners from a static table of positions, normals and texture coordinates, all four
// vertices coloured with the argument) and six indices in a CMesh; needs the buffer mapping described above. Returns a null handle.
boost::intrusive_ptr<IMesh> createPlaneMesh(std::uint32_t, video::IVideoDriver*, float, const video::SColor&) {
    return boost::intrusive_ptr<IMesh>();
}

// 0x6d7808 _ZN6glitch5scene16createSphereMeshEjPNS_5video12IVideoDriverEfjj
// PARTIAL: the UV sphere generator (both tessellation counts at least 2 and clamped so the vertex count stays below 0x8000); needs the
// buffer mapping described above. Returns a null handle.
boost::intrusive_ptr<IMesh> createSphereMesh(std::uint32_t, video::IVideoDriver*, float, std::uint32_t, std::uint32_t) {
    return boost::intrusive_ptr<IMesh>();
}

// 0x6d841c _ZN6glitch5scene19createHillPlaneMeshEjPNS_5video12IVideoDriverERKNS_4core11dimension2dIfEERKNS5_IjEERKN5boost13intrusive_ptrINS1_9CMaterialEEERKNSD_INS1_27CMaterialVertexAttributeMapEEEfS8_S8_
// PARTIAL: see createPlaneMesh. Returns a null handle.
boost::intrusive_ptr<IMesh> createHillPlaneMesh(std::uint32_t, video::IVideoDriver*, const core::dimension2d<float>&,
                                                const core::dimension2d<std::uint32_t>&, const boost::intrusive_ptr<video::CMaterial>&,
                                                const boost::intrusive_ptr<video::CMaterialVertexAttributeMap>&, float,
                                                const core::dimension2d<float>&, const core::dimension2d<float>&) {
    return boost::intrusive_ptr<IMesh>();
}

// 0x6d8cb4 _ZN6glitch5scene14createCubeMeshEjPNS_5video12IVideoDriverEf
// PARTIAL: see createPlaneMesh. Returns a null handle.
boost::intrusive_ptr<IMesh> createCubeMesh(std::uint32_t, video::IVideoDriver*, float) {
    return boost::intrusive_ptr<IMesh>();
}

// 0x6d945c _ZN6glitch5scene18createCylinderMeshEjPNS_5video12IVideoDriverEffjRKNS1_6SColorEbf
// PARTIAL: see createPlaneMesh. Returns a null handle.
boost::intrusive_ptr<IMesh> createCylinderMesh(std::uint32_t, video::IVideoDriver*, float, float, std::uint32_t, const video::SColor&,
                                               bool, float) {
    return boost::intrusive_ptr<IMesh>();
}

// 0x6d9fc4 _ZN6glitch5scene14createConeMeshEjPNS_5video12IVideoDriverEffjRKNS1_6SColorES6_f
// PARTIAL: see createPlaneMesh. Returns a null handle.
boost::intrusive_ptr<IMesh> createConeMesh(std::uint32_t, video::IVideoDriver*, float, float, std::uint32_t, const video::SColor&,
                                           const video::SColor&, float) {
    return boost::intrusive_ptr<IMesh>();
}

// 0x6da6d8 _ZN6glitch5scene15createArrowMeshEjPNS_5video12IVideoDriverEjjffffNS1_6SColorES4_
// PARTIAL: see createPlaneMesh. Returns a null handle.
boost::intrusive_ptr<IMesh> createArrowMesh(std::uint32_t, video::IVideoDriver*, std::uint32_t, std::uint32_t, float, float, float, float,
                                            video::SColor, video::SColor) {
    return boost::intrusive_ptr<IMesh>();
}

}  // namespace glitch::scene
