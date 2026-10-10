#include "../native_quest_runtime_v76.hpp"
#include <iostream>
#include <stdexcept>

int main(){
 using namespace dh2;
 unsigned checks{};
 const auto check=[&](bool value,const char* message){++checks;if(!value)throw std::runtime_error(message);};
 try{
  const std::uintptr_t owner=0x1234;
  const std::vector<data::QuestRewardDefinitionV51> rows{{0,10,0},{4,2,7},{1,50,0}};
  std::vector<int> visited;std::string error;
  auto give=[&](std::uintptr_t actual,const auto& row,bool& result,std::string&){
   check(actual==owner,"same retained Quest character owner");
   visited.push_back(row.type);
   // ConsumeLoot's actual virtual Give returns zero in both source branches.
   result=row.type!=4;
   return true;
  };
  check(world::quest_reward_sequence_v108(owner,rows,give,error),"source RewardList::Give sequence succeeds");
  check(visited==std::vector<int>({0,4}),"false Reward::Give stops before later authored reward");

  visited.clear();
  auto failing=[&](std::uintptr_t,const auto& row,bool& result,std::string& e){
   visited.push_back(row.type);
   if(row.type==4){e="actual reward provider failure";return false;}
   result=true;return true;
  };
  check(!world::quest_reward_sequence_v108(owner,rows,failing,error),"provider failure is propagated");
  check(visited==std::vector<int>({0,4})&&error=="actual reward provider failure","failure preserves exact reached prefix");
  check(!world::quest_reward_sequence_v108(owner,rows,{},error)&&!error.empty(),"missing reward provider fails explicitly");
  std::cout<<"PASS RewardList::Give stop/propagation checks="<<checks<<'\n';
  return 0;
 }catch(const std::exception& exception){std::cerr<<exception.what()<<'\n';return 1;}
}
