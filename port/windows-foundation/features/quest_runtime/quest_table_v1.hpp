#pragma once
// Original v2Quest table loader (all acts, all difficulties). The rows are the
// authored source table; nothing here selects an act or a map.
#include "../../../game-data/quest_persistence_v51.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::quest_runtime {

using QuestTableV1 = dh2::data::QuestTablesPersistenceV51;

// Decodes the v2quests PyArray and its name table (the two original-cache
// files `v2quests_pyarray.bin` / `v2quests_pyarraynames.bin`). Malformed or
// partial input rejects atomically and returns no table.
bool decode_quest_table_v1(const std::vector<std::uint8_t>& array,
    const std::vector<std::uint8_t>& names,
    std::shared_ptr<const QuestTableV1>& out, std::string& error);

// Reads both files from disk and decodes them.
bool load_quest_table_v1(const std::string& pyarray_path, const std::string& names_path,
    std::shared_ptr<const QuestTableV1>& out, std::string& error);

// Number of rows whose authored act is `act` (for coverage reports and tests).
std::size_t quest_rows_in_act_v1(const QuestTableV1& table, std::int32_t act) noexcept;

} // namespace dh::foundation::quest_runtime
