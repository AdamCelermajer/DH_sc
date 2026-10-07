#include "canonical_object_manager_v1.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::world {
bool CanonicalObjectManagerV1::source_is_online_deferred_v107(std::uintptr_t actor,
 const std::vector<std::int32_t>& actual_members){
 // Whole347dec after real Matching.GetMemberIdList. Source tree key is
 // signed16 although the temporary vector contains signed32 IDs.
 for(const auto member:actual_members){
  const auto bits=static_cast<std::uint16_t>(member);std::int16_t key;std::memcpy(&key,&bits,2);
  auto& list=deferred108_v107_[key]; //source operator[] insertion is visible.
  if(std::find(list.begin(),list.end(),actor)!=list.end())return true;
 }
 return false;
}
}
