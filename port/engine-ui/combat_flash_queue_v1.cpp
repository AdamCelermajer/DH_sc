#include "combat_flash_queue_v1.hpp"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <limits>
namespace dh2::ui {
namespace {std::int32_t bits(std::uint32_t value){std::int32_t result;std::memcpy(&result,&value,4);return result;}}
CombatFlashQueueV1::CombatFlashQueueV1(CombatFlashServicesV1 s):services_(s){}
int CombatFlashQueueV1::scan(){if(!services_.scan)return -1;std::vector<CombatFlashStyleV1> found;if(services_.scan(services_.context,found))return -2;styles_=std::move(found);return 1;}
int CombatFlashQueueV1::style_id(const char* name)const{if(name)for(std::size_t i=styles_.size();i>0;--i)if(styles_[i-1].name==name)return int(i-1);return 0;} // source map[name]=scan_index: final duplicate wins
void CombatFlashQueueV1::stop_all(){for(auto& c:contexts_)c.flags=0;}
int CombatFlashQueueV1::play(const char* name,const float position[3],const char* text,std::int32_t number,std::int32_t color,bool numeric){
 if(!position||!services_.project||!services_.inverse_pixel_scale)return -1;
 const auto style=style_id(name);if(std::size_t(style)>=styles_.size()||!styles_[style].instances[0].clip)return -2;
 std::int32_t px{},py{};float sx{},sy{};
 if(services_.project(services_.context,position,&px,&py)||services_.inverse_pixel_scale(services_.context,styles_[style].instances[0].clip,&sx,&sy))return -2;
 const float x=float(px)*sx,y=float(py)*sy;
 if(!std::isfinite(x)||!std::isfinite(y)||x<-2147483648.f||x>=2147483648.f||y<-2147483648.f||y>=2147483648.f)return -2;
 CombatFlashContextV1* context=nullptr;for(auto& c:contexts_)if(!(c.flags&1)){context=&c;break;}if(!context)return 1;
 context->style=style;auto& instances=styles_[style].instances;int instance=0;while(instance<8&&instances[instance].busy)++instance;if(instance==8)instance=7;
 auto& selected=instances[instance];
 if(!selected.clip){if(!services_.clone)return -2;char clone[32];std::snprintf(clone,sizeof clone,"_clone_%d",instance);if(services_.clone(services_.context,instances[0].clip,clone,&selected)||!selected.clip)return -2;}
 selected.busy=true;context->instance=instance;context->x=int(x);context->y=int(y);context->frame=0;context->timer=0;context->flags=3;context->color=color;
 if(numeric){if(number<0)context->text[0]=0;if(number<0x7ffffffe)std::snprintf(context->text.data(),context->text.size(),"%d",number);}
 else {if(!text)return -2;const auto length=std::strlen(text);if(length<=46)std::memcpy(context->text.data(),text,length+1);}
 if(selected.text&&(!services_.set_text||services_.set_text(services_.context,selected.text,context->text.data())))return -2;
 return 1;
}
int CombatFlashQueueV1::update(std::uint32_t dt,std::int32_t interval){
 if(interval<0||!services_.frame_count)return -1;
 for(auto& c:contexts_)if(c.flags&1){
  if(c.style<0||std::size_t(c.style)>=styles_.size()||c.instance<0||c.instance>=8)return -2;
  auto& instance=styles_[c.style].instances[c.instance];c.timer=bits(std::uint32_t(c.timer)+dt);
  while(c.timer>interval){c.timer=bits(std::uint32_t(c.timer)-std::uint32_t(interval));c.frame=bits(std::uint32_t(c.frame)+1);int count{};if(!instance.clip||services_.frame_count(services_.context,instance.clip,&count))return -2;if((c.flags&2)&&count<=c.frame){c.flags=0;instance.busy=false;}}
 }
 return 1;
}
int CombatFlashQueueV1::draw(bool disabled){if(disabled)return 1;if(!services_.begin||!services_.draw||!services_.end)return -1;if(services_.begin(services_.context))return -2;
 for(const auto& c:contexts_)if(c.flags&1){if(c.style<0||std::size_t(c.style)>=styles_.size()||c.instance<0||c.instance>=8){services_.end(services_.context);return -2;}const auto& i=styles_[c.style].instances[c.instance];if(i.clip&&services_.draw(services_.context,i.clip,i.text,&c)){services_.end(services_.context);return -2;}}
 return services_.end(services_.context)?-2:1;
}
}
