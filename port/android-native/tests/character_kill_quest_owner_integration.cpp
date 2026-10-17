#include "../app/src/main/cpp/native_quest_owner.hpp"
#include "../app/src/main/cpp/native_quest_cursor.hpp"
#include "../../level-world/character_kill_quest_tail_v1.hpp"
#include <fstream>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <vector>

namespace d=dh2::data;
namespace n=dh2::native::quests;
namespace t=d::quest_table_bindings_v1;
namespace k=dh2::character_kill_quest_tail_v1;

void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
auto read(const std::string& path){
 std::ifstream f(path,std::ios::binary|std::ios::ate);check(bool(f),"open cache input");
 auto bytes=std::make_shared<std::vector<std::uint8_t>>(std::size_t(f.tellg()));f.seekg(0);
 f.read(reinterpret_cast<char*>(bytes->data()),std::streamsize(bytes->size()));check(bool(f),"read cache input");
 return std::shared_ptr<const std::vector<std::uint8_t>>(bytes);
}
struct Context {
 n::Owner* owner=nullptr;
 const dh2_pycst_view* constants=nullptr;
 std::uintptr_t character=0,module_id=0,owner_token=0;
 std::int16_t character_table_id=-1,template_id=-1;
 std::vector<k::Event> delivered;
};
bool current_level(void* raw,std::uintptr_t& value,std::string& error){
 auto& c=*static_cast<Context*>(raw);value=c.owner_token;error.clear();return value!=0;
}
bool character_word25(void* raw,std::uintptr_t character,std::uintptr_t& value,std::string& error){
 auto& c=*static_cast<Context*>(raw);if(character!=c.character){error="wrong Character identity";return false;}
 value=c.module_id;error.clear();return true;
}
bool character_halfword(void* raw,std::uintptr_t character,std::uint32_t index,std::int16_t& value,std::string& error){
 auto& c=*static_cast<Context*>(raw);if(character!=c.character){error="wrong Character identity";return false;}
 if(index==2532)value=c.character_table_id;else if(index==2533)value=c.template_id;
 else{error="unexpected Character halfword";return false;}
 error.clear();return true;
}
bool get_constant(void* raw,const char* group,const char* key,std::int32_t& value,std::string& error){
 auto& c=*static_cast<Context*>(raw);dh2_pycst_result result{};
 if(!group||!key||std::string(group)!="v2QuestObjectiveType"||
    dh2_pycst_get(c.constants,group,std::uint32_t(std::strlen(group)),key,
                  std::uint32_t(std::strlen(key)),&result)||!result.found){
  error="missing source objective event constant";return false;
 }
 value=result.value;error.clear();return true;
}
bool raise_async(void* raw,std::uintptr_t level,k::Event& source,std::string& error){
 auto& c=*static_cast<Context*>(raw);
 if(level!=c.owner_token||!c.owner){error="wrong current-Level event owner";return false;}
 n::Owner::LevelEvent event{};event.objective_type=source.objective_type;
 event.character=source.character_word_25;event.network_id=source.source_word_24;
 event.subject_id=source.source_subject;event.flag0=source.flag0;event.flag1=source.flag1;
 if(!c.owner->raise_current_level_event(event,error))return false;
 source.source_subject=event.subject_id;source.flag0=event.flag0;source.flag1=event.flag1;
 c.delivered.push_back(source);error.clear();return true;
}
bool start_script(void*,std::int32_t,std::string& error){error.clear();return true;}

int main(int argc,char** argv){try{
 check(argc==2,"usage: host.exe <asset-data-dir>");const std::string cache=argv[1];
 const auto packed=read(cache+"/v2quests_pyarray.bin");
 const auto names=read(cache+"/v2quests_pyarraynames.bin");
 const auto constants_bytes=read(cache+"/v2quests_pycst.bin");
 t::Input input;check(dh2_quests_open(&input.table,packed->data(),std::uint32_t(packed->size()))==0,"open Quest table");
 input.packed_owner=packed;input.names=names->data();input.names_size=names->size();input.names_owner=names;
 t::Owner table;std::string error;check(table.load(input,error),"load Quest table");const auto definitions=table.borrow();
 n::Constants constants;check(dh2_pycst_open(&constants.view,constants_bytes->data(),std::uint32_t(constants_bytes->size()))==0,"open constants");constants.owner=constants_bytes;

 dh2_quest_objective selected{},clear_source{};bool found_pair=false;
 std::uint32_t quest_ordinal=0,kill_ordinal=0,clear_ordinal=0;
 dh2_quest_span clear_span{};t::Span clear_bytes{};
 // Find two source KillX records on one real Quest. The projection changes
 // only the second record's selector/filter words, allowing one selected Owner
 // to exercise distinct KillX and ClearEnemies event constants in one tail run.
 for(std::uint32_t q=0;q<definitions.count()&&!found_pair;++q){
  const auto* list=definitions.list(*definitions.row(q),1);
  if(!list||!list->definition||list->definition->count<2)continue;
  for(std::uint32_t first=0;first<list->definition->count&&!found_pair;++first){
   dh2_quest_objective a{};check(definitions.objective(*list,first,&a,error),"read first source objective");
   if(a.common[0]!=0||a.args[0]<0||a.args[0]>=INT16_MAX||(a.args[1]!=-1&&a.args[1]<0))continue;
   for(std::uint32_t second=0;second<list->definition->count;++second){
    if(second==first)continue;
    dh2_quest_objective b{};check(definitions.objective(*list,second,&b,error),"read second source objective");
    if(b.common[0]!=0)continue;
    dh2_quest_span span{};t::Span bytes{};
    check(definitions.list_record(*list,second,&span,error)&&definitions.bytes(span,&bytes,error),"resolve second objective's source bytes");
    if(span.size<20)continue;
    selected=a;clear_source=b;quest_ordinal=q;kill_ordinal=first;clear_ordinal=second;
    clear_span=span;clear_bytes=bytes;found_pair=true;break;
   }
  }
 }
 check(found_pair,"cache has no Quest with two source KillX objectives for in-memory Clear projection");
 check(clear_source.args[0]>=0&&clear_source.args[0]<INT16_MAX,"Clear projection source filter out of range");

 // Objective bytes are 3 common words, then two length-prefixed strings,
 // then args[0..2]. Keep both strings and all unrelated fields byte-identical.
 auto projected_bytes=std::make_shared<std::vector<std::uint8_t>>(
  input.table.bytes,input.table.bytes+input.table.size);
 std::size_t arg_offset=clear_span.offset+12;
 for(unsigned text=0;text<2;++text){
  check(arg_offset+4<=clear_span.offset+clear_span.size,"objective string length exceeds its record");
  std::uint32_t length=0;std::memcpy(&length,projected_bytes->data()+arg_offset,4);arg_offset+=4;
  check(length<=clear_span.offset+clear_span.size-arg_offset,"objective string exceeds its record");arg_offset+=length;
 }
 check(arg_offset+12<=clear_span.offset+clear_span.size,"objective arguments exceed their record");
 const std::uint32_t clear_selector=1;
 std::memcpy(projected_bytes->data()+clear_span.offset,&clear_selector,4);
 std::memcpy(projected_bytes->data()+arg_offset,&selected.args[0],4);
 std::memcpy(projected_bytes->data()+arg_offset+4,&selected.args[1],4);
 t::Input projected=input;
 check(dh2_quests_open(&projected.table,projected_bytes->data(),std::uint32_t(projected_bytes->size()))==0,
       "open in-memory ClearEnemies projection");projected.packed_owner=projected_bytes;
 t::Owner projected_table;check(projected_table.load(projected,error),"load projected Quest table");
 const auto projected_definitions=projected_table.borrow();
 dh2_quest_objective projected_clear{};
 check(projected_definitions.objective(*projected_definitions.list(*projected_definitions.row(quest_ordinal),1),
       clear_ordinal,&projected_clear,error)&&projected_clear.common[0]==1&&
       projected_clear.args[0]==selected.args[0]&&projected_clear.args[1]==selected.args[1],
       "ClearEnemies projection did not retain source data with the intended selector/filter");

 auto save=std::make_shared<d::PlayerSavegameV1>();save->set_character(UINT64_C(0x12345678000000a1));
 n::Owner owner(save,projected_definitions,constants);
 check(owner.initialize(0,error)&&owner.initialize(1,error),"initialize canonical Quest owner");
 const auto* quest=save->source_quest_log_b8().quests[0][quest_ordinal];
 check(quest&&owner.resolve(quest),"projected objectives lack a canonical Quest instance");
 const auto current_level_id=selected.args[1]<0?7:selected.args[1];
 check(owner.compile_kill_x_enemies_objective(quest->identity,kill_ordinal,current_level_id,2,nullptr,start_script,error),
       "compile actual KillXEnemies objective");
 check(owner.register_kill_x_enemies_objective(quest->identity,kill_ordinal,nullptr,start_script,error),
       "register actual KillXEnemies objective");
 check(owner.compile_clear_enemies_objective(quest->identity,clear_ordinal,current_level_id,2,nullptr,start_script,error),
       "compile projected ClearEnemies objective");
 check(owner.register_clear_enemies_objective(quest->identity,clear_ordinal,nullptr,start_script,error),
       "register projected ClearEnemies objective");

 dh2_pycst_result kill_constant{},clear_constant{};
 constexpr char group[]="v2QuestObjectiveType",kill_key[]="KillXEnemies",clear_key[]="ClearEnemies";
 check(dh2_pycst_get(&constants.view,group,sizeof(group)-1,kill_key,sizeof(kill_key)-1,&kill_constant)==0&&kill_constant.found,"KillXEnemies constant missing");
 check(dh2_pycst_get(&constants.view,group,sizeof(group)-1,clear_key,sizeof(clear_key)-1,&clear_constant)==0&&clear_constant.found,"ClearEnemies constant missing");

 Context context{&owner,&constants.view,UINT64_C(0x2000000073),73,
  reinterpret_cast<std::uintptr_t>(&owner),static_cast<std::int16_t>(selected.args[0]),-1,{}};
 const k::Services services{&context,current_level,character_word25,character_halfword,get_constant,raise_async};
 // Bounded to an offline direct actor; live ObjectBase virtual+84 / byte+5348
 // gates are still not wired in the production Character::Kill call site.
 const k::Bindings bindings{context.character,UINT64_C(0x1000000042),0,0};
 k::Runtime tail(bindings,services);k::Result result{};
 check(tail.run(&result,error)==k::Status::complete,"Character::Kill tail failed");
 check(result.events_attempted==2&&result.events_raised==2&&context.delivered.size()==2,
       "direct Character should raise both registered objective events");
 check(context.delivered[0].objective_type==kill_constant.value&&context.delivered[0].killer==bindings.killer&&
       context.delivered[0].character_word_25==73&&context.delivered[0].source_word_24==selected.args[0]&&
       context.delivered[0].source_subject==1&&context.delivered[0].flag0==1&&context.delivered[0].flag1==0,
       "KillX event did not update its actual registered objective");
 check(context.delivered[1].objective_type==clear_constant.value&&context.delivered[1].killer==bindings.killer&&
       context.delivered[1].character_word_25==73&&context.delivered[1].source_word_24==selected.args[0]&&
       context.delivered[1].source_subject==1&&context.delivered[1].flag0==1&&context.delivered[1].flag1==0,
       "ClearEnemies event did not update its registered projected objective");
 check(tail.run(&result,error)==k::Status::consumed,"Character::Kill tail replayed");
 check(owner.close(error),"close canonical Quest owner");
 std::printf("PASS: one Character::Kill tail run updated actual KillX and in-memory projected ClearEnemies objectives via the selected NativeQuestOwner; cache unchanged; host-only\n");
 return 0;
 }catch(const std::exception& ex){std::fprintf(stderr,"FAIL: %s\n",ex.what());return 1;}}
