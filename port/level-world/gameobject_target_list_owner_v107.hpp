#pragma once
#include <cstdint>
#include <deque>
#include <map>
#include <string>
#include <vector>
namespace dh2::world {
// Native storage successor of embedded GameObject.TargetList304. Original
//38c314 constructs(NULL,1,0,1);38c380 then SetRefObject(this), whose base
//IsCharacter virtual24 returns NULL. Heap deque/tree share this one owner.
struct GenericTargetInfoV107 {std::uintptr_t object{};float distance{},angle{},weight{};std::uint32_t flags{};};
class GameObjectTargetListOwnerV107 final {
 std::deque<GenericTargetInfoV107> heap_;
 std::map<std::string,std::vector<GenericTargetInfoV107>> backups_;
 std::uintptr_t ref2c_{};
 // Retained source constructor cells; this lifetime owner does not yet query them.
 [[maybe_unused]] std::uintptr_t character30_{};
 [[maybe_unused]] std::int32_t count34_{1},type38_{},sorter28_{1};
 bool destroyed_{};
public:
 explicit GameObjectTargetListOwnerV107(std::uintptr_t actual_gameobject):ref2c_(actual_gameobject){}
 bool destroy(std::string& e){
  if(destroyed_){e.clear();return true;}
  //38d18c: backup RB tree first (BackupObjectList D1 owns its vector/name),
  //then trivial TargetInfo deque elements and the deque block/map storage.
  backups_.clear();std::deque<GenericTargetInfoV107>{}.swap(heap_);destroyed_=true;e.clear();return true;
 }
 bool destroyed()const noexcept{return destroyed_;}
 bool flush_backup_results_v111(std::string& e){
  if(destroyed_){e="TargetList.FlushBackupResults after source D1";return false;}
  //4a1a38 walks retained tree nodes, invoking BackupObjectList._Clear.
  //Each vector is cleared; the map keys/nodes and vector capacity survive.
  for(auto& entry:backups_)entry.second.clear();e.clear();return true;
 }
 const std::uintptr_t& reference()const noexcept{return ref2c_;}
};
}
