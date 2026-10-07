#include "item_color_lookup_v2.hpp"
namespace dh2::character {
bool item_color_lookup_v2(const data::ItemInstanceV1& item,const ItemColorLookupServicesV2& s,std::uint32_t& color,std::string& e){
 const auto count=item.powers.size();std::int32_t font=2;
 if(count<=4){
  const auto* design=s.design.design();if(!design||!design->lookup){e="Required actual ItemPowerColor GameDesign";return false;}
  static const char* keys[]={"zero","one","two","three","four"};
  if(design->lookup(design->context,0,"ItemPowerColor",keys[count],&font)){e="Actual ItemPowerColor lookup failed";return false;}
 }
 if(!s.font_text_color){e="Required actual fonts palette textcolor";return false;}
 return s.font_text_color(s.context,font,color,e);
}
}
