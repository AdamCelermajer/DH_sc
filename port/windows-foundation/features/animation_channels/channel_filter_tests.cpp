// P17 DOORS: unit test for the compiled-channel filter (material/FX/emitter channels with no scene-node target).
#include "../../../engine-animation/channel_filter.hpp"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

namespace {
int failures = 0;
void check(bool condition, const char* what) {
    if (!condition) {
        ++failures;
        std::cout << "FAIL: " << what << '\n';
    }
}
}

int main() {
    using dh2::animation::skip_channel_without_node;

    // Capture the log so the once-only behaviour is observable.
    std::ostringstream captured;
    auto* previous = std::cerr.rdbuf(captured.rdbuf());

    // Material/FX-parameter channel (type 86) targeting no node: skipped.
    check(skip_channel_without_node(86, "Standard_8zxcv", false, false), "material channel without node is skipped");
    // Particle emitter channel (type 28) targeting no node: skipped.
    check(skip_channel_without_node(28, "GL_Fresh-emitter", false, false), "emitter channel without node is skipped");
    // The same channel targeting a real node is not skipped.
    check(!skip_channel_without_node(86, "Standard_8zxcv", true, false), "channel targeting a node is kept");
    // Transform channels that the compiler already handles are never skipped by this filter.
    check(!skip_channel_without_node(1, "node-a", false, false), "type 1 is not filtered");
    check(!skip_channel_without_node(5, "node-a", false, false), "type 5 is not filtered");
    check(!skip_channel_without_node(10, "node-a", false, false), "type 10 is not filtered");
    // Dynamic position and angle channels keep their existing path (an error when unsupported).
    check(!skip_channel_without_node(2, "node-a", false, true), "dynamic position channel is not filtered");
    check(!skip_channel_without_node(9, "node-a", false, true), "dynamic angle channel is not filtered");
    // A static position channel with no node is still filtered like any other non-node channel.
    check(skip_channel_without_node(2, "node-a", false, false), "static position channel without node is skipped");
    // A missing target keeps the existing rejection.
    check(!skip_channel_without_node(86, nullptr, false, false), "null target is not filtered");

    // Logs once per distinct (type, target): repeated calls log nothing new; a new target logs once.
    captured.str("");
    skip_channel_without_node(86, "Standard_8zxcv", false, false);
    skip_channel_without_node(86, "Standard_8zxcv", false, false);
    skip_channel_without_node(86, "Standard_8zxcv", false, true);
    const std::string afterRepeats = captured.str();
    check(afterRepeats.empty(), "repeat of the same channel prints nothing (already logged in this process)");
    skip_channel_without_node(28, "GL_Second-emitter", false, false);
    skip_channel_without_node(28, "GL_Second-emitter", false, false);
    const std::string afterEmitter = captured.str();
    std::size_t lines = 0;
    for (char c : afterEmitter) lines += c == '\n';
    check(lines == 1, "a new distinct channel logs exactly one line");
    check(afterEmitter.find("type=28") != std::string::npos && afterEmitter.find("GL_Second-emitter") != std::string::npos,
          "log line names the type and the target");

    std::cerr.rdbuf(previous);
    if (failures == 0) std::cout << "channel_filter_tests passed\n";
    return failures == 0 ? 0 : 1;
}
