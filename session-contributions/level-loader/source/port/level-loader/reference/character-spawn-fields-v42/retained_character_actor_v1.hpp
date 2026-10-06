#pragma once
#include "canonical_object_manager_v1.hpp"
#include "canonical_property_map_v1.hpp"
#include "character_script_objects.hpp"
#include "character_world_npc_initialization_v1.hpp"
#include "character_animation_instance.hpp"
#include "character_animation_ai.hpp"
#include "actor_runtime.hpp"
#include "npc_injury_runtime_v1.hpp"
#include "character_npc_initpost_owner_v1.hpp"
#include "gameobject_visual_field_borrow_v5.hpp"
#include "character_set_position_v7.hpp"
#include "character_model_name_v38.hpp"
#include "character_is_player_v41.hpp"
#include <array>
namespace dh2::character {
enum class RetainedCharacterConstructionV7 {existing_actor_adoption,fresh_canonical};
struct CharacterLoaderFieldsV38 {
 std::shared_ptr<const void> receiver_lease;
 std::uintptr_t identity{};
 std::int16_t* properties13c8{};std::int16_t* template13ca{};
 std::uintptr_t* master418{};
};
struct CharacterSpawnFieldsV42 {
 std::shared_ptr<const void> receiver_lease;std::uintptr_t identity{};
 std::int32_t* cached_roll270{};const std::int32_t* probability274{};
};
// Native retained ownership extracted from the renderer. No second properties,
// HP, target, FSM or timers: object is the same registry-published script object.
// This constructs the ownership graph, not a claim of complete InitPost.
class RetainedCharacterActorV1 {
 std::shared_ptr<void> world_pin_; // Must outlive every callback/VM resource.
 std::uintptr_t identity_{};
 const std::uint32_t type_f4_{0}; // Original Character factory GO_ID 0.
 target_providers::Handle16 handle_{};
 std::int32_t room64_{-1};
 std::uint8_t across_rooms87_{};bool across_rooms_written_{};
 std::string source_name_,source_archetype_,error_;
 std::string template_name_;
 std::string class_name_;
 const char* class_name20_{};
 std::map<std::uint32_t,std::uint8_t> source_bools_;
 std::map<std::uint32_t,std::string> source_strings_;
 std::int32_t source_cached_spawn270_{-1}; // original GameObject C2 store38c4a8
 std::int32_t spawn_probability_{};bool spawn_probability_written_{};
 std::array<std::int32_t,2> spawn_delay_{};bool spawn_delay_written_{};
 float spawn_view_radius_{};bool spawn_view_radius_written_{};
 std::array<float,3> source_position_{},source_scale_{};
 bool source_position_written_{},source_scale_written_{},source_rotation_written_{};
 std::uint8_t init_post_called1394_{};
 std::int16_t char_properties_id13c8_{-1};
 std::int16_t char_template_id13ca_{-1}; // original C1 signed-halfword store3aa354
 bool loader_fields_produced_v38_{};
 std::int32_t self_fx_offset1488_{};
 std::uintptr_t self_fx1484_{},visual2d8_{};
 CharacterPositionFieldsV7 source_position_fields_v7_;
 // Original CharacterC1 3aa4c0/3aa4e8 stores the same NULL FX fields.
 std::uintptr_t state_fx148c_{},highlight14a0_{};
 float fade1440_{},fade1444_{};
 std::array<float,3> initial_position1450_{},initial_rotation145c_{};
 bool graph_attempted_{},script_attempted_{},script_load_attempted_{};
 bool animation_attempted_{};
 data::PropertyView* initialization_properties_{};
 std::uint32_t initial_delayed3ec_{}; // CharAI ctor0; retired on Session adoption.
 static std::uint32_t* live_delayed(void*);
 static bool set_name(void*,const char*,std::string&);
 static bool set_archetype(void*,const char*,std::string&);
 static bool adoption_name_v2(void*,const char*,std::string&);
 static bool adoption_archetype_v2(void*,const char*,std::string&);
 static bool as_character(void*,std::uintptr_t&,std::string&);
 static bool read_bool(void*,std::uint32_t,std::uint8_t&,std::string&);
 static bool read_across_rooms(void* p,std::uint8_t& value,std::string& e){return read_bool(p,0x87,value,e);}
 static bool write_bool(void*,std::uint32_t,std::uint8_t,std::string&);
 static bool write_int(void*,std::uint32_t,std::int32_t,std::string&);
 static bool write_float(void*,std::uint32_t,float,std::string&);
 static bool write_string(void*,std::uint32_t,const std::string&,std::string&);
 static bool write_vector3(void*,std::uint32_t,const std::array<float,3>&,std::string&);
 static bool write_point2(void*,std::uint32_t,const std::array<std::int32_t,2>&,std::string&);
public:
 std::shared_ptr<ScriptCharacterObject> object;
 std::unique_ptr<CharacterWorldNpcStateOwnerV1> machine;
 Facts facts{};StateOwnerBehaviorPredicate8 predicates{};Services bodies{};
 std::unique_ptr<StateOwnerDebugDiagnostics> diagnostics;
 std::unique_ptr<CharacterWorldNpcControllerV1> controller;
 AIEventOwner48 ai_owner{};AIEventState64 ai_events{};
 std::unique_ptr<CharacterWorldNpcStateChangedV1> state_changed;
 std::unique_ptr<CharacterAnimationInstance> animation;
 actor::RuntimeState runtime{};AnimationAIState96 animation_ai{};
 std::uint32_t scene_clock{},application_dt{},source_animation_events{};
 float injury_gate{-1.f};bool visual_initialized{};
 // SAME GameObject+26c path-limit word; base source constructor stores0.
 std::uint32_t source_path_limit26c{};
 // Source CharacterC1 3a967c: OOI+14a4 is distinct from CharAI current target.
 std::uintptr_t source_ooi14a4{};
 // CharAI constructor's group pointer+34. Source Group/config publication
 // writes this SAME field; absence is constructor NULL, not an inferred gate.
 std::uintptr_t source_ai_group34{};
 std::uintptr_t source_ai_master50{}; // SAME CharAI+50 / Character+418
 std::uint8_t source_bounds_flat{0};
 std::array<float,16> bounds_root_matrix{};bool bounds_root_ready{},source_bounds_ready{};
 std::array<float,6> rendered_bounds{};bool rendered_bounds_ready{};
 std::array<float,4> rendered_screen_bounds{};bool rendered_screen_bounds_ready{};
 skills::SkillAttackNativeServicesV6 injury_debug{};
 std::unique_ptr<NpcInjuryRuntimeV1> injury;
 std::unique_ptr<CharacterScriptSession> session;
 RetainedCharacterActorV1(std::uintptr_t,std::shared_ptr<void> world_pin,const std::string& actual_catalog_name,
  RetainedCharacterConstructionV7=RetainedCharacterConstructionV7::existing_actor_adoption);
 virtual ~RetainedCharacterActorV1();
 RetainedCharacterActorV1(const RetainedCharacterActorV1&)=delete;
 RetainedCharacterActorV1& operator=(const RetainedCharacterActorV1&)=delete;
 // Derived callback transport must call close() in its destructor body, before
 // derived context disappears. Idempotent; World lease remains pinned.
 void close() noexcept;
 world::CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> receiver_lease);
 // Development graph adoption into SAME source manager; never repeats C1,
 // InitProperties, defaults, VM/FSM construction or changes source strings.
 // Preflight completes before manager Add's publication/mutation prefix.
 bool canonical_adoption_v2(std::shared_ptr<void>,const std::string& actual_name,
  const std::string& actual_archetype,std::int32_t actual_room,
  world::CanonicalObjectBorrowV1&,std::string&);
 world::CanonicalPropertyActorV1 properties()noexcept;
 bool source_position(std::array<float,3>&,std::string&)const;
 // Same source Position160 before/after ScriptCharacterObject publication.
 const float* source_position160_v7()const noexcept{return object?object->position.data():source_position_.data();}
 CharacterPositionFieldsV7& position_fields_v7()noexcept{return source_position_fields_v7_;}
 const CharacterPositionFieldsV7& position_fields_v7()const noexcept{return source_position_fields_v7_;}
 bool source_scale(std::array<float,3>&,std::string&)const;
 const std::string* source_string(std::uint32_t offset)const noexcept;
 // Mutable borrow of existing inherited CString fields after PropertyMap
 // produced them. Does not insert defaults or allocate a second base owner.
 bool visual_strings_v6(std::string*& model290,std::string*& xref2a8,std::string&);
 const std::uint8_t* source_bool_field(std::uint32_t offset)const noexcept{
  if(offset==0x15c)return &source_bounds_flat;
  const auto found=source_bools_.find(offset);return found==source_bools_.end()?nullptr:&found->second;
 }
 // Source factory/visual helper publishes its real retained VisualObject field.
 std::uintptr_t& source_visual()noexcept{return visual2d8_;}
 std::uintptr_t& source_self_fx1484()noexcept{return self_fx1484_;}
 std::uintptr_t& source_state_fx148c()noexcept{return state_fx148c_;}
 std::uintptr_t& source_highlight14a0()noexcept{return highlight14a0_;}
 bool spawn_fields_v42(std::shared_ptr<const void>,CharacterSpawnFieldsV42&,std::string&);
 bool is_player_fields_v41(std::shared_ptr<const void>,CharacterIsPlayerFieldsV41&,std::string&);
 bool loader_fields_v38(std::shared_ptr<const void>,CharacterLoaderFieldsV38&,std::string&);
 bool model_name_fields_v38(std::shared_ptr<const void>,CharacterModelFieldsV38&,std::string&);
 bool init_post_fields(NpcInitPostBorrowV1&,std::string&);
 bool visual_fields_v5(std::shared_ptr<void> receiver_lease,
  const std::function<bool(std::string&)>& actual_update_pf,
  world::GameObjectVisualFieldBorrowV5&,std::string&);
 target_providers::Handle16& shared_handle() noexcept{return handle_;}
 const std::string& source_name()const noexcept{return source_name_;}
 const std::string& source_archetype()const noexcept{return source_archetype_;}
 void publish_across_rooms(std::uint8_t value)noexcept{across_rooms87_=value;across_rooms_written_=true;}
 // After registered PropertyMap initialization and canonical Add: adopt SAME
 // object, create actual CPU animation, one controller and one state graph.
 // Caller supplies whole source callbacks; failure retains its mutation prefix
 // and cannot be retried on the same receiver.
 bool construct_graph(std::shared_ptr<ScriptCharacterObject>,
  std::shared_ptr<const CharacterAnimationResources>,WorldNpcStateServicesV1);
 // Canonical source staging: constructor graph precedes property/model init;
 // CPU animation resources bind at real visual initialization, not beforehand.
 bool construct_fields(std::shared_ptr<ScriptCharacterObject>,WorldNpcStateServicesV1);
 bool bind_animation(std::shared_ptr<const CharacterAnimationResources>);
 bool bind_initialization_properties(data::PropertyView&,std::string&);
 // Host/Level/target/object providers remain caller-owned and pinned above.
 // Identity/properties/life/position/FSM are forced to the same retained graph.
 bool construct_script(CharacterGameDesign::Borrow&&,CharacterScriptSessionInput);
 // Called at original LoadScriptProcess point, not during native allocation.
 bool load_script();
 const std::string& error()const noexcept{return error_;}
};
}
