#include "module_draw_frame_v1.hpp"
#include "retained_module_visual_v3.hpp"
#include <cmath>

namespace dh2::loader {
bool capture_module_draw_frame_v1(const std::shared_ptr<world::RetainedModuleVisualV3>& visual,
    std::int32_t module_id,ModuleDrawFrameV1& out,std::string& error) {
    error.clear();
    if(!visual||!visual->ready()||!visual->root_identity()||!visual->resource_lease()) {
        error="Required live retained Module visual/root/resource for render submission";return false;
    }
    // No callbacks occur while reading the live graph. Renderers receive only
    // owned frame values and the immutable resource pin, never scene pointers.
    ModuleDrawFrameV1 next;next.module_id=module_id;
    next.visual_identity=visual->identity();next.root_identity=visual->root_identity();
    next.resource=visual->resource_lease();next.bres=visual->bres();
    const auto& root=visual->scene();const auto& selected=root.selected();
    if(selected.scene.instances.size()!=selected.instance_visibility.size()) {
        error="Actual Module render visibility/instance cardinality mismatch";return false;
    }
    if(root.root_flags()&1u)for(std::uint32_t i=0;i<selected.scene.instances.size();++i) {
        if(!root.mesh_attached(i)||!selected.instance_visibility[i])continue;
        const auto& mesh=selected.scene.instances[i];
        if(mesh.controller>=0) {
            error="Required actual skinned Module renderer";return false;
        }
        assets::Mesh payload{};
        if(dh2_mesh_open(&payload,&next.bres,mesh.geometry)!=assets::Error::ok||
           payload.primitives!=mesh.materials.size()) {
            error="Actual Module render mesh/material binding mismatch";return false;
        }
        ModuleDrawMeshV1 draw;draw.source_instance=i;draw.geometry=mesh.geometry;
        std::shared_ptr<world::RetainedMeshNodeV91> native_mesh;
        if(!root.borrow_mesh_source_v111(i,native_mesh,error))return false;
        draw.source_mesh_v111=native_mesh;
        draw.cached_world=mesh.world;
        for(float v:draw.cached_world)if(!std::isfinite(v)) {
            error="Actual Module render cache has nonfinite matrix";return false;
        }
        for(auto material:mesh.materials) {
            if(material>=selected.scene.materials.size()) {
                error="Actual Module render material index outside retained scene";return false;
            }
            draw.materials.push_back(selected.scene.materials[material]);
        }
        next.meshes.push_back(std::move(draw));
    }
    out=std::move(next);return true;
}
}
