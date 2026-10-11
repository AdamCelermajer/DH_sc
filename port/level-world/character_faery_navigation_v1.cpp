#include "character_faery_controller_v1.hpp"

#include "character_runtime_factory_v1.hpp"
#include "navigation_producers.hpp"

#include <algorithm>
#include <cmath>

namespace dh2::character_faery_controller_v1 {
namespace {

bool valid_record(const factory::Record& record) noexcept {
    const auto identity=record.character.identity;
    return identity&&record.constructor.identity==identity&&
        record.game_object.identity==identity&&record.object_registered&&
        record.character_listed;
}

} // namespace

Status initialize_navigation(factory::Record& record,
    const navigation::CollisionWorld& geometry,
    navigation::ObstacleRegistry& registry,const float position[3],
    float physical_radius,const float bounds[4],std::string& error) {
    error.clear();
    auto& owner=record.faery_controller;
    if(!valid_record(record)||!owner.character||
       owner.character!=record.character.identity||!owner.controller||
       !owner.controllable||!position||!bounds||owner.navigation_ready||
       !std::isfinite(physical_radius)||physical_radius<0.f) {
        error="Faery PFObject requires its registered controller Record and source physical bounds";
        return Status::invalid_record;
    }
    navigation::NavigationObject next{};
    if(dh2_nav_object_defaults(&next)!=0) {
        error="Faery PFObject source defaults were rejected";
        return Status::command_failed;
    }
    const navigation::ObjectInitRequest init{&geometry,&next,
        record.character.identity,{position[0],position[1],position[2]},
        physical_radius*100.f,0,0};
    if(dh2_nav_init_object(&init)!=0) {
        error="Faery PFObject source floor initialization failed";
        return Status::command_failed;
    }
    const navigation::ProducerFields fields{
        navigation::ProducerClass::character,1,physical_radius,0,
        {bounds[0],bounds[1]},{bounds[2],bounds[3]}};
    const navigation::ProducerRequest producer{&geometry,&registry,&next,
        record.character.identity,&fields};
    if(dh2_nav_update_game_object(&producer)!=0) {
        error="Faery GameObject::UpdatePFObject obstacle registration failed";
        return Status::command_failed;
    }
    owner.navigation=next;
    std::copy_n(next.motion.position,3,owner.destination);
    owner.navigation_ready=true;
    return Status::complete;
}

Status synchronize_placement(factory::Record& record,
    const navigation::CollisionWorld& geometry,
    navigation::ObstacleRegistry& registry,const float position[3],
    std::string& error) {
    error.clear();
    auto& owner=record.faery_controller;
    if(!position||!valid_record(record)||!owner.navigation_ready||
       !owner.character||owner.character!=record.character.identity||
       owner.navigation.user!=record.character.identity||
       !std::isfinite(position[0])||!std::isfinite(position[1])||
       !std::isfinite(position[2])) {
        error="Faery placement requires its canonical registered PFObject owner";
        return Status::invalid_record;
    }
    auto next=owner.navigation;
    const navigation::ObjectInitRequest init{&geometry,&next,
        record.character.identity,{position[0],position[1],position[2]},
        next.radius,0,0};
    if(dh2_nav_init_object(&init)!=0) {
        error="Faery direct placement could not resolve the shared PFObject floor";
        return Status::command_failed;
    }
    if(dh2_nav_change_obstacle_parent(&registry,&owner.navigation,
          record.character.identity,next.motion.floor)!=0) {
        error="Faery direct placement could not move its PFObject obstacle parent";
        return Status::command_failed;
    }
    owner.navigation=next;
    std::copy_n(next.motion.position,3,owner.destination);
    return Status::complete;
}

Status validate_warp(factory::Record& record,
    const navigation::CollisionWorld& geometry,
    navigation::ObstacleRegistry& registry,
    const navigation::MotionPolicy& policy,const float point[3],
    float accepted[3],navigation::PositionResult* output,
    std::string& error) {
    error.clear();
    if(output)*output={};
    auto& owner=record.faery_controller;
    if(!output||!accepted||!point||!valid_record(record)||
       !owner.navigation_ready||!owner.character||
       owner.character!=record.character.identity||
       owner.navigation.user!=record.character.identity) {
        error="Faery WarpTo requires the canonical registered PFObject owner";
        return Status::invalid_record;
    }
    float candidate[3]{point[0],point[1],point[2]};
    navigation::PositionResult result{};
    const navigation::ObjectPositionRequest request{&geometry,&registry,
        &owner.navigation,record.character.identity,candidate,&policy};
    if(dh2_nav_validate_object_position(&result,&request)!=0||!result.valid) {
        error="Faery WarpTo source PFWorld::ValidatePosition rejected the destination";
        return Status::command_failed;
    }
    std::copy_n(candidate,3,accepted);
    *output=result;
    std::copy_n(candidate,3,owner.destination);
    return Status::complete;
}

Status retire_navigation(factory::Record& record,
    const navigation::CollisionWorld& geometry,
    navigation::ObstacleRegistry& registry,std::string& error) {
    error.clear();
    auto& owner=record.faery_controller;
    if(!owner.navigation_ready)return Status::complete;
    if(!valid_record(record)||owner.character!=record.character.identity||
       owner.navigation.user!=record.character.identity) {
        error="Faery PFObject retirement lost its canonical Character identity";
        return Status::invalid_record;
    }
    const navigation::ObstacleInitRequest remove{&geometry,&registry,
        &owner.navigation,record.character.identity,0.f,0.f,0,0};
    if(dh2_nav_init_obstacle(&remove)!=0) {
        error="Faery PFObject obstacle retirement failed in the shared Level registry";
        return Status::command_failed;
    }
    owner.navigation={};
    owner.destination[0]=owner.destination[1]=owner.destination[2]=0.f;
    owner.navigation_ready=false;
    return Status::complete;
}

} // namespace dh2::character_faery_controller_v1
