#include "../localization_parse_ex_v1.hpp"
#include <fstream>
#include <iterator>
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace dh2::ui;
static void require(bool b){if(!b){std::fprintf(stderr,"FAIL\n");std::exit(1);}}
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 std::uint32_t word(){require(at+4<=bytes.size());std::uint32_t n=0;for(int i=0;i<4;++i)n|=std::uint32_t(bytes[at++])<<(8*i);return n;}
 std::string text(){auto n=word();require(at+n<=bytes.size());std::string s(reinterpret_cast<const char*>(bytes.data()+at),n);at+=n;return s;}
};
static bool title(void*,std::string& text,std::string&){text="Dungeon Hunter 2";return true;}
static bool version(void*,std::string& text,std::string&){text="1.0.4";return true;}
int main(int argc,char** argv){
 require(argc==2);std::ifstream file(argv[1],std::ios::binary);require(bool(file));Reader r;r.bytes.assign(std::istreambuf_iterator<char>(file),{});
 require(r.word()==0x30375350);auto count=r.word();std::string error;
 LocalizationNumberStyleV1 style{".",",",1000};
 for(unsigned i=0;i<count;++i){
  auto input=r.text();auto pack=r.word(),n=r.word();std::vector<LocalizationArgumentV1> args;
  for(unsigned j=0;j<n;++j){LocalizationArgumentV1 value;auto type=r.word();if(type){value.has_text=true;value.text=r.text();}else{auto bits=r.word();std::memcpy(&value.number,&bits,4);}args.push_back(value);}
  auto expected=r.text();auto expected_changed=r.word();std::string output;bool changed=false;
  require(localization_parse_ex_v1(input,args,style,pack>=4&&pack<=6,{},output,changed,error));
  if(output!=expected||changed!=bool(expected_changed)){std::fprintf(stderr,"Case %u: %s expected [%s] actual [%s]\n",i,input.c_str(),expected.c_str(),output.c_str());return 1;}
 }
 require(r.at==r.bytes.size());std::string output="sentinel";bool changed=true;
 require(localization_parse_ex_v1("^t ^v",{},style,false,{nullptr,title,version},output,changed,error)&&output=="Dungeon Hunter 2 1.0.4");
 output="sentinel";require(!localization_parse_ex_v1("^v",{},style,false,{},output,changed,error)&&output=="sentinel");
 require(!localization_parse_ex_v1("^d",{{1e30f,false,{}}},style,false,{},output,changed,error)&&output=="sentinel");
 require(localization_parse_ex_v1("^d",{{1234567.f,false,{}}},{",",".",1000},false,{},output,changed,error)&&output=="1.234.567");
 std::printf("PASS %u original/native parseEx comparisons plus title/version providers, localized grouping and atomic failure\n",count);
}
