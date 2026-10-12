#include "character_skill_class_v3.hpp"
#include <cstring>
namespace {
std::int32_t bits(std::uint32_t v){std::int32_t n;std::memcpy(&n,&v,4);return n;}
std::int32_t asr8(std::int32_t v){const auto n=std::uint32_t(v);return bits((n>>8)|((n&0x80000000u)?0xff000000u:0));}
unsigned apply(const dh2::data::ClassRow* rows,unsigned count,std::int32_t id,
 dh2::data::PropertyView* view,std::int32_t* stack,unsigned depth,unsigned& budget){
 if(id<0||std::uint32_t(id)>=count)return 0;if(depth>=32)return 5;
 for(unsigned n=0;n<depth;++n)if(stack[n]==id)return 5;stack[depth]=id;
 const auto& row=rows[id];if(row.count>10000||(!row.data&&row.count))return 4;
 for(unsigned n=0;n<row.count;++n){if(++budget>100000)return 6;const auto& f=row.data[n];unsigned action=0;
  if(f.type==1){if(f.destination<0||f.destination>=224||f.p2<0||f.p2>=224)return 4;
   const auto base=f.p1==-666?view->resolved[f.destination]:f.p1;std::int32_t source;
   if(dh2_property_resolve(view,f.p2,&source))return 4;
   view->resolved[f.destination]=bits(std::uint32_t(base)+std::uint32_t(f.p3)*std::uint32_t(asr8(source)));
  }else action=dh2_class_formula(f.destination,f.type,f.p1,f.p2,f.p3,view->resolved,nullptr);
  if(action==4)return 4;if(action==1)for(auto child:{f.p1,f.p2,f.p3}){auto error=apply(rows,count,child,view,stack,depth+1,budget);if(error)return error;}
  if(action==2)break;
 }return 0;
}
}
extern "C" unsigned dh2_character_skill_class_v3(const dh2::data::ClassRow* rows,
 std::uint32_t count,std::int32_t id,dh2::data::PropertyView* view){
 if(!rows||count>10000||!view||dh2_property_validate(view))return 4;
 std::int32_t stack[32];unsigned budget=0;return apply(rows,count,id,view,stack,0,budget);
}
