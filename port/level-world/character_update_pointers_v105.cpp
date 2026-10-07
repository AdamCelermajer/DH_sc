#include "character_update_pointers_v105.hpp"
namespace dh2::character {
bool character_update_pointers_v105(TargetState48& target,CharacterAiPointerFieldsV105& ai,
 std::uintptr_t& ooi,std::uintptr_t& killer,const DisabledCharacterBorrowV105& borrow,std::string& e){
 auto clear=[&](std::uintptr_t& cell){
  if(!cell)return true;
  const std::uint8_t* disabled{};
  if(!borrow||!borrow(cell,disabled,e)||!disabled){if(e.empty())e="Required actual GameObject.disabled81 in UpdateAIPointers";return false;}
  if(*disabled)cell=0;
  return true;
 };
 if(!clear(target.target)||!clear(ai.master50)||!clear(ai.auxiliary58)||!clear(target.last_target))return false;
 for(auto& node:ai.observers5c)if(!clear(node.second))return false;
 return clear(ooi)&&clear(killer);
}
}
