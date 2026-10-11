// B079: source evidence for the item spawn arc and the hero Interact clip.
//  - ItemObject::InitAgain (0x3ec0f0) plays clip 0 of itemdrops.bdae once: every item dummy rises ~600 units, tumbles and
//    falls back to its rest pose by ~1.6 s (Z of dummy_itemdrop_* sampled here).
//  - CharAnimTable "Interact" -> prince_interaction.bdae: 1200 ms with the authored "interact" marker at 467 ms.
#include "../original_character.hpp"
#include "../animation_markers.hpp"
#include "../asset_catalog.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
int main(int argc,char** argv){try{
    check(argc==2,"Supply the asset root");AssetCatalog assets(argv[1]);std::string error;
    {   // item spawn clip
        const auto bytes=assets.read("data/3d/gameobjects/itemdrops.bdae");std::vector<EmbeddedSceneClip> clips;
        check(decode_embedded_scene_clips(bytes.data(),bytes.size(),clips,error),error);
        check(clips.size()==1&&clips[0].start_ms==0&&clips[0].end_ms==2566,"itemdrops.bdae must hold the single 0..2566 ms scene clip");
        CharacterVisual visual;check(visual.load_embedded_scene(assets,"data/3d/gameobjects/itemdrops.bdae",error),error);
        check(visual.select(clips[0].name,false,error),error);
        float rest=0,peak=0,peakMs=0,late=0;
        for(int ms=0;ms<=1800;ms+=8){
            if(ms&&!visual.update(0.008,error))throw std::runtime_error(error);
            Mat4 m;check(visual.bone_world("dummy_itemdrop_Longsword",m,error),error);
            if(ms==0)rest=m[14];if(m[14]-rest>peak){peak=m[14]-rest;peakMs=float(ms);}if(ms==1800)late=m[14];
        }
        check(peak>500&&peak<700,"Item arc peak height differs from the authored ~600 units");
        check(peakMs>=300&&peakMs<=700,"Item arc apex is not near 0.5 s");
        check(std::fabs(late-rest)<1.0f,"Item must be back at its rest pose after the clip");
        std::cout<<"item spawn arc peak="<<peak<<" at "<<peakMs<<" ms, rest after clip PASS\n";
    }
    {   // hero interact clip
        CharacterVisual visual;CharacterVisualConfig config;config.model_path="models/prince_modular.bdae";
        config.template_clip_path="animations/prince_template_anim.bdae";config.skin_id_contains="_default_warrior-mesh-skin";
        config.clips.push_back({"interact","animations/prince_interaction.bdae"});
        check(visual.load(assets,config,error),error);
        std::int32_t start=0,end=0;check(visual.animation_range("interact",start,end,error)&&end-start==1200,"Hero interact clip must last 1200 ms");
        const auto* markers=visual.markers("interact",error);check(markers,error);
        bool found=false;for(const auto& marker:markers->markers())if(marker.name=="interact"){found=true;check(marker.time_ms-markers->start_ms()==467,"interact marker must be at 467 ms");}
        check(found,"interact marker absent");
        std::cout<<"hero interact clip 1200 ms, marker 467 ms PASS\n";
    }
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
