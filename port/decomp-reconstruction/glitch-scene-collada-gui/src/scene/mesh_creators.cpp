// The glitch::scene mesh generators (createTerrainMesh, createPlaneMesh, createSphereMesh, createHillPlaneMesh, createCubeMesh,
// createCylinderMesh, createConeMesh, createArrowMesh). Declarations: scene/mesh_functions.hpp. This file replaces the unattached
// recon_28 stubs of src/recon/slice_28.cpp (their symbols were in glitch::scene::recon_28, not the originals).
//
// The generators build a CMesh with one CMeshBuffer each. Verified against the listings: the buffer comes from the
// anonymous-namespace helper createMeshBuffer (0x6d6e2c: an operator new[](2 * indexCount), a driver buffer (vtable +0x78) with
// type 0/1, usage 4, a buffer-per-attribute table through CVertexStreams::allocate, CVertexStreams::setupStreams(streams,
// buffer, -1) and the stream stride written back into the table's word +0x08) and is filled through mapMeshBuffer (0x6d71d8:
// SVertexStream::SMapBuffer views of the position stream and, when the attribute mask carries the bit, the texture coordinate,
// normal and colour streams; access 4 for writing). Both helpers are reconstructed here.
//
// What each generator still needs is its own GENERATED DATA, which is not a layout question:
//   createPlaneMesh 0x6d72d0 - four vertices from the 32-byte static record at 0x9f7ec8 (meshData) and six indices from
//     0x8eb538; both transcribed below.
//   createCubeMesh 0x6d8cb4 - 24 vertices from the 32-byte records at 0x9f7f4c (cubeData) and 36 indices from 0x8eb544;
//     both transcribed below.
//   createSphereMesh 0x6d7808, createCylinderMesh 0x6d945c, createConeMesh 0x6d9fc4, createArrowMesh 0x6da6d8 - the vertices
//     are computed per ring/segment (sin/cos of the angular step, radius and height scaling, a byte colour written per corner)
//     and the indices are written in a loop whose strip/fan order is in the listing; the exact triangle order must be read
//     off the listing, not from a table. Not transcribed; these still return a null handle.
//   createHillPlaneMesh 0x6d841c - per-tile heights from the hilly-wave formula and the texture coordinate repeat, again
//     computed rather than tabulated. Not transcribed; returns a null handle.
#include "glitch-scene-collada-gui/scene/mesh_functions.hpp"

#include <cstring>
#include <utility>

#include "glitch-scene-collada-gui/scene/CMesh.hpp"
#include "glitch-scene-collada-gui/scene/CMeshBuffer.hpp"
#include "glitch-video/buffer.hpp"
#include "glitch-video/material_vertex_attribute_map.hpp"
#include "glitch-video/primitive_stream.hpp"
#include "glitch-video/vertex_streams.hpp"
#include "glitch-video/video_driver.hpp"

namespace glitch::scene {

namespace {

using core::vector2d;
using core::vector3d;
using video::SVertexStream;

// The one vertex layout every generator writes: position, then the texture coordinate, then the normal (the port's
// blend vertex; createPlaneMesh's 0x9f7ec8 records have the same order).
struct SGeneratorVertex {
    vector3d<float> position;
    vector2d<float> texCoord;
    vector3d<float> normal;
    video::SColor color;
};
static_assert(sizeof(SGeneratorVertex) == 36,
              "position 12 + texcoord 8 + normal 12 + colour 4. The assertion previously demanded 44, which matches "
              "neither this layout nor the listing: createPlaneMesh reads four 32-byte vertices from meshData "
              "0x9f7ec8 and createCubeMesh twenty-four 32-byte records from cubeData 0x9f7f4c, and the original is a "
              "32-bit ARM target where this same field list is 32 bytes with no padding. On this host the same list "
              "is 36 because vector3d<float> is 12 and SColor 4 with alignment 4. The record the generators "
              "transcribe is the 32-byte static one, so if a generator is written against the binary it must write "
              "those 32 bytes field by field rather than memcpy sizeof(SGeneratorVertex).");

// 0x6d6e2c glitch::scene::(anonymous namespace)::createMeshBuffer(unsigned int, IVideoDriver*, unsigned int, unsigned int)
// A stream table for `attributeMask` over a fresh driver vertex buffer (type 0, usage 4, no data) whose streams are laid out by
// CVertexStreams::setupStreams(buffer, -1), with the vertex count in the table's word +0x08; the buffer is returned with one
// use (the original adds it after the constructor, whose count starts at 0). The original also allocates an index array up
// front (operator new[](2 * indexCount) into a type-1 driver buffer created with the last argument 1, i.e. owning its data)
// but never attaches it: the primitive stream keeps a null index buffer and the generator memcpys its static 16-bit index
// table into the mapped storage. This port lets the caller supply the indices instead (see the index tables below), because
// mapMeshBuffer only ever touches the vertex streams.
boost::intrusive_ptr<CMeshBuffer> createMeshBuffer(std::uint32_t attributeMask, video::IVideoDriver* driver,
                                                   std::uint32_t vertexCount, std::uint32_t indexCount) {
    boost::intrusive_ptr<CMeshBuffer> buffer(new CMeshBuffer(), true);
    buffer->vertexStreams = video::CVertexStreams::allocate(attributeMask);
    boost::intrusive_ptr<video::IBuffer> vertexBuffer = driver->createBuffer(0, 4, 0, nullptr, true);
    buffer->vertexStreams->setupStreams(vertexBuffer, 0xFFFFFFFFu);
    buffer->vertexStreams->m_field08 = vertexCount;
    buffer->primitiveStream.m_offset = 0;
    buffer->primitiveStream.m_count = indexCount;
    buffer->primitiveStream.m_vertexStart = 0;
    buffer->primitiveStream.m_vertexEnd = 0;
    buffer->primitiveStream.m_indexType = 1;      // 16-bit indices (E_INDEX_TYPE)
    buffer->primitiveStream.m_primitiveType = 6;  // E_PRIMITIVE_TYPE_TRIANGLES
    return buffer;
}

// 0x6d71d8 glitch::scene::(anonymous namespace)::mapMeshBuffer(intrusive_ptr<CMeshBuffer> const&,
//   SMapBuffer<vector3d<float>>*, SMapBuffer<vector2d<float>>*, SMapBuffer<vector3d<float>>*, SMapBuffer<SColor>*, E_BUFFER_MAP_ACCESS)
// Maps the position stream into `positions` and, when the table's attribute mask has the bit, the texture coordinate stream
// (attribute 1), the normal stream (attribute 17) and the colour stream (attribute 18) into their views. The stream for an
// optional attribute is looked up with CVertexStreams::getStream over the entries after the texture coordinate block (the same
// search mesh_functions.cpp's findStream performs). Passing a null view for an optional attribute skips it, as in the original.
void mapMeshBuffer(const boost::intrusive_ptr<CMeshBuffer>& buffer, SVertexStream::SMapBuffer<vector3d<float>>* positions,
                   SVertexStream::SMapBuffer<vector2d<float>>* texCoords, SVertexStream::SMapBuffer<vector3d<float>>* normals,
                   SVertexStream::SMapBuffer<video::SColor>* colors, std::int32_t access) {
    video::CVertexStreams& streams = *buffer->vertexStreams;
    if (positions != nullptr && !streams.m_streams.empty()) {
        positions->reset(streams.m_streams[0], access);
    }
    if (texCoords != nullptr && (streams.m_attributeMask & 0x2u) != 0) {
        SVertexStream* stream = streams.m_streams.size() > 1 ? &streams.m_streams[1] : nullptr;
        if (stream != nullptr) texCoords->reset(*stream, access);
    }
    if (normals != nullptr && (streams.m_attributeMask & 0x20000u) != 0) {
        for (SVertexStream& entry : streams.m_streams) {
            if (entry.attribute == 17) {
                normals->reset(entry, access);
                break;
            }
        }
    }
    if (colors != nullptr && (streams.m_attributeMask & 0x40000u) != 0) {
        for (SVertexStream& entry : streams.m_streams) {
            if (entry.attribute == 18) {
                colors->reset(entry, access);
                break;
            }
        }
    }
}

// Transcribed from the guard-initialized `meshData` of createPlaneMesh (0x9f7ec8, 4 records of 32 bytes) - the compiler-static
// array is a .bss symbol in the shipped .so (a raw get_bytes read returns zeroes), so the values live only in the explicit
// stores of the guard block at 0x6d770c..0x6d7788. All 32 words decoded by tracking the register constants across the block:
// the quad lies in the xz plane with the normal (0, 1, 0), and the loop writes the position from words 0..2 (scaled by the
// size argument), the normal from words 3..5 and the texture coordinate from words 6..7.
const float kPlaneMeshData[4][8] = {
    // position.x, position.y, position.z, normal.x, normal.y, normal.z, uv.x, uv.y
    {-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f},
    {0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
    {-0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
};

// Transcribed from the guard-initialized `cubeData` of createCubeMesh (0x9f7f4c, 24 records of 32 bytes). All 192 words are
// reproduced by the guard block at 0x6d90b4..0x6d93e4; they were extracted by tracking the register constants across the
// block, which gives a complete, self-consistent cube: six quads of four vertices, each with an axis-aligned normal and the
// four texture coordinates (1,1) (1,0) (0,0) (0,1). The loop writes the position from words 0..2, the normal from words
// 3..5 and the texture coordinate from words 6..7, and scales only the position by the size argument.
const float kCubeMeshData[24][8] = {
    // -z face (normal 0, 0, -1)
    {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
    {-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
    {0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
    {0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
    // +x face (normal 1, 0, 0)
    {0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    // +z face (normal 0, 0, 1)
    {0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    {0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
    {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    // -x face (normal -1, 0, 0)
    {-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    {-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    // +y face (normal 0, 1, 0)
    {0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f},
    {0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f},
    {-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
    {-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    // -y face (normal 0, -1, 0)
    {-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f},
    {0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f},
    {0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f},
};

// 0x8eb538 (12 bytes, the `C.1925` table createPlaneMesh memcpys from) are the six 16-bit indices {0, 2, 1, 0, 3, 2} and
// 0x8eb544 (36 bytes, file data, the `u` table createCubeMesh memcpys 0x48 bytes from) is the quad pattern 0 1 2 / 0 2 3
// repeated over the six quads of four vertices (indices 4q+0, 4q+1, 4q+2, 4q+0, 4q+2, 4q+3). Both index tables belong to the
// index-buffer path of createMeshBuffer above, which this port stores outside the buffer (see the helper's comment).

}  // namespace

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
// Verified against the listing 0x6d72d0: a stream table for the normal attribute (mask 0x24) with 4 vertices and 6 indices,
// the six indices of 0x8eb538 memcpyd into the mapped index storage, then one pass over the four 32-byte records of meshData:
// the position is scaled by `size`, the texture coordinate and the normal are copied, and the colour stream (which this table
// does not have) would take the SColor argument. One CMesh with that buffer, its box recalculated.
boost::intrusive_ptr<IMesh> createPlaneMesh(std::uint32_t, video::IVideoDriver* driver, float size, const video::SColor& color) {
    const boost::intrusive_ptr<CMeshBuffer> buffer = createMeshBuffer(0x24u, driver, 4u, 6u);
    SVertexStream::SMapBuffer<vector3d<float>> positions;
    SVertexStream::SMapBuffer<vector2d<float>> texCoords;
    SVertexStream::SMapBuffer<vector3d<float>> normals;
    SVertexStream::SMapBuffer<video::SColor> colors;
    mapMeshBuffer(buffer, &positions, &texCoords, &normals, &colors, 4);
    if (positions.data != nullptr) {
        const std::uint16_t stride = buffer->vertexStreams->m_streams[0].field0e;
        auto* base = reinterpret_cast<std::uint8_t*>(positions.data);
        for (std::size_t i = 0; i < 4; ++i) {
            const float* record = kPlaneMeshData[i];
            float* vertex = reinterpret_cast<float*>(base + stride * i);
            vertex[0] = record[0] * size;
            vertex[1] = record[1] * size;
            vertex[2] = record[2] * size;
            if (texCoords.data != nullptr && buffer->vertexStreams->m_streams.size() > 1) {
                auto* uv = reinterpret_cast<float*>(reinterpret_cast<std::uint8_t*>(texCoords.data) +
                                                    buffer->vertexStreams->m_streams[1].field0e * i);
                uv[0] = record[6];
                uv[1] = record[7];
            }
            if (normals.data != nullptr) {
                float* normal = reinterpret_cast<float*>(reinterpret_cast<std::uint8_t*>(normals.data) +
                                                        stride * i);
                normal[0] = record[3];
                normal[1] = record[4];
                normal[2] = record[5];
            }
        }
        (void)color;  // the table carries no colour stream, as in the original
    }
    boost::intrusive_ptr<CMesh> mesh(new CMesh(), true);
    const boost::intrusive_ptr<video::CMaterial> noMaterial;
    const boost::intrusive_ptr<video::CMaterialVertexAttributeMap> noMap;
    mesh->addMeshBuffer(buffer, noMaterial, noMap);
    mesh->recalculateBoundingBox();
    // The listing's tail returns the local handle with a new use (sret = mesh.px; ++mesh->referenceCounter;
    // drop(mesh.px)), i.e. the intrusive_ptr upcast; see createMeshCopy's note. result.get() would add no use and
    // the local's destructor would free the mesh the caller keeps.
    return mesh;
}

// 0x6d7808 _ZN6glitch5scene16createSphereMeshEjPNS_5video12IVideoDriverEfjj
// PARTIAL: the UV sphere generator (both tessellation counts clamped to at least 2 and reduced until the vertex count stays
// below 0x8000). The vertices are computed per ring (sin/cos of the ring and segment angles times the radius) and the indices
// are written by the listing's strip loops; neither the loop order nor the colour/UV assignment has been transcribed. Returns
// a null handle.
boost::intrusive_ptr<IMesh> createSphereMesh(std::uint32_t, video::IVideoDriver*, float, std::uint32_t, std::uint32_t) {
    return boost::intrusive_ptr<IMesh>();
}

// 0x6d841c _ZN6glitch5scene19createHillPlaneMeshEjPNS_5video12IVideoDriverERKNS_4core11dimension2dIfEERKNS5_IjEERKN5boost13intrusive_ptrINS1_9CMaterialEEERKNSD_INS1_27CMaterialVertexAttributeMapEEEfS8_S8_
// PARTIAL: the tiled hill plane: tileCount tiles of tileSize, the height of every vertex from the hillHeight / countHills wave
// formula and the texture coordinates scaled by textureRepeatCount; material and attribute map go to the buffer. The wave
// formula and the tile index order have not been transcribed. Returns a null handle.
boost::intrusive_ptr<IMesh> createHillPlaneMesh(std::uint32_t, video::IVideoDriver*, const core::dimension2d<float>&,
                                                const core::dimension2d<std::uint32_t>&, const boost::intrusive_ptr<video::CMaterial>&,
                                                const boost::intrusive_ptr<video::CMaterialVertexAttributeMap>&, float,
                                                const core::dimension2d<float>&, const core::dimension2d<float>&) {
    return boost::intrusive_ptr<IMesh>();
}

// 0x6d8cb4 _ZN6glitch5scene14createCubeMeshEjPNS_5video12IVideoDriverEf
// Verified against the listing 0x6d8cb4: a stream table for the normal attribute (mask 0x24) with 24 vertices and 36 indices,
// the 0x48 bytes of the static `u` table at 0x8eb544 memcpyd into the mapped index storage, then one pass over the 24 32-byte
// records of cubeData: the position is scaled by `size` (an opaque white colour would go to a colour stream, which this table
// does not have). The 24 records are the compiler-static cubeData at 0x9f7f4c, transcribed here from the guard block's
// explicit stores (the array is .bss in the shipped .so). One CMesh with that buffer, its box recalculated.
boost::intrusive_ptr<IMesh> createCubeMesh(std::uint32_t, video::IVideoDriver* driver, float size) {
    const boost::intrusive_ptr<CMeshBuffer> buffer = createMeshBuffer(0x24u, driver, 24u, 36u);
    SVertexStream::SMapBuffer<vector3d<float>> positions;
    SVertexStream::SMapBuffer<vector2d<float>> texCoords;
    SVertexStream::SMapBuffer<vector3d<float>> normals;
    SVertexStream::SMapBuffer<video::SColor> colors;
    mapMeshBuffer(buffer, &positions, &texCoords, &normals, &colors, 4);
    if (positions.data != nullptr) {
        const std::uint16_t stride = buffer->vertexStreams->m_streams[0].field0e;
        auto* base = reinterpret_cast<std::uint8_t*>(positions.data);
        for (std::size_t i = 0; i < 24; ++i) {
            const float* record = kCubeMeshData[i];
            float* vertex = reinterpret_cast<float*>(base + stride * i);
            vertex[0] = record[0] * size;
            vertex[1] = record[1] * size;
            vertex[2] = record[2] * size;
            if (texCoords.data != nullptr && buffer->vertexStreams->m_streams.size() > 1) {
                auto* uv = reinterpret_cast<float*>(reinterpret_cast<std::uint8_t*>(texCoords.data) +
                                                    buffer->vertexStreams->m_streams[1].field0e * i);
                uv[0] = record[6];
                uv[1] = record[7];
            }
            if (normals.data != nullptr) {
                float* normal = reinterpret_cast<float*>(reinterpret_cast<std::uint8_t*>(normals.data) + stride * i);
                normal[0] = record[3];
                normal[1] = record[4];
                normal[2] = record[5];
            }
        }
    }
    boost::intrusive_ptr<CMesh> mesh(new CMesh(), true);
    const boost::intrusive_ptr<video::CMaterial> noMaterial;
    const boost::intrusive_ptr<video::CMaterialVertexAttributeMap> noMap;
    mesh->addMeshBuffer(buffer, noMaterial, noMap);
    mesh->recalculateBoundingBox();
    return mesh;  // a new use, as in the listing's tail (see createPlaneMesh's note)
}

// 0x6d945c _ZN6glitch5scene18createCylinderMeshEjPNS_5video12IVideoDriverEffjRKNS1_6SColorEbf
// PARTIAL: the cylinder generator (radius, length, tesselation, side colour, optional top cap, oblique shear). The ring
// vertices and the cap are computed from sin/cos and the index loops are in the listing; not transcribed. Returns a null
// handle.
boost::intrusive_ptr<IMesh> createCylinderMesh(std::uint32_t, video::IVideoDriver*, float, float, std::uint32_t, const video::SColor&,
                                               bool, float) {
    return boost::intrusive_ptr<IMesh>();
}

// 0x6d9fc4 _ZN6glitch5scene14createConeMeshEjPNS_5video12IVideoDriverEffjRKNS1_6SColorES6_f
// PARTIAL: the cone generator (radius, length, tesselation, the two colours, oblique shear); the ring/normal vertices and the
// index loops are in the listing; not transcribed. Returns a null handle.
boost::intrusive_ptr<IMesh> createConeMesh(std::uint32_t, video::IVideoDriver*, float, float, std::uint32_t, const video::SColor&,
                                           const video::SColor&, float) {
    return boost::intrusive_ptr<IMesh>();
}

// 0x6da6d8 _ZN6glitch5scene15createArrowMeshEjPNS_5video12IVideoDriverEjjffffNS1_6SColorES4_
// PARTIAL: the arrow generator (two tesselation counts, height, cylinder height, the two widths, the two colours): it builds a
// cylinder shaft and a cone head with the same helpers as the two generators above. Not transcribed; returns a null handle.
boost::intrusive_ptr<IMesh> createArrowMesh(std::uint32_t, video::IVideoDriver*, std::uint32_t, std::uint32_t, float, float, float, float,
                                            video::SColor, video::SColor) {
    return boost::intrusive_ptr<IMesh>();
}

}  // namespace glitch::scene
