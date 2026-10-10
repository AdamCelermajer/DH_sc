#include "source_companion_campaign_animation_v1.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace dh::foundation::companions { namespace {
bool fail(std::string& error, const char* message) { error = message; return false; }
bool scalar(const OriginalCampaignCommand& command, unsigned offset, std::uint32_t& value) {
    const auto found = command.scalars.find(offset);
    if (found == command.scalars.end()) return false;
    value = found->second;
    return true;
}
bool string_field(const OriginalCampaignCommand& command, unsigned offset, std::string& value) {
    const auto found = command.strings.find(offset);
    if (found == command.strings.end()) return false;
    value = found->second;
    return true;
}
const char* supported_slot_state(std::uint32_t slot) {
    // Script_PlayActorAnim transitions to Slot+2. Only the directly verified
    // Character FSM slots Idle=3 and Attack=4 are composed by this feature.
    if (slot == 1) return "Idle";
    if (slot == 2) return "Attack";
    return nullptr;
}
}

const CompanionCampaignAnimationNeedV1* CompanionCampaignAnimationBankV1::find(
    const OriginalCampaignCommand& command) const noexcept {
    const auto found = std::find_if(commands.begin(), commands.end(),
        [&](const auto& item) { return item.command == &command && item.resolved; });
    return found == commands.end() ? nullptr : &*found;
}

bool build_companion_campaign_animation_bank_v1(const OriginalMeleeBindings& bindings,
    const OriginalCampaignRuntime& campaign, const std::string& script_name,
    const std::vector<CompanionCampaignActorPlanV1>& actors,
    CompanionCampaignAnimationBankV1& output, std::string& error) {
    try {
        if (script_name.empty() || actors.empty())
            throw std::runtime_error("Companion campaign animation bank needs a script and actor plans");
        CompanionCampaignAnimationBankV1 next;
        next.campaign = &campaign;
        next.script_name = script_name;
        next.script_id = campaign.script_id(script_name, true);
        if (next.script_id < 0 || static_cast<std::size_t>(next.script_id) >= campaign.scripts().size())
            throw std::runtime_error("Loaded campaign has no requested companion animation script");
        const auto& script = campaign.scripts()[static_cast<std::size_t>(next.script_id)];
        if (script.name != script_name) throw std::runtime_error("Companion campaign script identity changed");
        std::map<std::string, const CompanionCampaignActorPlanV1*> byName;
        for (const auto& actor : actors) {
            if (actor.source_object_name.empty() || actor.profile_id.empty() || !actor.plan ||
                actor.plan->profileId != actor.profile_id ||
                !byName.emplace(actor.source_object_name, &actor).second)
                throw std::runtime_error("Companion animation actor name/profile plan is missing, duplicated, or mismatched");
        }
        for (std::size_t index = 0; index < script.commands.size(); ++index) {
            const auto& command = script.commands[index];
            if (command.kind != 45) continue;
            std::string receiver;
            if (!string_field(command, 24, receiver))
                throw std::runtime_error("PlayActorAnim receiver field is missing from loaded campaign script");
            const auto actor = byName.find(receiver);
            if (actor == byName.end()) continue;

            CompanionCampaignAnimationNeedV1 need;
            need.command_index = index;
            need.command = &command;
            need.source_object_name = receiver;
            need.profile_id = actor->second->profile_id;
            std::uint32_t wait = 0;
            if (!scalar(command, 8, need.animation_id) || !scalar(command, 12, need.next_id) ||
                !scalar(command, 16, need.slot) || !scalar(command, 28, wait) || wait > 1) {
                need.unsupported_reason = "PlayActorAnim Anim/Next/Slot/Wait fields are incomplete or invalid";
                next.commands.push_back(std::move(need));
                continue;
            }
            need.wait = wait != 0;
            const auto* stateName = supported_slot_state(need.slot);
            if (!stateName) {
                need.unsupported_reason = "Source Slot does not map to a supported generic Session state";
                next.commands.push_back(std::move(need));
                continue;
            }
            need.state = stateName;
            if (need.next_id != need.animation_id && need.next_id != UINT32_MAX) {
                need.unsupported_reason = "Distinct Next animation needs source slot-pair transition semantics";
                next.commands.push_back(std::move(need));
                continue;
            }
            std::vector<std::pair<const OriginalCombatSequencePlan*, const OriginalCombatPhase*>> matches;
            for (const auto& sequence : actor->second->plan->sequences) {
                if (sequence.state != need.state) continue;
                for (const auto& phase : sequence.phases)
                    if (phase.animationId == static_cast<std::int64_t>(need.animation_id) && phase.has_visual())
                        matches.emplace_back(&sequence, &phase);
            }
            if (matches.size() != 1) {
                need.unsupported_reason = matches.empty()
                    ? "Source Anim ID has no visual leaf in this same actor-profile Session plan"
                    : "Source Anim ID is ambiguous within this actor-profile Session plan";
                next.commands.push_back(std::move(need));
                continue;
            }
            const auto* sequence = matches.front().first;
            const auto* phase = matches.front().second;
            const auto* clip = bindings.find_clip(phase->sourceUri);
            if (!clip) {
                need.unsupported_reason = "Same-profile source clip metadata is unavailable";
                next.commands.push_back(std::move(need));
                continue;
            }
            if (!clip->markers.empty()) {
                need.unsupported_reason = "Generic Session leaf has authored events without a source event bridge";
                next.commands.push_back(std::move(need));
                continue;
            }
            need.looping = sequence->loop != 0;
            if (need.wait && need.looping) {
                need.unsupported_reason = "Wait=true cannot complete for the authored looping Session state";
                next.commands.push_back(std::move(need));
                continue;
            }
            need.clip_name = phase->clipName;
            need.selection.state = sequence->state;
            need.selection.variant = sequence->variant;
            need.selection.leafPath = phase->sourcePath;
            need.resolved = true;
            next.commands.push_back(std::move(need));
        }
        if (next.commands.empty())
            throw std::runtime_error("Requested campaign script has no PlayActorAnim records for supplied companion actors");
        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

SourceCompanionCampaignAnimationV1::SourceCompanionCampaignAnimationV1(
    OriginalCampaignRuntime& campaign, CombatSession& session,
    const CompanionCampaignAnimationBankV1& bank, CompanionCampaignAnimationServicesV1 services)
    : campaign_(campaign), session_(session), bank_(bank), services_(std::move(services)),
      session_lease_(session.actor_binding_lease()) {
    if (bank_.campaign != &campaign_ || bank_.script_id < 0 ||
        static_cast<std::size_t>(bank_.script_id) >= campaign_.scripts().size() ||
        campaign_.scripts()[static_cast<std::size_t>(bank_.script_id)].name != bank_.script_name)
        construction_error_ = "Companion animation bank is not bound to this loaded OriginalCampaignRuntime";
    else if (session_lease_.expired())
        construction_error_ = "Companion campaign animation requires an initialized CombatSession";
}

bool SourceCompanionCampaignAnimationV1::validate_session(std::string& error) const {
    const auto expected = session_lease_.lock();
    const auto current = session_.actor_binding_lease().lock();
    if (!expected || !current || expected != current) {
        error = "Companion campaign animation CombatSession lease expired or changed";
        return false;
    }
    error.clear();
    return true;
}

bool SourceCompanionCampaignAnimationV1::validate_active(const Active& active,
                                                           std::string& error) const {
    if (active.skipped) { error.clear(); return true; }
    const auto expected = active.session_lease.lock();
    const auto current = session_.actor_binding_lease().lock();
    if (!expected || !current || expected != current || active.session_binding_lease != expected ||
        active.actor_id == invalid_actor_id || active.module < 0 || active.source_object_name.empty() ||
        active.profile_id.empty() || !session_.actor(active.actor_id) || !active.pose_owner ||
        session_.retained_actor_pose(active.actor_id) != active.pose_owner) {
        error = "Companion campaign animation lost its same ActorId/Session/retained-pose binding";
        return false;
    }
    error.clear();
    return true;
}

bool SourceCompanionCampaignAnimationV1::command(CampaignCommandPhase phase,
    const OriginalCampaignCommand& command, int module, bool skip, bool& handled,
    bool& blocking, std::string& error) {
    handled = false;
    blocking = false;
    error.clear();
    const auto* need = bank_.find(command);
    if (!need) return true;
    handled = true;
    if (!construction_error_.empty()) return fail(error, construction_error_.c_str());
    if (!validate_session(error)) return false;
    if (phase == CampaignCommandPhase::update) return true;
    if (phase == CampaignCommandPhase::execute) {
        Active active;
        active.source_object_name = need->source_object_name;
        active.profile_id = need->profile_id;
        active.module = module;
        active.wait = need->wait;
        active.session_lease = session_lease_;
        if (skip) {
            active.skipped = true;
            active_[&command] = std::move(active);
            return true;
        }
        if (!services_.borrow_actor) return fail(error, "Same-session companion source object to ActorId resolver is unavailable");
        CompanionCampaignActorBorrowV1 borrowed;
        if (!services_.borrow_actor(need->source_object_name, need->profile_id, module,
                                    session_, borrowed, error)) return false;
        const auto currentLease = session_.actor_binding_lease().lock();
        if (borrowed.session != &session_ || borrowed.source_object_name != need->source_object_name ||
            borrowed.profile_id != need->profile_id || borrowed.module != module ||
            borrowed.actor_id == invalid_actor_id || !borrowed.session_binding_lease ||
            !currentLease || borrowed.session_binding_lease != currentLease ||
            !session_.actor(borrowed.actor_id) || !session_.retained_actor_pose(borrowed.actor_id))
            return fail(error, "Companion resolver did not map the exact authored name/profile/module to this CombatSession ActorId");

        const auto actorId = borrowed.actor_id;
        const auto actorName = borrowed.source_object_name;
        const auto profileId = borrowed.profile_id;
        const auto moduleId = borrowed.module;
        const auto sessionLease = session_lease_;
        CombatSessionStateAnimationServices stateServices;
        stateServices.finished = [session = &session_, sessionLease, actorId,
                                  actorName, profileId, moduleId](ActorId finishedActor, std::string& e) {
            const auto expected = sessionLease.lock();
            const auto current = session->actor_binding_lease().lock();
            if (!expected || !current || expected != current || finishedActor != actorId ||
                !session->actor(actorId) || !session->retained_actor_pose(actorId) ||
                actorName.empty() || profileId.empty() || moduleId < 0) {
                e = "Companion visual completion lost its same actor/session owner";
                return false;
            }
            e.clear();
            return true;
        };
        if (!session_.select_actor_state_leaf(actorId, need->selection, 1.0, false,
                                              std::move(stateServices), error)) {
            error = "Companion generic Session clip selection: " + error;
            return false;
        }
        active.actor_id = actorId;
        active.session_binding_lease = std::move(borrowed.session_binding_lease);
        active.pose_owner = session_.retained_actor_pose(actorId);
        active_[&command] = std::move(active);
        return true;
    }
    const auto active = active_.find(&command);
    if (active == active_.end()) return fail(error, "Companion animation Wait queried before its source command executed");
    if (!active->second.skipped && active->second.module != module)
        return fail(error, "Companion animation Wait changed the authored source module");
    if (!validate_active(active->second, error)) return false;
    if (active->second.wait && !active->second.skipped) {
        const auto* pose = session_.retained_actor_pose(active->second.actor_id);
        blocking = !pose->current_ended();
    }
    error.clear();
    return true;
}

} // namespace dh::foundation::companions
