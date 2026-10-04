#include "shader_sources.hpp"
#include <limits>
#include <utility>

namespace dh2::scene {
namespace {
constexpr std::size_t limit=1024*1024;
bool text(std::string_view s) {return s.size()<=limit && s.find('\0')==s.npos;}
bool fail(std::string& e,const char* s){e=s;return false;}
std::uint16_t u16(const std::uint8_t* p){return std::uint16_t(p[0])|(std::uint16_t(p[1])<<8);}
std::uint32_t u32(const std::uint8_t* p){return std::uint32_t(u16(p))|(std::uint32_t(u16(p+2))<<16);}
std::uint32_t crc(const std::uint8_t* p,std::size_t n){std::uint32_t c=~0u;while(n--){c^=*p++;for(int j=0;j<8;++j)c=(c>>1)^(0xedb88320u& (0u-(c&1)));}return ~c;}
bool span(std::size_t at,std::size_t n,std::size_t size){return at<=size && n<=size-at;}
}
bool shader_code_name(std::string_view a,std::string_view b,std::string_view c,const std::string* extra,std::string& out,std::string& e){
    if(!text(a)||!text(b)||!text(c)||(extra&&!text(*extra)))return fail(e,"Invalid shader text");
    const auto n=a.size()+b.size()+c.size()+(extra?extra->size():0);
    if(n>limit)return fail(e,"Shader name too large");
    std::string v;v.reserve(n);v.append(a);v.append(b);v.append(c);if(extra)v+=*extra;out=std::move(v);e.clear();return true;
}
bool shader_config_text(std::string_view input,std::string& out,std::string& e){
    if(!text(input))return fail(e,"Invalid config text");std::string v(input);for(char& c:v)if(c=='^')c='\n';out=std::move(v);e.clear();return true;
}
bool shader_source_plan(std::uint32_t flags,std::uint32_t type,std::string_view filename,std::string_view caller,const std::string* extra,std::string_view body,ShaderSourcePlan& out,std::string& e){
    if(!text(body))return fail(e,"Invalid shader body");ShaderSourcePlan v;
    if(!shader_code_name(filename,{},caller,extra,v.cache_name,e))return false;
    v.chunks[0]=(flags&0x400)?"#define GLITCH_USE_HIGHP\n":"";
    v.chunks[1]=(flags&0x800)?"#define GLITCH_USE_BIAS\n":"";
    v.chunks[2]=(flags&0x1000)?"#define GLITCH_FORCE_USE_BIAS\n":"";
    v.chunks[3]="#define GLITCH_OPENGLES_2\n";
    v.chunks[4]=extra?*extra:"";v.chunks[5]=caller;v.chunks[6]="\n";v.chunks[7]=body;
    v.gl_type=type==4?0x8b31u:0x8b30u;out=std::move(v);e.clear();return true;
}
const ShaderPackMember* ShaderSourcePack::find(std::string_view n)const noexcept {for(const auto& m:members_)if(m.name==n)return &m;return nullptr;}
bool ShaderSourcePack::load(const std::uint8_t* p,std::size_t n,std::string& e){
    if(!p||n<22||n>16*limit)return fail(e,"Invalid shader pack");
    std::size_t end=n-22;const auto lower=n>65557?n-65557:0;
    for(;;){if(u32(p+end)==0x06054b50&&span(end,22u+u16(p+end+20),n)&&end+22u+u16(p+end+20)==n)break;if(end==lower)return fail(e,"Missing ZIP directory");--end;}
    const auto count=u16(p+end+10);const std::size_t offset=u32(p+end+16),length=u32(p+end+12);
    if(u16(p+end+4)||u16(p+end+6)||u16(p+end+8)!=count||count>4096||!span(offset,length,end)||offset+length!=end)return fail(e,"Unsupported ZIP directory");
    std::vector<ShaderPackMember> v;std::size_t at=offset;v.reserve(count);
    for(std::uint32_t i=0;i<count;++i){
        if(!span(at,46,end)||u32(p+at)!=0x02014b50)return fail(e,"Invalid ZIP member");
        const std::size_t size=u32(p+at+24),name_n=u16(p+at+28),extra_n=u16(p+at+30),comment_n=u16(p+at+32),local=u32(p+at+42);
        if(u16(p+at+8)||u16(p+at+10)||u16(p+at+34)||size!=u32(p+at+20)||size>limit||!name_n||!span(at,46+name_n+extra_n+comment_n,end)||!span(local,30,offset)||u32(p+local)!=0x04034b50)return fail(e,"Unsupported ZIP member");
        const std::string name(reinterpret_cast<const char*>(p+at+46),name_n);
        if(!text(name)||name.find('/')!=name.npos||name.find('\\')!=name.npos||name=="."||name=="..")return fail(e,"Invalid shader member name");
        for(const auto& m:v)if(m.name==name)return fail(e,"Duplicate shader member");
        const std::size_t lname=u16(p+local+26),lextra=u16(p+local+28),data=local+30+lname+lextra;
        if(u16(p+local+6)||u16(p+local+8)||u32(p+local+14)!=u32(p+at+16)||u32(p+local+18)!=size||u32(p+local+22)!=size||lname!=name_n||!span(local,30+lname+lextra,offset)||!span(data,size,offset)||std::string_view(reinterpret_cast<const char*>(p+local+30),lname)!=name||crc(p+data,size)!=u32(p+at+16))return fail(e,"Invalid stored shader data");
        v.push_back({name,std::vector<std::uint8_t>(p+data,p+data+size)});at+=46+name_n+extra_n+comment_n;
    }
    if(at!=end)return fail(e,"Invalid directory extent");members_=std::move(v);e.clear();return true;
}
}
