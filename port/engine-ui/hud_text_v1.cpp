#include "hud_text_v1.hpp"
#include "localization_parse_ex_v1.hpp"
#include <cstring>
#include <stdexcept>
#include <utility>

namespace dh2::ui { namespace {
constexpr std::size_t limit=16u*1024u*1024u,text_limit=1024u*1024u;
bool text_ok(const std::string& s){return s.size()<=text_limit&&s.find('\0')==std::string::npos;}
bool fail(std::string& e,const char* s){e=s;return false;}
struct Reader {
 LocalizationBytes b;std::size_t at{};
 explicit Reader(LocalizationBytes v):b(v){if(!b.data||b.size>limit)throw std::runtime_error("Localization input outside bounds");}
 std::uint32_t word(unsigned n=4){if(n>b.size-at)throw std::runtime_error("Truncated localization input");std::uint32_t v=0;for(unsigned i=0;i<n;++i)v|=std::uint32_t(b.data[at++])<<(i*8);return v;}
 std::string text(unsigned n=4){auto size=word(n);if(size>text_limit||size>b.size-at)throw std::runtime_error("Localization text outside bounds");std::string s(reinterpret_cast<const char*>(b.data+at),size);at+=size;if(!text_ok(s))throw std::runtime_error("Embedded NUL localization input");return s;}
 std::vector<std::string> names(){auto n=word();if(n>65536)throw std::runtime_error("Localization names outside bounds");std::vector<std::string> out;for(unsigned i=0;i<n;++i)out.push_back(text());return out;}
 void end(){if(at!=b.size)throw std::runtime_error("Localization trailing bytes");}
};
unsigned lower(unsigned c){return c>='A'&&c<='Z'?c+32:c;}
bool prefix_equal(const std::string& symbol,const std::string& name,std::size_t n){
 for(std::size_t i=0;i<n;++i){unsigned a=i<symbol.size()?static_cast<unsigned char>(symbol[i]):0,b=i<name.size()?static_cast<unsigned char>(name[i]):0;if(lower(a)!=lower(b))return false;if(!a)return true;}return true;
}
bool constant(const LocalizationServices& s,const char* group,const char* key,std::uint32_t& value,std::string& e){if(!s.constant)return fail(e,"Localization constant provider missing");return s.constant(s.context,group,key,value,e);}
bool add_space(std::int32_t pack){return pack>=4&&pack<=6;}
std::uint32_t asr(std::uint32_t v,std::uint32_t n){n&=255;if(n>=32)return (v&0x80000000u)?~0u:0u;if(!n)return v;return(v>>n)|((v&0x80000000u)?(~0u<<(32-n)):0u);}
struct Busy {bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}};
} // namespace
bool HudTextV1::load(LocalizationBytes bytes,LocalizationBytes names,LocalizationBytes schema,std::string& e){
 e.clear();if(busy_)return fail(e,"Localization reentry unsupported");try{Reader r(bytes),n(names),s(schema);HudTextV1 next;
 if(r.word()!=9)throw std::runtime_error("Localization pack count differs");next.pack_names_=n.names();n.end();
 if(next.pack_names_!=std::vector<std::string>{"ENGLISH","FRENCH","GERMAN","ITALIAN","JAPANESE","KOREAN","SC","SPANISH","SYMBOLS"}||s.names()!=std::vector<std::string>{"filename","name"}||s.names()!=std::vector<std::string>{"list"})throw std::runtime_error("Localization source names/schema differ");s.end();
 for(auto& pack:next.sheets_){if(r.word()!=37)throw std::runtime_error("Localization sheet count differs");for(auto& sheet:pack){sheet.filename=r.text();sheet.name=r.text();if(sheet.filename.empty()||sheet.name.empty())throw std::runtime_error("Empty localization sheet metadata");}}r.end();next.ready_=true;*this=std::move(next);return true;
 }catch(const std::exception& x){e=x.what();return false;}
}
bool HudTextV1::switch_pack(std::int32_t p,bool unload,std::string& e){e.clear();if(busy_||p< -1||p>8)return fail(e,"Localization pack caller outside bounds");if(unload&&p!=pack_&&pack_!=-1)for(auto& sheet:sheets_[pack_]){sheet.strings.clear();sheet.loaded=false;}pack_=p;return true;}
bool HudTextV1::preload(std::uint32_t p,std::uint32_t sheet,bool force,const LocalizationServices& svc,std::string& e){if(busy_)return fail(e,"Localization reentry unsupported");Busy guard(busy_);return preload_impl(p,sheet,force,svc,e);}
bool HudTextV1::preload_impl(std::uint32_t p,std::uint32_t sheet,bool force,const LocalizationServices& svc,std::string& e){
 e.clear();if(!ready_||p>8||sheet>36)return fail(e,"Localization preload outside bounds");auto& target=sheets_[p][sheet];if(target.loaded&&!force)return true;
 if(force){target.strings.clear();target.loaded=false;}if(!svc.open||!svc.close||!svc.debug)return fail(e,"Localization file/debug provider missing");
 bool found=false;std::vector<std::uint8_t> bytes;std::uintptr_t lease=0;const auto uri="text/"+target.filename;
 if(!svc.open(svc.context,uri.c_str(),found,bytes,lease,e))return false;if(!found){if(lease)return fail(e,"Missing localization file returned lease");return true;}
 if(!lease)return fail(e,"Localization file has no retained lease");bool success=false;
 try{Reader r({bytes.data(),bytes.size()});auto count=r.word(2);if(count>32767)throw std::runtime_error("Localization signed source count outside port domain");std::vector<std::string> values;values.reserve(count);
 for(unsigned i=0;i<count;++i){auto raw=r.text(2);if(raw.find_first_of("^|")!=std::string::npos){std::string colored;bool changed;if(!localization_colors(raw,add_space(pack_),svc,colored,changed,e))throw std::runtime_error(e);if(changed)raw=std::move(colored);}values.push_back(std::move(raw));}r.end();target.strings=std::move(values);target.loaded=true;
 success=svc.debug(svc.context,"isTracingStringManager",e);
 }catch(const std::exception& x){e=x.what();}
 std::string closed;if(!svc.close(svc.context,lease,closed)){if(success)e=closed;success=false;}return success;
}
bool HudTextV1::index(std::uint32_t sheet,std::uint32_t number,std::int32_t p,const LocalizationServices& svc,std::string& out,std::string& e){
 if(p==-1)p=0;if(p<0||p>8||sheet>36)return fail(e,"Localization string index outside bounds");if(!preload_impl(p,sheet,false,svc,e))return false;const auto& row=sheets_[p][sheet];
 if(number>=row.strings.size())out="#!WTF!#";else out=row.strings[number].empty()?"#!SNL!#":row.strings[number];return true;
}
bool HudTextV1::id(std::uint32_t idvalue,const LocalizationServices& svc,std::string& out,std::string& e){std::uint32_t ps,pm,ss,sm;
 if(!constant(svc,"StringConfig","PackIDShift",ps,e)||!constant(svc,"StringConfig","PackIDMask",pm,e)||!constant(svc,"StringConfig","StrIDShift",ss,e)||!constant(svc,"StringConfig","StrIDMask",sm,e))return false;
 return index(asr(idvalue,ps)&pm,asr(idvalue,ss)&sm,pack_,svc,out,e);
}
bool HudTextV1::defaults(const LocalizationServices& svc,std::string& e){for(const char* key:{"GLOBAL_DECIMAL_SEPERATOR","GLOBAL_THOUSANDS_SEPERATOR","GLOBAL_THOUSANDS_GROUP_AT"}){std::uint32_t v;std::string out;if(!constant(svc,"StrID",key,v,e)||!id(v,svc,out,e))return false;}return true;}
bool HudTextV1::native_string(const std::string& symbol,const LocalizationServices& svc,LocalizationResult& out,std::string& e){
 e.clear();auto split=symbol.find('_');if(!ready_||busy_||!text_ok(symbol)||split==std::string::npos)return fail(e,"Localization symbol caller outside bounds");
 if(!svc.debug||!svc.player_character||!svc.player_name)return fail(e,"Localization debug/player provider missing");Busy guard(busy_);LocalizationResult next;next.sets_menu_string_flag=symbol=="MENU_ERROR_NO_USERNAME"||symbol=="MENU_ERROR_NO_PASSWORD";
 if(!svc.debug(svc.context,"isTracingStringManager",e))return false;
 unsigned p=pack_==-1?0:static_cast<unsigned>(pack_);std::string localized;
 for(unsigned sheet=0;sheet<37&&!next.found;++sheet){if(!prefix_equal(symbol,sheets_[p][sheet].name,split))continue;
  std::string text;if(!index(sheet,0,8,svc,text,e))return false;
  for(unsigned i=0;i<sheets_[8][sheet].strings.size();++i){if(i&&!index(sheet,i,8,svc,text,e))return false;if(text.size()!=symbol.size()||!prefix_equal(text,symbol,text.size()))continue;
   if(!index(sheet,i,pack_,svc,localized,e)||!svc.debug(svc.context,"isTracingStringManager",e))return false;next.found=true;break;}
 }
 if(!next.found){next.text="notfound";out=std::move(next);return true;}
 if(!localized.empty()&&!defaults(svc,e))return false;if(!localization_plain(localized,add_space(pack_),next.text,e))return false;
 std::uintptr_t character=0;if(!svc.player_character(svc.context,character,e))return false;
 if(character){if(!svc.player_character(svc.context,character,e))return false;if(!character)return fail(e,"Source second player/character query became null");std::string name;if(!svc.player_name(svc.context,character,name,e)||!defaults(svc,e))return false;std::string parsed;if(!localization_player(next.text,name,add_space(pack_),parsed,e))return false;next.text=std::move(parsed);}
 out=std::move(next);return true;
}
const std::string& HudTextV1::sheet_name(std::uint32_t p,std::uint32_t s)const{if(!ready_||p>8||s>36)throw std::out_of_range("Localization sheet");return sheets_[p][s].name;}
bool HudTextV1::parsed_string_v4(const std::string& symbol,const std::vector<LocalizationArgumentV1>& args,
 const HudTextEnvironmentV1& env,std::string& output,std::string& error){
 error.clear();const auto split=symbol.find('_');
 if(!ready_||busy_||!text_ok(symbol)||split==std::string::npos)return fail(error,"Parsed HUD symbol outside bounds");
 const auto& svc=env.localization;if(!svc.debug)return fail(error,"Required actual parsed StringManager Debug");
 std::string localized="notfound";
 {
  Busy guard(busy_);if(!svc.debug(svc.context,"isTracingStringManager",error))return false;
  const unsigned pack=pack_==-1?0:static_cast<unsigned>(pack_);bool found=false;
  for(unsigned sheet=0;sheet<37&&!found;++sheet){
   if(!prefix_equal(symbol,sheets_[pack][sheet].name,split))continue;
   std::string candidate;if(!index(sheet,0,8,svc,candidate,error))return false;
   for(unsigned i=0;i<sheets_[8][sheet].strings.size();++i){
    if(i&&!index(sheet,i,8,svc,candidate,error))return false;
    if(candidate.size()!=symbol.size()||!prefix_equal(candidate,symbol,candidate.size()))continue;
    if(!index(sheet,i,pack_,svc,localized,error)||!svc.debug(svc.context,"isTracingStringManager",error))return false;
    found=true;break;
   }
  }
 }
 std::vector<HudTextVariantV1> values;values.reserve(args.size());
 for(const auto& arg:args)values.push_back({arg.number,0,arg.has_text?arg.text.c_str():nullptr});
 std::string next;bool changed=false;
 if(!parse_ex(localized.c_str(),values.data(),values.size(),env,next,changed,error))return false;
 output=std::move(next);return true;
}
const std::string& HudTextV1::sheet_filename(std::uint32_t p,std::uint32_t s)const{if(!ready_||p>8||s>36)throw std::out_of_range("Localization sheet");return sheets_[p][s].filename;}
std::size_t HudTextV1::loaded_sheets()const{std::size_t count=0;for(const auto& p:sheets_)for(const auto& s:p)count+=s.loaded;return count;}

bool HudTextV1::integer_string(std::int32_t number,const LocalizationServices& svc,std::string& out,bool& is_null,std::string& e){
 e.clear();if(!ready_||busy_)return fail(e,"HUD integer localization owner unavailable");
 if(number<0){is_null=true;return true;}Busy guard(busy_);if(!id(static_cast<std::uint32_t>(number),svc,out,e))return false;is_null=false;return true;
}
bool HudTextV1::source_string_index_v1(std::uint32_t sheet,std::uint32_t number,std::int32_t p,const LocalizationServices& svc,std::string& out,std::string& e){
 e.clear();if(!ready_||busy_)return fail(e,"Source StringManager cache unavailable");
 Busy guard(busy_);return index(sheet,number,p,svc,out,e);
}
bool HudTextV1::source_string_count_v1(std::uint32_t sheet,std::int32_t p,std::int32_t& out,std::string& e)const{
 e.clear();if(!ready_||busy_||p<0||p>8||sheet>36)return fail(e,"Source StringManager count outside bounds");
 // getNumberOfStrings507550 reads its signed16 field without loading a file.
 out=static_cast<std::int32_t>(sheets_[static_cast<unsigned>(p)][sheet].strings.size());return true;
}
bool HudTextV1::parse_ex(const char* input,const HudTextVariantV1* values,std::size_t count,const HudTextEnvironmentV1& env,std::string& out,bool& changed,std::string& e){
 e.clear();if(!ready_||busy_)return fail(e,"HUD formatting owner unavailable");Busy guard(busy_);
 struct Context {HudTextV1* owner;const HudTextEnvironmentV1* env;std::string scratch;} c{this,&env,{}};
 HudTextServicesV1 svc{&c,[](void* p,const HudTextRequestV1& q,HudTextResponseV1& r,std::string& error){
  auto& c=*static_cast<Context*>(p);const auto& env=*c.env;
  if(q.operation==hud_text_constant_v1){std::uint32_t v;if(!constant(env.localization,q.group,q.key,v,error))return false;r.value=static_cast<std::int32_t>(v);return true;}
  if(q.operation==hud_text_integer_string_v1){if(q.value<0)return fail(error,"Source formatter localized default is null");if(!c.owner->id(static_cast<std::uint32_t>(q.value),env.localization,c.scratch,error))return false;r.text=c.scratch.c_str();return true;}
  if(q.operation==hud_text_pack_v1){r.value=c.owner->pack_;return true;}
  if(q.operation==hud_text_version_v1){if(!env.version)return fail(error,"HUD Application.GetVersionString provider missing");if(!env.version(env.application,q.limit,q.flag!=0,c.scratch,error))return false;r.text=c.scratch.c_str();return true;}
  if(q.operation==hud_text_title_v1){if(!env.title)return fail(error,"HUD Application.GetTitleString provider missing");if(!env.title(env.application,q.limit,c.scratch,error))return false;r.text=c.scratch.c_str();return true;}
  return fail(error,"Unknown HUD text operation");
 }};
 return hud_text_parse_ex_v1(input,values,count,svc,out,changed,e);
}
} // namespace dh2::ui
