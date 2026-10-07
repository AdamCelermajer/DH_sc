#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
namespace model_renderer {
enum class SourceCampaignDeliveryKindV104:std::uint8_t {update,draw,input};
// Host delivery bookkeeping only. Source World, Level and rendering fields
// are never mutated or freed by closing this gate.
class SourceCampaignAdmissionV104 final {
 std::mutex mutex_;
 std::thread::id thread_{std::this_thread::get_id()};
 std::array<std::uint32_t,3> active_{};
 bool closed_{};
public:
 bool enter(SourceCampaignDeliveryKindV104 kind,bool& blocked,std::string& e){
  std::lock_guard<std::mutex> lock(mutex_);blocked=false;
  if(thread_!=std::this_thread::get_id()){e="Source campaign delivery requires its actual GL owner thread";return false;}
  if(closed_){blocked=true;e.clear();return true;}
  auto& count=active_[static_cast<unsigned>(kind)];
  if(count==UINT32_MAX){e="Source delivery nesting exhausted";return false;}
  ++count;e.clear();return true;
 }
 void leave(SourceCampaignDeliveryKindV104 kind)noexcept{
  std::lock_guard<std::mutex> lock(mutex_);--active_[static_cast<unsigned>(kind)];
 }
 bool close(bool& pending,std::string& e){
  std::lock_guard<std::mutex> lock(mutex_);
  if(thread_!=std::this_thread::get_id()){e="Source retirement requires the actual GL owner thread";return false;}
  closed_=true;pending=active_[0]||active_[1]||active_[2];e.clear();return true;
 }
 bool rebind_after_context_loss(std::string& e){
  std::lock_guard<std::mutex> lock(mutex_);
  if(active_[0]||active_[1]||active_[2]){e="GL context handoff overlaps an admitted source operation";return false;}
  thread_=std::this_thread::get_id();e.clear();return true;
 }
 bool require_class_cleanup_v106(std::string& e){
  std::lock_guard<std::mutex> lock(mutex_);
  if(thread_!=std::this_thread::get_id()||active_[1]){e="Class resource cleanup requires actual GL owner with no active draw";return false;}
  e.clear();return true;
 }
 bool require_closed_quiescent(std::string& e){
  std::lock_guard<std::mutex> lock(mutex_);
  if(thread_!=std::this_thread::get_id()||!closed_||active_[0]||active_[1]||active_[2]){
   e="Source resource retirement requires closed admission and completed GL deliveries";return false;
  }
  e.clear();return true;
 }
};
class SourceCampaignDeliveryV104 final {
 std::shared_ptr<SourceCampaignAdmissionV104> gate_;
 SourceCampaignDeliveryKindV104 kind_{};
public:
 SourceCampaignDeliveryV104()=default;
 SourceCampaignDeliveryV104(const SourceCampaignDeliveryV104&)=delete;
 SourceCampaignDeliveryV104& operator=(const SourceCampaignDeliveryV104&)=delete;
 ~SourceCampaignDeliveryV104(){if(gate_)gate_->leave(kind_);}
 bool acquire(std::shared_ptr<SourceCampaignAdmissionV104> gate,SourceCampaignDeliveryKindV104 kind,
  bool& blocked,std::string& e){
  if(gate_||!gate){e="Required actual new source delivery scope";return false;}
  if(!gate->enter(kind,blocked,e))return false;
  if(!blocked){gate_=std::move(gate);kind_=kind;}return true;
 }
};
bool borrow_source_campaign_admission_v104(const std::shared_ptr<void>&,
 std::shared_ptr<SourceCampaignAdmissionV104>&,std::string&);
bool borrow_source_campaign_world_owner_v104(std::shared_ptr<void>&,std::string&);
bool rebind_source_campaign_admission_after_context_loss_v104(std::string&);
bool close_source_campaign_admission_v104(const std::shared_ptr<void>&,bool& pending,std::string&);
bool require_source_campaign_quiescence_v104(const std::shared_ptr<void>&,std::string&);
}
