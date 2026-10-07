#pragma once
#include "character_loot_live_v22.hpp"
#include "character_loot_creation_bindings_v22.hpp"
#include "world_loot_pickup_v23.hpp"
#include "world_item_frame_v5.hpp"
#include "gameobject_update_prefix_v23.hpp"
#include "light_set_name_owner_v3.hpp"
#include "world_item_physical_contact_v4.hpp"
namespace dh2::character {
struct LootCurrentLevelV23 {
 std::uintptr_t identity{};
 const std::int32_t* difficulty118{};
 const std::uint32_t* phase130{}; // source word, never a native pointer
 std::shared_ptr<void> receiver_lease;
};
struct WorldLootGameplayServicesV23 {
 std::shared_ptr<void> application_lease;
 void* context{};
 player::PlayerEquipmentRenderOwnerV1* gear{};
 //Actual immutable Arrays/Text/RNG source producer. Source ItemManager C1
 //precedes XML and selected Gear; the old Gear borrow remains for legacy
 //callers only. This supplies the SAME tables later shared by real Gear.
 bool(*factory_source_v88)(void*,data::LootTablesV2::Borrow&,data::ItemPowerTablesV5::Borrow&,
  data::ItemTextServicesV5&,data::LootRandom8V2*&,std::string&){};
 DebugSwitches* debug{};DebugFileServices24 debug_files{};
 std::shared_ptr<data::ItemPresentationOwnerV5> presentation;
 std::shared_ptr<world::LightSetNameOwnerV3> light_names;
 CharacterLootLiveServicesV22 loot;
 CharacterLootCreationBackendV22 creation;
 WorldItemLiveServicesV5 items;
 WorldLootPickupServicesV23 pickup;
 world::GameObjectUpdatePrefixServicesV23 update_prefix;
 bool(*current_level)(void*,LootCurrentLevelV23&,std::string&){};
 // SAME geometry/path/body/camera/auxiliary policies and source service facts.
 bool(*frame_services)(void*,WorldItemGraphV3&,WorldItemFrameServicesV5&,std::string&){};
 // Source Item.Update tooltip branch reads actual current Character OOI.
 // NULL tooltip skips this callback exactly as the original does.
 bool(*tooltip_ooi)(void*,std::uintptr_t actual_character,std::uintptr_t&,std::string&){};
 // Actual remaining source sound operations after disabled/current-Level.
 sound::VoxPlay3DServicesV2 sound;
};
struct LootDrawSceneV23 {
 std::uintptr_t object{};
 std::shared_ptr<world::RetainedGameObjectVisualV1> visual;
 // Pins BRES bytes, actual node visibility, same animated matrices/materials.
 // Submission must use those resources, never a billboard/icon replacement.
};
// One source gameplay composition on the existing World. It owns no Level,
// player/save/property/physics registry/RNG authority and uses ONE145 pool.
class WorldLootGameplayV23 {
 skills::CharacterWorldRuntimeV1& world_;player::PlayerManagerOwnerV1& players_;
 world::CanonicalObjectManagerV1& manager_;world::CanonicalPropertyMapV1& properties_;
 physical::NativeWorld& physics_;const navigation::CollisionWorld* geometry_;
 navigation::ObstacleRegistry* obstacles_;data::LootPowerResourcesV7::Borrow powers_;
 data::LootAudioVisualV8::Borrow audiovisual_;WorldLootGameplayServicesV23 services_;
 std::unique_ptr<CharacterLootCreationBindingsV22> creation_;
 std::unique_ptr<CharacterLootLiveV22> loot_;
 std::unique_ptr<WorldLootPickupV23> pickup_;
 std::unique_ptr<WorldItemLiveOwnerV5> items_;
 std::map<std::uintptr_t,std::unique_ptr<WorldItemFrameV5>> frames_;
 std::vector<std::uintptr_t> item_ids_; // delivery index ONLY, not a registry/pool.
 struct Destruction {WorldLootGameplayV23* self;void* context{};bool(*before)(void*,data::ItemInstanceV1&,std::string&){};};
 std::map<std::uintptr_t,Destruction> destruction_;
 sound::VoxPlay3DOwnerV2 vox_;
 std::uintptr_t vox_identity_{};
 std::uint32_t absolute_ms_{};bool attempted_{},factory_prepared_{},precache_attempted_{},ready_{},frame_failed_{},final_attempted_{};
 std::string error_;
 static bool query(void*,const data::LootCreationQueryV8&,data::LootCreationResponseV8&,std::string&);
 static bool notifications(void*,data::LootTemporaryInventoryV8&,data::ItemInstanceV1&,std::string&);
 static bool graph(void*,RetainedWorldItemObjectV1&,WorldItemGraphServicesV3&,std::string&);
 static bool interaction(void*,const LootInteractRequestV8&,LootInteractResponseV8&,std::string&);
 static bool outer(void*,const WorldItemRequestV1&,std::int32_t&,std::string&);
 static int sound(void*,const sound::VoxPlay3DRequestV2&,sound::VoxPlay3DResponseV2&);
 bool current(LootCurrentLevelV23&,std::string&);
public:
 WorldLootGameplayV23(skills::CharacterWorldRuntimeV1&,player::PlayerManagerOwnerV1&,
  world::CanonicalObjectManagerV1&,world::CanonicalPropertyMapV1&,physical::NativeWorld&,
  const navigation::CollisionWorld*,navigation::ObstacleRegistry*,data::LootPowerResourcesV7::Borrow,
  data::LootAudioVisualV8::Borrow,WorldLootGameplayServicesV23);
 WorldLootGameplayV23(const WorldLootGameplayV23&)=delete;
 // Prepare the same factory before authored Item construction; stage29
 // performs PreCache on that same pool. Neither preparation nor borrowing
 // executes inherited InitPost/InitFinal or creates another item owner.
 bool prepare_source_factory_v88(std::string&);
 bool precache_source_pool_v88(std::string&);
 bool factory_prepared_v88()const noexcept{return factory_prepared_;}
 // Existing combined entry keeps prepare+precache ordering for older callers.
 bool initialize(std::string&);
 // Called at the actual source pending-object InitFinal boundary; separate
 // from construction/PreCache, no replay on retained resource restoration.
 bool initialize_final(std::string&);
 bool route(KillActor56&,const KillRequest56&,KillResponse16&,bool&,std::string&);
 bool update(std::uint32_t actual_absolute_ms,std::uint32_t actual_dt_ms,std::string&);
 //ONE actual ObjectManager virtual2c receiver visit. Separate draw capture
 //occurs after the visitor; this never updates all145 for a single Item.
 bool update_item_source_v104(std::uintptr_t,std::uint32_t absolute_ms,std::uint32_t dt,std::string&);
 bool draw_scenes(std::vector<LootDrawSceneV23>&,std::string&);
 //All actual Item roots, including currently invisible pooled entries. The
 //source cached scene list reads their real visibility rather than dropping
 //and recreating render resources whenever an Item spawns/despawns.
 bool source_visuals_v104(std::vector<std::shared_ptr<world::RetainedGameObjectVisualV1>>&,std::string&);
 // Mixed PhysicalWorld dispatch: recognizes only this owner's exact retained
 // POItem receiver addresses; unknown peers remain for actual actor service.
 bool borrow_contact(void* actual_peer,navigation::PhysicalContact&,std::uintptr_t& owner,bool& handled,std::string&);
 bool interact(std::uintptr_t actual_item,std::uintptr_t actual_character,std::string&);
 bool detach_physics(std::string&);
 // Actual ItemManager Flush precedes canonical receiver removal. Releases
 // source graphs while SAME Scene/PhysicalWorld still exist; no hidden retry.
 bool release(std::string&);
 void rebind(const navigation::CollisionWorld*,navigation::ObstacleRegistry*,std::string&);
 // Called by the existing inventory storage observer BEFORE real free/merge.
 bool forget_presentation(data::ItemInstanceV1&,std::string&)noexcept;
 WorldItemLiveOwnerV5* items()noexcept{return items_.get();}
 CharacterLootLiveV22* loot()noexcept{return loot_.get();}
 bool borrow_creation_services_v114(WorldItemLootCreationServicesV10& out,std::string& e){if(!factory_prepared_||!creation_){e="Actual source Item factory/creation owner not prepared";return false;}out=creation_->services();e.clear();return true;}
 bool ready()const noexcept{return ready_;}
 const std::string& error()const noexcept{return error_;}
};
}
