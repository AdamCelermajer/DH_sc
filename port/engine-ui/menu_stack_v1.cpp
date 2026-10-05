#include "menu_stack_v1.hpp"
#include <cstring>
using namespace dh2::ui;
namespace {
bool valid(const MenuStackV1* s,const MenuStackServicesV1* v=nullptr){
 if(!s||s->reserved||s->count>s->capacity||(s->capacity&&!s->renders)||(s->registry_count&&!s->registry)||!s->globals||s->globals->reserved)return false;
 if(v&&!v->invoke)return false;
 for(std::uint32_t i=0;i<s->registry_count;++i){auto m=s->registry[i];if(!m||!m->name||m->reserved||!m->render)return false;auto r=m->render;if(r->reserved||r->catalog_reserved||r->count>r->capacity||(r->capacity&&!r->states)||(r->catalog_count&&!r->catalog))return false;for(std::uint32_t j=0;j<r->catalog_count;++j)if(!r->catalog[j]||!r->catalog[j]->name)return false;}
 for(std::uint32_t i=0;i<s->count;++i){auto r=s->renders[i];if(!r||r->reserved||r->catalog_reserved||r->count>r->capacity||(r->capacity&&!r->states)||(r->catalog_count&&!r->catalog))return false;for(std::uint32_t j=0;j<r->count;++j)if(!r->states[j]||!r->states[j]->name)return false;}
 return true;
}
int call(MenuStackV1*s,const MenuStackServicesV1*v,MenuStackOperationV1 o,MenuStackRenderV1*r=nullptr,MenuStackMenuV1*m=nullptr,MenuStackCharacterV1*c=nullptr,const char*t=nullptr,std::uint32_t n=0,std::uintptr_t*out=nullptr){
 if(!v||!v->invoke)return -1;MenuStackRequestV1 q{o,n,r,m,c,t,0};if(v->invoke(v->context,s,&q))return -2;if(out)*out=q.result;return valid(s)?0:-3;
}
MenuStackCharacterV1* weak(MenuStackCharacterV1*& p){if(p&&!p->live)p=nullptr;return p;}
MenuStackMenuV1* top(MenuStackRenderV1*r){return r&&r->count&&r->count<=r->capacity?r->states[r->count-1]:nullptr;}
MenuStackMenuV1* find(const MenuStackV1*s,const char*n){for(std::uint32_t i=0;i<s->registry_count;++i)if(!std::strcmp(n,s->registry[i]->name))return s->registry[i];return nullptr;}
bool contains(const MenuStackV1*s,const MenuStackMenuV1*m){for(std::uint32_t i=0;i<s->count;++i){auto r=s->renders[i];for(std::uint32_t j=0;j<r->count;++j)if(r->states[j]==m)return true;}return false;}
int visibility(MenuStackMenuV1*m){auto c=weak(m->character);if(!c)return -3;c->visible=1;return 0;}
int focusflag(MenuStackMenuV1*m,std::uint32_t value){auto c=weak(m->character);if(!c)return -3;if(c->type_is_two)c->focus_enabled=value;return 0;}
int context(MenuStackRenderV1*r,MenuStackMenuV1*m){r->context=weak(m->character);return 0;}
int restorefocus(MenuStackV1*s,const MenuStackServicesV1*v,MenuStackRenderV1*r){auto m=top(r);if(!m)return -3;if(weak(m->saved_focus)){int e=call(s,v,MenuStackOperationV1::reset_focus,r,nullptr,nullptr,nullptr,0);if(e)return e;m=top(r);if(!m)return -3;return call(s,v,MenuStackOperationV1::set_focus,r,nullptr,weak(m->saved_focus),nullptr,0);}return 0;}
void classify(MenuStackGlobalsV1&g,const char*n){
 if(!std::strcmp(n,"menu_VerificationLoading"))g.back_glive=1;
 else if(!std::strcmp(n,"menu_MainMenu")){g.last_open_menu=1;g.in_game_menu=0;}
 else {
  const char* names[]={"menu_StartGame","menu_Options","menu_info","menu_HelpButtons","menu_hud_confirm","menu_Loading","menu_FadeFromBlackScreen","menu_Ingame","menu_splash","menu_About","menu_Help","menu_EnterName","menu_SelectClass","menu_playlist","menu_confirm","menu_confirm2"};
  const int ids[]={2,3,4,g.in_game_menu?5:6,7,8,9,10,11,15,14,16,17,18,19,19};
  int id=13;for(unsigned i=0;i<16;++i)if(!std::strcmp(n,names[i])){id=ids[i];break;}
  const char* ingame[]={"menu_CharacterMenu","menu_CharacterSheetNew","menu_CharacterSheetStats","menu_InventorySheetMain","menu_SkillTreeSheetNew","menu_FaerySheet","menu_QuestLogSheetNEW","menu_MapSheet"};
  for(auto x:ingame)if(!std::strcmp(n,x)){id=12;break;}
  g.last_open_menu=id;if(id==10)g.in_game_menu=1;
 }
}
int popglobals(MenuStackV1*s,const MenuStackServicesV1*v){
 auto&g=*s->globals;
 int&x=g.last_open_menu;
 if(g.in_game_menu){if(x==7)x=10;else if(x!=10&&x!=5&&x!=14)x=9;int e=call(s,v,MenuStackOperationV1::debug_message,nullptr,nullptr,nullptr,"pop",0x438c70);if(e)return e;}
 if(!g.back_pressed){if(x==4)x=1;else if(x==6)x=g.in_game_menu?10:4;else if(x==14)x=g.in_game_menu?5:6;else if(x==3)x=g.in_game_menu?10:1;else if(x==5)x=10;else if(x==12)x=9;else if(x==15)x=4;int e=call(s,v,MenuStackOperationV1::debug_message,nullptr,nullptr,nullptr,"pop",0x438ce4);if(e)return e;}
 g.back_pressed=0;return 0;
}
int resume(MenuStackV1*s,const MenuStackServicesV1*v,MenuStackRenderV1*r,bool named_remove=false,std::int32_t removed_index=-1){
 auto m=top(r);if(!m)return 0;int e=visibility(m);if(e)return e;
 if(r->flags&8){auto base=top(s->base_render);if(!base)return -3;auto bc=weak(base->character);if(!bc)return -3;if(bc->type_is_two){auto current=top(r);if(!current)return -3;auto cc=weak(current->character);if(!cc)return -3;cc->focus_enabled=1;}}
 m=top(r);if(!m)return -3;context(r,m);
 if(!(r->flags&0x40)){
  if(!named_remove||removed_index==static_cast<std::int32_t>(r->count)){
   auto asmenu=m;
   if(named_remove){if(removed_index<=0||static_cast<std::uint32_t>(removed_index-1)>=r->count)return -3;asmenu=r->states[removed_index-1];}
   e=call(s,v,MenuStackOperationV1::invoke_as,r,asmenu,nullptr,"OnShow");if(e)return e;
   m=top(r);if(!m)return -3;std::uintptr_t result=0;e=call(s,v,MenuStackOperationV1::play_animation,r,m,weak(m->character),"focus_in",0,&result);if(e)return e;
   if(!result){m=top(r);if(!m)return -3;e=call(s,v,MenuStackOperationV1::play_animation,r,m,weak(m->character),"show");if(e)return e;}
  }
 }
 if(r->flags&1){e=restorefocus(s,v,r);if(e)return e;}
 m=top(r);if(!m)return -3;e=call(s,v,MenuStackOperationV1::menu_focus,r,m);if(e)return e;m=top(r);if(!m)return -3;m->status=3;return 0;
}
int hidecurrent(MenuStackV1*s,const MenuStackServicesV1*v,MenuStackRenderV1*r,std::int32_t index=-1){
 auto get=[&](){return index<0?top(r):(static_cast<std::uint32_t>(index)<r->capacity?r->states[index]:nullptr);};auto m=get();if(!m)return -3;
 int e=call(s,v,MenuStackOperationV1::menu_blur,r,m);if(e)return e;m=get();if(!m)return -3;e=call(s,v,MenuStackOperationV1::menu_hide,r,m);if(e)return e;m=get();if(!m)return -3;e=call(s,v,MenuStackOperationV1::invoke_as,r,m,nullptr,"OnHide");if(e)return e;
 if(!(r->flags&0x40)){m=get();if(!m)return -3;e=call(s,v,MenuStackOperationV1::play_animation,r,m,weak(m->character),"hide");if(e)return e;}
 m=get();if(!m)return -3;m->status=2;
 if(r->flags&8){e=focusflag(m,0);if(e)return e;}
 r->context=r->root;return 0;
}
}
extern "C" int dh2_menu_stack_find_v1(const MenuStackV1*s,const char*n,MenuStackMenuV1**out){if(!valid(s)||!n||!out)return -1;*out=find(s,n);return 0;}
extern "C" int dh2_menu_stack_contains_v1(const MenuStackV1*s,const MenuStackMenuV1*m,std::uint32_t*out){if(!valid(s)||!out)return -1;*out=contains(s,m);return 0;}
extern "C" int dh2_menu_stack_push_v1(MenuStackV1*s,MenuStackMenuV1*requested,const MenuStackServicesV1*v){
 if(!v||!valid(s,v)||!requested||!requested->render||!requested->name)return -1;
 auto r=requested->render;MenuStackMenuV1*m=nullptr;for(std::uint32_t i=0;i<r->catalog_count;++i)if(!std::strcmp(requested->name,r->catalog[i]->name)){m=r->catalog[i];break;}if(!m)return 0;
 int e=call(s,v,MenuStackOperationV1::debug_message,r,m,nullptr,"push",0x4382b4);if(e)return e;
 const char*n=m->name;
 if(!std::strcmp(n,"menu_MultiLogin")){if(s->globals->use_native_drm){e=call(s,v,MenuStackOperationV1::license_check);if(e)return e;}e=call(s,v,MenuStackOperationV1::debug_message,nullptr,nullptr,nullptr,"multilogin",0x438530);if(e)return e;s->globals->multiplayer=1;s->globals->multiplayer_igm=1;}
 if(!std::strcmp(n,"menu_MultiplayerConnectivity")){e=call(s,v,MenuStackOperationV1::debug_message,nullptr,nullptr,nullptr,"multiplayer_connectivity",0x4385b8);if(e)return e;s->globals->multiplayer=1;s->globals->multiplayer_igm=1;}
 if(!std::strcmp(n,"menu_MainMenu")){e=call(s,v,MenuStackOperationV1::debug_message,nullptr,nullptr,nullptr,"main_menu",0x43858c);if(e)return e;s->globals->multiplayer=1;s->globals->multiplayer_igm=0;}
 classify(*s->globals,n);
 if(!std::strcmp(n,"menu_confirm")||!std::strcmp(n,"menu_confirm2")||!std::strcmp(n,"menu_playlist")){auto site=!std::strcmp(n,"menu_confirm")?0x438998u:!std::strcmp(n,"menu_confirm2")?0x438a08u:0x4389e8u;e=call(s,v,MenuStackOperationV1::debug_message,nullptr,nullptr,nullptr,"classification",site);if(e)return e;}
 s->globals->back_pressed=0;
 MenuStackCharacterV1**newchar=&m->character;
 if(s->count){auto oldr=s->renders[s->count-1];if(top(oldr)){
  auto oldm=top(oldr);e=call(s,v,MenuStackOperationV1::menu_blur,oldr,oldm);if(e)return e;
  e=call(s,v,MenuStackOperationV1::invoke_as,oldr,oldm,nullptr,"OnHide");if(e)return e;
  if(!(oldr->flags&0x40)){std::uintptr_t result=0;e=call(s,v,MenuStackOperationV1::play_animation,oldr,oldm,weak(oldm->character),"focus_out",0,&result);if(e)return e;
   if(result)oldm->status=4;else{e=call(s,v,MenuStackOperationV1::play_animation,oldr,oldm,weak(oldm->character),"hide",0,&result);if(e)return e;if(result)oldm->status=2;}
  }
  oldm->saved_focus=oldr->controller_focus;
  if(oldr->flags&8){auto c=weak(*newchar);if(!c)return -3;if(c->type_is_two){e=focusflag(oldm,0);if(e)return e;}}
 }}
 // Source captures count, then appends an occurrence even for the same RenderFX.
 if(s->count==s->capacity||r->count==r->capacity)return -4;
 s->renders[s->count++]=r;r->states[r->count++]=m;
 e=visibility(m);if(e)return e;if(r->flags&8){e=focusflag(m,1);if(e)return e;}
 context(r,m);e=call(s,v,MenuStackOperationV1::invoke_as,r,m,nullptr,"OnShow");if(e)return e;
 if(!(r->flags&0x40)){e=call(s,v,MenuStackOperationV1::play_animation,r,m,weak(*newchar),"show");if(e)return e;}
 if(r->flags&1){e=call(s,v,MenuStackOperationV1::render_reset,r);if(e)return e;}
 e=call(s,v,MenuStackOperationV1::menu_show,r,m);if(e)return e;e=call(s,v,MenuStackOperationV1::menu_focus,r,m);if(e)return e;m->status=1;return 0;
}
extern "C" int dh2_menu_stack_pop_v1(MenuStackV1*s,std::uint32_t all,const MenuStackServicesV1*v){
 if(!v||!valid(s,v)||all>1)return -1;int e=popglobals(s,v);if(e)return e;
 do {if(!s->count)return 0;auto r=s->renders[s->count-1];if(!top(r))return 0;e=hidecurrent(s,v,r);if(e)return e;if(!r->count||!s->count)return -3;--r->count;--s->count;if(s->count){e=resume(s,v,s->renders[s->count-1]);if(e)return e;}}while(all&&s->count&&top(s->renders[s->count-1]));return 0;
}
extern "C" int dh2_menu_stack_pop_name_v1(MenuStackV1*s,const char*n,std::uint32_t above,const MenuStackServicesV1*v){
 if(!v||!valid(s,v)||!n||above>1)return -1;
 if(above){while(true){auto target=find(s,n);if(!contains(s,target)||!s->count)return 0;auto r=s->renders[s->count-1];if(!top(r)||top(r)==find(s,n))return 0;int e=hidecurrent(s,v,r);if(e)return e;if(!r->count||!s->count)return -3;--r->count;--s->count;if(s->count){e=resume(s,v,s->renders[s->count-1]);if(e)return e;}}}
 auto target=find(s,n);
 for(std::int32_t i=static_cast<std::int32_t>(s->count)-1;i>=0;--i){if(static_cast<std::uint32_t>(i)>=s->capacity)return -3;auto r=s->renders[i];
  for(std::int32_t j=static_cast<std::int32_t>(r->count)-1;j>=0;--j){if(static_cast<std::uint32_t>(i)>=s->capacity)return -3;r=s->renders[i];if(static_cast<std::uint32_t>(j)>=r->capacity)return -3;if(r->states[j]!=target)continue;
   int e=hidecurrent(s,v,r,j);if(e)return e;if(static_cast<std::uint32_t>(j)>=r->count||static_cast<std::uint32_t>(i)>=s->count)return -3;
   for(std::uint32_t k=j+1;k<r->count;++k)r->states[k-1]=r->states[k];--r->count;
   for(std::uint32_t k=i+1;k<s->count;++k)s->renders[k-1]=s->renders[k];--s->count;
   if(s->count){e=resume(s,v,s->renders[s->count-1],true,j);if(e)return e;}
  }
 }return 0;
}
extern "C" int dh2_menu_stack_manager_push_v1(MenuStackV1*s,MenuStackMenuV1*m,const MenuStackServicesV1*v){
 if(!v||!valid(s,v))return -1;if(!m)return 0;std::uintptr_t valid_menu=0;int e=call(s,v,MenuStackOperationV1::menu_valid,m->render,m,nullptr,nullptr,0,&valid_menu);if(e)return e;
 if(valid_menu&&!contains(s,m)){e=call(s,v,MenuStackOperationV1::reset_touch);if(e)return e;e=call(s,v,MenuStackOperationV1::process_touch);if(e)return e;e=dh2_menu_stack_push_v1(s,m,v);if(e)return e;e=call(s,v,MenuStackOperationV1::debug_load);if(e)return e;e=call(s,v,MenuStackOperationV1::debug_switch,nullptr,nullptr,nullptr,"isTracingMenuManager");if(e)return e;}
 if(m->render==s->hud_root)return call(s,v,MenuStackOperationV1::register_listener,m->render,m);return 0;
}
extern "C" int dh2_menu_stack_manager_pop_v1(MenuStackV1*s,MenuStackMenuV1*m,const MenuStackServicesV1*v){
 if(!v||!valid(s,v)||!m)return -1;int e=0;if(m->render&&contains(s,m)){e=call(s,v,MenuStackOperationV1::reset_touch);if(e)return e;e=call(s,v,MenuStackOperationV1::process_touch);if(e)return e;e=dh2_menu_stack_pop_v1(s,0,v);if(e)return e;}
 if(m->render==s->hud_root)return call(s,v,MenuStackOperationV1::unregister_listener,m->render,m);return 0;
}
extern "C" int dh2_menu_stack_hide_all_v1(MenuStackV1*s,const MenuStackServicesV1*v){
 if(!v||!valid(s,v))return -1;const auto n=s->registry_count;for(std::uint32_t i=0;i<n;++i){if(i>=s->registry_count)return -3;auto m=s->registry[i];if(m->visible){m=s->registry[i];int e=call(s,v,MenuStackOperationV1::menu_set_visible,m->render,m,nullptr,nullptr,0);if(e)return e;}}return 0;
}
extern "C" int dh2_menu_stack_native_v1(MenuStackV1*s,std::uint32_t op,std::uint32_t argc,std::uint32_t type,const char*n,const MenuStackServicesV1*v){
 if(!v||!valid(s,v)||op>3)return -1;
 if(op==0){if(!argc||!n)return -1;return dh2_menu_stack_manager_push_v1(s,find(s,n),v);}
 if(op==1){if(!argc)return dh2_menu_stack_pop_v1(s,0,v);if(!n)return -1;auto m=find(s,n);return m?dh2_menu_stack_manager_pop_v1(s,m,v):0;}
 if(op==2){if(argc!=1||(type!=3&&type!=4))return 0;if(!n)return -1;return dh2_menu_stack_pop_name_v1(s,n,1,v);}
 return dh2_menu_stack_pop_v1(s,1,v);
}
static_assert(sizeof(MenuStackCharacterV1)==24&&sizeof(MenuStackMenuV1)==56&&sizeof(MenuStackRenderV1)==72&&sizeof(MenuStackV1)==56&&sizeof(MenuStackRequestV1)==48,"native64 menu projection ABI");
