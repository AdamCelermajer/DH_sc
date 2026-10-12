#include "../character_deferred_queue.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>
using namespace dh2::character;
using Rows=std::vector<std::pair<std::uint32_t,std::uint32_t>>;
void require(bool value){if(!value)throw std::runtime_error("Deferred queue audit mismatch");}
std::int32_t signed_word(std::uint32_t word){std::int32_t value;std::memcpy(&value,&word,4);return value;}
struct Event {
 std::array<std::uint32_t,5> fields;Rows rows;
 bool operator==(const Event& other)const{return fields==other.fields&&rows==other.rows;}
};
struct Fixture {
 CharacterDeferredQueue* queue=nullptr;
 std::array<DeferredQueueOwner16,32> owners{};
 DeferredQueueServices24 services{this,invoke,3,0};
 std::vector<Event> events;std::uint32_t action=0;bool mutated=false;
 int failure=-1;bool delete_active=false,erase_captured=false;int destroy_status=-1;
 Fixture(){for(std::uint32_t i=0;i<owners.size();++i)owners[i]={i+1,0x6000+i+1};}
 ~Fixture(){if(queue)require(dh2_character_deferred_queue_destroy(queue)==0);}
 Rows snapshot(){
  std::array<DeferredQueueRow16,1024> rows{};std::uint32_t count=0;
  require(dh2_character_deferred_queue_snapshot(queue,rows.data(),rows.size(),&count)==0);
  Rows out;for(std::uint32_t i=0;i<count;++i){require(rows[i].reserved==0);out.emplace_back(static_cast<std::uint32_t>(rows[i].key),rows[i].owner);}return out;
 }
 void reset(){require(!queue);queue=dh2_character_deferred_queue_create();require(queue);events.clear();action=0;mutated=false;failure=-1;delete_active=false;erase_captured=false;destroy_status=-1;}
 void assign(std::uint32_t key,std::uint32_t id){require(id&&id<=owners.size());require(dh2_character_deferred_queue_assign(queue,signed_word(key),&owners[id-1],nullptr)==0);}
 int unload(std::uint32_t id,std::uint32_t key,std::uint32_t final,bool explicit_token){
  DeferredQueueToken24 token{queue,0,0,0};if(explicit_token)require(dh2_character_deferred_queue_find(queue,signed_word(key),&token)==0);
  return dh2_character_deferred_queue_unload(queue,&owners[id-1],&token,final,&services);
 }
 static int invoke(void* context,CharacterDeferredQueue* q,const DeferredQueueRequest32* request){
  auto& f=*static_cast<Fixture*>(context);require(q==f.queue&&request&&request->reserved==0);
  f.events.push_back({{request->operation,static_cast<std::uint32_t>(request->owner),static_cast<std::uint32_t>(request->controller),request->final,static_cast<std::uint32_t>(request->key)},f.snapshot()});
  if(f.delete_active)f.destroy_status=dh2_character_deferred_queue_destroy(q);
  if(f.erase_captured&&request->operation==deferred_queue_kill&&!f.mutated){
   f.mutated=true;require(f.unload(request->owner,static_cast<std::uint32_t>(request->key),1,true)==0);f.assign(static_cast<std::uint32_t>(request->key),32);
  }
  if(!f.mutated&&((request->operation==deferred_queue_kill&&f.action>=1&&f.action<=3)||(request->operation==deferred_queue_unload_ai&&f.action==4))){
   f.mutated=true;
   if(f.action==1)f.assign(static_cast<std::uint32_t>(request->key),32);
   else if(f.action==2)f.assign(0x80000000,31);
   else if(f.action==3){
    std::uint32_t unrelated=0;for(const auto& row:f.events.back().rows)if(row.second!=request->owner){unrelated=row.second;break;}
    require(unrelated&&f.unload(unrelated,0,255,false)==0);
   }else require(dh2_character_deferred_queue_evict(q,0,&f.services)==0);
  }
  return static_cast<int>(request->operation)==f.failure?1:0;
 }
 void destroy(){require(dh2_character_deferred_queue_destroy(queue)==0);queue=nullptr;}
};
std::uint32_t word(std::istream& input){std::uint32_t value=0;input.read(reinterpret_cast<char*>(&value),4);require(bool(input));return value;}
Rows read_rows(std::istream& input,std::uint32_t count){Rows out;for(std::uint32_t i=0;i<count;++i){const auto key=word(input),owner=word(input);out.emplace_back(key,owner);}return out;}
int main(int argc,char** argv){try{
 require(argc==2);std::ifstream input(argv[1],std::ios::binary);char magic[4];input.read(magic,4);require(!std::memcmp(magic,"CDQ1",4));const auto count=word(input);
 Fixture f;std::uint32_t callbacks=0,entries=0,reentries=0,guards=0;
 for(std::uint32_t i=0;i<count;++i){
  const auto op=word(input),a=word(input),b=word(input),d=word(input),e=word(input),nr=word(input),ne=word(input);
  const auto rows=read_rows(input,nr);std::vector<Event> events;
  for(std::uint32_t j=0;j<ne;++j){Event event;for(auto& value:event.fields)value=word(input);const auto n=word(input);event.rows=read_rows(input,n);entries+=n;events.push_back(event);}
  f.events.clear();
  if(op==0)f.reset();
  else if(op==1)f.assign(a,b);
  else if(op==2)require(f.unload(a,b,d,e!=0)==0);
  else if(op==3){f.action=b;f.mutated=false;require(dh2_character_deferred_queue_evict(f.queue,a,&f.services)==0);reentries+=f.mutated;}
  else if(op==4)f.destroy();else require(false);
  require((f.queue?f.snapshot():Rows{})==rows&&f.events==events);callbacks+=ne;entries+=nr;
 }
 require(input.peek()==std::char_traits<char>::eof()&&!f.queue);
 // Safety cases are native provider contracts; deleted source iterators are UB.
 f.reset();for(std::uint32_t i=0;i<9;++i)f.assign(i,i+1);
 f.delete_active=true;require(dh2_character_deferred_queue_evict(f.queue,0,&f.services)==0&&f.destroy_status==2);++guards;f.delete_active=false;
 require(f.snapshot().size()==8);++guards;
 std::uint32_t output_count=0xdeadbeef;DeferredQueueRow16 row{123,456,789};const auto copy=row;
 require(dh2_character_deferred_queue_snapshot(f.queue,&row,1,&output_count)==1&&output_count==0xdeadbeef&&!std::memcmp(&row,&copy,sizeof row));++guards;
 DeferredQueueToken24 stale;require(dh2_character_deferred_queue_find(f.queue,1,&stale)==0);require(dh2_character_deferred_queue_unload(f.queue,&f.owners[2],&stale,255,&f.services)==0);++guards;
 // Explicit iterator receiver may differ from stored mapped owner, as source.
 require(f.events.back().fields[1]==3&&f.events.back().fields[3]==255&&f.events.back().fields[4]==1);++guards;
 require(dh2_character_deferred_queue_unload(f.queue,&f.owners[2],&stale,1,&f.services)==1);++guards;
 f.assign(1,2);require(dh2_character_deferred_queue_unload(f.queue,&f.owners[2],&stale,1,&f.services)==1);++guards;
 auto* other=dh2_character_deferred_queue_create();require(other);DeferredQueueToken24 foreign{other,0,0,0};
 require(dh2_character_deferred_queue_unload(f.queue,&f.owners[0],&foreign,1,&f.services)==1);++guards;require(dh2_character_deferred_queue_destroy(other)==0);
 DeferredQueueToken24 end{f.queue,0,0,0};const auto before=f.snapshot();f.services.available=0;
 require(dh2_character_deferred_queue_unload(f.queue,&f.owners[1],&end,1,&f.services)==2&&f.snapshot().size()==before.size()-1);++guards;
 f.services.available=3;f.destroy();f.reset();for(std::uint32_t i=0;i<9;++i)f.assign(i,i+1);
 f.services.available=2;require(dh2_character_deferred_queue_evict(f.queue,0,&f.services)==2&&f.snapshot().size()==9&&f.events.empty());++guards;
 f.services.available=3;f.erase_captured=true;require(dh2_character_deferred_queue_evict(f.queue,0,&f.services)==1&&f.snapshot().size()==9);++guards;f.erase_captured=false;
 f.failure=0;f.mutated=false;require(dh2_character_deferred_queue_evict(f.queue,0,&f.services)==3&&f.snapshot().size()==9);++guards;f.failure=1;
 require(dh2_character_deferred_queue_evict(f.queue,0,&f.services)==3&&f.snapshot().size()==8);++guards;f.failure=-1;
 alignas(DeferredQueueOwner16) std::array<unsigned char,sizeof(DeferredQueueOwner16)+8> raw{};
 require(dh2_character_deferred_queue_assign(f.queue,1,reinterpret_cast<DeferredQueueOwner16*>(raw.data()+1),nullptr)==1);++guards;
 DeferredQueueOwner16 zero{};require(dh2_character_deferred_queue_assign(f.queue,1,&zero,nullptr)==1);++guards;
 require(dh2_character_deferred_queue_snapshot(f.queue,nullptr,1024,&output_count)==1);++guards;
 f.destroy();require(f.owners[0].identity==1&&f.owners[31].identity==32);++guards;
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"ordered_callbacks\":"<<callbacks<<",\"ordered_entries\":"<<entries<<",\"synchronous_reentry_records\":"<<reentries<<",\"native_safety_checks\":"<<guards<<",\"mismatches\":0}\n";
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
