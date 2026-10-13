#include "../skill_projectile_delivery_v1.hpp"

#include <cassert>
#include <string>
#include <vector>

using namespace dh2::skill_projectile_delivery_v1;

struct Trace {
    std::vector<int> order;
    Launch launch{};
    std::uintptr_t target{};
    int fail_position{};
};
static int spawn_cb(void* raw,const Launch& launch,std::uintptr_t* projectile) {
    auto& t=*static_cast<Trace*>(raw);t.order.push_back(1);t.launch=launch;
    *projectile=0x9001;return 0;
}
static int position_cb(void* raw,std::uintptr_t projectile,const Point3f& p) {
    auto& t=*static_cast<Trace*>(raw);assert(projectile==0x9001);
    assert(p.x==1&&p.y==2&&p.z==3);t.order.push_back(2);return t.fail_position;
}
static int destination_cb(void* raw,std::uintptr_t projectile,const Point3f& p) {
    auto& t=*static_cast<Trace*>(raw);assert(projectile==0x9001);
    assert(p.x==4&&p.y==5&&p.z==6);t.order.push_back(3);return 0;
}
static int target_cb(void* raw,std::uintptr_t projectile,std::uintptr_t target) {
    auto& t=*static_cast<Trace*>(raw);t.order.push_back(4);
    assert(projectile==0x9001);t.target=target;return 0;
}

int main() {
    Trace trace{};
    const Services services{&trace,spawn_cb,position_cb,destination_cb,target_cb};
    Result result{};std::string error;
    Value basic[]{{ValueType::number,2.9f},{ValueType::number,7.0f}};
    assert(spawn(basic,2,0x100,0x200,&services,&result,error)==Status::complete);
    assert(trace.order==std::vector<int>{1}&&trace.launch.skill_index==2&&
        trace.launch.projectile_id==7&&trace.launch.info_kind==
        dh2::projectile_lifecycle_runtime_v1::SetInfoKind::flag_bool&&
        !trace.launch.flag&&result.projectile==0x9001&&result.returned==1);

    trace={};
    Value extended[]{{ValueType::number,1.0f},{ValueType::number,12.0f},
        {ValueType::userdata,0,0,0x300,{1,2,3}},
        {ValueType::userdata,0,0,0x400,{4,5,6}}};
    assert(spawn(extended,4,0x100,0x201,&services,&result,error)==Status::complete);
    assert(trace.order==std::vector<int>({1,2,3})&&result.returned==1&&
        result.position_written&&result.destination_written&&
        trace.launch.has_position&&trace.launch.has_destination&&
        trace.launch.skill_index==1&&trace.launch.projectile_id==12);

    trace={};
    Value flagged[]{{ValueType::number,0},{ValueType::number,2},
        {ValueType::boolean,0,1}};
    assert(spawn(flagged,3,0x100,0x202,&services,&result,error)==Status::complete);
    assert(trace.launch.info_kind==
        dh2::projectile_lifecycle_runtime_v1::SetInfoKind::flag_bool&&
        trace.launch.flag&&result.returned==1);

    trace={};trace.fail_position=1;
    assert(spawn(extended,4,0x100,0x203,&services,&result,error)==Status::service_failed);
    assert(trace.order==std::vector<int>({1,2})&&!result.destination_written);

    trace={};
    Value target_args[]{{ValueType::pointer,0,0,0x9001},
                        {ValueType::userdata,0,0,0x777}};
    assert(set_target(target_args,2,&services,&result,error)==Status::complete);
    assert(trace.order==std::vector<int>{4}&&trace.target==0x777&&
        result.target_written&&result.projectile==0x9001);

    trace={};
    target_args[0]={ValueType::number,1};
    assert(set_target(target_args,2,&services,&result,error)==Status::complete);
    assert(trace.order.empty()&&!result.target_written);

    trace={};
    target_args[0]={ValueType::pointer,0,0,0x9001};
    assert(set_target(target_args,1,&services,&result,error)==Status::complete);
    assert(trace.order.empty()&&!result.target_written);
}
