#include "input_manager_v1.hpp"

#include <cmath>
#include <cstring>

namespace dh2::input_manager_v1 {
namespace {
float literal(std::uint32_t word) { float value;std::memcpy(&value,&word,4);return value; }
float add(float a,float b) { volatile float value=a+b;return value; }
float sub(float a,float b) { volatile float value=a-b;return value; }
float mul(float a,float b) { volatile float value=a*b;return value; }
float divide(float a,float b) { volatile float value=a/b;return value; }
float neg(float value) { std::uint32_t bits;std::memcpy(&bits,&value,4);bits^=0x80000000;return literal(bits); }
float absolute(float value) { std::uint32_t bits;std::memcpy(&bits,&value,4);bits&=0x7fffffff;return literal(bits); }
float length_squared(const Point& p) {return add(add(mul(p.x,p.x),mul(p.y,p.y)),mul(p.z,p.z));}
float dot(const Point& a,const Point& b) {return add(add(mul(a.x,b.x),mul(a.y,b.y)),mul(a.z,b.z));}
void normalize(Point& p) {
    // Point3D<float>::normalize divides each component by the captured length.
    // The engine Vector3f reciprocal-multiply helper has different rounding.
    const float length=::sqrtf(length_squared(p));
    p.x=divide(p.x,length);p.y=divide(p.y,length);p.z=divide(p.z,length);
}
bool same(const Point& a,const Point& b) {
    const float epsilon=literal(0x38d1b717);
    return absolute(sub(a.x,b.x))<epsilon && absolute(sub(a.y,b.y))<epsilon &&
           absolute(sub(a.z,b.z))<epsilon;
}
std::int32_t delta(std::int32_t current,std::int32_t prior) {
    const auto word=static_cast<std::uint32_t>(current)-static_cast<std::uint32_t>(prior);
    std::int32_t value;std::memcpy(&value,&word,4);return value;
}
bool tick(const Clock* clock,Result& result,std::int32_t& value) {
    if (!clock || !clock->read) {result.status=Status::missing_provider;return false;}
    ++result.clock_reads;
    try {if (clock->read(clock->context,&value)==0) return true;} catch (...) {}
    result.status=Status::provider_failed;return false;
}
void construct_channel(Channel& c) {
    c.field_10=0;c.previous_minimum=0;c.previous=0;c.minimum=0;c.value=0;
    c.previous_maximum=1;c.maximum=1;c.previous_above_threshold=0;
}
void update_channel(Channel& c,Result& result) {
    const float threshold=mul(add(add(c.previous_minimum,c.previous_maximum),1.0f),0.5f);
    c.previous_above_threshold=c.previous>=threshold;
    c.previous=c.value;c.previous_minimum=c.minimum;c.previous_maximum=c.maximum;
    ++result.updated_channels;
}
}
void construct_device(Device& d,std::uint32_t type) {d.type=type;d.field_8=0;d.dispatch=Dispatch::device;}
void construct_keyboard(Keyboard& k) {
    construct_device(k.device,1);k.device.dispatch=Dispatch::keyboard;
    for(auto& c:k.keys) construct_channel(c);
}
void construct_mouse(Mouse& m) {
    construct_device(m.device,1);m.device.dispatch=Dispatch::mouse;
    for(auto& c:m.buttons) construct_channel(c);
    for(auto& c:m.axes) construct_channel(c);
}
void construct_gamepad(Gamepad& g,const Globals& globals) {
    construct_device(g.device,2);g.device.dispatch=Dispatch::gamepad;
    for(auto& c:g.buttons) construct_channel(c);
    for(auto& c:g.axes) construct_channel(c);
    for(auto& p:g.first_vectors) p={0,0,0};
    for(auto& p:g.repeat_vectors) p={0,0,0};
    g.field_754=128;g.vibration_74c=0;g.connected_758=0;g.field_750=0;
    for(unsigned i=0;i<4;++i) {
        g.first_vectors[i]=globals.origin;g.first_stamps[i]=0;
        g.repeat_vectors[i]=globals.origin;g.repeat_stamps[i]=0;
    }
}
void construct_base(Base& b,std::int32_t mice,std::int32_t keyboards,std::int32_t gamepads) {
    b.enabled_10=1;b.mice=mice;b.dispatch=Dispatch::manager;b.keyboards=keyboards;b.gamepads=gamepads;
}
void construct_win32(Manager& m,const Globals& globals) {
    construct_base(m.base,1,1,4);m.base.dispatch=Dispatch::win32;
    construct_keyboard(m.keyboard);construct_mouse(m.mouse);
    for(auto& g:m.gamepads) construct_gamepad(g,globals);
}
std::int32_t num_mice(const Base& b) {return b.mice;}
std::int32_t num_keyboards(const Base& b) {return b.keyboards;}
std::int32_t num_gamepads(const Base& b) {return b.gamepads;}
Mouse* get_mouse(Manager& m,std::int32_t) {return &m.mouse;}
Keyboard* get_keyboard(Manager& m,std::int32_t) {return &m.keyboard;}
Gamepad* get_gamepad(Manager& m,std::int32_t index) {return index>=0&&index<4?&m.gamepads[index]:nullptr;}
std::int32_t connected_gamepad_count(Manager& m) {
    std::int32_t count=0;
    for(std::int32_t i=0;i<num_gamepads(m.base);++i) {
        auto* g=get_gamepad(m,i);if(!g) return -1;
        if(g->connected_758) ++count;
    }
    return count;
}
Gamepad* first_connected_gamepad(Manager& m) {
    for(std::int32_t i=0;i<num_gamepads(m.base);++i) {
        auto* g=get_gamepad(m,i);if(!g) return nullptr;
        if(g->connected_758) return get_gamepad(m,i);
    }
    return nullptr;
}
void update_keyboard(Keyboard& k,Result& result) {for(auto& c:k.keys) update_channel(c,result);++result.updated_devices;}
void update_mouse(Mouse& m,Result& result) {
    for(auto& c:m.buttons) update_channel(c,result);
    for(auto& c:m.axes) update_channel(c,result);
    ++result.updated_devices;
}
Status update_gamepad(Gamepad& g,const Globals& globals,const Clock* clock,Result& result) {
    for(auto& c:g.buttons) update_channel(c,result);
    for(auto& c:g.axes) update_channel(c,result);
    for(unsigned i=0;i<4;++i) {
        result.vector_index=i;
        if(!g.first_stamps[i]) g.first_vectors[i]=globals.origin;
        if(!g.repeat_stamps[i]) g.repeat_vectors[i]=globals.origin;
        const auto& x=g.axes[2*i];const auto& y=g.axes[2*i+1];
        Point vector{divide(add(x.value,x.value),sub(x.maximum,x.minimum)),
                     neg(divide(add(y.value,y.value),sub(y.maximum,y.minimum))),0};
        const float length=::sqrtf(add(length_squared(vector),0));
        Point normalized=globals.origin;
        if(!(length<literal(0x3f266666))) {
            normalize(vector);
            if(length<literal(0x3f733333)) {
                const float scale=divide(sub(length,literal(0x3f266666)),literal(0x3e99999a));
                vector.x=mul(scale,vector.x);vector.y=mul(scale,vector.y);vector.z=mul(scale,vector.z);
            }
            normalized=vector;
        }
        if(length_squared(normalized)>0) {
            normalize(normalized);
            std::int32_t now;
            if(!g.first_stamps[i]) {
                g.first_vectors[i]=normalized;
                if(!tick(clock,result,now)) return result.status;
                g.first_stamps[i]=now;
            } else {
                if(!tick(clock,result,now)) return result.status;
                if(delta(now,g.first_stamps[i])>200) g.first_vectors[i]=globals.origin;
            }
            continue;
        }
        g.first_stamps[i]=0;
        if(same(g.first_vectors[i],globals.origin)) {
            std::int32_t now;
            if(!tick(clock,result,now)) return result.status;
            if(delta(now,g.repeat_stamps[i])<=400) continue;
            g.repeat_stamps[i]=0;
            g.repeat_vectors[i]=globals.origin;
            continue;
        }
        if(!g.repeat_stamps[i]) {
            std::int32_t now;
            if(!tick(clock,result,now)) return result.status;
            g.repeat_stamps[i]=now;g.repeat_vectors[i]=g.first_vectors[i];
            continue;
        }
        std::int32_t now;
        if(!tick(clock,result,now)) return result.status;
        if(delta(now,g.repeat_stamps[i])>400) {
            g.repeat_vectors[i]=globals.origin;
            continue;
        }
        if(same(g.repeat_vectors[i],globals.origin)) continue;
        const float product=dot(g.repeat_vectors[i],g.first_vectors[i]);
        if(product>1) {g.repeat_stamps[i]=0;continue;}
        if(!(product<neg(1.0f)) && !(::acosf(product)>literal(0x3ea0d97c))) {
            g.repeat_stamps[i]=0;continue;
        }
        g.repeat_vectors[i]=globals.origin;
        g.repeat_stamps[i]=0;
    }
    ++result.updated_devices;return result.status;
}
Status update_frame(Manager& m,const Globals& globals,const Clock* clock,Result& result) {
    for(std::int32_t i=0;i<num_mice(m.base);++i) update_mouse(*get_mouse(m,i),result);
    for(std::int32_t i=0;i<num_keyboards(m.base);++i) update_keyboard(*get_keyboard(m,i),result);
    for(std::int32_t i=0;i<num_gamepads(m.base);++i) {
        result.gamepad_index=i;auto* g=get_gamepad(m,i);
        if(!g) {result.status=Status::invalid_argument;return result.status;}
        if(update_gamepad(*g,globals,clock,result)!=Status::complete) return result.status;
    }
    return result.status;
}
} // namespace dh2::input_manager_v1
