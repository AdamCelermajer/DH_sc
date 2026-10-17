#include "character_faery_selection.hpp"

#include <cstddef>
#include <limits>

namespace dh2::character_faery_selection {
namespace {
struct Range { std::uintptr_t first, end; };

bool valid_range(const void* pointer, std::size_t size, std::size_t alignment,
                 Range& out) {
    const auto first = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || first % alignment ||
        first > std::numeric_limits<std::uintptr_t>::max() - size) return false;
    out = {first, first + size};
    return true;
}
bool overlaps(Range a, Range b) { return a.first < b.end && b.first < a.end; }

Status source_assert(Character* character, const Services& services, Result& result,
                     Assertion assertion, std::uint32_t line) {
    if (services.get_constant == nullptr || services.report_assertion == nullptr)
        return Status::service_unavailable;
    if (result.assertions == std::numeric_limits<std::uint32_t>::max())
        return Status::invalid_source_fact;
    const Request request{Operation::assertion, assertion, line,
                          "FaeryTypes", "COUNT"};
    ++result.assertions;
    result.last_operation = static_cast<std::uint32_t>(Operation::assertion);
    try {
        if (services.report_assertion(services.context, character, &request) != 0)
            return Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
    return Status::complete;
}

Status query_count(Character* character, const Services& services, Result& result,
                   std::int32_t& count) {
    if (!services.get_constant) return Status::service_unavailable;
    if (result.constant_queries == std::numeric_limits<std::uint32_t>::max())
        return Status::invalid_source_fact;
    const Request request{Operation::faery_types_count,
                          Assertion::faery_id_in_range, 0,
                          "FaeryTypes", "COUNT"};
    Response response{};
    ++result.constant_queries;
    result.last_operation = static_cast<std::uint32_t>(Operation::faery_types_count);
    try {
        if (services.get_constant(services.context, character, &request, &response) != 0)
            return Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
    count = response.word;
    return Status::complete;
}
} // namespace

Status select(Character* character, std::int32_t faery_id, const Globals* globals,
              const Services* services, Result* result) {
    Range ranges[4];
    if (!valid_range(character, sizeof(*character), alignof(Character), ranges[0]) ||
        !valid_range(globals, sizeof(*globals), alignof(Globals), ranges[1]) ||
        !valid_range(services, sizeof(*services), alignof(Services), ranges[2]) ||
        !valid_range(result, sizeof(*result), alignof(Result), ranges[3]) ||
        overlaps(ranges[0], ranges[1]) || overlaps(ranges[0], ranges[2]) ||
        overlaps(ranges[0], ranges[3]) || overlaps(ranges[1], ranges[2]) ||
        overlaps(ranges[1], ranges[3]) || overlaps(ranges[2], ranges[3]) ||
        character->identity == 0 || globals->tables == nullptr)
        return Status::invalid_argument;

    const Services bound = *services;
    const Tables* const tables = globals->tables;
    const std::uint32_t assert_level = globals->assert_level;
    *result = {};

    // Exact GetCharFaeryListId leaf: read the authored ID, use fallback row 0
    // for either negative IDs or IDs at/above the current table count.
    const std::int32_t authored_list_id = character->faery_list_id_106c;
    result->authored_list_id = authored_list_id;
    if (!tables->list_rows || tables->list_count == 0 ||
        tables->list_count > 1'000'000u)
        return Status::invalid_source_fact;
    const std::uint32_t list_id = authored_list_id >= 0 &&
            static_cast<std::uint32_t>(authored_list_id) < tables->list_count
        ? static_cast<std::uint32_t>(authored_list_id) : 0u;
    result->selected_list_id = list_id;

    // Source holds the list table base/row across the first COUNT query.
    const FaeryListRow* const list_row = tables->list_rows + list_id;
    if (!list_row) return Status::invalid_source_fact;

    if (faery_id < 0) {
        if (assert_level == 2) return Status::fatal_source_assertion;
        if (assert_level == 1) {
            auto status = source_assert(character, bound, *result,
                                        Assertion::faery_id_in_range, 0x3e);
            if (status != Status::complete) return status;
        }
    } else {
        std::int32_t count = 0;
        auto status = query_count(character, bound, *result, count);
        if (status != Status::complete) return status;
        if (count <= faery_id) {
            if (assert_level == 2) return Status::fatal_source_assertion;
            if (assert_level == 1) {
                status = source_assert(character, bound, *result,
                                       Assertion::faery_id_in_range, 0x3e);
                if (status != Status::complete) return status;
            }
        }
    }

    // Source reads row.ListSize before the second COUNT callback, then compares
    // against that callback's fresh result.
    const std::int32_t list_size = list_row->list_size;
    std::int32_t count = 0;
    auto status = query_count(character, bound, *result, count);
    if (status != Status::complete) return status;
    if (list_size != count) {
        if (assert_level == 2) return Status::fatal_source_assertion;
        if (assert_level == 1) {
            status = source_assert(character, bound, *result,
                                  Assertion::list_size_matches_types, 0x3f);
            if (status != Status::complete) return status;
        }
    }

    // This is exactly where source reads row.List and then the FaeryTable
    // global. Invalid/negative index and malformed backing are port guards;
    // the original asserted but still performed these raw accesses.
    if (faery_id < 0 || static_cast<std::uint32_t>(faery_id) >=
            static_cast<std::uint32_t>(list_size) || !list_row->members ||
        !tables->faery_rows || tables->faery_count == 0 ||
        tables->faery_count > 1'000'000u)
        return Status::invalid_source_fact;
    const std::int32_t row_index = list_row->members[faery_id];
    if (row_index < 0 || static_cast<std::uint32_t>(row_index) >= tables->faery_count)
        return Status::invalid_source_fact;
    const FaeryRow* selected = tables->faery_rows + static_cast<std::uint32_t>(row_index);
    const bool type_matches = selected->words[8] == static_cast<std::uint32_t>(faery_id);
    result->type_matches = type_matches ? 1u : 0u;
    if (!type_matches) {
        if (assert_level == 2) return Status::fatal_source_assertion;
        if (assert_level == 1) {
            status = source_assert(character, bound, *result,
                                   Assertion::table_type_matches_index, 0x40);
            if (status != Status::complete) return status;
            // The source's assert-level-1 path recomputes this pointer after
            // fprintf. Honor a provider-visible table/list mutation here.
            const std::int32_t refreshed_index = list_row->members[faery_id];
            if (refreshed_index < 0 ||
                static_cast<std::uint32_t>(refreshed_index) >= tables->faery_count)
                return Status::invalid_source_fact;
            selected = tables->faery_rows + static_cast<std::uint32_t>(refreshed_index);
        }
    }
    result->row = selected;
    return Status::complete;
}

} // namespace dh2::character_faery_selection
