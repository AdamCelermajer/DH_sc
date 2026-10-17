#include "../input_manager_v1.hpp"
#include <cstdio>
#include <cstring>
#include <vector>
using namespace dh2::input_manager_v1;
struct Header { std::uint32_t op;std::int32_t index;std::uint32_t count,fail;Point origin; };
struct Ticks {std::vector<std::int32_t> values;std::uint32_t calls=0,fail=0;};
static std::int32_t clock_read(void* p,std::int32_t* value) {
    auto& t=*static_cast<Ticks*>(p);++t.calls;
    if(t.calls==t.fail || t.calls>t.values.size()) return 1;
    *value=t.values[t.calls-1];return 0;
}
int main(int argc,char** argv) {
    if(argc!=3) return 2;
    auto* in=std::fopen(argv[1],"rb");auto* out=std::fopen(argv[2],"wb");
    if(!in||!out) return 3;
    std::uint32_t count=0;if(std::fread(&count,4,1,in)!=1) return 4;
    for(std::uint32_t c=0;c<count;++c) {
        Header h{};Manager m{};Result r{};Ticks ticks;
        if(std::fread(&h,sizeof(h),1,in)!=1 || std::fread(&m,sizeof(m),1,in)!=1) return 5;
        ticks.values.resize(h.count);ticks.fail=h.fail;
        if(std::fread(ticks.values.data(),4,h.count,in)!=h.count) return 6;
        Globals g{h.origin};Clock clock{&ticks,clock_read};std::uint32_t value=0;
        auto offset=[&](const void* pointer) {return pointer?static_cast<std::uint32_t>(static_cast<const char*>(pointer)-reinterpret_cast<const char*>(&m)):0xffffffffu;};
        switch(h.op) {
        case 0:construct_win32(m,g);break;
        case 1:construct_keyboard(m.keyboard);break;
        case 2:construct_mouse(m.mouse);break;
        case 3:construct_gamepad(m.gamepads.at(h.index),g);break;
        case 4:update_frame(m,g,h.fail==0xffffffffu?nullptr:&clock,r);break;
        case 5:update_keyboard(m.keyboard,r);break;
        case 6:update_mouse(m.mouse,r);break;
        case 7:update_gamepad(m.gamepads.at(h.index),g,h.fail==0xffffffffu?nullptr:&clock,r);break;
        case 8:value=num_mice(m.base);break;
        case 9:value=num_keyboards(m.base);break;
        case 10:value=num_gamepads(m.base);break;
        case 11:value=offset(get_mouse(m,h.index));break;
        case 12:value=offset(get_keyboard(m,h.index));break;
        case 13:value=offset(get_gamepad(m,h.index));break;
        case 14:value=connected_gamepad_count(m);break;
        case 15:value=offset(first_connected_gamepad(m));break;
        case 16:construct_base(m.base,m.base.mice,m.base.keyboards,m.base.gamepads);break;
        default:return 7;
        }
        std::uint32_t result[]={static_cast<std::uint32_t>(r.status),r.clock_reads,r.updated_channels,r.updated_devices,value};
        if(std::fwrite(result,sizeof(result),1,out)!=1 || std::fwrite(&m,sizeof(m),1,out)!=1) return 8;
    }
    std::fclose(in);std::fclose(out);return 0;
}
