#include "../character_coordinator.hpp"

#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

namespace cast = dh2::character_cast_lifecycle_v1;
using namespace dh2::character;

namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Fixture {
    Coordinator coordinator{0x710000001ull, 1};
    Facts facts{};
    std::uint8_t ooi_intent = 1;
    cast::Projection projection{};
    std::vector<int> order;
    bool fail_cast_event = false;

    Fixture() {
        facts.is_player = 1;
        facts.idle = 11;
        facts.walk = 22;
        facts.run = 33;
        facts.attack_static = 44;
        facts.attack_moving = 55;
        facts.walk_threshold = .45f;
        facts.run_threshold = .85f;
        facts.walk_speed = 1.3f;
        projection.character = {coordinator.owner(), &coordinator.state,
            &coordinator.state.flags, 0x710000002ull, 0x710000003ull,
            &ooi_intent};
        projection.globals = {0x710000004ull};
        projection.services = {this, cast_service};
        CoordinatorBindings bindings{};
        bindings.context = this;
        bindings.facts = read_facts;
        bindings.services = {this, state_service};
        coordinator.bind(bindings);
    }

    static Facts read_facts(void* context) {
        return static_cast<Fixture*>(context)->facts;
    }
    static void state_service(void* context, State* state,
                              const Request* request) {
        auto& self = *static_cast<Fixture*>(context);
        check(state == &self.coordinator.state,
              "generic state service changed Coordinator owner");
        if (request->service == set_animation)
            state->current_animation = request->argument[0];
        if (request->service == raise_event && request->argument[0] == 0x1d)
            self.order.push_back(100 + request->argument[1]);
    }
    static int cast_service(void* context, const cast::Request* request,
                            cast::Response* response) {
        auto& self = *static_cast<Fixture*>(context);
        switch (request->operation) {
        case cast::Operation::debug_load: self.order.push_back(1); break;
        case cast::Operation::string_construct:
            self.order.push_back(2);
            if (!request->text ||
                std::string(request->text) != "isTracingCharState")
                return -1;
            response->identity = 0x710000005ull;
            break;
        case cast::Operation::debug_query: self.order.push_back(3); break;
        case cast::Operation::string_destroy: self.order.push_back(4); break;
        case cast::Operation::raise_event:
            self.order.push_back(static_cast<int>(request->argument));
            if (request->argument == 32u) {
                check(self.coordinator.state.current == 7 &&
                          self.coordinator.state.elapsed_ms == 0 &&
                          self.coordinator.state.flags == 25345u &&
                          self.ooi_intent == 1,
                      "Focus RaiseEvent32 ran before state7/flags commit or after OOI clear");
            } else if (request->argument == 33u) {
                check(self.coordinator.state.current == 7,
                      "Blur RaiseEvent33 ran after leaving state7");
            } else {
                return -1;
            }
            if (self.fail_cast_event) return -1;
            break;
        case cast::Operation::set_animation:
            self.order.push_back(5);
            if (request->subject != self.projection.character.machine ||
                request->argument != UINT32_MAX) return -1;
            break;
        case cast::Operation::set_speed:
            self.order.push_back(6);
            if (request->subject != self.projection.character.animator ||
                request->argument != 0x3f800000u) return -1;
            break;
        case cast::Operation::cancel_sneaking:
            self.order.push_back(7);
            if (self.ooi_intent != 0) return -1;
            break;
        }
        return 0;
    }

    void bind() {
        check(coordinator.bind_cast_projection(&projection),
              "cast callback projection did not borrow Coordinator state");
    }
};

void c356_focus_update_and_c34_exit() {
    Fixture fixture;
    check(fixture.coordinator.transition(3) == 1,
          "cast fixture could not enter Idle");
    fixture.bind();
    fixture.order.clear();
    fixture.coordinator.state.elapsed_ms = 77;

    check(fixture.coordinator.event(50006u) == 1 &&
              fixture.coordinator.state.current == 7 &&
              fixture.coordinator.state.elapsed_ms == 0,
          "C356 did not enter CSCast with elapsed reset");
    const std::vector<int> focus_expected{1, 2, 3, 4, 32, 5, 6, 7, 103};
    check(fixture.order == focus_expected,
          "cast Focus/Coordinator event order differs");
    check(fixture.coordinator.update_state(19) == 1 &&
              fixture.coordinator.state.current == 7 &&
              fixture.coordinator.state.elapsed_ms == 19,
          "CSCast source no-op update did not retain elapsed time");
    check(!fixture.coordinator.unbind_cast_projection(&fixture.projection),
          "cast projection detached while state7 Blur still needs it");

    fixture.order.clear();
    check(fixture.coordinator.event(34u) == 1 &&
              fixture.coordinator.state.current == 3 &&
              fixture.coordinator.state.elapsed_ms == 0,
          "CSCast event34 did not return to Idle and reset elapsed time");
    const std::vector<int> blur_expected{1, 2, 3, 4, 33, 107};
    check(fixture.order == blur_expected,
          "cast Blur/Idle Focus/Coordinator event order differs");
    check(fixture.coordinator.unbind_cast_projection(&fixture.projection),
          "cast projection could not detach after leaving state7");
    const auto flags = fixture.coordinator.state.flags;
    check(fixture.coordinator.event(50006u) == -1 &&
              fixture.coordinator.state.current == 3 &&
              fixture.coordinator.state.flags == flags,
          "cast entry ran after projection was detached");
}

void c358_death_transition_and_binding_guards() {
    Fixture fixture;
    check(fixture.coordinator.transition(3) == 1,
          "C358 fixture could not enter Idle");
    fixture.bind();
    State foreign_state{};
    auto wrong = fixture.projection;
    wrong.character.coordinator_state = &foreign_state;
    check(!fixture.coordinator.bind_cast_projection(&wrong),
          "cast projection accepted a different Coordinator State");
    auto replacement = fixture.projection;
    check(!fixture.coordinator.bind_cast_projection(&replacement),
          "cast projection binding was replaceable");

    check(fixture.coordinator.event(50006u) == 1,
          "C358 fixture could not enter CSCast");
    fixture.order.clear();
    fixture.coordinator.state.elapsed_ms = 42;
    check(fixture.coordinator.event(50008u) == 1 &&
              fixture.coordinator.state.current == 12 &&
              fixture.coordinator.state.elapsed_ms == 0,
          "C358 did not leave CSCast for state12 with elapsed reset");
    check(!fixture.order.empty() && fixture.order.front() == 1 &&
              fixture.order[4] == 33 && fixture.order.back() == 107,
          "C358 did not run Cast Blur before state12 focus/event");
    check(fixture.coordinator.unbind_cast_projection(&fixture.projection),
          "cast projection could not detach after C358 left state7");
}

void failure_prefix_retains_cast_state() {
    Fixture fixture;
    check(fixture.coordinator.transition(3) == 1,
          "failure fixture could not enter Idle");
    fixture.bind();
    fixture.fail_cast_event = true;
    fixture.coordinator.state.elapsed_ms = 91;
    check(fixture.coordinator.event(50006u) == -1 &&
              fixture.coordinator.state.current == 7 &&
              fixture.coordinator.state.elapsed_ms == 0 &&
              fixture.coordinator.state.flags == 25345u &&
              fixture.order == std::vector<int>({1, 2, 3, 4, 32}) &&
              !fixture.coordinator.unbind_cast_projection(&fixture.projection),
          "failed Cast Focus rolled back or ran beyond its completed prefix");
}
} // namespace

int main() {
    try {
        c356_focus_update_and_c34_exit();
        c358_death_transition_and_binding_guards();
        failure_prefix_retains_cast_state();
        std::printf("{\"suite\":\"character_cast_coordinator_v1\","
                    "\"c356_focus_event_order\":true,"
                    "\"c34_blur_event_order\":true,"
                    "\"c358_death_transition\":true,"
                    "\"elapsed_reset_and_state7_update\":true,"
                    "\"borrowed_projection_guards\":true,"
                    "\"failure_prefix\":true}\n");
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "character cast coordinator: %s\n", error.what());
        return 1;
    }
}
