#include "../../android-app/scene_buffers.hpp"

#include "CNullDriver.h"
#include "irrlicht.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {
void require(bool ok, const char* message) {
    if (ok) return;
    std::fprintf(stderr, "Irrlicht SceneMesh host probe failed: %s\n", message);
    std::exit(1);
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(static_cast<bool>(input), "cannot read the source BRES");
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
}

int main(int argc, char** argv) {
    require(argc == 2, "usage: scene_mesh_null_host <candle_flame.bdae>");
    auto bytes = read_file(argv[1]);
    dh2::resources::BresView image{};
    require(dh2_bres_open(&image, bytes.data(), bytes.size()) ==
                dh2::resources::BresError::ok,
            "checked BRES reader rejected candle_flame.bdae");

    dh2::viewer::SceneMesh source{};
    require(dh2_viewer_scene_mesh(&source, &image) == dh2::viewer::SceneMeshError::ok,
            "repo SceneMesh assembler rejected candle_flame.bdae");
    require(source.draw_commands == 2 && source.vertex_count == 8 &&
                source.index_count == 12,
            "repo SceneMesh counts differ from the checked candle fixture");
    require(std::strcmp(source.first_diffuse_texture, "env_crypt.tga") == 0 &&
                source.texture_reference_count > 0,
            "repo SceneMesh material/texture result differs from the candle fixture");

    irr::video::CNullDriver driver(nullptr, irr::core::dimension2d<irr::u32>(16, 16));
    require(driver.beginScene(false, false), "Irrlicht Null driver failed beginScene");
    irr::u32 expected_triangles = 0;
    for (irr::u32 d = 0; d < source.draw_commands; ++d) {
        const auto& draw = source.draws[d];
        require(draw.visible != 0 && draw.index_count % 3 == 0,
                "source draw is invisible or is not a triangle list");
        irr::scene::SMeshBuffer target;
        for (irr::u32 i = 0; i < draw.vertex_count; ++i) {
            const auto vertex = draw.first_vertex + i;
            const float* p = source.vertices + 5U * vertex;
            target.Vertices.push_back(irr::video::S3DVertex(
                p[0], p[1], p[2], 0.f, 0.f, 1.f,
                irr::video::SColor(255, 255, 255, 255), p[3], p[4]));
        }
        for (irr::u32 i = 0; i < draw.index_count; ++i) {
            const auto source_index = source.indices[draw.first_index + i];
            require(source_index >= draw.first_vertex &&
                        source_index < draw.first_vertex + draw.vertex_count,
                    "repo SceneMesh index falls outside its draw vertex range");
            target.Indices.push_back(static_cast<irr::u16>(source_index - draw.first_vertex));
        }
        target.recalculateBoundingBox();
        target.Material.Lighting = false;
        driver.setMaterial(target.Material);
        driver.drawMeshBuffer(&target);
        expected_triangles += draw.index_count / 3;

        std::printf("draw=%u geometry=%s material=%s material_name=%s vertices=%u triangles=%u textures=%u",
            d, draw.geometry_id, draw.material_id, draw.material_name,
            draw.vertex_count, draw.index_count / 3, draw.texture_count);
        for (irr::u32 t = 0; t < draw.texture_count; ++t) {
            const auto& reference = source.texture_references[draw.first_texture + t];
            std::printf(" sampler=%s image=%s path=%s",
                reference.parameter_id, reference.image_name, reference.source_path);
        }
        std::putchar('\n');
    }
    require(driver.endScene(), "Irrlicht Null driver failed endScene");
    require(driver.getPrimitiveCountDrawn() == expected_triangles,
            "Irrlicht Null driver did not consume every triangle from SceneMesh");
    require(expected_triangles == 4, "candle fixture should provide four triangles");
    std::printf("pass=true engine=Irrlicht-1.8.5 driver=null draws=%u triangles=%u source_mesh=%ux%u diffuse=%s\n",
        source.draw_commands, expected_triangles, source.vertex_count, source.index_count,
        source.first_diffuse_texture);
    dh2_viewer_scene_mesh_free(&source);
    return 0;
}
