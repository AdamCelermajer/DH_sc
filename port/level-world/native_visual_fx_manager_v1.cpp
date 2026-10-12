#include "native_visual_fx_manager_v1.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <new>
#include <stdexcept>
#include <utility>

namespace dh2::native_visual_fx_manager_v1 {

bool exact_missing_effect_dict_asset(std::string_view diagnostic,
                                     std::string_view source_path) noexcept {
    constexpr std::string_view prefix = "Bundled asset missing: actors/";
    return !source_path.empty() && diagnostic.size() == prefix.size() + source_path.size() &&
           diagnostic.substr(0, prefix.size()) == prefix &&
           diagnostic.substr(prefix.size()) == source_path;
}

namespace {
constexpr std::size_t kMaxRows = 10000;
constexpr std::size_t kMaxString = 4096;
constexpr std::size_t kPoolLimit = 6; // _GetAnimFX: reject when count > 5.
constexpr std::size_t kMaxNestedSetDepth = 64;

class Reader {
public:
    explicit Reader(Bytes bytes) : bytes_(bytes) {
        if (!bytes.data || bytes.size < 4 || bytes.size > 64u * 1024u * 1024u)
            throw std::runtime_error("Visual FX source table is missing or outside size limit");
    }
    std::size_t offset() const noexcept { return at_; }
    std::size_t remaining() const noexcept { return bytes_.size - at_; }
    std::uint8_t byte() {
        require(1); return bytes_.data[at_++];
    }
    bool boolean() {
        const auto value = byte();
        if (value > 1) throw std::runtime_error("Visual FX table boolean is invalid");
        return value != 0;
    }
    std::uint32_t word() {
        require(4); const auto* p = bytes_.data + at_; at_ += 4;
        return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
               (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
    }
    std::int32_t integer() {
        const auto bits = word(); std::int32_t out{};
        std::memcpy(&out, &bits, sizeof(out)); return out;
    }
    float real() {
        const auto bits = word(); float out{};
        std::memcpy(&out, &bits, sizeof(out));
        if (!std::isfinite(out) || out < 0.0f || out > 100.0f)
            throw std::runtime_error("Visual FX source speed is invalid");
        return out;
    }
    std::uint32_t count(std::uint32_t limit = static_cast<std::uint32_t>(kMaxRows)) {
        const auto value = word();
        if (value > limit) throw std::runtime_error("Visual FX source count is outside limit");
        return value;
    }
    std::string string(bool allow_empty = false) {
        const auto size = count(static_cast<std::uint32_t>(kMaxString));
        if ((!allow_empty && size == 0) || size > remaining())
            throw std::runtime_error("Visual FX source string length is invalid");
        std::string out(reinterpret_cast<const char*>(bytes_.data + at_), size);
        at_ += size;
        if (std::any_of(out.begin(), out.end(), [](unsigned char c) {
                return c < 32 || c > 126;
            })) throw std::runtime_error("Visual FX source string is not printable ASCII");
        return out;
    }
    std::vector<std::string> strings() {
        const auto size = count(); std::vector<std::string> out; out.reserve(size);
        for (std::uint32_t i = 0; i < size; ++i) out.push_back(string());
        return out;
    }
    void require(std::size_t count) const {
        if (at_ > bytes_.size || count > bytes_.size - at_)
            throw std::runtime_error("Visual FX source table is truncated");
    }
private:
    Bytes bytes_{};
    std::size_t at_{};
};

void require_schema(Bytes bytes, const char* const* expected, std::size_t expected_count) {
    Reader reader(bytes);
    auto compare = [&](const char* const* section, std::size_t count) {
        const auto names = reader.strings();
        if (names.size() != count || !std::equal(names.begin(), names.end(), section))
            throw std::runtime_error("Visual FX source field schema differs from the recovered layout");
    };
    compare(expected, expected_count);
    // effects_pystructnames.bin contains the schemas for all four source
    // effect-related arrays, not just the AnimatedEffectTable row above.
    static const char* const animation_fields[] = {"ForceCache", "LoopAFX", "Steps", "Type"};
    static const char* const effect_fields[] = {
        "BloodDeathEffect", "BloodEffect", "FootprintEffect", "SwooshEffect", "TriggerFloorFX"
    };
    static const char* const footstep_fields[] = {"Effect", "Floortype", "RunSound", "WalkSound"};
    compare(animation_fields, sizeof(animation_fields) / sizeof(animation_fields[0]));
    compare(effect_fields, sizeof(effect_fields) / sizeof(effect_fields[0]));
    compare(footstep_fields, sizeof(footstep_fields) / sizeof(footstep_fields[0]));
    if (reader.remaining() != 0)
        throw std::runtime_error("Unexpected suffix in Visual FX field schemas");
    if (expected_count != 12)
        throw std::runtime_error("Visual FX source field schema differs from the recovered layout");
}

const EffectSequence* lookup(const Catalog* catalog, std::int32_t id) noexcept {
    if (!catalog || id < 0 || static_cast<std::size_t>(id) >= catalog->sequences.size())
        return nullptr;
    const auto& sequence = catalog->sequences[static_cast<std::size_t>(id)];
    return sequence.row == static_cast<std::uint32_t>(id) ? &sequence : nullptr;
}
}

struct Owner::SetData {
    std::int32_t effect_table_id{-1};
    std::size_t step_index{};
    std::size_t depth{};
    std::int32_t loops_remaining{};
    std::int32_t source_loop_count{};
    std::array<float, 3> position{};
    std::array<float, 3> origin{};
    void* source_game_object{};
    // PlayAnimFX's GameObject argument (AnimatedFX+0x28) for the attached
    // overload; null for the Point3D overload.
    void* target_character{};
    void* target_object{};
    SetData* parent{};
    bool end_of_loop_pending{};
    bool completed{};
};

Owner::Owner(const Catalog* catalog) noexcept : catalog_(catalog) {}

bool load_catalog(Bytes effect_array, Bytes names, Bytes fields,
                  Bytes dictionary, Bytes dictionary_names,
                  Catalog& output, std::string& error) {
    output = {}; error.clear();
    try {
        static const char* const expected_fields[] = {
            "File", "ForceCancel", "Loop", "OrientOnce", "OrientWithAnchor",
            "PlayTime", "PoolSize", "Redir", "ScaleWithAnchor", "SelfIllum",
            "Speed", "SubObject"
        };
        require_schema(fields, expected_fields, sizeof(expected_fields) / sizeof(expected_fields[0]));

        Reader name_reader(names);
        auto sequence_names = name_reader.strings();
        auto character_effect_names = name_reader.strings();
        auto footstep_names = name_reader.strings();
        if (name_reader.remaining() != 0 || character_effect_names.empty() ||
            footstep_names.empty())
            throw std::runtime_error("Visual FX source name sections are incomplete");
        if (sequence_names.empty() || sequence_names.size() > kMaxRows)
            throw std::runtime_error("Visual FX sequence-name section is empty or too large");

        Reader dictionary_name_reader(dictionary_names);
        auto names_for_dictionary = dictionary_name_reader.strings();
        if (dictionary_name_reader.remaining() != 0)
            throw std::runtime_error("Unexpected suffix in EffectDict name section");
        Reader dictionary_reader(dictionary);
        auto paths = dictionary_reader.strings();
        if (dictionary_reader.remaining() != 0 || paths.size() != names_for_dictionary.size() || paths.empty())
            throw std::runtime_error("EffectDict names and path records differ");
        for (const auto& path : paths) {
            if (path.rfind("data/3D/", 0) != 0 || path.size() > kMaxString)
                throw std::runtime_error("EffectDict path is not a source 3D resource path");
        }

        Reader data(effect_array);
        const auto sequence_count = data.count();
        if (sequence_count == 0 || sequence_count != sequence_names.size())
            throw std::runtime_error("AnimatedEffectTable row/name counts differ");
        Catalog next;
        next.effect_names = std::move(sequence_names);
        next.effect_paths = std::move(paths);
        next.dictionary_names = std::move(names_for_dictionary);
        next.sequences.reserve(sequence_count);
        for (std::uint32_t row = 0; row < sequence_count; ++row) {
            EffectSequence sequence;
            sequence.row = row;
            sequence.name = next.effect_names[row];
            sequence.template_flag = data.boolean();
            sequence.loop = data.integer();
            const auto step_count = data.count();
            if (step_count == 0)
                throw std::runtime_error("AnimatedEffectTable sequence has no source steps");
            sequence.steps.reserve(step_count);
            for (std::uint32_t i = 0; i < step_count; ++i) {
                EffectStep step;
                step.effect_dict_id = data.integer();
                step.force_cancel = data.boolean();
                step.loop = data.integer();
                step.orient_once = data.boolean();
                step.orient_with_anchor = data.boolean();
                step.play_time = data.integer();
                step.pool_size = data.integer();
                step.redirect = data.integer();
                step.scale_with_anchor = data.boolean();
                step.self_illum = data.boolean();
                step.speed = data.real();
                step.sub_object = data.string(true);
                if (step.effect_dict_id < -1 ||
                    (step.effect_dict_id >= 0 &&
                     static_cast<std::size_t>(step.effect_dict_id) >= next.effect_paths.size()) ||
                    (step.redirect != 0 && step.redirect != 1))
                    throw std::runtime_error("AnimatedEffectTable step reference is invalid");
                sequence.steps.push_back(std::move(step));
            }
            sequence.type = data.integer();
            next.sequences.push_back(std::move(sequence));
        }
        next.effect_array_consumed = data.offset();
        next.effect_array_suffix = data.remaining();

        // The source CharEffectTable follows AnimatedEffectTable in the same
        // pyarray stream. Each serialized row is four effect-table indices
        // and the TriggerFloorFX byte; the runtime's 24-byte row includes
        // its leading object word and alignment padding.
        const auto character_effect_count = data.count();
        if (character_effect_count != character_effect_names.size())
            throw std::runtime_error("CharEffectTable row/name counts differ");
        next.character_effects.reserve(character_effect_count);
        for (std::uint32_t row = 0; row < character_effect_count; ++row) {
            CharacterEffect effect;
            effect.row = row;
            effect.name = character_effect_names[row];
            effect.blood_death_effect = data.integer();
            effect.blood_effect = data.integer();
            effect.footprint_effect = data.integer();
            effect.swoosh_effect = data.integer();
            effect.trigger_floor_fx = data.boolean();
            const auto valid_effect_id = [&](std::int32_t id) {
                return id >= -1 && (id < 0 || static_cast<std::size_t>(id) < next.sequences.size());
            };
            if (!valid_effect_id(effect.blood_death_effect) ||
                !valid_effect_id(effect.blood_effect) ||
                !valid_effect_id(effect.footprint_effect) ||
                !valid_effect_id(effect.swoosh_effect))
                throw std::runtime_error("CharEffectTable effect reference is invalid");
            next.character_effects.push_back(std::move(effect));
        }
        next.character_effect_table_consumed = data.offset();
        // FootstepEffectTable is the final section of this same effects array
        // (the third names section is its seven exact source member names).
        const auto footstep_count = data.count();
        if (footstep_count != footstep_names.size())
            throw std::runtime_error("FootstepEffectTable row/name counts differ");
        next.footstep_effects.reserve(footstep_count);
        for (std::uint32_t row = 0; row < footstep_count; ++row) {
            FootstepEffect effect;
            effect.row = row;
            effect.name = footstep_names[row];
            effect.effect = data.integer();
            effect.floor_type = data.string(true);
            const auto valid_effect_id = [&](std::int32_t id) {
                return id >= -1 && (id < 0 || static_cast<std::size_t>(id) < next.sequences.size());
            };
            if (!valid_effect_id(effect.effect))
                throw std::runtime_error("FootstepEffectTable effect reference is invalid");
            const auto sound_rows = [&](std::vector<std::int32_t>& output) {
                const auto count = data.count();
                output.reserve(count);
                for (std::uint32_t i = 0; i < count; ++i) output.push_back(data.integer());
            };
            sound_rows(effect.run_sound);
            sound_rows(effect.walk_sound);
            next.footstep_effects.push_back(std::move(effect));
        }
        next.remaining_effect_array_suffix = data.remaining();
        if (next.remaining_effect_array_suffix != 0)
            throw std::runtime_error("Unexpected bytes after FootstepEffectTable");
        output = std::move(next);
        return true;
    } catch (const std::exception& exception) {
        error = exception.what(); output = {}; return false;
    }
}

Owner::~Owner() { Flush(); }

Owner::Slot* Owner::find(void* instance) noexcept {
    const auto it = std::find_if(slots_.begin(), slots_.end(),
        [instance](const Slot& slot) { return slot.instance == instance; });
    return it == slots_.end() ? nullptr : &*it;
}
const Owner::Slot* Owner::find(void* instance) const noexcept {
    const auto it = std::find_if(slots_.begin(), slots_.end(),
        [instance](const Slot& slot) { return slot.instance == instance; });
    return it == slots_.end() ? nullptr : &*it;
}

const EffectSequence* Owner::sequence(std::int32_t id) const noexcept {
    return lookup(catalog_, id);
}

const FootstepEffect* Owner::footstep_effect(std::string_view floor_type) const noexcept {
    if (!catalog_ || floor_type.empty()) return nullptr;
    const auto found = std::find_if(catalog_->footstep_effects.begin(),
        catalog_->footstep_effects.end(), [floor_type](const FootstepEffect& row) {
            return row.floor_type == floor_type;
        });
    return found == catalog_->footstep_effects.end() ? nullptr : &*found;
}

Status Owner::GrabAnimFX(std::int32_t id, void* source_game_object,
                         void** output, std::string& error) {
    error.clear();
    if (output) *output = nullptr;
    const auto* selected = sequence(id);
    if (!output || id < 0) {
        error = "GrabAnimFX request is missing its source row or output";
        return Status::invalid_request;
    }
    if (!catalog_ || !selected) {
        error = "AnimatedEffectTable catalog is not bound or the source row is absent";
        return Status::catalog_missing;
    }
    if (selected->type != 1 && selected->type != 2 && selected->steps.size() > 1) {
        error = "AnimatedEffectTable sequence type has no source-backed progression provider";
        return Status::unsupported_sequence;
    }
    if (!backend_.create || !backend_.destroy || !backend_.start_step ||
        !backend_.reset_for_pool) {
        error = "VisualFXManager requires the shared AnimatedFX/scene backend";
        return Status::backend_missing;
    }
    const auto status = start_step_instance(id, 0, 0, selected->loop, true,
        selected->type == 2, source_game_object, nullptr, nullptr, nullptr,
        output, error);
    if (status == Status::source_no_launch) {
        error.clear();
        return Status::ok;
    }
    return status;
}

Status Owner::RegisterFXSetToLoad(std::int32_t id, std::string& error) {
    error.clear();
    if (!catalog_) {
        error = "VisualFXManager cannot register effect sets without its source catalog";
        return Status::catalog_missing;
    }
    std::vector<std::int32_t> recursion_stack;
    try {
        const auto status = register_fx_set_recursive(id, recursion_stack, error);
        return status;
    } catch (const std::bad_alloc&) {
        error = "VisualFXManager effect-set registration allocation failed";
        return Status::allocation_failed;
    }
}

Status Owner::PlayAnimFXSetByName(std::string_view source_name,
                                  const std::array<float, 3>& position,
                                  void* source_game_object,
                                  bool& matched, bool& launched,
                                  std::string& error) {
    error.clear();
    matched = false;
    launched = false;
    if (!catalog_) {
        error = "VisualFXManager cannot play a named set without its source catalog";
        return Status::catalog_missing;
    }
    if (source_name.empty()) {
        error = "PlayAnimFXSet source name is empty";
        return Status::invalid_request;
    }
    const auto it = std::find_if(catalog_->sequences.begin(), catalog_->sequences.end(),
        [source_name](const EffectSequence& row) { return row.name == source_name; });
    if (it == catalog_->sequences.end()) return Status::ok; // source lookup miss: no-op
    return PlayAnimFXSet(static_cast<std::int32_t>(it->row), position,
                         source_game_object, matched, launched, error);
}

Status Owner::PlayAnimFXSetAttached(std::int32_t id, void* source_game_object,
                                    void* target_character, void* target_object,
                                    bool& matched, bool& launched,
                                    std::string& error) {
    error.clear();
    matched = false;
    launched = false;
    if (!catalog_) {
        error = "VisualFXManager cannot play a source set without its catalog";
        return Status::catalog_missing;
    }
    if ((target_character != nullptr) == (target_object != nullptr)) {
        error = "PlayAnimFXSet attach requires exactly one bound GameObject projection";
        return Status::invalid_request;
    }
    if (!sequence(id)) return Status::ok; // source integer overload returns unchanged
    matched = true;
    static constexpr std::array<float, 3> source_origin{};
    try {
        // GameObject overload: PlayAnimFXSet(id, Vec3f_Origin, object, data).
        return begin_anim_fx_set(id, source_origin, source_origin,
                                 source_game_object, nullptr, 0, launched,
                                 error, target_character, target_object);
    } catch (const std::bad_alloc&) {
        error = "PlayAnimFXSet source data allocation failed";
        return Status::allocation_failed;
    }
}

Status Owner::PlayAnimFXSet(std::int32_t id,
                            const std::array<float, 3>& position,
                            void* source_game_object,
                            bool& matched, bool& launched,
                            std::string& error) {
    error.clear();
    matched = false;
    launched = false;
    if (!catalog_) {
        error = "VisualFXManager cannot play a source set without its catalog";
        return Status::catalog_missing;
    }
    if (!sequence(id)) return Status::ok; // source integer overload returns unchanged
    matched = true;
    static constexpr std::array<float, 3> source_origin{};
    try {
        return begin_anim_fx_set(id, position,
                                 source_origin, source_game_object, nullptr, 0,
                                 launched, error);
    } catch (const std::bad_alloc&) {
        error = "PlayAnimFXSet source data allocation failed";
        return Status::allocation_failed;
    }
}

Status Owner::register_fx_set_recursive(std::int32_t id,
                                        std::vector<std::int32_t>& recursion_stack,
                                        std::string& error) {
    if (id < 0 || !sequence(id)) {
        error = "RegisterFXSetToLoad references an absent AnimatedEffectTable row";
        return Status::invalid_request;
    }
    if (std::find(recursion_stack.begin(), recursion_stack.end(), id) != recursion_stack.end()) {
        error = "RegisterFXSetToLoad encountered a recursive source set cycle";
        return Status::invalid_request;
    }
    if (std::find(registered_effect_sets_.begin(), registered_effect_sets_.end(), id) !=
        registered_effect_sets_.end())
        return Status::ok;
    registered_effect_sets_.push_back(id);
    recursion_stack.push_back(id);
    const auto* selected = sequence(id);
    for (const auto& step : selected->steps) {
        if (step.effect_dict_id < 0) continue;
        if (step.redirect == 1) {
            const auto status = register_fx_set_recursive(step.effect_dict_id,
                                                           recursion_stack, error);
            if (status != Status::ok) {
                recursion_stack.pop_back();
                return status;
            }
        } else if (std::find(registered_effect_dicts_.begin(),
                             registered_effect_dicts_.end(), step.effect_dict_id) ==
                   registered_effect_dicts_.end()) {
            registered_effect_dicts_.push_back(step.effect_dict_id);
        }
    }
    recursion_stack.pop_back();
    return Status::ok;
}

Status Owner::start_step_instance(std::int32_t id,
                                  std::size_t pool_step_index,
                                  std::size_t playback_step_index,
                                  std::int32_t sequence_loops_remaining,
                                  bool grab_entry,
                                  bool random_set_entry,
                                  void* source_game_object,
                                  void* target_character,
                                  void* target_object,
                                  void* animator_character,
                                  void** output, std::string& error,
                                  const SetData* set_data) {
    error.clear();
    if (output) *output = nullptr;
    const auto* selected = sequence(id);
    if (!output || !selected || pool_step_index >= selected->steps.size() ||
        playback_step_index >= selected->steps.size()) {
        error = "AnimatedEffectTable step request is outside the source sequence";
        return selected ? Status::invalid_request : Status::catalog_missing;
    }
    const bool has_start = set_data ? backend_.start_anim_fx_set_step != nullptr :
                                      backend_.start_step != nullptr;
    if (!catalog_ || !backend_.create || !backend_.destroy || !has_start ||
        !backend_.reset_for_pool) {
        error = "VisualFXManager requires the shared AnimatedFX/scene backend";
        return catalog_ ? Status::backend_missing : Status::catalog_missing;
    }
    // IDA: GrabAnimFX and PlayAnimFXStep both reach _GetAnimFX for the
    // selected step's EffectDict row. All steps share this one manager pool.
    // GrabAnimFX obtains its AnimatedFX from step zero's EffectDict before
    // choosing a type-2 AnimFXSetData variant. Later PlayAnimFXStep calls use
    // that selected step's own dictionary. Keep pool and playback indices
    // distinct so the initial acquisition follows the source order.
    const auto dict_id = selected->steps[pool_step_index].effect_dict_id;
    if (dict_id < 0 || static_cast<std::size_t>(dict_id) >= catalog_->effect_paths.size()) {
        error = "AnimatedEffectTable step has no loadable EffectDict row";
        return Status::invalid_request;
    }
    const auto& path = catalog_->effect_paths[static_cast<std::size_t>(dict_id)];
    Slot* chosen = nullptr;
    for (auto& slot : slots_) {
        if (slot.effect_dict_id == dict_id && !slot.active) { chosen = &slot; break; }
    }
    if (!chosen) {
        std::size_t allocated = 0;
        for (const auto& slot : slots_) if (slot.effect_dict_id == dict_id) ++allocated;
        if (allocated >= kPoolLimit) {
            error = "VisualFXManager source pool limit of six instances is reached";
            return Status::pool_exhausted;
        }
        void* instance = nullptr;
        const auto create_result = backend_.create(
            backend_.context, dict_id, path, &instance, error);
        if (create_result == CreateResult::source_no_launch) {
            if (instance) {
                backend_.destroy(backend_.context, instance);
                error = "AnimatedFX create returned an instance with source no-launch";
                return Status::load_failed;
            }
            error.clear();
            return Status::source_no_launch;
        }
        if (create_result != CreateResult::created || !instance) {
            if (error.empty()) error = "AnimatedFX backend did not produce a loaded instance";
            return Status::load_failed;
        }
        try {
            Slot slot{};
            slot.instance = instance;
            slot.effect_dict_id = dict_id;
            slot.effect_table_id = id;
            slots_.push_back(slot);
            chosen = &slots_.back();
        } catch (...) {
            backend_.destroy(backend_.context, instance);
            error = "VisualFXManager pool allocation failed";
            return Status::allocation_failed;
        }
    }
    if (random_set_entry) {
        if (!backend_.random_below) {
            error = "Type-2 AnimatedEffectTable set requires the canonical source RNG provider";
            return Status::backend_missing;
        }
        std::uint32_t choice = 0;
        if (!backend_.random_below(backend_.context,
                static_cast<std::uint32_t>(selected->steps.size()), choice, error)) {
            if (error.empty()) error = "Canonical source RNG rejected type-2 set selection";
            return Status::lifecycle_failed;
        }
        if (choice >= selected->steps.size()) {
            error = "Canonical source RNG returned a type-2 index outside the source set";
            return Status::lifecycle_failed;
        }
        playback_step_index = choice;
    }
    bool start_ok = false;
    if (set_data) {
        const auto& step = selected->steps[playback_step_index];
        std::int32_t loop_count = step.loop;
        if (selected->type != 1) {
            if (step.loop == -1 || set_data->source_loop_count == -1) {
                loop_count = -1;
            } else if (step.loop != 0) {
                const auto combined = static_cast<std::int64_t>(step.loop) *
                                      static_cast<std::int64_t>(set_data->source_loop_count);
                if (combined < std::numeric_limits<std::int32_t>::min() ||
                    combined > std::numeric_limits<std::int32_t>::max()) {
                    error = "PlayAnimFXSet effective loop count overflows its source field";
                    return Status::invalid_request;
                }
                loop_count = set_data->source_loop_count != 0 ? static_cast<std::int32_t>(combined) :
                                                   step.loop;
            } else {
                loop_count = set_data->source_loop_count;
            }
        }
        const AnimFXSetPlayback playback{
            id, playback_step_index, set_data->position, set_data->origin,
            step.orient_once, step.orient_with_anchor, step.self_illum,
            step.scale_with_anchor, step.speed, step.play_time, loop_count,
            set_data->source_game_object, set_data, set_data->parent
        };
        start_ok = backend_.start_anim_fx_set_step(
            backend_.context, chosen->instance, *selected, playback, error);
    } else {
        start_ok = backend_.start_step(backend_.context, chosen->instance, *selected,
                                       playback_step_index, grab_entry,
                                       source_game_object, error);
    }
    if (!start_ok) {
        std::string reset_error;
        if (!backend_.reset_for_pool(backend_.context, chosen->instance, reset_error)) {
            error += error.empty() ? "Source animation start and pool reset both failed" :
                "; pool reset also failed: " + reset_error;
        }
        chosen->active = false;
        return Status::start_failed;
    }
    chosen->effect_table_id = id;
    chosen->step_index = playback_step_index;
    chosen->sequence_loops_remaining = sequence_loops_remaining;
    chosen->source_game_object = source_game_object;
    chosen->target_character = nullptr;
    chosen->target_object = nullptr;
    chosen->animator_character = nullptr;
    chosen->set_data = set_data;
    if (target_character) {
        if (!backend_.bind_game_object ||
            !backend_.bind_game_object(backend_.context, chosen->instance,
                                       target_character, error) ||
            !backend_.sync_irr_data ||
            !backend_.sync_irr_data(backend_.context, chosen->instance, true, error) ||
            !backend_.set_visible ||
            !backend_.set_visible(backend_.context, chosen->instance, true, error)) {
            if (error.empty()) error = "AnimatedFX next-step Character bind failed";
            std::string reset_error;
            (void)backend_.reset_for_pool(backend_.context, chosen->instance, reset_error);
            chosen->active = false;
            return Status::lifecycle_failed;
        }
        chosen->target_character = target_character;
    }
    if (target_object) {
        if (!backend_.bind_target_object ||
            !backend_.bind_target_object(backend_.context, chosen->instance,
                                         target_object, error) ||
            !backend_.sync_irr_data ||
            !backend_.sync_irr_data(backend_.context, chosen->instance, true, error) ||
            !backend_.set_visible ||
            !backend_.set_visible(backend_.context, chosen->instance, true, error)) {
            if (error.empty()) error = "AnimatedFX next-step GameObject bind failed";
            std::string reset_error;
            (void)backend_.reset_for_pool(backend_.context, chosen->instance, reset_error);
            chosen->active = false;
            return Status::lifecycle_failed;
        }
        chosen->target_object = target_object;
    }
    if (animator_character) {
        if (!backend_.bind_animator_character ||
            !backend_.bind_animator_character(backend_.context, chosen->instance,
                                              animator_character, error)) {
            if (error.empty()) error = "AnimatedFX next-step Animator bind failed";
            std::string reset_error;
            (void)backend_.reset_for_pool(backend_.context, chosen->instance, reset_error);
            chosen->active = false;
            return Status::lifecycle_failed;
        }
        chosen->animator_character = animator_character;
    }
    chosen->active = true;
    *output = chosen->instance;
    return Status::ok;
}

Status Owner::begin_anim_fx_set(std::int32_t id,
                                const std::array<float, 3>& position,
                                const std::array<float, 3>& origin,
                                void* source_game_object,
                                SetData* parent, std::size_t depth,
                                bool& launched, std::string& error,
                                void* target_character,
                                void* target_object) {
    launched = false;
    const auto* selected = sequence(id);
    if (!selected) {
        // The source integer overload returns without dispatch for an invalid
        // AnimatedEffectTable ID. A name hit can therefore still be a source
        // no-launch if its redirected ID is invalid.
        return Status::ok;
    }
    if (depth >= kMaxNestedSetDepth) {
        error = "PlayAnimFXSet exceeded the supported source nesting depth";
        return Status::invalid_request;
    }
    if (selected->steps.empty()) return Status::ok;

    std::size_t first_step = 0;
    if (selected->type == 2) {
        if (!backend_.random_below) {
            error = "Type-2 PlayAnimFXSet requires the canonical source RNG provider";
            return Status::backend_missing;
        }
        std::uint32_t choice = 0;
        if (!backend_.random_below(backend_.context,
                static_cast<std::uint32_t>(selected->steps.size()), choice, error)) {
            if (error.empty()) error = "Canonical source RNG rejected PlayAnimFXSet selection";
            return Status::lifecycle_failed;
        }
        if (choice >= selected->steps.size()) {
            error = "Canonical source RNG returned a PlayAnimFXSet index outside the set";
            return Status::lifecycle_failed;
        }
        first_step = choice;
    }

    auto set_data = std::make_unique<SetData>();
    set_data->effect_table_id = id;
    set_data->step_index = first_step;
    set_data->loops_remaining = selected->loop;
    set_data->source_loop_count = selected->loop;
    set_data->position = position;
    set_data->origin = origin;
    set_data->source_game_object = source_game_object;
    set_data->target_character = target_character;
    set_data->target_object = target_object;
    set_data->parent = parent;
    auto* retained = set_data.get();
    set_data_.push_back(std::move(set_data)); // source row list owns this until Flush
    retained->depth = depth;
    return dispatch_anim_fx_set_step(*retained, launched, error);
}

Status Owner::dispatch_anim_fx_set_step(SetData& set_data, bool& launched,
                                        std::string& error) {
    launched = false;
    const auto* selected = sequence(set_data.effect_table_id);
    if (!selected || set_data.step_index >= selected->steps.size()) {
        error = "PlayAnimFXSet source step is outside its AnimatedEffectTable row";
        return Status::stale_instance;
    }
    const auto& step = selected->steps[set_data.step_index];
    if (step.effect_dict_id < 0) return Status::ok; // source _GetAnimFX null is a no-op
    if (step.redirect == 1) {
        bool child_launched = false;
        const auto status = begin_anim_fx_set(step.effect_dict_id, set_data.position,
            set_data.origin, set_data.source_game_object, &set_data,
            set_data.depth + 1, child_launched, error,
            set_data.target_character, set_data.target_object);
        if (status == Status::ok) { launched = child_launched; return Status::ok; }
        return status;
    }
    void* instance = nullptr;
    const auto status = start_step_instance(set_data.effect_table_id,
        set_data.step_index, set_data.step_index, set_data.loops_remaining,
        false, false, set_data.source_game_object, set_data.target_character,
        set_data.target_object, nullptr, &instance, error, &set_data);
    if (status == Status::pool_exhausted) {
        // PlayAnimFX returns without effect when _GetAnimFX cannot allocate.
        error.clear();
        return Status::ok;
    }
    if (status == Status::source_no_launch) {
        error.clear();
        return Status::ok;
    }
    if (status == Status::ok) launched = instance != nullptr;
    return status;
}

Status Owner::advance_anim_fx_set(SetData& set_data, std::string& error) {
    const auto* selected = sequence(set_data.effect_table_id);
    if (!selected) {
        error = "PlayAnimFXSet callback lost its source AnimatedEffectTable row";
        return Status::stale_instance;
    }
    auto next_step = set_data.step_index + 1;
    if (next_step >= selected->steps.size()) {
        if (set_data.loops_remaining == -1) {
            next_step = 0;
        } else if (set_data.loops_remaining > 0) {
            --set_data.loops_remaining;
            next_step = 0;
        } else {
            return complete_anim_fx_set(set_data, error);
        }
    }
    set_data.step_index = next_step;
    bool launched = false;
    return dispatch_anim_fx_set_step(set_data, launched, error);
}

Status Owner::complete_anim_fx_set(SetData& set_data, std::string& error) {
    if (set_data.completed) return Status::ok;
    set_data.completed = true;
    if (!set_data.parent) return Status::ok;
    // Child completion is a source callback boundary. Only a type-1 parent
    // consumes its child-complete flag and advances to its next step.
    const auto* parent_sequence = sequence(set_data.parent->effect_table_id);
    if (!parent_sequence) {
        error = "PlayAnimFXSet parent callback lost its source sequence";
        return Status::stale_instance;
    }
    if (parent_sequence->type != 1) {
        set_data.parent->end_of_loop_pending = true;
        return Status::ok;
    }
    set_data.parent->end_of_loop_pending = true;
    return handle_anim_fx_set_end(set_data.parent, error);
}

Status Owner::handle_anim_fx_set_end(const SetData* set_data,
                                     std::string& error) {
    if (!set_data) return Status::ok;
    auto* mutable_data = const_cast<SetData*>(set_data);
    if (mutable_data->completed) return Status::ok;
    const auto* selected = sequence(mutable_data->effect_table_id);
    if (!selected) {
        error = "AnimatedFX SetData callback references a missing source row";
        return Status::stale_instance;
    }
    if (selected->type == 1) {
        // IDA _HandleSequence (0x496144) only consumes set-data records whose
        // +0 child-complete byte is set. A direct type-1 leaf callback alone
        // does not advance its parent set; nested completion sets this bit.
        if (!mutable_data->end_of_loop_pending) return Status::ok;
        mutable_data->end_of_loop_pending = false;
        return advance_anim_fx_set(*mutable_data, error);
    }
    if (!mutable_data->parent) return Status::ok; // source top-level non-type-1 no-op
    return complete_anim_fx_set(*mutable_data, error);
}

Status Owner::DropAnimatedFX(void** instance, std::string& error) {
    error.clear();
    if (!instance) { error = "DropAnimatedFX requires the caller's handle slot"; return Status::invalid_request; }
    if (!*instance) return Status::ok;
    auto* slot = find(*instance);
    if (!slot || !slot->active) { error = "DropAnimatedFX received an unknown or inactive instance"; return Status::stale_instance; }
    // IDA: DropAnimatedFX (0x494978) removes the FX from the active set,
    // resets its scene state, and returns it to the dictionary's free pool.
    if (!backend_.reset_for_pool) { error = "VisualFXManager pool reset backend is not bound"; return Status::backend_missing; }
    if (!backend_.reset_for_pool(backend_.context, slot->instance, error)) return Status::lifecycle_failed;
    slot->active = false;
    slot->source_game_object = nullptr;
    slot->target_character = nullptr;
    slot->target_object = nullptr;
    slot->animator_character = nullptr;
    slot->set_data = nullptr;
    *instance = nullptr;
    return Status::ok;
}

Status Owner::DropAnimatedFXSetByID(std::int32_t id, std::string& error) {
    error.clear();
    if (!catalog_) { error = "StopEffect cannot drop without the source catalog"; return Status::catalog_missing; }
    const auto* selected = sequence(id);
    if (!selected || selected->steps.empty()) return Status::ok; // source range/empty no-op
    const auto dict = selected->steps.front().effect_dict_id;
    for (auto& slot : slots_) {
        if (!slot.instance || !slot.active || slot.effect_table_id != id ||
            slot.effect_dict_id != dict) continue;
        void* instance = slot.instance;
        return DropAnimatedFX(&instance, error);
    }
    return Status::ok;
}

Status Owner::BindCharacter(void* instance, void* character, std::string& error) {
    error.clear(); auto* slot = find(instance);
    if (!slot) { error = "Character bind uses an unowned AnimatedFX"; return Status::stale_instance; }
    if (!backend_.bind_game_object || !backend_.bind_game_object(backend_.context, instance, character, error)) {
        if (error.empty()) error = "AnimatedFX GameObject binding backend is missing";
        return backend_.bind_game_object ? Status::lifecycle_failed : Status::backend_missing;
    }
    slot->target_character = character;
    slot->target_object = nullptr;
    return Status::ok;
}
Status Owner::BindTargetObject(void* instance, void* object, std::string& error) {
    error.clear(); auto* slot = find(instance);
    if (!slot) { error = "GameObject bind uses an unowned AnimatedFX"; return Status::stale_instance; }
    if (!object) { error = "GameObject bind requires a source object projection"; return Status::invalid_request; }
    if (!backend_.bind_target_object ||
        !backend_.bind_target_object(backend_.context, instance, object, error)) {
        if (error.empty()) error = "AnimatedFX GameObject binding backend is missing";
        return backend_.bind_target_object ? Status::lifecycle_failed : Status::backend_missing;
    }
    slot->target_character = nullptr;
    slot->target_object = object;
    return Status::ok;
}
Status Owner::BindAnimatorCharacter(void* instance, void* character, std::string& error) {
    error.clear(); auto* slot = find(instance);
    if (!slot) { error = "Animator character bind uses an unowned AnimatedFX"; return Status::stale_instance; }
    if (!backend_.bind_animator_character || !backend_.bind_animator_character(backend_.context, instance, character, error)) {
        if (error.empty()) error = "AnimatedFX nested animator binding backend is missing";
        return backend_.bind_animator_character ? Status::lifecycle_failed : Status::backend_missing;
    }
    slot->animator_character = character;
    return Status::ok;
}
Status Owner::SyncIrrData(void* instance, bool force, std::string& error) {
    error.clear(); auto* slot = find(instance);
    if (!slot) { error = "Irrlicht sync uses an unowned AnimatedFX"; return Status::stale_instance; }
    if (!backend_.sync_irr_data || !backend_.sync_irr_data(backend_.context, instance, force, error)) {
        if (error.empty()) error = "AnimatedFX Irrlicht synchronization backend is missing";
        return backend_.sync_irr_data ? Status::lifecycle_failed : Status::backend_missing;
    }
    return Status::ok;
}
Status Owner::SetVisible(void* instance, bool visible, std::string& error) {
    error.clear(); auto* slot = find(instance);
    if (!slot) { error = "Visibility change uses an unowned AnimatedFX"; return Status::stale_instance; }
    if (!backend_.set_visible || !backend_.set_visible(backend_.context, instance, visible, error)) {
        if (error.empty()) error = "AnimatedFX visibility backend is missing";
        return backend_.set_visible ? Status::lifecycle_failed : Status::backend_missing;
    }
    return Status::ok;
}
Status Owner::ResumeAnimator(void* instance, std::string& error) {
    error.clear(); auto* slot = find(instance);
    if (!slot) { error = "Animator resume uses an unowned AnimatedFX"; return Status::stale_instance; }
    if (!backend_.resume_animator || !backend_.resume_animator(backend_.context, instance, error)) {
        if (error.empty()) error = "AnimatedFX animator resume backend is missing";
        return backend_.resume_animator ? Status::lifecycle_failed : Status::backend_missing;
    }
    return Status::ok;
}

Status Owner::GetAnimController(void* instance, void** controller,
                                std::string& error) {
    error.clear();
    if (controller) *controller = nullptr;
    auto* slot = find(instance);
    if (!controller || !slot) {
        error = "AnimatedFX controller query uses an invalid instance or output";
        return slot ? Status::invalid_request : Status::stale_instance;
    }
    if (!backend_.get_anim_controller) {
        error = "AnimatedFX controller query backend is missing";
        return Status::backend_missing;
    }
    void* resolved = nullptr;
    if (!backend_.get_anim_controller(backend_.context, slot->instance,
                                      &resolved, error)) {
        if (error.empty()) error = "AnimatedFX controller query failed";
        return Status::lifecycle_failed;
    }
    slot->anim_controller = resolved;
    *controller = resolved;
    return Status::ok; // Source GetAnimController may return null.
}

Status Owner::PlayAnimControllerClip(void* controller,
                                     std::uint32_t clip_index,
                                     bool enabled,
                                     std::string& error) {
    error.clear();
    if (!controller) return Status::ok; // Source method lookup on absent controller is a no-op.
    auto slot = std::find_if(slots_.begin(), slots_.end(),
        [controller](const Slot& candidate) {
            return candidate.anim_controller == controller;
        });
    if (slot == slots_.end()) {
        error = "Animation clip request uses an unowned AnimatedFX controller";
        return Status::stale_instance;
    }
    if (!backend_.play_anim_controller_clip) {
        error = "AnimatedFX PlayClip backend is missing";
        return Status::backend_missing;
    }
    if (!backend_.play_anim_controller_clip(backend_.context, controller,
                                           clip_index, enabled, error)) {
        if (error.empty()) error = "AnimatedFX PlayClip backend failed";
        return Status::lifecycle_failed;
    }
    return Status::ok;
}

Status Owner::advance_sequence_step(void* instance, std::string& error) {
    error.clear();
    auto* current = find(instance);
    if (!current || !current->active) {
        error = "AnimatedFX sequence callback uses an unknown or inactive instance";
        return Status::stale_instance;
    }
    const auto* selected = sequence(current->effect_table_id);
    if (!selected) {
        error = "Active AnimatedFX lost its source sequence row";
        return Status::stale_instance;
    }
    if (selected->type == 2) {
        // A top-level GrabAnimFX type-2 set stores its own selected data with
        // no parent AnimFXSetData link (+44 is null), so its end callback is
        // source-observed as a no-op. Nested set traversal belongs to the
        // PlayAnimFXSet call path and is not represented by this top-level API.
        return Status::ok;
    }
    if (selected->type != 1) {
        if (selected->steps.size() <= 1) return Status::ok;
        error = "AnimatedEffectTable sequence type has no source-backed progression provider";
        return Status::unsupported_sequence;
    }

    auto next_step = current->step_index + 1;
    auto loops_remaining = current->sequence_loops_remaining;
    if (next_step >= selected->steps.size()) {
        if (loops_remaining == -1) {
            next_step = 0;
        } else if (loops_remaining > 0) {
            --loops_remaining;
            next_step = 0;
        } else {
            return Status::ok;
        }
    }

    const auto effect_table_id = current->effect_table_id;
    const auto source_game_object = current->source_game_object;
    const auto target_character = current->target_character;
    const auto target_object = current->target_object;
    const auto animator_character = current->animator_character;
    void* next_instance = nullptr;
    const auto status = start_step_instance(effect_table_id, next_step, next_step,
        loops_remaining, false, false, source_game_object, target_character,
        target_object, animator_character, &next_instance, error);
    if (status == Status::pool_exhausted || status == Status::source_no_launch) {
        // Source PlayAnimFXStep simply receives null when _GetAnimFX reaches
        // its per-dictionary cap; no sibling instance is fabricated.
        error.clear();
        return Status::ok;
    }
    return status;
}

Status Owner::Update(std::string& error) {
    error.clear();
    if (!catalog_) { error = "VisualFXManager cannot update without its source catalog"; return Status::catalog_missing; }
    if (!backend_.update || !backend_.reset_for_pool) {
        error = "VisualFXManager requires the shared AnimatedFX update backend";
        return Status::backend_missing;
    }
    // Source callback transitions can append a new step instance. Visit only
    // the instances that were active at frame start; new steps begin on the
    // following manager tick, matching the callback/animator phase ordering.
    const auto frame_slot_count = slots_.size();
    for (std::size_t i = 0; i < frame_slot_count; ++i) {
        if (!slots_[i].instance || !slots_[i].active) continue;
        const auto instance = slots_[i].instance;
        const auto* selected = sequence(slots_[i].effect_table_id);
        if (!selected) { error = "Active AnimatedFX lost its source sequence row"; return Status::stale_instance; }
        const auto step_index = slots_[i].step_index;
        bool end_of_loop_callback_due = false;
        bool source_hidden = false;
        if (!backend_.update(backend_.context, instance, *selected, step_index,
                             end_of_loop_callback_due, source_hidden, error)) {
            if (error.empty()) error = "AnimatedFX source frame update failed";
            return Status::lifecycle_failed;
        }
        if (end_of_loop_callback_due) {
            const auto status = slots_[i].set_data
                ? handle_anim_fx_set_end(slots_[i].set_data, error)
                : advance_sequence_step(instance, error);
            if (status != Status::ok) return status;
        }
        if (source_hidden) {
            void* dropped_instance = instance;
            const auto status = DropAnimatedFX(&dropped_instance, error);
            if (status != Status::ok) return status;
        }
    }
    return Status::ok;
}

Status Owner::Render(std::string& error) {
    error.clear();
    if (!backend_.render) {
        if (backend_.draw_registered_by_scene) return Status::ok;
        error = "VisualFXManager has no FX render submission or shared-scene registration";
        return Status::backend_missing;
    }
    for (auto& slot : slots_) {
        if (!slot.instance || !slot.active) continue;
        if (!backend_.render(backend_.context, slot.instance, error)) {
            if (error.empty()) error = "AnimatedFX renderer submission failed";
            return Status::lifecycle_failed;
        }
    }
    return Status::ok;
}

Status Owner::InstallObjectiveMarker(std::uintptr_t* objective_fx_slot,
                                     std::int32_t effect_table_id,
                                     void* target_character,
                                     void* source_game_object,
                                     std::string& error) {
    error.clear();
    if (!objective_fx_slot || !target_character) {
        error = "Objective marker install is missing the Objective slot or Character";
        return Status::invalid_request;
    }
    // Source InstallObjectiveMarker calls its own RemoveObjectiveMarker
    // vtable slot (+0x14) before GrabAnimFX. Remove only detaches and hides;
    // the old instance remains manager-owned until a later source Drop.
    if (*objective_fx_slot) {
        const auto removed = RemoveObjectiveMarker(objective_fx_slot, target_character, error);
        if (removed != Status::ok) return removed;
    }
    void* instance = nullptr;
    auto status = GrabAnimFX(effect_table_id, source_game_object, &instance, error);
    if (status != Status::ok) {
        // At 0x47d9cc the source unconditionally stores GrabAnimFX's return
        // into Objective+0x28 before its null check. A genuine source pool
        // exhaustion therefore replaces the previously hidden handle with
        // null. Infrastructure failures are different: no source Grab ran,
        // so retain the old detached handle for a truthful retry.
        if (status == Status::pool_exhausted) *objective_fx_slot = 0;
        return status;
    }
    *objective_fx_slot = reinterpret_cast<std::uintptr_t>(instance);
    if (!instance) return Status::ok; // Source installs the null Grab result and returns.
    if ((status = BindCharacter(instance, target_character, error)) != Status::ok) return status;
    if ((status = SyncIrrData(instance, true, error)) != Status::ok) return status;
    if ((status = BindAnimatorCharacter(instance, target_character, error)) != Status::ok) return status;
    if ((status = SetVisible(instance, true, error)) != Status::ok) return status;
    return ResumeAnimator(instance, error);
}

Status Owner::RemoveObjectiveMarker(std::uintptr_t* objective_fx_slot,
                                    void* target_character,
                                    std::string& error) {
    error.clear(); (void)target_character;
    if (!objective_fx_slot) { error = "Objective marker remove is missing the Objective slot"; return Status::invalid_request; }
    if (!*objective_fx_slot) return Status::ok;
    void* instance = reinterpret_cast<void*>(*objective_fx_slot);
    auto status = BindCharacter(instance, nullptr, error);
    if (status != Status::ok) return status;
    if ((status = SyncIrrData(instance, true, error)) != Status::ok) return status;
    if ((status = BindAnimatorCharacter(instance, nullptr, error)) != Status::ok) return status;
    return SetVisible(instance, false, error); // Source Remove retains Objective+0x28.
}

void Owner::Flush() noexcept {
    if (backend_.reset_for_pool) {
        for (auto& slot : slots_) if (slot.instance && slot.active) {
            std::string ignored; (void)backend_.reset_for_pool(backend_.context, slot.instance, ignored);
        }
    }
    if (backend_.destroy) for (auto& slot : slots_) if (slot.instance)
        backend_.destroy(backend_.context, slot.instance);
    slots_.clear();
    set_data_.clear();
    registered_effect_sets_.clear();
    registered_effect_dicts_.clear();
}

PoolSnapshot Owner::pool(std::int32_t id) const noexcept {
    PoolSnapshot out; out.effect_dict_id = id;
    for (const auto& slot : slots_) if (slot.effect_dict_id == id) {
        ++out.allocated; if (slot.active) ++out.active; else ++out.free;
    }
    return out;
}
} // namespace dh2::native_visual_fx_manager_v1
