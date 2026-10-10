#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <optional>
#include <string>
#include <functional>

namespace dh::foundation {

struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

// Column-major matrices, matching OpenGL's convention.
using Mat4 = std::array<float, 16>;
Mat4 identity();
Mat4 translation(Vec3 position);

struct Vertex {
    Vec3 position;
    Vec3 normal{0.0f, 1.0f, 0.0f};
    float u = 0.0f;
    float v = 0.0f;
    std::array<float, 4> color{1.0f, 1.0f, 1.0f, 1.0f};
};

// Values are the original pass's OpenGL enums, decoded from its source state.
// Presence makes this authoritative over texture/vertex alpha heuristics.
struct SourceMaterialPass {
    std::uint32_t blendSource = 1, blendDestination = 0;
    std::uint32_t depthFunction = 0x203, cullFace = 0x405, frontFace = 0x901;
    bool blend = false, depthTest = true, depthWrite = true, cull = true;
    bool alphaTest = false;
};

struct Material {
    std::array<float, 4> color{1.0f, 1.0f, 1.0f, 1.0f};
    std::uint32_t texture = 0;
    bool transparent = false;
    bool doubleSided = false;
    // Normalized threshold: authored fragment shaders discard alpha <= alpha_ref.
    float alphaReference = 0.0f;
    bool additive = false;
    // Authored vertex lighting can be displayed without applying a second light.
    bool lightingEnabled = true;
    std::optional<SourceMaterialPass> sourcePass;
};

struct DrawRange {
    // Index offsets for indexed meshes, vertex offsets for unindexed meshes.
    std::size_t firstIndex = 0;
    std::size_t indexCount = 0;
    Material material;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
    // Empty ranges means draw all geometry with a default material.
    std::vector<DrawRange> ranges;
};

struct Camera {
    Vec3 eye{0.0f, 3.0f, 8.0f};
    Vec3 target{0.0f, 0.0f, 0.0f};
    Vec3 up{0.0f, 1.0f, 0.0f};
    float verticalFovDegrees = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 5000.0f;
    // Zero uses the viewport; a positive value preserves authored projection.
    float aspectRatio = 0.0f;
};

enum class RenderPass { All, Opaque, Transparent };
// Port-owned asset selection policy for original GameObject facultative
// visuals. This is not an Android driver enum or a hardware performance score.
enum class VisualAssetQuality { Full, ReducedOptional };

// The host owns its current WGL context, window and SwapBuffers call.
// Call every method on the context's owning thread while that context is current.
class Renderer {
public:
    bool initialize(int width, int height);
    void setVisualAssetQuality(VisualAssetQuality quality)noexcept{visualQuality_=quality;}
    bool game_object_visual_quality(bool& full_quality,std::string& error)const;
    void resize(int width, int height);
    void beginFrame(const Camera& camera);
    // Pixel rectangle uses OpenGL's bottom-left origin. Synchronous drawing
    // preserves surrounding GL state and color; only the clipped depth is cleared.
    bool withViewport(int x, int y, int width, int height, const Camera& camera,
                      const std::function<void()>& drawContent);
    void draw(const Mesh& mesh, const Mat4& transform = identity(),
              RenderPass pass = RenderPass::All);
    // Single-range submission permits global ordering without copying geometry.
    void drawRange(const Mesh& mesh, const DrawRange& range,
                   const Mat4& transform = identity());
    bool isTransparent(const Mesh& mesh, const DrawRange& range) const;
    void endFrame();

    // RGBA8 rows are supplied in OpenGL texture coordinate order.
    std::uint32_t createTexture(int width, int height, const std::uint8_t* rgba);
    void destroyTexture(std::uint32_t texture);
    bool textureHasAlpha(std::uint32_t texture) const;

private:
    void applyCamera(const Camera& camera);
    VisualAssetQuality visualQuality_=VisualAssetQuality::Full;
    std::uintptr_t qualityContext_{},qualityDeviceContext_{};
    std::uint32_t qualityThread_{};
    void drawInternal(const Mesh& mesh, const Mat4& transform,
                      RenderPass pass, const DrawRange* singleRange);
    int width_ = 1;
    int height_ = 1;
    Camera camera_;
    struct TextureAlpha { bool hasAlpha = false; bool hasPartialAlpha = false; };
    std::unordered_map<std::uint32_t, TextureAlpha> textureAlpha_;
    std::vector<std::array<float, 4>> vertexColors_;
};

} // namespace dh::foundation
