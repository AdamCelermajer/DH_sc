#include "../character_zonability.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <vector>

using namespace dh2::character_zonability;
namespace {
struct Reader {
    std::vector<std::uint8_t> bytes;
    std::size_t offset = 0;
    explicit Reader(const char* path) {
        std::ifstream stream(path, std::ios::binary);
        if (!stream) throw std::runtime_error("missing ARM zonability corpus");
        bytes = {std::istreambuf_iterator<char>(stream), {}};
    }
    template<class T> T get() {
        if (offset + sizeof(T) > bytes.size()) throw std::runtime_error("truncated corpus");
        T value{};
        std::memcpy(&value, bytes.data() + offset, sizeof(T));
        offset += sizeof(T);
        return value;
    }
};
struct Context {
    std::uint32_t player = 0, type = 0;
    std::uintptr_t captured = 0;
    std::vector<Operation> requests;
};
std::int32_t invoke(void* opaque, State*, const Request* request, Response* response) {
    auto& context = *static_cast<Context*>(opaque);
    if (!request || !response || request->reserved || !request->character)
        return 1;
    if (!context.captured) context.captured = request->character;
    if (request->character != context.captured) return 2;
    context.requests.push_back(request->operation);
    switch (request->operation) {
        case Operation::is_player: response->word = context.player; break;
        case Operation::is_faerie: response->word = context.type == 3; break;
        default: return 3;
    }
    return 0;
}
}

int main(int argc, char** argv) {
    try {
        if (argc != 2) throw std::runtime_error("usage: zonability-replay corpus.bin");
        Reader reader(argv[1]);
        if (reader.get<std::uint32_t>() != 0x314e5a43u)
            throw std::runtime_error("wrong ARM zonability corpus magic");
        const auto count = reader.get<std::uint32_t>();
        unsigned callbacks = 0;
        constexpr std::uintptr_t identity = 0x100000123ull;
        for (std::uint32_t i = 0; i < count; ++i) {
            const auto player = reader.get<std::uint32_t>();
            const auto type = reader.get<std::uint32_t>();
            const auto expected_zonable = reader.get<std::uint32_t>();
            const auto trace_count = reader.get<std::uint32_t>();
            std::array<std::uint32_t, 8> trace{};
            for (auto& value : trace) value = reader.get<std::uint32_t>();

            Context context{};
            context.player = player;
            context.type = type;
            State state{identity};
            const Services services{&context, invoke};
            Result result{};
            if (evaluate(&state, &services, &result) != Status::complete)
                throw std::runtime_error("portable zonability query failed");
            const auto expected_decision = player ? Decision::player
                : type == 3 ? Decision::faerie : Decision::base_condition;
            if (result.zonable != expected_zonable || result.decision != expected_decision ||
                result.captured_character != identity) {
                std::fprintf(stderr, "case %u: player=%08x type=%08x original=%u helper=(decision=%u zonable=%u captured=%llx)\n",
                    i, player, type, expected_zonable, static_cast<unsigned>(result.decision),
                    result.zonable, static_cast<unsigned long long>(result.captured_character));
                throw std::runtime_error("portable result/order differs from ARM source");
            }

            std::array<std::uint32_t, 2> expected_requests{};
            std::uint32_t expected_count = 0;
            for (std::uint32_t j = 0; j < trace_count; ++j) {
                if (trace[j] == 1) expected_requests[expected_count++] =
                    static_cast<std::uint32_t>(Operation::is_player);
                else if (trace[j] == 2) expected_requests[expected_count++] =
                    static_cast<std::uint32_t>(Operation::is_faerie);
            }
            if (expected_count != context.requests.size() || result.service_calls != expected_count) {
                std::fprintf(stderr, "case %u: source queries=%u helper queries=%u result calls=%u trace=",
                    i, expected_count, static_cast<unsigned>(context.requests.size()), result.service_calls);
                for (std::uint32_t j = 0; j < trace_count; ++j) std::fprintf(stderr, "%u,", trace[j]);
                std::fprintf(stderr, "\n");
                throw std::runtime_error("ARM top-level query count differs");
            }
            for (std::uint32_t j = 0; j < expected_count; ++j)
                if (static_cast<std::uint32_t>(context.requests[j]) != expected_requests[j])
                    throw std::runtime_error("ARM top-level query order differs");
            callbacks += expected_count;
        }
        if (reader.offset != reader.bytes.size()) throw std::runtime_error("trailing ARM corpus data");
        std::printf("{\"validation\":\"PASS\",\"arm_cases\":%u,\"portable_provider_calls\":%u,\"source_identity_width\":%u}\n",
                    count, callbacks, static_cast<unsigned>(sizeof(std::uintptr_t) * 8));
    } catch (const std::exception& exception) {
        std::fprintf(stderr, "%s\n", exception.what());
        return 1;
    }
    return 0;
}
