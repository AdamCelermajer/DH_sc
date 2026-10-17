#include "../player_saved_fast_travel_v1.hpp"
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <vector>
using namespace dh2::data;
namespace travel=dh2::data::player_saved_fast_travel_v1;
std::vector<std::uint8_t> payload(const std::vector<std::string>& strings){std::vector<std::uint8_t> out;for(const auto& value:strings){const auto n=std::uint32_t(value.size()+1);for(unsigned j=0;j<4;++j)out.push_back(std::uint8_t(n>>(j*8)));out.insert(out.end(),value.begin(),value.end());out.push_back(0);}return out;}
unsigned policies(){unsigned checks=0;const auto check=[&](bool value){if(!value)throw std::runtime_error("saved fast-travel policy failed at "+std::to_string(checks));++checks;};PlayerSavegameV1 save;travel::Runtime runtime(&save);travel::Result result;std::string error;
 for(unsigned d=0;d<3;++d)check(*save.source_fast_travel_bits(d)==std::array<std::uint32_t,2>{0,0});
 check(!save.source_fast_travel_bits(3)&&!static_cast<const PlayerSavegameV1&>(save).source_fast_travel_bits(3));
 const auto valid=payload({"1",std::string(33,'1'),std::string(64,'1')});
 for(std::size_t n=0;n<valid.size();++n){PlayerSavegameV1 s;travel::Runtime r(&s);check(r.load({valid.data(),n},&result,error)==travel::Status::failed);check(result.consumed<=n);check((*s.source_fast_travel_bits(0))[0]==(n>=6?1u:0u));check((*s.source_fast_travel_bits(1))[1]==(n>=44?1u:0u));}
 check(runtime.load({valid.data(),valid.size()},&result,error)==travel::Status::complete);check(result.word_stores==6&&result.completed_difficulties==3);check(*save.source_fast_travel_bits(2)==std::array<std::uint32_t,2>{UINT32_MAX,UINT32_MAX});
 const auto oversized=payload({"0",std::string(65,'1'),"0"});check(runtime.load({oversized.data(),oversized.size()},&result,error)==travel::Status::complete);check(result.decision==travel::Decision::oversized_stopped&&result.completed_difficulties==1&&result.string_reads==2&&result.consumed==76);check((*save.source_fast_travel_bits(0))[0]==0&&(*save.source_fast_travel_bits(1))[0]==UINT32_MAX&&(*save.source_fast_travel_bits(2))[0]==UINT32_MAX);
 const auto bad=payload({"1","1X1","0"});check(runtime.load({bad.data(),bad.size()},&result,error)==travel::Status::failed);check(result.characters==3&&result.completed_difficulties==1&&(*save.source_fast_travel_bits(0))[0]==1&&(*save.source_fast_travel_bits(1))[0]==UINT32_MAX);check(runtime.load({valid.data(),valid.size()},&result,error)==travel::Status::complete);
 for(std::uint32_t n:{0u,UINT32_MAX,1048577u}){check(runtime.load({reinterpret_cast<const std::uint8_t*>(&n),4},&result,error)==travel::Status::failed);check(result.consumed==4&&!result.word_stores);}
 auto broken=valid;broken[5]='X';check(runtime.load({broken.data(),broken.size()},&result,error)==travel::Status::failed);check(result.consumed==6&&!result.word_stores);
 const auto before=result;check(runtime.load({valid.data(),valid.size()},nullptr,error)==travel::Status::invalid_argument);check(runtime.load({nullptr,1},&result,error)==travel::Status::invalid_argument);check(runtime.load({reinterpret_cast<const std::uint8_t*>(save.source_fast_travel_bits(0)),8},&result,error)==travel::Status::invalid_argument);check(std::memcmp(&before,&result,sizeof(result))==0);
 bool rejected=false;try{travel::Runtime missing(nullptr);}catch(const std::invalid_argument&){rejected=true;}check(rejected);return checks;
}
int main(int argc,char** argv){try{
 if(argc!=3)return 2;
 auto* input=std::fopen(argv[1],"rb");auto* output=std::fopen(argv[2],"wb");if(!input||!output)return 3;std::uint32_t count;if(std::fread(&count,4,1,input)!=1)return 4;
 for(std::uint32_t i=0;i<count;++i){std::uint32_t size;if(std::fread(&size,4,1,input)!=1)return 5;std::vector<std::uint8_t> bytes(size);if(size&&std::fread(bytes.data(),1,size,input)!=size)return 6;PlayerSavegameV1 save;for(unsigned d=0;d<3;++d)*save.source_fast_travel_bits(d)={0x10203040u+d,0x50607080u+d};travel::Runtime runtime(&save);travel::Result result;std::string error;const auto status=runtime.load({bytes.data(),bytes.size()},&result,error);const std::uint32_t fields[]{status==travel::Status::complete?0u:1u,result.consumed,result.read_calls,result.string_reads,result.characters,result.word_stores,result.completed_difficulties,std::uint32_t(result.decision)};std::fwrite(fields,sizeof(fields),1,output);for(unsigned d=0;d<3;++d)std::fwrite(save.source_fast_travel_bits(d)->data(),4,2,output);}
 std::fclose(input);std::fclose(output);std::printf("%u\n",policies());return 0;
}catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 9;}}
