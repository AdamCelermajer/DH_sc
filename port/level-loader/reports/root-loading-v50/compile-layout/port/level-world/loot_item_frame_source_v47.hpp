#pragma once
#include "world_item_frame_v5.hpp"
namespace dh2::character {
// Retained callback/policy bridge for the default source Item family. This owns
// no pose/body/floor/camera or auxiliary fields. Non-NULL auxiliary/explicit
// boundary-validation branches require their positive source owner.
class LootItemFrameSourceBridgeV47 {
 WorldItemGraphV3& graph_;
 actor::RuntimePolicy policy_{};
 subobjects::Services preceding_{};
 std::string error_;
 static std::uint32_t world(void*,std::uint32_t,float*);
public:
 explicit LootItemFrameSourceBridgeV47(WorldItemGraphV3& graph):graph_(graph){}
 bool bind(WorldItemFrameServicesV5 actual,WorldItemFrameServicesV5&,std::string&);
 const std::string& error()const noexcept{return error_;}
};
}

