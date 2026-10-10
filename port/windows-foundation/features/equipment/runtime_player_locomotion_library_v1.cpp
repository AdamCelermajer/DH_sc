#include "runtime_player_locomotion_library_v1.hpp"

#include "../../actor_profiles.hpp"
#include "../../asset_catalog.hpp"
#include "../../content_paths.hpp"
#include "../../../game-data/animation_tables.hpp"
#include "../../../game-data/items.hpp"
#include "../../../script-runtime/script_constants.hpp"

#include <algorithm>
#include <charconv>
#include <set>
#include <stdexcept>

namespace dh::foundation::equipment_menu {
namespace {
using namespace dh2::data;

constexpr std::size_t kMaximumPyDataBytes = 16u * 1024u * 1024u;

std::vector<std::uint8_t> read_pydata(const AssetCatalog& assets,
                                      const char* filename) {
    // read_content owns the original Android flattened-data alias and applies
    // the exact candidate order inside this caller's AssetCatalog.
    return read_content(assets, std::string("data/pydata/") + filename);
}

Bytes bytes(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}

std::int32_t parse_animation_table(const std::string& text) {
    if (text.empty()) throw std::runtime_error("Actual profile has no animationTable ID");
    std::int32_t result{};
    const auto parsed = std::from_chars(text.data(), text.data() + text.size(), result);
    if (parsed.ec != std::errc{} || parsed.ptr != text.data() + text.size() || result < 0)
        throw std::runtime_error("Actual profile animationTable is not a nonnegative source ID");
    return result;
}

std::vector<std::pair<std::int32_t, std::int32_t>> equipment_pairs(
        std::int32_t main_hand, std::int32_t off_hand) {
    std::vector<std::pair<std::int32_t, std::int32_t>> result;
    std::set<std::pair<std::int32_t, std::int32_t>> seen;
    const std::pair<std::int32_t, std::int32_t> requested[] = {
        {main_hand, off_hand}, {main_hand, -1}, {-1, off_hand}, {-1, -1}
    };
    for (const auto& pair : requested)
        if (seen.insert(pair).second) result.push_back(pair);
    return result;
}

bool same_path(const AssetCatalog& assets, const std::string& left,
               const std::filesystem::path& right) {
    if (left.empty() || right.empty()) return false;
    // resolve_content_path already returned an absolute path within this
    // catalog. Resolve only the caller's configured path here.
    return assets.resolve(left) == right;
}

struct ConstantsOwner {
    dh2_script_constants* value{};
    ~ConstantsOwner() { if (value) dh2_script_constants_destroy(value); }
};

} // namespace

bool RuntimePlayerLocomotionLibraryV1::build(
        const AssetCatalog& actual_assets,
        const CharacterVisualConfig& same_character_visual,
        const RuntimePlayerLocomotionLibraryRequestV1& request,
        RuntimePlayerLocomotionLibraryV1& output, std::string& error) {
    try {
        if (request.profile_id.empty() || request.profile_library_uri.empty() || request.role.empty())
            throw std::runtime_error("Player locomotion library requires the caller profile table URI, profile ID, and role");
        if (request.main_hand_item_id < -1 || request.off_hand_item_id < -1)
            throw std::runtime_error("Player locomotion equipment IDs must be actual ItemTable IDs or -1");
        if (!request.max_programs || request.max_programs > hard_program_limit)
            throw std::runtime_error("Player locomotion request exceeds the hard program limit");
        if (same_character_visual.model_path.empty())
            throw std::runtime_error("Player locomotion clips require the same initialized-character model path");

        const auto pairs = equipment_pairs(request.main_hand_item_id, request.off_hand_item_id);
        if (pairs.size() > hard_program_limit || pairs.size() > request.max_programs)
            throw std::runtime_error("Player locomotion equipment/null combinations exceed the requested program bound");

        ActorProfileLibrary profiles;
        if (!profiles.load(actual_assets, request.profile_library_uri, error))
            throw std::runtime_error(error.empty() ? "Actual source actor profile table load failed" : error);
        const auto* profile = profiles.find(request.profile_id);
        if (!profile) throw std::runtime_error("Actual actor profile ID is absent: " + request.profile_id);
        const auto animation_table = parse_animation_table(profile->animation_table);
        const auto source_model = resolve_content_path(actual_assets, profile->model_uri,
                                                       profile->character_uri);
        if (!same_path(actual_assets, same_character_visual.model_path, source_model))
            throw std::runtime_error("CharacterVisualConfig model does not match the selected source profile");

        auto clip_names = read_pydata(actual_assets, "animations_dictionary_pyarraynames.bin");
        auto clip_values = read_pydata(actual_assets, "animations_dictionary_pyarray.bin");
        Dictionary clip_dictionary;
        if (!load_dictionary(bytes(clip_names), bytes(clip_values), clip_dictionary, error))
            throw std::runtime_error(error.empty() ? "Actual animation clip dictionary load failed" : error);

        const auto animation_records = read_pydata(actual_assets, "animations_pyarray.bin");
        const auto animation_names = read_pydata(actual_assets, "animations_pyarraynames.bin");
        const auto animation_fields = read_pydata(actual_assets, "animations_pystructnames.bin");
        AnimationTables animations;
        if (!load_animation_tables(bytes(animation_records), bytes(animation_names),
                                   bytes(animation_fields), clip_dictionary, animations, error))
            throw std::runtime_error(error.empty() ? "Actual AnimationTables load failed" : error);
        if (static_cast<std::size_t>(animation_table) >= animations.characters.size())
            throw std::runtime_error("Profile animationTable is outside the loaded source AnimationTables");

        const auto item_records = read_pydata(actual_assets, "loot_table_pyarray.bin");
        const auto item_names = read_pydata(actual_assets, "loot_table_pyarraynames.bin");
        const auto item_fields = read_pydata(actual_assets, "loot_table_pystructnames.bin");
        ItemTable items;
        if (!load_items(bytes(item_records), bytes(item_names), bytes(item_fields), items, error))
            throw std::runtime_error(error.empty() ? "Actual ItemTable load failed" : error);
        for (const auto& pair : pairs) {
            if ((pair.first >= 0 && !item(items, pair.first)) ||
                (pair.second >= 0 && !item(items, pair.second)))
                throw std::runtime_error("Requested equipment pair contains an ID outside the actual ItemTable");
        }

        const auto constants_bytes = read_pydata(actual_assets, "animations_pycst.bin");
        if (constants_bytes.empty() || constants_bytes.size() > kMaximumPyDataBytes ||
            constants_bytes.size() > UINT32_MAX)
            throw std::runtime_error("Actual animation PyDataConstants file is outside its source size bound");
        ConstantsOwner constants{dh2_script_constants_create()};
        if (!constants.value) throw std::runtime_error("PyDataConstants owner allocation failed");
        dh2_script_constants_reload reload{};
        if (dh2_script_constants_load(constants.value, constants_bytes.data(),
                static_cast<std::uint32_t>(constants_bytes.size()), &reload) != 0 ||
            reload.consumed != constants_bytes.size())
            throw std::runtime_error("Actual animations_pycst.bin did not complete its source reload");
        RuntimePlayerLocomotionConstantsV1 source_constants;
        const RuntimePlayerLocomotionConstantLookupV1 lookup = [table = constants.value](
                const char* group, const char* key, std::int32_t& value, std::string& message) {
            if (dh2_script_constants_get(table, group, key, &value) != 0) {
                message = std::string("Actual PyDataConstants lookup failed: ") + group + "." + key;
                return false;
            }
            return true;
        };
        if (!load_runtime_player_locomotion_constants_v1(lookup, source_constants, error))
            throw std::runtime_error(error.empty() ? "Actual source stance constants are incomplete" : error);

        RuntimePlayerLocomotionLibraryV1 next;
        next.profile_id_ = request.profile_id;
        next.role_ = request.role;
        next.base_visual_ = same_character_visual;
        next.entries_.reserve(pairs.size());
        for (const auto& pair : pairs) {
            RuntimePlayerLocomotionV1 resolved;
            if (!resolve_runtime_player_locomotion_v1(animation_table, items, pair.first,
                    pair.second, request.character_flag_1324, source_constants, animations,
                    clip_dictionary, resolved, error))
                throw std::runtime_error(error.empty() ? "Actual equipment/stance resolver failed" : error);
            RuntimePlayerLocomotionProgramV1 program;
            if (!build_runtime_player_locomotion_program_v1(actual_assets, resolved, animations,
                    clip_dictionary, same_character_visual, request.role, program, error))
                throw std::runtime_error(error.empty() ? "Actual locomotion program construction failed" : error);
            next.entries_.push_back({pair.first, pair.second, resolved.stance,
                std::make_shared<const RuntimePlayerLocomotionProgramV1>(std::move(program))});
        }

        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

bool RuntimePlayerLocomotionLibraryV1::select_exact(
        std::int32_t main_hand_item_id, std::int32_t off_hand_item_id,
        RuntimePlayerLocomotionSelectionV1& output, std::string& error) const {
    const auto found = std::find_if(entries_.begin(), entries_.end(), [&](const Entry& entry) {
        return entry.main_hand_item_id == main_hand_item_id &&
               entry.off_hand_item_id == off_hand_item_id;
    });
    if (found == entries_.end() || !found->program) {
        error = "Requested exact current equipment pair was not prepared by this source library";
        return false;
    }
    RuntimePlayerLocomotionSelectionV1 next;
    next.main_hand_item_id = found->main_hand_item_id;
    next.off_hand_item_id = found->off_hand_item_id;
    next.stance = found->stance;
    next.program = found->program;
    output = std::move(next);
    error.clear();
    return true;
}

bool RuntimePlayerLocomotionLibraryV1::select_ordinary_one_hand(
        std::int32_t main_hand_item_id, RuntimePlayerLocomotionSelectionV1& output,
        std::string& error) const {
    if (main_hand_item_id < 0) {
        error = "Ordinary one-hand selection requires an actual nonnegative main-hand ItemTable ID";
        return false;
    }
    RuntimePlayerLocomotionSelectionV1 next;
    if (!select_exact(main_hand_item_id, -1, next, error)) return false;
    if (next.stance != 0) {
        error = "Actual source equipment resolves to a non-ordinary stance; ordinary one-hand selection rejected";
        return false;
    }
    output = std::move(next);
    error.clear();
    return true;
}

bool RuntimePlayerLocomotionLibraryV1::merge_named_clips(
        CharacterVisualConfig& same_character_visual, std::string& error) const {
    try {
        if (entries_.empty() || same_character_visual.model_path != base_visual_.model_path)
            throw std::runtime_error("Named locomotion clips require the same source character visual/configuration");
        CharacterVisualConfig next = same_character_visual;
        std::map<std::string, std::string> aliases;
        for (const auto& clip : next.clips) {
            if (clip.first.empty() || clip.second.empty() || !aliases.emplace(clip.first, clip.second).second)
                throw std::runtime_error("Same CharacterVisualConfig has an empty or duplicate clip alias");
        }
        for (const auto& entry : entries_) {
            if (!entry.program) throw std::runtime_error("Locomotion library contains an empty program lease");
            for (const auto& receipt : entry.program->named_clips) {
                if (receipt.named_alias.empty() || receipt.resolved_path.empty())
                    throw std::runtime_error("Source locomotion named clip receipt is incomplete");
                const auto found = aliases.find(receipt.named_alias);
                if (found != aliases.end()) {
                    if (found->second != receipt.resolved_path)
                        throw std::runtime_error("Named locomotion clip collides with a different visual resource");
                    continue;
                }
                aliases.emplace(receipt.named_alias, receipt.resolved_path);
                next.clips.emplace_back(receipt.named_alias, receipt.resolved_path);
            }
        }
        same_character_visual = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

} // namespace dh::foundation::equipment_menu
