#include "../camera_design_zoom_v1.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
unsigned checks = 0;

void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
    ++checks;
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("Cannot read DesignSettings cache fixture");
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

std::vector<std::string> read_name_array(const std::vector<std::uint8_t>& bytes,
                                         std::size_t* offset) {
    if (!offset || *offset > bytes.size() || bytes.size() - *offset < 4)
        throw std::runtime_error("Truncated source names fixture");
    const auto read_word = [&](std::size_t at) {
        return std::uint32_t(bytes[at]) | (std::uint32_t(bytes[at + 1]) << 8) |
               (std::uint32_t(bytes[at + 2]) << 16) | (std::uint32_t(bytes[at + 3]) << 24);
    };
    const auto count = read_word(*offset);
    *offset += 4;
    if (count > 4096) throw std::runtime_error("Source names count outside fixture bounds");
    std::vector<std::string> names;
    names.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        if (*offset > bytes.size() || bytes.size() - *offset < 4)
            throw std::runtime_error("Truncated source name length");
        const auto size = read_word(*offset);
        *offset += 4;
        if (size > bytes.size() - *offset)
            throw std::runtime_error("Truncated source name value");
        names.emplace_back(reinterpret_cast<const char*>(bytes.data() + *offset), size);
        *offset += size;
    }
    return names;
}

void set_u32(std::vector<std::uint8_t>& bytes, std::size_t offset, std::uint32_t value) {
    for (unsigned i = 0; i < 4; ++i)
        bytes[offset + i] = static_cast<std::uint8_t>(value >> (i * 8));
}

void set_float(std::vector<std::uint8_t>& bytes, std::size_t word, float value) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof(bits));
    set_u32(bytes, 4 + word * 4, bits);
}
} // namespace

int main(int argc, char** argv) {
    try {
        if (argc != 2) throw std::runtime_error("usage: camera_design_zoom_v1_host assets-root");
        const auto cache = read_file(std::string(argv[1]) +
            "/original-cache/data/pydata/design_pyarray.bin");
        const auto row_names = read_file(std::string(argv[1]) +
            "/original-cache/data/pydata/design_pyarraynames.bin");
        const auto schema = read_file(std::string(argv[1]) +
            "/original-cache/data/pydata/design_pystructnames.bin");
        std::size_t names_offset = 0, schema_offset = 0;
        check(read_name_array(row_names, &names_offset) == std::vector<std::string>{"Default"},
              "cache exposes exactly the global Default DesignSettings row");
        const auto design_fields = read_name_array(schema, &schema_offset);
        check(design_fields.size() == 43 &&
              design_fields[17] == "MiniMapZoomMaxLimit" &&
              design_fields[18] == "MiniMapZoomMinLimit" &&
              design_fields[41] == "ZoomMaxLimit" &&
              design_fields[42] == "ZoomMinLimit",
              "cache schema confirms field names and serialized indexes");
        dh2::camera_design_zoom_v1::Bounds bounds{99u, 8.0f, 9.0f, 10.0f, 11.0f};
        using dh2::camera_design_zoom_v1::Status;
        check(dh2::camera_design_zoom_v1::decode_global_bounds(
                  cache.data(), cache.size(), &bounds) == Status::complete,
              "original DesignSettings cache row decodes");
        check(bounds.row == 0u && std::fabs(bounds.normal_min - 0.0f) < 1e-7f &&
              std::fabs(bounds.normal_max - 0.35f) < 1e-6f &&
              std::fabs(bounds.alternate_min - (-1.5f)) < 1e-6f &&
              std::fabs(bounds.alternate_max - 0.5f) < 1e-6f,
              "serialized words map to HandleZoom runtime offsets and source values");
        std::uint32_t source_rate_bits=4u;
        float source_rate=0.0f;
        std::memcpy(&source_rate,&source_rate_bits,sizeof(source_rate));
        check(std::fabs(bounds.auto_zoom_step-0.1f)<1e-7f &&
              bounds.auto_zoom_rate==source_rate &&
              std::fabs(bounds.coop_bottom-1.0f)<1e-6f &&
              std::fabs(bounds.coop_sides-0.9f)<1e-6f &&
              std::fabs(bounds.coop_top-0.4f)<1e-6f,
              "Level::UpdateCameraZoom cache words map to +12/+16/+24/+28/+32 source fields");
        const std::uint32_t identity[16]{
            0x3f800000u,0,0,0,0,0x3f800000u,0,0,
            0,0,0x3f800000u,0,0,0,0,0x3f800000u};
        dh2::camera_design_zoom_v1::ScreenPoint point{};
        check(dh2::camera_design_zoom_v1::project_screen_point(identity,0.25f,-0.5f,0.0f,&point)&&
              std::fabs(point.x-0.25f)<1e-6f&&std::fabs(point.y+0.5f)<1e-6f,
              "CameraBase::GetScreenCoord returns homogeneous clip x/w,y/w");
        dh2::camera_design_zoom_v1::Bounds auto_zoom{};
        auto_zoom.auto_zoom_step=0.1f;auto_zoom.auto_zoom_rate=2.0f;
        auto_zoom.coop_bottom=1.0f;auto_zoom.coop_sides=0.9f;auto_zoom.coop_top=0.4f;
        float target_zoom=1.0f;
        const dh2::camera_design_zoom_v1::ScreenPoint centered{0.0f,0.0f};
        check(dh2::camera_design_zoom_v1::update_target_zoom(auto_zoom,&centered,1,&target_zoom)&&
              std::fabs(target_zoom-1.6f)<1e-6f,
              "screen margin above AutoZoomStep moves CameraLevel target +140 outward");
        const dh2::camera_design_zoom_v1::ScreenPoint high{0.0f,0.5f};
        check(dh2::camera_design_zoom_v1::update_target_zoom(auto_zoom,&high,1,&target_zoom)&&
              std::fabs(target_zoom-1.2f)<1e-6f,
              "top-edge margin below AutoZoomStep moves CameraLevel target +140 inward");

        const auto preserved = bounds;
        check(dh2::camera_design_zoom_v1::decode_global_bounds(nullptr, 0, &bounds) == Status::invalid_input &&
              bounds.row == preserved.row && bounds.normal_min == preserved.normal_min,
              "invalid cache input preserves the caller's bounds");
        std::vector<std::uint8_t> malformed(4 + 43 * 4, 0);
        set_u32(malformed, 0, 2);
        check(dh2::camera_design_zoom_v1::decode_global_bounds(malformed.data(), malformed.size(), &bounds) ==
              Status::unexpected_row_count && bounds.row == preserved.row,
              "unknown multi-row table fails closed without guessing a row");
        set_u32(malformed, 0, 1);
        set_float(malformed, 42, 2.0f);
        set_float(malformed, 41, 1.0f);
        check(dh2::camera_design_zoom_v1::decode_global_bounds(malformed.data(), malformed.size(), &bounds) ==
              Status::invalid_bounds && bounds.row == preserved.row,
              "inverted source bounds fail atomically");
        set_float(malformed, 42, 0.0f);
        set_float(malformed, 41, std::numeric_limits<float>::quiet_NaN());
        check(dh2::camera_design_zoom_v1::decode_global_bounds(malformed.data(), malformed.size(), &bounds) ==
              Status::invalid_bounds && bounds.row == preserved.row,
              "nonfinite cache bounds fail atomically");
        std::printf("PASS: source DesignSettings camera zoom bounds (%u checks)\n", checks);
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "FAIL: %s\n", error.what());
        return 1;
    }
}
