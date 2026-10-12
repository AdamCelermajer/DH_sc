from pathlib import Path
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/condition-list-v55'
(r/'condition_list_owner_v55.hpp').write_text(r'''#pragma once
#include "condition_data_services_v51.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace dh2::world {
// Decoded actual source transport, not a live native C++ vtable. Original
// CondStub reader emits this source-vtable provenance word plus op/args.
struct ConditionStubV55 {std::uint32_t source_vtable;std::int32_t op,argument1,argument2;};
static_assert(sizeof(ConditionStubV55)==16);
struct ConditionTableRowV55 {std::string name;std::int32_t list_op;std::vector<ConditionStubV55> stubs;};
class ConditionTablesV55 {
 std::vector<ConditionTableRowV55> storage_;
 std::vector<ConditionDataRowV3> rows_;
 ConditionTablesV55()=default;
public:
 static bool load(const std::uint8_t*,std::size_t,const std::uint8_t*,std::size_t,std::shared_ptr<const ConditionTablesV55>&,std::string&);
 const auto& rows()const noexcept{return rows_;}
 const auto& storage()const noexcept{return storage_;}
 bool contains(const ConditionStubV55*,std::int32_t)const noexcept;
};
struct ActualConditionBorrowV55 {
 std::uintptr_t receiver{};
 std::shared_ptr<const void> receiver_alias; // alias.get()==actual receiver
 std::uintptr_t* live_stub4{}; // main's checked SAME actual Condition field
};
struct ActualConditionFactoriesV55 {
 std::shared_ptr<void> owner;
 // Source table index0..6. Main owns each genuine constructor and evaluator.
 std::array<std::function<bool(ActualConditionBorrowV55&,std::string&)>,7> create;
 std::function<bool(const ActualConditionBorrowV55&,std::string&)> deleting_destructor;
};
// Three source cells of the real ConditionList. Native pointers use native
// width; original ARM allocation is12 bytes. This is NOT an ARM-image memcpy.
struct ConditionListFieldsV55 {std::int32_t count{};std::uintptr_t* instances4{};const ConditionStubV55* stubs8{};};
class ConditionListOwnerV55:public std::enable_shared_from_this<ConditionListOwnerV55> {
 struct Array;struct List;
 std::shared_ptr<const ConditionTablesV55> table_;
 ActualConditionFactoriesV55 factories_;
 std::vector<std::unique_ptr<List>> lists_;
 std::vector<std::unique_ptr<Array>> arrays_;
 List* find(std::uintptr_t)const noexcept;
 bool construct(std::uintptr_t&,std::string&);
 bool assign(std::uintptr_t,const ConditionStubV55*,std::int32_t,std::string&);
 bool destroy(std::uintptr_t,std::string&);
public:
 ConditionListOwnerV55(std::shared_ptr<const ConditionTablesV55>,ActualConditionFactoriesV55);
 ~ConditionListOwnerV55();
 ConditionDataInitServicesV3 services();
 const ConditionListFieldsV55* fields(std::uintptr_t)const noexcept;
 std::size_t retained_list_allocations()const noexcept;
 // Source repeated Init/Assign leaks are retained until owning-world teardown;
 // no eager Clear, evaluator, save/quest state, or second global is introduced.
};
}
''')
(r/'condition_list_owner_v55.cpp').write_text(r'''#include "condition_list_owner_v55.hpp"
#include <limits>
#include <stdexcept>
namespace dh2::world {
namespace {
struct Reader {
 const std::uint8_t* b;std::size_t n,at{};
 void need(std::size_t k){if(!b||k>n-at)throw std::runtime_error("Truncated actual Conditions data");}
 std::uint32_t word(){need(4);const auto* p=b+at;at+=4;return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::string text(){const auto k=word();need(k);std::string s(reinterpret_cast<const char*>(b+at),k);at+=k;return s;}
 void end(){if(at!=n)throw std::runtime_error("Unexpected Conditions data suffix");}
};
}
bool ConditionTablesV55::load(const std::uint8_t* data,std::size_t size,const std::uint8_t* names,std::size_t names_size,std::shared_ptr<const ConditionTablesV55>& out,std::string& error){
 try{
  if(size>8u*1024u*1024u||names_size>8u*1024u*1024u)throw std::runtime_error("Conditions input exceeds engineering limit");
  Reader r{data,size},n{names,names_size};const auto count=r.word();if(count!=n.word()||count>100000)throw std::runtime_error("Conditions name/row dimensions differ");
  auto table=std::shared_ptr<ConditionTablesV55>(new ConditionTablesV55);table->storage_.reserve(count);
  for(std::uint32_t i=0;i<count;++i){ConditionTableRowV55 row;row.name=n.text();const auto k=r.word();if(k>100000||k>std::numeric_limits<std::int32_t>::max())throw std::runtime_error("Malformed signed Conditions count");row.stubs.reserve(k);
   for(std::uint32_t j=0;j<k;++j){ConditionStubV55 stub{0x96a318u,static_cast<std::int32_t>(r.word()),static_cast<std::int32_t>(r.word()),static_cast<std::int32_t>(r.word())};row.stubs.push_back(stub);}
   row.list_op=static_cast<std::int32_t>(r.word());table->storage_.push_back(std::move(row));
  }
  r.end();n.end();table->rows_.reserve(count);
  for(const auto& row:table->storage_)table->rows_.push_back({row.name,row.stubs.size(),reinterpret_cast<std::uintptr_t>(row.stubs.data())});
  out=std::move(table);error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool ConditionTablesV55::contains(const ConditionStubV55* ptr,std::int32_t count)const noexcept {
 for(const auto& row:storage_)if(row.stubs.data()==ptr&&static_cast<std::int32_t>(row.stubs.size())==count)return true;return false;
}
struct ConditionListOwnerV55::Array {
 std::unique_ptr<std::uintptr_t[]> cells;
 std::vector<ActualConditionBorrowV55> produced;
 std::int32_t capacity{};
};
struct ConditionListOwnerV55::List {ConditionListFieldsV55 fields;Array* current{};};
ConditionListOwnerV55::ConditionListOwnerV55(std::shared_ptr<const ConditionTablesV55> table,ActualConditionFactoriesV55 factories):table_(std::move(table)),factories_(std::move(factories)){}
ConditionListOwnerV55::~ConditionListOwnerV55()=default;
ConditionListOwnerV55::List* ConditionListOwnerV55::find(std::uintptr_t receiver)const noexcept {
 for(const auto& list:lists_)if(reinterpret_cast<std::uintptr_t>(&list->fields)==receiver)return list.get();return nullptr;
}
bool ConditionListOwnerV55::construct(std::uintptr_t& out,std::string& error){
 auto p=std::make_unique<List>();const auto receiver=reinterpret_cast<std::uintptr_t>(&p->fields);lists_.push_back(std::move(p));out=receiver;error.clear();return true;
}
bool ConditionListOwnerV55::assign(std::uintptr_t receiver,const ConditionStubV55* stubs,std::int32_t count,std::string& error){
 auto* list=find(receiver);if(!list||!table_||(count>0&&!table_->contains(stubs,count))){error="Required same actual ConditionList/table receiver";return false;}
 auto& f=list->fields;f.stubs8=stubs;f.count=count;
 if(count<=0){error.clear();return true;}
 auto array=std::make_unique<Array>();array->capacity=count;array->cells.reset(new std::uintptr_t[static_cast<std::size_t>(count)]);array->produced.reserve(count);auto* current=array.get();arrays_.push_back(std::move(array));list->current=current;f.instances4=current->cells.get();
 for(std::int32_t i=0;i<count;++i){const auto op=stubs[i].op;
  if(op<0||op>=7){error="Unsupported original Condition factory index "+std::to_string(op);return false;}
  if(!factories_.owner||!factories_.create[op]){error="Required actual Condition factory type "+std::to_string(op);return false;}
  ActualConditionBorrowV55 instance;if(!factories_.create[op](instance,error))return false;
  if(!instance.receiver||!instance.receiver_alias||instance.receiver_alias.get()!=reinterpret_cast<const void*>(instance.receiver)||!instance.live_stub4){error="Required checked same actual Condition receiver/stub4 borrow";return false;}
  f.instances4[i]=instance.receiver;*instance.live_stub4=reinterpret_cast<std::uintptr_t>(stubs+i);current->produced.push_back(std::move(instance));
 }
 error.clear();return true;
}
bool ConditionListOwnerV55::destroy(std::uintptr_t receiver,std::string& error){
 auto* list=find(receiver);if(!list){error="Required same actual ConditionList destructor receiver";return false;}
 auto& f=list->fields;auto* array=list->current;
 if(f.count>0){
  if(!array||f.instances4!=array->cells.get()||f.count>array->capacity||array->produced.size()<static_cast<std::size_t>(f.count)){error="Required fully produced ConditionList before original D1 (partial constructor stop)";return false;}
  for(std::int32_t i=0;i<f.count;++i)if(f.instances4[i]){
   if(!factories_.owner||!factories_.deleting_destructor){error="Required actual Condition deleting destructor";return false;}
   auto& instance=array->produced[i];if(f.instances4[i]!=instance.receiver){error="Required same actual Condition array receiver";return false;}
   if(!factories_.deleting_destructor(instance,error))return false;f.instances4[i]=0;instance.receiver_alias.reset();
  }
 }
 if(f.instances4){if(!array||f.instances4!=array->cells.get()){error="Required actual ConditionList allocated array";return false;}array->cells.reset();f.instances4=nullptr;}
 // Original ConditionList D1 leaves count/stubs; caller CustomFree follows.
 for(auto i=lists_.begin();i!=lists_.end();++i)if(i->get()==list){lists_.erase(i);break;}
 error.clear();return true;
}
ConditionDataInitServicesV3 ConditionListOwnerV55::services(){
 ConditionDataInitServicesV3 s;s.owner=shared_from_this();s.conditions=table_?&table_->rows():nullptr;std::weak_ptr<ConditionListOwnerV55> weak=shared_from_this();
 s.construct_condition=[weak](std::uintptr_t& out,std::string& e){auto self=weak.lock();if(!self){e="Required retained actual ConditionList owner";return false;}return self->construct(out,e);};
 s.initialize_condition=[weak](std::uintptr_t id,std::uintptr_t stubs,std::uintptr_t count,std::string& e){auto self=weak.lock();if(!self){e="Required retained actual ConditionList owner";return false;}return self->assign(id,reinterpret_cast<const ConditionStubV55*>(stubs),static_cast<std::int32_t>(static_cast<std::uint32_t>(count)),e);};
 s.destroy_condition=[weak](std::uintptr_t id,std::string& e){auto self=weak.lock();if(!self){e="Required retained actual ConditionList owner";return false;}return self->destroy(id,e);};return s;
}
const ConditionListFieldsV55* ConditionListOwnerV55::fields(std::uintptr_t id)const noexcept {auto* p=find(id);return p?&p->fields:nullptr;}
std::size_t ConditionListOwnerV55::retained_list_allocations()const noexcept {return lists_.size();}
}
''')
(r/'write_conditions_candidate_v55.py').write_bytes(Path(__file__).read_bytes());print('WROTE actual table/list transport provider2files; no condition factories/evaluator implemented')
