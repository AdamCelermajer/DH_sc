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
#include "character_kill_fields_v21.hpp"
#include "gameobject_target_list_owner_v107.hpp"
#include "game_object_initialization_borrow_v62.hpp"
#include "source_character_aggro_v84.hpp"
#include "world_click_fields_owner_v1.hpp"
#include "character_ai_pointer_fields_v105.hpp"
#include "character_frame_fields_v106.hpp"
#include <array>
namespace dh2::character {
enum class RetainedCharacterConstructionV7 {existing_actor_adoption,fresh_canonical};
// Native retained ownership extracted from the renderer. No second properties,
// HP, target, FSM or timers: object is the same registry-published script object.
// This constructs the ownership graph, not a claim of complete InitPost.
class RetainedCharacterActorV1 {
 // Native object lifetime token: survives diagnostic/facade aliases only
 //while this actual unique actor allocation lives. Never an HP/ready flag.
 std::shared_ptr<void> native_lifetime_v107_=std::make_shared<std::uint8_t>(0);
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
 // Source inherited C1 cells not otherwise projected by the runtime. Fresh
 // construction alone produces these; observed adoption must supply them.
 std::map<std::uint32_t,std::int32_t> source_integers_v62_;
 std::map<std::uint32_t,std::uintptr_t> source_pointers_v62_;
 std::uint8_t source_byte82_v62_{}; // Native backing only; C1 does not produce a readable value.
 std::int32_t spawn_probability_{};bool spawn_probability_written_{};
 std::array<std::int32_t,2> spawn_delay_{};bool spawn_delay_written_{};
 float spawn_view_radius_{};bool spawn_view_radius_written_{};
 std::array<float,3> source_position_{},source_scale_{};
 bool source_position_written_{},source_scale_written_{},source_rotation_written_{};
 std::uint8_t init_post_called1394_{};
 std::int16_t char_properties_id13c8_{-1};
 std::int32_t self_fx_offset1488_{};
 std::uintptr_t self_fx1484_{},visual2d8_{};
 CharacterPositionFieldsV7 source_position_fields_v7_;
 CharacterKillFieldsV21 source_kill_fields_v42_;
 std::unique_ptr<SourceCharacterAggroV84> source_aggro_v84_;
 std::unique_ptr<world::WorldClickFieldsOwnerV1> source_click_v101_;
 std::unique_ptr<CharacterAiPointerFieldsV105> source_ai_pointers_v105_;
 std::unique_ptr<CharacterFrameFieldsV106> source_frame_fields_v106_;
 std::unique_ptr<world::GameObjectTargetListOwnerV107> source_target_list304_v111_;
 bool source_ctor_empty_timers_v111_{}; //CharTimersC1 3dbb78..88 count0
 bool source_ai_queue_registered_v105_{};
 std::array<std::uint32_t,2> source_ctor_skill_vector_counts_v84_{};
 bool source_ctor_skill_vectors_v84_{};
 // Original CharacterC1 3aa4c0/3aa4e8 stores the same NULL FX fields.
 std::uintptr_t state_fx148c_{},highlight14a0_{};
 std::uintptr_t target_cross149c_v70_{};bool target_cross149c_produced_v70_{};
 std::uint32_t network114_v70_{};bool network114_produced_v70_{};
 float fade1440_{},fade1444_{};
 std::array<float,3> initial_position1450_{},initial_rotation145c_{};
 std::array<float,3> checkpoint1468_v83_{},save_position1474_v83_{};
 bool checkpoint_c1_v83_{};
 bool graph_attempted_{},script_attempted_{},script_load_attempted_{};
 bool animation_attempted_{};
 data::PropertyView* initialization_properties_{};
 std::uint32_t initial_delayed3ec_{}; // CharAI ctor0; retired on Session adoption.
 ScriptLifecycleState64* player_lifecycle_v62_{}; // Borrowed SAME V6 owner, never a second lifecycle.
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
 std::weak_ptr<void> native_lifetime_v107()const noexcept{return native_lifetime_v107_;}
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
 std::int32_t source_ai_group_role38{-1}; //actual CharAI C1 3cec8c, Character400
 //Borrow real authored Point2Di1434, not a separate spawn-selection state.
 std::int32_t* source_spawn_delay_v87()noexcept{return spawn_delay_written_?spawn_delay_.data():nullptr;}
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
 SourceCharacterAggroV84* source_aggro_v84()noexcept{return source_aggro_v84_.get();}
 world::WorldClickFieldsOwnerV1* source_click_fields_v101()noexcept{return source_click_v101_.get();}
 CharacterAiPointerFieldsV105* source_ai_pointers_v105()noexcept{return source_ai_pointers_v105_.get();}
 CharacterFrameFieldsV106* source_frame_fields_v106()noexcept{return source_frame_fields_v106_.get();}
 world::GameObjectTargetListOwnerV107* source_target_list304_v111()noexcept{return source_target_list304_v111_.get();}
 bool source_ctor_empty_timers_v111()const noexcept{return source_ctor_empty_timers_v111_;}
 void source_delete_v111(){source_bools_[0x82]=2;source_bools_[0x81]=1;} //ObjectBase.Delete33ddb4
 std::uintptr_t* source_auxiliary14ec_v107()noexcept{
  auto at=source_pointers_v62_.find(0x14ec);return at==source_pointers_v62_.end()?nullptr:&at->second;
 }
 std::uint8_t* source_heading_enabled412_v101()noexcept{
  auto at=source_bools_.find(0x412);return at==source_bools_.end()?nullptr:&at->second;
 }
 const std::array<std::uint32_t,2>* source_ctor_empty_skill_vectors_v84()const noexcept{
  return source_ctor_skill_vectors_v84_?&source_ctor_skill_vector_counts_v84_:nullptr;
 }
 //Retire only when the SAME native SkillOwner lends the actual live vectors.
 void source_retire_ctor_skill_vectors_v84()noexcept{source_ctor_skill_vectors_v84_=false;}
 float* checkpoint1468_v83()noexcept{return checkpoint_c1_v83_?checkpoint1468_v83_.data():nullptr;}
 float* save_position1474_v83()noexcept{return checkpoint_c1_v83_?save_position1474_v83_.data():nullptr;}
 // Fresh canonical C1 produces these automatically. Development allocation
 // that adopts live position/PF may publish ONLY its actual fresh Character
 // C1 kill prefix separately; restoration never calls this again.
 bool construct_kill_fields_source_c1_v42(std::string&);
 bool adopt_kill_fields_observed_v42(std::uintptr_t killer,std::uintptr_t master,
  std::int16_t template_id,std::uint8_t suppress_quest,std::string&);
 CharacterKillFieldsV21& kill_fields_v42()noexcept{return source_kill_fields_v42_;}
 const CharacterKillFieldsV21& kill_fields_v42()const noexcept{return source_kill_fields_v42_;}
 bool source_scale(std::array<float,3>&,std::string&)const;
 const std::string* source_string(std::uint32_t offset)const noexcept;
 // Mutable borrow of existing inherited CString fields after PropertyMap
 // produced them. Does not insert defaults or allocate a second base owner.
 bool visual_strings_v6(std::string*& model290,std::string*& xref2a8,std::string&);
 const std::uint8_t* source_bool_field(std::uint32_t offset)const noexcept{
  if(offset==0x15c)return &source_bounds_flat;
  const auto found=source_bools_.find(offset);return found==source_bools_.end()?nullptr:&found->second;
 }
 //Source GameObject.SetVisible38b0f0 publishes80 before visual sync. True
 //rereads actual8a; this is an observed method store, never a C1 default.
 bool source_set_visible80_v96(bool visible,std::uint8_t& value,std::string& e){
  value=0;if(visible){auto field=source_bool_field(0x8a);if(!field){e="Required source ObjectBase always-visible8a";return false;}value=*field;}
  source_bools_[0x80]=value;e.clear();return true;
 }
 //Observed source Objective_TalkToNPC Register47dbb4/Unregister47dc44
 //writes. Fresh GameObjectC2 produces0; observed adoption remains required.
 void publish_talk_requirement_v75(std::uint8_t value){source_bools_[0x2fa]=value;}
 void publish_anim_state_flags_v116(bool end_on_event,bool end_on_update){source_bools_[0x540]=end_on_event;source_bools_[0x541]=end_on_update;}
 void publish_limbus_mode_v118(bool value){source_bools_[0x530]=value;}
 //Only original TalkToNPC marker Install/Remove stores these source flags.
 void publish_talk_marker_flags_v76(std::uint8_t required,std::uint8_t primary){source_bools_[0x2fa]=required;source_bools_[0x2fb]=primary;}
 // Source factory/visual helper publishes its real retained VisualObject field.
 std::uintptr_t& source_visual()noexcept{return visual2d8_;}
 std::uintptr_t& source_self_fx1484()noexcept{return self_fx1484_;}
 std::uintptr_t& source_state_fx148c()noexcept{return state_fx148c_;}
 std::uintptr_t& source_highlight14a0()noexcept{return highlight14a0_;}
 std::uint32_t* source_network114_v70()noexcept{return network114_produced_v70_?&network114_v70_:nullptr;}
 std::uintptr_t* source_target_cross149c_v70()noexcept{return target_cross149c_produced_v70_?&target_cross149c_v70_:nullptr;}
 bool init_post_fields(NpcInitPostBorrowV1&,std::string&);
 bool inherited_initialization_fields_v62(std::shared_ptr<void>,world::GameObjectInitializationFieldsV62&,std::string&);
 std::uint8_t* failed_spawn_byte82_v62()noexcept{return &source_byte82_v62_;}
 //Actual ObjectBase lock29 C1/observed storage; adoption never inserts a zero.
 std::uint8_t* source_lock29_v88()noexcept{auto at=source_bools_.find(0x29);return at==source_bools_.end()?nullptr:&at->second;}
 void source_object_base_delete_v62()noexcept{source_byte82_v62_=2;source_bools_[0x81]=1;}
 bool visual_fields_v5(std::shared_ptr<void> receiver_lease,
  const std::function<bool(std::string&)>& actual_update_pf,
  world::GameObjectVisualFieldBorrowV5&,std::string&);
 target_providers::Handle16& shared_handle() noexcept{return handle_;}
 // Additive Kill borrow only: these are the SAME constructor/InitPre/OOI
 // cells. No PropertyMap, initialization provider or field replay occurs.
 void kill_metadata_borrow_v23(const std::int32_t*& room,const std::int16_t*& properties,std::uintptr_t*& tracked)noexcept{room=&room64_;properties=&char_properties_id13c8_;tracked=&source_ooi14a4;}
 bool source_set_name_v114(const char* name,std::string& e){if(!name||!object){e="Require actual runtime ObjectBase.SetName receiver";return false;}source_name_=name;object->name=source_name_;e.clear();return true;}
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
 bool bind_player_script_lifecycle_v62(ScriptLifecycleState64*,std::string&);
 void detach_player_script_lifecycle_v62()noexcept{player_lifecycle_v62_=nullptr;}
 const std::string& error()const noexcept{return error_;}
};
}
