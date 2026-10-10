#include "camera.hpp"

#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

using namespace dh::foundation;

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
bool near(double a, double b, double tolerance = 0.0001) {
    return std::abs(a - b) <= tolerance;
}
void vectorNear(CameraVec3 actual, CameraVec3 expected, const char* message) {
    require(near(actual.x, expected.x) && near(actual.y, expected.y) &&
            near(actual.z, expected.z), message);
}
template<class Function> void rejects(Function function, const char* message) {
    try { function(); } catch (const std::invalid_argument&) { return; }
    throw std::runtime_error(message);
}
CameraPose shot(float x, float fov = 60) {
    return {{x, 0, 10}, {x, 0, 0}, {0, 1, 0}, fov};
}
CameraVec3 transform(const std::array<float, 16>& m, CameraVec3 p) {
    return {m[0]*p.x+m[4]*p.y+m[8]*p.z+m[12],
            m[1]*p.x+m[5]*p.y+m[9]*p.z+m[13],
            m[2]*p.x+m[6]*p.y+m[10]*p.z+m[14]};
}
void timelineTests() {
    // Deliberately unordered input; camera choreography must be chronological.
    CameraTimeline timeline({{8, shot(80, 100), false}, {2, shot(0, 40), false},
                             {6, shot(40, 80), true}, {4, shot(20, 60), false}});
    require(near(timeline.duration(), 8), "duration follows last chronological shot");
    vectorNear(timeline.sample(-10).position, shot(0).position, "hold before timeline");
    vectorNear(timeline.sample(1).position, shot(0).position, "hold before first key");
    vectorNear(timeline.sample(3).position, {10, 0, 10}, "linear position midpoint");
    vectorNear(timeline.sample(3).target, {10, 0, 0}, "linear target midpoint");
    require(near(timeline.sample(3).verticalFovDegrees, 50), "linear FOV midpoint");
    vectorNear(timeline.sample(5.999).position, shot(20).position, "cut holds outgoing shot");
    vectorNear(timeline.sample(6).position, shot(40).position, "cut occurs at exact key time");
    vectorNear(timeline.sample(7).position, {60, 0, 10}, "interpolation resumes after cut");
    vectorNear(timeline.sample(100).position, shot(80).position, "hold after timeline");

    timeline.update(3);
    require(near(timeline.elapsed(), 0), "stopped timeline does not advance");
    timeline.play(); timeline.update(3);
    require(timeline.playing() && near(timeline.elapsed(), 3), "play advances timeline");
    timeline.update(std::numeric_limits<double>::max());
    require(timeline.finished() && !timeline.playing() && near(timeline.elapsed(), 8),
            "large delta stops exactly at final shot");
    timeline.play();
    require(!timeline.playing(), "finished timeline cannot replay without reset");
    timeline.reset();
    require(!timeline.finished() && !timeline.playing() && near(timeline.elapsed(), 0),
            "reset returns timeline to stopped beginning");
    timeline.play(); timeline.update(0);
    require(timeline.playing() && near(timeline.elapsed(), 0), "zero delta preserves playback");
    rejects([&] { timeline.update(-1); }, "negative delta rejected");
    rejects([&] { timeline.update(std::numeric_limits<double>::infinity()); }, "infinite delta rejected");
    rejects([&] { timeline.sample(std::numeric_limits<double>::quiet_NaN()); }, "NaN sample rejected");
    require(near(timeline.elapsed(), 0) && timeline.playing(), "rejected updates preserve playback");
    rejects([&] { timeline.setKeyframes({{1, shot(0)}, {1, shot(1)}}); }, "duplicate times rejected");
    require(near(timeline.duration(), 8), "invalid replacement preserves existing timeline");
    rejects([] { CameraTimeline t({{-1, shot(0)}}); }, "negative key time rejected");
    rejects([] { CameraTimeline t({{std::numeric_limits<double>::quiet_NaN(), shot(0)}}); },
            "NaN key time rejected");

    CameraTimeline empty;
    empty.play(); empty.update(100);
    require(empty.finished() && !empty.playing() && near(empty.duration(), 0), "empty playback is safe");
    CameraTimeline single({{0, shot(4)}});
    require(single.finished(), "instantaneous single key is complete");
    vectorNear(single.sample().position, shot(4).position, "single key remains visible");

    CameraPose tilted = shot(10); tilted.up = {1, 1, 0};
    CameraTimeline tilt({{0, shot(0)}, {2, tilted}});
    const auto up = tilt.sample(1).up;
    require(near(up.x*up.x+up.y*up.y+up.z*up.z, 1), "interpolated up is normalized");
}
void freeCameraTests() {
    FreeCamera camera(shot(0));
    camera.move(2, 3, 4);
    vectorNear(camera.pose().position, {2, 3, 6}, "movement follows local camera axes");
    vectorNear(camera.pose().target, {2, 3, -4}, "movement keeps viewing direction and distance");
    camera.rotate(90, 0);
    vectorNear(camera.pose().position, {2, 3, 6}, "rotation keeps camera position");
    vectorNear(camera.pose().target, {12, 3, 6}, "positive yaw looks right");
    camera.move(0, 0, 2);
    vectorNear(camera.pose().position, {4, 3, 6}, "movement uses rotated heading");
    camera.rotate(0, 90);
    const auto pose = camera.pose();
    require(pose.target.y > pose.position.y, "positive pitch looks up");
    const auto matrix = cameraViewMatrix(pose);
    for (float value : matrix) require(std::isfinite(value), "pole clamping leaves finite view");
    rejects([&] { camera.move(0, 0, std::numeric_limits<float>::infinity()); }, "infinite movement rejected");
    rejects([&] { camera.rotate(std::numeric_limits<float>::quiet_NaN(), 0); }, "NaN rotation rejected");

    CameraPose degenerate = shot(0); degenerate.target = degenerate.position; degenerate.up = {0, 0, 0};
    camera.setPose(degenerate);
    camera.move(0, 0, 1);
    vectorNear(camera.pose().position, {0, 0, 9}, "degenerate pose recovers useful forward direction");
}
void matrixTests() {
    const CameraPose pose = shot(5);
    const auto view = cameraViewMatrix(pose);
    vectorNear(transform(view, pose.position), {0, 0, 0}, "view maps camera eye to origin");
    vectorNear(transform(view, pose.target), {0, 0, -10}, "view maps target down negative Z");
    const auto projection = cameraProjectionMatrix(90, 2, 1, 101);
    const auto depth = [&](float z) { return (projection[10]*z+projection[14])/(-z); };
    require(near(depth(-1), -1) && near(depth(-101), 1), "perspective maps near and far clip planes");
    require(near(projection[0]*2, projection[5]), "perspective honors aspect ratio");
    for (float fov : {0.0f, 180.0f, std::numeric_limits<float>::quiet_NaN(),
                      std::numeric_limits<float>::infinity()}) {
        rejects([&] { cameraProjectionMatrix(fov, 1, 1, 10); }, "invalid perspective FOV rejected");
        auto invalid = pose; invalid.verticalFovDegrees = fov;
        rejects([&] { FreeCamera c(invalid); }, "invalid free-camera FOV rejected");
        rejects([&] { CameraTimeline t({{0, invalid}}); }, "invalid timeline FOV rejected");
    }
    rejects([] { cameraProjectionMatrix(60, 0, 1, 10); }, "zero aspect rejected");
    rejects([] { cameraProjectionMatrix(60, 1, 0, 10); }, "zero near clip rejected");
    rejects([] { cameraProjectionMatrix(60, 1, 10, 1); }, "inverted clip planes rejected");
    auto nonfinite = pose; nonfinite.target.x = std::numeric_limits<float>::infinity();
    rejects([&] { cameraViewMatrix(nonfinite); }, "nonfinite target rejected");
    nonfinite = pose; nonfinite.up.z = std::numeric_limits<float>::quiet_NaN();
    rejects([&] { CameraTimeline t({{0, nonfinite}}); }, "nonfinite up rejected");
    auto parallel = pose; parallel.up = {0, 0, -1};
    const auto safe = cameraViewMatrix(parallel);
    vectorNear(transform(safe, parallel.target), {0, 0, -10}, "parallel up recovers stable view basis");
}
}

int main() {
    try {
        timelineTests(); freeCameraTests(); matrixTests();
        std::cout << "camera tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "camera tests failed: " << error.what() << '\n';
        return 1;
    }
}
