#include "../plane_intersection.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

using namespace dh2::engine_camera::plane_intersection;

namespace {
struct Context {
    std::uintptr_t token;
    char events[4096]{};
    std::size_t event_count = 0;
    std::size_t calls = 0;
    struct Trace { char operation; Word inputs[4]; std::size_t count; } trace[4096]{};
    std::size_t trace_count = 0;
    MathServices* mutable_services = nullptr;
    std::size_t replacement_calls = 0;
};

void mark(void* opaque, char value, Word a = 0, Word b = 0,
          Word c = 0, Word d = 0, std::size_t count = 0) {
    auto* context = static_cast<Context*>(opaque);
    if (!context || context->token != static_cast<std::uintptr_t>(0x12345678abcdef01ull))
        std::abort();
    if (context->event_count >= sizeof(context->events)) std::abort();
    ++context->calls;
    context->events[context->event_count++] = value;
    if (context->trace_count >= sizeof(context->trace) / sizeof(context->trace[0])) std::abort();
    auto& entry = context->trace[context->trace_count++];
    entry.operation = value;
    entry.inputs[0] = a; entry.inputs[1] = b;
    entry.inputs[2] = c; entry.inputs[3] = d;
    entry.count = count;
}

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
double as_double(Float64Words value) {
    const std::uint64_t bits = (static_cast<std::uint64_t>(value.high) << 32) | value.low;
    double result;
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}
Float64Words as_words(double value) {
    std::uint64_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return {static_cast<Word>(bits), static_cast<Word>(bits >> 32)};
}

Word replacement_add(void* opaque, Word a, Word b);
Word fmul(void* c, Word a, Word b) {
    auto* context = static_cast<Context*>(c);
    mark(c, 'm', a, b, 0, 0, 2);
    if (context->mutable_services) context->mutable_services->add = &replacement_add;
    volatile float x = as_float(a), y = as_float(b); volatile float z = x * y; return as_word(z);
}
Word fadd(void* c, Word a, Word b) {
    mark(c, 'a', a, b, 0, 0, 2); volatile float x = as_float(a), y = as_float(b); volatile float z = x + y; return as_word(z);
}
Word replacement_add(void* opaque, Word a, Word b) {
    auto* context = static_cast<Context*>(opaque);
    ++context->replacement_calls;
    return a + b;
}
Word fsub(void* c, Word a, Word b) {
    mark(c, 's', a, b, 0, 0, 2); volatile float x = as_float(a), y = as_float(b); volatile float z = x - y; return as_word(z);
}
Word fdiv(void* c, Word a, Word b) {
    mark(c, 'd', a, b, 0, 0, 2); volatile float x = as_float(a), y = as_float(b); volatile float z = x / y; return as_word(z);
}
Word fequal(void* c, Word a, Word b) {
    mark(c, 'e', a, b, 0, 0, 2); return static_cast<Word>(as_float(a) == as_float(b));
}
void f2d(void* c, Word a, Float64Words* output) {
    mark(c, 't', a, 0, 0, 0, 1); *output = as_words(static_cast<double>(as_float(a)));
}
Word d2f(void* c, Float64Words a) {
    mark(c, 'u', a.low, a.high, 0, 0, 2); volatile float result = static_cast<float>(as_double(a)); return as_word(result);
}
void dsqrt(void* c, Float64Words a, Float64Words* output) {
    mark(c, 'q', a.low, a.high, 0, 0, 2); volatile double result = std::sqrt(as_double(a)); *output = as_words(result);
}
void dmul(void* c, Float64Words a, Float64Words b, Float64Words* output) {
    mark(c, 'M', a.low, a.high, b.low, b.high, 4); volatile double x = as_double(a), y = as_double(b); volatile double z = x * y; *output = as_words(z);
}
void ddiv(void* c, Float64Words a, Float64Words b, Float64Words* output) {
    mark(c, 'D', a.low, a.high, b.low, b.high, 4); volatile double x = as_double(a), y = as_double(b); volatile double z = x / y; *output = as_words(z);
}
Word dless(void* c, Float64Words a, Float64Words b) {
    mark(c, 'L', a.low, a.high, b.low, b.high, 4); return static_cast<Word>(as_double(a) < as_double(b));
}

MathServices make_services(Context& c) {
    return {&c, sizeof(c), &fmul, &fadd, &fsub, &fdiv, &fequal,
            &f2d, &d2f, &dsqrt, &dmul, &ddiv, &dless};
}

bool equal_words(const Word* actual, const Word* expected, std::size_t n) {
    for (std::size_t i = 0; i != n; ++i) if (actual[i] != expected[i]) return false;
    return true;
}

int guards() {
    constexpr Word sentinel = 0x5a5a5a5au;
    Word plane[4] = {0, 0, 0x3f800000u, 0};
    Word point[3] = {0, 0, 0x3f800000u};
    Word vector[3] = {0, 0, 0xbf800000u};
    Word output[3] = {sentinel, sentinel, sentinel};
    Context context{static_cast<std::uintptr_t>(0x12345678abcdef01ull)};
    auto service = make_services(context);
    const Word expected_output[3] = {sentinel, sentinel, sentinel};

    auto result = intersect_line(nullptr, point, vector, output, &service);
    if (result.status != Status::invalid_argument || !equal_words(output, expected_output, 3)) return 10;
    if (context.calls != 0) return 11;

    alignas(Word) unsigned char bad_storage[sizeof(plane) + alignof(Word)]{};
    auto* misaligned = reinterpret_cast<const Word*>(bad_storage + 1);
    result = intersect_line(misaligned, point, vector, output, &service);
    if (result.status != Status::invalid_argument || context.calls != 0) return 12;

    result = intersect_line(plane, point, vector, plane + 1, &service);
    if (result.status != Status::invalid_argument || context.calls != 0) return 13;

    auto context_alias = service;
    context_alias.context = plane;
    context_alias.context_extent = sizeof(plane);
    result = intersect_line(plane, point, vector, output, &context_alias);
    if (result.status != Status::invalid_argument || context.calls != 0) return 14;

    auto missing = service;
    missing.divide_double = nullptr;
    result = intersect_line(plane, point, vector, output, &missing);
    if (result.status != Status::invalid_argument || context.calls != 0) return 15;

    // Source false is not a port error and must not modify the caller's result.
    plane[0] = plane[1] = plane[2] = 0;
    plane[2] = 0x3f800000u;
    vector[0] = 0x3f800000u; vector[1] = vector[2] = 0;
    result = intersect_line(plane, point, vector, output, &service);
    if (result.status != Status::complete || result.source_boolean != 0 ||
        !equal_words(output, expected_output, 3)) return 16;
    Context mutation_context{static_cast<std::uintptr_t>(0x12345678abcdef01ull)};
    auto mutable_service = make_services(mutation_context);
    mutable_service.add = &fadd;
    mutation_context.mutable_services = &mutable_service;
    // The first imported multiply mutates the caller's table; a call must use
    // the prevalidated snapshot for all later operation dispatches.
    const Word mutation_plane[4] = {0x3f800000u, 0x3f800000u, 0, 0xbf800000u};
    const Word mutation_point[3] = {0x3f800000u, 0, 0};
    const Word mutation_vector[3] = {0, 0x3f800000u, 0};
    Word mutation_output[3] = {sentinel, sentinel, sentinel};
    result = intersect_line(mutation_plane, mutation_point, mutation_vector,
                            mutation_output, &mutable_service);
    if (result.status != Status::complete || mutation_context.replacement_calls != 0 ||
        mutation_output[0] != 0x3f800000u) return 17;
    std::puts("{\"guard_cases\":7,\"invalid_calls\":0}");
    return 0;
}

Word parse(const char* text) { return static_cast<Word>(std::strtoul(text, nullptr, 16)); }
void print_words(const char* key, const Word* values, std::size_t count, bool comma) {
    std::printf("%s\"%s\":[", comma ? "," : "", key);
    for (std::size_t i = 0; i != count; ++i) std::printf("%s%u", i ? "," : "", values[i]);
    std::putchar(']');
}

} // namespace

int main(int argc, char** argv) {
    if (argc == 2 && std::strcmp(argv[1], "guards") == 0) return guards();
    if (argc < 2) return 2;
    const char* mode = argv[1];
    std::size_t expected = 0;
    if (!std::strcmp(mode, "line")) expected = 13;
    else if (!std::strcmp(mode, "two")) expected = 14;
    else if (!std::strcmp(mode, "three")) expected = 15;
    else return 2;
    if (static_cast<std::size_t>(argc - 2) != expected) return 2;
    Word first[4]{}, second[4]{}, third[4]{}, point[3]{}, vector[3]{}, output[3]{};
    int index = 2;
    auto read = [&argv, &index](Word* target, std::size_t n) {
        for (std::size_t i = 0; i != n; ++i) target[i] = parse(argv[index++]);
    };
    if (!std::strcmp(mode, "line")) { read(first,4); read(point,3); read(vector,3); read(output,3); }
    if (!std::strcmp(mode, "two")) { read(first,4); read(second,4); read(point,3); read(vector,3); }
    if (!std::strcmp(mode, "three")) { read(first,4); read(second,4); read(third,4); read(output,3); }

    Context context{static_cast<std::uintptr_t>(0x12345678abcdef01ull)};
    const auto service = make_services(context);
    CallResult result{};
    if (!std::strcmp(mode, "line"))
        result = intersect_line(first, point, vector, output, &service);
    else if (!std::strcmp(mode, "two"))
        result = intersect_two_planes(first, second, point, vector, &service);
    else
        result = intersect_three_planes(first, second, third, output, &service);

    std::printf("{\"status\":%d,\"source_boolean\":%u,", static_cast<int>(result.status), result.source_boolean);
    if (!std::strcmp(mode, "two")) {
        print_words("line_point", point, 3, false);
        print_words("line_vector", vector, 3, true);
    } else print_words("output", output, 3, false);
    std::printf(",\"events\":\"%.*s\",\"calls\":%zu,\"trace\":[",
                static_cast<int>(context.event_count), context.events, context.calls);
    // Also publish raw operands: repeated operators such as two multiplies in
    // a row must still be ordered exactly like the original ARM calls.
    for (std::size_t i = 0; i != context.trace_count; ++i) {
        const auto& entry = context.trace[i];
        std::printf("%s[\"%c\"", i ? "," : "", entry.operation);
        for (std::size_t j = 0; j != entry.count; ++j)
            std::printf(",%u", entry.inputs[j]);
        std::putchar(']');
    }
    std::puts("]}");
    return result.status == Status::complete ? 0 : 1;
}
