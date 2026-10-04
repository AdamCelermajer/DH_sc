#include "../localization.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2::ui;
namespace {
unsigned word(const unsigned char*& p,const unsigned char* end){if(end-p<4)throw std::runtime_error("gold truncated");unsigned v;std::memcpy(&v,p,4);p+=4;return v;}
std::string text(const unsigned char*& p,const unsigned char* end){auto n=word(p,end);if(n>unsigned(end-p))throw std::runtime_error("gold text truncated");std::string s(reinterpret_cast<const char*>(p),n);p+=n;return s;}
void append(std::string& s,unsigned v){s.append(reinterpret_cast<const char*>(&v),4);}
bool constants(void* context,const char* group,const char* key,unsigned& v,std::string&){
 static const char* keys[]={"zero","one","two","three","four","five","six","seven","eight","nine"};v=0;if(std::strcmp(group,"FontTextColors"))return false;for(unsigned i=0;i<10;++i)if(!std::strcmp(key,keys[i])){v=context?static_cast<unsigned*>(context)[i]:i*0x010203;return true;}return false;
}
std::string execute(unsigned op,const unsigned char* p,unsigned n){
 auto end=p+n;std::string output,error,value;LocalizationServices s{};s.constant=constants;
 if(op<3){bool spacing=word(p,end)!=0;unsigned colors[10];if(op==0){for(auto& c:colors)c=word(p,end);s.context=colors;}auto input=text(p,end);bool changed=false;bool ok;
  if(op==0)ok=localization_colors(input,spacing,s,value,changed,error);else if(op==1)ok=localization_plain(input,spacing,value,error);else{auto name=text(p,end);ok=localization_player(input,name,spacing,value,error);}
  if(!ok||p!=end)throw std::runtime_error(error);if(!op)append(output,changed);output+=value;
 }else if(op==3){auto records=text(p,end),names=text(p,end),schema=text(p,end);Localization l;if(!l.load({reinterpret_cast<const unsigned char*>(records.data()),records.size()},{reinterpret_cast<const unsigned char*>(names.data()),names.size()},{reinterpret_cast<const unsigned char*>(schema.data()),schema.size()},error))throw std::runtime_error(error);if(p!=end)throw std::runtime_error("trailing oracle args");for(unsigned pack=0;pack<9;++pack)for(unsigned sheet=0;sheet<37;++sheet)for(const auto& x:{l.sheet_name(pack,sheet),"text/"+l.sheet_filename(pack,sheet)}){append(output,x.size());output+=x;}}
 else throw std::runtime_error("bad op");return output;
}
}
extern "C" unsigned dh2_localization_test(unsigned op,const unsigned char* bytes,unsigned n,unsigned char* out){try{auto s=execute(op,bytes,n);if(!s.empty())std::memcpy(out,s.data(),s.size());return s.size();}catch(...){return ~0u;}}
#ifndef DH2_LOCALIZATION_ORACLE
int main(int argc,char** argv){try{if(argc!=2)throw std::runtime_error("gold required");std::ifstream f(argv[1],std::ios::binary);std::vector<unsigned char> b((std::istreambuf_iterator<char>(f)),{});const auto* p=b.data();const auto end=p+b.size();if(word(p,end)!=0x31434f4c)throw std::runtime_error("bad gold");auto count=word(p,end);for(unsigned i=0;i<count;++i){auto op=word(p,end),n=word(p,end);auto input=p;p+=n;auto expected=text(p,end);if(execute(op,input,n)!=expected)throw std::runtime_error("gold mismatch "+std::to_string(i));}if(p!=end)throw std::runtime_error("trailing gold");
 LocalizationServices services{};services.constant=constants;std::string output="sentinel",error;bool changed=true;unsigned guards=0;
 if(localization_plain("abc^s",false,output,error)||output!="sentinel")throw std::runtime_error("directive guard");++guards;
 if(localization_colors(std::string("a\0b",3),false,services,output,changed,error)||output!="sentinel"||!changed)throw std::runtime_error("NUL guard");++guards;
 if(localization_player("x",std::string("a\0b",3),false,output,error)||output!="sentinel")throw std::runtime_error("player guard");++guards;
 Localization l;std::uint8_t raw=0;if(l.load({&raw,1},{&raw,1},{&raw,1},error)||l.loaded_sheets()!=0||l.pack()!=-1)throw std::runtime_error("metadata guard");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"atomic_guards\":"<<guards<<",\"mismatches\":0}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
#endif
