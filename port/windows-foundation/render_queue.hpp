#pragma once

#include "renderer.hpp"

namespace dh::foundation {

// Meshes must remain alive and unchanged between submit() and flush().
// The queue owns transforms and range descriptors, never vertex/index buffers.
class RenderQueue {
public:
    void submit(const Mesh& mesh, const Mat4& transform = identity());
    // One texture per authored range (one for a mesh without ranges). Only
    // queued range copies change; source buffers and material pass stay intact.
    // Texture handles must remain alive through flush. Mismatch adds nothing.
    bool submit(const Mesh&, const Mat4&, const std::vector<std::uint32_t>& rangeTextures,
                std::string& error);
    void flush(Renderer& renderer, const Camera& camera);
    void clear();
    std::size_t size() const { return entries_.size(); }

private:
    struct Entry {
        const Mesh* mesh;
        DrawRange range;
        Mat4 transform;
    };
    std::vector<Entry> entries_;
};

} // namespace dh::foundation
