#include "authored_menu_weak_character_v59.hpp"
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wself-assign"
#pragma clang diagnostic ignored "-Wdeprecated-copy-with-user-provided-copy"
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#pragma clang diagnostic ignored "-Wmismatched-tags"
#pragma clang diagnostic ignored "-Wnew-returns-null"
#pragma clang diagnostic ignored "-Wignored-qualifiers"
#endif
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_sprite.h"
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#include <cstring>
namespace dh2::ui {
struct AuthoredMenuWeakCharacterV59::Impl {
 weak_ptr<gameswf::character> character;
 std::weak_ptr<void> registered_graph;
 bool bound{};
};
AuthoredMenuWeakCharacterV59::AuthoredMenuWeakCharacterV59():impl_(std::make_unique<Impl>()){}
AuthoredMenuWeakCharacterV59::~AuthoredMenuWeakCharacterV59()=default;
bool AuthoredMenuWeakCharacterV59::bind(SwfAsGraph& graph,const SwfAsValue& value,std::string& error){
 if(impl_->bound){error="MenuBase weak character already bound by RegisterState";return false;}
 return source_assign_v67(graph,value,error);
}
bool AuthoredMenuWeakCharacterV59::source_assign_v67(SwfAsGraph& graph,const SwfAsValue& value,std::string& error){
 gameswf::as_object* object{};
 if(!graph.borrow_object(value,object,error))return false;
 if(!object||!object->is(gameswf::character::m_class_id)){
  error="Required actual registered MenuBase character";return false;
 }
 std::shared_ptr<void> token;if(!graph.scope_identity_v59(token,error))return false;
 impl_->character=static_cast<gameswf::character*>(object);impl_->registered_graph=token;impl_->bound=true;return true;
}
bool AuthoredMenuWeakCharacterV59::borrow(SwfAsGraph& graph,SwfAsValue& out,bool& live,std::string& error){
 out={};live=false;
 if(!impl_->bound){error="Required original RegisterState weak-character producer";return false;}
 std::shared_ptr<void> token;if(!graph.scope_identity_v59(token,error))return false;
 const auto registered=impl_->registered_graph.lock();
 if(!registered||registered.get()!=token.get()||registered.owner_before(token)||token.owner_before(registered)){
  error="MenuBase weak character belongs to another/expired actual movie generation";return false;
 }
 // get_ptr performs the original dead-proxy release/clear before returning0.
 auto* character=impl_->character.get_ptr();
 if(!character)return true;
 if(!graph.retain_object(character,out,error))return false;
 live=true;return true;
}
bool AuthoredMenuWeakCharacterV59::source_assign_null_v68(SwfAsGraph& graph,std::string& e){
 std::shared_ptr<void> token;if(!graph.scope_identity_v59(token,e))return false;
 impl_->character=nullptr;impl_->registered_graph=token;impl_->bound=true;return true;
}
bool AuthoredMenuWeakCharacterV59::source_get_char_v68(SwfAsGraph& graph,SwfAsValue& out,bool& live,std::string& e){
 if(!impl_->bound){out={};live=false;return true;} //actual cache C1 NULL
 return borrow(graph,out,live,e);
}
bool AuthoredMenuWeakCharacterV59::update(SwfAsGraph& graph,AuthoredMenuFieldsV1& fields,std::string& error){
 SwfAsValue value;bool live{};
 if(!borrow(graph,value,live,error))return false;
 if(!live)return true; // IsAnimOver(NULL) returnsfalse, leaving counter intact.
 gameswf::as_object* object{};
 if(!graph.borrow_object(value,object,error))return false;
 if(!object||!object->is(gameswf::sprite_instance::m_class_id))return true;
 auto* character=static_cast<gameswf::character*>(object);
 // Original virtual sequence calls frame/count twice; no STOP test or movie
 // advance belongs to IsAnimOver. Keep the same getter call order.
 (void)character->get_current_frame();(void)character->get_frame_count();
 const auto current=character->get_current_frame();
 const auto count=static_cast<std::uint32_t>(character->get_frame_count());
 const auto last_bits=count-1u;std::int32_t last{};std::memcpy(&last,&last_bits,sizeof(last));
 if(current>=last)++fields.counter78;
 return true;
}
bool AuthoredMenuWeakCharacterV59::source_live_v114()noexcept{return impl_->character.get_ptr()!=nullptr;}
void AuthoredMenuWeakCharacterV59::reset()noexcept{impl_->character=nullptr;}
}
