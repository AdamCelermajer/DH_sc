#include "../edit_text_definition_v1.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <stdexcept>
using Blob=std::vector<unsigned char>;
unsigned word(const Blob&b,std::size_t&p){if(p+4>b.size())throw std::runtime_error("truncated gold");unsigned x;std::memcpy(&x,b.data()+p,4);p+=4;return x;}
std::string text(const Blob&b,std::size_t&p){auto n=word(b,p);if(p+n>b.size())throw std::runtime_error("truncated text");std::string s(b.begin()+p,b.begin()+p+n);p+=n;return s;}
int main(int argc,char**argv){try{if(argc!=2)throw std::runtime_error("usage: edit_text_definition_v1 gold.bin");std::ifstream f(argv[1],std::ios::binary);Blob b((std::istreambuf_iterator<char>(f)),{});std::size_t p{};if(word(b,p)!=0x31454454)throw std::runtime_error("bad gold");auto n=word(b,p);std::string error;
 for(unsigned i=0;i<n;++i){auto input=text(b,p),expected=text(b,p);if(!dh2::ui::edit_text_definition_v1::remove_html(input,error)||input!=expected)throw std::runtime_error("removeHTML mismatch "+std::to_string(i)+" "+error);}if(p!=b.size())throw std::runtime_error("trailing gold");
 for(auto s:{std::string("unsafe</"),std::string("<p>")+std::string(512,'x')+"</p>"}){auto original=s;if(dh2::ui::edit_text_definition_v1::remove_html(s,error)||s!=original)throw std::runtime_error("unsafe source text accepted/modified");}
 std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<n<<",\"unsafe_guards\":2,\"mismatches\":0}\n";return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
