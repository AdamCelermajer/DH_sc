#include "../camera_animset_bank_playback_v1.hpp"
#include "../character_critical_camera_bridge_v1.hpp"
#include "../../game-data/data.hpp"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using Bytes=std::vector<std::uint8_t>;
Bytes read_file(const std::string& path){
    std::ifstream file(path,std::ios::binary);
    if(!file)throw std::runtime_error("cannot open "+path);
    return {std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};
}
void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
struct Assets {std::string root;};
bool read_asset(void* context,const std::string& path,Bytes& output,std::string& error){
    const auto& assets=*static_cast<Assets*>(context);
    try{output=read_file(assets.root+"/original-cache/"+path);return true;}
    catch(const std::exception& first){
        const auto slash=path.find_last_of("/\\");
        const auto leaf=slash==std::string::npos?path:path.substr(slash+1);
        try{output=read_file(assets.root+"/animations/"+leaf);return true;}
        catch(const std::exception&){error=first.what();return false;}
    }
}
struct State {
    dh2::camera_level_runtime_v1::Owner* camera=nullptr;
    bool accept=true;
    int calls=0;
    bool bytes_valid=false;
    bool saw_zoom_before_callback=false;
};
struct CriticalQueries {
    std::vector<dh2::character_critical_camera_bridge_v1::Query> calls;
    std::uintptr_t identity=0;
};
std::int32_t critical_query(void* raw,
    const dh2::character_critical_camera_bridge_v1::QueryRequest& request,
    std::int32_t* value){
    auto& state=*static_cast<CriticalQueries*>(raw);
    state.calls.push_back(request.operation);
    if(request.character!=state.identity||!value)return -1;
    switch(request.operation){
        case dh2::character_critical_camera_bridge_v1::Query::requester_is_player:
            *value=1;return 0;
        case dh2::character_critical_camera_bridge_v1::Query::local_player_count:
            *value=1;return 0;
        case dh2::character_critical_camera_bridge_v1::Query::requester_is_local:
            *value=1;return 0;
    }
    return -1;
}
bool start_bdae(void* context,const dh2::camera_animset_v1::PlayRequest& request,
                const std::uint8_t* bytes,std::size_t size){
    auto& state=*static_cast<State*>(context);++state.calls;
    state.bytes_valid=request.present&&bytes&&size>=4&&bytes[0]=='B'&&bytes[1]=='R'&&
                      bytes[2]=='E'&&bytes[3]=='S';
    state.saw_zoom_before_callback=state.camera->current_zoom()==0.25f&&
                                   state.camera->target_zoom()==0.75f;
    return state.accept&&state.bytes_valid;
}
}

int main(int argc,char** argv){
    try{
        if(argc!=2)throw std::runtime_error("usage: camera_animset_bank_playback_v1 <assets>");
        const std::string data_root=std::string(argv[1])+"/data/";
        const auto read=[&](const char* name){return read_file(data_root+name);};
        const auto records=read("animations_pyarray.bin");
        const auto names=read("animations_pyarraynames.bin");
        const auto fields=read("animations_pystructnames.bin");
        const auto clip_names=read("animations_dictionary_pyarraynames.bin");
        const auto clip_values=read("animations_dictionary_pyarray.bin");
        const auto view=[](const Bytes& bytes){return dh2::data::Bytes{bytes.data(),bytes.size()};};
        dh2::data::Dictionary clips;dh2::data::AnimationTables tables;std::string error;
        require(dh2::data::load_dictionary(view(clip_names),view(clip_values),clips,error),
                "could not load source AnimDict");
        require(dh2::data::load_animation_tables(view(records),view(names),view(fields),clips,tables,error),
                "could not load source CamAnimSets");
        dh2::camera_animset_v1::Selection selected;
        require(dh2::camera_animset_v1::select(tables,clips,"Default",selected,error),
                "Default CamAnimSet selection failed");
        dh2::camera_animset_bank_v1::Owner bank;Assets assets{argv[1]};
        require(bank.load(selected,read_asset,&assets,error),"Default clip bank failed to load");
        dh2::camera_animset_v1::PlaybackOwner requests;
        require(requests.bind(selected),"Default playback request owner failed to bind");
        dh2::camera_animset_v1::PlayRequest idle;
        require(requests.level_idle(idle,error)&&idle.present,"Default Idle request failed");

        dh2::camera_level_runtime_v1::Owner camera;camera.set_zoom(0.25f,0.75f);
        State state{&camera,true,0,false,false};
        const dh2::camera_animset_bank_playback_v1::Backend backend{&state,start_bdae};
        using Status=dh2::camera_animset_bank_playback_v1::Status;
        require(dh2::camera_animset_bank_playback_v1::start(camera,bank,{},backend,error)==
                    Status::no_request&&state.calls==0,"absent request reached animator callback");
        require(dh2::camera_animset_bank_playback_v1::start(camera,bank,idle,backend,error)==
                    Status::started&&state.calls==1&&state.bytes_valid&&state.saw_zoom_before_callback,
                "selected BDAE did not reach existing animator callback before CameraLevel update");
        require(camera.current_zoom()==0.0f&&camera.target_zoom()==1.0f,
                "source preserveZoom=false did not update the one CameraLevel owner");

        auto missing=idle;missing.resource.path+=".missing";
        state.calls=0;camera.set_zoom(0.25f,0.75f);
        require(dh2::camera_animset_bank_playback_v1::start(camera,bank,missing,backend,error)==
                    Status::bank_resource_missing&&state.calls==0&&camera.current_zoom()==0.25f,
                "missing bank resource reached callback or changed zoom state");

        state.accept=false;camera.set_zoom(0.25f,0.75f);
        require(dh2::camera_animset_bank_playback_v1::start(camera,bank,idle,backend,error)==
                    Status::playback_rejected&&camera.current_zoom()==0.25f&&
                    camera.target_zoom()==0.75f,
                "rejected animation changed CameraLevel zoom state");

        Bytes scene_bytes,idle_bytes;
        require(read_asset(&assets,"data/3d/camera/cameratests.bdae",scene_bytes,error),
                "CameraTests scene was not staged");
        require(read_asset(&assets,selected.idle_path,idle_bytes,error),
                "selected Idle BDAE was not staged");
        dh2::player_camera_rig_v1::Rig rig;
        require(rig.load({scene_bytes.data(),scene_bytes.size(),idle_bytes.data(),idle_bytes.size()},error),
                "CameraTests rig did not load");
        dh2::player_camera_rig_v1::Playback timeline;
        camera.set_zoom(0.25f,0.75f);
        require(dh2::camera_animset_bank_playback_v1::start_rig(camera,bank,idle,rig,timeline,error)==
                    Status::started&&camera.animation_active()&&
                    camera.current_zoom()==0.0f&&camera.target_zoom()==1.0f,
                "source Level::_LoadCamera Idle PlayAnim did not activate CameraLevel state");
        dh2::player_camera_rig_v1::Pose idle_pose{},effect_start{},effect_middle{};
        require(timeline.advance(rig,0,&idle_pose,error),"source Idle did not sample");

        dh2::data::CombatResult critical_result{};critical_result.outcomes=8;
        CriticalQueries critical_queries{};critical_queries.identity=0x1234;
        const dh2::character_critical_camera_bridge_v1::Request critical_request{
            &critical_result,critical_queries.identity,&critical_queries,&critical_query};
        dh2::camera_animset_v1::PlayRequest critical;
        dh2::character_critical_camera_bridge_v1::Report critical_report{};
        require(dh2::character_critical_camera_bridge_v1::resolve(
                    &critical_request,requests,&critical,&critical_report)==
                    dh2::character_critical_camera_bridge_v1::Status::complete&&
                    critical_report.play_requested&&critical.present&&
                    critical.trigger==dh2::camera_animset_v1::Trigger::combat_crit&&
                    critical.resource.slot==dh2::camera_animset_v1::Resource::Slot::crit&&
                    critical.preserve_zoom&&critical_queries.calls.size()==4,
                "Character::F_ApplyResult did not source-gate Crit/PlayAnim(id,0,true)");
        camera.set_zoom(0.25f,0.75f);
        require(dh2::camera_animset_bank_playback_v1::start_rig(
                    camera,bank,critical,rig,timeline,error)==Status::started&&
                    camera.current_zoom()==0.25f&&camera.target_zoom()==0.75f,
                "Combat Crit request did not preserve the active CameraLevel zoom");
        require(timeline.advance(rig,0,&effect_start,error),"source combat Crit did not sample");
        require(timeline.advance(rig,(rig.animation_end()-rig.animation_start())/2,
                                 &effect_middle,error),"source combat Crit midpoint did not sample");
        float effect_delta=0.0f;
        for(unsigned i=0;i<16;++i)
            effect_delta+=std::abs(effect_start.camera[i]-effect_middle.camera[i])+
                         std::abs(effect_start.target[i]-effect_middle.target[i]);
        require(effect_delta>1e-4f,"source combat Crit request did not change the camera pose");
        require(camera.animation_active(),"accepted combat Crit did not enter CameraLevel animation mode");
        require(dh2::camera_animset_bank_playback_v1::advance_rig(
                    camera,rig,timeline,
                    static_cast<std::uint32_t>(rig.animation_end()-rig.animation_start()+1),
                    &effect_middle,error),error.c_str());
        require(timeline.completed()&&!camera.animation_active()&&
                    camera.current_zoom()==0.25f&&camera.target_zoom()==0.75f,
                "completed preserveZoom Crit did not run CameraLevel callback semantics");

        dh2::camera_animset_v1::PlayRequest crit;
        require(requests.script_play_camera(true,-1,crit,error)&&crit.present,
                "source Script_PlayCamera Crit did not select registered Crit");
        camera.set_zoom(0.25f,0.75f);
        require(dh2::camera_animset_bank_playback_v1::start_rig(
                    camera,bank,crit,rig,timeline,error)==Status::started&&
                    camera.current_zoom()==0.0f&&camera.target_zoom()==1.0f,
                "Crit request did not apply source preserveZoom=false to CameraLevel");
        require(timeline.advance(rig,0,&effect_start,error),"source Crit did not sample");
        require(timeline.current_time_ms()==rig.animation_start(),
                "Crit did not restart the single animator timeline");
        require(camera.animation_active(),"accepted Script Crit did not enter CameraLevel animation mode");
        require(dh2::camera_animset_bank_playback_v1::advance_rig(
                    camera,rig,timeline,
                    static_cast<std::uint32_t>(rig.animation_end()-rig.animation_start()+1),
                    &effect_middle,error),error.c_str());
        require(timeline.completed()&&!camera.animation_active()&&
                    camera.current_zoom()==0.0f&&camera.target_zoom()==1.0f,
                "completed non-preserving Crit did not return to normal zoom mode");
        return 0;
    }catch(const std::exception& e){fprintf(stderr,"FAIL: %s\n",e.what());return 1;}
}
