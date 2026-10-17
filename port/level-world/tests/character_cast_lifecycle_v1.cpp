#include "../character_cast_lifecycle_v1.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

namespace cast = dh2::character_cast_lifecycle_v1;

namespace {
constexpr cast::Operation focus_order[] = {
    cast::Operation::debug_load, cast::Operation::string_construct,
    cast::Operation::debug_query, cast::Operation::string_destroy,
    cast::Operation::raise_event, cast::Operation::set_animation,
    cast::Operation::set_speed, cast::Operation::cancel_sneaking};
constexpr cast::Operation blur_order[] = {
    cast::Operation::debug_load, cast::Operation::string_construct,
    cast::Operation::debug_query, cast::Operation::string_destroy,
    cast::Operation::raise_event};

int failures = 0;
int checks = 0;

void expect(bool condition, const char* name) {
    ++checks;
    if (!condition) {
        ++failures;
        std::fprintf(stderr, "FAIL %s\n", name);
    }
}

struct Fixture {
    dh2::character::State coordinator_state{};
    cast::Character character{};
    cast::Globals globals{0x2222};
    std::uint8_t ooi_intent = 1;
    cast::Operation calls[16]{};
    std::uint32_t arguments[16]{};
    std::uintptr_t subjects[16]{};
    unsigned count = 0;
    int fail_at = -1;
    bool focus_event_saw_flags = false;
    bool focus_event_saw_intent = false;
    bool cancel_saw_cleared_intent = false;

    Fixture() {
        coordinator_state.current = 7;
        coordinator_state.flags = 9;
        character.identity = 0x1111;
        character.coordinator_state = &coordinator_state;
        character.flags_520 = &coordinator_state.flags;
        character.machine = 0x3333;
        character.animator = 0x4444;
        character.ooi_intent_412 = &ooi_intent;
    }

    static int invoke(void* context, const cast::Request* request,
                      cast::Response* response) {
        auto& self = *static_cast<Fixture*>(context);
        const auto index = self.count++;
        self.calls[index] = request->operation;
        self.arguments[index] = request->argument;
        self.subjects[index] = request->subject;
        if (request->operation == cast::Operation::raise_event &&
            request->argument == 32u) {
            self.focus_event_saw_flags = self.coordinator_state.flags == 25345u;
            self.focus_event_saw_intent = self.ooi_intent == 1;
        }
        if (request->operation == cast::Operation::cancel_sneaking)
            self.cancel_saw_cleared_intent = self.ooi_intent == 0;
        if (request->operation == cast::Operation::string_construct) {
            expect(request->text &&
                       std::strcmp(request->text, "isTracingCharState") == 0,
                   "construct source tracing key");
            response->identity = 0x5555;
        }
        return static_cast<int>(index) == self.fail_at ? -1 : 0;
    }

    cast::Services services() { return {this, invoke}; }
};

void focus_order_test() {
    Fixture fixture;
    auto services = fixture.services();
    cast::Result result{};
    const auto status = cast::execute(cast::Callback::focus, &fixture.character,
                                      &fixture.globals, &services, &result);
    expect(status == cast::Status::complete && result.complete == 1,
           "focus completes");
    expect(fixture.count == sizeof(focus_order) / sizeof(focus_order[0]),
           "focus operation count");
    for (unsigned i = 0; i < fixture.count; ++i)
        expect(fixture.calls[i] == focus_order[i], "focus exact source order");
    expect(fixture.subjects[0] == fixture.globals.debug_switches &&
               fixture.subjects[4] == fixture.character.identity &&
               fixture.arguments[4] == 32u,
           "focus debug and RaiseEvent32 owners");
    expect(fixture.subjects[5] == fixture.character.machine &&
               fixture.arguments[5] == UINT32_MAX,
           "focus SM_SetAnim(-1)");
    expect(fixture.subjects[6] == fixture.character.animator &&
               fixture.arguments[6] == 0x3f800000u,
           "focus animation speed one");
    expect(fixture.focus_event_saw_flags && fixture.focus_event_saw_intent,
           "RaiseEvent32 observes flags write before OOI clear");
    expect(fixture.cancel_saw_cleared_intent && fixture.ooi_intent == 0 &&
               fixture.coordinator_state.flags == 25345u &&
               result.flags_written == 1 && result.ooi_intent_cleared == 1,
           "focus clears OOI before CancelSneaking");
}

void blur_order_test() {
    Fixture fixture;
    fixture.coordinator_state.flags = 0xabcdef01u;
    const auto flags_before = fixture.coordinator_state.flags;
    auto services = fixture.services();
    cast::Result result{};
    const auto status = cast::execute(cast::Callback::blur, &fixture.character,
                                      &fixture.globals, &services, &result);
    expect(status == cast::Status::complete && result.complete == 1,
           "blur completes");
    expect(fixture.count == sizeof(blur_order) / sizeof(blur_order[0]),
           "blur operation count");
    for (unsigned i = 0; i < fixture.count; ++i)
        expect(fixture.calls[i] == blur_order[i], "blur exact source order");
    expect(fixture.arguments[4] == 33u &&
               fixture.subjects[4] == fixture.character.identity,
           "blur raises event33 on same Character");
    expect(fixture.coordinator_state.flags == flags_before &&
               fixture.ooi_intent == 1 && result.flags_written == 0 &&
               result.ooi_intent_cleared == 0,
           "blur does not invent focus writes");
}

void failure_prefix_test(cast::Callback callback, const cast::Operation* order,
                         unsigned length) {
    for (unsigned failure_at = 0; failure_at < length; ++failure_at) {
        Fixture fixture;
        fixture.fail_at = static_cast<int>(failure_at);
        auto services = fixture.services();
        cast::Result result{};
        const auto status = cast::execute(callback, &fixture.character,
                                          &fixture.globals, &services, &result);
        expect(status == cast::Status::service_failed,
               "provider failure is surfaced");
        expect(fixture.count == failure_at + 1 && result.calls == failure_at + 1,
               "failure stops at exact prefix");
        for (unsigned i = 0; i < fixture.count; ++i)
            expect(fixture.calls[i] == order[i], "failure retains source prefix");
        expect(result.complete == 0, "failed callback is incomplete");
        if (callback == cast::Callback::focus) {
            expect(result.flags_written == (failure_at >= 4 ? 1u : 0u),
                   "failure preserves only prior flags write");
            expect(result.ooi_intent_cleared == (failure_at >= 7 ? 1u : 0u),
                   "failure preserves only prior OOI clear");
        }
    }
}
} // namespace

int main() {
    focus_order_test();
    blur_order_test();
    failure_prefix_test(cast::Callback::focus, focus_order,
                        sizeof(focus_order) / sizeof(focus_order[0]));
    failure_prefix_test(cast::Callback::blur, blur_order,
                        sizeof(blur_order) / sizeof(blur_order[0]));

    std::printf("{\"suite\":\"character_cast_lifecycle_v1\","
                "\"checks\":%d,\"failures\":%d}\n", checks, failures);
    return failures == 0 ? 0 : 1;
}
