#include "../resources.hpp"
#include <cstdint>

using namespace dh2::resources;
struct Capture {
    std::int32_t amount, error;
    ReadFile* file;
    void* user;
    std::uint32_t calls;
    std::int32_t position;
};
extern "C" void test_completion(std::int32_t n, std::int32_t e, ReadFile* f, void* user) {
    auto* c = static_cast<Capture*>(user);
    c->amount = n;
    c->error = e;
    c->file = f;
    c->user = user;
    ++c->calls;
    c->position = f->position();
}
extern "C" std::size_t test_memory_size() { return sizeof(MemoryReader); }
extern "C" std::size_t test_limit_size() { return sizeof(LimitReader); }
extern "C" std::size_t test_view_size() { return sizeof(BresView); }
extern "C" std::size_t test_capture_size() { return sizeof(Capture); }
