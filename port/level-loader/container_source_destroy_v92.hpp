#pragma once
#include <canonical_openable_graph_v21.hpp>
#include <canonical_colbox_v71.hpp>
#include <game_object_source_destroy_v72.hpp>
namespace dh2::loader {
// Genuine positive NetStructBase tree370fd0 remains a Main primitive; the
// actual C1-zero count11c branch is source absence, not fake network teardown.
using ContainerNetworkDestroyV92=std::function<bool(world::ContainerNetStructV1&,std::uint32_t,std::string&)>;
inline bool openable_source_destroy_v92(world::CanonicalOpenableGraphV21& graph,
 const world::ColBoxServicesV71& gameobject,ContainerNetworkDestroyV92 destroy_network,std::string& e){
 auto& actual=graph.receiver();std::string{}.swap(actual.fields().key_name); //OpenableD1 3a1c24
 for(auto index:{1u,0u}){
  auto& network=actual.network(index);auto count=network.raw(0x11c);auto root=network.raw(0x110);auto left=network.pointer(0x114);auto right=network.pointer(0x118);auto header=network.byte(0x10c);
  if(!count||!root||!left||!right||!header){e="Required SAME Container NetStruct source fields";return false;}
  if(*count){if(!destroy_network||!destroy_network(network,index?0x548u:0x3a0u,e)){if(e.empty())e="Required real NetStruct tree370fd0 destruction";return false;}*count=0;*root=0;*left=*right=reinterpret_cast<std::uintptr_t>(header);}
 }
 std::string{}.swap(actual.fields().data_desc); //ContainerD2 3a067c..6a4
 std::shared_ptr<void> physical_owner;
 return world::game_object_source_destroy_v72(actual.base(),gameobject,physical_owner,e);
}
}
