#include "../asset_catalog.hpp"
#include "../content_paths.hpp"
#include "../original_scene.hpp"
#include "../original_character.hpp"
#include "../../engine-skinning/skinning.hpp"
#include "../../game-data/items.hpp"
#include "../../game-data/loot_audiovisual_v8.hpp"
#include <iostream>
#include <stdexcept>
#include <set>

using namespace dh::foundation;
void require(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
int main(int argc,char** argv){try{
    require(argc==2,"Supply staged original asset root");AssetCatalog assets(argv[1]);std::string error;
    const auto read=[&](const char* file){return read_content(assets,std::string("data/pydata/")+file);};
    const auto bytes=[](const std::vector<std::uint8_t>& value){return dh2::data::Bytes{value.data(),value.size()};};
    auto records=read("loot_table_pyarray.bin"),names=read("loot_table_pyarraynames.bin"),fields=read("loot_table_pystructnames.bin");
    dh2::data::ItemTable items;require(dh2::data::load_items(bytes(records),bytes(names),bytes(fields),items,error),error);
    const auto swordId=dh2::data::item_id(items,"Longsword01");const auto* sword=dh2::data::item(items,swordId);
    require(sword!=nullptr,"Original Longsword01 row missing");
    std::cout<<"Source Longsword01 row="<<swordId<<" swooshSound="<<sword->record.words[5]<<" swooshFx="<<sword->record.words[6]<<'\n';
    records=read("loot_audiovisual_pyarray.bin");names=read("loot_audiovisual_pyarraynames.bin");fields=read("loot_audiovisual_pystructnames.bin");
    dh2::data::LootAudioVisualV8 audiovisual;require(audiovisual.load(bytes(records),bytes(names),bytes(fields),error),error);
    const auto model=assets.read("data/3d/gameobjects/itemdrops.bdae");
    dh2::resources::BresView view{};
    require(dh2_bres_open(&view,model.data(),model.size())==dh2::resources::BresError::ok,"Original itemdrop BRES rejected");
    dh2::scene::Scene authored;require(dh2::scene::load(view,authored,error),error);
    for(const char* itemName: {"Potion0","GoldStack01"}){
        const auto id=dh2::data::item_id(items,itemName);const auto* item=dh2::data::item(items,id);
        require(item!=nullptr,"Original drop ItemTable row missing");const auto av=item->record.words[21];
        const auto table=audiovisual.borrow();require(av>=0&&std::size_t(av)<table.rows().size(),"Original drop AudioVisual index out of bounds");
        const auto& visual=table.rows()[std::size_t(av)].visual;
        std::int32_t root=-1;
        for(std::size_t n=0;n<authored.graph.size();++n)if(authored.graph[n].name==visual||authored.graph[n].id==visual||authored.graph[n].sid==visual){
            require(root<0,"Ambiguous original drop visual");root=static_cast<std::int32_t>(n);
        }
        require(root>=0,"Original drop visual node missing");std::set<std::uint32_t> controllers;
        for(const auto& instance:authored.instances){
            auto ancestor=static_cast<std::int32_t>(instance.node_index);
            while(ancestor>=0&&ancestor!=root)ancestor=authored.graph.at(std::size_t(ancestor)).parent;
            if(ancestor==root&&instance.controller>=0)controllers.insert(static_cast<std::uint32_t>(instance.controller));
        }
        if(!controllers.empty()){
            CharacterVisualConfig config;config.model_path="data/3d/gameobjects/itemdrops.bdae";
            for(auto controller:controllers){dh2::skinning::Skin skin;require(dh2::skinning::load(view,controller,authored,skin,error),error);config.controller_ids.push_back(skin.id);}
            config.expected_controller_count=static_cast<unsigned>(controllers.size());CharacterVisual posed;
            require(posed.load(assets,config,error),error);std::size_t triangles=0,vertices=0;
            for(const auto& mesh:posed.meshes()){triangles+=mesh.indices.size()/3;vertices+=mesh.vertices.size();}
            require(triangles>0&&vertices>0,"Original skinned itemdrop rest pose has no geometry");
            std::cout<<"PASS item="<<itemName<<" id="<<id<<" audioVisual="<<av<<" visual="<<visual
                     <<" skinnedControllers="<<controllers.size()<<" triangles="<<triangles<<" vertices="<<vertices<<" pose=authoredRest\n";
            for(const auto& material:posed.original_materials())std::cout<<"material diffuse="<<material.diffuse<<" alphaMap="<<material.alphaMap<<'\n';
            continue;
        }
        OriginalScene scene;
        const Mat4 placement{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        require(decode_original_scene_module(model,visual,placement,scene,error),error);
        require(scene.triangleCount>0&&!scene.mesh.vertices.empty()&&!scene.mesh.indices.empty(),"Selected original drop has no visible geometry");
        require(!scene.materials.empty()&&scene.mesh.ranges.size()==scene.materials.size(),"Selected original drop lost material ranges");
        std::cout<<"PASS item="<<itemName<<" id="<<id<<" audioVisual="<<av<<" visual="<<visual
                 <<" triangles="<<scene.triangleCount<<" vertices="<<scene.mesh.vertices.size()<<" materials="<<scene.materials.size()<<'\n';
        for(const auto& material:scene.materials)std::cout<<"material diffuse="<<material.diffuse<<" alphaMap="<<material.alphaMap<<'\n';
    }
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
