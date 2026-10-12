#include "renderfx_text_connection.hpp"
#include "gameswf/gameswf_text.h"
#include "gameswf/gameswf_font.h"
#include "gameswf/gameswf_environment.h"
#include <algorithm>
#include <exception>
namespace dh2::ui {
namespace {
void format_plain(gameswf::edit_text_character& field){
 // Explicit per-call plain formatting. Never toggle shared movie-definition
 // m_html: other instances and reentrant font callbacks see their own state.
 field.m_text_glyph_records.resize(0);
 field.m_x=field.m_y=field.m_xcursor=field.m_ycursor=0;
 field.reset_bounding_box(0,0);
 if(!field.m_font)return;
 gameswf::text_glyph_record record;
 record.m_style.m_scale=field.m_text_height/1024.0f;
 if(field.m_font->is_define_font3())record.m_style.m_scale/=20.0f;
 record.m_style.m_leading=field.m_leading+field.m_font->get_leading()*record.m_style.m_scale;
 record.m_style.m_font=field.m_font.get_ptr();record.m_style.m_color=field.m_color;
 record.m_style.m_x_offset=std::max(0.0f,field.m_left_margin+field.m_indent)+field.m_def->m_rect.m_x_min;
 record.m_style.m_y_offset=field.m_text_height+(field.m_font->get_leading()-field.m_font->get_descent())*record.m_style.m_scale+field.m_def->m_rect.m_y_min;
 record.m_style.m_text_height=field.m_text_height;
 record.m_style.m_has_x_offset=record.m_style.m_has_y_offset=true;
 field.m_x=record.m_style.m_x_offset;field.m_y=record.m_style.m_y_offset;
 field.format_plain_text(field.m_text,record);
}
}
bool renderfx_set_plain_text(SwfAsGraph& graph,const SwfAsValue& receiver,const char* input,bool parse,std::string& error){
 gameswf::as_object* object=nullptr;
 if(!graph.borrow_object(receiver,object,error))return false;
 if(!object||!object->is(gameswf::edit_text_character::m_class_id)){error.clear();return true;}
 if(parse){error="Required source HUD HTML text reader unavailable";return false;}
 if(!input){error="Required HUD text bytes unavailable";return false;}
 try{
  gameswf::gc_ptr<gameswf::edit_text_character> field=static_cast<gameswf::edit_text_character*>(object);
  if(!field->m_def){error="Required HUD text definition unavailable";return false;}
  const tu_string text(input);
  if(field->m_text!=text){
   field->m_text=text;
   const auto maximum=field->m_def->m_max_length;
   if(maximum>0&&field->m_text.length()>maximum)field->m_text.resize(maximum);
   format_plain(*field);
  }
  // Original set_text_value writes the UNTRUNCATED supplied string to its
  // optional bound variable even when the displayed text was already equal.
  if(field->get_var_name().size()>0){
   gameswf::gc_ptr<gameswf::as_object> parent=field->get_parent();
   tu_string path,variable=field->get_var_name();
   if(gameswf::as_environment::parse_path(field->get_var_name(),&path,&variable))
    parent=parent?parent->find_target(path.c_str()):nullptr;
   if(parent)parent->set_member(variable,text.c_str());
  }
  error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
}
