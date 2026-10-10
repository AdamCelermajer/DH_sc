#include "../camera_animset_v1.hpp"
#include "../character_critical_camera_bridge_v1.hpp"

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
using namespace dh2::character_critical_camera_bridge_v1;
struct QueryState {
    std::vector<Query> operations;
    std::vector<std::int32_t> values;
    std::size_t next=0;
    std::uintptr_t identity=0;
    bool fail=false;
};
std::int32_t invoke(void* raw,const QueryRequest& request,std::int32_t* out){
    auto& state=*static_cast<QueryState*>(raw);
    state.operations.push_back(request.operation);
    if(request.character!=state.identity||!out||state.next>=state.values.size())return -1;
    if(state.fail)return 1;
    *out=state.values[state.next++];return 0;
}
}

int main(){
    using namespace dh2::camera_animset_v1;
    Selection selected;selected.name="fixture";selected.idle_clip_id=901;
    selected.idle_path="camera/idle.bdae";
    selected.resources={
        {Resource::Slot::template_animation,0,900,"camera/template.bdae"},
        {Resource::Slot::idle,1,901,"camera/idle.bdae"},
        {Resource::Slot::shake,2,44,"camera/shake.bdae"},
        {Resource::Slot::crit,3,45,"camera/crit.bdae"},
        {Resource::Slot::cam_animation,4,1040,"camera/event.bdae"}};
    dh2::data::Dictionary clips;
    clips.names.resize(1041);clips.values.resize(1041);
    clips.names[1040]="event";clips.values[1040]="camera/event.bdae";

    PlaybackOwner owner;PlayRequest request;std::string error;
    require(owner.bind(selected),"selected AnimSet failed to bind");
    require(owner.level_idle(request,error)&&request.present&&
            request.resource.clip_id==901&&!request.preserve_zoom,
            "entry Idle request differs from source flags");

    dh2::data::AnimationStep step;step.cam=1040;
    require(owner.animation_step(step,-1,request,error)&&request.present&&
            request.resource.clip_id==1040&&request.preserve_zoom,
            "Cam step did not resolve its global AnimDict ID");
    step.cam=-1;step.random_cam={1040};
    require(owner.animation_step(step,1040,request,error)&&request.present&&
            request.resource.clip_id==1040,
            "RandomCam choice did not resolve to the authored resource");
    require(!owner.animation_step(step,45,request,error),
            "RandomCam choice outside its authored group was accepted");

    require(owner.object_event("camera/event",clips,true,request,error)&&request.present&&
            request.resource.clip_id==1040&&request.preserve_zoom,
            "camera/<clip-name> event failed AnimDict-to-AnimSet resolution");
    require(owner.object_event("camera/event",clips,false,request,error)&&!request.present,
            "CanPlayShakeAnim denial did not suppress an event");
    require(owner.combat_shake(true,true,true,request,error)&&request.present&&
            request.trigger==Trigger::combat_crit&&
            request.resource.slot==Resource::Slot::crit&&request.preserve_zoom,
            "critical Player result failed to select configured Crit with preserved zoom");
    require(owner.combat_shake(false,true,true,request,error)&&!request.present&&
            owner.combat_shake(true,true,false,request,error)&&!request.present,
            "Crit request bypassed its source gates");

    using namespace dh2::data;
    CombatResult result{};
    QueryState query_state{{},{},0,0x1234,false};
    Request bridge_request{&result,query_state.identity,&query_state,&invoke};
    Report bridge_report{};
    require(resolve(&bridge_request,owner,&request,&bridge_report)==Status::complete&&
            !request.present&&bridge_report.queries_attempted==0,
            "noncritical result queried player/camera owners");

    result.outcomes=8;
    query_state.values={0};query_state.next=0;query_state.operations.clear();
    require(resolve(&bridge_request,owner,&request,&bridge_report)==Status::complete&&
            !request.present&&query_state.operations==std::vector<Query>{Query::requester_is_player}&&
            bridge_report.combat_result_player_gate==0,
            "nonplayer critical result entered CameraLevel::CanPlayShakeAnim");

    query_state.values={1,1,1,1};query_state.next=0;query_state.operations.clear();
    require(resolve(&bridge_request,owner,&request,&bridge_report)==Status::complete&&
            request.present&&request.trigger==Trigger::combat_crit&&
            request.resource.slot==Resource::Slot::crit&&request.preserve_zoom&&
            query_state.operations==std::vector<Query>{Query::requester_is_player,
                Query::requester_is_player,Query::local_player_count,
                Query::requester_is_local}&&bridge_report.play_requested&&
            bridge_report.camera_can_play,
            "sole local Player critical result did not request source Crit playback");

    query_state.values={1,1,2};query_state.next=0;query_state.operations.clear();
    require(resolve(&bridge_request,owner,&request,&bridge_report)==Status::complete&&
            !request.present&&query_state.operations==std::vector<Query>{
                Query::requester_is_player,Query::requester_is_player,
                Query::local_player_count}&&!bridge_report.camera_can_play,
            "multiple local Players incorrectly reached IsLocalPlayer or PlayAnim");

    query_state.values={1,1,1,0};query_state.next=0;query_state.operations.clear();
    require(resolve(&bridge_request,owner,&request,&bridge_report)==Status::complete&&
            !request.present&&query_state.operations.back()==Query::requester_is_local&&
            !bridge_report.play_requested,
            "nonlocal Player incorrectly requested combat Crit playback");

    query_state.values={1};query_state.next=0;query_state.operations.clear();
    query_state.fail=true;
    require(resolve(&bridge_request,owner,&request,&bridge_report)==Status::provider_failed&&
            !request.present&&bridge_report.queries_attempted==1&&
            bridge_report.queries_completed==0,
            "failed source IsPlayer query was not fail-closed");
    query_state.fail=false;

    // CanPlayShakeAnim makes a second IsPlayer query. Preserve a changed
    // value across that source boundary instead of reusing the earlier fact.
    query_state.values={1,0};query_state.next=0;query_state.operations.clear();
    require(resolve(&bridge_request,owner,&request,&bridge_report)==Status::complete&&
            request.present&&query_state.operations.size()==2&&
            bridge_report.camera_can_play,
            "fresh CanPlayShakeAnim IsPlayer read was not preserved");
    require(owner.script_play_camera(true,-1,request,error)&&request.present&&
            request.resource.slot==Resource::Slot::crit&&!request.preserve_zoom,
            "Script_PlayCamera Crit did not use selected Crit");
    require(owner.script_play_camera(false,1040,request,error)&&request.present&&
            request.resource.clip_id==1040&&!request.preserve_zoom,
            "Script_PlayCamera external AnimDict ID did not resolve");
    require(!owner.script_play_camera(false,99,request,error),
            "unregistered Script_PlayCamera AnimDict ID was accepted");
    return 0;
}
