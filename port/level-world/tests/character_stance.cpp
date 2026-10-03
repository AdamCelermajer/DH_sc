#include "character_stance.hpp"
#include <cstdio>
#include <fstream>
#include <stdexcept>
using namespace dh2::character;
namespace {
void check(bool yes){if(!yes)throw std::runtime_error("stance mismatch");}
template<class T>T read(std::ifstream& in){T x{};in.read(reinterpret_cast<char*>(&x),sizeof x);check(bool(in));return x;}
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream in(argv[1],std::ios::binary);check(bool(in));check(read<std::uint32_t>(in)==0x31415453);auto count=read<std::uint32_t>(in);
 for(unsigned i=0;i<count;++i){StanceFacts16 facts;facts.predicates=read<std::uint32_t>(in);facts.count=read<std::int32_t>(in);auto expected=read<std::int32_t>(in);int result=-77;check(dh2_character_anim_stance(&result,&facts)==1&&result==expected);}
 check(in.peek()==std::ifstream::traits_type::eof());unsigned malformed=0;StanceFacts16 facts;int output=99;
 check(dh2_character_anim_stance(&output,nullptr)==-1&&output==99);++malformed;
 check(dh2_character_anim_stance(nullptr,&facts)==-1);++malformed;
 for(unsigned i=0;i<3;++i){facts={};if(i==0)facts.predicates=64;else facts.reserved[i-1]=1;check(dh2_character_anim_stance(&output,&facts)==-1&&output==99);++malformed;}
 facts={stance_is_player,5,{0,0}};check(dh2_character_anim_stance(&output,&facts)==1&&output==0);
 facts.count=6;check(dh2_character_anim_stance(&output,&facts)==1&&output==5);
 std::printf("{\"original_reference_cases\":%u,\"malformed_no_mutation_cases\":%u,\"bare_count5_clamps0_count6_keeps5\":true,\"mismatches\":0}\n",count,malformed);return 0;
}catch(const std::exception& e){std::fprintf(stderr,"stance audit: %s\n",e.what());return 1;}}
