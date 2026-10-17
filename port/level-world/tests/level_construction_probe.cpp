#include "../level_construction_fields.hpp"

#include <cstdint>
#include <cstring>
#include <new>
#include <string_view>
#include <vector>

struct ProbeRow {
    const char* level_file;
    std::int32_t hub;
    std::uint8_t is_random;
    std::uint8_t reserved[3];
};

struct ProbeOutput {
    std::int32_t index_3c;
    std::int32_t hub_40;
    std::uint8_t is_random_e8;
    std::uint8_t reserved[3];
    std::int32_t difficulty_118;
    std::uint32_t status;
    std::uint32_t rows_examined;
};

static_assert(sizeof(ProbeRow) == 16, "probe row ABI");
static_assert(sizeof(ProbeOutput) == 24, "probe output ABI");

#if defined(_WIN32)
#define DH2_PROBE_EXPORT __declspec(dllexport)
#else
#define DH2_PROBE_EXPORT __attribute__((visibility("default")))
#endif

extern "C" DH2_PROBE_EXPORT std::uint32_t dh2_level_construction_probe(
    const ProbeRow* rows, std::uint32_t count, const char* incoming_file,
    std::int32_t difficulty, ProbeOutput* output) noexcept {
    if (!output || !incoming_file || (count != 0 && !rows) || count > 4096) return 2;
    try {
        dh2::data::LevelTables tables;
        tables.levels.reserve(count);
        for (std::uint32_t i = 0; i < count; ++i) {
            if (!rows[i].level_file || rows[i].is_random > 1) return 2;
            dh2::data::LevelDeclaration row{};
            row.level_file = rows[i].level_file;
            row.hub = rows[i].hub;
            row.is_random = rows[i].is_random != 0;
            tables.levels.push_back(std::move(row));
        }
        dh2::level_construction_fields::State state{};
        dh2::level_construction_fields::Result result{};
        const auto status = dh2::level_construction_fields::initialize(
            &tables, std::string_view(incoming_file, std::strlen(incoming_file)),
            difficulty, &state, &result);
        output->index_3c = state.level_list_index_3c;
        output->hub_40 = state.hub_40;
        output->is_random_e8 = state.is_random_e8;
        output->reserved[0] = output->reserved[1] = output->reserved[2] = 0;
        output->difficulty_118 = state.difficulty_118;
        output->status = static_cast<std::uint32_t>(status);
        output->rows_examined = result.rows_examined;
        return output->status;
    } catch (...) {
        return 4;
    }
}
