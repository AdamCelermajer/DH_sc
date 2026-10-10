#include "../original_actor_target_position.hpp"
#include "../original_character.hpp"
#include "../asset_catalog.hpp"
#include "../../scene-materials/scene.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool yes,const char* message){if(!yes)throw std::runtime_error(message);}
int main(int argc,char** argv){try{
    std::optional<std::uintptr_t> node;
    std::optional<OriginalTargetPosition> cache;
    OriginalTargetPosition position{7,8,9};const OriginalTargetPosition* selected=&position;
    OriginalTargetPositionUpdateResult result;std::string error;int calls=0;
    check(!original_update_target_position(node,cache,{},result,error)&&!cache&&!result.node_queried,
          "unknown node fabricated no-target success");
    check(!original_get_target_position(node,cache,position,{},selected,error)&&selected==&position,
          "unknown selector changed output");
    node=0;cache=OriginalTargetPosition{11,12,13};
    check(original_update_target_position(node,cache,{},result,error)&&(*cache)[0]==11&&!result.cache_written,
          "known null node overwrote retained cache");
    check(original_get_target_position(node,cache,position,{},selected,error)&&selected==&position,
          "null node required enabled/cache services");
    original_target_position_ctor_prefix(node,cache);
    check(node==0&&cache&&*cache==OriginalTargetPosition{0,0,0},"constructor cells differ");
    node=91;
    check(original_target_position_named_node_prefix(node,0,{},error)&&node==91,"absent root cleared node");
    check(!original_target_position_named_node_prefix(node,10,{},error)&&node==91,"missing reached lookup wrote zero");
    check(original_target_position_named_node_prefix(node,10,[&](auto root,const char* name,auto& out,std::string&){
        check(root==10&&std::strcmp(name,"target_node")==0,"wrong own scene lookup");out=19;return true;
    },error)&&node==19,"named-node producer failed");
    cache.reset();
    check(!original_update_target_position(node,cache,{},result,error)&&!cache,"missing absolute getter invented cache");
    check(!original_update_target_position(node,cache,[&](auto token,auto& out,std::string&){
        check(token==19,"wrong own scene node");++calls;out={1,2,3};return false;
    },result,error)&&calls==1&&result.node_queried&&!result.cache_written&&!cache,
        "failed borrowed service rolled back external effect or wrote cache");
    std::uint32_t bits=0x7fc01234;float nan;std::memcpy(&nan,&bits,4);
    check(original_update_target_position(node,cache,[&](auto token,auto& out,std::string&){
        check(token==19,"node identity drifted");out={-0.0f,nan,31};node=0;return true;
    },result,error)&&result.cache_written&&cache&&(*cache)[2]==31,
        "snapshot update reread changed node or rejected source values");
    std::uint32_t actual=0;std::memcpy(&actual,&(*cache)[1],4);check(actual==bits,"source NaN bits sanitized");
    node=19;
    for(unsigned value=0;value<256;++value){
        check(original_get_target_position(node,cache,position,[&](auto& out,std::string&){
            out=static_cast<std::uint8_t>(value);return true;
        },selected,error)&&selected==(value?&*cache:&position),"source byte selector differs");
    }
    cache.reset();selected=&position;
    check(!original_get_target_position(node,cache,position,[](auto& out,std::string&){out=1;return true;},selected,error)&&
          selected==&position,"missing selected cache silently fell back");
    check(original_get_target_position(node,cache,position,[](auto& out,std::string&){out=0;return true;},selected,error)&&
          selected==&position,"disabled owner required unused cache");
    check(!original_get_target_position(node,cache,position,[](auto&,std::string&){return false;},selected,error)&&
          selected==&position,"failed enabled owner changed pointer");
    check(argc==2,"actual original staged asset root required");
    AssetCatalog assets(argv[1]);CharacterVisual visual;CharacterVisualConfig config;
    config.model_path="models/prince_modular.bdae";
    config.skin_id_contains="_default_warrior-mesh-skin";config.expected_controller_count=4;
    std::uintptr_t token=88;Mat4 world{};world[0]=77;
    check(!visual.source_target_node(token,error)&&token==88,"unloaded visual fabricated NULL target node");
    check(visual.load(assets,config,error),error.c_str());
    const auto bytes=assets.read(config.model_path);dh2::resources::BresView view{};dh2::scene::Scene decoded;
    check(dh2_bres_open(&view,bytes.data(),bytes.size())==dh2::resources::BresError::ok&&
          dh2::scene::load(view,decoded,error),"actual model independent decode failed");
    const dh2::scene::Node* authored=nullptr;
    // Loader appends nodes in authored depth-first order. Independent data check
    // deliberately reads NAME only, not id/sid aliases.
    for(const auto& candidate:decoded.graph)if(candidate.name=="target_node"){authored=&candidate;break;}
    check(visual.source_target_node(token,error),error.c_str());
    check((token!=0)==(authored!=nullptr),"own source node absence differs from actual original NAME data");
    check(!visual.source_target_node_world(1,world,error)&&world[0]==77,"foreign node token dereferenced or mutated output");
    if(authored){
        check(visual.source_target_node_world(token,world,error)&&world==authored->world,
              "actual own static target node world differs from authored source matrix");
        check(original_target_position_named_node_prefix(node,1,[&](auto,const char* name,auto& out,std::string& e){
            check(std::strcmp(name,"target_node")==0,"host requested alias");return visual.source_target_node(out,e);
        },error)&&node==token,"actual visual producer did not bind own node");
        check(original_update_target_position(node,cache,[&](auto actual_token,auto& out,std::string& e){
            Mat4 current;if(!visual.source_target_node_world(actual_token,current,e))return false;
            out={current[12],current[13],current[14]};return true;
        },result,error)&&*cache==OriginalTargetPosition{world[12],world[13],world[14]},"actual source node failed cache update");
        CharacterVisual second;check(second.load(assets,config,error),error.c_str());
        check(!second.source_target_node_world(token,world,error),"different actor accepted borrowed node token");
        check(visual.update(0.1,error)&&visual.source_target_node_world(token,world,error),"same graph update invalidated actual token");
    }
    CharacterVisual boss;CharacterVisualConfig boss_config;
    boss_config.model_path="original-cache/data/3d/characters/swampking/swampking.bdae";
    check(boss.load(assets,boss_config,error),error.c_str());
    const auto boss_bytes=assets.read(boss_config.model_path);dh2::resources::BresView boss_view{};dh2::scene::Scene boss_scene;
    check(dh2_bres_open(&boss_view,boss_bytes.data(),boss_bytes.size())==dh2::resources::BresError::ok&&
          dh2::scene::load(boss_view,boss_scene,error),"boss original independent decode failed");
    const dh2::scene::Node* boss_authored=nullptr;
    for(const auto& candidate:boss_scene.graph)if(candidate.name=="target_node"){boss_authored=&candidate;break;}
    check(boss_authored!=nullptr,"staged original boss lost authored target_node");
    check(boss.source_target_node(token,error)&&token!=0&&boss.source_target_node_world(token,world,error)&&world==boss_authored->world,
          "original boss target node world is not actual authored matrix");
    check(!visual.source_target_node_world(token,world,error),"player visual accepted boss node token");
    node=token;
    check(original_update_target_position(node,cache,[&](auto own_token,auto& out,std::string& e){
        Mat4 matrix;if(!boss.source_target_node_world(own_token,matrix,e))return false;
        out={matrix[12],matrix[13],matrix[14]};return true;
    },result,error)&&cache&&*cache==OriginalTargetPosition{boss_authored->world[12],boss_authored->world[13],boss_authored->world[14]},
          "actual boss own scene node failed source cache producer");
    check(boss.update(.1,error)&&boss.source_target_node_world(token,world,error),"same boss graph update invalidated node");
    auto bad_boss=boss_config;bad_boss.model_path="missing-model.bdae";
    check(!boss.load(assets,bad_boss,error)&&boss.source_target_node_world(token,world,error),"failed reload invalidated retained node");
    check(boss.load(assets,boss_config,error)&&!boss.source_target_node_world(token,world,error),"successful reload retained stale node token");
    std::cout<<"original actor target position tests PASS; actual model target_node="<<(authored?"present":"absent")<<"\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
