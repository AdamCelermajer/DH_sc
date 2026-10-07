#pragma once
#include "character_target_providers.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <functional>
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
 std::vector<std::uintptr_t> pending_; // Original ctor-empty ObjectManager+34 list.
 std::uint32_t next_key_{},count_{},frame_{}; // explicit ctor stores +4c/+50/+78
 std::uint32_t update_count58_{}; // SAME ObjectManager C2 store34a4c8, GameObject.Update38cc68.
 CanonicalObjectManagerServicesV1 services_;
 bool required(bool,const char*,std::string&);
public:
 explicit CanonicalObjectManagerV1(CanonicalObjectManagerServicesV1 services):services_(services){
  // C1 34a404 ends with Flush3496b8. Its genuinely empty constructor
  // branch inserts the null map node0 (349944..349958), then stores1
  // into next_key4c (34995c..349960). Zero never names a live receiver.
  entries_.try_emplace(0);next_key_=1;
 }
 CanonicalObjectManagerV1(const CanonicalObjectManagerV1&)=delete;
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
 void begin_frame(std::uint32_t v)noexcept{frame_=v;}
 std::uint32_t source_count50()const noexcept{return count_;}
 std::uint32_t source_next_key4c()const noexcept{return next_key_;}
 std::uint32_t& source_update_count58_v23()noexcept{return update_count58_;}
 const std::uint32_t& source_update_count58_v23()const noexcept{return update_count58_;}
 const std::vector<std::uintptr_t>& characters()const noexcept{return characters_;}
 const std::vector<std::uintptr_t>& modules()const noexcept{return modules_;}
 // Source Spawn accepted virtual38 appends the SAME published receiver.
 // Ordered duplicates are source-valid; this does not enable/update it.
 bool append_pending(const CanonicalObjectBorrowV1&,std::string&);
 const std::vector<std::uintptr_t>& pending()const noexcept{return pending_;}
};
}
