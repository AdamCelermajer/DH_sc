#include "../frustum.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

using namespace dh2::engine_camera::frustum;

namespace {
struct MathContext {
    std::uintptr_t token;
    std::uint32_t counts[5]{};
    char events[128]{};
    std::size_t event_count = 0;
    bool bad_context = false;
};

float as_float(Word bits) {
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

Word as_word(float value) {
    Word bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

MathContext& context(void* opaque, unsigned operation, char event) {
    auto* value = static_cast<MathContext*>(opaque);
    if (!value || value->token != static_cast<std::uintptr_t>(0x1122334455667788ull)) {
        if (value) value->bad_context = true;
        std::abort();
    }
    ++value->counts[operation];
    if (value->event_count >= sizeof(value->events)) std::abort();
    value->events[value->event_count++] = event;
    return *value;
}

Word add(void* opaque, Word left, Word right) {
    (void)context(opaque, 0, 'a');
    volatile float a = as_float(left), b = as_float(right);
    volatile float result = a + b;
    return as_word(result);
}

Word subtract(void* opaque, Word left, Word right) {
    (void)context(opaque, 1, 's');
    volatile float a = as_float(left), b = as_float(right);
    volatile float result = a - b;
    return as_word(result);
}

Word multiply(void* opaque, Word left, Word right) {
    (void)context(opaque, 2, 'm');
    volatile float a = as_float(left), b = as_float(right);
    volatile float result = a * b;
    return as_word(result);
}

Word divide(void* opaque, Word numerator, Word denominator) {
    (void)context(opaque, 3, 'd');
    volatile float a = as_float(numerator), b = as_float(denominator);
    volatile float result = a / b;
    return as_word(result);
}

Word square_root(void* opaque, Word value) {
    (void)context(opaque, 4, 'q');
    volatile float input = as_float(value);
    volatile float result = std::sqrt(input);
    return as_word(result);
}

MathServices services_for(MathContext& state) {
    return {&state, sizeof(state), &add, &subtract, &multiply, &divide, &square_root};
}

bool unchanged(const PlaneSet& planes, Word sentinel) {
    const auto* words = reinterpret_cast<const Word*>(&planes);
    for (std::size_t i = 0; i < sizeof(planes) / sizeof(Word); ++i)
        if (words[i] != sentinel) return false;
    return true;
}

int guards() {
    Matrix4Words matrix{};
    PlaneSet planes{};
    std::memset(&planes, 0x5a, sizeof(planes));
    constexpr Word sentinel = 0x5a5a5a5au;
    MathContext context_value{static_cast<std::uintptr_t>(0x1122334455667788ull)};
    auto services = services_for(context_value);

    auto status = set_from(nullptr, &planes, &services);
    if (status != Status::invalid_argument || !unchanged(planes, sentinel)) return 10;

    alignas(Matrix4Words) unsigned char matrix_storage[sizeof(Matrix4Words) + alignof(Matrix4Words)]{};
    auto* misaligned_matrix = reinterpret_cast<const Matrix4Words*>(matrix_storage + 1);
    status = set_from(misaligned_matrix, &planes, &services);
    if (status != Status::invalid_argument || !unchanged(planes, sentinel)) return 11;

    alignas(PlaneSet) unsigned char plane_storage[sizeof(PlaneSet) + alignof(PlaneSet)]{};
    auto* misaligned_planes = reinterpret_cast<PlaneSet*>(plane_storage + 1);
    status = set_from(&matrix, misaligned_planes, &services);
    if (status != Status::invalid_argument) return 12;

    alignas(MathServices) unsigned char service_storage[sizeof(MathServices) + alignof(MathServices)]{};
    auto* misaligned_services = reinterpret_cast<const MathServices*>(service_storage + 1);
    status = set_from(&matrix, &planes, misaligned_services);
    if (status != Status::invalid_argument || !unchanged(planes, sentinel)) return 13;

    status = set_from(&matrix, reinterpret_cast<PlaneSet*>(&matrix), &services);
    if (status != Status::invalid_argument || !unchanged(planes, sentinel)) return 14;

    auto context_alias = services;
    context_alias.context = &matrix;
    context_alias.context_extent = sizeof(matrix);
    status = set_from(&matrix, &planes, &context_alias);
    if (status != Status::invalid_argument || !unchanged(planes, sentinel)) return 15;

    auto overflow_context = services;
    overflow_context.context = reinterpret_cast<void*>(
        std::numeric_limits<std::uintptr_t>::max() - 15u);
    overflow_context.context_extent = 64;
    status = set_from(&matrix, &planes, &overflow_context);
    if (status != Status::invalid_argument || !unchanged(planes, sentinel)) return 16;

    auto missing_helper = services;
    missing_helper.square_root = nullptr;
    status = set_from(&matrix, &planes, &missing_helper);
    if (status != Status::invalid_argument || !unchanged(planes, sentinel)) return 17;

    // Also exercise the normal call with a nontruncated 64-bit service context.
    status = set_from(&matrix, &planes, &services);
    if (status != Status::complete || context_value.bad_context) return 18;
    if (context_value.counts[0] != 20 || context_value.counts[1] != 12 ||
        context_value.counts[2] != 42 || context_value.counts[3] != 6 ||
        context_value.counts[4] != 6) return 19;

    std::puts("{\"host_guard_cases\":8,\"math_calls\":[20,12,42,6,6]}");
    return 0;
}
} // namespace

int main(int argc, char** argv) {
    if (argc == 2 && std::strcmp(argv[1], "guards") == 0) return guards();
    if (argc != 18 || std::strcmp(argv[1], "matrix") != 0) return 2;

    Matrix4Words matrix{};
    for (std::size_t i = 0; i != 16; ++i)
        matrix.elements[i] = static_cast<Word>(std::strtoul(argv[i + 2], nullptr, 16));
    PlaneSet planes{};
    MathContext state{static_cast<std::uintptr_t>(0x1122334455667788ull)};
    const auto services = services_for(state);
    const auto status = set_from(&matrix, &planes, &services);
    std::printf("{\"status\":%d,\"planes\":[", static_cast<int>(status));
    for (std::size_t p = 0; p != 6; ++p) {
        if (p) std::putchar(',');
        std::putchar('[');
        for (std::size_t k = 0; k != 4; ++k) {
            if (k) std::putchar(',');
            std::printf("%u", planes.coefficients[p][k]);
        }
        std::putchar(']');
    }
    std::printf("],\"math_calls\":[%u,%u,%u,%u,%u],\"events\":\"%.*s\"}\n",
        state.counts[0], state.counts[1], state.counts[2], state.counts[3], state.counts[4],
        static_cast<int>(state.event_count), state.events);
    return status == Status::complete ? 0 : 1;
}
