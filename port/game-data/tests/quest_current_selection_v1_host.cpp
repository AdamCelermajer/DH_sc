#include "../quest_current_selection_v1.hpp"
#include <cassert>
#include <string>
#include <vector>
int main(){
 using namespace dh2::data;
 PlayerSavegameV1 save;save.set_character(UINT64_C(0x12345678));std::string error;
 unsigned transport_writes=0;
 auto transport=[&](PlayerSavegameV1& selected,std::string& save_error){
  assert(&selected==&save&&selected.character()==UINT64_C(0x12345678));
  ++transport_writes;save_error.clear();return true;
 };
 assert(quest_current_selection_v1::set_and_persist(save,false,1,42,transport,error));
 assert(transport_writes==1);
 assert(save.source_quest_log_b8().word_2c[1]==42&&save.source_quest_log_118().word_2c[1]==-1);
 assert(quest_current_selection_v1::is_current(save,1,42));
 assert(!quest_current_selection_v1::is_current(save,1,41));
 assert(!quest_current_selection_v1::is_current(save,-1,42));
 assert(!quest_current_selection_v1::is_current(save,3,42));
 save.source_quest_log_b8().word_38[1]=42;
 save.source_quest_log_b8().word_2c[1]=41;
 assert(!quest_current_selection_v1::is_current(save,1,42));
 save.source_quest_log_b8().word_2c[1]=42;
 save.source_quest_log_b8().word_38[1]=-1;
 assert(quest_current_selection_v1::is_current(save,1,42));
 assert(quest_current_selection_v1::set_and_persist(save,false,1,42,transport,error));
 assert(transport_writes==1);
 assert(!quest_current_selection_v1::set_and_persist(save,true,1,19,transport,error));
 assert(transport_writes==1&&save.source_quest_log_b8().word_2c[1]==42&&!error.empty());
 assert(!quest_current_selection_v1::set_and_persist(save,false,3,19,transport,error));
 assert(transport_writes==1&&save.source_quest_log_b8().word_2c[1]==42&&!error.empty());
 auto failed_transport=[&](PlayerSavegameV1& selected,std::string& save_error){
  assert(&selected==&save&&selected.character()==UINT64_C(0x12345678));
  ++transport_writes;save_error="fixture PlayerSavegame transport failure";return false;
 };
 assert(!quest_current_selection_v1::set_and_persist(save,false,1,77,failed_transport,error));
 assert(transport_writes==2&&save.source_quest_log_b8().word_2c[1]==77&&
        error=="fixture PlayerSavegame transport failure");

 struct QuestRow {std::int32_t id,state,priority;};
 const std::vector<QuestRow> rows{{0,5,2},{1,6,0},{2,6,2}};
 save.source_quest_log_b8().word_2c[1]=1;
 assert(quest_current_selection_v1::verify_current(save,1,rows,2)==1);
 assert(save.source_quest_log_b8().word_2c[1]==1);
 save.source_quest_log_b8().word_2c[1]=0;
 const auto fallback=quest_current_selection_v1::verify_current(save,1,rows,2);
 assert(fallback==2&&save.source_quest_log_b8().word_2c[1]==2);
 assert(rows[std::size_t(fallback)].id==fallback);
 save.source_quest_log_b8().word_2c[1]=0;
 const std::vector<QuestRow> no_active_primary{{0,5,2},{1,6,0}};
 assert(quest_current_selection_v1::verify_current(save,1,no_active_primary,2)==0);
 assert(save.source_quest_log_b8().word_2c[1]==0);
}
