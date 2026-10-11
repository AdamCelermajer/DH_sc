#include "native_visual_fx_manager_v1.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <utility>

namespace dh2::native_visual_fx_manager_v1 {
namespace {
constexpr std::size_t kMaxRows = 10000;
constexpr std::size_t kMaxString = 4096;
constexpr std::size_t kPoolLimit = 6; // _GetAnimFX: reject when count > 5.

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
    static const char* const floor_fields[] = {"Effect", "Floortype", "RunSound", "WalkSound"};
    compare(animation_fields, sizeof(animation_fields) / sizeof(animation_fields[0]));
    compare(effect_fields, sizeof(effect_fields) / sizeof(effect_fields[0]));
    compare(floor_fields, sizeof(floor_fields) / sizeof(floor_fields[0]));
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
        // effects_pyarraynames.bin contains additional array-name sections.
        // The source AnimatedEffectTable reader consumes only its first section.
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
    if (!backend_.create || !backend_.destroy || !backend_.start_sequence ||
        !backend_.reset_for_pool) {
        error = "VisualFXManager requires the shared AnimatedFX/scene backend";
        return Status::backend_missing;
    }
    // IDA: VisualFXManager::GrabAnimFX (0x495430) selects the first step's
    // File/EffectDict id, then _GetAnimFX (0x494ad4) owns/reuses that bucket.
    const auto dict_id = selected->steps.front().effect_dict_id;
    if (dict_id < 0 || static_cast<std::size_t>(dict_id) >= catalog_->effect_paths.size()) {
        error = "AnimatedEffectTable first step has no loadable EffectDict row";
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
        if (!backend_.create(backend_.context, dict_id, path, &instance, error) || !instance) {
            if (error.empty()) error = "AnimatedFX backend did not produce a loaded instance";
            return Status::load_failed;
        }
        try {
            slots_.push_back(Slot{instance, dict_id, id, false});
            chosen = &slots_.back();
        } catch (...) {
            backend_.destroy(backend_.context, instance);
            error = "VisualFXManager pool allocation failed";
            return Status::allocation_failed;
        }
    }
    if (!backend_.start_sequence(backend_.context, chosen->instance, *selected,
                                source_game_object, error)) {
        std::string reset_error;
        if (!backend_.reset_for_pool(backend_.context, chosen->instance, reset_error)) {
            error += error.empty() ? "Source animation start and pool reset both failed" :
                "; pool reset also failed: " + reset_error;
        }
        chosen->active = false;
        return Status::start_failed;
    }
    chosen->effect_table_id = id;
    chosen->active = true;
    *output = chosen->instance;
    return Status::ok;
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
    *instance = nullptr;
    return Status::ok;
}

Status Owner::BindCharacter(void* instance, void* character, std::string& error) {
    error.clear(); auto* slot = find(instance);
    if (!slot || !slot->active) { error = "Character bind uses a stale AnimatedFX"; return Status::stale_instance; }
    if (!backend_.bind_game_object || !backend_.bind_game_object(backend_.context, instance, character, error)) {
        if (error.empty()) error = "AnimatedFX GameObject binding backend is missing";
        return backend_.bind_game_object ? Status::lifecycle_failed : Status::backend_missing;
    }
    return Status::ok;
}
Status Owner::BindAnimatorCharacter(void* instance, void* character, std::string& error) {
    error.clear(); auto* slot = find(instance);
    if (!slot || !slot->active) { error = "Animator character bind uses a stale AnimatedFX"; return Status::stale_instance; }
    if (!backend_.bind_animator_character || !backend_.bind_animator_character(backend_.context, instance, character, error)) {
        if (error.empty()) error = "AnimatedFX nested animator binding backend is missing";
        return backend_.bind_animator_character ? Status::lifecycle_failed : Status::backend_missing;
    }
    return Status::ok;
}
Status Owner::SyncIrrData(void* instance, bool force, std::string& error) {
    error.clear(); auto* slot = find(instance);
    if (!slot || !slot->active) { error = "Irrlicht sync uses a stale AnimatedFX"; return Status::stale_instance; }
    if (!backend_.sync_irr_data || !backend_.sync_irr_data(backend_.context, instance, force, error)) {
        if (error.empty()) error = "AnimatedFX Irrlicht synchronization backend is missing";
        return backend_.sync_irr_data ? Status::lifecycle_failed : Status::backend_missing;
    }
    return Status::ok;
}
Status Owner::SetVisible(void* instance, bool visible, std::string& error) {
    error.clear(); auto* slot = find(instance);
    if (!slot || !slot->active) { error = "Visibility change uses a stale AnimatedFX"; return Status::stale_instance; }
    if (!backend_.set_visible || !backend_.set_visible(backend_.context, instance, visible, error)) {
        if (error.empty()) error = "AnimatedFX visibility backend is missing";
        return backend_.set_visible ? Status::lifecycle_failed : Status::backend_missing;
    }
    return Status::ok;
}
Status Owner::ResumeAnimator(void* instance, std::string& error) {
    error.clear(); auto* slot = find(instance);
    if (!slot || !slot->active) { error = "Animator resume uses a stale AnimatedFX"; return Status::stale_instance; }
    if (!backend_.resume_animator || !backend_.resume_animator(backend_.context, instance, error)) {
        if (error.empty()) error = "AnimatedFX animator resume backend is missing";
        return backend_.resume_animator ? Status::lifecycle_failed : Status::backend_missing;
    }
    return Status::ok;
}

Status Owner::Update(std::string& error) {
    error.clear();
    if (!catalog_) { error = "VisualFXManager cannot update without its source catalog"; return Status::catalog_missing; }
    if (!backend_.update || !backend_.reset_for_pool) {
        error = "VisualFXManager requires the shared AnimatedFX update backend";
        return Status::backend_missing;
    }
    for (auto& slot : slots_) {
        if (!slot.instance || !slot.active) continue;
        const auto* selected = sequence(slot.effect_table_id);
        if (!selected) { error = "Active AnimatedFX lost its source sequence row"; return Status::stale_instance; }
        bool drop = false;
        if (!backend_.update(backend_.context, slot.instance, *selected, drop, error)) {
            if (error.empty()) error = "AnimatedFX source frame update failed";
            return Status::lifecycle_failed;
        }
        if (drop) {
            void* instance = slot.instance;
            const auto status = DropAnimatedFX(&instance, error);
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
}

PoolSnapshot Owner::pool(std::int32_t id) const noexcept {
    PoolSnapshot out; out.effect_dict_id = id;
    for (const auto& slot : slots_) if (slot.effect_dict_id == id) {
        ++out.allocated; if (slot.active) ++out.active; else ++out.free;
    }
    return out;
}
} // namespace dh2::native_visual_fx_manager_v1
