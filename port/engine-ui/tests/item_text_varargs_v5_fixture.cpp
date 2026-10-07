#include "../item_text_varargs_v5.hpp"
#include <cstring>
using namespace dh2;
namespace {
std::uint32_t read(const std::uint8_t*& p){std::uint32_t x;std::memcpy(&x,p,4);p+=4;return x;}
std::string string(const std::uint8_t*& p){auto n=read(p);std::string s(reinterpret_cast<const char*>(p),n);p+=n;return s;}
struct Context {int pack;std::string defaults[3];};
bool service(void* p,const ui::HudTextRequestV1& q,ui::HudTextResponseV1& r,std::string&){auto& c=*static_cast<Context*>(p);if(q.operation==ui::hud_text_constant_v1){const char* keys[]={"GLOBAL_DECIMAL_SEPERATOR","GLOBAL_THOUSANDS_SEPERATOR","GLOBAL_THOUSANDS_GROUP_AT"};for(int i=0;i<3;++i)if(!std::strcmp(q.key,keys[i])){r.value=i;return true;}return false;}if(q.operation==ui::hud_text_integer_string_v1){r.text=c.defaults[q.value].c_str();return true;}if(q.operation==ui::hud_text_pack_v1){r.value=c.pack;return true;}return false;}
}
extern "C" std::uint32_t dh2_item_text_varargs_fixture_v5(const std::uint8_t* input,std::uint8_t* output){auto* p=input;Context c;c.pack=std::int32_t(read(p));auto count=read(p);std::vector<data::ItemTextArgumentV5> values;std::vector<std::string> strings;values.resize(count);strings.resize(count);for(unsigned i=0;i<count;++i){values[i].integer=std::int32_t(read(p));auto present=read(p);strings[i]=string(p);values[i].text=present?strings[i].c_str():nullptr;}auto text=string(p),out=string(p);for(auto& s:c.defaults)s=string(p);std::string e;bool changed=false;if(!ui::item_text_varargs_v5(text.c_str(),values.data(),values.size(),{&c,service},out,changed,e))return UINT32_MAX;std::uint32_t n=out.size(),flag=changed;std::memcpy(output,&flag,4);std::memcpy(output+4,&n,4);std::memcpy(output+8,out.data(),n);return n+8;}
