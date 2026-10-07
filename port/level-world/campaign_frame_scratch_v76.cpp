#include "campaign_frame_scratch_v76.hpp"
#include <algorithm>
#include <exception>
namespace dh2::navigation {
CampaignFrameScratchV76::CampaignFrameScratchV76(){dh2_nav_motion_policy_defaults(&motion_);}
bool CampaignFrameScratchV76::borrow(std::uint32_t segments,std::uint32_t actors,
 std::uint32_t floors,std::shared_ptr<void>& pin,ControllerWorkspace*& workspace,std::string& e){
 pin.reset();workspace=nullptr;
 // Explicit native allocation domain, independent of source logical counts.
 if(segments>262144||actors>65536||floors>65536){e="Campaign frame scratch exceeds native allocation domain";return false;}
 std::shared_ptr<Entry> entry;
 {
  std::lock_guard<std::mutex> lock(mutex_);
  for(auto& candidate:entries_){bool unused=false;
   if(candidate->busy.compare_exchange_strong(unused,true,std::memory_order_acq_rel)){entry=candidate;break;}
  }
  if(!entry){
   if(entries_.size()>=64){e="Campaign frame scratch nesting exceeds native allocation domain";return false;}
   try{entry=std::make_shared<Entry>();entries_.push_back(entry);entry->busy.store(true,std::memory_order_release);}
   catch(const std::exception& failure){e=failure.what();return false;}
  }
 }
 std::shared_ptr<Lease> lease;
 try{
  lease=std::make_shared<Lease>(entry);
  if(entry->segments.size()<segments)entry->segments.resize(segments);
  if(entry->actors.size()<actors)entry->actors.resize(actors);
  if(entry->floors.size()<floors)entry->floors.resize(floors);
  entry->workspace={entry->segments.data(),static_cast<std::uint32_t>(entry->segments.size()),0,
   entry->actors.data(),static_cast<std::uint32_t>(entry->actors.size()),0,
   entry->floors.data(),static_cast<std::uint32_t>(entry->floors.size()),0};
  workspace=&entry->workspace;pin=std::move(lease);e.clear();return true;
 }catch(const std::exception& failure){
  if(lease)lease.reset();else entry->busy.store(false,std::memory_order_release);
  e=failure.what();return false;
 }
}
}
