#include "../text_filter_v1.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::ui::text_filter_v1;
using Bytes=std::vector<std::uint8_t>;
static void check(bool b,const char* e){if(!b)throw std::runtime_error(e);}
static std::uint32_t word(std::ifstream& f){unsigned char p[4];check(bool(f.read(reinterpret_cast<char*>(p),4)),"truncated filter corpus");return p[0]|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
static void put(Bytes& b,std::uint32_t w){for(unsigned i=0;i<4;++i)b.push_back((w>>(8*i))&255);}
int main(int argc,char** argv){try{
 check(argc==2,"usage: filter audit corpus");std::ifstream f(argv[1],std::ios::binary);check(bool(f)&&word(f)==0x31465445,"missing original filter corpus");auto count=word(f);
 for(unsigned i=0;i<count;++i){
  auto n=word(f);check(n<1000,"unbounded primitive count");std::vector<std::array<std::uint32_t,3>> inputs(n);for(auto& q:inputs)for(auto& w:q)w=word(f);
  n=word(f);check(n<100000,"unbounded output");Bytes expected(n);check(bool(f.read(reinterpret_cast<char*>(expected.data()),n)),"truncated filter output");
  Effect e;Reader r;std::string error;Bytes events;unsigned cursor=0;
  auto next=[&](unsigned op,unsigned arg){check(cursor<inputs.size(),"extra stream call");auto q=inputs[cursor++];check(q[0]==op&&q[1]==arg,"stream call order");for(auto w:q)put(events,w);return q[2];};
  r.byte=[&](auto& o,std::string&){o=next(1,0);return true;};r.half=[&](auto& o,std::string&){o=next(2,0);return true;};
  r.bits=[&](auto bits,auto& o,std::string&){o=next(3,bits);return true;};r.bit=[&](auto& o,std::string&){o=next(4,0);return true;};
  r.fixed=[&](auto& o,std::string&){auto w=next(5,0);std::memcpy(&o,&w,4);return true;};
  r.rgba=[&](auto& o,std::string&){auto w=next(6,0);for(unsigned j=0;j<4;++j)o[j]=(w>>(8*j))&255;return true;};
  check(read(e,r,error),error.c_str());check(cursor==inputs.size(),"unconsumed stream input");Bytes actual;put(actual,e.filters.size());
  for(auto& q:e.filters){actual.insert(actual.end(),q.defined.begin(),q.defined.end());for(unsigned j=0;j<44;++j)actual.push_back(q.defined[j]?q.bytes[j]:0);}
  put(actual,events.size());actual.insert(actual.end(),events.begin(),events.end());
  if(actual!=expected){std::cerr<<"filter case "<<i<<" bytes "<<actual.size()<<"/"<<expected.size()<<'\n';throw std::runtime_error("original filter mismatch");}
 }
 check(f.peek()==std::char_traits<char>::eof(),"trailing filter corpus");unsigned guards=0;Effect e;Reader r;std::string error;
 check(!read(e,r,error)&&e.filters.empty(),"missing stream accepted");++guards;
 auto source=std::make_shared<Effect>();source->blend=8;Binding b;b.place(source);auto weak=std::weak_ptr<Effect>(source);source.reset();check(!weak.expired()&&b.value()->blend==8,"tag lease lost");++guards;
 auto next=std::make_shared<Effect>();next->blend=4;b.move({});check(b.value()->blend==8,"null move replaced current effect");++guards;
 b.move(next);check(weak.expired()&&b.value()==next,"source move order/lease wrong");++guards;
 check(b.set(*next,error),"copy failed");next->blend=7;check(b.value()!=next&&b.value()->blend==4,"explicit setter retained source alias");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"original_filter_cases\":"<<count<<",\"ownership_guards\":"<<guards<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
