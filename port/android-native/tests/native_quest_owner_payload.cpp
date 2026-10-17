// Host composition of genuine NativeOwner startup and selected QEST readers.
// The packet is synthetic; all row/list/stub shapes and factories are actual.
#include "../app/src/main/cpp/native_quest_owner.hpp"
#include "../app/src/main/cpp/native_quest_cursor.hpp"
#include "../app/src/main/cpp/native_player_profile.hpp"
#include "../../level-world/character_gameplay_save_v1.hpp"
#include "../../game-data/quest_objective_factory_v1.hpp"
#include "../../game-data/player_profile_filename_v1.hpp"
#include "../../game-data/skill_tables.hpp"
#include "../../game-data/level_tables.hpp"
#include "../../game-data/world_map_tables.hpp"
#include "../../game-data/properties.hpp"
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <vector>
namespace d=dh2::data;
namespace n=dh2::native::quests;
namespace t=d::quest_table_bindings_v1;
namespace of=d::quest_objective_factory_v1;
namespace {
using Bytes=std::vector<std::uint8_t>;
unsigned checks=0;
unsigned prop_saved_count=0;
std::uint64_t gameplay_character_identity=0,gameplay_save_identity=0;
std::int32_t gameplay_mask2=-1,gameplay_mask4=-1;
bool gameplay_same_owner=false;
const char* context="fixture setup";
void check(bool value){if(!value)throw std::runtime_error("Native Quest owner payload check "+std::to_string(checks+1)+" at "+context);++checks;}
void word(Bytes& bytes,std::uint32_t value){for(unsigned i=0;i<4;++i)bytes.push_back(std::uint8_t(value>>(8*i)));}
std::int32_t signed_word(std::uint32_t value){std::int32_t result;std::memcpy(&result,&value,4);return result;}
auto file(const std::string& path){
 std::ifstream f(path,std::ios::binary|std::ios::ate);check(bool(f));
 auto bytes=std::make_shared<Bytes>(std::size_t(f.tellg()));f.seekg(0);
 f.read(reinterpret_cast<char*>(bytes->data()),std::streamsize(bytes->size()));check(bool(f));return bytes;
}
struct Cache {
 t::View view;n::Constants constants;d::SkillTables skills;d::LevelTables levels;d::WorldMapTables world_map;
 d::CharacterTable characters;d::PropertyRules property_rules;
 explicit Cache(const std::string& path){
  auto packed=file(path+"/v2quests_pyarray.bin"),names=file(path+"/v2quests_pyarraynames.bin");
  t::Input input;check(!dh2_quests_open(&input.table,packed->data(),std::uint32_t(packed->size())));
  input.packed_owner=packed;input.names=names->data();input.names_size=names->size();input.names_owner=names;
  t::Owner owner;std::string error;check(owner.load(input,error));view=owner.borrow();check(view.count()==64);
  auto bytes=file(path+"/v2quests_pycst.bin");check(!dh2_pycst_open(&constants.view,bytes->data(),std::uint32_t(bytes->size())));constants.owner=bytes;
  auto skill_data=file(path+"/skills_pyarray.bin"),skill_names=file(path+"/skills_pyarraynames.bin"),skill_schema=file(path+"/skills_pystructnames.bin");
  check(d::load_skill_tables({skill_data->data(),skill_data->size()},{skill_names->data(),skill_names->size()},{skill_schema->data(),skill_schema->size()},skills,error));
  auto level_data=file(path+"/levels_pyarray.bin"),level_names=file(path+"/levels_pyarraynames.bin"),level_schema=file(path+"/levels_pystructnames.bin");
  check(d::load_levels({level_data->data(),level_data->size()},{level_names->data(),level_names->size()},{level_schema->data(),level_schema->size()},levels,error));
  auto map_data=file(path+"/worldmap_pyarray.bin"),map_names=file(path+"/worldmap_pyarraynames.bin"),map_schema=file(path+"/worldmap_pystructnames.bin");
  check(d::load_world_map({map_data->data(),map_data->size()},{map_names->data(),map_names->size()},{map_schema->data(),map_schema->size()},world_map,error));
  auto character_data=file(path+"/character_properties_pyarray.bin"),character_names=file(path+"/character_properties_pyarraynames.bin"),character_schema=file(path+"/character_properties_pystructnames.bin");
  check(d::load_characters({character_data->data(),character_data->size()},{character_names->data(),character_names->size()},{character_schema->data(),character_schema->size()},characters,error));
  check(d::load_property_rules(characters,property_rules,error));
 }
};
std::uint64_t position(n::Cursor& cursor){std::uint64_t value=0;check(cursor.tell(cursor.stream(),&value));return value;}
struct QuestSaveSink {Bytes bytes;};
bool append_quest_save(void* raw,d::Bytes bytes,std::string& error){
 auto* sink=static_cast<QuestSaveSink*>(raw);
 if(!sink||(!bytes.data&&bytes.size)){error="invalid test QEST sink";return false;}
 sink->bytes.insert(sink->bytes.end(),bytes.data,bytes.data+bytes.size);return true;
}
Bytes save_quests(n::Owner& owner){
 QuestSaveSink sink;const d::player_save_section_writers_v1::WriteServicesV1 stream{&sink,append_quest_save};
 std::string error;check(owner.save_quests(stream,error)&&error.empty());return sink.bytes;
}
std::unique_ptr<n::Cursor> campaign(const Bytes& payload){
 Bytes bytes;word(bytes,2);word(bytes,11);
 for(char c:std::string("PNAM"))bytes.push_back(std::uint8_t(c));
 for(unsigned i=0;i<11;++i)bytes.push_back(std::uint8_t(0x90+i));
 word(bytes,std::uint32_t(payload.size()));for(char c:std::string("QEST"))bytes.push_back(std::uint8_t(c));
 bytes.insert(bytes.end(),payload.begin(),payload.end());
 d::PlayerProfileIndexV1 index;std::string error;check(index.load({bytes.data(),bytes.size()},error));
 const auto borrow=index.borrow();check(borrow.section("QEST")&&borrow.section("QEST")->offset==31);
 auto cursor=std::make_unique<n::Cursor>(borrow,"QEST");check(position(*cursor)==31);return cursor;
 // The local index/input are destroyed. Cursor keeps the same retained whole
 // campaign generation and uses absolute31, including the earlier section.
}
d::quest_savegame_v1::QuestSavegame& log(d::PlayerSavegameV1& save,unsigned index){return index?save.source_quest_log_118():save.source_quest_log_b8();}
std::uint8_t done(unsigned row,unsigned difficulty,unsigned slot){return std::uint8_t(0x80|((row*3+difficulty+slot)&0x7f));}
std::uint32_t amount(unsigned row,unsigned difficulty,unsigned slot){return 0xf1234000u|(difficulty<<12)|(row<<4)|slot;}
std::int32_t state(unsigned row,unsigned difficulty){return std::int32_t(2+(row+difficulty*3)%12);}
bool volatile_state(std::int32_t value){switch(value){case 2:case 4:case 5:case 7:case 8:case 10:case 11:return true;default:return false;}}
bool has_quantity(std::int32_t type){check(type>=0&&type<13);return type!=4&&type!=6&&type!=12;}
void append_objective(Bytes& bytes,std::int32_t type,unsigned row,unsigned difficulty,unsigned slot){
 bytes.push_back(done(row,difficulty,slot));if(has_quantity(type))word(bytes,amount(row,difficulty,slot));
}
Bytes packet(const Cache& cache){
 Bytes bytes;
 for(unsigned difficulty=0;difficulty<3;++difficulty){const auto start=bytes.size();unsigned object_count=0,object_bytes=0;
  word(bytes,64);
  // Streamed indices deliberately differ from ordinal: source chooses the live
  // indexed Quest, then consumes that row's real action/objective shapes.
  for(unsigned ordinal=0;ordinal<64;++ordinal){const auto row=63-ordinal;const auto* py=cache.view.row(row);const auto* definition=cache.view.record(*py);
   word(bytes,row);word(bytes,std::uint32_t(state(row,difficulty)));
   const auto before=bytes.size();append_objective(bytes,definition->accept.common[0],row,difficulty,0);append_objective(bytes,definition->end.common[0],row,difficulty,1);object_count+=2;
   const auto* list=cache.view.list(*py,1);check(list&&list->definition->count==definition->lists[1].count);
   for(unsigned slot=0;slot<list->definition->count;++slot){dh2_quest_objective objective{};std::string error;check(cache.view.objective(*list,slot,&objective,error));append_objective(bytes,objective.common[0],row,difficulty,2+slot);++object_count;}
   object_bytes+=unsigned(bytes.size()-before);
  }
  word(bytes,0x81220000u+difficulty);word(bytes,0x92330000u+difficulty);word(bytes,0xa3440000u+difficulty);
  check(object_count==194&&object_bytes==414&&bytes.size()-start==942);
 }
 check(bytes.size()==2826);return bytes;
}
of::Record& action(d::quest_runtime_fields_v1::ActionRef* ref){
 check(ref&&ref->identity>UINT32_MAX);
 // NativeOwner's allocator deliberately publishes the actual Record address
 // as identity. This is a verified host-owner borrow, never a packed ARM cast.
 auto* record=reinterpret_cast<of::Record*>(ref->identity);
 check(&record->ref.action==ref&&record->ref.fields==&record->fields&&ref->character_10==&record->fields.character_10);
 return *record;
}
void verify_action(d::quest_runtime_fields_v1::ActionRef* ref,const Cache& cache,unsigned row,unsigned difficulty,unsigned slot){
 auto& record=action(ref);const auto* actual=cache.view.record(*cache.view.row(row));
 const auto type=slot?actual->end.common[0]:actual->accept.common[0];
 check(record.fields.type_4==type&&record.fields.py_data_c.stub==cache.view.resolve_stub(cache.view.row(row)->identity+(slot?0x68:0x3c)));
 check(record.done_14==done(row,difficulty,slot));
 if(has_quantity(type))check(record.quantity_20==amount(row,difficulty,slot));
}
void initialize(n::Owner& owner,d::PlayerSavegameV1& save){std::string error;check(owner.initialize(0,error)&&owner.initialize(1,error));check(owner.receipt().published[0]==192&&owner.receipt().published[1]==192&&owner.receipt().constant_queries==1608);for(unsigned index=0;index<2;++index)for(const auto& vector:log(save,index).quests)check(vector.size()==64);}
void close(n::Owner& owner,d::PlayerSavegameV1& save){std::string error;check(owner.close(error)&&owner.receipt().destroyed[0]==192&&owner.receipt().destroyed[1]==192&&owner.receipt().unpublished_destroyed==0);for(unsigned index=0;index<2;++index)for(const auto& vector:log(save,index).quests)check(vector.empty());check(owner.close(error));}
void complete(Cache& cache){
 auto save=std::make_shared<d::PlayerSavegameV1>();save->set_character(UINT64_C(0x12345678000000a1));n::Owner owner(save,cache.view,cache.constants);initialize(owner,*save);
 auto* canonical0=&save->source_quest_log_b8();auto* canonical1=&save->source_quest_log_118();
 check(&canonical0->word_44==&save->quest_log_b8_act_words_v1()&&&canonical1->word_44==&save->quest_log_118_act_words_v1());
 for(unsigned index=0;index<2;++index)for(unsigned difficulty=0;difficulty<3;++difficulty)for(unsigned row=0;row<64;++row){auto* record=owner.resolve(log(*save,index).quests[difficulty][row]);check(record&&record->fields.row_8==std::int32_t(row)&&record->difficulty_10==std::int32_t(difficulty));record->byte_64=9;}
 const auto bytes=packet(cache);auto cursor=campaign(bytes);const auto identity=cursor->stream().identity;std::string error;
 check(owner.load_quests(*cursor,error)&&error.empty());
 check(owner.receipt().quest_payloads==384&&owner.receipt().objective_payloads==1164);
 // Both logs read one packet. Without rewinding the same retained stream,
 // the second log would encounter physical EOF at absolute31+2826.
 check(position(*cursor)==31+bytes.size()&&cursor->stream().identity==identity);
 save->set_source_save_mode(1);const auto saved_b8=save_quests(owner);
 save->set_source_save_mode(2);const auto saved_118=save_quests(owner);
 check(saved_b8==saved_118);
 auto* selected=owner.resolve(log(*save,0).quests[0][0]);check(selected);
 ++selected->state_0;
 save->set_source_save_mode(1);check(save_quests(owner)!=saved_b8);
 save->set_source_save_mode(2);check(save_quests(owner)==saved_118);
 --selected->state_0;
 auto roundtrip_save=std::make_shared<d::PlayerSavegameV1>();
 n::Owner roundtrip_owner(roundtrip_save,cache.view,cache.constants);
 initialize(roundtrip_owner,*roundtrip_save);auto roundtrip_cursor=campaign(saved_b8);
 check(roundtrip_owner.load_quests(*roundtrip_cursor,error)&&error.empty());
 roundtrip_save->set_source_save_mode(1);check(save_quests(roundtrip_owner)==saved_b8);
 close(roundtrip_owner,*roundtrip_save);
 check(&save->source_quest_log_b8()==canonical0&&&save->source_quest_log_118()==canonical1);
 for(unsigned index=0;index<2;++index)for(unsigned difficulty=0;difficulty<3;++difficulty){auto& store=log(*save,index);
  check(store.word_2c[difficulty]==signed_word(0x81220000u+difficulty)&&store.word_38[difficulty]==signed_word(0x92330000u+difficulty));
  check(store.word_44[difficulty]==signed_word(0xa3440000u+difficulty)&&store.word_50[difficulty]==store.word_44[difficulty]);
  for(unsigned row=0;row<64;++row){auto* record=owner.resolve(store.quests[difficulty][row]);check(record&&record->state_0==state(row,difficulty));check(record->byte_64==(volatile_state(state(row,difficulty))?1:9));check(record->fields.character_60==save->character()&&record->py_data_68==cache.view.row(row));verify_action(record->action_18,cache,row,difficulty,0);verify_action(record->action_1c,cache,row,difficulty,1);}
 }
 close(owner,*save);check(position(*cursor)==31+bytes.size());check(!owner.load_quests(*cursor,error)&&position(*cursor)==31+bytes.size());
}
unsigned truncated_quantity(Cache& cache){
 unsigned row=64;for(unsigned index=0;index<64;++index)if(cache.view.record(*cache.view.row(index))->accept.common[0]==5){row=index;break;}check(row<64);
 auto save=std::make_shared<d::PlayerSavegameV1>();n::Owner owner(save,cache.view,cache.constants);initialize(owner,*save);
 auto* record=owner.resolve(log(*save,0).quests[0][row]);auto* other=owner.resolve(log(*save,1).quests[0][row]);check(record&&other);
 auto& accept=action(record->action_18);auto& end=action(record->action_1c);check(accept.fields.type_4==5&&has_quantity(accept.fields.type_4));
 accept.done_14=0x7f;accept.quantity_20=0xcccccccc;end.done_14=0x55;const auto old_end_quantity=end.quantity_20;
 record->byte_64=9;const auto other_state=other->state_0;const auto old_tail0=log(*save,0).word_44,old_tail1=log(*save,1).word_44;
 Bytes bytes;word(bytes,64);word(bytes,row);word(bytes,5);bytes.push_back(done(row,0,0));bytes.push_back(0x78);bytes.push_back(0x56);
 auto cursor=campaign(bytes);const auto identity=cursor->stream().identity;std::string error;
 check(!owner.load_quests(*cursor,error)&&error=="Native QEST Quest payload provider failed");
 check(position(*cursor)==31+bytes.size()&&cursor->stream().identity==identity);
 check(record->state_0==5&&record->byte_64==9);check(accept.done_14==done(row,0,0)&&accept.quantity_20==0xcccccccc);
 check(end.done_14==0x55&&end.quantity_20==old_end_quantity&&other->state_0==other_state);
 check(owner.receipt().quest_payloads==0&&owner.receipt().objective_payloads==0);
 check(log(*save,0).word_44==old_tail0&&log(*save,1).word_44==old_tail1);
 for(unsigned index=0;index<2;++index)for(unsigned difficulty=0;difficulty<3;++difficulty){check(log(*save,index).word_2c[difficulty]==-1&&log(*save,index).word_38[difficulty]==-1&&log(*save,index).word_50[difficulty]==1);}
 // The synchronous external cursor borrow must be released even on failure;
 // actual source destructor closure needs only the retained factory owners.
 cursor.reset();close(owner,*save);return row;
}
void truncated_direct_state(Cache& cache){
 auto save=std::make_shared<d::PlayerSavegameV1>();n::Owner owner(save,cache.view,cache.constants);initialize(owner,*save);
 auto* record=owner.resolve(log(*save,0).quests[0][0]);check(record);record->state_0=signed_word(0xa5a5a5a5);record->byte_64=9;
 auto& accept=action(record->action_18);accept.done_14=0x7f;accept.quantity_20=0xcccccccc;
 Bytes bytes;word(bytes,64);word(bytes,0);bytes.push_back(0x78);bytes.push_back(0x56);auto cursor=campaign(bytes);std::string error;
 check(!owner.load_quests(*cursor,error));check(std::uint32_t(record->state_0)==0xa5a55678&&record->byte_64==9);
 check(accept.done_14==0x7f&&accept.quantity_20==0xcccccccc&&position(*cursor)==31+bytes.size());
 check(owner.receipt().quest_payloads==0&&owner.receipt().objective_payloads==0);close(owner,*save);
}
void transport_bridge(Cache& cache){
 context="transport bridge Save/Owner";
 auto save=std::make_shared<d::PlayerSavegameV1>();save->set_character(UINT64_C(0x12345678000000a1));save->set_slot(0);
 auto owner=std::make_shared<n::Owner>(save,cache.view,cache.constants);
 context="transport bridge synthetic profile";
 const auto skill_id=cache.skills.skill_lists.at(0).members.at(0);
 check(skill_id>=0&&std::size_t(skill_id)<cache.skills.skills.size());
 const auto& skill_name=cache.skills.skills[std::size_t(skill_id)].table_name;
 Bytes skil;word(skil,1);word(skil,std::uint32_t(skill_name.size()+1));
 skil.insert(skil.end(),skill_name.begin(),skill_name.end());skil.push_back(0);
 skil.push_back(7);skil.push_back(0);
 word(skil,1);word(skil,0);word(skil,0);
 word(skil,1);word(skil,3);word(skil,0);
 Bytes faes;
 for(unsigned difficulty=0;difficulty<3;++difficulty){
  word(faes,0xfffffff0u+difficulty);word(faes,5);
  for(unsigned row=0;row<5;++row){
   const auto level=std::uint16_t(difficulty*16+row);
   faes.push_back(std::uint8_t(level));faes.push_back(std::uint8_t(level>>8));
   faes.push_back(std::uint8_t(0x40+difficulty*5+row));
  }
 }
 Bytes qest=packet(cache),prop;word(prop,224);for(unsigned i=0;i<224;++i)word(prop,0x40000000u+i);prop.push_back(41);
 Bytes profile_bytes;word(profile_bytes,5);
 const auto append_section=[&](const char* tag,const Bytes& payload){
  word(profile_bytes,std::uint32_t(payload.size()));
  for(unsigned i=0;i<4;++i)profile_bytes.push_back(std::uint8_t(tag[i]));
  profile_bytes.insert(profile_bytes.end(),payload.begin(),payload.end());
 };
 Bytes name(11);for(unsigned i=0;i<name.size();++i)name[i]=std::uint8_t(0x90+i);
 append_section("PNAM",name);append_section("SKIL",skil);append_section("FAES",faes);append_section("QEST",qest);append_section("PROP",prop);
 auto directory=std::filesystem::temp_directory_path()/(
  "dh2-native-quest-transport-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
 std::filesystem::create_directories(directory);
 {std::ofstream out(directory/d::player_profile_filename_v1(0,false,false),std::ios::binary);
  check(bool(out));out.write(reinterpret_cast<const char*>(profile_bytes.data()),std::streamsize(profile_bytes.size()));check(bool(out));}
 d::PlayerSaveProfileV1 profile;dh2::native::player_profile::Transport transport(*save,profile);std::string error;
 std::uint8_t native_host_online=0;std::int32_t skill_tree_selector=0;
 d::PropertyState property_state;property_state.saved.fill(-7);property_state.resolved.fill(-9);
 dh2::native::player_profile::TransportBindings bindings;bindings.directory=directory;bindings.quests=owner;bindings.online=&native_host_online;
 bindings.levels=&cache.levels;bindings.world_map=&cache.world_map;bindings.skill_tables=&cache.skills;bindings.skill_tree_selector=&skill_tree_selector;
 bindings.property_rules=&cache.property_rules;bindings.properties=&property_state;
 context="transport bridge binding";
 if(!transport.bind(std::move(bindings),error))throw std::runtime_error("transport bridge bind: "+error);
 namespace gameplay=dh2::character_gameplay_save_v1;
 auto gameplay_save=gameplay::borrow_save(reinterpret_cast<std::uintptr_t>(save.get()),*save,&transport.loader());
 check(gameplay_save.identity==reinterpret_cast<std::uintptr_t>(save.get())&&gameplay_save.save==save.get()&&gameplay_save.loader==&transport.loader());
 check(gameplay_save.quest_character_174==&save->source_quest_log_118().character_5c&&gameplay_save.quest_character_114==&save->source_quest_log_b8().character_5c);
 gameplay::SaveRef* current_save=&gameplay_save;gameplay::Character character{save->character(),&current_save};
 gameplay::Runtime character_runtime(character);gameplay::Result mask2_result;
 if(character_runtime.load(2,&mask2_result,error)!=gameplay::Status::complete)throw std::runtime_error("Character SG_Load mask-2: "+error);
 check(mask2_result.stage==gameplay::Stage::complete&&mask2_result.captured_character==character.identity);
 check(mask2_result.captured_save==gameplay_save.identity&&mask2_result.mask==2&&mask2_result.load_calls==1);
 check(gameplay_save.loader==&transport.loader()&&&gameplay_save.loader->save()==gameplay_save.save&&owner->owns_save(gameplay_save.save));
 context="transport mask-2 initialization";
 check(transport.loader().delivered_calls()==7&&save->skills_initialized());
 check(save->skills().size()==cache.skills.skill_lists.at(0).members.size());
 check(save->skill_id(0)==skill_id&&save->skill_level(0)==0);
 check(std::all_of(save->faeries_initialized().begin(),save->faeries_initialized().end(),[](bool value){return value;}));
 gameplay::Result mask4_result;
 if(character_runtime.load(4,&mask4_result,error)!=gameplay::Status::complete)throw std::runtime_error("Character SG_Load mask-4: "+error);
 check(mask4_result.stage==gameplay::Stage::complete&&mask4_result.captured_character==mask2_result.captured_character);
 check(mask4_result.captured_save==mask2_result.captured_save&&mask4_result.mask==4&&mask4_result.load_calls==1);
 gameplay_character_identity=mask2_result.captured_character;gameplay_save_identity=mask2_result.captured_save;
 gameplay_mask2=mask2_result.mask;gameplay_mask4=mask4_result.mask;
 gameplay_same_owner=gameplay_save.save==save.get()&&gameplay_save.loader==&transport.loader()&&
  &gameplay_save.loader->save()==save.get()&&owner->owns_save(save.get())&&
  gameplay_save.quest_character_174==&save->source_quest_log_118().character_5c&&
  gameplay_save.quest_character_114==&save->source_quest_log_b8().character_5c;
 check(gameplay_same_owner);
 context="transport bridge masks / QEST";
 check(transport.loader().delivered_calls()==10);
 check(profile.identity&&profile.campaign.section("QEST"));
 check(profile.campaign.section("SKIL")&&save->skill_id(0)==skill_id&&save->skill_level(0)==7);
 check(save->skill_slots()[0].size()==1&&save->skill_slots()[0].at(0)==0);
 check(save->skill_slots()[1].size()==1&&save->skill_slots()[1].at(3)==0);
 check(profile.campaign.section("FAES")&&profile.campaign.payload("FAES").size==69);
 check(profile.campaign.section("QEST")&&profile.campaign.section("PROP"));
 check(profile.campaign.section("QEST")->offset<profile.campaign.section("PROP")->offset);
 for(unsigned difficulty=0;difficulty<3;++difficulty){
  check(save->current_faery(difficulty)==signed_word(0xfffffff0u+difficulty));
  for(unsigned row=0;row<5;++row){
   check(save->faeries()[difficulty][row].level==difficulty*16+row);
   check(save->faeries()[difficulty][row].state==0x40+difficulty*5+row);
  }
 }
 check(owner->owns_save(save.get())&&owner->receipt().published[0]==192&&owner->receipt().published[1]==192);
 check(owner->receipt().quest_payloads==384&&owner->receipt().objective_payloads==1164);
 check(save->saved_properties_byte_194()==41);
 unsigned stored_properties=0;
 for(unsigned i=0;i<224;++i){const auto type=cache.property_rules.types[i]==-1?16u:std::uint32_t(cache.property_rules.types[i]);
  const auto expected=(type&0x20)?std::int32_t(0x40000000u+i):-7;stored_properties+=(type&0x20)!=0;
  check(property_state.saved[i]==expected&&property_state.resolved[i]==-9);
 }
 check(stored_properties>0);
 prop_saved_count=stored_properties;
 for(unsigned index=0;index<2;++index)for(unsigned difficulty=0;difficulty<3;++difficulty){
  auto& store=log(*save,index);check(store.quests[difficulty].size()==64);
  check(store.word_2c[difficulty]==signed_word(0x81220000u+difficulty));
  for(unsigned row=0;row<64;++row){auto* record=owner->resolve(store.quests[difficulty][row]);check(record&&record->state_0==state(row,difficulty));}
 }
 context="transport bridge close";
 check(owner->close(error));std::filesystem::remove_all(directory);
}
void transport_bridge_faes_count_mismatch(Cache& cache){
 context="transport FAES mismatch Save/Owner";
 auto save=std::make_shared<d::PlayerSavegameV1>();save->set_character(UINT64_C(0x12345678000000a2));save->set_slot(0);
 auto owner=std::make_shared<n::Owner>(save,cache.view,cache.constants);
 const auto skill_id=cache.skills.skill_lists.at(0).members.at(0);
 const auto& skill_name=cache.skills.skills.at(std::size_t(skill_id)).table_name;
 Bytes skil;word(skil,1);word(skil,std::uint32_t(skill_name.size()+1));
 skil.insert(skil.end(),skill_name.begin(),skill_name.end());skil.push_back(0);
 skil.push_back(7);skil.push_back(0);word(skil,1);word(skil,0);word(skil,0);word(skil,1);word(skil,3);word(skil,0);
 Bytes faes;
 const auto faery_block=[&](unsigned difficulty,std::uint32_t current){
  word(faes,current);word(faes,5);
  for(unsigned row=0;row<5;++row){const auto level=std::uint16_t(difficulty*16+row);
   faes.push_back(std::uint8_t(level));faes.push_back(std::uint8_t(level>>8));faes.push_back(std::uint8_t(0x40+difficulty*5+row));}
 };
 faery_block(0,0x11223340u);word(faes,0x11223341u);word(faes,4);faery_block(2,0x11223342u);
 Bytes qest=packet(cache),profile_bytes;word(profile_bytes,4);
 const auto append_section=[&](const char* tag,const Bytes& payload){
  word(profile_bytes,std::uint32_t(payload.size()));for(unsigned i=0;i<4;++i)profile_bytes.push_back(std::uint8_t(tag[i]));
  profile_bytes.insert(profile_bytes.end(),payload.begin(),payload.end());
 };
 Bytes name(11);append_section("PNAM",name);append_section("SKIL",skil);append_section("FAES",faes);append_section("QEST",qest);
 auto directory=std::filesystem::temp_directory_path()/(
  "dh2-native-faes-mismatch-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
 std::filesystem::create_directories(directory);
 {std::ofstream out(directory/d::player_profile_filename_v1(0,false,false),std::ios::binary);
  check(bool(out));out.write(reinterpret_cast<const char*>(profile_bytes.data()),std::streamsize(profile_bytes.size()));check(bool(out));}
 d::PlayerSaveProfileV1 profile;dh2::native::player_profile::Transport transport(*save,profile);std::string error;
 std::uint8_t online=0;std::int32_t selector=0;dh2::native::player_profile::TransportBindings bindings;
 bindings.directory=directory;bindings.quests=owner;bindings.online=&online;bindings.levels=&cache.levels;bindings.world_map=&cache.world_map;
 bindings.skill_tables=&cache.skills;bindings.skill_tree_selector=&selector;
 context="transport FAES mismatch binding";if(!transport.bind(std::move(bindings),error))throw std::runtime_error("FAES mismatch bind: "+error);
 if(!transport.loader().load(2,error))throw std::runtime_error("FAES mismatch mask-2 load: "+error);
 check(transport.loader().delivered_calls()==7&&save->faeries_initialized()[1]&&save->faeries_initialized()[2]);
 for(unsigned row=0;row<5;++row)for(unsigned difficulty=1;difficulty<3;++difficulty){
  check(save->set_faery_level(row,std::int32_t(100+difficulty*10+row),difficulty,error));
  check(save->set_faery_state(row,std::int32_t(0x70+difficulty*5+row),difficulty,error));
 }
 context="transport FAES mismatch mask-4";
 if(!transport.loader().load(4,error))throw std::runtime_error("FAES mismatch mask-4 load: "+error);
 check(transport.loader().delivered_calls()==10&&profile.identity&&profile.campaign.section("FAES")&&profile.campaign.payload("FAES").size==54);
 check(save->current_faery(0)==signed_word(0x11223340u)&&save->current_faery(1)==signed_word(0x11223341u));
 check(save->current_faery(2)==0); // The source reader returned at difficulty 1's mismatched count.
 for(unsigned row=0;row<5;++row){
  check(save->faeries()[0][row].level==row&&save->faeries()[0][row].state==0x40+row);
  for(unsigned difficulty=1;difficulty<3;++difficulty){
   check(save->faeries()[difficulty][row].level==100+difficulty*10+row);
   check(save->faeries()[difficulty][row].state==0x70+difficulty*5+row);
  }
 }
 check(profile.campaign.section("QEST")&&owner->owns_save(save.get()));
 check(owner->receipt().quest_payloads==384&&owner->receipt().objective_payloads==1164);
 for(unsigned index=0;index<2;++index)for(unsigned difficulty=0;difficulty<3;++difficulty)
  for(unsigned row=0;row<64;++row){auto* record=owner->resolve(log(*save,index).quests[difficulty][row]);check(record&&record->state_0==state(row,difficulty));}
 context="transport FAES mismatch close";check(owner->close(error));std::filesystem::remove_all(directory);
}
}
int main(int argc,char** argv){try{check(argc==2);Cache cache(argv[1]);complete(cache);const auto row=truncated_quantity(cache);truncated_direct_state(cache);
 transport_bridge(cache);transport_bridge_faes_count_mismatch(cache);
 std::printf("{\"validation\":\"PASS\",\"checks\":%u,\"character_gameplay_save_load_masks\":[%d,%d],\"character_identity\":%llu,\"save_identity\":%llu,\"same_transport_save_loader_and_embedded_quest_owners\":%s,\"actual_quest_instances\":384,\"quest_payloads\":384,\"objective_payloads\":1164,\"transport_mask_2_4_bridge\":true,\"nonempty_skil_transport\":true,\"restored_skill_level\":7,\"restored_skill_slot_maps\":2,\"nonempty_faes_transport\":true,\"nonempty_prop_transport\":true,\"prop_after_qest_same_save_and_state\":true,\"prop_saved_property_count\":%u,\"prop_byte_194\":41,\"faes_count_mismatch_nonfatal_before_qest\":true,\"faes_later_difficulty_untouched\":true,\"restored_faery_difficulties\":3,\"objects_per_difficulty\":194,\"object_payload_bytes_per_difficulty\":414,\"qest_bytes_per_difficulty\":942,\"one_replay_packet_bytes\":2826,\"absolute_start\":31,\"same_cursor_replay\":true,\"canonical_state_volatile_tails\":true,\"genuine_close_after_success_and_failure\":true,\"truncated_saved_quantity\":{\"actual_row\":%u,\"quantity_not_published\":true,\"state_and_action_done_preserved\":true,\"cursor_prefix_retained\":true,\"assertion_policy_unavailable_rejected\":true},\"truncated_direct_state_prefix\":true,\"android_compilation\":false,\"live_gameplay\":false}\n",checks,gameplay_mask2,gameplay_mask4,static_cast<unsigned long long>(gameplay_character_identity),static_cast<unsigned long long>(gameplay_save_identity),gameplay_same_owner?"true":"false",prop_saved_count,row);return 0;
 }catch(const std::exception& error){std::fprintf(stderr,"%s\n",error.what());return 1;}}
