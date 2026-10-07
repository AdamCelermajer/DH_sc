#pragma once
#include "canonical_spawn_owner_v1.hpp"
#include "world_item_object_owner_v1.hpp"
#include "../game-data/item_pickup_type_v2.hpp"
namespace dh2::world {
struct CanonicalItemFactoryServicesV2 {
 void* context{};
 // Exact shared Handle resolver and whole ObjectBase.TestEnableCondition.
 // Item InitPost does not initialize ConditionData; constructor NULL is real.
 bool(*resolve)(void*,target_providers::Handle16&,bool,const CanonicalObjectBorrowV1*&,std::string&){};
 bool(*test_enable_condition)(void*,const CanonicalObjectBorrowV1&,bool,std::string&){};
 bool(*unknown_type_debug)(void*,const char*,std::string&){};
 character::WorldItemServicesV1 item;
};
// Production Item factory receiver and Spawn composition. The manager is the
// loader/World's sole registry; this map retains its actual virtual dispatch,
// not independent IDs. Actual audiovisual/body/condition services are borrowed.
class CanonicalItemFactoryV2 {
 CanonicalObjectManagerV1& manager_;CanonicalPropertyMapV1& map_;
 std::shared_ptr<void> world_;data::LootTablesV2::Borrow tables_;
 data::LootAudioVisualV8::Borrow audiovisual_;CanonicalItemFactoryServicesV2 services_;
 struct Record {std::shared_ptr<character::RetainedWorldItemObjectV1> item;CanonicalClassReceiverV1 dispatch;};
 std::map<std::uintptr_t,Record> records_;
 static bool construct(void*,const CanonicalFactoryEntryV1&,CanonicalClassReceiverV1&,std::string&);
 static bool resolve(void*,target_providers::Handle16&,bool,const CanonicalObjectBorrowV1*&,std::string&);
 static bool condition(void*,const CanonicalObjectBorrowV1&,bool,std::string&);
 static bool debug(void*,const char*,std::string&);
 static bool accepted(void*,const CanonicalObjectBorrowV1&,bool&,std::string&);
 static bool pending(void*,const CanonicalObjectBorrowV1&,std::string&);
 static bool dispatch(void*,const CanonicalObjectBorrowV1&,const CanonicalClassReceiverV1*&,std::string&);
public:
 CanonicalItemFactoryV2(CanonicalObjectManagerV1&,CanonicalPropertyMapV1&,std::shared_ptr<void>,data::LootTablesV2::Borrow,data::LootAudioVisualV8::Borrow,CanonicalItemFactoryServicesV2);
 bool spawn(const char* type,const char* name,bool deferred,bool network,std::shared_ptr<character::RetainedWorldItemObjectV1>&,std::string&);
 bool construct_receiver(const CanonicalFactoryEntryV1&,CanonicalClassReceiverV1&,std::string&);
 // Borrow existing references only, for fresh candidate transport identity
 // checks. These add no registry, owner, pool or publication authority.
 const CanonicalObjectManagerV1& source_manager_v57()const noexcept{return manager_;}
 const CanonicalPropertyMapV1& source_property_map_v57()const noexcept{return map_;}
 // Called only by SAME manager's genuine deleting destructor after unpublish
 // (or duplicate Add discarding the newly constructed receiver).
 void erased(std::uintptr_t identity){records_.erase(identity);}
 std::shared_ptr<character::RetainedWorldItemObjectV1> find(std::uintptr_t)const noexcept;
 static bool pickup_override58(void*,data::ItemInstanceV1& item,const std::int16_t*& out,std::string&)noexcept{out=&data::item_pickup_override58_v2(item);return true;}
};
}
