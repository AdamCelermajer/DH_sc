#include "character_deferred_queue.hpp"
#include <map>
#include <limits>
namespace dh2::character {
struct CharacterDeferredQueue {
 struct Entry {const DeferredQueueOwner16* owner;std::uint64_t generation;};
 std::map<std::int32_t,Entry> entries;
 std::uint64_t next_generation=1;
 std::uint32_t active=0;
};
}
namespace {
using namespace dh2::character;
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool owner(const DeferredQueueOwner16* p){return aligned(p)&&p->identity;}
bool token(CharacterDeferredQueue* queue,const DeferredQueueToken24* p){return aligned(p)&&p->queue==queue&&!p->reserved;}
DeferredQueueToken24 end(CharacterDeferredQueue* queue){return {queue,0,0,0};}
int call(CharacterDeferredQueue* queue,const DeferredQueueServices24* services,
 const DeferredQueueOwner16* receiver,std::uint32_t op,std::uint32_t final,std::int32_t key){
 if(!owner(receiver))return 1;
 if(!services)return 2;
 if(!aligned(services)||services->reserved)return 1;
 if(!services->invoke||!(services->available&(1u<<op)))return 2;
 if(op==deferred_queue_kill&&!receiver->controller)return 1;
 const DeferredQueueRequest32 request{op,final,receiver->identity,
                                    op==deferred_queue_kill?receiver->controller:0,key,0};
 ++queue->active;
 const int status=services->invoke(services->context,queue,&request);
 --queue->active;return status?3:0;
}
}
extern "C" CharacterDeferredQueue* dh2_character_deferred_queue_create(){
 try{return new CharacterDeferredQueue;}catch(...){return nullptr;}
}
extern "C" int dh2_character_deferred_queue_destroy(CharacterDeferredQueue* queue){
 if(!aligned(queue))return 1;
 if(queue->active)return 2;
 delete queue;return 0;
}
extern "C" int dh2_character_deferred_queue_assign(CharacterDeferredQueue* queue,std::int32_t key,
 const DeferredQueueOwner16* receiver,DeferredQueueToken24* output){
 if(!aligned(queue)||!owner(receiver)||(output&&!aligned(output)))return 1;
 try{
  auto found=queue->entries.find(key);
  if(found==queue->entries.end()){
   if(queue->next_generation==0||queue->entries.size()==std::numeric_limits<std::uint32_t>::max())return 3;
   found=queue->entries.emplace(key,CharacterDeferredQueue::Entry{receiver,queue->next_generation}).first;
   ++queue->next_generation;
  }else found->second.owner=receiver;
  if(output)*output={queue,key,0,found->second.generation};
  return 0;
 }catch(...){return 3;}
}
extern "C" int dh2_character_deferred_queue_first(CharacterDeferredQueue* queue,DeferredQueueToken24* output){
 if(!aligned(queue)||!aligned(output))return 1;
 const auto first=queue->entries.begin();
 *output=first==queue->entries.end()?end(queue):DeferredQueueToken24{queue,first->first,0,first->second.generation};return 0;
}
extern "C" int dh2_character_deferred_queue_find(CharacterDeferredQueue* queue,std::int32_t key,DeferredQueueToken24* output){
 if(!aligned(queue)||!aligned(output))return 1;
 const auto found=queue->entries.find(key);
 *output=found==queue->entries.end()?end(queue):DeferredQueueToken24{queue,key,0,found->second.generation};return 0;
}
extern "C" int dh2_character_deferred_queue_snapshot(const CharacterDeferredQueue* queue,
 DeferredQueueRow16* rows,std::uint32_t capacity,std::uint32_t* count){
 if(!aligned(queue)||!aligned(count)||capacity<queue->entries.size()||(!queue->entries.empty()&&!aligned(rows)))return 1;
 for(const auto& entry:queue->entries)if(!owner(entry.second.owner))return 1;
 std::uint32_t i=0;for(const auto& entry:queue->entries)rows[i++]={entry.first,0,entry.second.owner->identity};
 *count=i;return 0;
}
extern "C" int dh2_character_deferred_queue_unload(CharacterDeferredQueue* queue,
 const DeferredQueueOwner16* receiver,DeferredQueueToken24* iterator,std::uint32_t final,
 const DeferredQueueServices24* services){
 if(!aligned(queue)||!owner(receiver)||!token(queue,iterator))return 1;
 auto found=queue->entries.end();
 if(iterator->generation){
  found=queue->entries.find(iterator->key);
  if(found==queue->entries.end()||found->second.generation!=iterator->generation)return 1;
 }else{
  for(auto current=queue->entries.begin();current!=queue->entries.end();++current){
   if(!owner(current->second.owner))return 1;
   if(current->second.owner->identity==receiver->identity){found=current;break;}
  }
  *iterator=found==queue->entries.end()?end(queue):DeferredQueueToken24{queue,found->first,0,found->second.generation};
 }
 const auto key=found==queue->entries.end()?0:found->first;
 if(found!=queue->entries.end())queue->entries.erase(found);
 return call(queue,services,receiver,deferred_queue_unload_ai,final,key);
}
extern "C" int dh2_character_deferred_queue_evict(CharacterDeferredQueue* queue,
 std::uint32_t level,const DeferredQueueServices24* services){
 if(!aligned(queue))return 1;
 if(queue->entries.size()<=(level==29?24u:8u))return 0;
 auto first=queue->entries.begin();
 const DeferredQueueToken24 captured{queue,first->first,0,first->second.generation};
 int status=call(queue,services,first->second.owner,deferred_queue_kill,1,first->first);if(status)return status;
 first=queue->entries.find(captured.key);
 if(first==queue->entries.end()||first->second.generation!=captured.generation)return 1;
 auto iterator=captured;
 return dh2_character_deferred_queue_unload(queue,first->second.owner,&iterator,1,services);
}
