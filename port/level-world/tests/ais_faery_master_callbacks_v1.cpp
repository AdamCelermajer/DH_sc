#include "../ais_faery_master_callbacks_v1.hpp"
#include "../character_controller_commands.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <vector>

namespace callback = dh2::ais_faery_master_callbacks_v1;
namespace {
constexpr std::uintptr_t ais = 0x100000001ull;
constexpr std::uintptr_t faery = 0x200000002ull;
constexpr std::uintptr_t master = 0x300000003ull;
constexpr std::uintptr_t controller = 0x400000004ull;

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

struct Call {
    callback::Operation operation;
    std::uintptr_t subject;
    callback::Command command;
    callback::Point3 point;
};

struct Fixture {
    std::vector<Call> calls;
    callback::Point3 faery_position{{1.0f, 2.0f, 3.0f}};
    callback::Point3 master_position{{11.0f, 22.0f, 33.0f}};
    callback::Point3 master_look{{0.0f, 0.0f, 1.0f}};
    bool faery_idle{true};
    bool master_idle{true};
    int fail_at{-1};

    static std::int32_t invoke(void* context, const callback::Request* request,
                               callback::Response* response) {
        auto& fixture = *static_cast<Fixture*>(context);
        fixture.calls.push_back({request->operation, request->subject,
                                 request->command, request->point});
        const auto index = static_cast<int>(fixture.calls.size()) - 1;
        if (index == fixture.fail_at) return -1;
        if (request->operation == callback::Operation::target_position) {
            response->point = request->subject == master
                ? fixture.master_position : fixture.faery_position;
        } else if (request->operation == callback::Operation::look_at_vector) {
            response->point = fixture.master_look;
        } else if (request->operation == callback::Operation::state_machine_idle) {
            response->word = request->subject == faery
                ? fixture.faery_idle : fixture.master_idle;
        }
        return 0;
    }
};

callback::State state() { return {ais, faery, master, controller}; }
callback::Services services(Fixture& fixture) { return {&fixture, Fixture::invoke}; }

void require_point(const callback::Point3& actual, float x, float y, float z,
                   const char* message) {
    require(std::fabs(actual.value[0] - x) < 0.0001f &&
            std::fabs(actual.value[1] - y) < 0.0001f &&
            std::fabs(actual.value[2] - z) < 0.0001f, message);
}

struct PointControllerFixture {
    std::vector<dh2::character::ControllerPointCommandRequest32> calls;
    static int invoke(void* context,
                      const dh2::character::ControllerPointCommandRequest32* request) {
        static_cast<PointControllerFixture*>(context)->calls.push_back(*request);
        return 1;
    }
};

void point_controller_gate() {
    using namespace dh2::character;
    PointControllerFixture fixture;
    ControllerPointCommandServices16 bound{&fixture, PointControllerFixture::invoke};
    ControllerCommandState32 controller_state{0x100000011ull, 0x200000022ull,
                                               1, 0, 0, 0};
    const float point[3]{7.0f, -11.0f, 13.0f};
    require(dh2_character_controller_point_command(&controller_state,
                controller_warp_to_point, point, &bound) == 1 && fixture.calls.empty(),
            "blocked point command must not reach controllable");
    controller_state.forced = 1;
    require(dh2_character_controller_point_command(&controller_state,
                controller_head_towards_point, point, &bound) == 1 &&
                fixture.calls.size() == 1 &&
                fixture.calls[0].command == controller_head_towards_point &&
                fixture.calls[0].owner == controller_state.owner &&
                fixture.calls[0].point[0] == 7.0f &&
                fixture.calls[0].point[1] == -11.0f &&
                fixture.calls[0].point[2] == 13.0f,
            "forced point command dispatch mismatch");
}
}

int main() {
    try {
        point_controller_gate();
        Fixture fixture;
        auto current = state();
        auto bound = services(fixture);
        callback::Result result{};

        require(callback::dispatch(&current, 0x14, &bound, &result) ==
                    callback::Status::complete, "out-of-sight callback failed");
        require(fixture.calls.size() == 2, "out-of-sight call count");
        require(fixture.calls[0].operation == callback::Operation::target_position &&
                    fixture.calls[0].subject == master, "warp target lookup order");
        require(fixture.calls[1].operation == callback::Operation::controller_point_command &&
                    fixture.calls[1].subject == controller &&
                    fixture.calls[1].command == callback::Command::warp_to_point,
                "warp controller dispatch");
        require_point(result.point, 11.0f, 22.0f, 33.0f, "warp target position");

        fixture.calls.clear();
        require(callback::dispatch(&current, 0x16, &bound, &result) ==
                    callback::Status::complete, "out-of-range callback failed");
        require(fixture.calls.size() == 3, "head-towards call count");
        require(fixture.calls[0].subject == master && fixture.calls[1].subject == faery,
                "head-towards position order");
        require(fixture.calls[2].command == callback::Command::head_towards_point,
                "head-towards controller dispatch");
        require_point(result.point, 10.0f, 20.0f, 30.0f, "head-towards delta");

        fixture.calls.clear();
        require(callback::dispatch(&current, 0x19, &bound, &result) ==
                    callback::Status::complete, "melee callback failed");
        require(fixture.calls.size() == 5, "head-to call count");
        require(fixture.calls[0].operation == callback::Operation::state_machine_idle &&
                    fixture.calls[0].subject == faery &&
                    fixture.calls[1].subject == master,
                "idle checks not source ordered");
        require(fixture.calls[2].operation == callback::Operation::look_at_vector &&
                    fixture.calls[3].operation == callback::Operation::target_position,
                "head-to look/position order");
        require(fixture.calls[4].command == callback::Command::head_to_point,
                "head-to controller dispatch");
        require_point(result.point, 11.0f, 22.0f, -167.0f, "head-to point");

        fixture.calls.clear();
        fixture.faery_idle = false;
        require(callback::dispatch(&current, 0x19, &bound, &result) ==
                    callback::Status::complete && fixture.calls.size() == 1 &&
                    !result.command_issued, "non-idle faery must not move");

        fixture.calls.clear();
        require(callback::dispatch(&current, 0x15, &bound, &result) ==
                    callback::Status::unsupported_event && fixture.calls.empty(),
                "unowned AIS event was consumed");

        fixture.calls.clear();
        fixture.faery_idle = true;
        fixture.fail_at = 1;
        require(callback::dispatch(&current, 0x16, &bound, &result) ==
                    callback::Status::failed && fixture.calls.size() == 2 &&
                    !result.command_issued, "failed service must stop at source prefix");

        std::puts("{\"ais_faery_master_callbacks\":\"PASS\",\"source_events\":3,\"point_controller_dispatch\":true,\"unowned_events_preserved\":true}");
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "%s\n", error.what());
        return 1;
    }
}
