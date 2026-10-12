#include "../object_identity.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
using namespace dh2::object_identity;
int main(int argc,char** argv){
 assert(argc==2);std::ifstream file(argv[1],std::ios::binary);assert(file);char magic[4];file.read(magic,4);assert(std::memcmp(magic,"OIL1",4)==0);
 auto word=[&](){std::uint32_t x=0;file.read(reinterpret_cast<char*>(&x),4);assert(file);return x;};
 const auto count=word();std::uint32_t checks=0;
 for(std::uint32_t i=0;i<count;++i){
  const auto op=word(),na=word(),no=word();assert(na<=65536&&no<=65536);std::vector<std::uint32_t>a(na),expected(no),out;
  for(auto&v:a)v=word();
  for(auto&v:expected)v=word();
  if(op==1){
   assert(na==5);Handle16 shared{static_cast<std::int32_t>(a[1]),a[3],a[2]},result{};
   assert(dh2_object_handle_from_pointer(&result,a[0]?&shared:nullptr,a[4])==0);
   out={static_cast<std::uint32_t>(result.key),result.frame,static_cast<std::uint32_t>(result.cached),static_cast<std::uint32_t>(shared.key),shared.frame,static_cast<std::uint32_t>(shared.cached)};
  }else if(op==2||op==3){
   assert(na==5);Flags8 flags{static_cast<std::uint8_t>(a[1]),static_cast<std::uint8_t>(a[2]),static_cast<std::uint8_t>(a[3]),{17,18,19,20,21}};
   assert((op==2?dh2_object_mark_deleted(&flags):dh2_object_set_updating(&flags,a[4]))==0);
   const auto*bytes=reinterpret_cast<const std::uint8_t*>(&flags);for(unsigned j=0;j<8;++j)out.push_back(bytes[j]);
  }else{
   assert(op==4&&na);std::vector<std::uintptr_t>values(a.begin()+1,a.end());values.push_back(0);
   DeletionQueue16 queue{values.data(),na-1,na};assert(dh2_object_queue_deletion(&queue,a[0])==0);
   for(std::uint32_t j=0;j<queue.count;++j)out.push_back(static_cast<std::uint32_t>(values[j]));
  }
  assert(out==expected);checks+=no;
 }
 assert(file.peek()==EOF);
 std::uint32_t guards=0;
 Handle16 h{-1,99,123},saved=h;assert(dh2_object_handle_from_pointer(&h,&h,5)==1&&std::memcmp(&h,&saved,sizeof(h))==0);++guards;
 alignas(Handle16) unsigned char storage[64]{};assert(dh2_object_handle_from_pointer(reinterpret_cast<Handle16*>(storage+1),nullptr,0)==1);++guards;
 assert(dh2_object_mark_deleted(nullptr)==1&&dh2_object_set_updating(nullptr,0)==1);guards+=2;
 std::uintptr_t values[]={8,0};DeletionQueue16 q{values,2,2};assert(dh2_object_queue_deletion(&q,9)==1&&q.count==2&&values[0]==8&&values[1]==0);++guards;
 assert(dh2_object_queue_deletion(&q,0)==0&&q.count==2);++guards;
 q.count=3;assert(dh2_object_queue_deletion(&q,8)==1&&q.count==3);++guards;
 q={reinterpret_cast<std::uintptr_t*>(storage+1),0,1};assert(dh2_object_queue_deletion(&q,8)==1&&q.count==0);++guards;
 q={reinterpret_cast<std::uintptr_t*>(&q),0,1};assert(dh2_object_queue_deletion(&q,8)==1&&q.count==0);++guards;
 q={nullptr,0,0};assert(dh2_object_queue_deletion(&q,0)==1&&q.count==0);++guards;
 // Native identities retain all 64 bits; source 32-bit address normalization
 // in the corpus is not a license to truncate actual ARM64 receiver pointers.
 std::uintptr_t wide[]={UINT64_C(0x1234567800000001),1,0};q={wide,2,3};assert(dh2_object_queue_deletion(&q,UINT64_C(0x1234567800000001))==0&&q.count==2);assert(dh2_object_queue_deletion(&q,UINT64_C(0x8765432100000001))==0&&q.count==3);guards+=2;
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"word_checks\":"<<checks<<",\"native_guards\":"<<guards<<",\"mismatches\":0}\n";
}
