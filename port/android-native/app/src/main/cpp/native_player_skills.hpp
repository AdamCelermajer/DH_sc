#pragma once
#include "properties.hpp"
#include "player_skill_tables_adapter.hpp"
#include "player_ai_death_v1.hpp"
#include "player_enemy_kill_credit_v1.hpp"
#include "player_hud_skill_slot_resolution_v1.hpp"
#include "../../../../../../port/level-world/character_ai_on_revived_v1.hpp"
#include "../../../../../../port/level-world/character_skill_fsm_callbacks_v1.hpp"
#include "../../../../../../port/level-world/character_cast_lifecycle_v1.hpp"
#include "../../../../../../port/level-world/ais_combat_result_dispatch_v1.hpp"
#include "../../../../../../port/level-world/character_ai_melee_range.hpp"
#include "../../../../../../port/level-world/character_player_buffs_v1.hpp"
#include "../../../../../../port/adam-script-runtime/script_runtime.h"
#include <memory>
#include <vector>
#include <string>
struct AAssetManager;
struct dh2_pycst_view;
namespace dh2::character {class Coordinator;struct Timer32;}
namespace dh2::character_ai_skill_commands_v1 {struct SkillRow;}
namespace dh2::character_ai_skill_commands_v1 {struct Result;}
namespace dh2::character_ai_initialization {struct State;}
namespace dh2::object_update_culling {struct Object;}
namespace dh2::data {class PlayerSavegameV1;struct AiProps;}
namespace dh2::data {class PlayerSaveLoadOwnerV1;}
namespace dh2::data {class FreshInventoryOwnedV4;}
namespace dh2::data {class TrophyManagerOwnerV1;}
namespace dh2::data {struct AiTables;struct AnimationTables;}
namespace dh2::data::savegame_options_v1 {class Owner;}
namespace dh2::native::debug_files {class Backend;}
namespace dh2::native::player_skills {
enum class SkillCallback : std::uint32_t {pre,use,post};
struct SkillCallbackResult {
    std::uint32_t value=0,call_count=0;
    std::int32_t last_lua_status=0;
};
struct SpellOperationResult {
    std::int32_t saved_faery_slot=-1;
    std::int32_t spell_type=0;
    std::uint32_t usable=0;
    std::uint32_t is_casting=0;
};
// Dependencies captured from the active native Character/PlayerManager and
// the single externally owned source TrophyManager. This is a synchronous
// borrowed view; Runtime never creates a second manager or animation owner.
struct BeginSkillServices {
    void* context=nullptr;
    int (*get_anim_stance)(void*,std::uintptr_t,std::int32_t&,std::string&)=nullptr;
    int (*is_local_player)(void*,std::uintptr_t,std::uint32_t&,std::string&)=nullptr;
    data::TrophyManagerOwnerV1* trophy_manager=nullptr;
};
// Live producer fields and operation adapter for the CSSkill Focus/Blur
// callbacks. The callback must implement each reached source operation and
// return failure when an owner is absent; this is not a default/no-op service.
// Runtime combines it with its existing Coordinator and skill-machine fields.
struct SkillStateServices {
    std::uintptr_t debug_switches_identity=0;
    std::uint8_t* ooi_intent_412=nullptr;
    const std::uintptr_t* physical_2dc=nullptr;
    character_skill_fsm_callbacks_v1::Services callbacks{};
};
// Live producer fields and real operation adapter for CSCast state7 Focus/Blur.
// Runtime binds these to the already-retained Coordinator and skill-machine;
// it does not construct another Character, FSM, Player VM, or animation owner.
struct CastStateServices {
    std::uintptr_t debug_switches_identity=0;
    std::uint8_t* ooi_intent_412=nullptr;
    character_cast_lifecycle_v1::Services callbacks{};
};
// Borrows the single level-owned VisualFXManager used by Faery and spawned
// Characters. Player BuffOwner forwards only its source fx_* requests here;
// it creates no AnimatedFX pool, controller, or renderer state of its own.
struct BuffFxServices {
    void* context=nullptr;
    int (*invoke)(void*,const character_player_buffs_v1::Request*,
                  character_player_buffs_v1::Response*)=nullptr;
};
struct Bindings {
    std::uintptr_t character=0,ai=0;
    std::shared_ptr<void> ai_lifetime;
    character_ai_initialization::State* source_ai=nullptr;
    character::set_target::OwnerFacts* target_owner=nullptr;
    character_sm_set_dead_state_v1::AnimStanceServices death_anim_stance{};
    // The canonical Player CharAI aggro relation owner for OnDeath cleanup
    // (relations/retain/mirror/clear/notify/release). Unbound means empty only.
    player_ai_death_v1::Backend death_aggro{};
    const std::uintptr_t* group_identity=nullptr;
    const data::AiTables* ai_tables=nullptr;
    const data::AnimationTables* animation_tables=nullptr;
    std::uintptr_t* self_fx=nullptr;
    std::uintptr_t* state_fx=nullptr;
    std::uintptr_t* highlight_fx=nullptr;
    void* character_queries_context=nullptr;
    int (*is_player)(void*,std::uintptr_t,std::uint32_t*,std::string&)=nullptr;
    const data::AiProps* declaration=nullptr;
    const std::uint32_t* dead=nullptr;
    const object_update_culling::Object* object=nullptr;
    std::uintptr_t controller=0;
    std::shared_ptr<const player_skill_tables_adapter::Tables> tables;
    std::shared_ptr<const void> catalogue_lifetime;
    data::PropertyRules* rules=nullptr;
    data::PropertyState* properties=nullptr;
    data::PropertySheet* shared_property_temp=nullptr;
    std::shared_ptr<data::PlayerSavegameV1> savegame;
    std::uintptr_t* application_singleton=nullptr;
    const data::savegame_options_v1::Owner* saved_options=nullptr;
    // Existing offline session projection. Full COnline/networking remains
    // pending; a reached online remote-object query must fail explicitly.
    std::uintptr_t online_identity=0;
    const std::uint8_t* online=nullptr;
    std::uint8_t* mana_exempt_14f0=nullptr;
    const std::int32_t* current_difficulty=nullptr;
    const data::ClassTables* classes=nullptr;
    const std::vector<std::string>* fields=nullptr;
    const dh2_pycst_view* design=nullptr;
    const dh2_pycst_view* ai_constants=nullptr;
    const dh2_pycst_view* faery_constants=nullptr;
    // Skill-only Character/GameObject APIs backed by the current gameplay
    // owners. This forwards unsupported native calls to the existing world,
    // target list and combat owners; it does not install another VM or list.
    void* skill_gameplay_context=nullptr;
    int (*skill_gameplay_native)(void*,std::uint32_t,
        const void*,std::uint32_t,void*,
        std::uint32_t,std::uint32_t*,char*,std::size_t,const char*)=nullptr;
    character::Coordinator* coordinator=nullptr;
    debug_files::Backend* debug=nullptr;
    AAssetManager* assets=nullptr;
    std::vector<std::uint8_t> (*read)(AAssetManager*,const std::string&)=nullptr;
    // Optional source Character::_SpellCombatRoll adapter. It is distinct from
    // SkillCombatRoll__ and borrows the same Player gameplay/target owners.
    int (*spell_combat_roll_native)(void*,std::uint32_t,
        const void*,std::uint32_t,void*,
        std::uint32_t,std::uint32_t*,char*,std::size_t,const char*)=nullptr;
    BuffFxServices buff_fx{};
    // UINT32_MAX keeps the BuffOwner's source string/name guard explicitly
    // unresolved until the loaded AnimatedEffectTable is supplied.
    std::uint32_t fx_count=UINT32_MAX;
};
// Sole per-Prince skill VM/preparation owner, borrowing current properties,
// Coordinator and immutable catalogue. Full Player AIS/savegame/skill-use
// lifecycle remains unfinished. Keep the two unrelated script Value ABIs in
// their respective translation units; neither is reinterpreted as the other.
class Runtime {
public:
    enum class ProjectileSourceState : std::int32_t {
        invalid = -1,
        lua_script_present = 0,
        lua_script_absent = 1,
    };
    static std::unique_ptr<Runtime> create(Bindings,std::string&);
    ~Runtime();
    Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
    void update();
    // Character::SG_SetSkillInSlot mutates the canonical Save map, then calls
    // CharAI::UpdateSkills. This invokes that tail through the retained Player
    // VM/update owner and reports failure to the synchronous UI action.
    bool update_after_saved_skill_slot_write(std::string& error);
    // Character::ChangeFaery and other source skill-input mutations converge
    // on the same CharAI::UpdateAllSkills owner.
    bool update_after_source_skill_inputs_changed(std::string& error);
    // Exact AI_ReloadSkills middle phase: delete current nullable skill
    // instances in place, reload rows through this same Save/LoadOwner, and
    // rebuild only the skill vector in the same retained VM/preparation owner.
    // UpdateSkills remains the following, separate source phase.
    bool reload_skill_instances(data::PlayerSaveLoadOwnerV1&,std::string& error);
    // NativeReloadSkills providers over the existing BuffOwner/property graph.
    bool remove_all_buffs(std::string& error);
    // CharAI::OnTerminate / AI_ScriptCleanUp terminal prefix. Runs the
    // existing skill then faery cleanup loops while the Character timers,
    // CharAI, and retained Player VM are still alive; it does not dispatch
    // CSSkill/CSCast state Blur callbacks.
    bool cleanup_ai_scripts(std::string& error);
    // Character::CancelSneaking over this Character's existing BuffOwner,
    // computed properties, selected SkillList and retained Player VM. The
    // caller supplies the actual projected Character+0x415 byte; this method
    // does not create a second buff, property, script or skill-vector owner.
    bool cancel_sneaking(std::uint8_t* character_byte_415,
                         std::string& error);
    bool recalculate_properties(bool source_argument,std::string& error);
    // Source Character::IncSkill over the same retained Save, skill VM,
    // property/buff view and caller-supplied canonical V4 inventory.
    bool train_skill(std::uint32_t skill_index,bool test_only,
                     data::FreshInventoryOwnedV4&,std::uint32_t& source_return,
                     std::string& error);
    // Character::InitPost's source _InitSkillsSlots call, using this runtime's
    // Save, buff-aware PropertyView, skill VM/update owner and the canonical V4.
    bool initialize_initial_skill_grants(data::FreshInventoryOwnedV4&,
                                         std::string& error);
    // Source AI_SkillInfo on the retained preparation instance. This executes
    // SetSkill(script,index) and OnSkillInfo(level) on the same Player VM, then
    // snapshots the already-shared Character property temp sheet for UI text.
    // Returns 0 when delivered, 1 for an empty slot/ordinary Lua error, -1 for
    // a required owner/provider failure. No VM, sheet, or timer owner is made.
    int skill_info(std::uint32_t skill_index,std::int32_t level,
                   std::vector<std::int32_t>& shared_temp,std::string& error);
    // Route a projectile's delayed callback through this Player's retained
    // skill VM and the exact Arguments owned by the prepared script instance.
    int projectile_skill_callback(const char* callback_name,
        std::uintptr_t target_identity,std::uintptr_t projectile_identity,
        std::uintptr_t skill_script_identity,
        dh2_script_returns_observer_v1 observer,void* context,
        std::string& error);
    // Validates the exact retained CharAISkillScript+0x0c Arguments owner
    // before observing this Character's canonical LuaScript pointer. Only
    // lua_script_absent corresponds to the original callback's return-1 path.
    ProjectileSourceState projectile_skill_callback_source_state(
        std::uintptr_t skill_script_identity,std::string& error)const;
    // Read HUD usability and cooldown visuals from the same prepared Player
    // script instance, Save slot and Coordinator timer store. This executes
    // the retained OnSkillCheck query but creates no skill/timer/property owner.
    bool hud_info(bool faery,std::uint32_t list_index,bool refresh_usable,
                  std::uint32_t& usable,float& cooldown_fraction,
                  std::string& error);
    // Borrow the exact nullable skill-script vector already owned by the one
    // retained Player preparation. The view is valid only until source
    // UpdateSkills/reload changes it; cast dispatch refreshes it per call.
    const std::vector<std::uintptr_t>* prepared_skill_scripts()const noexcept;
    std::uintptr_t character_identity()const noexcept;
    // Borrow the same Player BuffOwner and attached PropertyView used by
    // status/buff operations. The returned pointers remain owned here and are
    // valid only while this Runtime stays initialized.
    data::PropertyView* combat_status_properties() noexcept;
    character_player_buffs_v1::Owner* combat_status_buffs() noexcept;
    // Resolve NativeHUDSkill's HUD slot through the canonical Save map to its
    // selected SkillList/Save row and same-index retained Player script. The
    // SkillList selector is explicit for callers that have a source value.
    bool resolve_hud_skill_slot(std::int32_t hud_slot,
        std::int32_t skill_list_selector,
        player_hud_skill_slot_resolution_v1::Result&,
        std::string& error)const;
    // Character+0x1068 is CharProperties.resolved[28]. Both convenience
    // resolvers use that proven selector and fallback SkillList 3.
    bool resolve_hud_skill_slot(std::int32_t hud_slot,
        player_hud_skill_slot_resolution_v1::Result&,
        std::string& error)const;
    // Source Character::GetCharSkill over the existing immutable SkillTables.
    // The caller supplies the Character's captured SkillList selector; this
    // returns borrowed fields from the exact selected member row and creates
    // no Skill, VM, Save row, or state-machine owner.
    bool resolve_character_skill_row(std::uint32_t skill_index,
        std::int32_t skill_list_selector,
        character_ai_skill_commands_v1::SkillRow&,
        std::string& error)const;
    bool resolve_character_skill_row(std::uint32_t skill_index,
        character_ai_skill_commands_v1::SkillRow&,
        std::string& error)const;
    // Exact AI_EndSkill kernel on the existing Character, CharAI state,
    // Coordinator and retained SkillTables. The animation owner is borrowed
    // from the host; StopLoop is required only on the source branch that calls
    // it. This method does not synthesize animation events or trigger OnSkill.
    bool end_skill(std::uint32_t skill_index,
        std::uintptr_t animation_owner_identity, void* animation_context,
        bool (*stop_loop)(void*, bool, std::string&),
        character_ai_skill_commands_v1::Result&, std::string& error);
    // Source-ordered AI_BeginSkill over the same Character, CharAI fields,
    // Coordinator, SkillTables and prepared Player VM. animation_owner_identity
    // must be the live Character+0x49c receiver. A missing CSSkill event graph
    // or TrophyManager fails at its reached source operation; no action caller
    // should be enabled until both are actually bound.
    bool begin_skill(std::uint32_t skill_index,
        std::uintptr_t animation_owner_identity,
        const BeginSkillServices&,
        character_ai_skill_commands_v1::Result&, std::string& error);
    // Source SM_SetAnim toggle over the retained CharStateMachine projection.
    // Mutates only its animation_28 field and returns the animation ID that
    // CharAnimator::ANIM_Set receives; the host still owns playback.
    bool select_skill_animation(std::uintptr_t animation_owner_identity,
        std::int32_t requested_animation,std::int32_t& selected_animation,
        std::string& error);
    // Source SM_SetCastState animation selection. Uses the active Character's
    // CharAnimTable `Spells[saved_faery_slot]` sequence and writes the same
    // retained machine animation_28 field used by skill state selection.
    // This does not raise C356 or install state 7; the caller owns that event.
    bool select_cast_animation(std::int32_t animation_table_id,
        std::int32_t saved_faery_slot,
        std::uintptr_t animation_owner_identity,
        std::int32_t& selected_animation,std::string& error);
    // Source-ordered offline AI_BeginSpell/AI_EndSpell over the same Save,
    // prepared faery vector/VM, CharAI bytes, Coordinator and retained
    // Character animation owner used by CSCast. Online controller messaging
    // is not part of this projection. EndSpell requires the real animator
    // StopLoop provider only on the source SpellType==2/byte_d0 branch.
    bool begin_spell(std::uintptr_t animation_owner_identity,
        SpellOperationResult&,std::string& error);
    bool end_spell(std::uintptr_t animation_owner_identity,void* animation_context,
        bool (*stop_loop)(void*,bool,std::string&),
        SpellOperationResult&,std::string& error);
    // Bind CSSkill's exact Focus/Blur callbacks to the same persistent
    // CharStateMachine projection and Coordinator used by Begin/EndSkill.
    // Call before BeginSkill can issue event C355. Character OOI-intent and
    // physical fields and every callback operation are explicit borrowed
    // providers; no fallback owners are manufactured. Normally unbind after
    // leaving state 6; terminal Character destruction uses retire_* instead.
    bool bind_skill_state_callbacks(std::uintptr_t animation_owner_identity,
        const SkillStateServices&,std::string& error);
    bool unbind_skill_state_callbacks(std::string& error);
    bool retire_skill_state_callbacks(std::string& error);
    bool skill_state_callbacks_bound()const noexcept;
    // Bind CSCast state7 Focus/Blur to the same Character/Coordinator and
    // retained machine/animator used by skill and cast-animation selection.
    // Requires that existing skill_machine; does not allocate a second one.
    // Normally unbind after leaving state7; terminal Character destruction
    // uses retire_* instead.
    bool bind_cast_state_callbacks(std::uintptr_t animation_owner_identity,
        const CastStateServices&,std::string& error);
    bool unbind_cast_state_callbacks(std::string& error);
    bool retire_cast_state_callbacks(std::string& error);
    bool cast_state_callbacks_bound()const noexcept;
    // Exact token in cast lifecycle Request.subject for set_animation.
    bool cast_state_machine_identity(std::uintptr_t& identity)const noexcept;
    // CharAI::AI_IsSkillCheck_Usable/Active over that same vector and retained
    // VM. The caller owns source ordering; this does not begin a cast.
    int skill_check(std::uint32_t skill_slot,bool active,std::uint32_t& value,
                    std::string& error);
    // Dispatch AISDefault's selected combat callback through the exact retained
    // Player Session VM. Damage has already been applied by the caller.
    int dispatch_combat_result(std::uintptr_t ais,
        ais_combat_result_dispatch_v1::Callback,
        std::uintptr_t attacker,std::uintptr_t defender,std::string& error);
    // Borrowed source AIS/VCB view for synchronous source-ordered combat
    // dispatch. The pointer is valid only while this Runtime remains alive.
    bool combat_owner(std::uintptr_t& ais,const std::uint32_t*& flags)const noexcept;
    // Borrow the same AISPlayerIPhone source words at +0xd0/+0xd4 that
    // SetScript initializes. AISPlayer::OnAggro/OnDeAggro use them as signed
    // AggroCount/AggroForMusic counters. Requires the requested identity to
    // be this Runtime's currently active AIS; pointers live with this Runtime.
    bool aggro_music_owner(std::uintptr_t ais,std::int32_t*& aggro_count,
        std::int32_t*& aggro_for_music)noexcept;
    // Original Character::TargetInMeleeRange/CharAI::AI_IsInMeleeRange
    // over this retained Character, CharAI identity, and current target word.
    // The renderer supplies only the already-existing native world/provider
    // boundary; the source kernel owns null-target and operation ordering.
    bool target_in_melee_range(void* provider_context,
        std::int32_t (*provider)(void*,character_ai_melee_range::State*,
            const character_ai_melee_range::Request*,
            character_ai_melee_range::Response*),
        std::uint32_t& value,std::string& error)const;
    // Invoke one original Player skill Lua callback through the same retained
    // VM/prepared instance used by HUD checks. The source caller owns timing:
    // OnPreSkill belongs to AI_BeginSkill's passive-skill branch, OnSkill to
    // the selected skill animation event, and OnPostSkill to its source tail.
    // This method does not begin/end a cast, change the FSM, or synthesize an
    // animation event; do not call SkillCallback::use directly from a HUD tap.
    int invoke_skill_callback(std::uint32_t skill_slot,
        SkillCallback callback,SkillCallbackResult& result,std::string& error);
    // Source CSCast callbacks use the faery-script vector, not the skill
    // vector: Character events 32/33 dispatch OnPreSkill/OnPostSkill, and the
    // authored do_spell marker dispatches OnSkill (IDA CharAI::RaiseAIEvent /
    // _SpellFocus, _SpellBlur, _SpellEvent). Invokes through the same retained
    // Player VM and preparation owner; caller retains source timing. Like the
    // source helpers, an out-of-vector or null script slot completes as a
    // no-op; stale ownership for a nonnull instance is an error.
    int invoke_faery_callback(std::uint32_t faery_slot,
        SkillCallback callback,SkillCallbackResult& result,std::string& error);
    // Source CharAI::OnDied only. Character::Kill/rewards/event2 caller is a
    // separate integration boundary; this retains the same AIS and VM.
    void died(std::uintptr_t killer);
    // Character::Revive event3 reaches CharAI::OnRevived on this same
    // CharAI/Coordinator/AIS owner and reopens its source tick timers.
    bool on_revived(std::string& error);
    // Character::Deserialize's reusable CharAI::AI_ScriptCleanUp followed by
    // AI_ScriptInit. Uses this same timer store, Player skill/spell instances,
    // and AIS; this is not the one-shot terminal teardown operation.
    bool deserialize_script_lifecycle(std::string& error);
    bool deserialize_clear_target(std::string& error);
    // One reached Character::Kill episode, after its initial DropLoot attempt.
    // Reuses the live CharAI, Player AIS/Session, Coordinator, source target,
    // and PropertyView. Renderer callback only projects the direct target write.
    player_enemy_kill_credit_v1::Status credit_enemy_kill(
        std::uintptr_t victim,std::uintptr_t killer,std::uint32_t kill_force,
        const data::AggroTable& victim_outgoing,std::uint32_t controller_forced,
        std::uint32_t global_blocked,void* renderer_context,
        int (*clear_renderer_selection)(void*,std::uintptr_t),
        player_enemy_kill_credit_v1::Result*,std::string& error);
    void state_service(std::uint32_t service);
    void timer(std::uint32_t id);
    void buff_expired(const character::Timer32&);
    bool ai_timer(std::int32_t event,const character::Timer32&);
    bool initialized()const noexcept;
    void restore(AAssetManager*,const void* ai_owner,const void* catalogue_owner);
    std::string cooldown_probe(std::uint32_t delay_ms);
    std::string check_probe(std::uint32_t slot);
    std::string mana_probe(std::uint32_t raw_amount);
    std::string scalar_probe(std::int32_t value,bool write);
private:
    struct Impl;
    explicit Runtime(std::unique_ptr<Impl>);
    std::unique_ptr<Impl> impl_;
};
// One GL/game owning thread. Provider globals/storage remain live through calls
// and VM close. Runtime must retire before properties/timers/Debug are reset.
}
