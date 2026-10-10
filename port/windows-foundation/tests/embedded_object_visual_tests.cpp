#include "../original_character.hpp"
#include "../retained_animation_owner.hpp"
#include "../animation_markers.hpp"
#include "../asset_catalog.hpp"
#include "../../scene-materials/scene.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
static std::vector<Vec3> positions(const CharacterVisual& v){std::vector<Vec3> out;for(const auto& mesh:v.meshes())for(const auto& vertex:mesh.vertices)out.push_back(vertex.position);return out;}
static bool same(const std::vector<Vec3>& a,const std::vector<Vec3>& b){if(a.size()!=b.size())return false;for(std::size_t i=0;i<a.size();++i)if(a[i].x!=b[i].x||a[i].y!=b[i].y||a[i].z!=b[i].z)return false;return true;}
int main(int argc,char** argv){try{
    check(argc==2,"Supply isolated original object asset root");AssetCatalog assets(argv[1]);std::string error;
    for(const auto* model:{"go_chest_swamp.bdae","go_swamp_urn_breakable.bdae"}){
        const auto bytes=assets.read(model);std::vector<EmbeddedSceneClip> clips;
        check(decode_embedded_scene_clips(bytes.data(),bytes.size(),clips,error),error);
        check(clips.size()==3,"Actual object named clip count differs");
        CharacterVisual visual;check(visual.load_embedded_scene(assets,model,error),error);
        check(!visual.meshes().empty()&&visual.meshes().size()==visual.original_materials().size(),"Original object geometry/materials not retained");
        check(visual.meshes().size()==2,"Object physics helper entered draw geometry");
        for(const auto& texture:visual.texture_uris())check(texture=="atlas_dh2_game_objects_001.tga","Original object draw texture differs");
        RetainedAnimationOwner animation(visual,[](const std::string&,std::int32_t,std::array<float,3>& point,std::string&){point={0,0,0};return true;});
        for(const auto& clip:clips){
            std::int32_t start{},end{};check(visual.animation_range(clip.name,start,end,error)&&start==clip.start_ms&&end==clip.end_ms,"Embedded visual range differs");
            check(animation.bind_events(clip.name,bytes.data(),bytes.size(),error),error);
        }
        RetainedAnimationFrame frame;check(animation.select("idle",true,1,0,false,frame,error),error);
        const auto closed=positions(visual);
        check(animation.select("activate",false,1,0,false,frame,error),error);
        const auto* markers=visual.markers("activate",error);check(markers,error);
        const auto opened=std::find_if(markers->markers().begin(),markers->markers().end(),[](const auto& m){return m.name=="opened";});
        check(opened!=markers->markers().end(),"Original opened marker absent");
        unsigned events=0,completions=0;std::uint32_t emittedWall=0;bool moved=false;
        for(unsigned ms=1;ms<2200;++ms){
            check(animation.advance(.001,frame,error),error);
            for(const auto& event:frame.events)if(event.name=="opened"){++events;emittedWall=event.wall_timestamp_ms;check(event.clip_id=="activate","Opened event came from wrong clip");}
            if(frame.completion.pending){++completions;animation.take_completion();}
            if(!same(closed,positions(visual)))moved=true;
            if(ms==300){const auto paused=positions(visual);for(unsigned p=0;p<10;++p){check(animation.advance(0,frame,error),error);check(frame.events.empty()&&same(paused,positions(visual)),"Paused object advanced/emitted");}}
        }
        check(events==1&&moved,"Original activate pose/opened missing or repeated");
        check(completions==1,"Original completion missing or repeated");
        check(emittedWall>=std::uint32_t(opened->time_ms-markers->start_ms())&&
              emittedWall<=std::uint32_t(opened->time_ms-markers->start_ms()+1),"Opened did not follow original source event boundary");
        check(animation.select("idleactive",true,1,0,false,frame,error),error);
        check(frame.events.empty(),"Restored opened-state selection replayed opening");
        check(animation.advance(.1,frame,error)&&frame.events.empty(),"Restored idleactive replayed opening");
        const auto held=positions(visual);check(!visual.load_embedded_scene(assets,"missing.bdae",error)&&same(held,positions(visual)),"Failed object reload discarded pose");
        check(!animation.select("missing",false,1,0,false,frame,error)&&same(held,positions(visual)),"Invalid object clip changed pose");
        std::cout<<model<<" meshes="<<visual.meshes().size()<<" openedWall="<<emittedWall<<"ms completion="<<completions<<" original pose/events/restore PASS\n";
    }
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
