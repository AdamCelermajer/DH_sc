#include "source_companion_animation_bank_v1.hpp"
#include <algorithm>
#include <limits>
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

std::string alias_for(std::uint32_t id, const std::string& name) {
    return "source-animdict-" + std::to_string(id) + "-" + name;
}

bool is_dictionary_path(const std::string& path) {
    return path.size() > 5 && path.rfind("data/", 0) == 0 &&
        path.size() >= 5 && path.substr(path.size() - 5) == ".bdae";
}
}

const SourceCompanionAnimationCommandV1* SourceCompanionAnimationBankV1::find(
    const OriginalCampaignCommand& command) const noexcept {
    const auto found = std::find_if(commands.begin(), commands.end(),
        [&](const auto& item) { return item.command == &command; });
    return found == commands.end() ? nullptr : &*found;
}

bool build_source_companion_animation_bank_v1(
    const OriginalCampaignRuntime& campaign, const std::string& script_name,
    const dh2::data::Dictionary& dictionary, const dh2::data::AnimationTables& tables,
    const AssetCatalog& assets, const std::vector<SourceCompanionAnimationActorV1>& actors,
    SourceCompanionAnimationBankV1& output, std::string& error) {
    try {
        if (script_name.empty() || actors.empty())
            throw std::runtime_error("Source companion animation bank needs a script and actor owners");
        if (dictionary.names.empty() || dictionary.names.size() != dictionary.values.size())
            throw std::runtime_error("Exact source AnimDict is unavailable or malformed");

        SourceCompanionAnimationBankV1 next;
        next.campaign = &campaign;
        next.script_name = script_name;
        next.script_id = campaign.script_id(script_name, true);
        if (next.script_id < 0 || static_cast<std::size_t>(next.script_id) >= campaign.scripts().size())
            throw std::runtime_error("Loaded campaign has no requested companion animation script");
        const auto& script = campaign.scripts()[static_cast<std::size_t>(next.script_id)];
        if (script.name != script_name)
            throw std::runtime_error("Loaded companion campaign script identity changed");

        std::map<std::string, const SourceCompanionAnimationActorV1*> by_name;
        std::map<CombatSessionProfile*, std::vector<std::pair<std::string, std::string>>> staged_clips;
        for (const auto& actor : actors) {
            if (actor.source_object_name.empty() || actor.profile_id.empty() || !actor.visual_plan ||
                !actor.session_profile || actor.visual_plan->profileId != actor.profile_id ||
                !actor.session_profile->animationOnly ||
                !by_name.emplace(actor.source_object_name, &actor).second)
                throw std::runtime_error("Source companion name/profile must uniquely own an animationOnly Session profile");
            staged_clips.emplace(actor.session_profile, actor.session_profile->sourceAnimationClips);
        }

        for (std::size_t index = 0; index < script.commands.size(); ++index) {
            const auto& command = script.commands[index];
            if (command.kind != 45) continue; // Script_PlayActorAnim
            std::string receiver;
            if (!string_field(command, 24, receiver))
                throw std::runtime_error("PlayActorAnim receiver missing from loaded campaign source");
            const auto actor_it = by_name.find(receiver);
            if (actor_it == by_name.end()) continue;

            SourceCompanionAnimationCommandV1 item;
            item.command_index = index;
            item.command = &command;
            item.source_object_name = receiver;
            item.profile_id = actor_it->second->profile_id;
            std::uint32_t wait = 0;
            if (!scalar(command, 8, item.animation_id) || !scalar(command, 12, item.next_id) ||
                !scalar(command, 16, item.slot) || !scalar(command, 20, item.offset20) ||
                !scalar(command, 28, wait) || wait > 1) {
                item.unsupported_reason = "PlayActorAnim fields are incomplete or invalid";
                next.commands.push_back(std::move(item));
                continue;
            }
            item.wait = wait != 0;
            if (item.animation_id >= dictionary.values.size()) {
                item.unsupported_reason = "Anim ID is outside the exact source animations_dictionary";
                next.commands.push_back(std::move(item));
                continue;
            }
            item.dictionary_name = dictionary.names[item.animation_id];
            item.dictionary_path = dictionary.values[item.animation_id];
            if (item.dictionary_name.empty() || !is_dictionary_path(item.dictionary_path)) {
                item.unsupported_reason = "AnimDict row has no valid BDAE name/path";
                next.commands.push_back(std::move(item));
                continue;
            }
            item.clip_alias = alias_for(item.animation_id, item.dictionary_name);
            try {
                (void)assets.resolve(item.dictionary_path);
                item.clip_available = true;
            } catch (...) {
                item.unsupported_reason = "Exact AnimDict BDAE is absent from the selected local asset cache";
            }
            if (item.clip_available) {
                auto& clips = staged_clips.at(actor_it->second->session_profile);
                const auto existing = std::find_if(clips.begin(), clips.end(), [&](const auto& clip) {
                    return clip.first == item.clip_alias;
                });
                if (existing != clips.end() && existing->second != item.dictionary_path)
                    throw std::runtime_error("AnimDict alias conflicts with an existing animationOnly source clip");
                if (existing == clips.end()) clips.emplace_back(item.clip_alias, item.dictionary_path);
                item.preloaded = true;
            }

            // Preserve any real AnimationTables row that references this ID;
            // the command does not inherit such metadata automatically.
            for (std::size_t seq = 0; seq < tables.sequences.size(); ++seq) {
                const auto& source_sequence = tables.sequences[seq];
                for (std::size_t step = 0; step < source_sequence.steps.size(); ++step) {
                    const auto& source_step = source_sequence.steps[step];
                    if (!source_step.redir && source_step.anim ==
                        static_cast<std::int32_t>(item.animation_id)) {
                        item.animation_table_references.push_back({seq, step,
                            source_sequence.loop, source_sequence.type, source_step.speed, false});
                    }
                }
            }
            next.commands.push_back(std::move(item));
        }
        if (next.commands.empty())
            throw std::runtime_error("Requested script has no PlayActorAnim records for supplied source actor owners");
        for (auto& staged : staged_clips)
            staged.first->sourceAnimationClips = std::move(staged.second);
        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        return fail(error, exception.what());
    }
}

} // namespace dh::foundation::companions
