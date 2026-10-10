#include "original_actor_target_position.hpp"

namespace dh::foundation {
void original_target_position_ctor_prefix(std::optional<std::uintptr_t>& node,
                                         std::optional<OriginalTargetPosition>& cache) {
    node=0;cache=OriginalTargetPosition{0,0,0};
}
bool original_target_position_named_node_prefix(std::optional<std::uintptr_t>& node,
    std::uintptr_t root,const OriginalNamedNodeLookup& lookup,std::string& error) {
    if(!root){error.clear();return true;}
    if(!lookup){error="reached target_node lookup requires actual scene hierarchy service";return false;}
    std::uintptr_t found=0;
    if(!lookup(root,"target_node",found,error))return false;
    node=found;error.clear();return true;
}
bool original_update_target_position(const std::optional<std::uintptr_t>& node,
    std::optional<OriginalTargetPosition>& cache,const OriginalAbsoluteNodePosition& absolute,
    OriginalTargetPositionUpdateResult& result,std::string& error) {
    result={};
    if(!node){error="actual own GameObject target_node180 is unbound";return false;}
    const auto retained_node=*node;
    if(!retained_node){error.clear();return true;}
    if(!absolute){error="reached non-null target_node requires absolute scene position service";return false;}
    OriginalTargetPosition snapshot{};
    result.node_queried=true;
    if(!absolute(retained_node,snapshot,error))return false;
    // Original snapshots XYZ then stores Y,Z,X. Optional contains the actual
    // caller-owned source cache; a successful producer makes it known.
    if(!cache)cache.emplace();
    (*cache)[1]=snapshot[1];(*cache)[2]=snapshot[2];(*cache)[0]=snapshot[0];
    result.cache_written=true;error.clear();return true;
}
bool original_get_target_position(const std::optional<std::uintptr_t>& node,
    const std::optional<OriginalTargetPosition>& cache,const OriginalTargetPosition& position,
    const std::function<bool(std::uint8_t&,std::string&)>& enabled,
    const OriginalTargetPosition*& selected,std::string& error) {
    if(!node){error="actual own GameObject target_node180 is unbound";return false;}
    if(!*node){selected=&position;error.clear();return true;}
    if(!enabled){error="reached target position selector requires actual owner enabled80";return false;}
    std::uint8_t value=0;
    if(!enabled(value,error))return false;
    if(!value){selected=&position;error.clear();return true;}
    if(!cache){error="selected source target cache184 has no actual producer";return false;}
    selected=&*cache;error.clear();return true;
}
}
