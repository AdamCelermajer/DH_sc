#include "../decor_body_config.hpp"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::physical;
static void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
template<class T>static T read(std::ifstream& in){T value{};in.read(reinterpret_cast<char*>(&value),sizeof value);require(bool(in),"truncated fixture");return value;}
static bool floats_equal(const void* a,const void* b,std::size_t n){
 const auto* x=static_cast<const unsigned char*>(a);const auto* y=static_cast<const unsigned char*>(b);
 for(std::size_t i=0;i<n;i+=4){float p,q;std::memcpy(&p,x+i,4);std::memcpy(&q,y+i,4);if(std::memcmp(x+i,y+i,4)&&!(std::isnan(p)&&std::isnan(q)))return false;}return true;
}
static bool equal(const DecorBodyConfig& expected,const DecorBodyConfig& actual){
 const auto* x=reinterpret_cast<const unsigned char*>(&expected);const auto* y=reinterpret_cast<const unsigned char*>(&actual);
 return !std::memcmp(x,y,8)&&!std::memcmp(x+44,y+44,36)&&!std::memcmp(x+136,y+136,16)&&!std::memcmp(x+156,y+156,52)&&!std::memcmp(x+256,y+256,8)&&floats_equal(x+8,y+8,36)&&floats_equal(x+80,y+80,56)&&floats_equal(x+152,y+152,4)&&floats_equal(x+208,y+208,48);
}
int main(int argc,char** argv){try{
 require(argc==2,"decor audit requires original-derived fixture");std::ifstream in(argv[1],std::ios::binary);require(read<std::uint32_t>(in)==0x31434444,"fixture magic");const auto mesh_count=read<std::uint32_t>(in),config_count=read<std::uint32_t>(in);
 float expected_bounds[4],bounds[4];in.read(reinterpret_cast<char*>(expected_bounds),16);require(bool(in),"bounds fixture");require(dh2_decor_level_world_bounds(bounds)==0&&!std::memcmp(expected_bounds,bounds,16),"level bounds differ");
 for(std::uint32_t i=0;i<mesh_count;++i){const auto input=read<DecorMeshBoxInput>(in);float expected[6],actual[6];in.read(reinterpret_cast<char*>(expected),24);require(bool(in),"mesh fixture");require(dh2_decor_marker_mesh_box(actual,&input)==0&&floats_equal(expected,actual,24),"marker mesh differs");}
 std::uint64_t requests=0;
 for(std::uint32_t i=0;i<config_count;++i){const auto input=read<DecorBodyInput>(in);const auto expected=read<DecorBodyConfig>(in);DecorBodyConfig actual{};require(dh2_decor_body_config(&actual,&input)==0,"config rejected");if(!equal(expected,actual)){std::cerr<<"fixture "<<i<<'\n';throw std::runtime_error("decor definition differs");}requests+=actual.physical.request_count;}
 require(in.peek()==std::char_traits<char>::eof(),"fixture suffix");
 DecorBodyInput input{};input.owner=reinterpret_cast<void*>(0x1000000012345678ull);input.new_physical=reinterpret_cast<void*>(0x200000009abcdef0ull);input.visual_present=input.colbox_found=1;DecorBodyConfig out;std::memset(&out,0x5a,sizeof out);const auto before=out;
 require(dh2_decor_body_config(nullptr,&input)==-1,"null output");require(dh2_decor_body_config(&out,nullptr)==-1,"null input");
 for(unsigned field=0;field<4;++field){auto malformed=input;switch(field){case 0:malformed.visual_present=2;break;case 1:malformed.colbox_found=2;break;case 2:malformed.collision_group_override=2;break;default:malformed.disable_physical=2;break;}require(dh2_decor_body_config(&out,&malformed)==-1,"non-boolean metadata");require(!std::memcmp(&before,&out,sizeof out),"rejection mutated output");}
 input.previous_flat=256;require(dh2_decor_body_config(&out,&input)==-1,"invalid byte");input.previous_flat=0;input.new_physical=nullptr;require(dh2_decor_body_config(&out,&input)==-1,"missing physical identity");input.colbox_found=0;input.owner=nullptr;require(dh2_decor_body_config(&out,&input)==-1,"missing owner");input.owner=reinterpret_cast<void*>(1);input.visual_present=0;input.colbox_found=1;require(dh2_decor_body_config(&out,&input)==-1,"marker without visual");require(!std::memcmp(&before,&out,sizeof out),"malformed mutated output");
 require(dh2_decor_level_world_bounds(nullptr)==-1,"null bounds");require(dh2_decor_marker_mesh_box(nullptr,nullptr)==-1,"null marker arguments");
 std::cout<<"{\"mesh_comparisons\":"<<mesh_count<<",\"config_comparisons\":"<<config_count<<",\"service_requests\":"<<requests<<",\"malformed_checks\":12,\"mismatches\":0,\"sanitizers\":\"address,undefined\",\"optimization\":\"O1,no fast math,fp-contract off\"}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
