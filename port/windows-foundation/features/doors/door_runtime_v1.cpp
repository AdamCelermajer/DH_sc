#include "door_runtime_v1.hpp"

#include "../../animation_markers.hpp"

#include <cctype>
#include <cstdlib>
#include <stdexcept>

namespace dh::foundation::doors {
namespace {

std::string property(const ActorDefinition& definition, const char* key) {
    const auto it = definition.properties.find(key);
    return it == definition.properties.end() ? std::string() : it->second;
}

bool is_no_condition(const std::string& name) {
    return name.empty() || name == "Invalid";
}

// Clip names: the source spells them "Open", "IdleOpened"; the embedded scene stores them lower case.
bool select_clip(CharacterVisual& visual, const char* name, bool loop, std::string& error) {
    if (visual.select(name, loop, error)) return true;
    std::string lower = name;
    for (auto& c : lower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    std::string second;
    if (visual.select(lower, loop, second)) return true;
    error = second;
    return false;
}

} // namespace

bool DoorRuntimeV1::load(const containers::ContainerTablesV1& tables, const std::vector<ActorDefinition>& definitions,
                         std::vector<std::string>& notices, std::string& error) {
    error.clear();
    slots_.clear();
    sounds_.clear();
    if (!tables.ready()) {
        error = "Door tables are not loaded (game_objects)";
        return false;
    }
    const auto& borrow = tables.borrow();
    for (const auto& definition : definitions) {
        if (definition.gametype != "Door") continue;
        Slot slot;
        auto& instance = slot.instance;
        instance.stableId = definition.stableId;
        instance.name = definition.name;
        instance.data = property(definition, "data");
        instance.transform = definition.placement;
        instance.activate_cond = property(definition, "activate_cond");
        instance.deactivate_cond = property(definition, "deactivate_cond");
        instance.opened_authored = property(definition, "opened") == "1";
        const auto collision = property(definition, "door_collision");
        if (collision.empty() || collision == "1" || collision == "true") instance.door_collision = true;
        else if (collision == "0" || collision == "false") instance.door_collision = false;
        else {
            error = "Door " + definition.name + ": unsupported door_collision value " + collision;
            return false;
        }
        const auto fail = [&](const char* reason) {
            notices.push_back("Door " + definition.name + " data=" + instance.data + ": " + reason);
        };
        if (!borrow.doors.names || !borrow.doors.rows) {
            fail("Arrays::Doors table missing");
            continue;
        }
        const auto& names = *borrow.doors.names;
        std::int32_t row = -1;
        for (std::size_t i = 0; i < names.size(); ++i)
            if (names[i] == instance.data) { row = static_cast<std::int32_t>(i); break; }
        if (row < 0 || std::size_t(row) >= borrow.doors.rows->size()) {
            fail("row not in Arrays::Doors");
            continue;
        }
        const auto& declared = (*borrow.doors.rows)[row];
        instance.row = row;
        instance.sound_close = declared.sound_c;
        instance.sound_open = declared.sound10;
        instance.visual_id = declared.visual14;
        const std::string* file = nullptr;
        std::string dictionaryError;
        if (instance.visual_id < 0 || !borrow.dictionary || !borrow.dictionary->file(instance.visual_id, file, dictionaryError) || !file)
            fail("visual id not in GameObjectDict");
        else
            instance.visual_file = *file;
        slot.state = instance.opened_authored ? DoorStateV1::opened : DoorStateV1::closed;
        slot.collision_enabled = instance.door_collision;
        slot.filter_enabled = !instance.opened_authored;
        slot.present = true;  // refresh_conditions applies the gates
        slot.dirty = false;
        slots_.push_back(std::move(slot));
    }
    return true;
}

bool DoorRuntimeV1::adopt(const AssetCatalog& assets, std::vector<std::string>& notices, std::string& error) {
    error.clear();
    for (auto& slot : slots_) {
        if (slot.instance.visual_file.empty()) continue;
        auto visual = std::make_unique<CharacterVisual>();
        std::string loadError;
        bool ok = false;
        try {
            ok = visual->load_embedded_scene(assets, slot.instance.visual_file, loadError) && !visual->meshes().empty();
        } catch (const std::exception& exception) {
            ok = false;
            loadError = exception.what();
        }
        if (!ok) {
            notices.push_back("door visual unavailable " + slot.instance.visual_file + ": " + loadError);
            continue;
        }
        std::string markerError;
        for (int direction = 0; direction < 2; ++direction) {
            const char* clip = direction ? "open" : "close";
            std::int32_t start = 0, end = 0;
            if (!visual->animation_range(clip, start, end, markerError)) {
                notices.push_back("door " + slot.instance.name + " clip missing: " + clip);
                continue;
            }
            slot.clip_end_ms[direction] = end;
            if (const auto* markers = visual->markers(clip, markerError))
                for (const auto& marker : markers->markers())
                    if (marker.name == "opened" || marker.name == "closed") slot.marker_ms[direction] = marker.time_ms;
        }
        slot.visual = std::move(visual);
        // Startup (Door::InitFinal -> Opened(0)/Closed(0)): the idle clip of the authored state.
        if (!show_idle(slot, error)) {
            notices.push_back("door idle clip unavailable " + slot.instance.name + ": " + error);
            error.clear();
        }
    }
    return true;
}

bool DoorRuntimeV1::find_by_name(const std::string& name, std::size_t& index) const {
    for (std::size_t i = 0; i < slots_.size(); ++i)
        if (slots_[i].instance.name == name) { index = i; return true; }
    return false;
}

bool DoorRuntimeV1::refresh_conditions(const std::function<bool(const std::string& condition, bool& value)>& condition,
                                       std::vector<std::string>& notices, std::string& error) {
    error.clear();
    for (std::size_t i = 0; i < slots_.size(); ++i) {
        const auto& instance = slots_[i].instance;
        bool met = true;
        if (!instance.activate_cond.empty() && instance.activate_cond != "Invalid") {
            bool value = false;
            if (!condition(instance.activate_cond, value)) { notices.push_back("Door " + instance.name + ": condition unavailable " + instance.activate_cond); value = false; }
            met = met && value;
        }
        if (!instance.deactivate_cond.empty() && instance.deactivate_cond != "Invalid") {
            bool value = false;
            if (!condition(instance.deactivate_cond, value)) { notices.push_back("Door " + instance.name + ": condition unavailable " + instance.deactivate_cond); value = false; }
            met = met && !value;
        }
        if (!set_present(i, met, error)) return false;
    }
    return true;
}

bool DoorRuntimeV1::find_by_stable_id(std::uint64_t id, std::size_t& index) const {
    for (std::size_t i = 0; i < slots_.size(); ++i)
        if (slots_[i].instance.stableId == id) { index = i; return true; }
    return false;
}

std::vector<DoorViewV1> DoorRuntimeV1::views() {
    std::vector<DoorViewV1> out;
    out.reserve(slots_.size());
    for (const auto& slot : slots_) {
        DoorViewV1 view;
        view.instance = &slot.instance;
        view.visual = slot.present ? slot.visual.get() : nullptr;
        view.state = slot.state;
        view.present = slot.present;
        out.push_back(view);
    }
    return out;
}

std::vector<DoorPhysicalBindingV1> DoorRuntimeV1::physical_bindings() const {
    std::vector<DoorPhysicalBindingV1> out;
    out.reserve(slots_.size());
    for (const auto& slot : slots_)
        out.push_back({slot.instance.stableId, slot.collision_enabled, slot.present, slot.filter_enabled});
    return out;
}

bool DoorRuntimeV1::show_idle(Slot& slot, std::string& error) {
    if (!slot.visual) return true;
    const bool opened = slot.state == DoorStateV1::opened || slot.state == DoorStateV1::opening;
    return select_clip(*slot.visual, opened ? "idleopened" : "idleclosed", true, error);
}

void DoorRuntimeV1::request_sound(const Slot& slot, bool opening) {
    const std::int32_t ordinal = opening ? slot.instance.sound_open : slot.instance.sound_close;
    if (ordinal == -1) return;  // source: `if (row id != -1)` before Play3D
    DoorSoundRequestV1 request;
    request.ordinal = ordinal;
    request.position = {slot.instance.transform[12], slot.instance.transform[13], slot.instance.transform[14]};
    sounds_.push_back(request);
}

bool DoorRuntimeV1::transition(std::size_t index, bool opening, bool skip, std::string& error) {
    auto& slot = slots_.at(index);
    slot.dirty = true;
    const DoorStateV1 end = opening ? DoorStateV1::opened : DoorStateV1::closed;
    const bool animated = slot.visual && !skip;
    if (animated) {
        slot.busy = true;
        slot.state = opening ? DoorStateV1::opening : DoorStateV1::closing;
        slot.marker_fired = false;
        if (!select_clip(*slot.visual, opening ? "open" : "close", false, error)) {
            // No transition clip: the source plays the end state at once.
            slot.busy = false;
            slot.state = end;
            slot.filter_enabled = !opening;
            error.clear();
            if (!show_idle(slot, error)) return false;
        }
    } else {
        slot.state = end;
        slot.filter_enabled = !opening;
        if (!show_idle(slot, error)) return false;
    }
    request_sound(slot, opening);
    return true;
}

bool DoorRuntimeV1::open(std::size_t index, bool skip, std::string& error) {
    error.clear();
    auto& slot = slots_.at(index);
    if (slot.busy || slot.state != DoorStateV1::closed) return true;  // Door::Open guard: state 0 and not busy
    return transition(index, true, skip, error);
}

bool DoorRuntimeV1::close(std::size_t index, bool skip, std::string& error) {
    error.clear();
    auto& slot = slots_.at(index);
    if (slot.busy || slot.state != DoorStateV1::opened) return true;  // Door::Close guard: state 1 and not busy
    return transition(index, false, skip, error);
}

bool DoorRuntimeV1::blocking(std::size_t index) const {
    return slots_.at(index).busy;
}

bool DoorRuntimeV1::update(double seconds, std::string& error) {
    error.clear();
    for (auto& slot : slots_) {
        if (!slot.visual || !slot.present) continue;
        if (!slot.visual->update(seconds, error)) return false;
        if (!slot.busy) continue;
        const double elapsedMs = slot.visual->animation_elapsed_seconds() * 1000.0;
        const bool opening = slot.state == DoorStateV1::opening;
        // __EventCallback: the authored marker applies the end state (no idle replay).
        const int direction = opening ? 1 : 0;
        if (!slot.marker_fired && slot.marker_ms[direction] >= 0.0 && elapsedMs >= slot.marker_ms[direction]) {
            slot.marker_fired = true;
            slot.state = opening ? DoorStateV1::opened : DoorStateV1::closed;
            slot.filter_enabled = !opening;
            slot.dirty = true;
        }
        // __Callback: the clip ended; the transition is over and the idle clip of the end state plays.
        if (slot.clip_end_ms[direction] >= 0.0 && elapsedMs >= slot.clip_end_ms[direction]) {
            slot.busy = false;
            if (!slot.marker_fired) {
                slot.state = opening ? DoorStateV1::opened : DoorStateV1::closed;
                slot.filter_enabled = !opening;
            }
            slot.dirty = true;
            if (!show_idle(slot, error)) return false;
        }
    }
    return true;
}

bool DoorRuntimeV1::set_present(std::size_t index, bool present, std::string& error) {
    error.clear();
    auto& slot = slots_.at(index);
    if (slot.present == present) return true;
    slot.present = present;
    if (present && slot.visual) return show_idle(slot, error);
    return true;
}

bool DoorRuntimeV1::restore(std::size_t index, DoorStateV1 state, std::string& error) {
    error.clear();
    auto& slot = slots_.at(index);
    slot.state = state;
    slot.busy = false;
    slot.dirty = false;
    // Door::Deserialize maps states 1/3 to opened; states 0/2 map closed.
    slot.filter_enabled = state != DoorStateV1::opened && state != DoorStateV1::opening;
    return show_idle(slot, error);
}

std::vector<DoorSoundRequestV1> DoorRuntimeV1::take_sound_requests() {
    auto out = std::move(sounds_);
    sounds_.clear();
    return out;
}

std::vector<std::size_t> DoorRuntimeV1::take_changed() {
    std::vector<std::size_t> out;
    for (std::size_t i = 0; i < slots_.size(); ++i)
        if (slots_[i].dirty) { out.push_back(i); slots_[i].dirty = false; }
    return out;
}

} // namespace dh::foundation::doors
