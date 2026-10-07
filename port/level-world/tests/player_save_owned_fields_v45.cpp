#include "../player_save_collections_writer_v45.hpp"
#include "../player_save_inventory_writer_v45.hpp"
#include <cassert>
#include <iostream>
using namespace dh2;
int main(){unsigned checks=0;std::string e;data::PlayerSavegameV1 save;save.set_character(1);
 assert((*save.source_property_tail194_v45()==0&&save.fast_travel_v45()==std::array<std::uint64_t,3>{}));++checks;
 save.load_property_tail194_v45(255);assert(*save.source_property_tail194_v45()==255);++checks;
 assert(save.initialize_level_states_v45({2,4},{6},e));for(unsigned i=0;i<3;++i){assert(save.level_states_v45()[i]==std::vector<int>({2,4})&&save.map_states_v45()[i]==std::vector<int>({6}));++checks;}
 auto lease=std::make_shared<int>(1);std::vector<std::string> levels{"L0","L1"},maps{"M0"};
 std::array<std::vector<int>,3> live{{{9,8},{7,6},{5,4}}},maplive{{{3},{2},{1}}};level::SavegameStreamV2 bytes;assert(level::player_save_level_states_v45({lease,&levels,&maps,&live,&maplive},bytes,e));std::size_t used{};assert(save.load_level_states_v45({bytes.bytes().data(),bytes.bytes().size()},levels,maps,used,e)&&used==bytes.size());assert(save.level_states_v45()==live&&save.map_states_v45()==maplive);checks+=2;
 assert(save.initialize_level_states_v45({100,200},{300},e)&&save.level_states_v45()==live&&save.map_states_v45()==maplive&&*save.source_property_tail194_v45()==255);++checks;
 std::array<std::uint64_t,3> bits{{1,2,0x8000000000000000ull}};level::SavegameStreamV2 travel;assert(level::player_save_fast_travel_v45(&bits,travel,e));assert(save.load_fast_travel_v45({travel.bytes().data(),travel.bytes().size()},used,e)&&save.fast_travel_v45()==bits);++checks;
 level::SavegameStreamV2 longtext;assert(longtext.write_string(std::string(65,'1'),e));assert(save.load_fast_travel_v45({longtext.bytes().data(),longtext.bytes().size()},used,e)&&used==longtext.size()&&save.fast_travel_v45()==bits);++checks;
 data::PropertyRules rules;rules.types[19]=32;data::PropertyState original,restored;original.saved[19]=512;original.saved[20]=999;restored.saved[20]=77;auto from=data::property_view(rules,original),to=data::property_view(rules,restored);level::SavegameStreamV2 props;const std::uint8_t flag=41;assert(level::player_save_properties_writer_v45(from,&flag,props,e));assert(level::player_save_properties_reader_v45(save,to,{props.bytes().data(),props.bytes().size()},used,e)&&used==901);assert(restored.saved[19]==512&&restored.saved[20]==77&&*save.source_property_tail194_v45()==41);checks+=2;
 const std::uint8_t wrongcount[]{1,0,0,0};assert(level::player_save_properties_reader_v45(save,to,{wrongcount,4},used,e)&&used==4&&*save.source_property_tail194_v45()==41);++checks;
 assert(!level::player_save_properties_reader_v45(save,to,{props.bytes().data(),900},used,e)&&restored.saved[19]==512&&*save.source_property_tail194_v45()==41);++checks;
 assert(&save.regular_quests_v45()!=&save.volatile_quests_v45());++checks;
 std::cout<<"SAME Save source194/LevelMap/FTVL/Quest fields +restore-prefix/no-replay PASS "<<checks<<"; authored initialization values fixture\n";
}
