#include "lua_script_level_queries.hpp"

#include <exception>

namespace dh2::lua_script_level_queries {
namespace {
constexpr std::uint32_t kPlayerLevel = 0x330;
constexpr std::uint32_t kCurrentLevelDifficulty = 0x118;
constexpr std::uint32_t kCurrentLevelOid = 0x3c;
constexpr std::uint32_t kLevelRowBytes = 72;

bool span(const void* value, std::size_t bytes, std::size_t alignment) noexcept {
    const auto address = reinterpret_cast<std::uintptr_t>(value);
    return value && address % alignment == 0 && address <= UINTPTR_MAX - bytes;
}

bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) noexcept {
    const auto a = reinterpret_cast<std::uintptr_t>(left);
    const auto b = reinterpret_cast<std::uintptr_t>(right);
    return a <= b ? b - a < left_size : a - b < right_size;
}

bool valid_controls(const Services* services, const Result* result) noexcept {
    return span(services, sizeof(*services), alignof(Services)) &&
           span(result, sizeof(*result), alignof(Result)) &&
           !overlaps(services, sizeof(*services), result, sizeof(*result));
}

struct Run {
    Services services;
    Result result{};

    template<class Callback, class... Args>
    Status invoke(Callback callback, Args... args) {
        if (!callback) return Status::service_unavailable;
        ++result.service_calls;
        try {
            return callback(services.context, args...) == 0 ?
                Status::complete : Status::service_failed;
        } catch (...) {
            return Status::service_exception;
        }
    }

    Status push(std::int32_t value) {
        if (result.values_pushed >= 2) return Status::invalid_argument;
        const auto status = invoke(services.push_integer, value);
        if (status != Status::complete) return status;
        result.values[result.values_pushed] = value;
        ++result.values_pushed;
        return Status::complete;
    }
};

Status commit(Status status, Run& run, Result* output) noexcept {
    *output = run.result;
    return status;
}
}  // namespace

Status get_host_player_level(const Services* services, Result* result) {
    if (!valid_controls(services, result)) return Status::invalid_argument;
    Run run{*services};
    std::uintptr_t application = 0, manager = 0, player = 0;
    auto status = run.invoke(run.services.get_application, &application);
    if (status != Status::complete) return commit(status, run, result);
    if (!application) return commit(Status::service_failed, run, result);

    status = run.invoke(run.services.get_application_player_manager, application, &manager);
    if (status != Status::complete) return commit(status, run, result);
    if (!manager) return commit(Status::service_failed, run, result);

    status = run.invoke(run.services.get_hosting_player, manager, &player);
    if (status != Status::complete) return commit(status, run, result);
    // The original caller dereferences the returned Player at +0x330 without
    // a null check. A port provider reports a missing Player as an explicit
    // boundary error instead of dereferencing a null native address.
    if (!player) return commit(Status::service_failed, run, result);

    std::int32_t level = 0;
    status = run.invoke(run.services.read_player_word, player, kPlayerLevel, &level);
    if (status != Status::complete) return commit(status, run, result);
    run.result.output_path = OutputPath::host_player_level;
    status = run.push(level);
    return commit(status, run, result);
}

Status get_host_player_difficulty(const Services* services, Result* result) {
    if (!valid_controls(services, result)) return Status::invalid_argument;
    Run run{*services};
    std::uintptr_t level = 0;
    auto status = run.invoke(run.services.get_current_level, &level);
    if (status != Status::complete) return commit(status, run, result);

    std::int32_t difficulty = 0;
    if (level) {
        status = run.invoke(run.services.read_level_word, level,
                            kCurrentLevelDifficulty, &difficulty);
        if (status != Status::complete) return commit(status, run, result);
    }
    run.result.output_path = OutputPath::current_level_difficulty;
    status = run.push(difficulty);
    return commit(status, run, result);
}

Status get_current_level_range(const Arguments* arguments,
                               const Services* services, Result* result) {
    if (!span(arguments, sizeof(*arguments), alignof(Arguments)) ||
        !valid_controls(services, result) ||
        overlaps(arguments, sizeof(*arguments), result, sizeof(*result)))
        return Status::invalid_argument;

    Run run{*services};
    std::uintptr_t level = 0;
    auto status = run.invoke(run.services.get_current_level, &level);
    if (status != Status::complete) return commit(status, run, result);

    // The source has no null-Level branch here. Keep the invalid pointer at a
    // checked port boundary instead of inventing a range or dereferencing 0.
    if (!level) return commit(Status::service_failed, run, result);

    std::int32_t oid = 0;
    status = run.invoke(run.services.read_level_word, level, kCurrentLevelOid, &oid);
    if (status != Status::complete) return commit(status, run, result);
    run.result.level_oid = oid;

    // OID -1 is the source sentinel branch: two integer pushes precede and
    // bypass all argument and LevelTable access.
    if (oid == -1) {
        run.result.output_path = OutputPath::sentinel_level;
        status = run.push(-1);
        if (status == Status::complete) status = run.push(-1);
        return commit(status, run, result);
    }

    // Arguments are deliberately read after GetCurrentLevel and its +0x3c
    // load. Source callbacks may change the live argument projection before
    // this point, so do not snapshot it on entry.
    const std::uint32_t argument_count = arguments->count;
    const Argument* front = argument_count ? arguments->values : nullptr;
    std::int32_t difficulty = 0;
    if (argument_count) {
        if (!span(front, sizeof(*front), alignof(Argument)))
            return commit(Status::invalid_argument, run, result);
        if (overlaps(front, sizeof(*front), result, sizeof(*result)))
            return commit(Status::invalid_argument, run, result);
        const Argument captured_front = *front;
        if (captured_front.type == 3U) {
            if (!captured_front.identity)
                return commit(Status::invalid_argument, run, result);
            float number = 0.0f;
            status = run.invoke(run.services.value_get_number,
                                captured_front.identity, &number);
            if (status != Status::complete) return commit(status, run, result);
            status = run.invoke(run.services.float_to_signed_int, number, &difficulty);
            if (status != Status::complete) return commit(status, run, result);
        }
    }
    run.result.selected_difficulty = difficulty;
    if (difficulty < 0 || difficulty > 2)
        return commit(Status::complete, run, result);

    // The original MUL is ARM32 word arithmetic; signed OIDs outside the
    // authored range retain the wrapped byte offset for the typed table read.
    const auto row_offset = static_cast<std::uint32_t>(oid) * kLevelRowBytes;
    run.result.row_byte_offset = row_offset;
    constexpr std::uint32_t minimum_offsets[] = {0x3c, 0x40, 0x44};
    constexpr std::uint32_t maximum_offsets[] = {0x30, 0x34, 0x38};

    // Capture the LevelTable object once. The original reloads its backing
    // row pointer from this object after the first push, not the object itself.
    std::uintptr_t table = 0;
    status = run.invoke(run.services.get_level_table, &table);
    if (status != Status::complete) return commit(status, run, result);
    if (!table) return commit(Status::service_failed, run, result);

    std::int32_t minimum = 0, maximum = 0;
    status = run.invoke(run.services.read_level_table_word, table, row_offset,
                        minimum_offsets[static_cast<std::size_t>(difficulty)], &minimum);
    if (status != Status::complete) return commit(status, run, result);
    run.result.output_path = OutputPath::selected_level_range;
    status = run.push(minimum);
    if (status != Status::complete) return commit(status, run, result);

    // Source reloads the backing row pointer from the captured table object
    // after the first ReturnValues push. A push provider may replace that
    // backing store while preserving the table object identity.
    status = run.invoke(run.services.read_level_table_word, table, row_offset,
                        maximum_offsets[static_cast<std::size_t>(difficulty)], &maximum);
    if (status != Status::complete) return commit(status, run, result);
    status = run.push(maximum);
    return commit(status, run, result);
}

}  // namespace dh2::lua_script_level_queries
