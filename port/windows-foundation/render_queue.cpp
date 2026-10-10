#include "render_queue.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace dh::foundation {
namespace {

float rangeDepth(const Mesh& mesh, const DrawRange& range, const Mat4& transform,
                 const Camera& camera) {
    const bool indexed = !mesh.indices.empty();
    const std::size_t size = indexed ? mesh.indices.size() : mesh.vertices.size();
    const std::size_t first = std::min(range.firstIndex, size);
    const std::size_t count = std::min(range.indexCount, size-first);
    Vec3 low{std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()};
    Vec3 high{-low.x,-low.y,-low.z};
    bool found = false;
    for (std::size_t element=first; element < first+count; ++element) {
        const std::size_t vertex = indexed ? mesh.indices[element] : element;
        if (vertex >= mesh.vertices.size()) continue;
        const auto& p = mesh.vertices[vertex].position;
        low.x=std::min(low.x,p.x); low.y=std::min(low.y,p.y); low.z=std::min(low.z,p.z);
        high.x=std::max(high.x,p.x); high.y=std::max(high.y,p.y); high.z=std::max(high.z,p.z);
        found = true;
    }
    if (!found) return -std::numeric_limits<float>::max();
    const Vec3 center{(low.x+high.x)*0.5f,(low.y+high.y)*0.5f,(low.z+high.z)*0.5f};
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
    for (const auto& entry : entries_) {
        if (renderer.isTransparent(*entry.mesh,entry.range))
            transparent.emplace_back(rangeDepth(*entry.mesh,entry.range,entry.transform,camera),&entry);
        else renderer.drawRange(*entry.mesh,entry.range,entry.transform);
    }
    // Stable ties retain authored submission order. Centers approximate ordering;
    // intersecting transparent polygons cannot be resolved by object sorting.
    std::stable_sort(transparent.begin(),transparent.end(),[](const auto& a,const auto& b) { return a.first > b.first; });
    for (const auto& entry : transparent)
        renderer.drawRange(*entry.second->mesh,entry.second->range,entry.second->transform);
    clear();
}

void RenderQueue::clear() { entries_.clear(); }

} // namespace dh::foundation
