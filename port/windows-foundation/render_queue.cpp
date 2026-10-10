#include "render_queue.hpp"

#include "frame_perf.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace dh::foundation {
namespace {

float rangeDepth(const Renderer& renderer, const Mesh& mesh, const DrawRange& range, const Mat4& transform,
                 const Camera& camera) {
    // B066: bounds come from the renderer's range cache (O(1) for static meshes) instead of rescanning every frame.
    const RangeStats stats = renderer.rangeStats(mesh, range);
    if (!stats.valid) return -std::numeric_limits<float>::max();
    const Vec3 center = stats.center;
    const Vec3 world{transform[0]*center.x+transform[4]*center.y+transform[8]*center.z+transform[12],
                     transform[1]*center.x+transform[5]*center.y+transform[9]*center.z+transform[13],
                     transform[2]*center.x+transform[6]*center.y+transform[10]*center.z+transform[14]};
    Vec3 forward{camera.target.x-camera.eye.x,camera.target.y-camera.eye.y,camera.target.z-camera.eye.z};
    const float squared = forward.x*forward.x+forward.y*forward.y+forward.z*forward.z;
    if (!std::isfinite(squared) || squared < 1.0e-12f) forward={0,0,-1};
    else {
        const float inverse=1.0f/std::sqrt(squared);
        forward.x*=inverse; forward.y*=inverse; forward.z*=inverse;
    }
    return (world.x-camera.eye.x)*forward.x+(world.y-camera.eye.y)*forward.y+(world.z-camera.eye.z)*forward.z;
}

} // namespace

void RenderQueue::submit(const Mesh& mesh, const Mat4& transform) {
    if (mesh.vertices.empty()) return;
    if (mesh.ranges.empty()) {
        entries_.push_back({&mesh,{0,mesh.indices.empty() ? mesh.vertices.size() : mesh.indices.size(),{}},transform});
    } else {
        for (const auto& range : mesh.ranges) entries_.push_back({&mesh,range,transform});
    }
}

bool RenderQueue::submit(const Mesh& mesh, const Mat4& transform,
                         const std::vector<std::uint32_t>& rangeTextures,
                         std::string& error) {
    error.clear();
    const auto count=mesh.ranges.empty()?std::size_t(1):mesh.ranges.size();
    if(rangeTextures.size()!=count){error="Texture overrides must match mesh draw ranges";return false;}
    if(mesh.vertices.empty())return true;
    entries_.reserve(entries_.size()+count);
    for(std::size_t index=0;index<count;++index){
        DrawRange range=mesh.ranges.empty()
            ?DrawRange{0,mesh.indices.empty()?mesh.vertices.size():mesh.indices.size(),{}}
            :mesh.ranges[index];
        range.material.texture=rangeTextures[index];
        entries_.push_back({&mesh,range,transform});
    }
    return true;
}

void RenderQueue::flush(Renderer& renderer, const Camera& camera) {
    std::vector<std::pair<float,const Entry*>> transparent;
    transparent.reserve(entries_.size());
    std::uint64_t culled = 0;
    for (const auto& entry : entries_) {
        // B066: static level ranges completely outside the view frustum are skipped (no visible change).
        if (!renderer.rangeVisible(*entry.mesh,entry.range,entry.transform)) {++culled;continue;}
        if (renderer.isTransparent(*entry.mesh,entry.range))
            transparent.emplace_back(rangeDepth(renderer,*entry.mesh,entry.range,entry.transform,camera),&entry);
        else renderer.drawRange(*entry.mesh,entry.range,entry.transform);
    }
    perf::FramePerf::get().counters().culledRanges += culled;
    // Stable ties retain authored submission order. Centers approximate ordering;
    // intersecting transparent polygons cannot be resolved by object sorting.
    std::stable_sort(transparent.begin(),transparent.end(),[](const auto& a,const auto& b) { return a.first > b.first; });
    for (const auto& entry : transparent)
        renderer.drawRange(*entry.second->mesh,entry.second->range,entry.second->transform);
    clear();
}

void RenderQueue::clear() { entries_.clear(); }

} // namespace dh::foundation
