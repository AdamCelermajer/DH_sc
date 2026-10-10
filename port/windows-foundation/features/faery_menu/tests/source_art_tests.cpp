#include "../faery_menu.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <set>
using namespace dh::foundation;using namespace dh::foundation::faery_menu;
int main(){
 const auto& art=original_faery_art();
 if(art.batches.size()!=2||art.text_fields.size()!=4||art.slots.size()!=5)return 1;
 std::set<std::string> paths;unsigned element_icons=0;
 for(const auto& b:art.batches){
  if(b.triangles.empty()||b.triangles.size()%3)return 2;
  if(b.role.find("FaeryElementImage")!=std::string::npos)++element_icons;
  for(const auto& v:b.triangles)if(!std::isfinite(v.x)||!std::isfinite(v.y)||!std::isfinite(v.u)||!std::isfinite(v.v))return 3;
 }
 if(element_icons!=0)return 4;
 const std::array<unsigned,5> image_shapes{{538,540,541,542,539}};
 for(unsigned slot=0;slot<image_shapes.size();++slot){const auto& image=original_faery_image(slot);if(image.empty())return 11;bool found=false;for(const auto& batch:image)if(batch.shape_id==image_shapes[slot]&&!batch.triangles.empty())found=true;if(!found)return 12;}
 const std::array<unsigned,5> button_shapes{{529,533,531,532,530}};
 for(unsigned slot=0;slot<button_shapes.size();++slot){
  for(auto state:{ButtonVisual::idle,ButtonVisual::focused,ButtonVisual::locked}){
   const auto& button=original_faery_button(slot,state);bool icon=false;
   for(const auto& batch:button){if(batch.shape_id==button_shapes[slot]&&!batch.triangles.empty())icon=true;for(const auto& v:batch.triangles)if(!std::isfinite(v.x)||!std::isfinite(v.y)||!std::isfinite(v.u)||!std::isfinite(v.v))return 15;}
   if(!icon)return 16;
   const auto& solids=original_faery_button_solids(slot,state);
   if(state==ButtonVisual::locked){if(solids.size()!=1||solids[0].geometry.shape_id!=535||solids[0].geometry.triangles.size()<600||solids[0].after_bitmap_role.find("FaeryElementImage")==std::string::npos||std::abs(solids[0].rgba[3]-0.7019608f)>1e-5f)return 17;}
   else if(!solids.empty())return 18;
  }
 }
 for(const auto& f:art.text_fields){paths.insert(f.path);if(f.font_id!=103&&f.font_id!=287)return 5;if(f.source_height<=0||f.bounds[0]>=f.bounds[1]||f.bounds[2]>=f.bounds[3])return 6;}
 if(paths.count("menu_FaerySheet/faery_desc/text")!=1||paths.count("menu_FaerySheet/faery_spell/text")!=1||paths.count("menu_FaerySheet/FaeryNameText/text")!=1||paths.count("menu_FaerySheet/menu_title/txt_title")!=1)return 7;
 for(const auto& hit:art.slots)if(hit.empty()||hit.size()%3)return 8;
 for(unsigned slot=0;slot<art.slots.size();++slot){const auto& hit=art.slots[slot];float x=0,y=0;for(const auto& v:hit){x+=v.x;y+=v.y;}x/=hit.size();y/=hit.size();if(slot_at(x,y)!=static_cast<int>(slot))return 13;}
 if(slot_at(479,319)!=-1||slot_at(std::numeric_limits<float>::quiet_NaN(),0)!=-1)return 14;
 const auto& tab=original_faery_tab_hit();if(tab.size()!=6)return 9;
 float minx=10000,maxx=-10000,miny=10000,maxy=-10000;for(const auto& v:tab){minx=std::min(minx,v.x);maxx=std::max(maxx,v.x);miny=std::min(miny,v.y);maxy=std::max(maxy,v.y);}
 if(std::abs(minx-317.15f)>0.02f||std::abs(maxx-380.55f)>0.02f||std::abs(miny+13.15f)>0.02f||std::abs(maxy-37.8745f)>0.02f)return 10;
 std::cout<<"PASS original faery art batches="<<art.batches.size()<<" fields="<<art.text_fields.size()<<" authored element icons="<<element_icons<<" five actual img_Faery named frames="<<image_shapes.size()<<" five real hit contours="<<art.slots.size()<<"\n";
}
