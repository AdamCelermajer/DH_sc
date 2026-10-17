#include "../character_animation_frame_v1.hpp"

#include <cstdio>
#include <stdexcept>
#include <vector>

namespace frame = dh2::character::animation_frame_v1;

namespace {
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
struct Trace {
    std::vector<unsigned> calls;
    std::uint32_t dt = 0;
    unsigned fail = 0;
    static int state(void* raw, std::uint32_t dt) {
        auto& self = *static_cast<Trace*>(raw);
        self.calls.push_back(1); self.dt = dt; return self.fail == 1;
    }
    static int animator(void* raw, std::uint32_t dt) {
        auto& self = *static_cast<Trace*>(raw);
        self.calls.push_back(2); self.dt = dt; return self.fail == 2;
    }
    static int game_object(void* raw, std::uint32_t dt) {
        auto& self = *static_cast<Trace*>(raw);
        self.calls.push_back(3); self.dt = dt; return self.fail == 3;
    }
};
}

int main() {
    try {
        Trace trace;
        const frame::Providers providers{
            &trace, Trace::state, Trace::animator, Trace::game_object};
        frame::Result result{};
        require(frame::advance(37, &providers, &result) == frame::Status::complete &&
                trace.calls == std::vector<unsigned>{1, 2, 3} && trace.dt == 37 &&
                result.phase == frame::Phase::complete &&
                result.failed_phase == frame::Phase::none,
                "Character update tail did not run state -> animator -> GameObject");

        trace.calls.clear(); trace.fail = 2;
        require(frame::advance(19, &providers, &result) == frame::Status::provider_failed &&
                trace.calls == std::vector<unsigned>{1, 2} &&
                result.phase == frame::Phase::animator &&
                result.failed_phase == frame::Phase::animator,
                "animator failure did not stop before GameObject update");
        std::puts("PASS: Character tail order state -> CharAnimator::Update -> GameObject::Update");
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "character animation frame: %s\n", e.what());
        return 1;
    }
}
