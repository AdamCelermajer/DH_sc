#include "container_runtime_v1.hpp"

#include "../../animation_markers.hpp"
#include "../../embedded_scene_clips.hpp"

#include <cmath>
#include <stdexcept>

namespace dh::foundation::containers {

const char* container_status_name(ContainerInteractStatusV1 status) {
    switch (status) {
    case ContainerInteractStatusV1::accepted: return "accepted";
    case ContainerInteractStatusV1::hit: return "hit";
    case ContainerInteractStatusV1::unknown_declaration: return "unknown_declaration";
    case ContainerInteractStatusV1::no_visual: return "no_visual";
    case ContainerInteractStatusV1::out_of_range: return "out_of_range";
    case ContainerInteractStatusV1::rejected_state: return "rejected_state";
    }
    return "unknown";
}

bool ContainerRuntimeV1::adopt(const AssetCatalog& assets, std::vector<ContainerInstanceV1> instances,
                               std::vector<std::string>& notices, std::string& error) {
    error.clear();
    slots_.clear();
    pending_.clear();
    slots_.reserve(instances.size());
    for (auto& instance : instances) {
        Slot slot;
        slot.instance = std::move(instance);
        slot.instance.visual_ready = false;
        if (!slot.instance.visual_file.empty()) {
            auto visual = std::make_unique<CharacterVisual>();
            std::string loadError;
            bool ok = false;
            try {
                ok = visual->load_embedded_scene(assets, slot.instance.visual_file, loadError) &&
                     !visual->meshes().empty();
                // Closed pose: the authored idle clip when present, else the bind pose.
                if (ok && !visual->select("idle", true, loadError)) loadError.clear();
            } catch (const std::exception& exception) {
                ok = false;
                loadError = exception.what();
            }
            if (ok) {
                // Authored 'opened' marker of the activate clip (milliseconds).
                std::string markerError;
                if (const auto* markers = visual->markers("activate", markerError)) {
                    for (const auto& marker : markers->markers())
                        if (marker.name == "opened") slot.opened_marker_ms = marker.time_ms;
                }
                std::int32_t clipStart = 0, clipEnd = 0;
                if (visual->animation_range("activate", clipStart, clipEnd, markerError)) slot.clip_end_ms = clipEnd;
                // Source Destructible::InitPost: stages = animation count - 3 (only above 3).
                // The clip library is the BDAE animation_clip list, so index = stage clip.
                const auto bytes = assets.read(slot.instance.visual_file);
                std::vector<EmbeddedSceneClip> clips;
                if (decode_embedded_scene_clips(bytes.data(), bytes.size(), clips, loadError)) {
                    for (const auto& clip : clips) slot.clip_names.push_back(clip.name);
                    slot.stages = clips.size() > 3 ? std::uint32_t(clips.size() - 3) : 0u;
                    slot.remaining = slot.stages;
                    loadError.clear();
                    if (slot.instance.family == ContainerFamilyV1::destructible)
                        notices.push_back("destructible clips declaration=" + slot.instance.name + " clips=" +
                                          std::to_string(clips.size()) + " stages=" + std::to_string(slot.stages) +
                                          " script=" + (slot.instance.script.empty() ? std::string("(none)") : slot.instance.script));
                } else {
                    ok = false;
                }
                slot.visual = std::move(visual);
                slot.instance.visual_ready = ok;
                if (!ok) {
                    notices.push_back("container clip library unavailable " + slot.instance.visual_file + ": " + loadError);
                    slot.visual.reset();
                }
            } else {
                notices.push_back("container visual unavailable " + slot.instance.visual_file + ": " + loadError);
            }
        }
        slots_.push_back(std::move(slot));
    }
    return true;
}

std::vector<ContainerRuntimeV1::View> ContainerRuntimeV1::views() const {
    std::vector<View> out;
    out.reserve(slots_.size());
    for (const auto& slot : slots_) {
        View view;
        view.instance = &slot.instance;
        view.visual = slot.visual.get();
        view.state = slot.state;
        out.push_back(view);
    }
    return out;
}

bool ContainerRuntimeV1::find_by_name(const std::string& name, std::size_t& index) const {
    for (std::size_t i = 0; i < slots_.size(); ++i) {
        if (slots_[i].instance.name == name) {
            index = i;
            return true;
        }
    }
    return false;
}

std::int32_t ContainerRuntimeV1::interaction_type(std::size_t index) const {
    return index < slots_.size() ? slots_[index].instance.interaction_type : -1;
}

ContainerInteractResultV1 ContainerRuntimeV1::interact(std::size_t index, const float player_position[3]) {
    ContainerInteractResultV1 result;
    result.index = index;
    if (index >= slots_.size()) {
        result.status = ContainerInteractStatusV1::unknown_declaration;
        return result;
    }
    auto& slot = slots_[index];
    result.state_before = result.state_after = slot.state;
    result.hits_remaining = slot.remaining;
    if (!slot.visual) {
        result.status = ContainerInteractStatusV1::no_visual;
        return result;
    }
    const auto& t = slot.instance.transform;
    const float dx = t[12] - player_position[0];
    const float dy = t[13] - player_position[1];
    result.distance = std::sqrt(dx * dx + dy * dy);
    if (!(result.distance <= kContainerInteractRange)) {
        result.status = ContainerInteractStatusV1::out_of_range;
        return result;
    }
    if (slot.state == kContainerStateActivating || slot.state == kContainerStateOpened) {
        result.status = ContainerInteractStatusV1::rejected_state;
        return result;
    }
    std::string error;
    if (slot.instance.family == ContainerFamilyV1::destructible && slot.remaining > 0) {
        // DestructibleContainer::Interact, non-final hit: decrement, play the stage clip
        // at index (stages - remaining) and the row sound. The state does not change.
        --slot.remaining;
        slot.dirty = true;
        const std::size_t stage = slot.stages - slot.remaining;
        if (stage < slot.clip_names.size() && !slot.visual->select(slot.clip_names[stage], false, error)) {
            result.status = ContainerInteractStatusV1::no_visual;
            return result;
        }
        result.status = ContainerInteractStatusV1::hit;
        result.hits_remaining = slot.remaining;
        result.sound_id = slot.instance.sound_id;
        return result;
    }
    // Container::Interact (openable) and the final destructible hit both set state 3 and
    // play 'activate'. DoOpen runs on the 'opened' event (update), never during the clip.
    slot.dirty = true;
    if (!slot.visual->select("activate", false, error)) {
        // No activate clip: the source's no-visual branch opens at once (DoOpen now).
        slot.state = kContainerStateOpened;
        slot.opened_fired = true;
        ContainerEventV1 event;
        event.kind = ContainerEventKindV1::opened;
        event.index = index;
        event.loot_id = slot.instance.loot_id;
        event.script = slot.instance.script;
        event.name = slot.instance.name;
        pending_.push_back(std::move(event));
        result.status = ContainerInteractStatusV1::accepted;
        result.state_after = slot.state;
        return result;
    }
    slot.state = kContainerStateActivating;
    slot.opened_fired = false;
    result.status = ContainerInteractStatusV1::accepted;
    result.state_after = slot.state;
    return result;
}

bool ContainerRuntimeV1::update(double seconds, std::vector<ContainerEventV1>& events, std::string& error) {
    error.clear();
    for (auto& event : pending_) events.push_back(std::move(event));
    pending_.clear();
    for (std::size_t i = 0; i < slots_.size(); ++i) {
        auto& slot = slots_[i];
        if (!slot.visual) continue;
        if (!slot.visual->update(seconds, error)) return false;
        if (slot.state != kContainerStateActivating) continue;
        // Clock of the visual's own 'activate' clip, the same timeline as the authored markers.
        const double elapsedMs = slot.visual->animation_elapsed_seconds() * 1000.0;
        // Without an authored 'opened' marker the DoOpen event falls back to the clip end.
        const double openAtMs = slot.opened_marker_ms >= 0.0 ? slot.opened_marker_ms : slot.clip_end_ms;
        if (!slot.opened_fired && openAtMs >= 0.0 && elapsedMs >= openAtMs) {
            // Container event 'opened' -> DoOpen. The clip keeps playing; the lid
            // finishes on the activate clip.
            slot.opened_fired = true;
            ContainerEventV1 event;
            event.kind = ContainerEventKindV1::opened;
            event.index = i;
            event.loot_id = slot.instance.loot_id;
            event.elapsed_ms = elapsedMs;
            event.script = slot.instance.script;
            event.name = slot.instance.name;
            events.push_back(std::move(event));
        }
        // Container::__Callback: activate finished while state 3 -> state 4 and idleactive.
        if (slot.clip_end_ms >= 0.0 && elapsedMs >= slot.clip_end_ms) {
            slot.state = kContainerStateOpened;
            slot.dirty = true;
            if (!slot.visual->select("idleactive", true, error)) return false;
        }
    }
    return true;
}

std::vector<std::size_t> ContainerRuntimeV1::take_changed() {
    std::vector<std::size_t> out;
    for (std::size_t i = 0; i < slots_.size(); ++i) {
        if (!slots_[i].dirty) continue;
        slots_[i].dirty = false;
        out.push_back(i);
    }
    return out;
}

bool ContainerRuntimeV1::restore(std::size_t index, std::uint8_t state, std::uint32_t hits_remaining,
                                 std::string& error) {
    error.clear();
    if (index >= slots_.size()) {
        error = "Container restore index outside the loaded declarations";
        return false;
    }
    auto& slot = slots_[index];
    if (slot.instance.family == ContainerFamilyV1::destructible)
        slot.remaining = hits_remaining <= slot.stages ? hits_remaining : slot.stages;
    slot.opened_fired = true;
    slot.dirty = false;
    if (!slot.visual) {
        slot.state = state == kContainerStateActivating ? kContainerStateOpened : state;
        return true;
    }
    if (state == kContainerStateOpened || state == kContainerStateActivating) {
        slot.state = kContainerStateOpened;
        if (!slot.visual->select("idleactive", true, error)) return false;
        return true;
    }
    slot.state = kContainerStateIdle;
    return slot.visual->select("idle", true, error);
}

} // namespace dh::foundation::containers
