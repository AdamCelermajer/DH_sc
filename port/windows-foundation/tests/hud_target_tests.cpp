#include "../hud_geometry.hpp"
#include <cassert>
#include <stdexcept>
#undef assert
#define assert(condition) do { if (!(condition)) throw std::runtime_error(#condition); } while(false)
#include <cmath>
#include <iostream>
using namespace dh::foundation;
int main(){HudTargetGeometry g;std::string e;for(unsigned frame=0;frame<100;++frame){assert(compose_original_target_hud(frame,g,e));assert(g.art.batches.size()==4);assert(g.text_fields.size()==2);assert(g.bounds[0]<g.bounds[1]&&g.bounds[2]<g.bounds[3]);for(const auto&b:g.art.batches){assert(b.triangles.size()%3==0);for(const auto&v:b.triangles){assert(std::isfinite(v.x)&&std::isfinite(v.y)&&v.u>=0&&v.u<=1&&v.v>=0&&v.v<=1);assert(v.x>=g.bounds[0]-.01f&&v.x<=g.bounds[1]+.01f&&v.y>=g.bounds[2]-.01f&&v.y<=g.bounds[3]+.01f);}}for(const auto&t:g.text_fields){assert(t.source_font==7&&t.source_height==12&&t.align==2&&t.leading==2);}}for(const auto&field:g.text_fields){std::array<float,2> baseline{};assert(layout_original_target_text(field,30,baseline,e));const bool name=field.role=="enemy_name";assert(std::abs(baseline[0]-(name?229.85f:193.45f))<.0001f);assert(std::abs(baseline[1]-(name?3.37578125f:24.32578125f))<.0001f);}auto prior=g.bounds;assert(!compose_original_target_hud(100,g,e));assert(g.bounds==prior);std::cout<<"100 original targetHPframes validated;4artbatches,2sourcefont7/12px centred textfields;bounds and transactional guards pass.\n";}