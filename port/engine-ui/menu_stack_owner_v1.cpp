#include "menu_stack_owner_v1.hpp"
#include <vector>
#include <algorithm>
#include <stdexcept>
namespace dh2::ui {
struct MenuStackOwnerV1::Impl {
 struct Render{MenuStackRenderV1 value{};std::vector<MenuStackMenuV1*> states,catalog;};
 struct Menu{MenuStackMenuV1 value{};std::string name;std::uintptr_t resource{};};
 std::uint32_t capacity;bool sealed=false;MenuStackV1 state{};
 std::vector<std::shared_ptr<Render>>renders;std::vector<std::shared_ptr<Menu>>menus;
 std::vector<MenuStackRenderV1*>stack;std::vector<MenuStackMenuV1*>registry;
 explicit Impl(std::uint32_t n):capacity(n),stack(n,nullptr){}
 Render*render(std::uintptr_t id){for(auto&r:renders)if(r->value.identity==id)return r.get();return nullptr;}
};
MenuStackOwnerV1::MenuStackOwnerV1(std::uint32_t n){if(!n)throw std::invalid_argument("menu stack occurrence budget is zero");p_=std::make_unique<Impl>(n);}
MenuStackOwnerV1::~MenuStackOwnerV1()=default;
bool MenuStackOwnerV1::add_render(std::uintptr_t id,std::uint32_t flags,MenuStackCharacterV1*root,MenuStackCharacterV1*focus,std::string&e){
 if(p_->sealed||!id||p_->render(id)){e="invalid or duplicate menu RenderFX identity";return false;}
 auto r=std::make_unique<Impl::Render>();r->states.resize(p_->capacity);r->value.identity=id;r->value.flags=flags;r->value.root=root;r->value.controller_focus=focus;p_->renders.push_back(std::move(r));return true;
}
bool MenuStackOwnerV1::add_menu(std::uintptr_t id,std::uintptr_t rid,const std::string&name,MenuStackCharacterV1*character,std::uint32_t valid_menu,std::uint32_t visible,std::string&e){
 auto r=p_->render(rid);if(p_->sealed||!id||!r||name.empty()||name.find('\0')!=std::string::npos||valid_menu>1||visible>1||std::any_of(p_->menus.begin(),p_->menus.end(),[&](const auto&m){return m->value.identity==id;})){e="invalid menu registration";return false;}
 auto m=std::make_unique<Impl::Menu>();m->resource=rid;m->name=name;m->value.identity=id;m->value.render=&r->value;m->value.name=m->name.c_str();m->value.character=character;m->value.valid_menu=valid_menu;m->value.visible=visible;r->catalog.push_back(&m->value);p_->registry.push_back(&m->value);p_->menus.push_back(std::move(m));return true;
}
bool MenuStackOwnerV1::seal(std::uintptr_t hid,std::uintptr_t bid,MenuStackGlobalsV1&g,std::string&e){
 auto h=p_->render(hid),b=p_->render(bid);if(p_->sealed||!h||!b||g.reserved){e="invalid menu stack publication";return false;}
 for(auto&r:p_->renders){r->value.states=r->states.data();r->value.capacity=p_->capacity;r->value.catalog=r->catalog.data();r->value.catalog_count=static_cast<std::uint32_t>(r->catalog.size());}
 p_->state={p_->stack.data(),0,p_->capacity,p_->registry.data(),static_cast<std::uint32_t>(p_->registry.size()),0,&b->value,&h->value,&g};p_->sealed=true;return true;
}
bool MenuStackOwnerV1::register_render_live_v27(std::uintptr_t id,std::uint32_t flags,MenuStackCharacterV1* root,MenuStackCharacterV1* focus,std::string& e){
 const bool sealed=p_->sealed;struct Restore {bool& field;bool value;~Restore(){field=value;}} restore{p_->sealed,sealed};p_->sealed=false;
 const bool okay=add_render(id,flags,root,focus,e);
 if(okay&&sealed){auto* r=p_->renders.back().get();r->value.states=r->states.data();r->value.capacity=p_->capacity;}
 return okay;
}
bool MenuStackOwnerV1::register_menu_live_v27(std::uintptr_t id,std::uintptr_t render,const std::string& name,MenuStackCharacterV1* character,std::uint32_t valid,std::uint32_t visible,std::string& e){
 const bool sealed=p_->sealed;struct Restore {bool& field;bool value;~Restore(){field=value;}} restore{p_->sealed,sealed};p_->sealed=false;
 const bool okay=add_menu(id,render,name,character,valid,visible,e);
 if(okay&&sealed){auto* r=p_->render(render);r->value.catalog=r->catalog.data();r->value.catalog_count=static_cast<std::uint32_t>(r->catalog.size());
  p_->state.registry=p_->registry.data();p_->state.registry_count=static_cast<std::uint32_t>(p_->registry.size());}
 return okay;
}
MenuStackV1*MenuStackOwnerV1::view()noexcept{return p_->sealed?&p_->state:nullptr;}
MenuStackRenderV1*MenuStackOwnerV1::render(std::uintptr_t id)noexcept{auto* r=p_->render(id);return r?&r->value:nullptr;}
MenuStackMenuV1*MenuStackOwnerV1::registered_menu_v27(std::uintptr_t id)noexcept{for(auto* m:p_->registry)if(m->identity==id)return m;return nullptr;}
std::uint32_t MenuStackOwnerV1::registry_count_v58()const noexcept{return static_cast<std::uint32_t>(p_->registry.size());}
MenuStackMenuV1* MenuStackOwnerV1::registry_at_v58(std::uint32_t index)noexcept{return index<p_->registry.size()?p_->registry[index]:nullptr;}
bool MenuStackOwnerV1::erase_registry_v58(std::uint32_t index,std::uintptr_t id,std::string& error){
 if(index>=p_->registry.size()||!p_->registry[index]||p_->registry[index]->identity!=id){
  error="Actual MenuManager directory changed at reached erase position";return false;
 }
 // Copying/removing directory pointers preserves separately retained projection
 // storage. Unowned source MenuBase receivers survive this directory mutation.
 p_->registry.erase(p_->registry.begin()+index);
 if(p_->sealed){p_->state.registry=p_->registry.data();p_->state.registry_count=static_cast<std::uint32_t>(p_->registry.size());}
 return true;
}
bool MenuStackOwnerV1::clear_catalog_104_v91(std::uintptr_t id,std::string& e){
 auto* r=p_->render(id);if(!r){e="Required same owned MenuFX catalog render";return false;}
 r->catalog.clear();r->value.catalog=r->catalog.data();r->value.catalog_count=0;e.clear();return true;
}
bool MenuStackOwnerV1::clear_active_states_114_v91(std::uintptr_t id,std::string& e){
 auto* r=p_->render(id);if(!r){e="Required same owned MenuFX active-state render";return false;}
 // Source resize0 retains backing capacity; stale pointer elements are not
 // dereferenced/deleted. Logical count is the only consumed source field.
 r->value.count=0;e.clear();return true;
}
bool MenuStackOwnerV1::clear_render_fields_v91(std::uintptr_t id,std::string& e){
 auto* r=p_->render(id);if(!r){e="Required same owned RenderFX field commit";return false;}
 r->value.flags=(r->value.flags&0xff000000u)|0xffffffu;r->value.context=nullptr;r->value.controller_focus=nullptr;r->value.root=nullptr;e.clear();return true;
}
MenuStackMenuV1*MenuStackOwnerV1::menu(const char*n)noexcept{MenuStackMenuV1*m=nullptr;if(view()&&n)dh2_menu_stack_find_v1(view(),n,&m);return m;}
bool MenuStackOwnerV1::capture_storage_v101(MenuStackMenuV1* menu,std::shared_ptr<void>& menu_storage,
 std::shared_ptr<void>& render_storage,std::string& error){
 menu_storage.reset();render_storage.reset();
 for(const auto& actual:p_->menus)if(&actual->value==menu){
  menu_storage=actual;
  for(const auto& render:p_->renders)if(&render->value==menu->render){render_storage=render;break;}
  if(menu->render&&!render_storage){menu_storage.reset();error="Captured menu lost SAME render storage";return false;}
  error.clear();return true;
 }
 error="Required actual captured menu storage on SAME manager";return false;
}
bool MenuStackOwnerV1::capture_render_storage_v114(MenuStackRenderV1* expected,std::shared_ptr<void>& pin,std::string& e){
 pin.reset();for(const auto& render:p_->renders)if(&render->value==expected){pin=render;e.clear();return true;}
 e="Required SAME existing MenuFX render allocation";return false;
}
bool MenuStackOwnerV1::resize_render_states_v114(MenuStackRenderV1& expected,std::uint32_t capacity,std::string& e){
 auto* actual=p_->render(expected.identity);
 if(!actual||&actual->value!=&expected||capacity<expected.count||capacity>p_->capacity){e="Actual MenuFX states resize exceeds retained source/native capacity domain";return false;}
 try{actual->states.resize(capacity,nullptr);}catch(const std::exception& x){e=x.what();return false;}
 expected.states=actual->states.data();expected.capacity=capacity;e.clear();return true;
}
bool MenuStackOwnerV1::publish_source_c1_v104(MenuStackGlobalsV1& globals,std::string& e){
 if(p_->sealed||globals.reserved||!p_->renders.empty()||!p_->menus.empty()){
  e="Required original empty MenuManager C1 directory publication";return false;
 }
 p_->state={p_->stack.data(),0,p_->capacity,p_->registry.data(),0,0,nullptr,nullptr,&globals};
 p_->sealed=true;e.clear();return true;
}
bool MenuStackOwnerV1::publish_base_render_v104(std::uintptr_t id,std::string& e){
 auto* actual=p_->render(id);
 if(!p_->sealed||!actual||(p_->state.base_render&&p_->state.base_render!=&actual->value)){
  e="Required actual newly registered base movie slot";return false;
 }
 p_->state.base_render=&actual->value;e.clear();return true;
}
bool MenuStackOwnerV1::remove_active_render_v93(std::uint32_t index,std::uintptr_t expected,std::string& e){
 if(!p_->sealed||p_->state.count>p_->capacity||index>=p_->state.count||
    !p_->stack[index]||p_->stack[index]->identity!=expected){e="Actual MultiMenu active array changed at remove index";return false;}
 for(std::uint32_t i=index+1;i<p_->state.count;++i)p_->stack[i-1]=p_->stack[i];
 --p_->state.count;e.clear();return true;
}
int MenuStackOwnerV1::push(const char*n,const MenuStackServicesV1&s){if(!view()||!n)return -1;return dh2_menu_stack_manager_push_v1(view(),menu(n),&s);}
bool MenuStackOwnerV1::retire_render_resource_v93(std::uintptr_t id,std::string& e){
 auto it=std::find_if(p_->renders.begin(),p_->renders.end(),[id](const auto& r){return r->value.identity==id;});
 if(!p_->sealed||it==p_->renders.end()){e="Required same unloaded render projection";return false;}
 auto* render=&(*it)->value;
 if(render->count||render->catalog_count){e="Render retirement precedes source catalog/state unload";return false;}
 for(std::uint32_t i=0;i<p_->state.count;++i)if(p_->stack[i]==render){e="Render retirement precedes active array removal";return false;}
 for(const auto& m:p_->menus)if(m->resource==id&&
    std::find(p_->registry.begin(),p_->registry.end(),&m->value)!=p_->registry.end()){
  e="Render retirement precedes source registry erase";return false;
 }
 if(p_->state.base_render==render)p_->state.base_render=nullptr;
 if(p_->state.hud_root==render)p_->state.hud_root=nullptr;
 // These are pointer projections, not source MenuBase D0 calls. Their actual
 // owners were already released (or retained if unowned) by UnloadMenu.
 p_->menus.erase(std::remove_if(p_->menus.begin(),p_->menus.end(),[id](const auto& m){return m->resource==id;}),p_->menus.end());
 p_->renders.erase(it);e.clear();return true;
}
bool MenuStackOwnerV1::publish_hud_render_v93(std::uintptr_t id,std::string& e){
 auto* actual=p_->render(id);
 if(!p_->sealed||!actual||(p_->state.hud_root&&p_->state.hud_root!=&actual->value)){
  e="HUD publication requires same newly registered resource after old retirement";return false;
 }
 p_->state.hud_root=&actual->value;e.clear();return true;
}
int MenuStackOwnerV1::pop(const char*n,const MenuStackServicesV1&s){if(!view()||!n)return -1;auto m=menu(n);return m?dh2_menu_stack_manager_pop_v1(view(),m,&s):0;}
int MenuStackOwnerV1::pop_current(bool all,const MenuStackServicesV1&s){return dh2_menu_stack_pop_v1(view(),all,&s);}
int MenuStackOwnerV1::pop_above(const char*n,const MenuStackServicesV1&s){return dh2_menu_stack_pop_name_v1(view(),n,1,&s);}
int MenuStackOwnerV1::pop_named_direct(const char*n,const MenuStackServicesV1&s){return dh2_menu_stack_pop_name_v1(view(),n,0,&s);}
int MenuStackOwnerV1::hide_all(const MenuStackServicesV1&s){return dh2_menu_stack_hide_all_v1(view(),&s);}
}
