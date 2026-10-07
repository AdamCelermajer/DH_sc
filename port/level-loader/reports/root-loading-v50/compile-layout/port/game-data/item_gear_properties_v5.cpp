#include "item_gear_properties_v5.hpp"
#include <cstring>
namespace {
constexpr std::size_t sheet_size=224*4;
bool span(const void* p,std::size_t n,std::size_t a){auto x=reinterpret_cast<std::uintptr_t>(p);return p&&x%a==0&&n<=UINTPTR_MAX-x;}
bool overlap(const void* p,std::size_t n,const void* q,std::size_t k){auto x=reinterpret_cast<std::uintptr_t>(p),y=reinterpret_cast<std::uintptr_t>(q);return x<y+k&&y<x+n;}
bool sheets(std::int32_t* g,const std::int32_t* d){return span(g,sheet_size,4)&&span(d,sheet_size,4)&&!overlap(g,sheet_size,d,sheet_size);}
std::int32_t word(std::uint32_t x){std::int32_t r;std::memcpy(&r,&x,4);return r;}
void add(std::int32_t* g,const std::int32_t* d,unsigned p,std::int32_t x){g[p]=g[p]==d[p]?x:word(std::uint32_t(g[p])+std::uint32_t(x));}
// Actual 0..48 jump-table branches at 3e3308. Multi-destination order retained.
constexpr std::uint8_t targets[49][5]={
 {149,150,151,152,0},{149},{150},{151},{152},{38},{43},{95},{96},{97},
 {101},{102},{105},{106},{109},{110},{113},{114},{117},{118},
 {39},{44},{40},{45},{63},{60},{61},{71},{79,80},{50},{59},
 {74},{77},{75},{78},{76},{74,77,75,78,76},{132},{133},{165},{158},
 {166,169,167,170,168},{170},{167},{168},{169},{166},{195},{196}
};
unsigned destination(unsigned type,unsigned p,bool left){
 if(!left)return p;if(type>=7&&type<=9)return p+3;
 if(type>=10&&type<=19)return p+2;if(type==28)return p+2;return p;
}
}
extern "C" int dh2_gear_reset_v5(std::int32_t* g,const std::int32_t* d) noexcept{
 if(!sheets(g,d))return -1;for(unsigned i=0;i<224;++i)g[i]=d[i];return 0;
}
extern "C" int dh2_gear_stats_v5(std::int32_t* g,const std::int32_t* d,const dh2::data::ItemRecord164* item,std::uint32_t left) noexcept{
 if(!sheets(g,d)||left>1||!span(item,sizeof(*item),4)||overlap(g,sheet_size,item,sizeof(*item)))return -1;
 auto& w=item->words;auto type=std::uint32_t(w[22]);if(type>12||type==11)return 0;
 if(type<=3){unsigned p=(type==1||((type==0||type==3)&&left))?81:79;add(g,d,p,w[35]);add(g,d,p+1,w[36]);}
 else if(type<=5){add(g,d,79,w[35]);add(g,d,80,w[36]);add(g,d,97,word(std::uint32_t(w[25])<<8));}
 else {add(g,d,71,w[35]);if(type==6)add(g,d,61,w[36]);}
 return 0;
}
extern "C" int dh2_gear_power_v5(std::int32_t* g,const std::int32_t* d,const dh2::data::GearPowerView16V5* v,std::uint32_t left) noexcept{
 if(!sheets(g,d)||left>1||!span(v,sizeof(*v),alignof(dh2::data::GearPowerView16V5))||v->reserved||v->count>65536||overlap(g,sheet_size,v,sizeof(*v)))return -1;
 auto n=std::size_t(v->count)*sizeof(dh2::data::GearPowerProperty12V5);
 if(n&&(!span(v->entries,n,4)||overlap(g,sheet_size,v->entries,n)))return -1;
 for(unsigned i=0;i<v->count;++i){auto& x=v->entries[i];auto type=std::uint32_t(x.type);if(type>=49)continue;
  for(unsigned p:targets[type]){if(!p)break;p=destination(type,p,left!=0);if(type==9)g[p]=x.value;else add(g,d,p,x.value);}
 }
 return 0;
}
extern "C" int dh2_gear_validate_vitals_v5(dh2::data::PropertyView* v) noexcept{
 if(dh2_property_validate(v))return -1;
 auto value=v->resolved[36]<v->resolved[38]?v->resolved[36]:v->resolved[38];if(dh2_property_set(v,36,value))return -1;
 // Source re-reads MP after the first complete Set/Resolve operation.
 value=v->resolved[41]<v->resolved[43]?v->resolved[41]:v->resolved[43];return dh2_property_set(v,41,value)?-1:0;
}
