#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace dh2::scene { struct Scene; }
namespace dh2::visual { class SceneBinding; }

namespace dh2::irrlicht_game {

struct PrinceVertex {
    std::array<float, 3> rest_position{};
    std::array<float, 3> position{};
    std::array<float, 2> uv{};
    std::array<float, 4> color{1.0f, 1.0f, 1.0f, 1.0f};
};

struct PrinceMeshPart {
    std::uint32_t skin_index = 0;
    std::uint32_t geometry_index = 0;
    std::uint32_t primitive_index = 0;
    std::int32_t position_attribute = -1;
    std::int32_t uv_attribute = -1;
    std::int32_t color_attribute = -1;
    std::uint32_t material_index = 0;
    std::string material_id;
    std::string diffuse_texture;
    std::string alpha_texture;
    std::array<float, 4> material_color{1.0f, 1.0f, 1.0f, 1.0f};
    std::array<float, 16> texture_matrix{
        1, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 1};
    std::vector<PrinceVertex> vertices;
    std::vector<std::uint16_t> indices;
};

// Renderer-neutral Prince rig and mutable vertex source. It assembles the four
// selected default-warrior controllers, preserves immutable bind-pose streams,
// and deforms against the live source Scene after the caller advances authored
// Character playback. SceneBinding owns owner * helper * authored graph and
// root-displacement composition. The Irrlicht node must remain at identity.
class PrinceActor {
    struct Impl;
    std::unique_ptr<Impl> impl_;

public:
    PrinceActor();
    ~PrinceActor();
    PrinceActor(PrinceActor&&) noexcept;
    PrinceActor& operator=(PrinceActor&&) noexcept;
    PrinceActor(const PrinceActor&) = delete;
    PrinceActor& operator=(const PrinceActor&) = delete;

    bool load(const std::uint8_t* model, std::size_t model_size,
              std::string& error);
    dh2::scene::Scene& scene();
    const dh2::scene::Scene& scene() const;
    dh2::visual::SceneBinding& visual_binding();
    bool deform(std::string& error);

    const std::vector<PrinceMeshPart>& parts() const;
    std::uint32_t controller_count() const;
    std::uint32_t joint_count() const;
    std::uint32_t vertex_count() const;
    std::uint32_t triangle_count() const;
    bool ready() const;
};

} // namespace dh2::irrlicht_game
