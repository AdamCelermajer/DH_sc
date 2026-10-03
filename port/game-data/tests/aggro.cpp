#include "aggro.hpp"
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::data;
namespace {
void check(bool condition,const char* message){if(!condition)throw std::runtime_error(message);}
template<class T>T load(const std::uint8_t* bytes){T result;std::memcpy(&result,bytes,sizeof result);return result;}
bool float_equal(std::uint32_t expected,std::uint32_t actual){
 if(expected==actual)return true;float a,b;std::memcpy(&a,&expected,4);std::memcpy(&b,&actual,4);return std::isnan(a)&&std::isnan(b);
}
void compare_entries(const AggroTable& actual,const std::uint8_t* expected){
 for(unsigned i=0;i<actual.count;++i){const auto entry=load<AggroEntry>(expected+i*16);check(actual.entries[i].character==entry.character&&actual.entries[i].reserved==entry.reserved&&float_equal(entry.threat_bits,actual.entries[i].threat_bits),"Aggression entry differs from original");}
}
}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;std::ifstream reference(argv[1],std::ios::binary);std::uint32_t count=0;reference.read(reinterpret_cast<char*>(&count),4);check(bool(reference)&&count,"Missing original aggression reference");
 check(sizeof(AggroEntry)==16&&sizeof(AggroTable)==16&&sizeof(AggroRequest)==40&&sizeof(AggroChange)==16&&sizeof(AggroQuery)==24,"Aggression ABI dimensions differ");
 std::array<std::uint8_t,600> record{};unsigned inserted=0,removed=0,notifications=0,clears=0;
 for(unsigned i=0;i<count;++i){
  reference.read(reinterpret_cast<char*>(record.data()),record.size());check(bool(reference),"Truncated aggression reference");const auto* r=record.data();
  std::array<AggroEntry,8> entries{},reverse{};std::memcpy(entries.data(),r+40,128);std::memcpy(reverse.data(),r+168,128);
  AggroTable outgoing{entries.data(),load<unsigned>(r+32),8},incoming{reverse.data(),load<unsigned>(r+36),8};
  const auto operation=load<unsigned>(r);const AggroRequest request{&outgoing,&incoming,load<std::uint64_t>(r+16),load<std::uint64_t>(r+24),load<unsigned>(r+4),load<unsigned>(r+8)};
  AggroChange result{};check(dh2_aggro_apply(&result,&request,operation)==0,"Original aggression fixture rejected");const auto expected=load<AggroChange>(r+560);
  check(float_equal(expected.returned_bits,result.returned_bits)&&expected.requests==result.requests&&expected.inserted==result.inserted&&expected.removed==result.removed,"Aggression change differs from original");
  if(operation==aggro_set)check(result.returned_bits==expected.returned_bits,"Set did not preserve supplied IEEE bits");
  check(outgoing.count==load<unsigned>(r+296)&&incoming.count==load<unsigned>(r+300),"Aggression counts differ from original");compare_entries(outgoing,r+304);compare_entries(incoming,r+432);
  AggroQuery queried{};check(dh2_aggro_query(&queried,&outgoing,request.target,load<unsigned>(r+12))==0,"Aggression query rejected");const auto q=load<AggroQuery>(r+576);
  check(float_equal(q.threat_bits,queried.threat_bits)&&q.count==queried.count&&q.has_aggro==queried.has_aggro&&q.is_aggroed==queried.is_aggroed&&q.highest==queried.highest,"Aggression queries differ from original");
  inserted+=result.inserted;removed+=result.removed;notifications+=bool(result.requests&aggro_notify_target);clears+=bool(result.requests&aggro_clear_target);
 }
 check(reference.peek()==std::char_traits<char>::eof(),"Unexpected aggression reference suffix");
 std::array<AggroEntry,2> entries{{{2,0x3f800000,0},{0,0,0}}},reverse{{{3,0x40000000,0},{0,0,0}}};AggroTable outgoing{entries.data(),1,2},incoming{reverse.data(),1,1};AggroRequest request{&outgoing,&incoming,1,4,0x40000000,0};AggroChange result{10,11,12,13},snapshot=result;
 const auto entry_snapshot=entries,reverse_snapshot=reverse;const auto outgoing_snapshot=outgoing,incoming_snapshot=incoming;
 auto unchanged=[&]{return std::memcmp(entries.data(),entry_snapshot.data(),sizeof entries)==0&&std::memcmp(reverse.data(),reverse_snapshot.data(),sizeof reverse)==0&&std::memcmp(&outgoing,&outgoing_snapshot,sizeof outgoing)==0&&std::memcmp(&incoming,&incoming_snapshot,sizeof incoming)==0&&std::memcmp(&result,&snapshot,sizeof result)==0;};
 check(dh2_aggro_apply(&result,&request,aggro_set)==2&&unchanged(),"Reciprocal capacity failure partially wrote tables");
 incoming.capacity=2;outgoing.capacity=1;auto limited_outgoing=outgoing;auto limited_incoming=incoming;
 check(dh2_aggro_apply(&result,&request,aggro_add)==2&&std::memcmp(entries.data(),entry_snapshot.data(),sizeof entries)==0&&std::memcmp(reverse.data(),reverse_snapshot.data(),sizeof reverse)==0&&std::memcmp(&outgoing,&limited_outgoing,sizeof outgoing)==0&&std::memcmp(&incoming,&limited_incoming,sizeof incoming)==0&&std::memcmp(&result,&snapshot,sizeof result)==0,"Outgoing capacity failure partially wrote tables");
 outgoing=outgoing_snapshot;incoming=incoming_snapshot;
 for(unsigned invalid:{0u,16u}){request.facts=invalid?invalid:0;request.target=invalid?4:0;check(dh2_aggro_apply(&result,&request,aggro_set)==1&&unchanged(),"Invalid aggression request mutated tables");}
 request.target=4;request.facts=0;check(dh2_aggro_apply(&result,&request,3)==1&&unchanged(),"Invalid operation mutated tables");
 request.owner=0;check(dh2_aggro_apply(&result,&request,aggro_clear)==1&&unchanged(),"Missing owner mutated tables");request.owner=1;
 request.target_incoming=&outgoing;check(dh2_aggro_apply(&result,&request,aggro_set)==1&&unchanged(),"Aliased table accepted");request.target_incoming=&incoming;
 auto overlap=incoming;overlap.entries=entries.data();request.target_incoming=&overlap;check(dh2_aggro_apply(&result,&request,aggro_set)==1&&unchanged(),"Overlapping storage accepted");request.target_incoming=&incoming;
 entries[0].reserved=1;const auto reserved_entries=entries;check(dh2_aggro_apply(&result,&request,aggro_set)==1&&std::memcmp(entries.data(),reserved_entries.data(),sizeof entries)==0&&std::memcmp(&result,&snapshot,sizeof result)==0,"Invalid reserved entry accepted");entries=entry_snapshot;
 entries[1]=entries[0];outgoing.count=2;const auto duplicate_entries=entries;check(dh2_aggro_apply(&result,&request,aggro_clear)==1&&std::memcmp(entries.data(),duplicate_entries.data(),sizeof entries)==0&&std::memcmp(&result,&snapshot,sizeof result)==0,"Duplicate keys accepted");entries=entry_snapshot;outgoing=outgoing_snapshot;
 check(dh2_aggro_apply(nullptr,&request,0)==1&&dh2_aggro_apply(&result,nullptr,0)==1&&unchanged(),"Missing request/output accepted");
 AggroQuery query{10,11,12,13,14},query_snapshot=query;check(dh2_aggro_query(&query,&outgoing,0,1)==1&&std::memcmp(&query,&query_snapshot,sizeof query)==0&&dh2_aggro_query(nullptr,&outgoing,4,1)==1&&dh2_aggro_query(&query,nullptr,4,1)==1,"Invalid query modified output");
 // Rejected original Set/Add needs no new capacity and must not insert.
 request.facts=aggro_owner_player;check(dh2_aggro_apply(&result,&request,aggro_set)==0&&result.returned_bits==0&&result.requests==0&&outgoing.count==1&&incoming.count==1,"Rejected Set tried allocating a reciprocal entry");
 std::cout<<"{\"original_aggro_cases\":"<<count<<",\"inserted\":"<<inserted<<",\"removed\":"<<removed<<",\"aggro_notifications\":"<<notifications<<",\"target_clear_requests\":"<<clears<<",\"finite_float_bits_exact\":true,\"arithmetic_nan_comparison\":\"unordered class\",\"invalid_input_and_capacity_failures_atomic\":true}\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 3;}}
