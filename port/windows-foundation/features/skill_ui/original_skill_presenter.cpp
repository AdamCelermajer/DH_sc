#include "original_skill_art.hpp"
#include <algorithm>
#include <cmath>
namespace dh::foundation::skill_ui {
namespace {
bool icon(unsigned cf,int pos,IconKind kind,const std::string& label,HudGeometry& out,std::string& error){
 error.clear();if(cf>2){error="Skill UI: source class frame outside Warrior/Rogue/Mage";return false;}
 const auto& states=original_skill_icon_states();const auto name=label.empty()?"undefined":label;
 auto state=std::find_if(states.begin(),states.end(),[&](const auto& s){return s.label==name;});if(state==states.end()){error="Skill UI: original SkillIcon label absent: "+name;return false;}
 auto next=out;bool found=false;
 for(const auto& p:original_skill_icon_placements())if(p.class_frame==cf&&p.position==pos&&(p.kind==kind||(kind==IconKind::tree&&p.kind==IconKind::drag))){found=true;
  next.batches.erase(std::remove_if(next.batches.begin(),next.batches.end(),[&](const auto& b){return b.role==p.path||b.role.compare(0,p.path.size()+1,p.path+"/")==0;}),next.batches.end());
  for(auto batch:state->batches){batch.role=p.path+"/"+state->label;for(auto& v:batch.triangles){const float x=v.x,y=v.y;v.x=p.matrix[0]*x+p.matrix[2]*y+p.matrix[4];v.y=p.matrix[1]*x+p.matrix[3]*y+p.matrix[5];}next.batches.push_back(std::move(batch));}
 }
 if(!found){error="Skill UI: icon position outside original authored placements";return false;}out=std::move(next);return true;
}
bool inside(float x,float y,const HudGeometryVertex& a,const HudGeometryVertex& b,const HudGeometryVertex& c){auto cross=[](float x1,float y1,float x2,float y2){return x1*y2-y1*x2;};float area=cross(b.x-a.x,b.y-a.y,c.x-a.x,c.y-a.y);if(std::abs(area)<0.00001f)return false;float u=cross(b.x-a.x,b.y-a.y,x-a.x,y-a.y),v=cross(c.x-b.x,c.y-b.y,x-b.x,y-b.y),w=cross(a.x-c.x,a.y-c.y,x-c.x,y-c.y);return (u>=0&&v>=0&&w>=0)||(u<=0&&v<=0&&w<=0);}
}
bool append_original_skill_icon(unsigned cf,int pos,const std::string& label,HudGeometry& out,std::string& e){return icon(cf,pos,IconKind::tree,label,out,e);}
bool append_original_skill_slot_icon(unsigned cf,int slot,const std::string& label,HudGeometry& out,std::string& e){return icon(cf,slot,IconKind::slot,label,out,e);}
std::optional<HitZone> original_skill_hit(unsigned cf,float x,float y){if(cf>2||!std::isfinite(x)||!std::isfinite(y))return {};
 // Source topmost placement wins. Authored drop buttons/train overlap none of
 // the tree; reverse order handles overlaid draggable/static skill contours.
 const auto& zones=original_skill_hit_zones(cf);for(auto it=zones.rbegin();it!=zones.rend();++it){for(std::size_t i=0;i+2<it->triangles.size();i+=3)if(inside(x,y,it->triangles[i],it->triangles[i+1],it->triangles[i+2]))return *it;}return {};
}
}
