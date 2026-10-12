#include "../edit_text_event_v1.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <vector>
using namespace dh2::ui::edit_text_event_v1;
using Blob=std::vector<unsigned char>;
unsigned word(const Blob&b,std::size_t&p){if(p+4>b.size())throw std::runtime_error("truncated gold");unsigned x;std::memcpy(&x,b.data()+p,4);p+=4;return x;}
void append(Blob&b,unsigned x){auto p=b.size();b.resize(p+4);std::memcpy(b.data()+p,&x,4);}
Blob take(const Blob&b,std::size_t&p,std::size_t n){if(p+n>b.size())throw std::runtime_error("truncated gold");Blob r(b.begin()+p,b.begin()+p+n);p+=n;return r;}
std::string string(const Blob&b,std::size_t&p){auto r=take(b,p,word(b,p));return {r.begin(),r.end()};}
void text(Blob&b,const std::string&s){append(b,s.size());b.insert(b.end(),s.begin(),s.end());}
int main(int argc,char**argv){try{
 if(argc!=2)throw std::runtime_error("usage: edit_text_event_v1 event-gold.bin");std::ifstream in(argv[1],std::ios::binary);Blob b((std::istreambuf_iterator<char>(in)),{});std::size_t p{};if(word(b,p)!=0x31564554)throw std::runtime_error("bad gold");auto n=word(b,p);unsigned guards{};
 for(unsigned c=0;c<n;++c){auto flags=word(b,p),id=word(b,p),key=word(b,p),cursor=word(b,p),mutate=word(b,p);State q{bool(flags&1),bool(flags&2),std::int32_t(cursor),string(b,p)};auto expected=take(b,p,word(b,p));Blob events;
  Services s;s.active=[&](auto&){append(events,1);if(mutate&1){q.focus=true;q.text="active";}return true;};s.handler=[&](const char*name,auto&){append(events,2);text(events,name);if(mutate&2)q.text="handler";return true;};
  s.listener=[&](bool add,auto&){append(events,add?3:4);if(mutate&4)q.text="listener";return true;};s.format=[&](auto&){append(events,5);append(events,q.cursor);text(events,q.text);return true;};s.set_value=[&](const auto&value,auto&){append(events,6);append(events,q.cursor);text(events,value);q.text=value;return true;};
  bool accepted{};std::string e;if(!event(q,id,key,s,accepted,e))throw std::runtime_error(e);Blob out;append(out,accepted);append(out,q.focus);append(out,q.cursor);text(out,q.text);append(out,events.size());out.insert(out.end(),events.begin(),events.end());
  if(out!=expected)throw std::runtime_error("event trace mismatch case "+std::to_string(c));
 }
 if(p!=b.size())throw std::runtime_error("trailing gold");State q;bool accepted{};std::string e;Services s;if(event(q,20,0,s,accepted,e))throw std::runtime_error("missing active accepted");++guards;
 q.text="A";q.cursor=-1;s.set_value=[](auto&,auto&){return true;};if(event(q,8,65,s,accepted,e))throw std::runtime_error("unsafe cursor accepted");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<n<<",\"guards\":"<<guards<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
