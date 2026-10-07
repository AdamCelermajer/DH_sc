#include "source_character_aggro_v84.hpp"
#include <algorithm>
#include <limits>
namespace dh2::character {
bool SourceCharacterAggroV84::reserve(data::AggroTable& table,std::vector<data::AggroEntry>& storage,
 std::uint64_t key,std::string& e){
 if(!source_c1_||table.count>table.capacity||table.capacity!=storage.size()||table.entries!=storage.data()){
  // Empty vectors may use a native nonnull data pointer; initial source C1
  // does not publish an allocation pointer into the semantic native view.
  if(!source_c1_||table.count||table.capacity||!storage.empty()){e="Malformed SAME source CharAI Aggro storage";return false;}
 }
 for(std::uint32_t i=0;i<table.count;++i)if(table.entries[i].character==key){e.clear();return true;}
 if(table.count<table.capacity){e.clear();return true;}
 if(table.capacity==std::numeric_limits<std::uint32_t>::max()){e="Native Aggro source storage exhausted";return false;}
 const auto next=std::min<std::uint64_t>(std::numeric_limits<std::uint32_t>::max(),std::max<std::uint64_t>(8,std::uint64_t(table.capacity)*2));
 storage.resize(static_cast<std::size_t>(next));table.entries=storage.data();table.capacity=static_cast<std::uint32_t>(storage.size());e.clear();return true;
}
bool SourceCharacterAggroV84::prepare_outgoing_insert(std::uint64_t key,std::string& e){return reserve(outgoing7c_,outgoing_,key,e);}
bool SourceCharacterAggroV84::prepare_incoming_insert(std::uint64_t key,std::string& e){return reserve(incoming94_,incoming_,key,e);}
}
