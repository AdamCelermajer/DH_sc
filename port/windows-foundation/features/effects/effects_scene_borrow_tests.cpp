#include "effects_anchor_binding.hpp"
#include "../../original_character.hpp"
#include "../../asset_catalog.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
using namespace dh::foundation::effects;
static unsigned checks;
static void check(bool value,const std::string& message) { ++checks;if(!value)throw std::runtime_error(message); }
int main(int argc,char** argv) { try {
    check(argc==2,"original staged asset root required");AssetCatalog assets(argv[1]);
    auto visual=std::make_shared<CharacterVisual>();CharacterVisualConfig config;
    config.model_path="models/prince_modular.bdae";config.template_clip_path="animations/prince_template_anim.bdae";
    config.clips={{"idle","animations/prince_idle_shield.bdae"},{"walk","animations/prince_walk_1hand.bdae"}};
    config.skin_id_contains="_default_warrior-mesh-skin";config.expected_controller_count=4;
    config.motion_node_id="auto";config.consume_root_motion=true;std::string error;
    check(visual->retained_scene_borrow()==nullptr,"unloaded visual cannot supply a source Scene");
    check(visual->load(assets,config,error),error);const auto* same=visual->retained_scene_borrow();
    check(same&&same->graph.size()==35,"actual retained Prince source graph");
    auto hand=std::find_if(same->graph.begin(),same->graph.end(),[](const auto& node) { return node.name=="Bip01_R_Hand"; });
    check(hand!=same->graph.end(),"original authored right-hand socket");
    const auto hand_index=std::size_t(hand-same->graph.begin());
    const auto source_name=hand->id;bool dead=false;unsigned detach_calls=0;
    EffectsAnchorBinding binding([&](std::uintptr_t actor) { check(actor==71,"same original actor detachment token");++detach_calls; });
    EffectAnchorBorrow borrow{visual,[&,visual](EffectAnchorSample& result,std::string& e) {
        check(visual->retained_scene_borrow()==same,"same Scene identity across actual animation updates");
        Mat4 world;if(!visual->bone_world(source_name,world,e))return false;
        result.dead=dead;result.disabled=false;result.stationary=false;
        result.source_position={world[12],world[13],world[14]};
        std::copy_n(same->graph[hand_index].scale,3,result.source_scale.begin());
        return true;
    }};
    check(binding.bind(71,std::move(borrow),error),error);
    check(visual->restart("walk",true,error)&&visual->update(0,error),error);
    dh2::fx::MeshFxRequestV1 request{dh2::fx::MeshFxOperationV1::anchor_position,71,nullptr,0,{},same};
    bool handled=false;check(binding.invoke(request,handled,error)&&handled,error);
    const std::array<float,3> initial{request.point[0],request.point[1],request.point[2]};
    check(visual->update(.4,error),error);const auto source_clock=visual->animation_elapsed_seconds();
    check(binding.invoke(request,handled,error)&&handled,error);
    check(visual->animation_elapsed_seconds()==source_clock,"anchor reads do not tick retained actor clock");
    check(visual->retained_scene_borrow()==same,"pose sampling keeps the actual Scene borrower");
    // Source graph itself, not rendered-vertex bounding boxes, supplies position.
    Mat4 reference;check(visual->bone_world(source_name,reference,error),error);
    check(request.point[0]==reference[12]&&request.point[1]==reference[13]&&request.point[2]==reference[14],"actual posed source attachment");
    check(request.point[0]!=initial[0]||request.point[1]!=initial[1]||request.point[2]!=initial[2],"FX attachment follows actual posed hand");
    dead=true;request.operation=dh2::fx::MeshFxOperationV1::anchor_dead;
    check(binding.invoke(request,handled,error)&&request.result==1,"actual lifetime status is re-read");
    binding.release(71);check(detach_calls==1,"release delegates exactly once to source FX lifecycle");
    check(!binding.invoke(request,handled,error),"released source borrower cannot sample stale pose");
    auto invalid=config;invalid.model_path="missing-source-actor.bdae";
    check(!visual->load(assets,invalid,error)&&visual->retained_scene_borrow()==same,"failed reload preserves existing graph");
    CharacterVisual fairy;CharacterVisualConfig fairy_config;
    fairy_config.model_path="original-cache/data/3d/characters/faeries/faeries_02_celeste.bdae";
    fairy_config.template_clip_path="original-cache/data/3d/characters/faeries/animations/faeries_template_anim.bdae";
    fairy_config.clips={{"idle","original-cache/data/3d/characters/faeries/animations/faeries_celeste_idle.bdae"}};
    fairy_config.include_static_instances=true;fairy_config.allow_missing_animation_targets=true;
    check(fairy.load(assets,fairy_config,error),error);const auto* fairy_scene=fairy.retained_scene_borrow();
    check(fairy_scene&&fairy_scene->instances.size()==2,"actual faery body and rigid glow source instances");
    check(fairy.update(.2,error),error);unsigned glow=0;
    for(const auto& mesh:fairy.meshes())if(mesh.vertices.size()==8&&mesh.indices.size()==12)++glow;
    check(glow==1,"original faery eight-vertex/twelve-index glow renders through same CharacterVisual");
    unsigned emitters=0;for(const auto& node:fairy_scene->graph)if(node.id.find("GL_PCloud")!=std::string::npos)++emitters;
    check(emitters==3,"original embedded faery emitter nodes remain on same retained graph");
    check(fairy.retained_scene_borrow()==fairy_scene,"faery source effects share retained pose scene identity");
    std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_scene_nodes\":35,\"same_pose_clock\":true,\"lifetime_detach_fixture\":true}\n";
} catch(const std::exception& e) { std::cerr<<e.what()<<'\n';return 1; } }
