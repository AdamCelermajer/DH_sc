#include "../item_random_drop_position_v1.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace {
struct RandomFixture {
    std::array<std::int32_t, 4> values{};
    std::array<std::int32_t, 4> bounds{};
    std::array<std::uint32_t, 4> streams{};
    std::size_t count{};
};

bool next(void* context, std::int32_t bound, std::uint32_t stream,
          std::int32_t& result, std::string& error) {
    auto& fixture = *static_cast<RandomFixture*>(context);
    if (fixture.count >= fixture.values.size()) {
        error = "fixture random stream exhausted";
        return false;
    }
    fixture.bounds[fixture.count] = bound;
    fixture.streams[fixture.count] = stream;
    result = fixture.values[fixture.count++];
    error.clear();
    return true;
}

void check(bool value, const char* message, unsigned& checks) {
    ++checks;
    if (!value) {
        std::fprintf(stderr, "FAIL item_random_drop_position_v1: %s\n", message);
        std::exit(1);
    }
}
}

int main() {
    using namespace dh2::item_random_drop_position_v1;
    unsigned checks = 0;
    std::string error;

    RandomFixture directed{{0, 0}};
    const Point center{10.0f, 20.0f, 30.0f};
    const Point killer{10.0f, 20.0f, 31.0f};
    Point output{};
    check(generate(center, &killer, {&directed, next}, &output, error),
          "source-directed scatter succeeds", checks);
    check(output.x == 10.0f && output.y == 20.0f && output.z == 180.0f,
          "vertical direction uses source Vec3f_K cross-product basis", checks);
    check(directed.count == 2 && directed.bounds[0] == 200 &&
              directed.bounds[1] == 300 && directed.streams[0] == 0 &&
              directed.streams[1] == 0,
          "killer scatter draws distance then lateral from ordinary process stream", checks);

    RandomFixture null_killer{{499, 0}};
    check(generate(center, nullptr, {&null_killer, next}, &output, error),
          "source null-killer scatter succeeds", checks);
    check(output.x == 259.0f && output.y == -230.0f && output.z == 30.0f,
          "null-killer scatter draws independent XY offsets and preserves Z", checks);
    check(null_killer.count == 2 && null_killer.bounds[0] == 500 &&
              null_killer.bounds[1] == 500,
          "null-killer source path consumes two bound-500 draws", checks);

    RandomFixture coincident{{0, 0}};
    const Point same_as_center = center;
    check(generate(center, &same_as_center, {&coincident, next}, &output, error),
          "coincident killer still consumes both source draws", checks);
    check(std::isnan(output.x) && std::isnan(output.y) && std::isnan(output.z) &&
              coincident.count == 2,
          "source Point3D normalize division-by-zero NaNs are retained", checks);

    std::puts("PASS item_random_drop_position_v1 source scatter order and branches");
}
