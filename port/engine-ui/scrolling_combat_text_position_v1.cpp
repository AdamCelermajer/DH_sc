#include "scrolling_combat_text_position_v1.hpp"
#include "../scene-materials/scene.hpp"

#include <cmath>
#include <limits>
#include <vector>

namespace dh2::ui {

bool resolve_scrolling_combat_target_node_v1(const scene::Scene& scene,
    bool& present,std::uint32_t& node_index,float out[3],
    std::string& error) {
    error.clear();
    if(!out){error="Source target_node position output is missing";return false;}
    const auto count=scene.graph.size();
    if(count>static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max())){
        error="Source scene graph exceeds target_node lookup capacity";return false;
    }
    std::vector<std::vector<std::uint32_t>> children(count+1);
    for(std::uint32_t i=0;i<count;++i){
        const auto parent=scene.graph[i].parent;
        if(parent < -1 || parent>=static_cast<std::int32_t>(i)){
            error="Source scene graph has an invalid target_node parent";return false;
        }
        children[parent<0?count:static_cast<std::size_t>(parent)].push_back(i);
    }
    std::vector<std::uint32_t> pending;
    const auto& roots=children[count];
    for(auto it=roots.rbegin();it!=roots.rend();++it)pending.push_back(*it);
    while(!pending.empty()){
        const auto current=pending.back();pending.pop_back();
        const auto& node=scene.graph[current];
        if(node.name=="target_node"){
            const float candidate[3]={node.world[12],node.world[13],node.world[14]};
            for(float value:candidate)if(!std::isfinite(value)){
                error="Source target_node absolute position is non-finite";return false;
            }
            out[0]=candidate[0];out[1]=candidate[1];out[2]=candidate[2];
            present=true;node_index=current;return true;
        }
        const auto& descendants=children[current];
        for(auto it=descendants.rbegin();it!=descendants.rend();++it)
            pending.push_back(*it);
    }
    present=false;node_index=UINT32_MAX;out[0]=out[1]=out[2]=0.f;
    return true;
}

bool resolve_scrolling_combat_text_position_v1(
    std::uintptr_t requested_identity,
    const ScrollingCombatTextPositionFactsV1& facts,
    float out[3],std::string& error) noexcept {
  try {
    error.clear();
    if(!requested_identity||facts.identity!=requested_identity||!out||
       !facts.game_object_position||!facts.relative_box||
       facts.source_visible_80>1) {
        error="Source SCT position facts are missing or identity-mismatched";
        return false;
    }
    float target_node_world[3]{};bool target_node_present=false;
    std::uint32_t target_node_index=UINT32_MAX;
    if(facts.visual_scene&&
       !resolve_scrolling_combat_target_node_v1(*facts.visual_scene,
          target_node_present,target_node_index,target_node_world,error))return false;
    const float* selected=(target_node_present&&facts.source_visible_80)
        ?target_node_world:facts.game_object_position;
    const float height=facts.relative_box[5]-facts.relative_box[2];
    float result[3]={selected[0],selected[1],selected[2]+height};
    for(float value:result)if(!std::isfinite(value)){
        error="Source SCT world position is non-finite";return false;
    }
    for(unsigned i=0;i<6;++i)if(!std::isfinite(facts.relative_box[i])){
        error="Source SCT character bounds are non-finite";return false;
    }
    out[0]=result[0];out[1]=result[1];out[2]=result[2];
    return true;
  } catch(...) {
    error="Source SCT position resolution failed";return false;
  }
}

bool scrolling_combat_text_position_v1(void* raw,std::uintptr_t identity,
    float out[3],std::string& error) noexcept {
  try {
    error.clear();
    auto* lookup=static_cast<ScrollingCombatTextPositionLookupV1*>(raw);
    if(!lookup||!lookup->lookup){
        error="Source SCT live position lookup is unavailable";return false;
    }
    ScrollingCombatTextPositionFactsV1 facts{};
    if(!lookup->lookup(lookup->context,identity,facts,error)){
        if(error.empty())error="Source SCT live position lookup failed";
        return false;
    }
    return resolve_scrolling_combat_text_position_v1(identity,facts,out,error);
  } catch(...) {
    error="Source SCT live position provider threw";return false;
  }
}

} // namespace dh2::ui
