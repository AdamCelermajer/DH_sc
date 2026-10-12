#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
namespace dh2::application {
// Sole process CXPlayerManager score facet. Full C1 52df54 stores these cells
// zero; unwritten transport/player resources are not fabricated here.
class SourceCXPlayerScoreV114 {
 std::uint8_t byte4_{},byte5_{};
 std::uintptr_t player8_{};
 std::int32_t state14_{},score18_{},addition1c_{};
 std::uintptr_t field7c_{};
 std::shared_ptr<void> player_pin_;
 const std::uint8_t* player_ready40_{};
 std::shared_ptr<void> init_owner_;
 std::function<bool(SourceCXPlayerScoreV114&,std::string&)> actual_init_;
public:
 std::uint8_t* source_byte5()noexcept{return &byte5_;}
 std::int32_t state14()const noexcept{return state14_;}
 std::int32_t score18()const noexcept{return score18_;}
 // Genuine original Init52e524 supplies its SAME protocol/player allocation.
 // This is not a fake logged-in predicate or a second player controller.
 bool bind_init(std::shared_ptr<void> owner,std::function<bool(SourceCXPlayerScoreV114&,std::string&)> callback,std::string& e){
  if(!owner||!callback||init_owner_){e="Require once-only actual CXPlayerManager.Init provider";return false;}init_owner_=std::move(owner);actual_init_=std::move(callback);return true;
 }
 bool publish_player(std::shared_ptr<void> pin,std::uintptr_t identity,const std::uint8_t* ready,std::string& e){
  if(!pin||!identity||!ready||player8_){e="Require original CX player8 allocation/byte40";return false;}player_pin_=std::move(pin);player8_=identity;player_ready40_=ready;return true;
 }
 void source_initialized_store(std::uint8_t value)noexcept{byte4_=value;}
 void source_addition_store(std::int32_t value)noexcept{addition1c_=value;}
 // Entire UploadMyScore52e6cc control and SwitchState52e0d0 field store.
 bool upload(std::int32_t requested,bool& uploaded,std::string& e){
  uploaded=false;
  if(requested<=0||score18_>requested){state14_=16;e.clear();return true;}
  score18_=requested; // source store precedes player NULL/readiness branches
  if(!player8_){
   if(!actual_init_){e="Required positive original CXPlayerManager.Init52e524";return false;}
   if(!actual_init_(*this,e))return false;
   e.clear();return true; // original returns captured NULL player register0
  }
  if(!player_pin_||!player_ready40_){e="Actual CX player8 lease/byte40 absent";return false;}
  if(!*player_ready40_){e.clear();return true;}
  if(addition1c_>0){score18_=static_cast<std::int32_t>(std::uint32_t(score18_)+std::uint32_t(addition1c_));addition1c_=0;}
  state14_=10;uploaded=true;e.clear();return true;
 }
};
}
