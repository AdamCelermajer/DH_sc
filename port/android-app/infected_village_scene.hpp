#pragma once

#include "scene_buffers.hpp"
#include "../world-data/world.hpp"

#include <cstddef>
#include <cstdint>

namespace dh2::infectedpreview {
constexpr std::uint32_t module_count = 2;

struct SourceFiles {
    const std::uint8_t* mlx; std::size_t mlx_size;
    const std::uint8_t* catalogue; std::size_t catalogue_size;
    const std::uint8_t* mgp[module_count]; std::size_t mgp_size[module_count];
    const std::uint8_t* mvp[module_count]; std::size_t mvp_size[module_count];
};

struct Preview {
    dh2::world::SourceLevel level{};
    dh2::viewer::SceneMesh modules[module_count]{};
    bool ready = false;
};

enum class Error : std::uint32_t {
    ok, argument, level_import, source_paths, module_import,
    bres_open, scene_open, module_bind, subtree, geometry, allocation
};

struct Diagnostic { Error error; char message[192]; };

// `output` must be a zero-initialized or previously loaded Preview. A load
// replaces it: any prior buffers in `*output` are freed before new inputs are
// validated or imported. `Preview{}` is the supported initial state.
Error load(Preview*, const SourceFiles*, Diagnostic*);
void free(Preview*);
}
