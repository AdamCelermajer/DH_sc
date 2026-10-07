#include "../player_save_metadata_writer_v45.hpp"
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <cstring>
using namespace dh2;
int main(){std::ifstream in("port/level-world/reference/campaign-save-v45/metadata-original.bin",std::ios::binary);assert(in);unsigned count{};in.read(reinterpret_cast<char*>(&count),4);assert(count==320);std::string e;
 for(unsigned index=0;index<count;++index){char tag[5]{};std::array<std::uint32_t,19> row{};unsigned n{};in.read(tag,4);in.read(reinterpret_cast<char*>(row.data()),76);in.read(reinterpret_cast<char*>(&n),4);assert(in&&n<=1000);std::vector<std::uint8_t> gold(n);in.read(reinterpret_cast<char*>(gold.data()),n);assert(in);
  auto save=std::make_shared<data::PlayerSavegameV1>();std::int32_t classid{};std::memcpy(&classid,&row[0],4);save->set_player_class(classid);
  level::SavegameStreamV2 location;assert(location.write_u32(row[3],e));for(unsigned i=0;i<3;++i)assert(location.write_u32(row[4+i],e)&&location.write_u32(row[7+i],e)&&location.write_u32(row[10+i],e));std::size_t consumed{};assert(save->load_location({location.bytes().data(),location.bytes().size()},consumed,e));
  level::SavegameStreamV2 entries;for(unsigned i=0;i<3;++i)assert(entries.write_u32(row[13+i],e));assert(save->load_entry_points({entries.bytes().data(),entries.bytes().size()},consumed,e));
  std::uint8_t spawn[3]{std::uint8_t(row[16]),std::uint8_t(row[17]),std::uint8_t(row[18])};assert(save->load_spawn_points({spawn,3},consumed,e));
  level::SavegameStreamV2 difficulty;assert(difficulty.write_u32(row[1],e)&&difficulty.write_u32(row[2],e));assert(save->load_difficulty({difficulty.bytes().data(),difficulty.bytes().size()},nullptr,[](void*,int,std::string&){return true;},consumed,e));
  auto authority=std::make_shared<data::PlayerSaveLoadOwnerV1>(save);auto lease=std::make_shared<int>(1);data::CharacterTable names;names.names={"KnightPlayerBase","RoguePlayerBase","MagePlayerBase"};
  level::PlayerSaveMetadataWriterV45 writer(authority,{lease,&names,[&](int& out,std::string&){out=int(row[1]);return true;}});level::SavegameStreamV2 stream;assert(writer.write(tag,stream,e));
  if(stream.bytes()!=gold){std::cerr<<"Metadata original mismatch "<<index<<" "<<tag<<" native "<<stream.size()<<" original "<<gold.size()<<'\n';return 1;}
 }
 assert(in.peek()==std::char_traits<char>::eof());std::cout<<"Whole original metadata writer replay PASS "<<count<<" exact byte cases; original stream/CString leaves fixture\n";
}
