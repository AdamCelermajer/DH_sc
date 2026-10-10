#include "modular_defaults.hpp"
#include "original_character.hpp"
#include "asset_catalog.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
namespace fs=std::filesystem;
using namespace dh::foundation;
std::vector<std::uint8_t> read(const fs::path& path) {
    std::ifstream file(path,std::ios::binary);
    if(!file)throw std::runtime_error("Missing original modular fixture: "+path.string());
    return {std::istreambuf_iterator<char>(file),{}};
}
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
int main(int argc,char** argv) {
    try {
        check(argc==2,"Supply repository root");const fs::path root=argv[1];
        const auto prince=read(root/"port/android-native/app/src/main/assets/models/prince_modular.bdae");
        std::vector<ModularDefaultCategory> categories;std::string error;
        check(decode_modular_defaults(prince,categories,error),error.c_str());
        check(categories.size()==4,"Original Prince category count differs");
        for(const auto& category:categories) {
            check(category.node_id=="prince_modular-node","Original node differs");
            check(category.controller_id==category.category+"__naked-mesh-skin","Serialized default is not naked category");
            check(category.default_uri=="#"+category.controller_id,"URI fragment differs");
            check(category.available_controller_ids.size()==43,"Original module count differs");
            for(const auto* type:{"warrior","rogue","mage"}) {
                if(category.category=="MC_Head")continue;
                const auto wanted=category.category+"_default_"+type+"-mesh-skin";
                check(std::find(category.available_controller_ids.begin(),category.available_controller_ids.end(),wanted)!=category.available_controller_ids.end(),
                      "Source class clothing variant missing");
                check(category.controller_id!=wanted,"Provider guessed a class instead of serialized default");
            }
        }
        auto previous=categories;
        AssetCatalog actorAssets(root/".local-inputs/windows-shared-assets");
        CharacterVisualConfig visualConfig;visualConfig.model_path="models/prince_modular.bdae";
        visualConfig.template_clip_path="animations/prince_template_anim.bdae";
        visualConfig.clips={{"idle","animations/prince_idle_shield.bdae"}};
        visualConfig.expected_controller_count=5;visualConfig.allow_missing_animation_targets=true;
        visualConfig.use_authored_modular_defaults=true;CharacterVisual defaultsVisual,exactVisual;
        check(defaultsVisual.load(actorAssets,visualConfig,error),error.c_str());
        visualConfig.use_authored_modular_defaults=false;
        visualConfig.expected_controller_count=4;
        for(const auto& category:previous)visualConfig.controller_ids.push_back(category.controller_id);
        check(exactVisual.load(actorAssets,visualConfig,error),error.c_str());
        check(defaultsVisual.meshes().size()==5&&exactVisual.meshes().size()==4,"Actor did not render four source modules plus its referenced auxiliary");
        for(std::size_t i=0;i<4;++i) {
            check(defaultsVisual.texture_uris()[i]==exactVisual.texture_uris()[i],"Source defaults and exact controller textures differ");
            const auto& a=defaultsVisual.meshes()[i].vertices;const auto& b=exactVisual.meshes()[i].vertices;
            check(a.size()==b.size(),"Selected modular geometry differs");
            for(std::size_t v=0;v<a.size();++v)check(a[v].position.x==b[v].position.x&&a[v].position.y==b[v].position.y&&a[v].position.z==b[v].position.z,"Source defaults and exact controller poses differ");
        }
        const auto before=exactVisual.meshes().size();visualConfig.controller_ids.push_back("missing-controller");
        check(!exactVisual.load(actorAssets,visualConfig,error)&&exactVisual.meshes().size()==before,"Unknown controller did not fail atomically");
        // B061: swapping equipped modular parts keeps the same Scene/skeleton, redeforms, and fails atomically.
        {
            const auto* sceneBefore=exactVisual.retained_scene_borrow();
            std::vector<std::string> ids;for(const auto& category:previous)ids.push_back(category.controller_id);
            std::size_t head=ids.size();for(std::size_t i=0;i<ids.size();++i)if(ids[i].find("MC_Head")!=std::string::npos)head=i;
            check(head<ids.size(),"Default modular set lacks a head controller");
            const auto oldTexture=exactVisual.texture_uris()[head];const auto oldVertices=exactVisual.meshes()[head].vertices.size();
            auto swapped=ids;swapped[head]="MC_Head_Plate_01-mesh-skin";
            check(exactVisual.reselect_controllers(actorAssets,swapped,error),error.c_str());
            check(exactVisual.retained_scene_borrow()==sceneBefore&&exactVisual.meshes().size()==ids.size(),"Part swap replaced the Scene or lost a part");
            bool replaced=false;for(const auto& mesh:exactVisual.meshes()){check(!mesh.vertices.empty(),"Swapped part has no geometry");}
            replaced=exactVisual.texture_uris()[head]==oldTexture&&exactVisual.meshes()[head].vertices.size()==oldVertices;
            check(!replaced,"Helm swap still draws the old head part");
            auto broken=swapped;broken.push_back("missing-controller");
            const auto texturesBefore=exactVisual.texture_uris();
            check(!exactVisual.reselect_controllers(actorAssets,broken,error)&&exactVisual.texture_uris()==texturesBefore&&exactVisual.meshes().size()==ids.size(),"Unknown controller swap was not atomic");
            check(exactVisual.reselect_controllers(actorAssets,ids,error)&&exactVisual.texture_uris()[head]==oldTexture&&exactVisual.meshes()[head].vertices.size()==oldVertices,"Restoring the default part set failed");
        }
        for(auto size:{std::size_t(0),std::size_t(24),prince.size()/2}) {
            auto truncated=prince;truncated.resize(size);
            check(!decode_modular_defaults(truncated,categories,error),"Truncated BRES accepted");
            check(categories.size()==previous.size()&&categories[0].controller_id==previous[0].controller_id,"Failure overwrote caller output");
        }
        const auto priest=read(root/".local-inputs/windows-shared-assets/original-cache/data/3d/characters/npcs/priest_good.bdae");
        check(decode_modular_defaults(priest,categories,error),error.c_str());
        check(categories.empty(),"Nonmodular Priest unexpectedly borrowed player modules");
        std::cout<<"PASS serialized defaults, class-module availability, no player-class guess, nonmodular NPC, atomic malformed rejection\n";
        return 0;
    }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
