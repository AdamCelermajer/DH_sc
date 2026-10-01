// Host ASan/UBSan checks for the port's intentional checked-input behavior.
#include "../resources.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>
#include <vector>

using namespace dh2::resources;
int main(int argc, char** argv) {
    assert(argc == 2);
    std::ifstream file(argv[1], std::ios::binary);
    const std::vector<std::uint8_t> original{std::istreambuf_iterator<char>(file), {}};
    assert(original.size() >= 60);
    BresView view;
    assert(dh2_bres_open(&view, original.data(), original.size()) == BresError::ok);
    std::vector<Fixup> fixups(view.fixup_count);
    assert(dh2_bres_fixups(&view, fixups.data(), fixups.size()) == BresError::ok);
    for (const auto& pair : fixups) {
        assert(pair.field >= original.data() && pair.field + 4 <= original.data() + original.size());
        assert(pair.target >= original.data() && pair.target <= original.data() + original.size());
    }
    std::mt19937 random(0xd22026);
    for (unsigned iteration = 0; iteration < 10000; ++iteration) {
        auto bytes = original;
        for (unsigned mutation = 0; mutation < 4; ++mutation)
            bytes[random() % bytes.size()] = static_cast<std::uint8_t>(random());
        const auto size = iteration % 2 ? bytes.size() : random() % (bytes.size()+1);
        if (dh2_bres_open(&view, bytes.data(), size) != BresError::ok) continue;
        fixups.resize(view.fixup_count);
        assert(dh2_bres_fixups(&view, fixups.data(), fixups.size()) == BresError::ok);
        for (unsigned kind = 0; kind < static_cast<unsigned>(Library::count); ++kind) {
            const auto n = dh2_bres_library_count(&view, static_cast<Library>(kind));
            const auto* p = dh2_bres_library_item(&view, static_cast<Library>(kind), n ? n-1 : 0);
            assert(!p || (p >= bytes.data() && p < bytes.data()+size));
            assert(!dh2_bres_library_item(&view, static_cast<Library>(kind), -1));
        }
        const auto* version = dh2_bres_version(&view);
        assert(!version || std::strlen(version) < size);
    }
    std::uint8_t bytes[16]{}, output[16];
    std::memset(output, 0xa5, sizeof(output));
    MemoryReader reader(bytes, sizeof(bytes), "borrowed");
    assert(reader.seek(-1, false));
    assert(reader.read(output, 1) == 0);
    assert(output[0] == 0xa5 && reader.position() == -1);
    reader.cursor = 0x7fffffff;
    assert(reader.read(output, 2) == 0);
    MemoryReader empty(nullptr, 0, "empty");
    assert(!empty.valid() && empty.read(output, 1) == 0);
}
