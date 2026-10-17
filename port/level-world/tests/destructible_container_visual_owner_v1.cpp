#include "../destructible_container_visual_owner_v1.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace visual = dh2::destructible_container_visual_owner_v1;

namespace {
struct Trace {
    visual::Owner* owner{};
    std::vector<std::string> events;
    std::vector<std::string> callbacks;
    std::uint32_t completions{};
};

bool event(void* raw, const char* name, std::int32_t, std::string& error) {
    auto& trace = *static_cast<Trace*>(raw);
    if (!name) {
        error = "event name is null";
        return false;
    }
    trace.events.emplace_back(name);
    trace.callbacks.emplace_back(std::string("event:") + name);
    error.clear();
    return true;
}

bool completion(void* raw, std::string& error) {
    auto& trace = *static_cast<Trace*>(raw);
    ++trace.completions;
    trace.callbacks.emplace_back("completion");
    error.clear();
    return true;
}

std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot open BDAE: " + path);
    return {std::istreambuf_iterator<char>(input), {}};
}

void require(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

void audit(const std::string& path) {
    const auto bytes = read(path);
    visual::Owner owner;
    std::string error;
    require(owner.load(bytes.data(), bytes.size(), error), error);
    require(owner.controller_present() && owner.root_animator_count() == 1,
            "BDAE did not retain one root Animator");
    auto find = [&](const char* name) -> const visual::Clip* {
        const auto found = std::find_if(owner.clips().begin(), owner.clips().end(),
            [&](const visual::Clip& clip) { return clip.name == name; });
        return found == owner.clips().end() ? nullptr : &*found;
    };
    const auto* idle = find("idle");
    const auto* activate = find("activate");
    const auto* idleactive = find("idleactive");
    require(idle && idle->start_ms == 0 && idle->end_ms == 166,
            "authored idle interval differs");
    require(activate && activate->start_ms == 166 && activate->end_ms == 1899,
            "authored activate interval differs");
    require(idleactive && idleactive->start_ms == 1899 && idleactive->end_ms == 2066,
            "authored idleactive interval differs");
    require(owner.start_initial_state(error) && owner.active_clip() == "idle", error);
    require((owner.resource().scene.graph.front().source_flags_284 & 0x400u) != 0,
            "Container::SetState(2) did not set root SceneNode +0x284 bit 0x400");
    require(owner.set_container_state(3, error), error);
    require((owner.resource().scene.graph.front().source_flags_284 & 0x400u) == 0,
            "Container::SetState(3) did not clear root SceneNode +0x284 bit 0x400");
    require(owner.set_container_state(4, error), error);
    require((owner.resource().scene.graph.front().source_flags_284 & 0x400u) != 0,
            "Container::SetState(4) did not set root SceneNode +0x284 bit 0x400");
    require(owner.set_current_frame(4, error), error);

    Trace trace{};
    trace.owner = &owner;
    require(owner.play_activate(event, completion, &trace, error), error);
    require(owner.update(0, error), error);
    require(owner.update(5000, error), error);
    require(std::find(trace.events.begin(), trace.events.end(), "opened") !=
                trace.events.end(),
            "activate timeline did not dispatch the authored opened event");
    require(std::find(trace.events.begin(), trace.events.end(), "fx") !=
                trace.events.end(),
            "activate timeline did not dispatch the authored fx event");
    require(trace.completions == 1,
            "finite activate clip did not dispatch one separate completion");
    const auto opened = std::find(trace.callbacks.begin(), trace.callbacks.end(),
                                  "event:opened");
    const auto completed = std::find(trace.callbacks.begin(), trace.callbacks.end(),
                                     "completion");
    require(opened != trace.callbacks.end() && completed != trace.callbacks.end() &&
                opened < completed,
            "authored named event did not precede the separate completion callback");
    require(owner.play_named("idleactive", true, event, completion, &trace, error), error);
    require(owner.active_clip() == "idleactive", "idleactive clip was not selected");
    require(owner.update(6000, error), error);
    require(trace.completions == 1,
            "looping idleactive clip unexpectedly completed");
    require(owner.detach() && !owner.controller_present(),
            "visual detach retained the root Animator");
}
} // namespace

int main(int argc, char** argv) {
    try {
        if (argc != 3) return 2;
        audit(argv[1]);
        audit(argv[2]);
        std::cout << "{\"assets\":2,\"root_animators_per_asset\":1,"
                     "\"activate_events\":true,\"fx_events\":true,"
                     "\"finite_completion\":true,"
                     "\"idleactive_loop\":true}\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
