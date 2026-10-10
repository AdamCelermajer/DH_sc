#include "../original_character.hpp"
#include "../asset_catalog.hpp"
#include <iostream>
#include <cmath>
#include <stdexcept>
using namespace dh::foundation;
void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
float floor_z(const CharacterVisual& visual){float minimum=1e30f;for(const auto& mesh:visual.meshes())for(const auto& vertex:mesh.vertices)minimum=std::min(minimum,vertex.position.z);return minimum;}
int main(int argc,char**argv){try {
    check(argc==2,"original staged asset root required");AssetCatalog assets(argv[1]);CharacterVisualConfig config;
    config.model_path="models/prince_modular.bdae";config.template_clip_path="animations/prince_template_anim.bdae";config.clips={{"stand","animations/prince_idle_shield.bdae"},{"move","animations/prince_walk_1hand.bdae"}};config.skin_id_contains="_default_warrior-mesh-skin";config.expected_controller_count=4;
    // Data supplied from original GetAnimRoot(false), not a loader actor preset.
    config.motion_node_id="auto";
    CharacterVisual raw,consumed;std::string error;check(raw.load(assets,config,error),error);config.consume_root_motion=true;check(consumed.load(assets,config,error),error);
    check(std::string(consumed.root_motion_node_id()).find("root_camera")!=std::string::npos,"source root priority chose wrong pivot");
    check(std::string(consumed.source_motion_root_name(false))=="root_camera" && std::string(consumed.source_motion_root_name(true))=="Bip01","GetAnimRoot false/true source distinction differs");
    const auto origin=consumed.root_motion_rest_origin();check(std::abs(origin.z)<.001,"source locomotion root includes pelvis height");
    for(double delta:{0.,.1,.15,.2}) {
        check(raw.update(delta,error),error);check(consumed.update(delta,error),error);
        check(raw.meshes().size()==consumed.meshes().size(),"mesh count differs");
        for(std::size_t m=0;m<raw.meshes().size();++m)for(std::size_t v=0;v<raw.meshes()[m].vertices.size();++v){const auto a=raw.meshes()[m].vertices[v].position,b=consumed.meshes()[m].vertices[v].position;check(std::abs(a.x-b.x)+std::abs(a.y-b.y)+std::abs(a.z-b.z)<.001,"root consumption changes grounded idle pose");}
    }
    Vec3 minimum{},maximum{};check(consumed.indexed_bounds(minimum,maximum,error),error);
    // Extraneous, unreferenced CPU vertices must not alter a visible extent.
    consumed.mutable_meshes()[0].vertices.push_back({{100000,100000,100000}});Vec3 checkedMin{},checkedMax{};check(consumed.indexed_bounds(checkedMin,checkedMax,error),error);check(checkedMax.z==maximum.z,"unused vertex inflated visible bounds");consumed.mutable_meshes()[0].vertices.pop_back();
    const auto standing=floor_z(consumed);check(consumed.select("move",true,error),error);check(consumed.update(.8,error),error);const auto full=consumed.take_root_motion();check(std::abs(full.y+603.274f)<.01,"authored source stride differs");check(consumed.update(.4,error),error);const auto first=consumed.take_root_motion();check(consumed.update(.4,error),error);const auto second=consumed.take_root_motion();check(std::abs(first.y+second.y-full.y)<.001,"split source stride differs");
    check(consumed.select("stand",true,error),error);check(consumed.update(0,error),error);check(std::abs(floor_z(consumed)-standing)<1,"idle return moved model pivot");check(std::abs(consumed.take_root_motion().y)<.001,"clip switch creates movement");
    check(consumed.restart("move",false,error),error);check(consumed.update(.8,error),error);const auto replayFirst=consumed.take_root_motion();
    check(consumed.restart("move",false,error),error);check(consumed.animation_elapsed_seconds()==0,"same action did not restart source clock");check(consumed.update(.8,error),error);const auto replaySecond=consumed.take_root_motion();
    check(std::abs(replayFirst.y-replaySecond.y)<.001&&std::abs(replaySecond.y-full.y)<.001,"same one-shot action did not replay authored displacement");
    const auto oldClock=consumed.animation_elapsed_seconds();check(!consumed.restart("missing",false,error)&&consumed.animation_elapsed_seconds()==oldClock,"failed action restart changed the active clock");
    std::cout<<"PASS source root="<<consumed.root_motion_node_id()<<" restOrigin="<<origin.x<<","<<origin.y<<","<<origin.z<<" idleFloor="<<standing<<" indexedBounds="<<minimum.x<<","<<minimum.y<<","<<minimum.z<<".."<<maximum.x<<","<<maximum.y<<","<<maximum.z<<" stride="<<full.x<<","<<full.y<<","<<full.z<<"\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
