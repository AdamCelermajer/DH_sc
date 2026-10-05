#include "port/engine-ui/combat_flash_queue_v1.hpp"
#include <cstring>
struct Fixture{int frames,calls;};
extern "C" int dh2_combat_flash_timer_v1(int* rows,unsigned dt,int interval,int frames){
 Fixture f{frames,0};dh2::ui::CombatFlashServicesV1 s{};s.context=&f;
 s.scan=[](void*,std::vector<dh2::ui::CombatFlashStyleV1>& out){dh2::ui::CombatFlashStyleV1 style;style.instances[0].clip=1;style.instances[0].busy=true;out.push_back(std::move(style));return 0;};
 s.frame_count=[](void* p,std::uintptr_t,int* n){auto& f=*static_cast<Fixture*>(p);++f.calls;*n=f.frames;return 0;};
 dh2::ui::CombatFlashQueueV1 queue(s);queue.scan();auto& c=const_cast<std::array<dh2::ui::CombatFlashContextV1,12>&>(queue.contexts());
 for(unsigned n=0;n<12;++n){c[n].frame=rows[n*3];c[n].timer=rows[n*3+1];c[n].flags=unsigned(rows[n*3+2]);c[n].style=0;c[n].instance=0;}
 const auto result=queue.update(dt,interval);
 for(unsigned n=0;n<12;++n){rows[n*3]=c[n].frame;rows[n*3+1]=c[n].timer;rows[n*3+2]=int(c[n].flags);}
 return result<0?result:f.calls;
}
