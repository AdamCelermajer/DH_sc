#include "transparent_sort_v2.hpp"
#include <limits>
#include <utility>
namespace dh2::scene {
namespace {
bool sink(std::vector<TransparentEntryV1>& entries,std::size_t index,
 std::size_t exclusive,const TransparentQueueServicesV1& services,std::string& error){
 for(std::size_t child=index*2;child<exclusive;child=index*2){
  if(child+1<exclusive){
   bool less=false;
   if(!transparent_less_v1(entries[child-1],entries[child],services,less,error))return false;
   if(less)++child;
  }
  bool less=false;
  if(!transparent_less_v1(entries[index-1],entries[child-1],services,less,error))return false;
  if(!less)break;
  std::swap(entries[index-1],entries[child-1]);index=child;
 }
 return true;
}
}
bool transparent_sort_v2(std::vector<TransparentEntryV1>& entries,
 const TransparentQueueServicesV1& services,std::string& error){
 const auto count=entries.size();
 if(count>std::size_t(std::numeric_limits<std::int32_t>::max()-1)){
  error="Source transparent queue exceeds signed count domain";return false;
 }
 if(!count)return true;
 for(std::size_t index=(count-1)/2+1;index;--index)
  if(!sink(entries,index,count+1,services,error))return false;
 for(std::size_t remaining=count;remaining;--remaining){
  std::swap(entries.front(),entries[remaining-1]);
  if(!sink(entries,1,remaining,services,error))return false;
 }
 return true;
}
}
