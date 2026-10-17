#include "../floor_types.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace {
using dh2::floor_types::Error;
using dh2::floor_types::Property;
using dh2::floor_types::Span;

unsigned checks = 0;

void require(bool condition, const char* label) {
    ++checks;
    if (!condition) {
        std::cerr << "FAIL: " << label << '\n';
        std::exit(1);
    }
}

Span span(const char* text) {
    std::size_t size = 0;
    while (text[size] != '\0') ++size;
    return {text, size};
}

bool equals(Span value, const char* expected) {
    const Span other = span(expected);
    if (value.size != other.size) return false;
    for (std::size_t i = 0; i < value.size; ++i) {
        if (value.data[i] != other.data[i]) return false;
    }
    return true;
}

void parser_cases() {
    constexpr char text[] =
        "alpha=one=two\n"
        " floortypes = %22hole%22\n"
        "floortypes=water\n"
        "quoted=%22water%22\r\n";
    Property property{};
    require(dh2::floor_types::find_property(
                text, sizeof(text), span("alpha"), &property) == Error::ok,
            "parse first-equals property");
    require(property.found && equals(property.value, "one=two"),
            "first equals retained inside value");

    require(dh2::floor_types::find_property(
                text, sizeof(text), span("floortypes"), &property) == Error::ok,
            "parse whitespace around key");
    require(property.found && equals(property.value, "water"),
            "duplicate property last value wins");

    require(dh2::floor_types::find_property(
                text, sizeof(text), span("quoted"), &property) == Error::ok,
            "parse CRLF property");
    require(property.found && equals(property.value, "water"),
            "encoded quote markers are decoded");

    constexpr char empty_equals[] = "floortypes=\n";
    require(dh2::floor_types::find_property(
                empty_equals, sizeof(empty_equals), span("floortypes"), &property) == Error::ok,
            "parse explicit empty equals value");
    require(property.found && property.value.size == 0,
            "explicit empty equals value remains present");

    constexpr char empty_key[] = "floortypes\n";
    require(dh2::floor_types::find_property(
                empty_key, sizeof(empty_key), span("floortypes"), &property) == Error::ok,
            "parse key-only property");
    require(property.found && property.value.size == 0,
            "key-only property is explicit empty value");

    constexpr char missing[] = "other=value\n";
    require(dh2::floor_types::find_property(
                missing, sizeof(missing), span("floortypes"), &property) == Error::ok,
            "parse missing property");
    require(!property.found && property.value.data == nullptr,
            "absent property differs from empty property");

    const char no_terminator[] = {'f', '=', 'x'};
    require(dh2::floor_types::find_property(
                no_terminator, sizeof(no_terminator), span("f"), &property) == Error::unterminated,
            "unterminated property text rejected");
    require(dh2::floor_types::find_property(
                "x", dh2::floor_types::kMaxUserPropertiesBytes + 1U,
                span("f"), &property) == Error::too_large,
            "unbounded property extent rejected");
    require(dh2::floor_types::find_property(
                nullptr, 1U, span("f"), &property) == Error::argument,
            "null property text rejected");
}

void mask_cases() {
    using namespace dh2::floor_types;
    require(floor_type_mask(true, span("hole")) == kFloorPathHole,
            "present property selects only its type");
    require(floor_type_mask(false, {nullptr, 0}) == 0,
            "absent property leaves the type string empty");
    require(floor_type_mask(true, {"", 0}) == 0,
            "explicit empty property has no floor type");
    require(floor_type_mask(true, span("void_wall_hole_water")) ==
                (kFloorTypeVoid | kFloorTypeWall | kFloorPathHole | kFloorPathWater),
            "multiple tokens combine distinct flags");
    require(floor_type_mask(true, span("VOID Wall Hole WATER")) == 0,
            "floor token matching is case-sensitive");
    require(floor_type_mask(true, span("underholewaterproof")) ==
                (kFloorPathHole | kFloorPathWater),
            "floor tokens use substring matching");
    require(floor_type_mask(true, span("wood")) == 0,
            "wood has no native mask");
    require(floor_type_mask(true, span("door")) == 0,
            "door has no native mask");
    require(floor_type_mask(true, span("sand")) == 0,
            "unrecognized source floor value adds no mask");

    require(can_path_on(0U, 0U), "zero floor mask is always eligible");
    require(can_path_on(kFloorPathHole, kFloorPathHole | kFloorPathWater),
            "floor requirement is subset of actor mask");
    require(!can_path_on(kFloorPathHole | kFloorPathWater, kFloorPathHole),
            "missing actor path bit rejects floor");
    require(!can_path_on(kFloorTypeVoid, kFloorPathHole | kFloorPathWater),
            "void category flag is not a path capability");
    require(!can_path_on(kFloorTypeWall, kFloorPathHole | kFloorPathWater),
            "wall category flag is not a path capability");
}

void cache_cases(const char* path) {
    std::ifstream input(path, std::ios::binary);
    require(input.good(), "open supplied swamp cache BRES");
    const std::vector<char> data((std::istreambuf_iterator<char>(input)),
                                 std::istreambuf_iterator<char>());
    require(!data.empty(), "supplied swamp BRES is nonempty");

    constexpr std::uint32_t offsets[] = {0x94b18U, 0x94c6cU, 0x95298U, 0x9c6a8U};
    constexpr std::uint32_t expected[] = {
        dh2::floor_types::kFloorPathHole,
        dh2::floor_types::kFloorPathWater,
        0U,
        0U
    };
    constexpr const char* labels[] = {"cache hole tag", "cache water tag",
                                      "cache wood tag", "cache door tag"};
    for (std::size_t i = 0; i < sizeof(offsets) / sizeof(offsets[0]); ++i) {
        const std::size_t offset = offsets[i];
        require(offset < data.size(), labels[i]);
        const std::size_t available = data.size() - offset < dh2::floor_types::kMaxUserPropertiesBytes
            ? data.size() - offset : dh2::floor_types::kMaxUserPropertiesBytes;
        Property property{};
        require(find_property(data.data() + offset, available,
                              span("floortypes"), &property) == Error::ok,
                labels[i]);
        require(property.found && floor_type_mask(true, property.value) == expected[i],
                labels[i]);
    }
}

} // namespace

int main(int argc, char** argv) {
    parser_cases();
    mask_cases();
    if (argc > 1) cache_cases(argv[1]);
    std::cout << "floor-types checks passed: " << checks
              << ", supplied_cache=" << (argc > 1 ? "yes" : "no") << '\n';
    return 0;
}
