#include "character_menu_recalc_owner_v1.hpp"
#include <stdexcept>
namespace dh2::character {
CharacterMenuRecalcOwnerV1::CharacterMenuRecalcOwnerV1(CharacterGameDesign::Borrow&& design,std::shared_ptr<data::PropertyState> properties,data::PropertyView& view):design_(std::move(design)),properties_(std::move(properties)),view_(view){
 if(!design_||!properties_||view_.base!=properties_->base.data()||view_.saved!=properties_->saved.data()||view_.gear!=properties_->gear.data()||view_.resolved!=properties_->resolved.data())throw std::invalid_argument("Menu Recalc requires actual retained property sheets");
}
bool CharacterMenuRecalcOwnerV1::recalculate(data::PropertyView& view,std::uint32_t load_class,std::string& error){
 if(&view!=&view_||dh2_property_validate(&view)){error="Original Recalc requires SAME valid live PropertyView";return false;}
 if(load_class){
  const auto* rows=design_.class_rows();
  if(!rows||rows->empty()||dh2_class_recalc_base(rows->data(),std::uint32_t(rows->size()),properties_->base.data(),&view)){error="Original uncached class/base/property recalculation failed";return false;}
 }else for(unsigned p=0;p<224;++p){std::int32_t value;if(dh2_property_resolve(&view,p,&value)){error="Original property recalculation failed at "+std::to_string(p);return false;}}
 error.clear();return true;
}
}
