#include "../player_save_collections_writer_v45.hpp"
#include <cassert>
#include <iostream>
using namespace dh2;
int main(){unsigned checks=0;std::string e;auto lease=std::make_shared<int>(1);
 std::vector<std::string> levels{"LevelA","LevelB"},maps{"ActA"};std::array<std::vector<std::int32_t>,3> states{{{1,2},{3,4},{5,6}}},acts{{{7},{8},{9}}};
 level::SavegameStreamV2 lvls;assert(level::player_save_level_states_v45({lease,&levels,&maps,&states,&acts},lvls,e));
 for(unsigned kind=0;kind<2;++kind)for(unsigned tier=0;tier<3;++tier){unsigned count{};assert(lvls.read_u32(count,e)&&count==(kind?1:2));for(unsigned i=0;i<count;++i){std::string name;unsigned value{};assert(lvls.read_string(name,e)&&name==(kind?maps:levels)[i]);assert(lvls.read_u32(value,e)&&value==unsigned((kind?acts:states)[tier][i]));checks+=2;}}
 states[2].clear();level::SavegameStreamV2 invalid;assert(!level::player_save_level_states_v45({lease,&levels,&maps,&states,&acts},invalid,e)&&invalid.size()>0);++checks;
 std::array<std::uint64_t,3> bits{{1,0x8000000000000000ull,0xaaaaaaaa55555555ull}};level::SavegameStreamV2 travel;assert(level::player_save_fast_travel_v45(&bits,travel,e));for(unsigned tier=0;tier<3;++tier){std::string value;assert(travel.read_string(value,e)&&value.size()==64);for(unsigned bit=0;bit<64;++bit)assert(value[63-bit]==((bits[tier]>>bit)&1?'1':'0'));checks+=64;}
 std::array<std::vector<std::uintptr_t>,3> quests{{{101},{102},{103}}};data::SavedQuestProgressV1 progress;
 level::QuestSaveCollectionBorrowV45 q{lease,&quests,&progress,[](std::uintptr_t id,auto& out,auto& error){return out.write_u32(unsigned(id),error);}}; // explicit Quest data leaf fixture
 level::SavegameStreamV2 payload;assert(level::player_save_quests_v45(1,q,{},payload,e));data::QuestSavegameV1 restored;assert(restored.attach_initialized_quests(quests,e));std::size_t used{};std::array<bool,3> mismatch{};unsigned leaves{};
 data::QuestLoadServicesV1 reader{&leaves,[](void* p,std::uintptr_t id,data::Bytes bytes,bool,std::size_t& used,std::string&){if(bytes.size<4)return false;unsigned word=unsigned(bytes.data[0])|unsigned(bytes.data[1])<<8|unsigned(bytes.data[2])<<16|unsigned(bytes.data[3])<<24;assert(word==id);used=4;++*static_cast<unsigned*>(p);return true;}};
 assert(restored.load({payload.bytes().data(),payload.bytes().size()},reader,used,mismatch,e)&&used==payload.size()&&leaves==3);assert(restored.progress().current_quest==progress.current_quest&&restored.progress().current_act==progress.current_act);checks+=4;
 q.save_quest={};level::SavegameStreamV2 failed;assert(!level::player_save_quests_v45(1,q,{},failed,e)&&failed.size()==8);++checks; // count+index source prefix
 level::SavegameStreamV2 wrong;assert(!level::player_save_quests_v45(2,q,{},wrong,e)&&wrong.size()==0);++checks;
 std::cout<<"Source LVLS/FTVL/QuestSavegame collection writers PASS "<<checks<<"; borrowed real-shaped cells; Quest condition/objective data leaf fixture, no live quest authority claim\n";
}
