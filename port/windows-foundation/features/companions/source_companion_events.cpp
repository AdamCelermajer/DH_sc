#include "source_owner_contract.hpp"

#include "../../../level-world/canonical_character_candidate_v60.hpp"
#include "../../../level-world/object_identity.hpp"
#include "../../../level-world/retained_character_actor_v1.hpp"

namespace dh::foundation::companions {
namespace {
using Record = dh2::world::CanonicalCharacterCandidateRecordV60;
using namespace dh2::character;

bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool source_rene(const Record& record) {
    if (!record.actor || !record.properties) return false;
    const auto* ai = record.design.ai();
    const auto* row = ai ? dh2::data::ai_props(*ai, record.properties->resolved[1]) : nullptr;
    return row && row->script == "rene";
}

bool same_target(const Record& record, TargetBindings48*& binding,
    std::uintptr_t& target, std::string& error) {
    auto* actor = record.actor.get();
    if (!actor || !actor->object) return fail(error, "Required retained source Rene Character object");
    binding = &actor->object->binding;
    auto& object = *actor->object;
    if (!binding->state || binding->state != &object.target ||
        !binding->state->owner || binding->state->owner != &object.owner ||
        binding->state->owner->identity != object.identity ||
        !object.identity) {
        return fail(error, "Rene TargetBindings do not borrow the same Character target owner");
    }
    target = binding->state->target;
    return true;
}

bool same_active_session(CharacterScriptSession& session, std::uintptr_t character,
    ScriptSessionView& view, std::string& error) {
    if (!session.owner().active(view) || view.kind != script_external || !view.identity ||
        !view.vm || !view.aliases || !view.constructor_fields ||
        view.constructor_fields->character != character ||
        session.owner().lifecycle().owner != character) {
        return fail(error, "Required active same-Character V1 AISExternal session");
    }
    return true;
}

bool call_alias(CharacterScriptSession& session, const ScriptSessionView& view,
    const char* alias, std::string& error) {
    if (!dh2_script_alias_contains(view.aliases, alias)) return true;
    std::uint32_t source_error{};
    const int status = session.owner().call_discard(view.identity, alias, nullptr, 0, source_error);
    if (status || source_error) {
        error = "Original Rene V1 callback failed: " + std::string(alias) +
            " (native " + std::to_string(status) + ", source " +
            std::to_string(source_error) + "): " + session.error();
        return false;
    }
    return true;
}

bool verify_after_callback(const Record& record, CharacterScriptSession& session,
    const ScriptSessionView& before, std::string& error) {
    ScriptSessionView after{};
    if (!same_active_session(session, before.constructor_fields->character, after, error) ||
        after.identity != before.identity || after.vm != before.vm || after.aliases != before.aliases) {
        if (error.empty()) error = "Rene callback replaced the same retained V1 script session";
        return false;
    }
    if (!record.actor || !record.actor->object ||
        record.actor->object->identity != before.constructor_fields->character) {
        return fail(error, "Rene callback changed the retained source Character identity");
    }
    return true;
}

struct TargetCallback { std::uint32_t event; std::uint32_t object_event; };
constexpr TargetCallback target_callbacks[]{
    {9, dh2::object_identity::enemy_spotted},
    {0xa, dh2::object_identity::target_died},
    {0xc, dh2::object_identity::target_out_of_sight},
    {0xd, dh2::object_identity::target_in_sight},
    {0xe, dh2::object_identity::target_out_of_range},
    {0xf, dh2::object_identity::target_in_ranged_range},
    {0x10, dh2::object_identity::target_in_close_range},
    {0x11, dh2::object_identity::target_in_melee_range}
};
struct MasterCallback { std::uint32_t event; const char* name; std::uint32_t vcb_flag; };
constexpr MasterCallback master_callbacks[]{
    {18, "OnMasterDied", 0}, {19, "OnMasterRevived", 0},
    {20, "OnMasterOutOfSight", 0}, {21, "OnMasterInSight", 0},
    {22, "OnMasterOutOfRange", 1u << 6},
    {23, "OnMasterInRangedRange", 1u << 7},
    {24, "OnMasterInCloseRange", 1u << 8},
    {25, "OnMasterInMeleeRange", 1u << 9}
};
} // namespace

bool dispatch_source_rene_character_event_v1(const std::shared_ptr<Record>& record,
    std::uint32_t event, std::uintptr_t subject, std::string& error) {
    error.clear();
    if (!record || record->failed || !record->actor || !record->actor->object ||
        !record->actor->machine || !record->actor->session || !record->fsm_context_v101 ||
        !source_rene(*record)) {
        return fail(error, "Required retained source Rene record, selected row, V1 session and CampaignFsm");
    }

    // Holding this typed receipt spans the source callback and any original
    // MoveTo/HasPath operation it reaches. This keeps the same CampaignFsm and
    // its PF-owned route scratch alive without cloning navigation state.
    model_renderer::SourceCharacterPathBorrowV105 path;
    if (!model_renderer::borrow_source_campaign_character_path_v105(record, path, error)) return false;
    auto& actor = *record->actor;
    const auto character = actor.object->identity;
    if (!character || path.character != character ||
        path.machine != &actor.machine->native_fsm() ||
        path.path != &actor.runtime.path || path.record_lease.get() != record.get() ||
        path.context_lease != record->fsm_context_v101) {
        return fail(error, "Source Rene event path receipt is not the same retained Character/FSM/path");
    }

    auto* fields = actor.source_ai_pointers_v105();
    if (!fields) return fail(error, "Required actual CharAI+0x50 master projection");
    auto& session = *actor.session;
    ScriptSessionView view{};
    if (!same_active_session(session, character, view, error)) return false;

    TargetBindings48* entry_binding{};
    std::uintptr_t target_before{};
    if (!same_target(*record, entry_binding, target_before, error)) return false;

    for (const auto& callback : target_callbacks) {
        if (callback.event != event) continue;
        if (event == 9) {
            if (!subject) return fail(error, "OnEnemySpotted requires the original nonnull Character payload");
        } else if (subject) {
            return fail(error, "Source Rene target transition has no Character payload");
        }
        const int status = session.dispatch_target(callback.object_event,
            event == 9 ? subject : 0);
        if (status) {
            error = session.error().empty() ? "Original Rene target callback failed" : session.error();
            return false;
        }
        if (!verify_after_callback(*record, session, view, error)) return false;
        std::uintptr_t target_after{};TargetBindings48* after_binding{};
        if (!same_target(*record, after_binding, target_after, error)) return false;
        if (after_binding != entry_binding || after_binding != &actor.object->binding ||
            target_after != actor.object->target.target) {
            return fail(error, "Rene target callback left the same TargetBindings publication incoherent");
        }
        (void)entry_binding;
        (void)target_before;
        return true;
    }

    if (event == 0xb) {
        if (!dh2_script_alias_contains(view.aliases, "OnTargetRevived")) return true;
        if (!call_alias(session, view, "OnTargetRevived", error) ||
            !verify_after_callback(*record, session, view, error)) return false;
        std::uintptr_t target_after{};TargetBindings48* after_binding{};
        if (!same_target(*record, after_binding, target_after, error)) return false;
        if (after_binding != entry_binding || after_binding != &actor.object->binding)
            return fail(error, "Rene revival changed its same Character TargetBindings");
        return true;
    }

    for (const auto& callback : master_callbacks) {
        if (callback.event != event) continue;
        if (!fields->master50 || subject != fields->master50) {
            return fail(error, "Rene master event subject differs from actual CharAI+0x50 identity");
        }
        if (callback.vcb_flag && !(view.callback_flags & callback.vcb_flag)) return true;
        if (!call_alias(session, view, callback.name, error) ||
            !verify_after_callback(*record, session, view, error)) return false;
        if (actor.source_ai_pointers_v105() != fields) {
            return fail(error, "Rene master callback replaced the same CharAI pointer-field owner");
        }
        return true;
    }
    return fail(error, "Character event is outside the original Rene target/master callback domain");
}
} // namespace dh::foundation::companions
