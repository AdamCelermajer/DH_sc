#pragma once
#include "canonical_openable_container_v1.hpp"
#include "canonical_spawn_point_v15.hpp"
#include "destructible_container_data_v16.hpp"
namespace dh2::world {
struct DestructibleQuestEventV16 {std::int32_t id{};std::uintptr_t actor{};std::int32_t room{};std::uint8_t flags10{},flags11{};std::int32_t relation{-1},data_id{-1};};
struct ContainerPresentationServicesV16 {
 std::shared_ptr<void> owner;
 std::function<bool(std::int32_t&,std::int32_t&,std::string&)> spawn_roll_and_probability;
 std::function<bool(std::int32_t,std::string&)> visual_asset;
 std::function<bool(const char*,bool&,std::string&)> play_animation;
 std::function<bool(std::uint32_t,std::uint32_t,std::string&)> scene_flags; // clear,set
 std::function<bool(bool&,std::string&)> has_sound_manager,has_script;
 std::function<bool(std::int32_t,std::string&)> load_sound,play_sound_3d;
 std::function<bool(const char*,const char*,std::string&)> load_object_script;
 std::function<bool(std::string&)> source_on_interact,create_attach_po_decor,detach_physical;
 std::function<bool(std::int32_t,std::uintptr_t,std::int32_t,bool,std::string&)> drop_loot_table;
 std::function<bool(const char*,std::uintptr_t,const char*,std::string&)> script_call;
 std::function<bool(std::string&)> precache_complete_source_v42;
};
struct DestructibleContainerServicesV16 {
 std::shared_ptr<void> owner;std::shared_ptr<const DestructibleContainerTableV16> table;
 // Generic source Container protocols; no Openable key fields/receiver alias.
 ContainerPresentationServicesV16 common;
 std::function<bool(bool derived,std::string&)> bind_callbacks;
 std::function<bool(std::uint32_t&,std::string&)> animation_count;
 std::function<bool(std::uint32_t,bool,std::string&)> play_index;
 std::function<bool(std::string&)> visual_sync;
 std::function<bool(const char*,const char*,std::int32_t&,std::string&)> constant;
 std::function<bool(const DestructibleQuestEventV16&,std::string&)> raise_quest;
 std::function<bool(std::uintptr_t,bool&,std::string&)> as_character;
 std::function<bool(std::uintptr_t,std::int32_t,std::int32_t,std::string&)> increment_stat;
 std::function<bool(std::uintptr_t,std::int32_t,std::int32_t&,std::string&)> get_stat;
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_local_player;
 std::function<bool(const char*,std::int32_t&,std::string&)> trophy_id;
 std::function<bool(std::int32_t,std::string&)> unlock_trophy;
 std::function<bool(ContainerNetStructV1&,std::uint32_t,std::string&)> destroy_network;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> destroy_base;
};
class CanonicalDestructibleContainerV16 {
 CanonicalGameObjectBaseOwnerV1 base_;std::array<ContainerNetStructV1,2> network_;
 GameObjectInitializationServicesV1 init_services_;GameObjectInitializationOwnerV1 initialization_;DestructibleContainerServicesV16 services_;
 std::int32_t data374_{},state394_{2};bool data_produced_{},death_reset390_{},destroyed_{};
 std::string data_desc378_;std::uintptr_t opener398_{};std::uint32_t stages6f0_{},remaining6f4_{};
 bool missing(const char*,std::string&)const;const DestructibleContainerRowV16* current_row()const;
 std::int32_t data_id()const noexcept{return services_.table->data_id(data_desc378_);}
 bool set_state(std::int32_t,std::string&);bool container_init_post(std::string&);bool container_interact(std::uintptr_t,std::string&);bool do_open(std::string&);bool base_animation_event(const char*,std::string&);bool destruction_quest(std::uintptr_t,std::string&);
public:
 CanonicalDestructibleContainerV16(std::shared_ptr<void>,actor::RuntimeState&,GameObjectInitializationServicesV1,DestructibleContainerServicesV16);
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> l){return base_.canonical(std::move(l));}
 CanonicalPropertyActorV1 properties()noexcept{return canonical_family_fields_v15(*this);}
 bool read_bool(std::uint32_t,std::uint8_t&,std::string&);bool write_bool(std::uint32_t,std::uint8_t,std::string&);bool write_int(std::uint32_t,std::int32_t,std::string&);bool write_string(std::uint32_t,const std::string&,std::string&);
 std::int32_t state()const noexcept{return state394_;}std::uint32_t stages()const noexcept{return stages6f0_;}std::uint32_t remaining()const noexcept{return remaining6f4_;}
 bool is_interactive()noexcept{return !*base_.byte(0x81)&&state394_==2;}
 static constexpr bool is_animated()noexcept{return true;}
 static constexpr bool is_updatable()noexcept{return false;}
 static constexpr std::int32_t interaction_type()noexcept{return 8;}
 bool get_data_id(std::int32_t& out,std::string& e)const{if(!services_.table)return missing("SAME Arrays DestructibleContainers",e);out=data_id();return true;}
 bool init_post(std::string&);bool init_final(std::string&);bool interact(std::uintptr_t,std::string&);bool animation_event(const char*,std::string&);bool animation_finished(bool,std::string&);bool spawn(std::string&);bool destroy(std::string&);bool set_position(const std::array<float,3>&,bool,std::string&);
 // Additive V21 callback adapter: qualified Container callback before the
 // actual derived InitPost replaces it. Same receiver/state; no new fields.
 bool container_animation_event_v21(const char* name,std::string& e){if(!name)return missing("authored Container event name",e);return base_animation_event(name,e);}
 ContainerNetStructV1& network(std::size_t n){return network_.at(n);}
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalDestructibleContainerV16>,std::shared_ptr<const void>);
};
}
