#include "authored_menu_drag_v68.hpp"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_sprite.h"
#include <optional>
namespace dh2::ui {
struct AuthoredMenuDragV68::Impl {
 struct Drag {
  std::uintptr_t menu_fx{};weak_ptr<gameswf::character> character;
  gameswf::rect receiver_pixels;std::uint32_t mode1c{},depth38{};
  AuthoredMenuDeadZoneV3 limits20;std::int32_t initial_x30{},initial_y34{};
  gameswf::matrix initial_matrix3c;
 };
 struct Drop {
  std::uintptr_t menu_fx{};weak_ptr<gameswf::character> character;gameswf::rect receiver_twips;
  //411e88 does NOT initialize words4/8. Keep them unpublished until their
  //actual TestDragable producer, instead of inventing a hovered/default row.
  std::optional<std::uint32_t> word4;std::optional<std::uint8_t> byte8;
 };
 Drag* selected0{};std::vector<Drag> drags4;std::vector<Drop> drops10;
};
namespace {
bool character(SwfAsGraph& graph,std::uintptr_t identity,gameswf::character*& out,std::string& e){
 SwfAsValue value;gameswf::as_object* object{};
 if(!identity||!graph.retain_object(reinterpret_cast<gameswf::as_object*>(identity),value,e)||!graph.borrow_object(value,object,e))return false;
 if(!object||!object->is(gameswf::character::m_class_id)){e="DragAndDrop requires actual scoped movie character";return false;}
 out=static_cast<gameswf::character*>(object);return true;
}
}
AuthoredMenuDragV68::AuthoredMenuDragV68():impl_(std::make_unique<Impl>()){}
AuthoredMenuDragV68::~AuthoredMenuDragV68()=default;
bool AuthoredMenuDragV68::add_drag(SwfAsGraph& graph,std::uintptr_t fx,AuthoredMenuCharacterBorrowV3& q,
 AuthoredMenuCharacterBorrowV3* limit,AuthoredMenuCharacterProjectionV4& projection,const Dimensions& dimensions,std::string& e){
 gameswf::character* actual{};if(!fx||!character(graph,q.identity,actual,e))return false;
 Impl::Drag row;row.menu_fx=fx;row.character=actual;actual->get_bound(&row.receiver_pixels);row.receiver_pixels.twips_to_pixels();
 row.depth38=static_cast<std::uint16_t>(actual->get_depth());
 if(limit){if(!projection.absolute_bounds(*limit,row.limits20,e))return false;}
 else {std::int32_t width{},height{};if(!dimensions||!dimensions(width,height,e)||width<=0||height<=0){if(e.empty())e="Dragable C1 needs actual renderer screen dimensions";return false;}
  row.limits20={0.f,static_cast<float>(width)*20.f,0.f,static_cast<float>(height)*20.f};}
 const auto& matrix=actual->get_matrix();row.initial_x30=static_cast<std::int32_t>(matrix.m_[0][2]);row.initial_y34=static_cast<std::int32_t>(matrix.m_[1][2]);row.initial_matrix3c=matrix;
 impl_->drags4.push_back(std::move(row));return true;
}
bool AuthoredMenuDragV68::add_drop(SwfAsGraph& graph,std::uintptr_t fx,AuthoredMenuCharacterBorrowV3& q,std::string& e){
 gameswf::character* actual{};if(!fx||!character(graph,q.identity,actual,e))return false;
 Impl::Drop row;row.menu_fx=fx;row.character=actual;actual->get_bound(&row.receiver_twips);impl_->drops10.push_back(std::move(row));return true;
}
bool AuthoredMenuDragV68::reset_positions(SwfAsGraph& graph,std::string& e){
 //412aa8 copies each row before ResetPosition; reset callbacks must not hold
 //a reference into the vector. selected0 is cleared only after the walk.
 for(auto row:impl_->drags4){auto* actual=row.character.get_ptr();if(!actual){e="Required live original Dragable.ResetPosition receiver";return false;}
  gameswf::character* scoped{};if(!character(graph,reinterpret_cast<std::uintptr_t>(actual),scoped,e))return false;
  scoped->set_matrix(row.initial_matrix3c);
  if(auto* parent=scoped->get_parent())parent->set_display_callback(nullptr,nullptr);
  scoped->set_display_callback(nullptr,nullptr);
  if(scoped->is(gameswf::sprite_instance::m_class_id)&&scoped->get_depth()!=static_cast<int>(row.depth38)){
   auto* parent=scoped->get_parent();if(!parent||!parent->is(gameswf::sprite_instance::m_class_id)){e="Dragable depth reset needs actual parent display list";return false;}
   static_cast<gameswf::sprite_instance*>(parent)->m_display_list.change_character_depth(scoped,static_cast<int>(row.depth38));
  }
 }
 impl_->selected0=nullptr;return true;
}
bool AuthoredMenuDragV68::empty()const noexcept{return impl_->drags4.empty()&&impl_->drops10.empty();}
bool AuthoredMenuDragV68::event(SwfEvent48& event,std::string& e){
 if(event.kind<5||event.kind>7)return true; //4130e0 unsigned range branch
 auto* previous=impl_->selected0;impl_->selected0=nullptr;
 for(auto& row:impl_->drags4)if(event.character==reinterpret_cast<std::uintptr_t>(row.character.get_ptr())){
  e="Required whole Dragable.OnEvent412b00 display callbacks/drop invocation";return false;
 }
 if(previous){e="Required retained selected Dragable/TestDropable event continuation";return false;}
 return true; //every actual Dragable returned literal false on identity miss
}
}
