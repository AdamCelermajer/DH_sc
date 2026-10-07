#include "hud_text_format_v1.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>

namespace dh2::ui {namespace {
constexpr std::size_t max_text=1024u*1024u,max_values=65536;
bool fail(std::string& e,const char* s){e=s;return false;}
bool length(const char* p,std::size_t& n){n=0;if(!p)return true;while(n<=max_text&&p[n])++n;return n<=max_text;}
bool call(const HudTextServicesV1& s,const HudTextRequestV1& q,HudTextResponseV1& r,std::string& e){if(!s.invoke)return fail(e,"HUD text required provider missing");if(!s.invoke(s.context,q,r,e)){if(e.empty())e="HUD text required provider failed";return false;}return true;}
std::int32_t integer(float f){if(std::isnan(f))return 0;if(f>=2147483648.0f)return INT32_MAX;if(f< -2147483648.0f)return INT32_MIN;return static_cast<std::int32_t>(f);}
std::int32_t ascii_integer(const std::string& s){const unsigned char* p=reinterpret_cast<const unsigned char*>(s.c_str());while(*p==' '||(*p>=9&&*p<=13))++p;bool negative=false;if(*p=='+'||*p=='-'){negative=*p=='-';++p;}std::uint32_t value=0;while(*p>='0'&&*p<='9'){value=value*10+(*p-'0');++p;}return static_cast<std::int32_t>(negative?0u-value:value);}
void group_number(std::int32_t value,std::int32_t threshold,const std::string& sep,char (&buf)[32]){
 if(value<threshold){std::snprintf(buf,sizeof buf,"%d",value);return;}
 auto million=value/1000000,rest=value-million*1000000,thousand=rest/1000,last=value-(value/1000)*1000;
 if(million)std::snprintf(buf,sizeof buf,"%d%s%03d%s%03d",million,sep.c_str(),thousand,sep.c_str(),last);
 else if(thousand)std::snprintf(buf,sizeof buf,"%d%s%03d",thousand,sep.c_str(),last);
 else std::snprintf(buf,sizeof buf,"%d",last);
}
std::string utf(const std::string& s,bool spacing){std::string out;for(std::size_t i=0;i<s.size();++i){unsigned char c=s[i];if(c==' '&&i+1<s.size()&&std::strchr("!.:;?",s[i+1]))out+="\xc2\xa0";else{out+=char(c);if(spacing&&(c=='!'||c=='?'))out+=' ';}}return out;}
} // namespace
bool hud_text_parse_ex_v1(const char* input,const HudTextVariantV1* values,std::size_t count,const HudTextServicesV1& svc,std::string& output,bool& changed,std::string& e){
 e.clear();std::size_t n;if(!length(input,n)||count>max_values||(count&&!values)||(values&&reinterpret_cast<std::uintptr_t>(values)%alignof(HudTextVariantV1))||output.size()>max_text||output.find('\0')!=std::string::npos)return fail(e,"HUD text input outside bounds");
 if(!input||!n){changed=false;return true;}
 for(std::size_t i=0;i<count;++i){std::size_t len;if(!length(values[i].text,len))return fail(e,"HUD text argument outside bounds");}
 std::string defaults[3];unsigned j=0;for(const char* key:{"GLOBAL_DECIMAL_SEPERATOR","GLOBAL_THOUSANDS_SEPERATOR","GLOBAL_THOUSANDS_GROUP_AT"}){
  HudTextResponseV1 c,r;if(!call(svc,{hud_text_constant_v1,0,0,0,"StrID",key},c,e)||!call(svc,{hud_text_integer_string_v1,0,c.value,0,nullptr,nullptr},r,e))return false;
  std::size_t len;if(!r.text||!length(r.text,len))return fail(e,"HUD text localized default missing");defaults[j++].assign(r.text,len);
 }
 const auto threshold=ascii_integer(defaults[2]);std::size_t arg=0;bool escape=false,flag=false;
 for(std::size_t i=0;i<n;++i){if(output.size()>max_text)return fail(e,"HUD text output outside bounds");unsigned char c=input[i];if(!escape){if(c=='^'){escape=true;continue;}if(c=='|'){output+='\x11';flag=true;}else output+=char(c);continue;}
  escape=false;
  if(c=='#'||c=='*'||c=='^')output+=char(c);
  else if(c=='n')output+='\n';
  else if(c=='v'||c=='t'){HudTextResponseV1 r;if(!call(svc,{c=='v'?hud_text_version_v1:hud_text_title_v1,c=='v'?10u:32u,0,c=='v'?1u:0u,nullptr,nullptr},r,e))return false;std::size_t len;if(!r.text||!length(r.text,len))return fail(e,"HUD text application string missing");output.append(r.text,len);}
  else if(c=='s'){if(arg<count){const char* p=values[arg++].text;if(p)output+=p;}}
  else if(std::strchr("dfghikmp",c)){if(arg>=count)continue;float v=values[arg++].number;
   if(c=='k'||c=='g')v=v/1000.0f;else if(c=='p'||c=='h')v=v*100.0f;else if(c=='i')v=v/5.0f;
   char buf[32];if(c=='d'||c=='k'||c=='p'){group_number(integer(v),threshold,defaults[1],buf);output+=buf;continue;}
   if(c=='m'){v=v/100.0f;v=v+(v<0.0f?-0.05f:0.05f);}else v=v+(v<0.0f?-0.005f:0.005f);
   float whole;float fraction=std::modf(v,&whole);fraction=fraction-(v<0.0f?-0.005f:0.005f);group_number(integer(whole),threshold,defaults[1],buf);output+=buf;
   fraction=std::fabs(fraction);if(fraction<0.0001f)continue;output+=defaults[0];char decimal[16];std::snprintf(decimal,sizeof decimal,c=='m'?"%.1f":"%.2f",static_cast<double>(fraction));if(std::strlen(decimal)>=2)output+=decimal+2;
  }
  if(output.size()>max_text)return fail(e,"HUD text output outside bounds");
 }
 if(output.size()>max_text)return fail(e,"HUD text output outside bounds");HudTextResponseV1 p;if(!call(svc,{hud_text_pack_v1,0,0,0,nullptr,nullptr},p,e))return false;output=utf(output,p.value>=4&&p.value<=6);if(output.size()>max_text)return fail(e,"HUD text output outside bounds");changed=flag;return true;
}
} // namespace dh2::ui
