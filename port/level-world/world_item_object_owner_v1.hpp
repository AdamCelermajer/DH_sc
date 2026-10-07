#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include "character_loot_interact_v8.hpp"
namespace dh2::character {
struct LootInventorySourceV9;
enum class WorldItemOperationV1 { game_init_post, apply_mesh_box, visual_item_material,
 drop_sound, create_decor_physical, set_physical, enable, remove_all,
 set_position, set_destination, game_update, is_at_destination, stop,
 tooltip_visible, tooltip_position, hide_tooltip, tooltip_update,
 collision_interact_type, is_local_player, is_moving, show_tooltip };
struct WorldItemRequestV1 {
 WorldItemOperationV1 operation{};std::uintptr_t object{},character{};
 data::LootTemporaryInventoryV8* inventory{};data::ItemInstanceV1* item{};
 const float* position{};std::int32_t integer{};bool flag{};
 // create_decor_physical is the exact PhysicalObjectC2 then PODecorItem
 // factory: false,true,true,false,-3,0x40,4,0. It must publish its real body.
};
struct WorldItemServicesV1 {
 void* context{};
 bool(*invoke)(void*,const WorldItemRequestV1&,std::int32_t&,std::string&){};
 data::LootEntryServicesV8 inventory_debug;
 bool(*full_notifications)(void*,data::LootTemporaryInventoryV8&,data::ItemInstanceV1&,std::string&){};
 // InitAgain Play3D3 uses actual cached position1a8, not position160.
 bool(*sound_position1a8)(void*,std::uintptr_t,const float*&,std::string&){};
 bool(*visual_present)(void*,std::uintptr_t,bool&,std::string&){};
};
// One ItemObject graph. Base fields/Handle and PF/pose are the canonical
// receiver's authority; its embedded NULL-character inventory owns transfers.
class RetainedWorldItemObjectV1 {
 actor::RuntimeState runtime_{};
 world::CanonicalGameObjectBaseOwnerV1 base_;
 LootItemFieldsV8 fields_;
 data::LootTemporaryInventoryV8 inventory_;
 data::LootAudioVisualV8::Borrow audiovisual_;
 WorldItemServicesV1 services_;
 std::unique_ptr<CharacterLootInteractV8> interact_;
 bool running_{},failed_{};
 bool source_update_interact_v11_{}; // host bounded callback scope, not a source life/FSM field
 bool call(WorldItemOperationV1,std::string&,std::uintptr_t=0,const float* =nullptr,std::int32_t=0,bool=false,data::ItemInstanceV1* =nullptr,std::int32_t* =nullptr);
 static bool add_destination(void*,std::unique_ptr<data::ItemInstanceV1>&,bool,bool,std::int32_t&,std::string&);
public:
 RetainedWorldItemObjectV1(std::uintptr_t,std::shared_ptr<void>,data::LootTablesV2::Borrow,data::LootAudioVisualV8::Borrow,WorldItemServicesV1);
 RetainedWorldItemObjectV1(const RetainedWorldItemObjectV1&)=delete;
 world::CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 world::CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> lease){return base_.canonical(std::move(lease));}
 world::CanonicalPropertyActorV1 properties()noexcept{return base_.properties();}
 actor::RuntimeState& runtime()noexcept{return runtime_;}
 // V8 compatibility enabled85/base2ee are unused by Interact; source base
 // authority lives ONLY in base().lifecycle() and pool_borrow().
 LootItemFieldsV8& fields()noexcept{return fields_;}
 data::LootTemporaryInventoryV8& inventory()noexcept{return inventory_;}
 LootItemObjectBorrowV8 pool_borrow()noexcept;
 void bind_interaction(LootInteractServicesV8);
 bool interact(std::uintptr_t,std::string&);
 bool interact_from_update_v11(std::uintptr_t,std::string&);
 bool init_once(std::int32_t,std::string&);
 bool init_again(data::LootTemporaryInventoryV8&,std::uint32_t,std::uintptr_t,std::string&);
 bool init_again_source_v9(LootInventorySourceV9&,std::uint32_t,std::uintptr_t,std::string&);
 bool pool_operation(const LootItemRequestV8&,std::string&);
 bool update(std::uint32_t dt_ms,std::uintptr_t tooltip_character_ooi,std::string&);
 bool collision(std::uintptr_t character,std::uintptr_t character_ooi,std::string&);
 // Whole original OnCollisionEnds3ebd2c; nullable tooltip branch is genuine.
 bool collision_end_v2(std::uintptr_t character,std::uintptr_t character_ooi,std::string&);
 bool is_interactive(bool&,std::string&);
 // Source XML factory uses inherited GameObject.SetPosition. Forward through
 // the SAME Item service graph; do not write a second position/PF projection.
 bool source_set_position_v57(const float*,bool set_destination,std::string&);
 bool failed()const noexcept{return failed_;}
};
}
