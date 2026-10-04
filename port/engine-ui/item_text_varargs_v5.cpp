#include "item_text_varargs_v5.hpp"
#include <cstdio>
#include <cstring>
namespace dh2::ui {namespace {
constexpr std::size_t limit=1024*1024;
bool fail(std::string& e,const char* s){e=s;return false;}
bool text(const char* p,std::size_t& n){n=0;if(p)while(n<=limit&&p[n])++n;return n<=limit;}
bool call(const HudTextServicesV1& s,const HudTextRequestV1& q,HudTextResponseV1& r,std::string& e){if(!s.invoke)return fail(e,"Item varargs formatter provider missing");return s.invoke(s.context,q,r,e);}
std::int32_t ascii(const std::string& s){const auto* p=reinterpret_cast<const unsigned char*>(s.c_str());while(*p==' '||(*p>=9&&*p<=13))++p;bool neg=*p=='-';if(*p=='+'||*p=='-')++p;std::uint32_t n=0;while(*p>='0'&&*p<='9')n=n*10+(*p++-'0');n=neg?0u-n:n;std::int32_t r;std::memcpy(&r,&n,4);return r;}
void number(std::int32_t v,std::int32_t threshold,const std::string& separator,char (&out)[32]){if(v<threshold){std::snprintf(out,sizeof out,"%d",v);return;}auto million=v/1000000,rest=v-million*1000000,thousand=rest/1000,last=v-v/1000*1000;if(million)std::snprintf(out,sizeof out,"%d%s%03d%s%03d",million,separator.c_str(),thousand,separator.c_str(),last);else if(thousand)std::snprintf(out,sizeof out,"%d%s%03d",thousand,separator.c_str(),last);else std::snprintf(out,sizeof out,"%d",last);}
std::string utf(const std::string& s,bool spacing){std::string out;for(std::size_t i=0;i<s.size();++i){unsigned char c=s[i];if(c==' '&&i+1<s.size()&&std::strchr("!.:;?",s[i+1]))out+="\xc2\xa0";else{out+=char(c);if(spacing&&(c=='!'||c=='?'))out+=' ';}}return out;}
}
bool item_text_varargs_v5(const char* input,const data::ItemTextArgumentV5* a,std::size_t count,const HudTextServicesV1& svc,std::string& out,bool& changed,std::string& e){
 e.clear();std::size_t n;if(!text(input,n)||count>65536||(count&&(!a||reinterpret_cast<std::uintptr_t>(a)%alignof(data::ItemTextArgumentV5)))||out.size()>limit||out.find('\0')!=std::string::npos)return fail(e,"Malformed Item varargs text");if(!input||!n){changed=false;return true;}for(std::size_t j=0;j<count;++j){std::size_t k;if(!text(a[j].text,k))return fail(e,"Malformed Item string argument");}
 std::string defaults[3];unsigned k=0;for(auto key:{"GLOBAL_DECIMAL_SEPERATOR","GLOBAL_THOUSANDS_SEPERATOR","GLOBAL_THOUSANDS_GROUP_AT"}){HudTextResponseV1 c,r;if(!call(svc,{hud_text_constant_v1,0,0,0,"StrID",key},c,e)||!call(svc,{hud_text_integer_string_v1,0,c.value},r,e))return false;std::size_t length;if(!r.text||!text(r.text,length))return fail(e,"Missing source localized numeric default");defaults[k++].assign(r.text,length);}
 const auto threshold=ascii(defaults[2]);bool escape=false,flag=false;std::size_t arg=0;
 for(std::size_t j=0;j<n;++j){auto c=input[j];if(!escape){if(c=='^')escape=true;else if(c=='|'){out+='\x11';flag=true;}else out+=c;}else{escape=false;flag=true;
  if(c=='#'||c=='*'||c=='^')out+=c;else if(c=='n')out+='\n';else if(c=='s'){if(arg>=count)return fail(e,"Missing source string vararg");out+=a[arg].text?a[arg].text:"<null>";++arg;}
  else if(c=='d'||c=='k'||c=='p'){if(arg>=count)return fail(e,"Missing source integer vararg");auto v=a[arg++].integer;if(c=='k')v/=1000;else if(c=='p'){auto u=std::uint32_t(v)*100;std::memcpy(&v,&u,4);}char buf[32];number(v,threshold,defaults[1],buf);out+=buf;}
  else if(c=='v'||c=='t'){HudTextResponseV1 r;if(!call(svc,{c=='v'?hud_text_version_v1:hud_text_title_v1,c=='v'?10u:32u,0,c=='v'?1u:0u},r,e))return false;std::size_t length;if(!r.text||!text(r.text,length))return fail(e,"Required Application string missing");out.append(r.text,length);}
  else if(std::strchr("fghim$",c))return fail(e,"Unrecovered Item varargs directive required");
 }if(out.size()>limit)return fail(e,"Item formatted text exceeds budget");}
 HudTextResponseV1 pack;if(!call(svc,{hud_text_pack_v1},pack,e))return false;out=utf(out,pack.value>=4&&pack.value<=6);if(out.size()>limit)return fail(e,"Item UTF text exceeds budget");changed=flag;return true;
}
}

