#include "aggro.hpp"
#include <cstring>
#include <limits>

namespace {
using namespace dh2::data;
static_assert(sizeof(float)==4 && std::numeric_limits<float>::is_iec559);
bool valid(const AggroTable* table){
 if(!table||table->count>table->capacity||table->capacity>1048576||(!table->entries&&table->capacity))return false;
 std::uint64_t previous=0;
 for(std::uint32_t i=0;i<table->count;++i){const auto& e=table->entries[i];if(!e.character||e.character<=previous||e.reserved)return false;previous=e.character;}
 return true;
}
std::uint32_t lower(const AggroTable& table,std::uint64_t key){
 std::uint32_t first=0,last=table.count;
 while(first<last){const auto middle=first+(last-first)/2;if(table.entries[middle].character<key)first=middle+1;else last=middle;}
 return first;
}
bool found(const AggroTable& table,std::uint32_t at,std::uint64_t key){return at<table.count&&table.entries[at].character==key;}
float floating(std::uint32_t bits){float value;std::memcpy(&value,&bits,4);return value;}
std::uint32_t bits(float value){std::uint32_t result;std::memcpy(&result,&value,4);return result;}
std::uint32_t sum(std::uint32_t a,std::uint32_t b){volatile float result=floating(a)+floating(b);return bits(result);}
std::uint32_t difference(std::uint32_t a,std::uint32_t b){volatile float result=floating(a)-floating(b);return bits(result);}
void put(AggroTable& table,std::uint32_t at,std::uint64_t key,std::uint32_t value,bool exists){
 if(!exists){for(auto i=table.count;i>at;--i)table.entries[i]=table.entries[i-1];++table.count;}
 table.entries[at]={key,value,0};
}
void erase(AggroTable& table,std::uint32_t at){for(auto i=at+1;i<table.count;++i)table.entries[i-1]=table.entries[i];--table.count;}
bool overlaps(const AggroTable& a,const AggroTable& b){
 if(!a.capacity||!b.capacity)return false;
 const auto first=reinterpret_cast<std::uintptr_t>(a.entries),second=reinterpret_cast<std::uintptr_t>(b.entries);
 const auto bytes_a=std::uintptr_t(a.capacity)*sizeof(AggroEntry),bytes_b=std::uintptr_t(b.capacity)*sizeof(AggroEntry);
 if(first>std::numeric_limits<std::uintptr_t>::max()-bytes_a||second>std::numeric_limits<std::uintptr_t>::max()-bytes_b)return true;
 return first<second+bytes_b&&second<first+bytes_a;
}
}
extern "C" unsigned dh2_aggro_apply(dh2::data::AggroChange* out,const dh2::data::AggroRequest* request,unsigned operation){
 using namespace dh2::data;
 if(!out||!request||operation>aggro_clear||!request->owner||!request->target||(request->facts&~15u)||!valid(request->outgoing)||!valid(request->target_incoming)||request->outgoing==request->target_incoming||overlaps(*request->outgoing,*request->target_incoming))return 1;
 auto& outgoing=*request->outgoing;auto& incoming=*request->target_incoming;
 const auto at=lower(outgoing,request->target),reciprocal=lower(incoming,request->owner);
 const bool exists=found(outgoing,at,request->target),reverse_exists=found(incoming,reciprocal,request->owner);
 AggroChange next{};
 if(operation==aggro_clear){
  if(exists){erase(outgoing,at);if(reverse_exists)erase(incoming,reciprocal);next.removed=1;next.requests=aggro_notify_target_cleared;}
  if(request->facts&aggro_target_targets_owner)next.requests|=aggro_clear_target|aggro_stop_controller;
 }else{
  const auto previous=exists?outgoing.entries[at].threat_bits:0u;
  // Add returns Set's result minus the previous value. A rejected Set with an
  // existing entry therefore returns -previous, without changing that entry.
  const bool rejected=request->facts&(aggro_owner_player|aggro_owner_dead|aggro_target_dead);
  const auto supplied=operation==aggro_add&&exists?sum(request->amount_bits,previous):request->amount_bits;
  const auto stored=rejected?0u:supplied;
  if(!rejected){
   if((!exists&&outgoing.count==outgoing.capacity)||(!reverse_exists&&incoming.count==incoming.capacity))return 2;
   put(outgoing,at,request->target,stored,exists);put(incoming,reciprocal,request->owner,stored,reverse_exists);
   next.inserted=!exists;next.requests=exists?0u:aggro_notify_target;
  }
  next.returned_bits=operation==aggro_add&&exists?difference(stored,previous):stored;
 }
 *out=next;return 0;
}
extern "C" unsigned dh2_aggro_query(dh2::data::AggroQuery* out,const dh2::data::AggroTable* table,std::uint64_t target,std::uint32_t incoming_count){
 using namespace dh2::data;
 if(!out||!target||!valid(table))return 1;
 const auto at=lower(*table,target);AggroQuery next{found(*table,at,target)?table->entries[at].threat_bits:0u,table->count,unsigned(table->count!=0),unsigned(incoming_count!=0),0};
 float best=0.f;
 for(std::uint32_t i=0;i<table->count;++i){const auto value=floating(table->entries[i].threat_bits);if(value>best){best=value;next.highest=table->entries[i].character;}}
 *out=next;return 0;
}
