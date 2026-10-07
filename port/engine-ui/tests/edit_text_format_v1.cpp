#include "../edit_text_format_v1.hpp"
#define main frozen_text_layout_audit_main
#include "text_layout_v1.cpp"
#undef main
namespace field=dh2::ui::edit_text_v1;
int main(int argc,char** argv){try{
 check(argc==2,"usage: format audit corpus");std::ifstream f(argv[1],std::ios::binary);check(bool(f)&&read(f)==0x314d4654,"missing format corpus");auto count=read(f);
 for(unsigned i=0;i<count;++i){
  unsigned cfg[22];for(auto& q:cfg)q=read(f);auto mutate=read(f);auto size=read(f);check(size<10000,"unbounded output");Bytes expected(size);check(bool(f.read(reinterpret_cast<char*>(expected.data()),size)),"truncated output");
  field::State q;q.definition=std::make_shared<field::Definition>();q.layout.font=std::make_shared<Font>();q.layout.font->name="Fontin";
  q.layout.font->bold=cfg[20];q.layout.font->italic=cfg[21];
  q.layout.left=real(cfg[12]);q.layout.indent=real(cfg[13]);q.layout.right=real(cfg[14]);q.layout.leading=real(cfg[15]);q.layout.letter_spacing=real(cfg[16]);q.layout.text_height=real(cfg[17]);q.layout.alignment=cfg[18];q.layout.rgba=cfg[19];
  Bytes actual;field::FormatWriter w;w.construct=[&](std::string&){word(actual,1);return true;};
  w.intern=[&](const auto& name,auto& out,std::string&){word(actual,2);text(actual,name);out=name;return true;};
  w.write=[&](const char* name,const field::FormatValue& value,std::string&){
   word(actual,3);text(actual,name);
   if(auto* n=std::get_if<double>(&value)){word(actual,2);std::uint64_t bits;std::memcpy(&bits,n,8);word(actual,bits&0xffffffff);word(actual,bits>>32);}
   else if(auto* b=std::get_if<bool>(&value)){word(actual,1);word(actual,*b);}
   else{word(actual,3);text(actual,std::get<std::string>(value));}
   if(mutate&&std::string(name)=="leftMargin")q.layout.indent=-7.5f;
   if(mutate&&std::string(name)=="font")q.layout.font->bold=true;
   if(mutate&&std::string(name)=="bold")q.layout.font->italic=true;
   return true;
  };
  std::string error;check(field::get_format(q,w,error),error);
  if(actual!=expected){std::size_t p=0;while(p<std::min(actual.size(),expected.size())&&actual[p]==expected[p])++p;
   std::cerr<<"format case "<<i<<" at "<<p<<" actual "<<actual.size()<<"/"<<expected.size()<<'\n';
   for(auto j=p;j<std::min(p+16,std::min(actual.size(),expected.size()));++j)std::cerr<<j<<":"<<unsigned(actual[j])<<"/"<<unsigned(expected[j])<<" ";
   throw std::runtime_error("source format write/reentry mismatch");}
 }
 check(f.peek()==std::char_traits<char>::eof(),"trailing corpus");unsigned guards=0;field::State q;field::FormatWriter w;std::string error;
 check(!field::get_format(q,w,error),"missing format services accepted");++guards;
 q.definition=std::make_shared<field::Definition>();unsigned writes=0;
 w.construct=[](std::string&){return true;};w.intern=[](const auto& n,auto& out,std::string&){out=n;return true;};w.write=[&](auto,auto&,std::string&){++writes;return true;};
 check(!field::get_format(q,w,error)&&writes==8,"null font failed to preserve property prefix");++guards;
 q.layout.font=std::make_shared<Font>();writes=0;w.write=[&](const char* n,auto&,std::string&){++writes;if(std::string(n)=="font")q.layout.font.reset();return true;};
 check(!field::get_format(q,w,error)&&writes==9,"reentered null font accepted or prefix lost");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"original_get_format_cases\":"<<count<<",\"required_prefix_guards\":"<<guards<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
