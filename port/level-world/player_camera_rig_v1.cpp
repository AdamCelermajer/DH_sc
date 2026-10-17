#include "player_camera_rig_v1.hpp"
#include "../engine-resources/resources.hpp"
#include "../scene-payloads/scene.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <utility>

namespace dh2::player_camera_rig_v1 {
namespace {
using Matrix=std::array<float,16>;

std::int32_t signed_time(std::uint32_t value) noexcept {
    std::int32_t result{};
    std::memcpy(&result,&value,sizeof(result));
    return result;
}

struct Reader {
    const resources::BresView& image;
    const std::uint8_t* at(std::uint64_t offset,std::uint64_t size) const {
        if(!image.bytes||offset>image.size||size>image.size-offset)
            throw std::runtime_error("Camera BRES field outside image");
        return image.bytes+offset;
    }
    std::uint32_t word(std::uint64_t offset) const {
        const auto* p=at(offset,4);
        return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)
             |(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);
    }
    float real(std::uint64_t offset) const {
        const auto bits=word(offset);float value;std::memcpy(&value,&bits,4);
        if(!std::isfinite(value))throw std::runtime_error("Nonfinite camera parameter");
        return value;
    }
    std::string string(std::uint32_t offset) const {
        if(!offset)throw std::runtime_error("Missing camera string");
        const auto* begin=at(offset,1);
        const auto remaining=std::min<std::size_t>(image.size-offset,4096);
        const auto* end=static_cast<const std::uint8_t*>(std::memchr(begin,0,remaining));
        if(!end)throw std::runtime_error("Unterminated camera string");
        return std::string(reinterpret_cast<const char*>(begin),end-begin);
    }
};

std::string field_string(const Reader& reader,std::uint64_t offset) {
    return reader.string(reader.word(offset));
}

Matrix identity() { return {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}; }

bool inverse_affine(const Matrix& in,Matrix& out) {
    // Gauss-Jordan on the 3x3 linear part, using doubles to keep the check
    // stable for authored non-uniform scale while returning source floats.
    double a[3][6]{};
    for(unsigned row=0;row<3;++row){
        for(unsigned column=0;column<3;++column)a[row][column]=in[column*4+row];
        a[row][3+row]=1.0;
    }
    for(unsigned column=0;column<3;++column){
        unsigned pivot=column;
        for(unsigned row=column+1;row<3;++row)
            if(std::abs(a[row][column])>std::abs(a[pivot][column]))pivot=row;
        if(!std::isfinite(a[pivot][column])||std::abs(a[pivot][column])<1e-12)return false;
        if(pivot!=column)for(unsigned c=0;c<6;++c)std::swap(a[pivot][c],a[column][c]);
        const double divisor=a[column][column];
        for(unsigned c=0;c<6;++c)a[column][c]/=divisor;
        for(unsigned row=0;row<3;++row)if(row!=column){
            const double factor=a[row][column];
            for(unsigned c=0;c<6;++c)a[row][c]-=factor*a[column][c];
        }
    }
    out=identity();
    for(unsigned row=0;row<3;++row)for(unsigned column=0;column<3;++column){
        const double value=a[row][3+column];
        if(!std::isfinite(value)||std::abs(value)>3.4e38)return false;
        out[column*4+row]=static_cast<float>(value);
    }
    for(unsigned row=0;row<3;++row){
        double value=0;
        for(unsigned column=0;column<3;++column)value-=a[row][3+column]*in[12+column];
        if(!std::isfinite(value)||std::abs(value)>3.4e38)return false;
        out[12+row]=static_cast<float>(value);
    }
    return true;
}

struct GraphCollector {
    scene::Scene* scene=nullptr;
    std::string camera_selector;
    std::int32_t parents[64]{};
    std::vector<std::pair<std::string,std::int32_t>> camera_instances;
    std::int32_t selected_node=-1;
    unsigned selector_matches=0;
    std::string error;
};

bool collect_node(const scene_payload::Node* source,const dh2::math::Matrix4f* world,
                  std::uint32_t depth,void* opaque) {
    auto& state=*static_cast<GraphCollector*>(opaque);
    if(!source||!world||depth>=64||state.scene->graph.size()>=10000){
        state.error="Camera graph exceeds its node limit";return false;
    }
    scene::Node node;
    node.id=source->id;node.name=source->name;
    node.parent=depth?state.parents[depth-1]:-1;
    std::copy(source->position,source->position+3,node.translation);
    std::copy(source->rotation,source->rotation+4,node.quaternion);
    std::copy(source->scale,source->scale+3,node.scale);
    std::copy(world->m,world->m+16,node.world.begin());
    const auto index=static_cast<std::int32_t>(state.scene->graph.size());
    state.parents[depth]=index;state.scene->graph.push_back(std::move(node));
    if(!state.camera_selector.empty()&&source->name==state.camera_selector){
        state.selected_node=index;++state.selector_matches;
    }

    // Camera instance type 1 stores the referenced camera URI at payload +4.
    // This is the camera-instance layout present in playercamera.bdae; the
    // generic scene decoder intentionally leaves non-geometry payloads opaque.
    for(std::uint32_t i=0;i<source->instances;++i){
        scene_payload::Instance instance{};
        if(dh2_scene_instance(source,static_cast<std::int32_t>(i),&instance)!=scene_payload::Error::ok){
            state.error="Invalid camera scene instance";return false;
        }
        if(instance.type!=1)continue;
        Reader reader{source->image};
        try{
            const auto uri=reader.string(reader.word(std::uint64_t(instance.payload_offset)+4));
            if(uri.size()>1&&uri[0]=='#')
                state.camera_instances.emplace_back(uri.substr(1),index);
        }catch(const std::exception& e){state.error=e.what();return false;}
    }
    return true;
}

std::size_t unique_node(const scene::Scene& graph,const std::string& id) {
    std::size_t found=graph.graph.size(),count=0;
    for(std::size_t i=0;i<graph.graph.size();++i)if(graph.graph[i].id==id){found=i;++count;}
    if(count!=1)throw std::runtime_error("Camera graph node is missing or ambiguous: "+id);
    return found;
}

bool is_descendant_or_same(const scene::Scene& graph,std::size_t node,
                           std::size_t ancestor) {
    if(node>=graph.graph.size()||ancestor>=graph.graph.size())return false;
    auto current=static_cast<std::int32_t>(node);
    for(std::size_t steps=0;current>=0&&steps<graph.graph.size();++steps){
        if(static_cast<std::size_t>(current)==ancestor)return true;
        if(static_cast<std::size_t>(current)>=graph.graph.size())return false;
        current=graph.graph[static_cast<std::size_t>(current)].parent;
    }
    return false;
}

bool load_impl(const Assets& assets,scene::Scene& out_scene,animation::Player& out_idle,
               Projection& out_projection,std::size_t& out_root,std::size_t& out_camera,
               std::size_t& out_target,std::size_t& out_up,bool& out_up_from_camera,
               float& out_default_target_distance,std::string& error) {
    if(!assets.camera_scene||!assets.idle_animation||!assets.camera_scene_size||!assets.idle_animation_size
       ||assets.camera_scene_size>32*1024*1024||assets.idle_animation_size>32*1024*1024){
        error="Invalid or oversized camera rig assets";return false;
    }
    resources::BresView camera_image{};
    if(dh2_bres_open(&camera_image,assets.camera_scene,assets.camera_scene_size)!=resources::BresError::ok){
        error="Camera scene BRES rejected";return false;
    }
    const auto camera_count=dh2_bres_library_count(&camera_image,resources::Library::camera);
    if(camera_count==0||camera_count>256){error="Camera scene has no supported Camera records";return false;}
    const bool named_camera=assets.camera_name&&*assets.camera_name;
    const auto requested_camera_name=named_camera?std::string(assets.camera_name):std::string{};
    if(requested_camera_name.size()>4096||requested_camera_name.find('\0')!=std::string::npos){
        error="Requested camera name is invalid";return false;
    }
    if(!named_camera&&camera_count!=1){error="Expected one authored Camera record";return false;}
    struct CameraRecord {
        std::string id;
        const std::uint8_t* bytes=nullptr;
    };
    std::vector<CameraRecord> camera_records;
    camera_records.reserve(camera_count);
    for(std::uint32_t i=0;i<camera_count;++i){
        const auto* record=dh2_bres_library_item(&camera_image,resources::Library::camera,
                                                 static_cast<std::int32_t>(i));
        if(!record){error="Camera scene contains an invalid Camera record";return false;}
        try{
            const auto offset=static_cast<std::uint64_t>(record-camera_image.bytes);
            const auto id=field_string(Reader{camera_image},offset);
            if(std::any_of(camera_records.begin(),camera_records.end(),
                [&id](const CameraRecord& existing){return existing.id==id;})){
                error="Camera scene contains duplicate Camera record identifiers";return false;
            }
            camera_records.push_back({id,record});
        }catch(const std::exception& e){error=e.what();return false;}
    }

    scene_payload::Scene serialized{};
    if(dh2_scene_open(&serialized,&camera_image)!=scene_payload::Error::ok){
        error="Camera scene payload rejected";return false;
    }
    if(!serialized.visuals||serialized.visuals>256){error="Camera scene has no supported visual";return false;}
    auto candidate_scene=scene::Scene{};
    std::int32_t camera_node=-1;
    std::int32_t camera_scope_node=-1;
    std::string camera_id;
    unsigned matching_visuals=0;
    for(std::uint32_t visual_index=0;visual_index<serialized.visuals;++visual_index){
        scene_payload::Visual visual{};
        if(dh2_scene_visual(&serialized,static_cast<std::int32_t>(visual_index),&visual)!=scene_payload::Error::ok){
            error="Camera visual record rejected";return false;
        }
        scene::Scene graph{};
        GraphCollector collector{&graph,requested_camera_name,{},
                                 {},-1,0,{}};
        const auto walked=dh2_scene_walk_visual(&visual,collect_node,&collector,10000);
        if(walked!=scene_payload::Error::ok){
            error=collector.error.empty()?"Camera visual graph rejected":collector.error;return false;
        }
        if(named_camera&&collector.selector_matches>1){
            error="Named camera selector is ambiguous in the visual graph";return false;
        }
        std::size_t matching_instances=0;
        std::string selected_id;
        std::int32_t selected_camera_node=-1;
        for(const auto& [instance_id,instance_node]:collector.camera_instances){
            const bool inside_selected=named_camera&&collector.selector_matches==1&&
                is_descendant_or_same(graph,static_cast<std::size_t>(instance_node),
                                      static_cast<std::size_t>(collector.selected_node));
            const auto record=std::find_if(camera_records.begin(),camera_records.end(),
                [&instance_id](const CameraRecord& item){return item.id==instance_id;});
            if(record==camera_records.end())continue;
            if((named_camera&&inside_selected)||(!named_camera&&camera_count==1)){
                selected_id=instance_id;selected_camera_node=instance_node;
                ++matching_instances;
            }
        }
        if(matching_instances>1){
            error=named_camera?"Named camera selector contains multiple Camera instances":
                               "Camera visual has duplicate authored Camera instances";
            return false;
        }
        if(matching_instances==1){
            candidate_scene=std::move(graph);
            camera_node=selected_camera_node;
            camera_scope_node=named_camera?collector.selected_node:-1;
            camera_id=std::move(selected_id);
            ++matching_visuals;
        }
    }
    if(matching_visuals!=1||camera_node<0){
        error=named_camera?"Could not uniquely bind the requested named camera node and its Camera record":
                           "Could not uniquely bind Camera record to a scene node";
        return false;
    }

    const auto camera_record_it=std::find_if(camera_records.begin(),camera_records.end(),
        [&camera_id](const CameraRecord& item){return item.id==camera_id;});
    if(camera_record_it==camera_records.end()){
        error="Selected scene Camera instance has no BRES Camera record";return false;
    }
    const auto* camera_record=camera_record_it->bytes;
    const Reader reader{camera_image};
    const auto record_offset=static_cast<std::uint64_t>(camera_record-camera_image.bytes);
    const auto target_uri=field_string(reader,record_offset+24);
    if(target_uri.size()<2||target_uri[0]!='#'){
        error="Camera target is not a local scene-node reference";return false;
    }

    const auto root=unique_node(candidate_scene,assets.root_node&&*assets.root_node
        ?assets.root_node:"Root_Camera-node");
    if(named_camera&&!is_descendant_or_same(candidate_scene,
          static_cast<std::size_t>(camera_scope_node),root)){
        error="Selected source camera is outside the configured root-relative pose space";return false;
    }
    const auto target=unique_node(candidate_scene,target_uri.substr(1));
    const bool up_from_camera=assets.derive_up_from_camera;
    const auto up=up_from_camera?static_cast<std::size_t>(camera_node):
        unique_node(candidate_scene,assets.up_vector_node&&*assets.up_vector_node
            ?assets.up_vector_node:"upvector-node");
    if(static_cast<std::size_t>(camera_node)>=candidate_scene.graph.size()){
        error="Camera instance node is outside the scene graph";return false;
    }
    const auto projection=Projection{reader.real(record_offset+8),reader.real(record_offset+12),
                                     reader.real(record_offset+16),reader.real(record_offset+20)};
    if(projection.source_fov_value<=0||projection.aspect_ratio<=0||projection.near_clip<=0
       ||projection.far_clip<=projection.near_clip){
        error="Invalid authored camera projection fields";return false;
    }

    animation::Player idle;
    if(!idle.load(assets.idle_animation,assets.idle_animation_size,candidate_scene,error))return false;
    if(idle.track_count()==0||idle.skipped||idle.unbound){
        error="Camera idle animation has no complete supported transform binding";return false;
    }
    if(!scene::update_world(candidate_scene,error))return false;

    // IDA CameraLevel::CalculateDefaultTargetDistance (0x40fd18) reads the
    // absolute targetCam position and its parent's absolute position during
    // Load. This is intentionally taken from the unanimated BDAE scene, before
    // the later Level::_LoadCamera PlayAnim(idle, 0, 0) changes those nodes.
    const auto target_parent=candidate_scene.graph[target].parent;
    if(target_parent<0||static_cast<std::size_t>(target_parent)>=candidate_scene.graph.size()){
        error="Camera target node has no source distance parent";return false;
    }
    const auto& target_world=candidate_scene.graph[target].world;
    const auto& parent_world=candidate_scene.graph[static_cast<std::size_t>(target_parent)].world;
    const double dx=double(target_world[12])-parent_world[12];
    const double dy=double(target_world[13])-parent_world[13];
    const double dz=double(target_world[14])-parent_world[14];
    const double default_distance=std::sqrt(dx*dx+dy*dy+dz*dz);
    if(!std::isfinite(default_distance)||default_distance<0.0||default_distance>3.4e38){
        error="Camera target-to-parent source distance is invalid";return false;
    }

    // Store the parsed objects only after every source-backed check succeeded.
    out_scene=std::move(candidate_scene);out_idle=std::move(idle);out_projection=projection;
    out_root=root;out_camera=static_cast<std::size_t>(camera_node);out_target=target;out_up=up;
    out_up_from_camera=up_from_camera;
    out_default_target_distance=static_cast<float>(default_distance);
    return true;
}
}

bool Rig::load(const Assets& assets,std::string& error) {
    error.clear();loaded_=false;scene_={};animation_=animation::Player{};projection_={};
    root_=camera_=target_=up_vector_=0;up_from_camera_=false;default_target_distance_=0.0f;
    try{
        scene::Scene candidate_scene;animation::Player candidate_idle;Projection candidate_projection{};
        std::size_t root=0,camera=0,target=0,up=0;bool up_from_camera=false;
        float default_target_distance=0.0f;
        if(!load_impl(assets,candidate_scene,candidate_idle,candidate_projection,
                      root,camera,target,up,up_from_camera,default_target_distance,error))return false;
        scene_=std::move(candidate_scene);animation_=std::move(candidate_idle);projection_=candidate_projection;
        root_=root;camera_=camera;target_=target;up_vector_=up;
        up_from_camera_=up_from_camera;default_target_distance_=default_target_distance;
        loaded_=true;return true;
    }
    catch(const std::exception& e){error=e.what();loaded_=false;scene_={};animation_=animation::Player{};return false;}
}

bool Rig::load_animation(const std::uint8_t* bytes,std::size_t size,std::string& error) {
    error.clear();
    if(!loaded_){error="Camera scene must be loaded before selecting an animation";return false;}
    animation::Player candidate;
    if(!candidate.load(bytes,size,scene_,error))return false;
    // The native scene sampler consumes node position/quaternion/scale tracks;
    // camera BDAEs can also contain non-transform channels that this sampler
    // intentionally skips. A nonzero unbound count is different: it means a
    // selected transform names a node absent from this camera scene.
    if(candidate.track_count()==0||candidate.unbound||candidate.end<=candidate.start){
        error="Selected camera animation has no bound transform tracks (tracks="+
              std::to_string(candidate.track_count())+", skipped="+
              std::to_string(candidate.skipped)+", unbound="+
              std::to_string(candidate.unbound)+", range="+
              std::to_string(candidate.start)+".."+std::to_string(candidate.end)+")";
        return false;
    }
    animation_=std::move(candidate);
    return true;
}

bool Rig::sample(std::int32_t milliseconds,Pose* out,std::string& error) {
    error.clear();if(!loaded_||!out){error="Camera rig is not loaded or pose output is null";return false;}
    if(!animation_.sample(scene_,milliseconds,error))return false;
    if(root_>=scene_.graph.size()||camera_>=scene_.graph.size()||target_>=scene_.graph.size()
       ||up_vector_>=scene_.graph.size()){
        error="Camera rig node binding became invalid";return false;
    }
    Matrix root_inverse{};
    if(!inverse_affine(scene_.graph[root_].world,root_inverse)){
        error="Camera rig root transform is singular";return false;
    }
    Pose candidate;
    candidate.camera=scene::multiply(root_inverse,scene_.graph[camera_].world);
    candidate.camera_world=scene_.graph[camera_].world;
    candidate.target=scene::multiply(root_inverse,scene_.graph[target_].world);
    if(up_from_camera_){
        candidate.up_vector=candidate.camera;
        candidate.up_vector[12]+=candidate.camera[4];
        candidate.up_vector[13]+=candidate.camera[5];
        candidate.up_vector[14]+=candidate.camera[6];
    }else{
        candidate.up_vector=scene::multiply(root_inverse,scene_.graph[up_vector_].world);
    }
    const auto target_parent=scene_.graph[target_].parent;
    Matrix target_parent_world{};
    if(target_parent>=0){
        if(static_cast<std::size_t>(target_parent)>=scene_.graph.size()){
            error="Camera target parent binding became invalid";return false;
        }
        target_parent_world=scene::multiply(
            root_inverse,scene_.graph[static_cast<std::size_t>(target_parent)].world);
    }else{
        // A parentless target receives positions in scene space. Express its
        // local Z translation in the same Root_Camera-relative coordinates.
        target_parent_world=root_inverse;
    }
    candidate.target_parent_z_axis={target_parent_world[8],target_parent_world[9],
                                    target_parent_world[10]};
    for(float value:candidate.camera)if(!std::isfinite(value)){error="Nonfinite camera pose";return false;}
    for(float value:candidate.target)if(!std::isfinite(value)){error="Nonfinite target pose";return false;}
    for(float value:candidate.up_vector)if(!std::isfinite(value)){error="Nonfinite up-vector pose";return false;}
    for(float value:candidate.target_parent_z_axis)
        if(!std::isfinite(value)){error="Nonfinite target parent basis";return false;}
    *out=candidate;return true;
}

bool Playback::start(const Rig& rig,std::string& error) {
    error.clear();started_=false;source_clock_ms_=0;timeline_={};
    if(rig.track_count()==0||rig.animation_end()<=rig.animation_start()){
        error="Camera timeline has no supported animation range";return false;
    }
    // IDA: Level::_LoadCamera calls CameraLevel::PlayAnim(idle, 0, 0).
    // AnimSetController::PlayClip forwards loop=false and sets scale=1.
    if(dh2_timeline_clip(&timeline_,0,rig.animation_start(),rig.animation_end())||
       dh2_timeline_loop(&timeline_,0)||dh2_timeline_scale(&timeline_,1.0f)){
        error="Camera source timeline rejected its clip range";timeline_={};return false;
    }
    // Prime the source timeline at the PlayClip start timestamp. This makes
    // the next game-frame delta advance the clip immediately, as the native
    // timeline does from its play-time baseline.
    if(dh2_timeline_update(&timeline_,signed_time(source_clock_ms_),nullptr)){
        error="Camera source timeline rejected its initial timestamp";timeline_={};return false;
    }
    started_=true;return true;
}

bool Playback::advance(Rig& rig,std::uint32_t dt_ms,Pose* out,std::string& error) {
    error.clear();
    if(!started_||!out){error="Camera playback is not started or pose output is null";return false;}
    source_clock_ms_+=dt_ms;
    if(dh2_timeline_update(&timeline_,signed_time(source_clock_ms_),nullptr)){
        error="Camera source timeline update rejected its timestamp";return false;
    }
    return rig.sample(timeline_.current_ms,out,error);
}

} // namespace dh2::player_camera_rig_v1
