#include "authored_menu_character_projection_v4.hpp"
#include "authored_menu_localization_v1.hpp"
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#pragma clang diagnostic ignored "-Wdeprecated-copy"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wself-assign"
#pragma clang diagnostic ignored "-Wnew-returns-null"
#pragma clang diagnostic ignored "-Wignored-qualifiers"
#pragma clang diagnostic ignored "-Wmismatched-tags"
#endif
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_movie_def.h"
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
namespace dh2::ui {
bool authored_menu_movie_rect_v4(SwfAsGraph& graph,float out[4],std::string& error){
 if(!out){error="Required movie rectangle output";return false;}
 SwfAsValue value;gameswf::as_object* object=nullptr;
 if(!graph.root_value(value,error)||!graph.borrow_object(value,object,error))return false;
 if(!object||!object->is(gameswf::sprite_instance::m_class_id)){error="Required actual root sprite movie definition";return false;}
 auto* def=dynamic_cast<gameswf::movie_def_impl*>(static_cast<gameswf::sprite_instance*>(object)->get_movie_definition());
 if(!def){error="Required actual cached movie definition";return false;}
 const auto& rect=def->m_frame_size;out[0]=rect.m_x_min;out[1]=rect.m_x_max;out[2]=rect.m_y_min;out[3]=rect.m_y_max;return true;
}
bool authored_menu_play_animation_v4(SwfAsGraph& graph,const SwfAsValue& value,const char* label,bool& accepted,std::string& error){
 accepted=false;if(!label){error="Required original animation label";return false;}
 gameswf::as_object* object=nullptr;if(!graph.borrow_object(value,object,error))return false;
 if(!object||!object->is(gameswf::sprite_instance::m_class_id))return true;
 auto* sprite=static_cast<gameswf::sprite_instance*>(object);accepted=sprite->goto_labeled_frame(label);
 if(accepted)sprite->set_play_state(gameswf::character::PLAY);
 return true;
}
bool authored_menu_stack_character_v4(gameswf::as_object* object,std::uint32_t flags,
 const std::function<bool(gameswf::as_object*,bool&,std::string&)>& focus,
 MenuStackCharacterV1& out,std::string& error){
 if(!object||!object->is(gameswf::character::m_class_id)){error="Required actual scoped MenuStack character";return false;}
 auto* character=static_cast<gameswf::character*>(object);bool enabled=false;
 if((flags&8)&&(!focus||!focus(object,enabled,error))){if(error.empty())error="Required source Gameloft focus-enabled field";return false;}
 out={reinterpret_cast<std::uintptr_t>(object),1,static_cast<std::uint32_t>(character->get_visible()),
  static_cast<std::uint32_t>(character->is(gameswf::sprite_instance::m_class_id)),static_cast<std::uint32_t>(enabled)};return true;
}
bool AuthoredMenuCharacterProjectionV4::append(gameswf::as_object* object,AuthoredMenuCharacterBorrowV3*& out,std::string& error){
 out=nullptr;if(!object)return true;
 if(!object->is(gameswf::character::m_class_id)){error="Required actual GameSWF display-list character";return false;}
 const auto identity=reinterpret_cast<std::uintptr_t>(object);
 for(const auto& n:nodes_)if(n->identity==identity){error="Cyclic or repeated actual display-list character";return false;}
 auto* character=static_cast<gameswf::character*>(object);
 auto node=std::make_unique<AuthoredMenuCharacterBorrowV3>();out=node.get();
 out->identity=identity;out->name=character->get_name().c_str();out->visible=character->get_visible();out->sprite=character->is(gameswf::sprite_instance::m_class_id);
 nodes_.push_back(std::move(node));
 if(out->sprite){auto* sprite=static_cast<gameswf::sprite_instance*>(character);
  for(int i=0;i<sprite->m_display_list.size();++i){AuthoredMenuCharacterBorrowV3* child=nullptr;
   if(!append(sprite->m_display_list.get_character(i),child,error))return false;
   if(child)out->children.push_back(child);
  }
 }
 return true;
}
bool AuthoredMenuCharacterProjectionV4::root(SwfAsGraph& graph,const std::string& name,AuthoredMenuCharacterBorrowV3*& out,std::string& error){
 nodes_.clear();out=nullptr;SwfAsValue root,value;gameswf::as_object* object=nullptr;
 if(!graph.root_value(root,error)||!graph.find_target(root,("_root."+name).c_str(),value,error)||!graph.borrow_object(value,object,error))return false;
 // MenuBase registration searches the whole display tree, so a live menu
 // need not be a direct child of _root. Resolve the same source name through
 // that index when the direct AS path misses; never pass a null projection
 // into CollectCharacters.
 if(!object){
  AuthoredMenuSearchIndexV1 index;bool found{};
  if(!index.initialize(graph,root,error)||!index.find(name,value,found,error)||!graph.borrow_object(value,object,error))return false;
  if(!found||!object){error="Required actual menu display-list character: "+name;return false;}
 }
 return append(object,out,error);
}
bool AuthoredMenuCharacterProjectionV4::absolute_bounds(AuthoredMenuCharacterBorrowV3& projection,AuthoredMenuDeadZoneV3& out,std::string& error){
 bool borrowed=false;for(const auto& n:nodes_)if(n.get()==&projection){borrowed=true;break;}
 if(!borrowed||!projection.identity){error="Required same scoped display-list bounds receiver";return false;}
 auto* character=reinterpret_cast<gameswf::character*>(projection.identity);
 gameswf::rect bounds;character->get_bound(&bounds);float tx=0,ty=0;
 std::vector<gameswf::character*> ancestors;
 for(auto* parent=character->get_parent();parent;parent=parent->get_parent()){
  for(auto* previous:ancestors)if(previous==parent){error="Cyclic actual GameSWF parent chain";return false;}
  ancestors.push_back(parent);tx+=parent->get_matrix().m_[0][2];ty+=parent->get_matrix().m_[1][2];
 }
 out=authored_menu_absolute_rectangle_v3({bounds.m_x_min,bounds.m_x_max,bounds.m_y_min,bounds.m_y_max},tx,ty);return true;
}
}
