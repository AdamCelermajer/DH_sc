#include "../hud_text_format_v1.hpp"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <vector>
using namespace dh2::ui;
namespace {
struct Reader {const unsigned char* p;std::size_t size,at{};std::uint32_t word(){if(size-at<4)throw std::runtime_error("Truncated text fixture");std::uint32_t v;std::memcpy(&v,p+at,4);at+=4;return v;}std::string text(){auto n=word();if(n>size-at)throw std::runtime_error("Truncated text string");std::string s(reinterpret_cast<const char*>(p+at),n);at+=n;return s;}};
void word(std::vector<unsigned char>& out,std::uint32_t v){auto p=reinterpret_cast<const unsigned char*>(&v);out.insert(out.end(),p,p+4);}
void text(std::vector<unsigned char>& out,const std::string& s){word(out,s.size());out.insert(out.end(),s.begin(),s.end());}
struct Context {std::int32_t pack{};std::string defaults[3];std::vector<std::uint32_t> trace;};
bool service(void* opaque,const HudTextRequestV1& q,HudTextResponseV1& r,std::string&){auto& c=*static_cast<Context*>(opaque);if(q.operation==hud_text_constant_v1){static const char* keys[]={"GLOBAL_DECIMAL_SEPERATOR","GLOBAL_THOUSANDS_SEPERATOR","GLOBAL_THOUSANDS_GROUP_AT"};int i=0;while(i<3&&std::strcmp(q.key,keys[i]))++i;if(i==3)return false;c.trace.insert(c.trace.end(),{1u,static_cast<unsigned>(i)});r.value=i;}
 else if(q.operation==hud_text_integer_string_v1){if(q.value<0||q.value>2)return false;c.trace.insert(c.trace.end(),{2u,static_cast<unsigned>(q.value)});r.text=c.defaults[q.value].c_str();}
 else if(q.operation==hud_text_version_v1){c.trace.insert(c.trace.end(),{3u,q.limit,q.flag});r.text="1.0.2";}
 else if(q.operation==hud_text_title_v1){c.trace.insert(c.trace.end(),{4u,q.limit,q.flag});r.text="Dungeon Hunter 2";}
 else if(q.operation==hud_text_pack_v1){c.trace.insert(c.trace.end(),{5u,static_cast<unsigned>(c.pack)});r.value=c.pack;}
 else return false;return true;}
std::vector<unsigned char> execute(const unsigned char* p,std::size_t n){Reader r{p,n};Context c;c.pack=static_cast<int>(r.word());auto count=r.word();if(count>65536)throw std::runtime_error("Count");std::vector<HudTextVariantV1> values(count);std::vector<std::string> texts(count);for(std::size_t i=0;i<count;++i){auto bits=r.word(),present=r.word();std::memcpy(&values[i].number,&bits,4);texts[i]=r.text();values[i].text=present?texts[i].c_str():nullptr;}auto input=r.text(),output=r.text();for(auto& s:c.defaults)s=r.text();if(r.at!=r.size)throw std::runtime_error("Trailing fixture");bool changed=false;std::string error;if(!hud_text_parse_ex_v1(input.c_str(),values.data(),values.size(),{&c,service},output,changed,error))throw std::runtime_error(error);std::vector<unsigned char> out;word(out,changed);text(out,output);word(out,c.trace.size());for(auto v:c.trace)word(out,v);return out;}
}
extern "C" std::uint32_t dh2_hud_text_format_test(const unsigned char* input,std::uint32_t size,unsigned char* output){try{auto out=execute(input,size);std::memcpy(output,out.data(),out.size());return out.size();}catch(...){return 0xffffffff;}}
#if !defined(DH2_TEXT_FORMAT_ORACLE)
int main(int argc,char** argv){try{if(argc!=2)throw std::runtime_error("Expected formatter gold path");std::ifstream f(argv[1],std::ios::binary);std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(f)),{});Reader r{bytes.data(),bytes.size()};if(r.word()!=0x31465448)throw std::runtime_error("Magic");auto count=r.word();std::size_t services=0;for(unsigned i=0;i<count;++i){auto input=r.text(),expected=r.text();auto actual=execute(reinterpret_cast<const unsigned char*>(input.data()),input.size());if(actual.size()!=expected.size()||std::memcmp(actual.data(),expected.data(),actual.size()))throw std::runtime_error("Formatter mismatch "+std::to_string(i));Reader result{actual.data(),actual.size()};result.word();result.text();services+=result.word();}if(r.at!=r.size)throw std::runtime_error("Trailing gold");std::string out="kept",e;bool changed=true;unsigned guards=0;if(!hud_text_parse_ex_v1("x",nullptr,1,{},out,changed,e)&&out=="kept")++guards;if(!hud_text_parse_ex_v1("x",nullptr,0,{},out,changed,e)&&out=="kept")++guards;if(hud_text_parse_ex_v1(nullptr,nullptr,0,{},out,changed,e)&&out=="kept"&&!changed)++guards;if(guards!=3)throw std::runtime_error("Malformed guards");std::printf("{\"validation\":\"PASS\",\"cases\":%u,\"service_words\":%zu,\"guards\":%u,\"mismatches\":0}\n",count,services,guards);return 0;}catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
#endif
