#include "menu_stack_owner_v1.hpp"
#include <vector>
#include <algorithm>
#include <stdexcept>
namespace dh2::ui {
struct MenuStackOwnerV1::Impl {
 struct Render{MenuStackRenderV1 value{};std::vector<MenuStackMenuV1*> states,catalog;};
 struct Menu{MenuStackMenuV1 value{};std::string name;};
 std::uint32_t capacity;bool sealed=false;MenuStackV1 state{};
 std::vector<std::unique_ptr<Render>>renders;std::vector<std::unique_ptr<Menu>>menus;
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
 auto m=std::make_unique<Impl::Menu>();m->name=name;m->value.identity=id;m->value.render=&r->value;m->value.name=m->name.c_str();m->value.character=character;m->value.valid_menu=valid_menu;m->value.visible=visible;r->catalog.push_back(&m->value);p_->registry.push_back(&m->value);p_->menus.push_back(std::move(m));return true;
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
MenuStackMenuV1*MenuStackOwnerV1::menu(const char*n)noexcept{MenuStackMenuV1*m=nullptr;if(view()&&n)dh2_menu_stack_find_v1(view(),n,&m);return m;}
int MenuStackOwnerV1::push(const char*n,const MenuStackServicesV1&s){if(!view()||!n)return -1;return dh2_menu_stack_manager_push_v1(view(),menu(n),&s);}
int MenuStackOwnerV1::pop(const char*n,const MenuStackServicesV1&s){if(!view()||!n)return -1;auto m=menu(n);return m?dh2_menu_stack_manager_pop_v1(view(),m,&s):0;}
int MenuStackOwnerV1::pop_current(bool all,const MenuStackServicesV1&s){return dh2_menu_stack_pop_v1(view(),all,&s);}
int MenuStackOwnerV1::pop_above(const char*n,const MenuStackServicesV1&s){return dh2_menu_stack_pop_name_v1(view(),n,1,&s);}
int MenuStackOwnerV1::pop_named_direct(const char*n,const MenuStackServicesV1&s){return dh2_menu_stack_pop_name_v1(view(),n,0,&s);}
int MenuStackOwnerV1::hide_all(const MenuStackServicesV1&s){return dh2_menu_stack_hide_all_v1(view(),&s);}
}
