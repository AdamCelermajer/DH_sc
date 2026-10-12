#include "hud_sprite_timeline.hpp"
#include <cstring>
namespace {
using namespace dh2::ui;
template<class T> bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool buffer(const HudActionBuffer16& b){return b.count<=b.capacity&&b.capacity<=4096&&(!b.capacity||aligned(b.values));}
bool disjoint(const void* a,std::size_t an,const void* b,std::size_t bn){if(!an||!bn)return true;const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=UINTPTR_MAX-an&&y<=UINTPTR_MAX-bn&&(x+an<=y||y+bn<=x);}
bool valid(const HudSpriteState64* s,const HudSpriteServices16* svc){return aligned(s)&&aligned(svc)&&s->reserved==0&&s->clip&&s->definition&&s->current_frame>=-32768&&s->current_frame<=32767&&buffer(s->pending)&&buffer(s->goto_actions)&&disjoint(s,sizeof(*s),s->pending.values,s->pending.capacity*sizeof(std::uintptr_t))&&disjoint(s,sizeof(*s),s->goto_actions.values,s->goto_actions.capacity*sizeof(std::uintptr_t))&&disjoint(s->pending.values,s->pending.capacity*sizeof(std::uintptr_t),s->goto_actions.values,s->goto_actions.capacity*sizeof(std::uintptr_t));}
int call(HudSpriteState64* s,const HudSpriteServices16* svc,HudSpriteOperation op,int frame=0,int only=0,std::uintptr_t sound=0,HudSpriteResponse16* result=nullptr){HudSpriteResponse16 out{};const HudSpriteRequest32 request{op,0,frame,only,s->clip,sound};if(!svc->invoke||svc->invoke(svc->context,s,&request,&out)!=1)return -2;if(result)*result=out;return 0;}
std::int32_t frame16(std::int32_t value){const auto bits=std::uint16_t(value);return bits<32768?std::int32_t(bits):std::int32_t(bits)-65536;}
std::int32_t byte8(std::int32_t value){const auto bits=std::uint8_t(value);return bits<128?std::int32_t(bits):std::int32_t(bits)-256;}
}
extern "C" int dh2_ui_hud_sprite_goto_v1(dh2::ui::HudSpriteState64* s,std::int32_t target,const dh2::ui::HudSpriteServices16* svc) noexcept {
 using namespace dh2::ui;if(!valid(s,svc))return -1;HudSpriteResponse16 count{};int rc=call(s,svc,HudSpriteOperation::frame_count,0,0,0,&count);if(rc)return rc;
 if(count.value<0||count.value>65536)return -2;
 if(target>=count.value||target<0||target==s->current_frame){s->play_state=1;return 0;}
 if(!buffer(s->pending)||!buffer(s->goto_actions)||s->pending.count>s->goto_actions.capacity)return -2;
 s->goto_actions.count=s->pending.count;
 for(unsigned i=0;i<s->pending.count;++i)s->goto_actions.values[i]=s->pending.values[i];
 const auto current=s->current_frame;s->pending.count=0;
 if(target<current){for(auto frame=current;frame>target;--frame){rc=call(s,svc,HudSpriteOperation::reverse_tags,frame);if(rc)return rc;}}
 else {for(auto frame=current+1;frame<target;++frame){rc=call(s,svc,HudSpriteOperation::forward_tags,frame,1);if(rc)return rc;}}
 s->pending.count=0;rc=call(s,svc,HudSpriteOperation::forward_tags,target,0);if(rc)return rc;
 s->current_frame=frame16(target);s->play_state=1;
 if(!buffer(s->pending)||!buffer(s->goto_actions)||s->pending.count>s->goto_actions.capacity-s->goto_actions.count)return -2;
 const auto old_count=s->goto_actions.count;
 for(unsigned i=0;i<s->pending.count;++i)s->goto_actions.values[old_count+i]=s->pending.values[i];
 s->goto_actions.count+=s->pending.count;s->pending.count=0;
 rc=call(s,svc,HudSpriteOperation::notify_advance);return rc?rc:1;
}
extern "C" int dh2_ui_hud_sprite_play_v1(dh2::ui::HudSpriteState64* s,std::int32_t play,const dh2::ui::HudSpriteServices16* svc) noexcept {
 using namespace dh2::ui;if(!valid(s,svc))return -1;HudSpriteResponse16 sound{};int rc=call(s,svc,HudSpriteOperation::sound_handler,0,0,0,&sound);if(rc)return rc;
 if(sound.identity&&s->stream_sound_id>=0){rc=call(s,svc,HudSpriteOperation::pause_sound,s->stream_sound_id,byte8(s->play_state)==0?1:0,sound.identity);if(rc)return rc;}
 s->play_state=byte8(play);return call(s,svc,HudSpriteOperation::notify_advance);
}
