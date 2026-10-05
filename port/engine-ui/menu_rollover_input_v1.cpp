#include "menu_rollover_input_v1.hpp"
#include "character_menu_queries_owner_v1.hpp"
#include <cstddef>
#include <utility>
static_assert(sizeof(void*)==8);
static_assert(sizeof(dh2::ui::MenuRolloverValueV1)==16);
static_assert(sizeof(dh2::ui::MenuRolloverCallV1)==16);
static_assert(sizeof(dh2::ui::MenuRolloverBindingsV1)==32);
static_assert(sizeof(dh2::ui::MenuRolloverServicesV1)==40);
extern "C" int dh2_menu_rollover_input_v1(const dh2::ui::MenuRolloverCallV1* c,
 const dh2::ui::MenuRolloverServicesV1* s){
 if(!c||!s||c->reserved||c->count<2||!c->arguments||!s->number||!s->integer||!s->boolean||!s->instance)return -1;
 for(unsigned i=0;i<2;++i)if(!c->arguments[i].identity||c->arguments[i].reserved[0]||c->arguments[i].reserved[1])return -1;
 double number{};std::int32_t index{};std::uint32_t enabled{};
 if(s->number(s->context,&c->arguments[0],&number))return -2;
 if(s->integer(s->context,number,&index))return -2;
 if(s->boolean(s->context,&c->arguments[1],&enabled))return -2;
 dh2::ui::MenuRolloverBindingsV1* binding{};
 if(s->instance(s->context,&binding))return -2;
 if(!binding||static_cast<std::uint32_t>(index)>3||!binding->renders[index])return -3;
 // Native SetInputBehavior replaces the complete source RenderFX+f8 word.
 binding->renders[index]->flags=enabled?0x84u:4u;
 return 0;
}
namespace dh2::ui {
MenuRolloverInputV1::MenuRolloverInputV1(MenuRolloverGraphV1 graph):graph_(std::move(graph)){}
bool MenuRolloverInputV1::dispatch(CharacterMenuCallV1& c,std::string& error)const{
 const auto graph=graph_;
 if(!graph.owner||!graph.integer||!graph.instance||!c.number||!c.boolean||c.arguments.size()<2){error="required original rollover argument/render service absent";return false;}
 // Root's conversion callbacks retain the original slot and property receiver.
 struct Context {const MenuRolloverGraphV1* graph;CharacterMenuCallV1* call;std::string* error;};
 Context context{&graph,&c,&error};
 MenuRolloverValueV1 values[2]{{1,{0,0}},{2,{0,0}}};
 MenuRolloverCallV1 call{values,2,0};
 MenuRolloverServicesV1 services{&context,
  [](void* p,const MenuRolloverValueV1* v,double* out){auto& x=*static_cast<Context*>(p);return x.call->number(x.call->arguments[v->identity-1],*out,*x.error)?0:-1;},
  [](void* p,double v,std::int32_t* out){auto& x=*static_cast<Context*>(p);return x.graph->integer(v,*out,*x.error)?0:-1;},
  [](void* p,const MenuRolloverValueV1* v,std::uint32_t* out){auto& x=*static_cast<Context*>(p);bool b{};if(!x.call->boolean(x.call->arguments[v->identity-1],b,*x.error))return -1;*out=b;return 0;},
  [](void* p,MenuRolloverBindingsV1** out){auto& x=*static_cast<Context*>(p);return x.graph->instance(*out,*x.error)?0:-1;}};
 const int result=dh2_menu_rollover_input_v1(&call,&services);
 if(result&&error.empty())error=result==-3?"source rollover selected an absent renderer":"required rollover service failed";
 return result==0;
}
}
