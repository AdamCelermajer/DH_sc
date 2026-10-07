#pragma once
#include <canonical_property_map_v1.hpp>
#include <canonical_object_loading_fields_v95.hpp>
#include <light_set_name_owner_v3.hpp>
#include <memory>
namespace dh2::world {
// The SAME receiver's source D0 uses unique genuine native-reference and base
// continuation leaves. Passive node pins/capability destructors never perform
// the actor120 intrusive drop. Runtime/barrier pins remain independent of the
// containing World/Level/manager; Main retains the barrier through Scene.Clear.
struct LightRuntimeStampV67 {
 std::uintptr_t application{},level{},object_manager{},scene_manager{},lightset_manager{};
 std::uint64_t generation{};
 bool complete()const noexcept{return application&&level&&object_manager&&scene_manager&&lightset_manager&&generation;}
 bool operator==(const LightRuntimeStampV67& x)const noexcept {
  return application==x.application&&level==x.level&&object_manager==x.object_manager&&scene_manager==x.scene_manager&&lightset_manager==x.lightset_manager&&generation==x.generation;
 }
};
struct LightQuiescenceLeaseV67 {
 std::shared_ptr<void> runtime_owner,owning_thread_barrier;
 LightRuntimeStampV67 stamp;
 virtual bool validate_current_owning_thread()const noexcept=0;
 virtual ~LightQuiescenceLeaseV67()=default;
};
class LightActorNodeReferenceV67 {
public:
 std::shared_ptr<void> runtime_owner;
 LightRuntimeStampV67 stamp;
 std::uintptr_t actor_identity{},node_identity{},light_identity{};
 LightActorNodeReferenceV67()=default;
 LightActorNodeReferenceV67(const LightActorNodeReferenceV67&)=delete;
 LightActorNodeReferenceV67& operator=(const LightActorNodeReferenceV67&)=delete;
 virtual bool validate_exact_actor_reference(const LightQuiescenceLeaseV67&)const noexcept=0;
 // Native31d584 equivalent, synchronous and nonthrowing. May destroy node.
 // Exactly one actor-owned native reference; destructor does not repeat drop.
 virtual void drop_actual_actor_node_reference()noexcept=0;
 virtual ~LightActorNodeReferenceV67()=default;
};
// Borrowed ONLY during the existing base continuation. These are the SAME
// canonical receiver cells, not a copied/native-ARM ObjectBase facade. Main
// must consume its real handle/condition/native release leaves in source order
// through this view; never free the host embedded Handle16 as an ARM pointer.
struct LightObjectBaseFieldsV67 {
 std::uintptr_t actor_identity{};
 target_providers::Handle16* handle{};std::uint8_t* publication29{};
 std::string* template8{};std::string* name30{};std::string* archetype48{};
 std::string* room68{};std::string* activate90{};std::string* deactivateb4{};
 std::string* difficultyd4{};
 std::uintptr_t *owned2c{},*condition_a8{},*condition_cc{};
 std::uint8_t *tested_ac{},*tested_d0{};
};
class LightObjectBaseContinuationV67 {
public:
 std::shared_ptr<void> runtime_owner;LightRuntimeStampV67 stamp;
 std::uintptr_t actor_identity{};
 virtual bool validate_exact_actor(const LightQuiescenceLeaseV67&)const noexcept=0;
 // Existing same-actor ObjectBase cleanup, source33e998 equivalent; never cast
 // host layout to original ARM storage or substitute an empty callback.
 virtual void destroy_actual_objectbase_fields(LightObjectBaseFieldsV67&)noexcept=0;
 virtual bool destroy_actual_objectbase_fields_checked_v113(LightObjectBaseFieldsV67& f,std::string& e){destroy_actual_objectbase_fields(f);e.clear();return true;}
 virtual ~LightObjectBaseContinuationV67()=default;
};
// A borrowed ACTUAL renderer light node. Main supplies its retained node owner,
// native identity and storage writer. A source miss is node.identity==0.
struct LightNodeBorrowV53 {std::shared_ptr<void> owner;std::uintptr_t identity{},actual_light_identity{};std::unique_ptr<LightActorNodeReferenceV67> actor_reference;};
struct LightParameterFieldsV53 {
 std::shared_ptr<void> owner;std::uintptr_t actual_light_identity{};
 std::array<float,4>* ambient4{};std::array<float,4>* diffuse14{};std::array<float,4>* specular24{};
 std::array<float,3>* attenuation34{};float* radius40{};
 std::function<bool(std::string&)> validate_current;
};
struct LightPointAttachmentServicesV113 {
 std::shared_ptr<void> owner;
 std::function<bool(bool&,std::string&,std::string&)> local_player_name;
 std::function<bool(std::int32_t,std::int32_t,std::string&)> assign_tweaker;
 std::function<bool(const std::string&,std::int32_t,target_providers::Handle16&,bool&,std::string&)> find_handle;
 std::function<bool(target_providers::Handle16&,bool&,std::array<float,3>&,std::string&)> resolve_position;
 std::function<bool(target_providers::Handle16,std::string&)> add_active;
 std::function<bool(bool&,std::string&)> debug_player_headlight;
 std::function<bool(const std::array<float,3>&,std::string&)> node_position;
};
struct LightPointInitServicesV53 {
 // Bind only the genuine existing base continuation for this exact canonical
 // receiver. Called during checked teardown preflight, never during mutation.
 std::function<bool(std::uintptr_t,std::unique_ptr<LightObjectBaseContinuationV67>&,std::string&)> bind_objectbase_dtor;
 std::shared_ptr<void> owner;
 // Original LoadScene(dae,"",false,false), choose first actual light node.
 std::function<bool(const std::string&,const char*,bool,bool,LightNodeBorrowV53&,std::string&)> first_scene_light;
 std::function<bool(bool,LightNodeBorrowV53&,std::string&)> construct_light_node;
 std::function<bool(const LightNodeBorrowV53&,std::string&)> attach_root;
 std::function<bool(std::uintptr_t,const LightNodeBorrowV53&,std::string&)> add_automatic;
 // Source SyncData's actual CLight storage/driver writer; not GPU completion.
 std::function<bool(const LightNodeBorrowV53&,LightParameterFieldsV53&,std::string&)> borrow_light_parameters;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 std::function<bool(const LightNodeBorrowV53&,std::uint16_t,std::string&)> set_type;
 // Must borrow the existing manager Names owner, not allocate a parallel one.
 std::shared_ptr<LightSetNameOwnerV3> names;
 std::function<bool(std::uintptr_t,const LightNodeBorrowV53&,LightSetNameOwnerV3&,std::string&)> refresh_attachment;
 std::shared_ptr<const LightPointAttachmentServicesV113> attachment_v113;
 std::function<bool(LightPointAttachmentServicesV113&,std::string&)> bind_attachment_v113;
};
// Genuine ObjectBase-derived factory34115c / LightPointC1_40bdd8, GO_ID19.
// Never inherits GameObject geometry/Visual/Character fields.
enum class LightTeardownStateV67 {Alive,Prepared,AttachedTo180Released,NativeActorReferenceDropped,Node120Cleared,Dae168Released,ObjectBaseCompleted};
class CanonicalLightPointV53 final {
 LightTeardownStateV67 teardown_state_{LightTeardownStateV67::Alive};
 std::shared_ptr<LightQuiescenceLeaseV67> teardown_lease_;
 std::unique_ptr<LightObjectBaseContinuationV67> objectbase_dtor_;
 bool teardown_busy_{},teardown_reentered_{};
 bool writable(std::string&)const;
 std::shared_ptr<void> pin_;std::uintptr_t identity_;
 target_providers::Handle16 handle_;std::uint32_t type_{19};std::int32_t room_{-1};const char* class_name_{};
 std::uintptr_t condition_a8_v95_{},condition_cc_v95_{};
 std::uintptr_t owned2c_v113_{}; //actual ObjectBase C1 NULL2c; no native allocation yet produced
 std::uint8_t tested_ac_v95_{},tested_d0_v95_{};
 std::string template_;std::map<std::uint32_t,std::uint8_t> bytes_;
 std::map<std::uint32_t,std::int32_t> ints_;std::map<std::uint32_t,float> floats_;
 std::map<std::uint32_t,std::string> strings_;std::map<std::uint32_t,std::array<float,3>> vectors_;
 LightPointInitServicesV53 services_;LightNodeBorrowV53 node_;bool attempted_{},complete_{};std::string error_;
 target_providers::Handle16 attached_handle198_v113_{-1,0,0}; //33f50c: key8=-1,frame4=0,cached0=NULL.
 static bool set_name(void*,const char*,std::string&);static bool set_archetype(void*,const char*,std::string&);
 static bool as_character(void*,std::uintptr_t&,std::string&);static bool across(void*,std::uint8_t&,std::string&);
 static bool read_bool(void*,std::uint32_t,std::uint8_t&,std::string&);
 static bool write_bool(void*,std::uint32_t,std::uint8_t,std::string&);static bool write_int(void*,std::uint32_t,std::int32_t,std::string&);
 static bool write_float(void*,std::uint32_t,float,std::string&);static bool write_string(void*,std::uint32_t,const std::string&,std::string&);
 static bool write_vector(void*,std::uint32_t,const std::array<float,3>&,std::string&);
 bool fail(const std::string&,std::string&);
 bool write_light_parameters(const LightParameterFieldsV53&,std::string&);
public:
 CanonicalLightPointV53(std::shared_ptr<void>,LightPointInitServicesV53);
 CanonicalLightPointV53(const CanonicalLightPointV53&)=delete;
 CanonicalLightPointV53& operator=(const CanonicalLightPointV53&)=delete;
 ~CanonicalLightPointV53()noexcept=default; // passive AFTER checked journal D0
 bool prepare_source_teardown(std::shared_ptr<LightQuiescenceLeaseV67>,std::string&);
 bool execute_source_teardown(std::shared_ptr<LightQuiescenceLeaseV67>,std::string&);
 LightTeardownStateV67 source_teardown_state()const noexcept{return teardown_state_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void>);
 CanonicalPropertyActorV1 properties()noexcept;
 bool init_post(std::string&);
 bool source_loading_fields_v95(std::shared_ptr<void>,CanonicalObjectLoadingFieldsV95&,std::string&);
 //Actual LightBase/LightPoint vtable40/3c/44/48 inherit these ObjectBase
 //source bodies. They do not toggle CLight5a or node visibility.
 bool source_set_visible_v96(bool,std::string&);
 bool source_set_updating_v96(bool,std::string&);
 bool source_enabled_event_v96(bool,std::string&);
 bool sync_data(const LightParameterFieldsV53&,std::string&);
 bool source_refresh_attachment_v113(const LightPointAttachmentServicesV113&,std::string&);
 bool source_frame_update_v113(std::string&);
 bool source_parameter_setter_v113(std::uint32_t,const std::array<float,3>&,std::string&);
 std::int32_t* source_integer_v113(std::uint32_t offset)noexcept;
 std::uintptr_t* source_pointer_v113(std::uint32_t offset)noexcept;
 const std::string* string(std::uint32_t)const noexcept;
 const std::array<float,3>* vector(std::uint32_t)const noexcept;
 const float* floating(std::uint32_t)const noexcept;
 const std::uint8_t* byte(std::uint32_t)const noexcept;
 std::uint8_t* source_save_byte_v89(std::uint32_t,std::string&);
 std::uintptr_t identity()const noexcept{return identity_;}
 const LightNodeBorrowV53& actual_node()const noexcept{return node_;}
 bool init_complete()const noexcept{return complete_;}
 static constexpr bool is_game_object()noexcept{return false;}
};
}


