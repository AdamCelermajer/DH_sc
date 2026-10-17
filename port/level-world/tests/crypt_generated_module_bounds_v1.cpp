#include "../crypt_module_bounds_registry_v1.hpp"
#include "../../random-level/crypt_level_generator_v1.hpp"
#include "../../random-level/crypt_module_catalog_v1.hpp"
#include "../../random-level/native_rule_plan_v1.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {

void require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "Generated Crypt module bounds failed: %s\n", message);
    std::exit(1);
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(static_cast<bool>(input), "cannot read source fixture");
    return {std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>()};
}

std::uint32_t read_u32_le(const std::uint8_t* bytes) {
    return std::uint32_t(bytes[0]) | (std::uint32_t(bytes[1]) << 8) |
           (std::uint32_t(bytes[2]) << 16) | (std::uint32_t(bytes[3]) << 24);
}

float read_f32_le(const std::uint8_t* bytes) {
    const auto bits = read_u32_le(bytes);
    float value = 0.0f;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

bool near(float lhs, float rhs) {
    return std::isfinite(lhs) && std::isfinite(rhs) &&
           std::fabs(lhs - rhs) <= 0.001f;
}

}  // namespace

int main(int argc, char** argv) {
    using namespace dh2::random_level;
    using namespace dh2::crypt_module_bounds_registry_v1;
    require(argc == 4,
            "usage: crypt_generated_module_bounds_v1 <rule.xml> <cache-root> <crypt.bdae>");

    const auto rule_bytes = read_file(argv[1]);
    const auto bres_bytes = read_file(argv[3]);
    const std::string rule_xml(rule_bytes.begin(), rule_bytes.end());
    const auto parsed = parse_crypt_rule_v1(rule_xml);
    require(static_cast<bool>(parsed), "source Crypt rule does not parse");

    const auto module_root = std::filesystem::path(argv[2]) / "data" / "3d" /
                             "modules" / "crypt";
    const CryptModuleAssetPathsV1 paths{
        module_root / "mgx", module_root / "mgp", module_root / "mvp",
        module_root / "mvx", module_root / "mgx" / "mgxlist.txt"};
    const auto catalogue = build_crypt_module_catalogue_v1(*parsed.document, paths);
    require(catalogue.catalogue_issues.empty() &&
                catalogue.unresolved_candidate_count() == 0,
            "source Crypt catalogue is incomplete");

    constexpr std::uint32_t seed = 0x00C0FFEEU;
    const auto generated = generate_crypt_level_v1(*parsed.document,
                                                   catalogue, seed);
    require(generated.status == CryptLevelGeneratorStatusV1::success,
            "source Crypt generator failed");
    require(!generated.modules.empty() &&
                generated.layout.dwld_v1.size() ==
                    24 + generated.modules.size() * 128,
            "generated DWLD module count or record size differs");

    const auto& xml = generated.layout.source_level_xml;
    Owner owner{};
    Result result{};
    const auto status = build_from_assets(
        "GOTHICUS_CRYPT_01", "data/scene/generated_crypt.mlx",
        reinterpret_cast<const std::uint8_t*>(xml.data()), xml.size(),
        bres_bytes.data(), bres_bytes.size(), &owner, &result);
    require(status == Status::complete, "generated XML cannot resolve source roots");
    require(result.module_count == generated.modules.size() &&
                owner.modules.size() == generated.modules.size() &&
                result.module_count == read_u32_le(generated.layout.dwld_v1.data() + 8),
            "XML, bounds registry and DWLD module counts differ");
    require(owner.catalogue_path == "data/3d/modules/crypt/crypt.bdae",
            "generated modules no longer select the source Crypt catalogue");

    for (std::size_t i = 0; i < generated.modules.size(); ++i) {
        const auto& module = generated.modules[i];
        const auto& bounded = owner.modules[i];
        const auto expected_name = module.block_name + "_" + std::to_string(i);
        const auto expected_root = module.xrefobject + "-node";
        const auto* dwld_record = generated.layout.dwld_v1.data() + 24 + i * 128;
        require(bounded.module_index == i && bounded.module_name == expected_name &&
                    bounded.root_id == expected_root,
                "generated XML, BRES root, and DWLD room order differ");
        require(bounded.geometry_instances != 0 && bounded.draw_buffers != 0,
                "generated module has no bounded source geometry");
        for (unsigned axis = 0; axis != 3; ++axis)
            require(near(bounded.origin[axis], read_f32_le(dwld_record + 112 + 4 * axis)),
                    "generated XML module origin differs from its DWLD room");
    }

    std::printf("Generated Crypt modules=%zu bounds=%llu geometry=%llu seed=0x%08x\n",
                owner.modules.size(),
                static_cast<unsigned long long>(result.scene_nodes),
                static_cast<unsigned long long>(result.geometry_instances), seed);
    return 0;
}
