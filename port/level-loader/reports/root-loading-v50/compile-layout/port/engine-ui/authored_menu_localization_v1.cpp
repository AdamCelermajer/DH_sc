#include "authored_menu_localization_v1.hpp"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_text.h"
#include <algorithm>
#include <exception>
namespace dh2::ui {
bool AuthoredMenuSearchIndexV1::initialize(SwfAsGraph& graph,const SwfAsValue& context,std::string& error){
 gameswf::as_object* object=nullptr;if(!graph.borrow_object(context,object,error))return false;
 if(!object||!object->is(gameswf::character::m_class_id)){error="Actual RenderFX SearchIndex context is not a character";return false;}
 entries_.clear();
 const auto collect=[&](const auto& self,gameswf::character* child)->bool {
  const std::string name=child->get_name().c_str();
  if(!name.empty()){
   std::vector<std::string> segments;
   for(auto* p=child;p;p=p->get_parent())if(p->get_name().length())segments.emplace_back(p->get_name().c_str());
   std::string path;for(auto it=segments.rbegin();it!=segments.rend();++it){if(!path.empty())path+='.';path+=*it;}
   if(path.size()>255){error="Original SearchIndex entry path exceeds its source256 byte domain";return false;}
   SwfAsValue value;if(!graph.retain_object(child,value,error))return false;
   entries_.push_back({name,std::move(path),std::move(value)});
  }
  // CollectCharacters(...,null,4): no visibility/focus gate, includes current
  // named context, then recurses actual sprite display children in their order.
  if(child->is(gameswf::sprite_instance::m_class_id)){
   auto* sprite=static_cast<gameswf::sprite_instance*>(child);
   for(int i=0;i<sprite->m_display_list.size();++i){auto* next=sprite->m_display_list.get_character(i);if(next&&!self(self,next))return false;}
  }
  return true;
 };
 return collect(collect,static_cast<gameswf::character*>(object));
}
bool AuthoredMenuSearchIndexV1::find(const std::string& query,SwfAsValue& out,bool& found,std::string& error)const{
 found=false;const auto last=query.find_last_of('.');const auto key=query.substr(last==std::string::npos?0:last+1);
 for(const auto& entry:entries_){
  if(entry.name!=key)continue;
  std::size_t query_at=0,path_at=0;
  for(;;){
   const auto dot=query.find('.',query_at);const auto segment=query.substr(query_at,dot==std::string::npos?std::string::npos:dot-query_at);
   if(segment.size()>127){error="Source SearchIndex query segment exceeds its source128 byte temporary domain";return false;}
   const auto at=entry.path.find(segment,path_at);if(at==std::string::npos)break;
   path_at=at+segment.size();
   if(path_at==entry.path.size()){out=entry.character;found=true;return true;}
   if(dot==std::string::npos)break;
   query_at=dot+1;
  }
 }
 out=SwfAsValue::null();return true;
}
bool authored_menu_process_localization_v1(SwfAsGraph& graph,AuthoredMenuFieldsV1& menu,const SwfAsValue& context,const AuthoredMenuLocalizationServicesV1& services,std::string& error){
 if(context.kind()==SwfAsValue::Kind::undefined||context.kind()==SwfAsValue::Kind::null_value)return true;
 if(!services.text||!services.debug||!services.set_context){error="Required original menu localization owner unavailable";return false;}
 if(!services.debug("isTracingMenuBase",error))return false;
 AuthoredMenuSearchIndexV1 index;
 for(const std::uint32_t sheet:{19u,20u,28u}){
  auto& text=*services.text;std::string symbol;
  if(!text.source_string_index_v1(sheet,0,text.source_symbol_pack_v1(),services.strings,symbol,error))return false;
  if(!services.set_context(context,error)||!index.initialize(graph,context,error))return false;
  for(std::uint32_t row=0;;++row){
   std::int32_t count=0;if(!text.source_string_count_v1(sheet,text.source_symbol_pack_v1(),count,error))return false;
   if(static_cast<std::int32_t>(row)>=count)break;
   if(!text.source_string_index_v1(sheet,row,text.source_symbol_pack_v1(),services.strings,symbol,error))return false;
   SwfAsValue receiver;bool found=false;
   for(const auto& path:{menu.name+".btn_"+symbol+".text",menu.name+"."+symbol+".text",menu.name+"."+symbol}){
    if(!index.find(path,receiver,found,error))return false;
    if(found)break;
   }
   if(!found)continue;
   gameswf::as_object* object=nullptr;if(!graph.borrow_object(receiver,object,error))return false;
   // First found non-edittext stops all fallback, exactly source Is(32).
   if(!object||!object->is(gameswf::edit_text_character::m_class_id))continue;
   if(!services.debug("isTracingMenuBase",error))return false;
   std::string localized;if(!text.source_string_index_v1(sheet,row,text.pack(),services.strings,localized,error))return false;
   try{
    // FormatHTML("%s",localized,1) => RenderFX.SetText(...,true) => genuine
    // edit_text_character::set_text_value(...,html=true), including variable
    // publication/font/layout; no generated Android widget or plain-text shim.
    static_cast<gameswf::edit_text_character*>(object)->source_set_text_v1(tu_string(localized.c_str()),true,true);
   }catch(const std::exception& e){error=e.what();return false;}
  }
 }
 menu.localized75=1;return true;
}
}
