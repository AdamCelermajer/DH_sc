#include "game_object_init_final_v1.hpp"

#include <array>
#include <cstring>
#include <exception>
#include <limits>

namespace dh2::game_object_init_final_v1 {
namespace {

thread_local bool active = false;

struct Range { std::uintptr_t begin{}, end{}; };

template<class T>
bool range(const T* value, Range& output) noexcept {
    const auto begin = reinterpret_cast<std::uintptr_t>(value);
    if (!value || begin % alignof(T) || begin >
            std::numeric_limits<std::uintptr_t>::max() - sizeof(T))
        return false;
    output = {begin, begin + sizeof(T)};
    return true;
}

bool overlaps(Range a, Range b) noexcept {
    return a.begin < b.end && b.begin < a.end;
}

} // namespace

std::int32_t light_set_id_from_name(std::string_view name) noexcept {
    static constexpr std::array<std::string_view, 4> source_names{
        "PlayerLight", "SceneLight", "CameraLight", "MonsterLight"};
    for (std::size_t i = 0; i < source_names.size(); ++i)
        if (name == source_names[i]) return static_cast<std::int32_t>(i);
    return 0;
}

Status initialize(character_runtime_factory_v1::Record& record,
                  Services services, Result* output, std::string& error) {
    using namespace character_runtime_factory_v1;
    Range record_range{}, result_range{}, error_range{};
    if (active) return Status::busy;
    if (!output || !range(&record, record_range) ||
        !range(output, result_range) || !range(&error, error_range) ||
        overlaps(record_range, result_range) ||
        overlaps(record_range, error_range) ||
        overlaps(result_range, error_range) ||
        !record.constructor.identity ||
        record.game_object.identity != record.constructor.identity ||
        record.character.identity != record.constructor.identity ||
        !record.object_registered) {
        error = "GameObject InitFinal requires one registered canonical Character Record";
        return Status::invalid_argument;
    }

    *output = {};
    output->character = record.constructor.identity;
    error.clear();
    active = true;
    struct Reset { ~Reset() { active = false; } } reset;
    const auto fail = [&](const char* message) {
        if (error.empty()) error = message;
        return Status::failed;
    };
    const auto call = [&](Operation operation, Reply& reply,
                          const char* name = nullptr) {
        output->last_operation = static_cast<std::uint32_t>(operation);
        ++output->entered_calls;
        reply = {};
        if (!services.invoke) {
            error = "Reached missing GameObject InitFinal provider";
            return false;
        }
        try {
            return services.invoke(services.context, record,
                                   {operation, name}, reply, error) == 0;
        } catch (...) {
            error = "GameObject InitFinal provider threw";
            return false;
        }
    };

    try {
        Reply reply{};
        if (!call(Operation::check_spawn_probability, reply))
            return fail("GameObject CheckSpawnProbability provider failed");
        record.game_object.spawn_probability_result_270 = reply.word;
        output->spawn_probability_result = reply.word;
        ++output->stores;
        if (reply.word >= record.game_object.spawn_probability_274) {
            output->decision = Decision::probability_rejected;
            return Status::complete;
        }
        if (record.game_object.deleted_81) {
            output->decision = Decision::deleted;
            return Status::complete;
        }

        if (record.game_object.init_final_enable_2ed &&
            !call(Operation::set_enabled_true, reply))
            return fail("GameObject InitFinal SetEnabled provider failed");

        if (!call(Operation::init_pf_object, reply))
            return fail("GameObject InitFinal PFWorld::InitObject provider failed");

        // Source copies the GameObject name into its +0x254 cached string.
        record.game_object.init_final_name_254 = record.game_object.name;
        ++output->stores;

        auto& visual = record.faery_visual;
        const bool has_visual = record.game_object.visual_object_2d8 != 0;
        if (has_visual) {
            if (record.game_object.visual_object_2d8 !=
                    reinterpret_cast<std::uintptr_t>(&visual) ||
                visual.character != record.constructor.identity ||
                !visual.asset || !visual.pose) {
                error = "GameObject InitFinal visual pointer has no ready same-Record owner";
                return Status::failed;
            }
            if (!call(Operation::sync_visual, reply))
                return fail("GameObject InitFinal VisualObject::Sync provider failed");

            visual.light_set_id_40 = light_set_id_from_name(
                record.game_object.init_final_light_set_name_288);
            ++output->stores;

            if (!call(Operation::find_target_node, reply, "target_node"))
                return fail("GameObject InitFinal target_node lookup provider failed");
            record.game_object.target_node_180 = reply.identity;
            ++output->stores;
        }

        if (!call(Operation::update_pf_object, reply))
            return fail("GameObject InitFinal UpdatePFObject provider failed");
        output->decision = Decision::initialized;
        return Status::complete;
    } catch (const std::exception& exception) {
        if (error.empty()) error = exception.what();
        return Status::failed;
    } catch (...) {
        return fail("GameObject InitFinal failed");
    }
}

} // namespace dh2::game_object_init_final_v1
