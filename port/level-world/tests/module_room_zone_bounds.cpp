#include "../module_room_zone_bounds.hpp"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace dh2::module_room_zone_bounds;

namespace {
struct Fixture {
    float minimum[3];
    float maximum[3];
    float dimensions[3];
    float relative_minimum[3];
    float relative_maximum[3];
    std::uint8_t visual_enabled;
    unsigned calls;
    int fail;
    Request last{};
};

float from_bits(std::uint32_t bits) {
    float value{};
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

std::uint32_t bits_of(float value) {
    std::uint32_t bits{};
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

std::int32_t invoke(void* context, const Request* request) {
    auto& fixture = *static_cast<Fixture*>(context);
    fixture.last = *request;
    ++fixture.calls;
    return fixture.fail ? 1 : 0;
}

std::uint32_t argument(const char* text) {
    return static_cast<std::uint32_t>(std::strtoul(text, nullptr, 16));
}
} // namespace

int main(int argc, char** argv) {
    if (argc != 9) return 2;
    Fixture f{};
    for (unsigned i = 0; i != 3; ++i) {
        f.minimum[i] = from_bits(argument(argv[1 + i]));
        f.maximum[i] = from_bits(argument(argv[4 + i]));
        f.dimensions[i] = from_bits(0x4f123456u + i);
        f.relative_minimum[i] = from_bits(0x4f223456u + i);
        f.relative_maximum[i] = from_bits(0x4f323456u + i);
    }
    f.visual_enabled = static_cast<std::uint8_t>(std::strtoul(argv[7], nullptr, 0));
    f.fail = std::strtol(argv[8], nullptr, 0) != 0;

    RoomZone zone{};
    zone.identity = 0x2000u;
    for (unsigned i = 0; i != 3; ++i) {
        zone.dimensions[i] = &f.dimensions[i];
        zone.relative_minimum[i] = &f.relative_minimum[i];
        zone.relative_maximum[i] = &f.relative_maximum[i];
    }
    zone.optional_visual_enabled = &f.visual_enabled;
    const Box3 box{f.minimum, f.maximum};
    const Services services{&f, &invoke};
    Result result{};
    const auto status = initialize(&zone, &box, &services, &result);

    std::cout << "{\"status\":" << static_cast<int>(status)
              << ",\"calls\":" << f.calls << ",\"request_update\":"
              << static_cast<unsigned>(f.last.update_position) << ",\"center\":[";
    for (unsigned i = 0; i != 3; ++i) {
        if (i) std::cout << ',';
        std::cout << bits_of(f.last.position[i]);
    }
    std::cout << "],\"dimensions\":[";
    for (unsigned i = 0; i != 3; ++i) {
        if (i) std::cout << ',';
        std::cout << bits_of(f.dimensions[i]);
    }
    std::cout << "],\"relative_minimum\":[";
    for (unsigned i = 0; i != 3; ++i) {
        if (i) std::cout << ',';
        std::cout << bits_of(f.relative_minimum[i]);
    }
    std::cout << "],\"relative_maximum\":[";
    for (unsigned i = 0; i != 3; ++i) {
        if (i) std::cout << ',';
        std::cout << bits_of(f.relative_maximum[i]);
    }
    std::cout << "]}\n";
}
