#pragma once
#include "character_game_design.hpp"
namespace dh2::character {
// Concrete original RecalcProperties(bool), using retained source class rows
// and the SAME live view. Mutable base owner is explicit, never const-cast.
class CharacterMenuRecalcOwnerV1 {
 CharacterGameDesign::Borrow design_;
 std::shared_ptr<data::PropertyState> properties_;
 data::PropertyView& view_;
public:
 CharacterMenuRecalcOwnerV1(CharacterGameDesign::Borrow&&,
  std::shared_ptr<data::PropertyState>,data::PropertyView&);
 bool recalculate(data::PropertyView&,std::uint32_t load_class,std::string&);
};
}
