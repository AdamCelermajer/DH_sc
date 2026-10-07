#include "character_fx_state_v1.hpp"
#include <cstring>
namespace {
using namespace dh2::fx;
bool valid(FxState96V1* s,const FxServices16V1* v){return s&&v&&v->invoke&&s->visible<=255&&s->looping<=255&&s->orient_once<=255&&s->orient_with_anchor<=255&&s->scale_with_anchor<=255&&s->fixed_rotation<=255;}
int call(FxState96V1* s,const FxServices16V1* v,FxOperationV1 op,std::uint32_t arg=0,std::uintptr_t id=0,float f=0,std::uintptr_t p=0,std::uint32_t* result=nullptr){FxRequest32V1 request{op,arg,id,f,0,p};if(v->invoke(v->context,s,&request))return -2;if(result)*result=request.argument;return 0;}
int sync(FxState96V1* s,const FxServices16V1* v,bool b){return call(s,v,FxOperationV1::sync,b);}
int speed(FxState96V1* s,const FxServices16V1* v,float f){s->speed=f;return call(s,v,FxOperationV1::speed,0,0,f);}
int loop(FxState96V1* s,const FxServices16V1* v,std::uint32_t l){if(call(s,v,FxOperationV1::loop,l))return -2;s->looping=l;return 0;}
int visible(FxState96V1* s,const FxServices16V1* v,std::uint32_t b){if(s->visible==b)return 0;s->visible=b;if(call(s,v,FxOperationV1::visible,b))return -2;return sync(s,v,false);}
bool data_valid(const FxData32V1* d){return d&&d->orient_once<=255&&d->orient_with_anchor<=255&&d->scale_with_anchor<=255;}
std::int32_t sub(std::int32_t x,std::uint32_t y){auto u=std::uint32_t(x)-y;std::int32_t z;std::memcpy(&z,&u,4);return z;}
}
extern "C" int dh2_fx_set_anim_v1(FxState96V1* s,const FxData32V1* d,std::uintptr_t cb,const FxServices16V1* v){
 if(!valid(s,v)||!data_valid(d))return -1;
 s->orient_once=d->orient_once;if(sync(s,v,false))return -2;
 s->orient_with_anchor=d->orient_with_anchor;if(sync(s,v,true))return -2;
 s->scale_with_anchor=d->scale_with_anchor;if(sync(s,v,false))return -2;
 if(speed(s,v,d->speed))return -2;s->set_identity=d->set_identity;s->loop=d->loop;s->callback=cb;return 0;
}
extern "C" int dh2_fx_play_v1(FxState96V1* s,const float* position,const float* rotation,std::uintptr_t anchor,const FxData32V1* d,std::uintptr_t cb,const FxServices16V1* v){
 if(!valid(s,v)||!position||(d&&!data_valid(d)))return -1;
 std::memcpy(s->position,position,12);if(sync(s,v,false))return -2;
 if(rotation){std::memcpy(s->rotation,rotation,12);s->fixed_rotation=1;if(sync(s,v,false))return -2;}
 if(loop(s,v,1)||call(s,v,FxOperationV1::start))return -2;
 const FxData32V1 defaults{1,0,1,1.f,0,-1,0};if(dh2_fx_set_anim_v1(s,d?d:&defaults,cb,v))return -2;
 if(d)s->timer=d->play_time;
 if(anchor){s->anchor=anchor;if(sync(s,v,true))return -2;}
 return visible(s,v,1);
}
extern "C" int dh2_fx_drop_reset_v1(FxState96V1* s,const FxServices16V1* v){
 if(!valid(s,v))return -1;s->anchor=0;if(sync(s,v,true))return -2;std::memset(s->position,0,12);if(sync(s,v,false))return -2;return visible(s,v,0);
}
extern "C" int dh2_fx_handle_loop_end_v1(FxState96V1* s,const FxServices16V1* v){
 if(!valid(s,v))return -1;if((s->speed<=0.f&&!s->callback)||s->loop<0)return 0;
 if(s->loop>0){s->loop=sub(s->loop,1);return call(s,v,FxOperationV1::start);}
 if(loop(s,v,0))return -2;
 // The source set-data finished byte is caller-owned and written before cb.
 if(s->set_identity&&call(s,v,FxOperationV1::end_callback,1,s->set_identity,0,0))return -2;
 const auto cb=s->callback;if(!cb)return 0;
 if(call(s,v,FxOperationV1::end_callback,0,cb,0,s->set_identity))return -2;
 s->callback=0;return call(s,v,FxOperationV1::end_sample);
}
extern "C" int dh2_fx_update_v1(FxState96V1* s,const FxServices16V1* v){
 if(!valid(s,v))return -1;std::uint32_t result;
 if(s->anchor){if(call(s,v,FxOperationV1::anchor_dead,0,s->anchor,0,0,&result))return -2;
  if(result)s->anchor=0;else{if(call(s,v,FxOperationV1::anchor_disabled,0,s->anchor,0,0,&result))return -2;if(result)s->anchor=0;}}
 const auto timer=s->timer;if(timer>=0){if(call(s,v,FxOperationV1::app_dt,0,0,0,0,&result))return -2;s->timer=sub(timer,result);if(s->timer<=0)s->loop=0;}
 if(s->anchor){if(call(s,v,FxOperationV1::anchor_stationary,0,s->anchor,0,0,&result))return -2;if(!result&&sync(s,v,false))return -2;}
 if(s->visible){if(s->set_identity){if(call(s,v,FxOperationV1::debug_load)||call(s,v,FxOperationV1::debug_set))return -2;}if(call(s,v,FxOperationV1::debug_load)||call(s,v,FxOperationV1::debug_instance))return -2;}
 if(speed(s,v,s->speed))return -2;
 if(!s->looping&&s->visible){if(call(s,v,FxOperationV1::completed,0,0,0,0,&result))return -2;if(result&&visible(s,v,0))return -2;}
 return 0;
}
