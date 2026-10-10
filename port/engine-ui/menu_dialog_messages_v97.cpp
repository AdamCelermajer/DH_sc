#include "menu_dialog_messages_v97.hpp"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_function.h"
#include <cstring>
#include <exception>
namespace dh2::ui {namespace {
struct Reader {
 const std::vector<std::uint8_t>& bytes;std::size_t at{};std::string& error;
 bool word(std::uint32_t& v){if(at>bytes.size()||bytes.size()-at<4){error="Truncated actual dialogs source word";return false;}
  const auto* p=bytes.data()+at;at+=4;v=std::uint32_t(p[0])|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;return true;}
 bool integer(std::int32_t& v){std::uint32_t w;if(!word(w))return false;std::memcpy(&v,&w,4);return true;}
 bool string(std::string& v){std::uint32_t n;if(!word(n))return false;if(n>bytes.size()-at){error="Truncated actual dialogs CString";return false;}
  v.assign(reinterpret_cast<const char*>(bytes.data()+at),n);at+=n;if(!v.empty()&&v.back()=='\0')v.pop_back();return true;}
};
}
bool DialogActorsV97::decode(const std::vector<std::uint8_t>& records,const std::vector<std::uint8_t>& names,
 const std::vector<std::uint8_t>& schema,std::string& e){
 if(initialized_){e="Actual DialogActors table already published; cannot replace live rows";return false;}
 if(records.size()>8u*1024u*1024u||names.size()>8u*1024u*1024u||schema.size()>1024u*1024u){e="Dialog actors exceed finite native cache transport budget";return false;}
 try{
  Reader r{records,0,e},n{names,0,e},s{schema,0,e};std::uint32_t fields{};
  if(!s.word(fields)||fields!=3)return false;
  for(const auto* expected:{"FrameID","FrameLabel","Name"}){std::string got;if(!s.string(got))return false;if(got!=expected){e="Original DialogActors field order differs";return false;}}
  std::uint32_t count{},named{};if(!r.word(count)||!n.word(named))return false;
  if(count!=named||count>65536||count>(records.size()-r.at)/12||count>(names.size()-n.at)/4){e="Original DialogActors count/span mismatch";return false;}
  std::vector<DialogActorRowV97> rows;rows.reserve(count);
  for(std::uint32_t i=0;i<count;++i){DialogActorRowV97 row;
   if(!r.integer(row.frame_id4)||!r.string(row.frame_label8)||!r.integer(row.name10)||!n.string(row.name))return false;
   rows.push_back(std::move(row));
  }
  rows_=std::move(rows);initialized_=true;e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return false;}
}
const DialogActorRowV97* DialogActorsV97::row(std::int32_t index)const noexcept{return initialized_&&index>=0&&std::size_t(index)<rows_.size()?&rows_[std::size_t(index)]:nullptr;}
MenuDialogMessagesV97::MenuDialogMessagesV97(MenuDialogServicesV97 services):services_(std::move(services)){}
namespace {
bool parse_player(const MenuDialogServicesV97& services,const std::string& input,std::string& output,std::string& e){
 if(!services.owner||!services.parse_player_name){e="Required same-campaign ParsePlayerName provider";return false;}
 return services.parse_player_name(input,output,e);
}
}
bool MenuDialogMessagesV97::construct(std::int32_t first,std::int32_t text,std::int32_t style,
 std::int32_t actor,DialogMsgV97& out,std::string& e){
 if(!services_.owner||!services_.localized){e="Required SAME StringManager for DialogMsg C1";return false;}
 std::string title,message;
 if(!services_.localized(first,title,e)||!services_.localized(text,message,e))return false;
 if(!parse_player(services_,title,out.title0,e)||!parse_player(services_,message,out.message18,e))return false;
 out.style30=style;out.actor34.clear(); //4344e0 then source empty CString34
 if(actor>=0){std::int32_t name{};
  if(!services_.actor_text_id||!services_.actor_text_id(actor,name,e)){if(e.empty())e="Required actual DialogActors row/name10";return false;}
  std::string localized;if(!services_.localized(name,localized,e)||!parse_player(services_,localized,out.actor34,e))return false;
 }e.clear();return true;
}
bool MenuDialogMessagesV97::construct_strings_v108(const std::string& title,const std::string& text,std::int32_t style,std::int32_t actor,DialogMsgV97& out,std::string& e){
 //Original strings overload433ea8 uses the same SetActorName branch.
 if(!parse_player(services_,title,out.title0,e)||!parse_player(services_,text,out.message18,e))return false;
 out.style30=style;out.actor34.clear();
 if(actor>=0){std::int32_t name;std::string localized;if(!services_.owner||!services_.actor_text_id||!services_.localized||!services_.actor_text_id(actor,name,e)||!services_.localized(name,localized,e)||!parse_player(services_,localized,out.actor34,e))return false;}
 e.clear();return true;
}
bool MenuDialogMessagesV97::enqueue(const DialogMsgV97& message,std::int32_t context,bool start,std::string& e){
 if(context!=0){e="Dialog context outside source one-queue template";return false;}
 queue_.push_back(message); //460e14 before start/size checks and invocation
 if(start&&queue_.size()==1){
  if(!services_.owner||!services_.invoke||!services_.start_method){e="Required original Dialog.StartDialog invocation";return false;}
  return services_.invoke(services_.start_method,0,e);
 }e.clear();return true;
}
bool MenuDialogMessagesV97::next(DialogMsgV97& message,bool& found,std::string& e)const{
 found=!queue_.empty();if(found)message=queue_.front();e.clear();return true;
}
bool MenuDialogMessagesV97::style_name(std::int32_t value,std::string& out,std::string& e)const{
 if(!services_.owner||!services_.style_name){e="Required actual PyDataConstants.DialogStyles reverse lookup";return false;}
 return services_.style_name(value,out,e);
}
bool MenuDialogMessagesV97::skip(std::string& e){
 if(queue_.empty()){e.clear();return true;}queue_.pop_front(); //442c44
 if(services_.skip_method){if(!services_.invoke||!services_.invoke(services_.skip_method,0,e))return false;}
 if(!queue_.empty()){
  if(!services_.invoke||!services_.start_method){e="Required next queued dialogue AS start";return false;}
  return services_.invoke(services_.start_method,0,e);
 }e.clear();return true;
}
void MenuDialogMessagesV97::flush()noexcept{while(!queue_.empty())queue_.pop_front();} //46049c, no invented hide callback
bool menu_dialog_native_v97(MenuDialogMessagesV97& owner,const char* name,const gameswf::fn_call& fn,
 bool& handled,std::string& e){
 handled=false;if(!name||std::strcmp(name,"NativeGetNextDialogMessage"))return true;
 handled=true;
 if(fn.nargs!=1||!fn.arg(0).is_object())return true;
 auto* object=fn.arg(0).to_object();if(!object){e="Original NativeGetNextDialogMessage dereferences NULL argument object";return false;}
 DialogMsgV97 message;bool found{};if(!owner.next(message,found,e))return false;
 object->set_member("DialogTitle",gameswf::as_value(message.title0.c_str()));
 object->set_member("DialogMessage",gameswf::as_value(message.message18.c_str()));
 object->set_member("DialogStyle",gameswf::as_value(double(message.style30)));
 std::string style;if(!owner.style_name(message.style30,style,e))return false;
 object->set_member("DialogStyleStr",gameswf::as_value(style.c_str()));
 object->set_member("DialogActorStr",gameswf::as_value(message.actor34.c_str()));
 if(fn.result)fn.result->set_bool(found);e.clear();return true;
}
}
