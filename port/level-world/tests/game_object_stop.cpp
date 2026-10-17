#include "../game_object_stop.hpp"
#include "../navigation_path.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace stop = dh2::game_object_stop;
namespace physical = dh2::physical;
// navigation_path.cpp also defines FindPath, which this Stop test never calls.
// Resolve its unrelated route dependency explicitly so the focused host link
// can retain the real dh2_nav_drop_path implementation only.
extern "C" int dh2_nav_route(dh2::navigation::RouteResult*,
    const dh2::navigation::RouteRequest*, dh2::navigation::RouteWorkspace*) {
    return 1;
}
namespace {
struct PathFixture {
    std::uint32_t count, owned;
    float position[3], target[3];
};
static_assert(sizeof(PathFixture) == 32);

void require(bool value, const char* message = "GameObject Stop host assertion") {
    static std::uint32_t number = 0;
    ++number;
    if (!value) throw std::runtime_error(std::string(message) + " #" + std::to_string(number));
}
template<class T> T read(std::ifstream& stream) {
    T value{};
    require(bool(stream.read(reinterpret_cast<char*>(&value), sizeof(value))),
            "truncated Stop reference corpus");
    return value;
}
bool same_float(float a, float b) {
    std::uint32_t left, right;
    std::memcpy(&left, &a, 4);
    std::memcpy(&right, &b, 4);
    if (left == right) return true;
    return std::isnan(a) && std::isnan(b);
}
bool same_body(const physical::BodyState& a, const physical::BodyState& b) {
    if (a.flags != b.flags) return false;
    const float* left = reinterpret_cast<const float*>(&a.position[0]);
    const float* right = reinterpret_cast<const float*>(&b.position[0]);
    for (unsigned i = 0; i < 11; ++i)
        if (!same_float(left[i], right[i])) return false;
    return true;
}
std::string body_words(const physical::BodyState& body) {
    const auto* words = reinterpret_cast<const std::uint32_t*>(&body);
    std::string result;
    for (unsigned i = 0; i < sizeof(body) / 4; ++i)
        result += std::to_string(words[i]) + ",";
    return result;
}
struct Context {
    physical::BodyState body{};
    physical::TransformRequest transform{};
    dh2::navigation::PathObject path{};
    std::array<dh2::navigation::PathSegment, 2> path_segments{};
    std::uint32_t updates_position_from_physics = 0;
    int fail_operation = -1;
    std::uintptr_t resolved_identity = 0;
    std::vector<std::uint32_t> trace;
    std::vector<std::uintptr_t> subjects;

    static std::int32_t invoke(void* raw, stop::State* state,
            const stop::Request* request, stop::Response* response) {
        auto& context = *static_cast<Context*>(raw);
        require(request && response, "request/response null");
        require(request->reserved == 0, "request reserved not zero");
        context.trace.push_back(static_cast<std::uint32_t>(request->operation));
        context.subjects.push_back(request->subject_identity);
        if (static_cast<int>(request->operation) == context.fail_operation)
            return 1;
        switch (request->operation) {
        case stop::Operation::drop_path:
            require(request->subject_identity == state->path_identity);
            return dh2_nav_drop_path(&context.path);
        case stop::Operation::is_updating_position_from_physics:
            require(request->subject_identity == state->object_identity);
            response->word = context.updates_position_from_physics;
            return 0;
        case stop::Operation::set_linear_velocity:
            require(request->subject_identity != 0);
            return dh2_physical_set_linear(&context.body, request->values);
        case stop::Operation::set_angular_velocity:
            require(request->subject_identity != 0);
            return dh2_physical_set_angular(&context.body, request->values);
        case stop::Operation::set_position: {
            require(request->subject_identity != 0);
            physical::TransformRequest transform{};
            if (dh2_physical_request_position(&transform, &context.body,
                                               request->values)) return 1;
            require(transform.pending == 1);
            context.transform = transform;
            context.body.position[0] = transform.position[0];
            context.body.position[1] = transform.position[1];
            context.trace.push_back(5);  // backend SetXForm boundary
            return 0;
        }
        }
        return 1;
    }
    static physical::BodyState* resolve(void* raw, std::uintptr_t identity) {
        auto& context = *static_cast<Context*>(raw);
        context.resolved_identity = identity;
        return identity ? &context.body : nullptr;
    }
};

void guards() {
    Context context;
    context.path.position[0] = 10.f; context.path.position[1] = 20.f; context.path.position[2] = 30.f;
    context.path.target[0] = 40.f; context.path.target[1] = 50.f; context.path.target[2] = 60.f;
    context.path.segments = context.path_segments.data();
    context.path.count = context.path.owned = 1;
    context.path.capacity = static_cast<std::uint32_t>(context.path_segments.size());
    context.path_segments[0].edge = 0xFFFFFFFFu;
    stop::State state{0x100000001ull, 0x200000003ull, 0x300000005ull,
        {10.f, 20.f, 30.f}, {1.f, 2.f, 3.f}, {4.f, 5.f, 6.f}, 1, 1, {}};
    stop::Services services{&context, Context::invoke, Context::resolve};
    stop::Result result{91, 92, 93, 94};
    const auto saved = state;
    auto missing_path = services;
    missing_path.invoke = nullptr;
    require(stop::execute(&state, &missing_path, &result) == stop::Status::service_unavailable);
    require(std::memcmp(&state, &saved, sizeof(state)) == 0 && result.phase == 91);

    context.fail_operation = static_cast<int>(stop::Operation::drop_path);
    require(stop::execute(&state, &services, &result) == stop::Status::service_failed);
    require(state.moving == 1 && state.heading_active == 1 && state.destination[0] == 1.f);
    require(context.trace.size() == 1 && context.path.count == 1);

    context.fail_operation = -1;
    context.trace.clear();
    context.updates_position_from_physics = 0;
    require(stop::execute(&state, &services, &result) == stop::Status::complete);
    require(context.path.count == 0 && context.path.owned == 0);
    require(state.destination[0] == 10.f && state.moving == 0 && state.heading_active == 0);
    require(context.trace.size() == 2 && context.trace[0] == 0 && context.trace[1] == 1);

    context.trace.clear();
    context.subjects.clear();
    context.updates_position_from_physics = 1;
    auto missing_resolver = services;
    missing_resolver.resolve_body = nullptr;
    const auto before_body = context.body;
    require(stop::execute(&state, &missing_resolver, &result) == stop::Status::service_unavailable);
    require(context.trace.size() == 6 && context.trace[0] == 0 && context.trace[1] == 1 &&
            context.trace[2] == 2 && context.trace[3] == 3 && context.trace[4] == 4 &&
            context.trace[5] == 5);
    require(context.body.flags == before_body.flags);

    // Full-width subject identity must survive the callback protocol.
    context.trace.clear();
    context.subjects.clear();
    require(stop::execute(&state, &services, &result) == stop::Status::complete);
    require(context.subjects[1] == 0x100000001ull &&
            context.subjects[2] == 0x300000005ull && result.phase == 7);

    // Each failed source provider preserves only the exact completed prefix;
    // no later setter or inline body reset is synthesized.
    for (int failing = 1; failing <= 4; ++failing) {
        context.trace.clear();
        context.fail_operation = failing;
        context.body = {};
        context.updates_position_from_physics = 1;
        require(stop::execute(&state, &services, &result) == stop::Status::service_failed);
        require(context.trace.size() == static_cast<std::size_t>(failing + 1));
        require(result.physical_setters == static_cast<std::uint32_t>(failing - 2 > 0 ? failing - 2 : 0));
    }
}
}

int main(int argc, char** argv) {
    try {
        if (argc == 2 && std::strcmp(argv[1], "--guards") == 0) {
            guards();
            std::cout << "{\"guard_checks\":7,\"service_failure_prefixes\":4}\n";
            return 0;
        }
        require(argc == 2, "usage: game_object_stop_host reference.bin | --guards");
        std::ifstream input(argv[1], std::ios::binary);
        require(bool(input), "cannot open source Stop reference");
        require(read<std::array<char, 4>>(input) == std::array<char, 4>{'G','O','S','1'});
        const auto count = read<std::uint32_t>(input);
        std::uint64_t physical = 0, emitted_transforms = 0, exact_float_words = 0;
        for (std::uint32_t i = 0; i < count; ++i) {
            const auto present = read<std::uint32_t>(input);
            const auto updates_position_from_physics = read<std::uint32_t>(input);
            auto state = read<stop::State>(input);
            auto expected_state = read<stop::State>(input);
            auto body = read<physical::BodyState>(input);
            auto expected_body = read<physical::BodyState>(input);
            auto path_input = read<PathFixture>(input);
            auto expected_path = read<PathFixture>(input);
            const auto trace_count = read<std::uint32_t>(input);
            require(trace_count <= 6);
            std::vector<std::uint32_t> expected_trace(trace_count);
            for (auto& item : expected_trace) item = read<std::uint32_t>(input);
            const auto expected_transform = read<physical::TransformRequest>(input);

            Context context;
            context.body = body;
            context.path.position[0] = path_input.position[0];
            context.path.position[1] = path_input.position[1];
            context.path.position[2] = path_input.position[2];
            context.path.target[0] = path_input.target[0];
            context.path.target[1] = path_input.target[1];
            context.path.target[2] = path_input.target[2];
            context.path.count = path_input.count;
            context.path.owned = path_input.owned;
            context.path.capacity = static_cast<std::uint32_t>(context.path_segments.size());
            context.path.segments = context.path_segments.data();
            if (context.path.count) context.path_segments[0].edge = 0xFFFFFFFFu;
            context.updates_position_from_physics = updates_position_from_physics;
            stop::Services services{&context, Context::invoke, Context::resolve};
            stop::Result result{};
            const auto status = stop::execute(&state, &services, &result);
            require(status == stop::Status::complete, "source Stop differential returned failure");
            require(present == static_cast<std::uint32_t>(state.physical_identity != 0), "presence mismatch");
            require(std::memcmp(&state, &expected_state, sizeof(state)) == 0,
                    "logical GameObject state differs from original ARM");
            if (!same_body(context.body, expected_body))
                require(false, ("physical body differs case=" + std::to_string(i) + " actual=" +
                    body_words(context.body) + " expected=" + body_words(expected_body)).c_str());
            const PathFixture actual_path{context.path.count, context.path.owned,
                {context.path.position[0], context.path.position[1], context.path.position[2]},
                {context.path.target[0], context.path.target[1], context.path.target[2]}};
            if (std::memcmp(&actual_path, &expected_path, sizeof(actual_path)) != 0) {
                const auto* actual_bytes = reinterpret_cast<const std::uint8_t*>(&actual_path);
                const auto* expected_bytes = reinterpret_cast<const std::uint8_t*>(&expected_path);
                std::string detail = "DropPath adapter projection mismatch case=" + std::to_string(i) + " actual=";
                for (unsigned k = 0; k < sizeof(actual_path); ++k) detail += std::to_string(actual_bytes[k]) + ",";
                detail += " expected=";
                for (unsigned k = 0; k < sizeof(actual_path); ++k) detail += std::to_string(expected_bytes[k]) + ",";
                require(false, detail.c_str());
            }
            if (context.trace != expected_trace) {
                std::string detail = "ordered source service calls differ case=" + std::to_string(i) + " expected=";
                for (auto value : expected_trace) detail += std::to_string(value) + ",";
                detail += " actual=";
                for (auto value : context.trace) detail += std::to_string(value) + ",";
                require(false, detail.c_str());
            }
            require(!context.subjects.empty() && context.subjects[0] == state.path_identity,
                    "full-width path identity truncated");
            if (present) {
                require(context.subjects.size() == (updates_position_from_physics ? 5u : 2u), "physical service subject count");
                require(context.subjects[1] == state.object_identity, "full-width object identity truncated");
                for (std::size_t subject = 2; subject < context.subjects.size(); ++subject)
                    require(context.subjects[subject] == state.physical_identity,
                            "full-width physical identity truncated");
            } else {
                require(context.subjects.size() == 1, "unexpected physical service on absent body");
            }
            require(context.resolved_identity == (present && updates_position_from_physics ? state.physical_identity : 0),
                    "final physical owner was not resolved live");
            require(result.service_calls == (present ? (updates_position_from_physics ? 5u : 2u) : 1u), "service count mismatch");
            require(result.physical_setters == (present && updates_position_from_physics ? 3u : 0u), "setter count mismatch");
            require(result.phase == (present && updates_position_from_physics ? 7u : 2u), "phase mismatch");
            require(context.transform.pending == expected_transform.pending &&
                    same_float(context.transform.position[0], expected_transform.position[0]) &&
                    same_float(context.transform.position[1], expected_transform.position[1]) &&
                    same_float(context.transform.angle, expected_transform.angle), "SetXForm args mismatch");
            physical += present && updates_position_from_physics;
            emitted_transforms += std::count(context.trace.begin(), context.trace.end(), 5u);
            exact_float_words += 9;
        }
        require(input.peek() == std::char_traits<char>::eof(), "trailing Stop reference bytes");
        std::cout << "{\"validation\":\"PASS\",\"original_arm_cases\":" << count
                  << ",\"physical_stop_cases\":" << physical
                  << ",\"set_xform_boundaries\":" << emitted_transforms
                  << ",\"logical_float_words\":" << exact_float_words
                  << ",\"mismatches\":0}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
