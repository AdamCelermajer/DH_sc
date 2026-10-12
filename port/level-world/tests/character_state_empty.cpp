#include "../character_state_empty.hpp"
#include <cstdio>
#include <fstream>
#include <stdexcept>
static void check(bool x){if(!x)throw std::runtime_error("empty state method mismatch");}
template<class T>T read(std::ifstream& f){T v{};f.read(reinterpret_cast<char*>(&v),sizeof v);check(bool(f));return v;}
int main(int argc,char** argv){try{check(argc==2);std::ifstream f(argv[1],std::ios::binary);check(read<unsigned>(f)==0x314d4553);const auto count=read<unsigned>(f);unsigned empty=0,required=0,guards=0;
 for(unsigned i=0;i<count;++i){const auto state=read<unsigned>(f),op=read<unsigned>(f),source=read<unsigned>(f),expected=read<unsigned>(f);check(dh2_character_state_empty_body(state,op,source)==int(expected));check(dh2_character_state_empty_body(state,op,source^4)==-1);check(dh2_character_state_empty_body(state,4,source)==-1);guards+=2;if(expected)++empty;else ++required;}
 check(f.peek()==std::ifstream::traits_type::eof());check(empty==18&&required==46);check(dh2_character_state_empty_body(-1,0,0)==-1&&dh2_character_state_empty_body(3,3,0x3c0004)==-1);guards+=2;
 std::printf("{\"validation\":\"PASS\",\"method_metadata_comparisons\":%u,\"empty_methods\":%u,\"required_nonempty_methods\":%u,\"invalid_metadata_checks\":%u,\"mismatches\":0}\n",count,empty,required,guards);return 0;}catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
