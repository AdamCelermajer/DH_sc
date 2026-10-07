#include "quest_persistence_v51.hpp"
#include <cstring>
#include <limits>
namespace dh2::data { namespace {
class Reader {
 Bytes bytes_;std::size_t at_{};std::string& error_;
public:
 Reader(Bytes b,std::string& e):bytes_(b),error_(e){}
 std::size_t used()const noexcept{return at_;}
 std::size_t left()const noexcept{return bytes_.size-at_;}
 bool word(std::uint32_t& v){if(left()<4){error_="Truncated Quest source word at "+std::to_string(at_);return false;}
  const auto* p=bytes_.data+at_;v=std::uint32_t(p[0])|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;at_+=4;return true;}
 bool integer(std::int32_t& v){std::uint32_t w;if(!word(w))return false;std::memcpy(&v,&w,4);return true;}
 bool byte(std::uint8_t& v){if(!left()){error_="Truncated Quest source byte at "+std::to_string(at_);return false;}v=bytes_.data[at_++];return true;}
 bool string(std::string& s){std::uint32_t n;if(!word(n))return false;if(n>left()){error_="Truncated Quest source CString";return false;}
  if(n){s.assign(reinterpret_cast<const char*>(bytes_.data+at_),n);if(s.back()=='\0')s.pop_back();}else s.clear();at_+=n;return true;}
 bool count(std::uint32_t& n,std::size_t minimum){if(!word(n))return false;if(minimum&&n>left()/minimum){error_="Unsafe Quest source array count";return false;}return true;}
};
bool objective(Reader& r,QuestObjectiveDefinitionV51& o){return r.integer(o.type)&&r.integer(o.description)&&r.integer(o.on_complete)&&r.string(o.str1)&&r.string(o.str2)&&r.integer(o.oid1)&&r.integer(o.oid2)&&r.integer(o.value);}
bool has_quantity(std::int32_t type){return type!=4&&type!=6&&type!=12;}
bool valid_type(std::int32_t type){return type>=0&&type<13;}
bool volatile_state(std::int32_t state){constexpr std::uint8_t table[]{1,0,1,1,0,1,1,0,1,1};return state>=2&&state<=11&&table[state-2];}
void put_word(std::vector<std::uint8_t>& bytes,std::int32_t value){std::uint32_t w;std::memcpy(&w,&value,4);for(unsigned i=0;i<4;++i)bytes.push_back(static_cast<std::uint8_t>(w>>(i*8)));}
bool load_objective(Reader& r,QuestObjectivePersistenceV51& o){return r.byte(o.completed14)&&(!has_quantity(o.definition->type)||r.integer(o.quantity20));}
void save_objective(std::vector<std::uint8_t>& b,const QuestObjectivePersistenceV51& o){b.push_back(o.completed14);if(has_quantity(o.definition->type))put_word(b,o.quantity20);}
}
bool QuestTablesPersistenceV51::decode(Bytes array,Bytes names,std::string& e){
 if(ready_){e="Actual retained Quest table cannot replace live row pointers";return false;}
 if((!array.data&&array.size)||(!names.data&&names.size)){e="Required actual Quest PyArray spans";return false;}
 Reader r(array,e),n(names,e);std::uint32_t count{},name_count{};
 // Smallest source v2Quest row has173 bytes with empty arrays/CStrings.
 // Reject impossible dimensions before allocating the native row containers.
 if(!r.count(count,173)||!n.count(name_count,4))return false;
 if(count!=name_count){e="Actual Quest table/name count mismatch";return false;}
 std::vector<QuestDefinitionV51> rows;rows.resize(count);
 for(auto& row:rows){
  if(!n.string(row.name))return false;
  for(auto& v:row.text_fields)if(!r.integer(v))return false;
  std::uint32_t size{};if(!r.count(size,12))return false;row.prerequisites.resize(size);
  for(auto& v:row.prerequisites)if(!r.integer(v.type)||!r.integer(v.parameter1)||!r.integer(v.parameter2))return false;
  if(!r.count(size,32))return false;row.objectives.resize(size);
  for(auto& o:row.objectives)if(!objective(r,o)||!valid_type(o.type)){if(e.empty())e="Required actual Objective factory type";return false;}
  for(auto& list:row.rewards){if(!r.count(size,12))return false;list.resize(size);for(auto& v:list)if(!r.integer(v.type)||!r.integer(v.parameter1)||!r.integer(v.parameter2))return false;}
  if(!objective(r,row.accept)||!objective(r,row.end))return false;
  if(!valid_type(row.accept.type)||!valid_type(row.end.type)){e="Required actual start/end Objective factory type";return false;}
  if(!r.integer(row.target_level)||!r.byte(row.repeatable)||!r.integer(row.state))return false;
  for(auto& script:row.scripts)if(!r.string(script))return false;
  if(!r.integer(row.priority)||!r.integer(row.act))return false;
 }
 if(r.left()||n.left()){e="Unconsumed actual Quest PyArray/name bytes";return false;}
 rows_.swap(rows);ready_=true;e.clear();return true;
}
bool QuestPersistenceOwnerV51::construct(std::shared_ptr<const QuestTablesPersistenceV51> tables,
 std::uintptr_t owner,QuestSavegameV1& collection,std::string& e){
 if(attempted_||collection.initialized()){e="Quest persistence C1/InitQuests cannot replay or replace an initialized collection";return false;}attempted_=true;
 if(!tables||!tables->ready()){e="Required decoded retained actual Quest table";return false;}tables_=std::move(tables);
 std::array<std::vector<std::uintptr_t>,3> ids;
 for(std::size_t d=0;d<3;++d){rows_[d].reserve(tables_->rows().size());ids[d].reserve(tables_->rows().size());
  for(std::size_t i=0;i<tables_->rows().size();++i){auto q=std::make_unique<QuestPersistenceStateV51>();
   const auto& definition=tables_->rows()[i];q->definition=&definition;q->character_owner=owner;q->difficulty=static_cast<std::int32_t>(d);q->index=static_cast<std::int32_t>(i);
   q->accept={&definition.accept,owner,0,0};q->end={&definition.end,owner,0,0};
   q->objectives.reserve(definition.objectives.size());for(const auto& o:definition.objectives)q->objectives.push_back({&o,owner,0,0});
   // Fresh source Quest C1 state=-1; ReInit's old-state switch reaches no
   // unregister/marker branch, then stores actual row+9c.
   q->state=definition.state;ids[d].push_back(reinterpret_cast<std::uintptr_t>(q.get()));rows_[d].push_back(std::move(q));
  }
 }
 if(!collection.attach_initialized_quests(ids,e))return false;ready_=true;return true;
}
bool QuestPersistenceOwnerV51::destroy_source_v108(QuestSavegameV1& collection,const std::function<bool(std::uintptr_t,std::string&)>& destroy,std::string& e){
 auto& arrays=collection.source_quest_arrays_d1_v108();
 for(std::size_t d=0;d<3;++d){if(arrays[d].size()!=rows_[d].size()){e="QuestSavegame D1 array differs from SAME owned Quest allocation";return false;}
  for(std::size_t i=0;i<rows_[d].size();++i){auto& q=rows_[d][i];if(!q){e="QuestSavegame D1 cannot replay a destroyed source prefix";return false;}
   const auto id=reinterpret_cast<std::uintptr_t>(q.get());if(arrays[d][i]!=id||!destroy||!destroy(id,e)){if(e.empty())e="Required actual positive Quest D0 before array free";return false;}q.reset();
  }
  rows_[d].clear();arrays[d].clear();arrays[d].shrink_to_fit();
 }
 ready_=false;e.clear();return true;
}
QuestPersistenceStateV51* QuestPersistenceOwnerV51::resolve(std::uintptr_t id)const noexcept{
 for(const auto& list:rows_)for(const auto& q:list)if(q&&reinterpret_cast<std::uintptr_t>(q.get())==id)return q.get();return nullptr;
}
bool QuestPersistenceOwnerV51::load_callback(void* c,std::uintptr_t id,Bytes b,bool,std::size_t& used,std::string& e){return static_cast<QuestPersistenceOwnerV51*>(c)->load_quest(id,b,used,e);}
bool QuestPersistenceOwnerV51::load_quest(std::uintptr_t id,Bytes b,std::size_t& used,std::string& e){
 used=0;auto* q=resolve(id);if(!ready_||!q||(!b.data&&b.size)){e="Required SAME initialized Quest persistence identity/span";return false;}
 Reader r(b,e);bool ok=r.integer(q->state)&&load_objective(r,q->accept)&&load_objective(r,q->end);
 if(ok)for(auto& o:q->objectives)if(!load_objective(r,o)){ok=false;break;}
 used=r.used();if(!ok)return false;if(volatile_state(q->state))q->volatile64=1;e.clear();return true;
}
bool QuestPersistenceOwnerV51::save_quest(std::uintptr_t id,std::vector<std::uint8_t>& bytes,std::string& e)const{
 auto* q=resolve(id);if(!ready_||!q){e="Required SAME initialized Quest persistence writer identity";return false;}
 put_word(bytes,q->state);save_objective(bytes,q->accept);save_objective(bytes,q->end);for(const auto& o:q->objectives)save_objective(bytes,o);e.clear();return true;
}
bool QuestPersistenceOwnerV51::reinitialize(const std::function<bool(QuestPersistenceStateV51&,std::int32_t,std::string&)>& deliver,std::string& e){
 if(!ready_){e="Required initialized SAME Quest ReInit receiver";return false;}
 for(auto& list:rows_)for(auto& q:list){switch(q->state){
 case 2:case 3:case 5:case 6:case 8:case 9:
  if(!deliver){e="Required Quest ReInit old-state objective/marker operation "+std::to_string(q->state);return false;}
  if(!deliver(*q,q->state,e))return false;break;
 default:break;
 }q->state=q->definition->state;}
 e.clear();return true;
}
bool load_quest_metadata_acts_v51(std::shared_ptr<const QuestTablesPersistenceV51> tables,Bytes b,
 std::array<std::int32_t,3>& regular,std::array<std::int32_t,3>& volatile_acts,std::string& e){
 QuestSavegameV1 collections[2];QuestPersistenceOwnerV51 owners[2];
 collections[0].set_current_acts(regular);collections[1].set_current_acts(volatile_acts);
 if(!owners[0].construct(tables,0,collections[0],e)||!owners[1].construct(tables,0,collections[1],e))return false;
 std::size_t used{};std::array<bool,3> mismatches{};
 if(!collections[0].load(b,owners[0].load_services(),used,mismatches,e)){
  regular=collections[0].progress().current_act;return false;
 }
 regular=collections[0].progress().current_act;
 const bool ok=collections[1].load(b,owners[1].load_services(),used,mismatches,e);
 volatile_acts=collections[1].progress().current_act;return ok;
}
}
