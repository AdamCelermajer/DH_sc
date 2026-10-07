#include "game_event_manager_v50.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::loader {
namespace {
std::int32_t signed_word(std::uint32_t v){std::int32_t out;std::memcpy(&out,&v,4);return out;}
class Reader {const std::uint8_t* p_;std::size_t n_,at_{};public:Reader(const std::uint8_t* p,std::size_t n):p_(p),n_(n){if((n&&!p)||n>8u*1024u*1024u)throw std::runtime_error("Invalid event table byte span");}
 std::uint32_t word(){if(n_-at_<4)throw std::runtime_error("Truncated event table word");auto p=p_+at_;at_+=4;return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::int32_t integer(){return signed_word(word());}
 std::string text(){auto n=word();if(n>65536||n>n_-at_)throw std::runtime_error("Truncated/oversize event string");std::string v(reinterpret_cast<const char*>(p_+at_),n);at_+=n;return v;}
 bool finished()const{return at_==n_;}
};
//Actual s_ObjectiveImplDataMap969908 is0x34 bytes: thirteen factories.
constexpr std::array<std::uint32_t,13> sizes{48,48,40,40,36,44,24,40,40,40,48,48,36};
bool table_valid(const GameEventTablesBorrowV50& t){return t.actual_owner&&t.size&&t.members&&t.names&&*t.size<=65536&&*t.size<=t.members_bound&&*t.size<=t.names_bound&&(!*t.size||(*t.members&&*t.names));}
bool same_table(const GameEventTablesBorrowV50& a,const GameEventTablesBorrowV50& b){return a.actual_owner.get()==b.actual_owner.get()&&!a.actual_owner.owner_before(b.actual_owner)&&!b.actual_owner.owner_before(a.actual_owner)&&a.size==b.size&&a.members==b.members&&a.names==b.names;}
}
struct GameEventTablesV50::Snapshot {std::uint32_t count{};std::vector<GameEventRowV50> rows;std::vector<std::string> names_storage;std::vector<const char*> name_pointers;const GameEventRowV50* members{};const char* const* names{};};
bool GameEventTablesV50::initialize(const std::uint8_t* records,std::size_t nr,const std::uint8_t* names,std::size_t nn,std::string& error){
 if(snapshot_&&snapshot_.use_count()!=1){error="Actual event table has live borrowers; refusing reload";return false;}
 try{Reader r(records,nr),n(names,nn);auto next=std::make_shared<Snapshot>();next->count=r.word();if(next->count>65536)throw std::runtime_error("Event row count outside native bound");next->rows.reserve(next->count);std::uint32_t total=0;
  for(std::uint32_t i=0;i<next->count;++i){GameEventRowV50 row;row.field4=r.integer();row.script_c=r.text();auto count=r.word();if(count>4096||total>65536-count)throw std::runtime_error("Event objectives outside native storage bound");total+=count;row.objectives14.reserve(count);
   for(std::uint32_t j=0;j<count;++j){GameEventObjectiveRowV50 s;s.type4=r.integer();s.field8=r.integer();s.fieldc=r.integer();s.string14=r.text();s.string1c=r.text();s.field20=r.integer();s.field24=r.integer();s.field28=r.integer();row.objectives14.push_back(std::move(s));}next->rows.push_back(std::move(row));}
  if(!r.finished()||n.word()!=next->count)throw std::runtime_error("Event records/name counts or stream tails differ");next->names_storage.reserve(next->count);next->name_pointers.reserve(next->count);
  for(std::uint32_t i=0;i<next->count;++i)next->names_storage.push_back(n.text());if(!n.finished())throw std::runtime_error("Unexpected event names tail");
  for(const auto& name:next->names_storage)next->name_pointers.push_back(name.c_str());next->members=next->rows.data();next->names=next->name_pointers.data();snapshot_=std::move(next);error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
GameEventTablesBorrowV50 GameEventTablesV50::borrow()const noexcept {if(!snapshot_)return {};return {snapshot_,&snapshot_->count,&snapshot_->members,&snapshot_->names,snapshot_->count,snapshot_->count};}
GameEventObjectiveV50::GameEventObjectiveV50(const GameEventObjectiveRowV50& row){
 if(row.type4<0||row.type4>=std::int32_t(sizes.size()))throw std::runtime_error("Required original-supported objective constructor type0..12");
 fields_.type4=row.type4;fields_.data_c=&row;original_size_=sizes[row.type4];saved_quantity_=row.type4!=4&&row.type4!=6&&row.type4!=12;
 if(row.type4!=6)fields_.words20_2c[0]=0;
 if(row.type4==0||row.type4==1||row.type4==10||row.type4==11){fields_.words20_2c[1]=0;fields_.words20_2c[2]=0;fields_.words20_2c[3]=0;}
 else if(row.type4!=4&&row.type4!=6&&row.type4!=12)fields_.words20_2c[1]=0xffffffffu;
 if(row.type4==5)fields_.words20_2c[2]=0;
}
void GameEventObjectiveV50::reset_data()noexcept {fields_.completed14=0;if(saved_quantity_)fields_.words20_2c[0]=0;}
bool GameEventV50::reinit_storage(std::string& error) {if(destruction_attempted_v1_){error="Cannot ReInit native-destroyed GameEvent storage";return false;}fields_.state0=0;for(std::int32_t i=0;i<fields_.objective_count_c;++i){if(std::size_t(i)>=objectives_.size()||!objectives_[i]){error="Incomplete ObjectiveList cannot ReInit";return false;}objectives_[i]->reset_data();}error.clear();return true;}
bool GameEventManagerV50::budget(GameEventStorageAllocationV50 kind,std::uint32_t n,std::string& e){if(!policy_.allow)return true;if(!policy_.owner){e="Required allocation policy owner";return false;}const bool ok=policy_.allow(kind,n,e);if(phase_==Phase::failed){e=diagnostics_.error;return false;}return ok;}
GameEventLoadStatusV50 GameEventManagerV50::fail(const std::string& e){phase_=Phase::failed;diagnostics_.error=e.empty()?"GameEvent storage allocation/provider failed":e;return GameEventLoadStatusV50::failed;}
GameEventV50* GameEventManagerV50::by_id(std::int32_t id)const noexcept {return id>=0&&std::size_t(id)<events_.size()?events_[id].get():nullptr;}
GameEventLoadStatusV50 GameEventManagerV50::load_step(GameEventTablesBorrowV50 input){
 if(destruction_attempted_v1_)return fail("Cannot load into native-destroyed GameEventManager storage");
 if(busy_)return fail("GameEvent.Load reentered; retaining original prefix");if(phase_==Phase::failed)return GameEventLoadStatusV50::failed;
 if(phase_==Phase::complete&&!events_.empty())return GameEventLoadStatusV50::complete; // Original nonempty Load no-op BEFORE tables read.
 struct Guard {bool& b;explicit Guard(bool& v):b(v){b=true;}~Guard(){b=false;}} guard(busy_);std::string e;
 try{
  if(phase_==Phase::complete){phase_=Phase::idle;tables_={};diagnostics_={};cursor_=0;}
  if(phase_==Phase::idle){if(!table_valid(input))return fail("Required actual initialized v2Events size/members/names owner");tables_=std::move(input);const auto count=*tables_.size;
   if(!budget(GameEventStorageAllocationV50::manager_slots,count,e))return fail(e);events_.resize(count);diagnostics_.slots=count;
   if(!count){phase_=Phase::complete;diagnostics_.storage_load_complete=true;return GameEventLoadStatusV50::complete;}phase_=Phase::event;return GameEventLoadStatusV50::pending;
  }
  if(!table_valid(input)||!same_table(input,tables_)||events_.size()>input.members_bound||events_.size()>input.names_bound||(!events_.empty()&&(!*input.members||!*input.names)))return fail("Actual v2Events borrow changed during source Load");
  if(phase_==Phase::event){const auto* row=*tables_.members+cursor_; // Original captures members BEFORE new/C1.
   if(!budget(GameEventStorageAllocationV50::event32,32,e))return fail(e);pending_=std::make_unique<GameEventV50>();auto& f=pending_->fields_;
   if(!*tables_.names||!(*tables_.names)[cursor_])return fail("Required actual event name pointer");f.id4=std::int32_t(cursor_);f.name8=(*tables_.names)[cursor_];f.data1c=row;
   if(row->objectives14.size()>4096)return fail("Actual objective count outside native bound");f.objective_count_c=std::int32_t(row->objectives14.size());f.objective_data14=row->objectives14.data();
   if(f.objective_count_c>0){if(!budget(GameEventStorageAllocationV50::objective_slots,std::uint32_t(f.objective_count_c),e))return fail(e);pending_->objectives_.resize(std::size_t(f.objective_count_c));}
   objective_cursor_=0;phase_=Phase::objective;return GameEventLoadStatusV50::pending;
  }
  if(phase_==Phase::objective){if(objective_cursor_<pending_->objectives_.size()){const auto& row=pending_->fields_.objective_data14[objective_cursor_];if(row.type4<0||row.type4>=13)return fail("Required original-supported objective constructor type0..12");if(!budget(GameEventStorageAllocationV50::objective,sizes[row.type4],e))return fail(e);pending_->objectives_[objective_cursor_]=std::make_unique<GameEventObjectiveV50>(row);++objective_cursor_;++diagnostics_.objective_constructors;return GameEventLoadStatusV50::pending;}
   pending_->fields_.state0=0;objective_cursor_=0;phase_=Phase::reset;return GameEventLoadStatusV50::pending;
  }
  if(phase_==Phase::reset){if(objective_cursor_<pending_->objectives_.size()){pending_->objectives_[objective_cursor_]->reset_data();++objective_cursor_;++diagnostics_.objective_resets;return GameEventLoadStatusV50::pending;}phase_=Phase::publish;}
  if(phase_==Phase::publish){events_[cursor_]=std::move(pending_);++cursor_;++diagnostics_.published;if(cursor_==events_.size()){phase_=Phase::complete;diagnostics_.storage_load_complete=true;return GameEventLoadStatusV50::complete;}phase_=Phase::event;}
  return GameEventLoadStatusV50::pending;
 }catch(const std::exception& ex){return fail(ex.what());}catch(...){return fail("GameEvent storage policy threw");}
}
// Source storage release, distinct from functional Compile/Register effects.
// Passive shared/unique expiration never certifies this native D0 boundary.
bool GameEventV50::destroy_native_storage_v1(const GameEventNativeDestructionV1& services,std::string& e){
 if(destruction_complete_v1_){e.clear();return true;}
 if(destruction_attempted_v1_){e=destruction_failure_v1_.empty()?"GameEvent D1 cannot replay/reenter a reached prefix":destruction_failure_v1_;return false;}
 destruction_attempted_v1_=true;
 auto fail=[&]{destruction_failure_v1_=e.empty()?"Required actual Objective derived D1 before slot/storage clear":e;e=destruction_failure_v1_;return false;};
 for(auto& objective:objectives_){if(!objective)continue;
  if(!services.owner||!services.objective_d1||!services.objective_d1(*objective,e))return fail();
  // Objective virtualD0 reached its genuine Main body above. Now the SAME
  // native successor allocation is freed, then this original slot is NULL.
  objective.reset();
 }
 std::vector<std::unique_ptr<GameEventObjectiveV50>>{}.swap(objectives_);
 destruction_complete_v1_=true;e.clear();return true;
}
bool GameEventManagerV50::destroy_native_storage_v1(const GameEventNativeDestructionV1& services,std::string& e){
 if(destruction_complete_v1_){e.clear();return true;}
 if(busy_||destruction_attempted_v1_){e=destruction_failure_v1_.empty()?"GameEventManager D1 cannot replay/reenter a reached prefix":destruction_failure_v1_;return false;}
 destruction_attempted_v1_=true;busy_=true;struct Guard{bool& busy;~Guard(){busy=false;}}guard{busy_};
 auto fail=[&]{destruction_failure_v1_=e.empty()?"Required actual GameEvent D1 before manager vector/storage clear":e;e=destruction_failure_v1_;return false;};
 for(auto& event:events_){if(!event)continue;if(!event->destroy_native_storage_v1(services,e))return fail();event.reset();}
 // Source Unload moves end to begin; D1 frees the actual vector allocation.
 std::vector<std::unique_ptr<GameEventV50>>{}.swap(events_);
 // This separate pending_ exists only for the native bounded Load adapter.
 // Preserve/retire its real constructed prefix; never create absent receivers.
 if(pending_){if(!pending_->destroy_native_storage_v1(services,e))return fail();pending_.reset();}
 destruction_complete_v1_=true;e.clear();return true;
}
}
