#include "edit_text_event_v1.hpp"
#include <algorithm>
#include <limits>
namespace dh2::ui::edit_text_event_v1 {
namespace {bool need(bool b,const char*n,std::string&e){if(b)return true;e=std::string("source edit event requires ")+n;return false;}}
bool event(State&q,std::uint8_t id,std::uint8_t key,const Services&s,bool&accepted,std::string&e){
 e.clear();accepted=false;if(q.readonly)return true;
 if(id==20){if(!need(bool(s.active),"active entity coordinator",e)||!s.active(e))return false;
  if(!q.focus){if(!need(bool(s.handler),"onSetFocus lookup/call",e)||!s.handler("onSetFocus",e))return false;
   if(!need(bool(s.listener),"keypress listener owner",e)||!s.listener(true,e))return false;q.focus=true;
   if(q.text.size()>INT32_MAX)return need(false,"source string length",e);q.cursor=std::int32_t(q.text.size());
   if(!need(bool(s.format),"whole source formatter",e)||!s.format(e))return false;
  }accepted=true;return true;
 }
 if(id==21){if(q.focus){if(!need(bool(s.handler),"onKillFocus lookup/call",e)||!s.handler("onKillFocus",e))return false;q.focus=false;
   if(!need(bool(s.listener),"keypress listener owner",e)||!s.listener(false,e))return false;
   if(!need(bool(s.format),"whole source formatter",e)||!s.format(e))return false;
  }accepted=true;return true;
 }
 if(id!=8)return true;
 if(q.text.size()>INT32_MAX)return need(false,"source string length",e);auto text=q.text;q.cursor=std::min(q.cursor,std::int32_t(q.text.size()));const auto cursor=q.cursor;
 auto format=[&]{return need(bool(s.format),"whole source formatter",e)&&s.format(e);};
 auto write=[&]{return need(bool(s.set_value),"source bound value setter",e)&&s.set_value(text,e);};
 switch(key){
  case 8:if(cursor>0){text.erase(std::size_t(cursor-1),1);--q.cursor;if(!write())return false;}break;
  case 46:if(cursor<std::int32_t(text.size())){if(cursor<0)return need(false,"safe erase index",e);text.erase(std::size_t(cursor),1);if(!write())return false;}break;
  case 34:case 36:case 38:q.cursor=0;if(!format())return false;break;
  case 33:case 35:case 40:q.cursor=std::int32_t(q.text.size());if(!format())return false;break;
  case 37:q.cursor=cursor>0?cursor-1:0;if(!format())return false;break;
  case 39:q.cursor=cursor<std::int32_t(q.text.size())?cursor+1:std::int32_t(q.text.size());if(!format())return false;break;
  default:if(cursor<0)return need(false,"safe insert index",e);text.insert(std::size_t(cursor),1,char(key));++q.cursor;if(!write())return false;break;
 }
 // Original key branch deliberately returns false, even after editing.
 return true;
}
}
