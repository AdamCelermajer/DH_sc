#include "world_objects_v1.hpp"

#include <cctype>
#include <stdexcept>

namespace dh::foundation::doors {
namespace {

std::string property(const ActorDefinition& definition, const char* key) {
    const auto it = definition.properties.find(key);
    return it == definition.properties.end() ? std::string() : it->second;
}

bool has_condition(const std::string& name) {
    return !name.empty() && name != "Invalid";
}

bool select_clip(CharacterVisual& visual, const std::string& name, bool loop, std::string& error) {
    if (visual.select(name, loop, error)) return true;
    std::string lower = name;
    for (auto& c : lower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    std::string second;
    if (visual.select(lower, loop, second)) return true;
    error = second;
    return false;
}

} // namespace

bool LeverRuntimeV1::load(const containers::ContainerTablesV1& tables, const std::vector<ActorDefinition>& definitions,
                          std::vector<std::string>& notices, std::string& error) {
    error.clear();
    slots_.clear();
    if (!tables.ready()) {
        error = "TriggerObject tables are not loaded (game_objects)";
        return false;
    }
    const auto& borrow = tables.borrow();
    for (const auto& definition : definitions) {
        if (definition.gametype != "TriggerObject") continue;
        Slot slot;
        auto& instance = slot.instance;
        instance.stableId = definition.stableId;
        instance.name = definition.name;
        instance.data = property(definition, "data");
        instance.script = property(definition, "script");
        instance.unlock_cond = property(definition, "unlock_cond");
        instance.transform = definition.placement;
        const auto fail = [&](const char* reason) {
            notices.push_back("TriggerObject " + definition.name + " data=" + instance.data + ": " + reason);
        };
        if (!borrow.triggers.names || !borrow.triggers.rows) {
            fail("Arrays::TriggerObjects table missing");
            continue;
        }
        const auto& names = *borrow.triggers.names;
        std::int32_t row = -1;
        for (std::size_t i = 0; i < names.size(); ++i)
            if (names[i] == instance.data) { row = static_cast<std::int32_t>(i); break; }
        if (row < 0 || std::size_t(row) >= borrow.triggers.rows->size()) {
            fail("row not in Arrays::TriggerObjects");
            continue;
        }
        const auto& declared = (*borrow.triggers.rows)[row];
        instance.row = row;
        instance.interaction_type = declared.raw4;   // source GetInteractionType: row+4
        instance.sound = declared.sound10;           // source Interact: Play3D row+16
        instance.visual_id = declared.visual14;
        const std::string* file = nullptr;
        std::string dictionaryError;
        if (instance.visual_id < 0 || !borrow.dictionary || !borrow.dictionary->file(instance.visual_id, file, dictionaryError) || !file)
            fail("visual id not in GameObjectDict");
        else
            instance.visual_file = *file;
        slots_.push_back(std::move(slot));
    }
    return true;
}

bool LeverRuntimeV1::adopt(const AssetCatalog& assets, std::vector<std::string>& notices, std::string& error) {
    error.clear();
    for (auto& slot : slots_) {
        if (slot.instance.visual_file.empty()) continue;
        auto visual = std::make_unique<CharacterVisual>();
        std::string loadError;
        bool ok = false;
        try {
            ok = visual->load_embedded_scene(assets, slot.instance.visual_file, loadError) && !visual->meshes().empty();
            if (ok && !select_clip(*visual, "idle", true, loadError)) ok = false;
        } catch (const std::exception& exception) {
            ok = false;
            loadError = exception.what();
        }
        if (!ok) {
            notices.push_back("lever visual unavailable " + slot.instance.visual_file + ": " + loadError);
            continue;
        }
        slot.visual = std::move(visual);
    }
    return true;
}

bool LeverRuntimeV1::refresh_conditions(const std::function<bool(const std::string& condition, bool& value)>& condition,
                                       std::vector<std::string>& notices, std::string& error) {
    error.clear();
    for (auto& slot : slots_) {
        if (!has_condition(slot.instance.unlock_cond)) {
            slot.unlocked = true;  // no unlock gate: usable
            continue;
        }
        bool value = false;
        if (!condition(slot.instance.unlock_cond, value)) {
            notices.push_back("TriggerObject " + slot.instance.name + ": condition unavailable " + slot.instance.unlock_cond);
            value = false;
        }
        slot.unlocked = value;
    }
    return true;
}

bool LeverRuntimeV1::find_by_name(const std::string& name, std::size_t& index) const {
    for (std::size_t i = 0; i < slots_.size(); ++i)
        if (slots_[i].instance.name == name) { index = i; return true; }
    return false;
}

bool LeverRuntimeV1::find_by_stable_id(std::uint64_t id, std::size_t& index) const {
    for (std::size_t i = 0; i < slots_.size(); ++i)
        if (slots_[i].instance.stableId == id) { index = i; return true; }
    return false;
}

void LeverRuntimeV1::set_unlocked(std::size_t i, bool value) {
    slots_.at(i).unlocked = value;
}

LeverUseResultV1 LeverRuntimeV1::use(std::size_t i) {
    LeverUseResultV1 result;
    if (i >= slots_.size()) return result;
    auto& slot = slots_[i];
    result.position = {slot.instance.transform[12], slot.instance.transform[13], slot.instance.transform[14]};
    if (!slot.unlocked) {
        result.status = LeverUseResultV1::Status::locked;
        return result;
    }
    result.status = LeverUseResultV1::Status::accepted;
    result.script = slot.instance.script;
    result.sound_ordinal = slot.instance.sound;
    if (slot.visual) {
        std::string error;
        // TriggerObject::Interact plays "activate" (non-looping); the lever is left at its end pose.
        if (!select_clip(*slot.visual, "activate", false, error)) result.status = LeverUseResultV1::Status::no_visual;
    } else {
        result.status = LeverUseResultV1::Status::no_visual;
    }
    return result;
}

bool LeverRuntimeV1::update(double seconds, std::string& error) {
    error.clear();
    for (auto& slot : slots_)
        if (slot.visual && !slot.visual->update(seconds, error)) return false;
    return true;
}

std::vector<LeverRuntimeV1::View> LeverRuntimeV1::views() const {
    std::vector<View> out;
    for (const auto& slot : slots_) out.push_back({&slot.instance, slot.visual.get()});
    return out;
}

bool DecorRuntimeV1::load(const std::vector<ActorDefinition>& definitions, std::vector<std::string>& notices,
                          std::string& error) {
    error.clear();
    notices.clear();
    slots_.clear();
    for (const auto& definition : definitions) {
        if (definition.gametype != "AnimatedDecor") continue;
        const auto activate = property(definition, "activate_cond");
        const auto deactivate = property(definition, "deactivate_cond");
        if (!has_condition(activate) && !has_condition(deactivate)) continue;  // ungated decor: not this owner
        Slot slot;
        slot.instance.stableId = definition.stableId;
        slot.instance.name = definition.name;
        slot.instance.dae = property(definition, "dae");
        slot.instance.startanim = property(definition, "startanim");
        if (slot.instance.startanim.empty()) slot.instance.startanim = "idle";  // source InitPost default
        slot.instance.activate_cond = activate;
        slot.instance.deactivate_cond = deactivate;
        slot.instance.transform = definition.placement;
        slots_.push_back(std::move(slot));
    }
    return true;
}

bool DecorRuntimeV1::refresh_conditions(const std::function<bool(const std::string& condition, bool& value)>& condition,
                                        std::vector<std::string>& notices, std::string& error) {
    error.clear();
    for (std::size_t i = 0; i < slots_.size(); ++i) {
        const auto& instance = slots_[i].instance;
        // Source InitPost: MeetCondition = activate true (when given) and deactivate false (when given).
        bool met = true;
        if (has_condition(instance.activate_cond)) {
            bool value = false;
            if (!condition(instance.activate_cond, value)) { notices.push_back("AnimatedDecor " + instance.name + ": condition unavailable " + instance.activate_cond); value = false; }
            met = met && value;
        }
        if (has_condition(instance.deactivate_cond)) {
            bool value = false;
            if (!condition(instance.deactivate_cond, value)) { notices.push_back("AnimatedDecor " + instance.name + ": condition unavailable " + instance.deactivate_cond); value = false; }
            met = met && !value;
        }
        if (!set_present(i, met, error)) return false;
    }
    return true;
}

bool DecorRuntimeV1::adopt(const AssetCatalog& assets, std::vector<std::string>& notices, std::string& error) {
    error.clear();
    assets_ = &assets;
    for (auto& slot : slots_) {
        if (!slot.present) continue;
        std::string createError;
        if (!create(slot, createError)) notices.push_back("AnimatedDecor visual unavailable " + slot.instance.name + " " + slot.instance.dae + ": " + createError);
    }
    return true;
}

bool DecorRuntimeV1::find_by_stable_id(std::uint64_t id, std::size_t& index) const {
    for (std::size_t i = 0; i < slots_.size(); ++i)
        if (slots_[i].instance.stableId == id) { index = i; return true; }
    return false;
}

bool DecorRuntimeV1::create(Slot& slot, std::string& error) {
    error.clear();
    if (slot.visual) return select_clip(*slot.visual, slot.instance.startanim, true, error);
    if (!assets_) {
        error = "AnimatedDecor assets are not bound";
        return false;
    }
    auto visual = std::make_unique<CharacterVisual>();
    bool ok = false;
    try {
        ok = visual->load_embedded_scene(*assets_, slot.instance.dae, error) && !visual->meshes().empty();
        if (ok && !select_clip(*visual, slot.instance.startanim, true, error)) ok = false;
    } catch (const std::exception& exception) {
        ok = false;
        error = exception.what();
    }
    if (!ok) return false;
    slot.visual = std::move(visual);
    return true;
}

bool DecorRuntimeV1::set_present(std::size_t i, bool value, std::string& error) {
    error.clear();
    auto& slot = slots_.at(i);
    if (slot.present == value) return true;
    slot.present = value;
    if (!value) return true;  // removed from the world; the visual object is kept for a later start
    return create(slot, error);
}

bool DecorRuntimeV1::update(double seconds, std::string& error) {
    error.clear();
    for (auto& slot : slots_)
        if (slot.present && slot.visual && !slot.visual->update(seconds, error)) return false;
    return true;
}

std::vector<DecorRuntimeV1::View> DecorRuntimeV1::views() const {
    std::vector<View> out;
    for (const auto& slot : slots_)
        if (slot.present && slot.visual) out.push_back({&slot.instance, slot.visual.get()});
    return out;
}

} // namespace dh::foundation::doors
