#pragma once
#include "scene.hpp"
#include <memory>
namespace dh2::world {
struct RetainedMeshDataV91 {
 using Point=std::array<float,3>;
 std::vector<Point> source_positions,positions;
 std::vector<std::array<std::uint32_t,3>> triangles;
 std::array<float,6> bounds{};
 static bool create(const resources::BresView&,std::uint32_t geometry,std::shared_ptr<RetainedMeshDataV91>&,std::string&);
};
struct RetainedMeshNodeV91 {
 std::shared_ptr<scene::InstanceStorageV91> fields;
 std::shared_ptr<RetainedMeshDataV91> mesh;
 void source_set_visible(bool visible)noexcept{auto& state=fields->mesh_fields;state.visibility.local120=visible?1:0;
  if(visible&&state.visibility.parent121&&!fields->detached)state.flags|=1u;else state.flags&=~1u;}
 void source_hide()noexcept{source_set_visible(false);}
};
class RetainedTriangleSelectorV91 {
 std::shared_ptr<RetainedMeshDataV91> mesh_;
public:
 explicit RetainedTriangleSelectorV91(std::shared_ptr<RetainedMeshDataV91> m):mesh_(std::move(m)){}
 const std::shared_ptr<RetainedMeshDataV91>& mesh()const noexcept{return mesh_;}
 bool line(const float* start,const float* end,bool& hit,std::array<float,3>& point,std::string&)const;
};
struct RetainedVisualNodeV91 {
 std::uint32_t index{};
 std::shared_ptr<scene::NodeStorageV91> fields;
 std::weak_ptr<RetainedVisualNodeV91> parent;
 std::vector<std::shared_ptr<RetainedVisualNodeV91>> children;
 std::vector<std::shared_ptr<RetainedMeshNodeV91>> meshes;
 std::shared_ptr<RetainedTriangleSelectorV91> selector;
 void collect_meshes(std::vector<std::shared_ptr<RetainedMeshNodeV91>>&)const;
};
}
