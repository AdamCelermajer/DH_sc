#pragma once
#include "../navigation.hpp"
#include <cstring>
#include <vector>
namespace dh2::navigation {
// Diagnostic format for original-verified logical snapshots. Allocation
// addresses and unused capacity do not participate in the state identity.
inline std::vector<unsigned char> link_state(const Graph& g,const FloorGraph* floors,unsigned count,const LinkWorkspace& w){
 std::vector<unsigned char> result;
 const auto append=[&](const void* p,std::size_t n){if(n){const auto* bytes=static_cast<const unsigned char*>(p);result.insert(result.end(),bytes,bytes+n);}};
 const unsigned prefix[]{g.node_count,g.edge_count,g.invalid_count,g.validation_count,count,w.link_count};
 append(prefix,sizeof(prefix));append(&g.root,16);append(g.nodes,g.node_count*sizeof(Node));append(g.edges,g.edge_count*sizeof(Edge));
 append(g.invalid,g.invalid_count*sizeof(InvalidNode));append(g.validation,g.validation_count*4);append(w.validation_floors,g.validation_count*4);
 append(floors,count*sizeof(FloorGraph));append(w.links,w.link_count*sizeof(FloorLink));return result;
}
inline std::uint64_t link_state_digest(const std::vector<unsigned char>& bytes){
 std::uint64_t value=0xcbf29ce484222325ull;
 for(std::size_t i=0;i<bytes.size();i+=4){unsigned word;std::memcpy(&word,bytes.data()+i,4);if((word&0x7f800000)==0x7f800000&&(word&0x7fffff))word=0x7fc00000;
  for(unsigned j=0;j<4;++j){value^=(word>>(8*j))&255;value*=0x100000001b3ull;}}
 return value;
}
}
