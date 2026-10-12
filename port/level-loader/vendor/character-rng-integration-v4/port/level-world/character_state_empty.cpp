#include "character_state_empty.hpp"
namespace {
struct Method {std::int32_t state;std::uint32_t operation,source;bool empty;};
#include "reference/character-state-methods/native-methods.inc"
}
extern "C" int dh2_character_state_empty_body(std::int32_t state,std::uint32_t operation,std::uint32_t source){
 if(operation>3)return -1;
 for(const auto& m:methods)if(m.state==state&&m.operation==operation)return m.source==source?(m.empty?1:0):-1;
 return -1;
}
