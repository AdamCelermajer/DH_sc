#pragma once
#include "character_target_providers.hpp"
#include "canonical_object_lifecycle_v1.hpp"
#include "object_manager_language_registry_v1.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <list>
#include <set>
namespace dh2::world {
struct CanonicalObjectBorrowV1 {
 std::uintptr_t identity{};
 std::shared_ptr<void> lease;
 target_providers::Handle16* shared_handle{};
 const std::uint32_t* type_f4{};
 const std::uint8_t* across_rooms87{};
 std::int32_t* room64{};
 void* context{};
 // Actual source receiver methods/storage, not detached registry substitutes.
 bool(*set_name)(void*,const char*,std::string&){};
 bool(*set_archetype)(void*,const char*,std::string&){};
 bool(*as_character)(void*,std::uintptr_t&,std::string&){};
 const char** class_name20{}; // SAME GetNewObject source catalog-name field
 bool(*read_across_rooms87)(void*,std::uint8_t&,std::string&){};
 const std::uintptr_t* source_room2f4_v89{};
 std::uint8_t* source_no_room2f8_v89{};
 std::uint8_t* type14_localization_valid819{};
};
struct CanonicalObjectManagerServicesV1 {
 void* context{};
 bool(*local_player)(void*,std::uintptr_t&,std::string&){};
 bool(*highest_threat)(void*,const char*,std::int32_t,bool,target_providers::Handle16&,std::string&){};
 bool(*missing_name_debug)(void*,std::string&){};
 bool(*destroy_duplicate)(void*,CanonicalObjectBorrowV1&,std::string&){};
 bool(*assign_network_id)(void*,CanonicalObjectBorrowV1&,std::string&){};
 // Actual Add observer: runs after source Character/Module append prefix.
 bool(*published)(void*,std::int32_t,const CanonicalObjectBorrowV1&,std::uintptr_t,std::string&){};
};
// Source ObjectManager ctor34a404/GetObjectByName34aca0/Add34b270 projection.
// Owns map/list nodes and leases only. Actor fields remain the receiver's one
// canonical storage. Remove/Flush lifecycle is deliberately a separate service.
class CanonicalObjectManagerV1 {
 struct Entry {std::string name;CanonicalObjectBorrowV1 actor;};
 std::map<std::int32_t,Entry> entries_;
 std::vector<std::uintptr_t> characters_,modules_;
 ObjectManagerLanguageRegistryV1 language_registry_v109_;
 std::list<std::uintptr_t> pending_; //Original C1-empty source34, splice owns its nodes.
 std::list<std::uintptr_t> marked_for_deletion3c_v89_;
 std::list<std::uintptr_t> active2c_v102_,conditions44_v102_,tracked_characters70_v102_;
 std::list<std::uintptr_t> next_start90_v102_;
 std::list<std::uintptr_t> network100_v102_;
 std::map<std::int16_t,std::list<std::uintptr_t>> deferred108_v107_; //actual C1-empty tree108.
 //Actual constructor-empty STL domains, same native ownership as2c..108.
 //Payload types come from selected ObjectManager D1 helpers349b14..349e70.
 std::list<std::uintptr_t> orphan4_v108_;
 std::list<std::uint16_t> removed120_v108_;
 std::list<std::int32_t> list128_v108_,list130_v108_;
 std::map<std::int32_t,std::list<std::int32_t>> tree98_v108_;
 std::map<std::int32_t,std::int16_t> treeb0_v108_,treec8_v108_,treee0_v108_;
 std::map<std::int32_t,std::int32_t> tree148_v108_;
 std::map<std::int16_t,std::set<std::int16_t>> tree164_v108_,tree17c_v108_,tree194_v108_;
 std::uint32_t word54_v108_{},word138_v108_{},word13c_v108_{},word140_v108_{};
 std::uint8_t byte160_v108_{}; //Ctor's genuine empty Flush writes zero.
 std::uint32_t updates58_v102_{},updates5c_v102_{};
 std::uint8_t onlinefc_v102_{},onlinefd_v102_{};
 std::uint8_t network_initialized1ac_v88_{}; //actual C1 zero34a604 before Flush
 std::list<std::uintptr_t> no_room88_v89_;
 std::list<std::uintptr_t> rooms24_v104_;
 //Original ObjectManagerC1 34a1e8 initializes80/84 empty. Entries are the
 //actual Room occupants-list addresses, never snapshots of their objects.
 std::list<const std::list<std::uintptr_t>*> room_objects80_v105_;
 std::uint32_t visible_rooms_f8_v104_{};
 std::uint32_t next_key_{},count_{},frame_{},init_phase7c_{}; // explicit ctor stores +4c/+50/+78
 CanonicalObjectManagerServicesV1 services_;
 // Native ownership receipt only: invalidated at source mutation prefixes.
 // Never substitutes source initialized/count/phase fields.
 bool source_c1_fresh_v50_{};
 //Native admission receipt of a COMPLETED original Flush, separate from all
 //source counters/C1 freshness. Mutations invalidate it; no empty-list scan.
 bool source_flush_complete_v121_{};
 std::uint64_t source_flush_generation_v121_{};
 bool lifecycle_busy_v1_{},lifecycle_failed_v1_{};std::string lifecycle_failure_v1_;
 bool lifecycle_admit_v1(const CanonicalObjectLifecycleV1&,ObjectManagerNativeStorageV1&,std::string&);
 bool lifecycle_fail_v1(std::string&);
 bool required(bool,const char*,std::string&);
public:
 explicit CanonicalObjectManagerV1(CanonicalObjectManagerServicesV1 services):services_(services){
  // C1 34a404 ends with Flush3496b8. Its genuinely empty constructor
  // branch inserts the null map node0 (349944..349958), then stores1
  // into next_key4c (34995c..349960). Zero never names a live receiver.
  entries_.try_emplace(0);next_key_=1;source_c1_fresh_v50_=true;
  //Constructor's empty Flush still registers the actual
  //no-room list address into80. This is not a generated Room or list copy.
  room_objects80_v105_.push_back(&no_room88_v89_);
 }
 CanonicalObjectManagerV1(const CanonicalObjectManagerV1&)=delete;
 bool remove_source_v1(target_providers::Handle16,const CanonicalObjectLifecycleV1&,std::string&);
 bool fake_remove_source_v106(target_providers::Handle16,const CanonicalObjectLifecycleV1&,
  const std::function<bool(const CanonicalObjectBorrowV1&,std::uint8_t*&,std::string&)>&,
  const std::function<bool(const CanonicalObjectBorrowV1&,std::string&)>& character_clean,
  std::string&);
 bool flush_source_v1(const CanonicalObjectLifecycleV1&,std::string&);
 bool borrow_native_storage_v108(const std::shared_ptr<CanonicalObjectManagerV1>&,ObjectManagerNativeStorageV1&,std::string&);
 bool source_lifecycle_failed_v1()const noexcept{return lifecycle_failed_v1_;}
 bool source_rename_v114(std::uintptr_t,const char*,const std::function<bool(std::string&)>&,std::string&);
 bool by_name(const char*,std::int32_t,bool,const char*,target_providers::Handle16&,std::string&);
 bool add(CanonicalObjectBorrowV1,const char*,const char*,std::int32_t,bool,target_providers::Handle16&,std::string&);
 bool get_handle(std::int32_t,target_providers::Handle16&,std::string&);
 // Whole ObjectHandle::GetObject33fdc0 cache/map prefix over this SAME map.
 // Reached NULL assertion is supplied explicitly; native code never emulates
 // the original mode2 NULL write or invents a default assertion mode.
 bool resolve_handle_v4(target_providers::Handle16&,bool,const CanonicalObjectBorrowV1*&,
   const std::function<bool(std::string&)>& null_assertion,std::string&);
 std::uint32_t source_frame78_v4()const noexcept{return frame_;}
 const CanonicalObjectBorrowV1* object(std::int32_t)const noexcept;
 // Host adoption preflight only: observes published entries without issuing
 // source GetObjectByName(create=true) twice or allocating a map node.
 bool published_name_conflict_v4(const char*,std::int32_t,bool&,std::string&)const;
 void begin_frame(std::uint32_t v)noexcept{source_c1_fresh_v50_=false;source_flush_complete_v121_=false;frame_=v;}
 // Source C1 field7c zero store34a4f8. This SAME manager owns InitPost phase;
 // scheduler stores only modern cursors, never another phase/registry.

 class FreshSourceBorrowV50 {
  friend class CanonicalObjectManagerV1;
  std::shared_ptr<CanonicalObjectManagerV1> owner_;
  std::uint64_t flush_generation_v121_{};
 public:
  bool same_fresh_producer(const std::shared_ptr<CanonicalObjectManagerV1>& owner)const noexcept{
   return owner_&&owner&&owner_.get()==owner.get()&&!owner_.owner_before(owner)&&!owner.owner_before(owner_)&&
    owner_->source_flush_generation_v121_==flush_generation_v121_&&owner_->source_initialization_admissible_v121();
  }
 };
 bool borrow_fresh_source_v50(const std::shared_ptr<CanonicalObjectManagerV1>& owner,FreshSourceBorrowV50& out,std::string& error){
  if(!owner||owner.get()!=this||!source_initialization_admissible_v121()){error="Required actual ObjectManager C1 or completed source Flush producer";return false;}
  FreshSourceBorrowV50 next;next.owner_=owner;next.flush_generation_v121_=source_flush_generation_v121_;out=std::move(next);error.clear();return true;
 }
 bool source_initialization_admissible_v121()const noexcept{
  return !lifecycle_failed_v1_&&!lifecycle_busy_v1_&&init_phase7c_==0&&(source_c1_fresh_v50_||source_flush_complete_v121_);
 }
 bool source_flush_complete_v121()const noexcept{return source_flush_complete_v121_;}
 std::uint64_t source_flush_generation_v121()const noexcept{return source_flush_generation_v121_;}
 using SourceActorBorrowV38=CanonicalObjectBorrowV1;
 std::uint32_t& source_init_phase7c_v38()noexcept{return init_phase7c_;}
 // Original map header+1c node count includes the real reserved null key0,
 // names allocated before publication and operator[]-inserted null entries.
 std::uint32_t source_map_size1c_v38()const noexcept{return std::uint32_t(entries_.size());}
 bool source_ordered_begin_v38(std::int32_t& key,const CanonicalObjectBorrowV1*& actor)const noexcept{
  const auto i=entries_.begin();if(i==entries_.end()){actor=nullptr;return false;}
  key=i->first;actor=i->second.actor.identity?&i->second.actor:nullptr;return true;
 }
 bool source_ordered_next_v38(std::int32_t previous,std::int32_t& key,const CanonicalObjectBorrowV1*& actor)const noexcept{
  const auto i=entries_.upper_bound(previous);if(i==entries_.end()){actor=nullptr;return false;}
  key=i->first;actor=i->second.actor.identity?&i->second.actor:nullptr;return true;
 }
 bool source_ordered_entry_v38(std::int32_t key,const CanonicalObjectBorrowV1*& actor)const noexcept{
  const auto i=entries_.find(key);if(i==entries_.end()){actor=nullptr;return false;}
  actor=i->second.actor.identity?&i->second.actor:nullptr;return true;
 }
 std::uint32_t source_count50()const noexcept{return count_;}
 std::uint32_t source_next_key4c()const noexcept{return next_key_;}
 const std::vector<std::uintptr_t>& characters()const noexcept{return characters_;}
 const std::vector<std::uintptr_t>& modules()const noexcept{return modules_;}
 // Persistent projection of this manager's real publication/lifecycle lists.
 ui::SettingsLanguageScene24V1& source_language_scene_v109()noexcept{return language_registry_v109_.scene();}
 // Source Spawn accepted virtual38 appends the SAME published receiver.
 // Ordered duplicates are source-valid; this does not enable/update it.
 bool append_pending(const CanonicalObjectBorrowV1&,std::string&);
 bool source_mark_for_deletion_v89(std::uintptr_t,std::string&);
 const std::list<std::uintptr_t>& source_marked_for_deletion_v89()const noexcept{return marked_for_deletion3c_v89_;}
 bool source_add_no_room_object_v89(const CanonicalObjectBorrowV1&,std::string&);
 bool source_remove_no_room_object_v108(const CanonicalObjectBorrowV1&,std::string&);
 bool source_handle_no_room_objects_v89(const std::function<bool(const CanonicalObjectBorrowV1&,bool&,std::string&)>&,std::string&);
 const std::list<std::uintptr_t>& source_no_room_objects_v89()const noexcept{return no_room88_v89_;}
 std::list<std::uintptr_t>& source_rooms24_v104()noexcept{return rooms24_v104_;}
 std::uint32_t& source_visible_rooms_f8_v104()noexcept{return visible_rooms_f8_v104_;}
 bool source_update_rooms_v104(const std::function<bool(std::uintptr_t,std::string&)>&,std::string&);
 bool source_add_room_objects_v105(const std::list<std::uintptr_t>*,std::string&);
 bool source_del_room_objects_v105(const std::list<std::uintptr_t>*,std::string&);
 const std::list<const std::list<std::uintptr_t>*>& source_room_objects80_v105()const noexcept{return room_objects80_v105_;}
 const std::list<std::uintptr_t>& pending()const noexcept{return pending_;}
 //Same original C1 lists and counters consumed by Update/InitPost/Flush.
 //Only source publication calls append/splice; these are not a map snapshot.
 std::list<std::uintptr_t>& source_active2c_v102()noexcept{return active2c_v102_;}
 std::list<std::uintptr_t>& source_pending34_v102()noexcept{return pending_;}
 std::list<std::uintptr_t>& source_deletion3c_v102()noexcept{return marked_for_deletion3c_v89_;}
 std::list<std::uintptr_t>& source_conditions44_v102()noexcept{return conditions44_v102_;}
 std::list<std::uintptr_t>& source_characters70_v102()noexcept{return tracked_characters70_v102_;}
 std::list<std::uintptr_t>& source_next_start90_v102()noexcept{return next_start90_v102_;}
 std::list<std::uintptr_t>& source_network100_v102()noexcept{return network100_v102_;}
 std::map<std::int16_t,std::list<std::uintptr_t>>& source_deferred108_v107()noexcept{return deferred108_v107_;}
 bool source_is_online_deferred_v107(std::uintptr_t,const std::vector<std::int32_t>&);
 std::uint32_t& source_updates58_v102()noexcept{return updates58_v102_;}
 std::uint32_t& source_updates5c_v102()noexcept{return updates5c_v102_;}
 std::uint8_t& source_onlinefc_v102()noexcept{return onlinefc_v102_;}
 std::uint8_t& source_onlinefd_v102()noexcept{return onlinefd_v102_;}
 std::uint8_t& source_network_initialized1ac_v88()noexcept{return network_initialized1ac_v88_;}
 std::list<std::uintptr_t>& source_orphan4_v108()noexcept{return orphan4_v108_;}
 std::list<std::uint16_t>& source_removed120_v108()noexcept{return removed120_v108_;}
 std::list<std::int32_t>& source_list128_v108()noexcept{return list128_v108_;}
 std::list<std::int32_t>& source_list130_v108()noexcept{return list130_v108_;}
 auto& source_tree98_v108()noexcept{return tree98_v108_;}
 auto& source_treeb0_v108()noexcept{return treeb0_v108_;}
 auto& source_treec8_v108()noexcept{return treec8_v108_;}
 auto& source_treee0_v108()noexcept{return treee0_v108_;}
 auto& source_tree148_v108()noexcept{return tree148_v108_;}
 auto& source_tree164_v108()noexcept{return tree164_v108_;}
 auto& source_tree17c_v108()noexcept{return tree17c_v108_;}
 auto& source_tree194_v108()noexcept{return tree194_v108_;}
};
}
