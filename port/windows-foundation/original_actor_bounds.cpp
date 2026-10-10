#include "original_actor_bounds.hpp"
#include "../level-world/character_world_npc_bounds_v1.hpp"
#include <algorithm>
#include <cmath>
#include <set>
#include <stdexcept>
namespace dh::foundation {
struct OriginalActorBounds::Impl { std::vector<std::uint8_t> bytes; dh2::scene::Scene factory; };
OriginalActorBounds::OriginalActorBounds()=default;
OriginalActorBounds::~OriginalActorBounds()=default;
OriginalActorBounds::OriginalActorBounds(OriginalActorBounds&&) noexcept=default;
OriginalActorBounds& OriginalActorBounds::operator=(OriginalActorBounds&&) noexcept=default;
bool OriginalActorBounds::load(const AssetCatalog& assets,const ActorBoundsConfig& config,std::string& error) {
    try {
        auto next=std::make_unique<Impl>();next->bytes=assets.read(config.model_path);dh2::resources::BresView view{};
        if(dh2_bres_open(&view,next->bytes.data(),next->bytes.size())!=dh2::resources::BresError::ok)throw std::runtime_error("Bounds model BRES rejected");
        if(!dh2::scene::load(view,next->factory,error))return false;
        if(!config.components.empty()) {
            auto& instances=next->factory.instances;
            instances.erase(std::remove_if(instances.begin(),instances.end(),[](const auto& entry){return entry.controller>=0;}),instances.end());
            std::set<std::pair<std::string,std::string>> selected;
            for(const auto& component:config.components) {
                if(component.controller_id.empty()||component.group_node_id.empty()||!selected.emplace(component.controller_id,component.group_node_id).second)throw std::runtime_error("Duplicate/empty source bounds component binding");
                auto node=std::find_if(next->factory.graph.begin(),next->factory.graph.end(),[&](const auto& n){return n.id==component.group_node_id;});
                if(node==next->factory.graph.end())throw std::runtime_error("Authored component group missing: "+component.group_node_id);
                bool found=false;
                for(unsigned c=0;c<dh2_bres_library_count(&view,dh2::resources::Library::controller);++c) {
                    dh2::skinning::Skin skin;if(!dh2::skinning::load(view,c,next->factory,skin,error))return false;
                    if(skin.id!=component.controller_id)continue;
                    if(found)throw std::runtime_error("Ambiguous authored bounds controller ID");
                    dh2::scene::Instance instance;instance.node=node->id;instance.node_index=static_cast<unsigned>(node-next->factory.graph.begin());instance.geometry=skin.geometry;instance.world=node->world;instance.controller=c;instances.push_back(std::move(instance));found=true;
                }
                if(!found)throw std::runtime_error("Selected authored controller missing: "+component.controller_id);
            }
        }
        if(next->factory.instances.empty())throw std::runtime_error("Source bounds has no actual providers");
        impl_=std::move(next);error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
bool OriginalActorBounds::calculate(const ActorBoundsPlacement& placement,const SkeletalPose* pose,OriginalActorBoundsResult& output,std::string& error) const {
    if(!impl_) {error="Original actor bounds model not loaded";return false;}
    auto scene=impl_->factory;if(pose&&!apply_scene_pose(*pose,scene,error))return false;
    dh2::resources::BresView view{};if(dh2_bres_open(&view,impl_->bytes.data(),impl_->bytes.size())!=dh2::resources::BresError::ok){error="Retained bounds BRES rejected";return false;}
    float scale[3],position[]{placement.position.x,placement.position.y,placement.position.z},rotation[]{placement.rotation_degrees.x,placement.rotation_degrees.y,placement.rotation_degrees.z};
    for(float coordinate:position)if(!std::isfinite(coordinate)){error="Nonfinite owner position";return false;}
    for(float coordinate:rotation)if(!std::isfinite(coordinate)){error="Nonfinite owner rotation";return false;}
    dh2::physical::ObjectVisualTransformV1 transform{};
    if(dh2_character_visual_scale(scale,placement.base_scale.data())||dh2_object_visual_transform_v1(&transform,position,rotation,scale)){error="Original owner visual TRS rejected";return false;}
    dh2::physical::CharacterOwnerBounds bounds{};
    if(!dh2::character::character_npc_visual_bounds_v1(view,scene,transform.root_matrix,position,placement.collision_scale,placement.previous_flat,bounds,error))return false;
    dh2::physical::DecorSceneMarker marker{};if(!dh2::physical::decor_scene_marker(view,scene,marker,error))return false;
    // Recover the same mesh producer independently, retaining actual unscaled
    // output rather than dividing collision-scaled/padded owner extents.
    dh2::physical::DecorSceneOutput visual{};
    dh2::physical::DecorSceneInput input{};std::copy_n(position,3,input.position);std::copy_n(rotation,3,input.rotation_degrees);std::copy_n(scale,3,input.scale);
    if(marker.found) {std::copy_n(marker.bounds,6,input.marker_bounds);std::copy_n(marker.parent_scale,3,input.marker_parent_scale);if(dh2_decor_scene(&visual,&input)){error="Original marker mesh bounds rejected";return false;}}
    else {std::vector<dh2::physical::CharacterMeshEntry> entries;if(!dh2::physical::character_scene_entries(view,scene,entries,error))return false;dh2::physical::CharacterMeshBoxInput request{entries.data(),static_cast<std::uint32_t>(entries.size()),0,input};if(dh2_character_mesh_box(&visual,&request)){error="Original actor mesh bounds rejected";return false;}}
    OriginalActorBoundsResult result;std::copy_n(visual.mesh_box,6,result.mesh_box.begin());std::copy_n(bounds.relative_box,6,result.relative_box.begin());std::copy_n(bounds.absolute_box,6,result.absolute_box.begin());result.marker=marker.found;result.flat=bounds.flat;result.pf_updates=bounds.update_pf_count;output=result;error.clear();return true;
}
bool OriginalActorBounds::update_absolute(OriginalActorBoundsResult& bounds,Vec3 position,std::string& error) {
    const float p[]{position.x,position.y,position.z};auto candidate=bounds;
    for(unsigned k=0;k<6;++k){candidate.absolute_box[k]=bounds.relative_box[k]+p[k%3];if(!std::isfinite(candidate.absolute_box[k])){error="Nonfinite source absolute bounds";return false;}}
    bounds=candidate;error.clear();return true;
}
}
