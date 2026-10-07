#pragma once
#include "../game-data/aggro.hpp"
#include <vector>
#include <string>
namespace dh2::character {
// Sole native storage for fresh CharAI's outgoing tree7c/count8c and incoming
// tree94/counta4. CharAI C1 explicitly constructs both empty3cee00..3cee3c.
// Slot capacity is native allocation policy; never a source relation count.
class SourceCharacterAggroV84 final {
 std::vector<data::AggroEntry> outgoing_,incoming_;
 data::AggroTable outgoing7c_{},incoming94_{};
 bool source_c1_{};
 bool reserve(data::AggroTable&,std::vector<data::AggroEntry>&,std::uint64_t,std::string&);
public:
 SourceCharacterAggroV84()=delete;
 explicit SourceCharacterAggroV84(bool actual_fresh_c1):source_c1_(actual_fresh_c1){}
 SourceCharacterAggroV84(const SourceCharacterAggroV84&)=delete;
 data::AggroTable* outgoing()noexcept{return source_c1_?&outgoing7c_:nullptr;}
 data::AggroTable* incoming()noexcept{return source_c1_?&incoming94_:nullptr;}
 // Called at actual Set/AddAggro insertion before the existing source kernel;
 // resizing itself inserts nothing and preserves every key/threat bit/count.
 bool prepare_outgoing_insert(std::uint64_t,std::string&);
 bool prepare_incoming_insert(std::uint64_t,std::string&);
 //Static CharAI.ClearAllAggro walks every actual receiver and destroys both
 //trees directly; unlike AI_ClearAllAggro it emits no relation callbacks.
 bool source_clear_storage_v108(std::string& e){
  if(!source_c1_){e="Aggro Flush requires original CharAI tree C1";return false;}
  std::vector<data::AggroEntry>{}.swap(outgoing_);outgoing7c_={};
  std::vector<data::AggroEntry>{}.swap(incoming_);incoming94_={};e.clear();return true;
 }
};
}
