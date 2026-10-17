#include "../character_ai_events.hpp"
#include <cstdint>
#include <cstdio>
#include <stdexcept>

namespace {
using namespace dh2::character;
constexpr std::uint32_t renderer_move_event = 0xc351u;
struct Capture {
    std::uint32_t calls = 0;
    AIEventRequest40 request{};
};
int capture(void* context, AIEventState64*, const AIEventRequest40* request,
            std::uint32_t* value) {
    auto& state = *static_cast<Capture*>(context);
    if (!request || !value || state.calls++) return 1;
    state.request = *request;
    *value = 0;
    return 0;
}
void require(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}
}

int main() {
    try {
        AIEventOwner48 owner{0x1000, 0x2000, 0x3000, 0x4000, 0, 0, 0, 0};
        AIEventState64 state{0x5000, &owner, nullptr, 0, 0, 0, 0, 0, 0, 0, 0};
        AIEventPayload24 payload{0x6000, 0, 0, 0};
        Capture captured{};
        AIEventServices24 services{&captured, capture,
            1u << ai_event_state_event, 0};
        AIEventResult16 result{};

        // Source CharAI::RaiseAIEvent(0) dispatches decimal 50001 to the
        // canonical Character state machine. Android currently enters that
        // same state machine directly with event 0xc351.
        constexpr std::uint32_t source_move_event = 50001u;
        static_assert(source_move_event == renderer_move_event,
                      "source and renderer movement event IDs diverged");
        require(dh2_character_ai_event(&result, &state, 0, &payload,
                                       &services) == 0,
                "source movement event dispatch failed");
        require(captured.calls == 1 && result.service_calls == 1 &&
                    result.last_service == ai_event_state_event,
                "source movement event must dispatch exactly once");
        require(captured.request.event == renderer_move_event &&
                    captured.request.subject == owner.state_machine &&
                    captured.request.operation == 0 &&
                    captured.request.argument == 0 &&
                    captured.request.callee == 0 &&
                    captured.request.payload == payload.value,
                "source movement dispatch differs from renderer C351 route");
        std::puts("PASS: CharAI event 0 and renderer C351 are one state-machine event");
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "%s\n", error.what());
        return 1;
    }
}
