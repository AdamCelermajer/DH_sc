#pragma once
#include "canonical_item_factory_v2.hpp"
#include "world_item_graph_v3.hpp"
#include "world_item_pf_connection_v4.hpp"
#include "world_item_scene_services_v3.hpp"
#include "world_loot_item_runtime_v1.hpp"
namespace dh2::character {
struct WorldItemLiveServicesV5 {
 std::shared_ptr<void> world;
 std::shared_ptr<world::GameObjectSceneRootRegistryV1> roots;
 world::CanonicalItemFactoryServicesV2 factory;
 LootDropAwardServicesV8 drop;
 LootInteractServicesV8 interaction;
 void* context{};
 // Supplies actual cache, conditions, device, physical peer, audio and color
 // receivers. The owner installs SAME Scene/PF plumbing, not fake callbacks.
 bool(*graph_services)(void*,RetainedWorldItemObjectV1&,WorldItemGraphServicesV3&,std::string&){};
 bool(*outer_item)(void*,const WorldItemRequestV1&,std::int32_t&,std::string&){};
};
// One construction/dispatch/pool/scene/body composition on an EXISTING World.
// Its maps only retain canonical receiver leases; manager owns published keys.
class WorldItemLiveOwnerV5 {
 struct Record {
  std::shared_ptr<RetainedWorldItemObjectV1> item;
  std::unique_ptr<WorldItemPfConnectionV4> pf;
  std::unique_ptr<WorldItemGraphV3> graph;
 };
 WorldItemLiveServicesV5 services_;
 physical::NativeWorld& physics_;
 const navigation::CollisionWorld* geometry_{};
 navigation::ObstacleRegistry* obstacles_{};
 std::map<std::uintptr_t,Record> records_;
 world::CanonicalItemFactoryV2 factory_;
 WorldLootItemRuntimeV1 pool_;
 bool detached_{};
 world::CanonicalItemFactoryServicesV2 factory_services();
 WorldLootFactoryServicesV1 pool_services();
 bool ensure(std::uintptr_t,std::string&);
 static bool operation(void*,const WorldItemRequestV1&,std::int32_t&,std::string&);
 static bool present(void*,std::uintptr_t,bool&,std::string&);
 static bool sound_position(void*,std::uintptr_t,const float*&,std::string&);
 static bool full_notifications(void*,data::LootTemporaryInventoryV8&,data::ItemInstanceV1&,std::string&);
 static bool spawn(void*,const char*,const char*,bool,bool,std::shared_ptr<RetainedWorldItemObjectV1>&,std::string&);
 static bool interaction(void*,const LootInteractRequestV8&,LootInteractResponseV8&,std::string&);
public:
 WorldItemLiveOwnerV5(world::CanonicalObjectManagerV1&,world::CanonicalPropertyMapV1&,
  physical::NativeWorld&,const navigation::CollisionWorld*,navigation::ObstacleRegistry*,
  data::LootTablesV2::Borrow,data::LootAudioVisualV8::Borrow,WorldItemLiveServicesV5);
 WorldItemLiveOwnerV5(const WorldItemLiveOwnerV5&)=delete;
 bool precache(std::string&);
 bool init_final_v23(std::uintptr_t,bool& source_eligible,std::string&);
 bool update_pf_v23(std::uintptr_t,std::string&);
 WorldLootItemRuntimeV1& pool()noexcept{return pool_;}
 world::CanonicalItemFactoryV2& factory()noexcept{return factory_;}
 WorldItemGraphV3* graph(std::uintptr_t)const noexcept;
 std::size_t retained_count()const noexcept{return records_.size();}
 bool interact(std::uintptr_t item,std::uintptr_t character,std::string&);
 bool update(std::uintptr_t item,std::uint32_t dt_ms,std::uintptr_t actual_tooltip_ooi,std::string&);
 bool sample_visuals(std::uint32_t absolute_ms,std::string&);
 // BEFORE floor graph or PhysicsWorld replacement. Items/scene/inventory stay
 // pinned; body teardown must precede the PhysicsWorld destructor.
 bool detach_physics(std::string&);
 void rebind(const navigation::CollisionWorld*,navigation::ObstacleRegistry*)noexcept;
 // Actual canonical deleting destructor invokes this AFTER manager unpublish.
 // Source ItemManager Flush must precede removal of its borrowed145 entries.
 bool erased(std::uintptr_t,std::string&);
};
}
