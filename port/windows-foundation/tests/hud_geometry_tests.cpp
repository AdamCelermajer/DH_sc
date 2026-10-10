#include "../hud_geometry.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool condition,const char* why){if(!condition)throw std::runtime_error(why);}
bool near(float a,float b){return std::abs(a-b)<.001f;}
const HudGeometryBatch& portrait(const HudGeometry& geometry){for(const auto& batch:geometry.batches)if(batch.shape_id>=38&&batch.shape_id<=40)return batch;throw std::runtime_error("Portrait missing");}
std::array<float,2> point_at_uv(const HudGeometryBatch& batch,float u,float v){
 for(std::size_t i=0;i+2<batch.triangles.size();i+=3){
  const auto& a=batch.triangles[i];const auto& b=batch.triangles[i+1];const auto& c=batch.triangles[i+2];
  const float det=(b.u-a.u)*(c.v-a.v)-(c.u-a.u)*(b.v-a.v);if(std::abs(det)<1e-10f)continue;
  const float p=((u-a.u)*(c.v-a.v)-(c.u-a.u)*(v-a.v))/det;
  const float q=((b.u-a.u)*(v-a.v)-(u-a.u)*(b.v-a.v))/det;
  return {a.x+p*(b.x-a.x)+q*(c.x-a.x),a.y+p*(b.y-a.y)+q*(c.y-a.y)};
 }throw std::runtime_error("Degenerate portrait atlas geometry");
}
}
int main(){try{
 std::string error;unsigned cases=0;
 const float centres[3][2]={{700,687},{630.5f,688.5f},{559,689}};
 const float scales[4][2]={{.7880091466f,.7881895269f},{.7880933241f,.7880933241f},{.7880933241f,.7880933241f},{.7880091466f,.7880933241f}};
 for(unsigned style=0;style<4;++style){
  std::array<float,4> aperture{};check(original_hud_portrait_bounds(style,aperture,error),"Aperture getter failed");
  check(aperture[0]<aperture[1]&&aperture[2]<aperture[3],"Empty source aperture");
  HudGeometry baseline;check(compose_original_hud(style,99,99,0,baseline,error),"Baseline compose failed");
  for(unsigned frame=0;frame<3;++frame){
   HudGeometry geometry;check(compose_original_hud(style,99,99,frame,geometry,error),"Class compose failed");
   check(geometry.batches.size()==7,"HUD art batch count changed");
   const auto& face=portrait(geometry);check(face.shape_id==38+frame,"Wrong original class portrait");
   const auto centre=point_at_uv(face,centres[frame][0]/1024.f,centres[frame][1]/1024.f);
   check(near(centre[0],(aperture[0]+aperture[1])*.5f)&&near(centre[1],(aperture[2]+aperture[3])*.5f),"Actual portrait paint centre not aligned to actual ring aperture");
   const HudShapeGeometry* source=nullptr;for(const auto& shape:original_hud_shapes())if(shape.shape_id==face.shape_id)source=&shape;
   check(source&&source->triangles.size()==face.triangles.size(),"Portrait original contour topology changed");
   for(std::size_t i=0;i<face.triangles.size();++i){
    const auto& a=source->triangles[i];const auto& b=face.triangles[i];
    check(a.u==b.u&&a.v==b.v,"Original portrait atlas UV changed");
    check(near(b.x-face.triangles[0].x,(a.x-source->triangles[0].x)*scales[style][0]/20.f)&&near(b.y-face.triangles[0].y,(a.y-source->triangles[0].y)*scales[style][1]/20.f),"Portrait scale or contour changed");
   }
   for(std::size_t i=0;i<geometry.batches.size();++i){
    const auto& b=geometry.batches[i];if(b.shape_id>=38&&b.shape_id<=40)continue;
    const auto& old=baseline.batches[i];check(b.role==old.role&&b.shape_id==old.shape_id&&b.triangles.size()==old.triangles.size(),"Nonportrait HUD batch changed by class");
    for(std::size_t j=0;j<b.triangles.size();++j){const auto& a=b.triangles[j];const auto& c=old.triangles[j];check(a.x==c.x&&a.y==c.y&&a.u==c.u&&a.v==c.v,"Nonportrait HUD coordinates changed by class");}
   }
   for(float scale:{1.f,1.125f,2.25f,2.5f})check(near(centre[0]*scale,(aperture[0]+aperture[1])*.5f*scale)&&near(centre[1]*scale,(aperture[2]+aperture[3])*.5f*scale),"Height-fit Android/desktop scaling changed relative alignment");
   HudGeometry repeat;check(compose_original_hud(style,99,99,frame,repeat,error),"Repeat compose failed");const auto& again=portrait(repeat);
   check(again.triangles.front().x==face.triangles.front().x&&again.triangles.front().y==face.triangles.front().y,"Alignment correction accumulated across calls");
   ++cases;
  }
 }
 std::array<float,4> bounds{1,2,3,4},before=bounds;check(!original_hud_portrait_bounds(4,bounds,error)&&bounds==before,"Invalid bounds query changed output");
 HudGeometry preserved;check(compose_original_hud(0,99,99,0,preserved,error),"Final baseline compose failed");const auto prior=portrait(preserved).triangles.front();
 check(!compose_original_hud(0,99,99,3,preserved,error)&&portrait(preserved).triangles.front().x==prior.x,"Invalid portrait compose changed output");
 std::cout<<cases<<" original layout/class portraits aligned to ring155; source contour, UV, scale, nonportrait batches, height-fit mapping and transactional guards passed\n";
 return 0;
}catch(const std::exception& exception){std::cerr<<"HUD geometry test failure: "<<exception.what()<<'\n';return 1;}}
