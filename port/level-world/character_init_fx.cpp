#include "character_init_fx.hpp"
namespace {
bool valid(const dh2::fx::PreloadTable16* t,const dh2::fx::PreloadQueue16* q,const dh2::fx::PreloadServices16* s){
 if(!t||!q||!s||!s->call||t->set_count>4096||t->effect_count>4096||(t->set_count&&!t->sets)||q->count>q->capacity||(q->capacity&&!q->ids))return false;
 for(std::uint32_t i=0;i<t->set_count;++i)if(t->sets[i].reserved||t->sets[i].count>4096||(t->sets[i].count>0&&!t->sets[i].steps))return false;
 return true;
}
}
extern "C" int dh2_character_init_fx_register(const dh2::character::InitFxRows16* source,
 const dh2::fx::PreloadTable16* table,dh2::fx::PreloadQueue16* queue,const dh2::fx::PreloadServices16* services){
 using namespace dh2::fx;
 if(!source||!source->rows||!source->count||source->count>4096||!valid(table,queue,services))return -1;
 const auto index=source->effects_index<0||static_cast<std::uint32_t>(source->effects_index)>=source->count?0:source->effects_index;
 const auto& row=source->rows[index];
 const std::int32_t captured[]={row.footprint,row.blood,row.blood_death};
 std::uint32_t ignored=0;
 if(services->call(services->context,debug_load,nullptr,&ignored)||services->call(services->context,debug_switch,"isTracingChar_Init",&ignored))return -2;
 for(auto id:captured)if(id>=0){const int result=dh2_fx_register_set(table,queue,id,services);if(result!=1)return result;}
 return 1;
}
extern "C" int dh2_character_init_fx_negative_grab(std::uintptr_t* out,std::int32_t id,
 std::uint32_t count,const dh2::fx::PreloadServices16* services){
 using namespace dh2::fx;
 if(!out||!services||!services->call||count>4096)return -1;
 std::uint32_t module=0;
 if(services->call(services->context,debug_load,nullptr,&module)||services->call(services->context,debug_module,"AnimatedFX",&module))return -2;
 if(module&&id>=0&&static_cast<std::uint32_t>(id)<count)return -3;
 *out=0;return 1;
}
