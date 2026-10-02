#include "../skin.hpp"
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <vector>

static std::uint32_t random_word = 20261002;
static std::uint32_t next() {
    random_word ^= random_word << 13; random_word ^= random_word >> 17;
    random_word ^= random_word << 5; return random_word;
}
static std::uint32_t word(const std::uint8_t* p) {
    return p[0] | (std::uint32_t(p[1]) << 8) | (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
}
static void write(std::vector<std::uint8_t>& b, std::uint32_t offset, std::uint32_t w) {
    for (int i = 0; i < 4; ++i) b[offset + i] = static_cast<std::uint8_t>(w >> (i * 8));
}
static void probe(const std::vector<std::uint8_t>& bytes, std::size_t size) {
    const auto before = bytes;
    dh2::resources::BresView b{};
    if (dh2_bres_open(&b, bytes.data(), size) == dh2::resources::BresError::ok) {
        const auto n = dh2_bres_library_count(&b, dh2::resources::Library::controller);
        for (std::uint32_t i = 0; i < n; ++i) {
            dh2::skin::Skin s{};
            if (dh2_skin_open(&s, &b, i) != dh2::skin::Error::ok) continue;
            dh2::skin::Influence influence{};
            assert(dh2_skin_influence(&s, s.vertices, &influence) != dh2::skin::Error::ok);
            assert(!dh2_skin_joint_name(&s, -1));
            assert(!dh2_skin_joint_name(&s, s.joints));
            dh2::math::Matrix4f palette[256]{};
            for (std::uint32_t j = 0; j < s.joints; ++j)
                palette[j].m[0] = palette[j].m[5] = palette[j].m[10] = palette[j].m[15] = 1;
            assert(dh2_skin_palette(&s, palette, s.joints, palette, s.joints - 1) == dh2::skin::Error::capacity);
            assert(dh2_skin_palette(&s, palette, s.joints, palette, s.joints) == dh2::skin::Error::ok);
            dh2::math::Vector3f p{1,2,3};
            const auto result = dh2_skin_position(&s, s.vertices / 2, palette, s.joints, &p, &p);
            assert(result == dh2::skin::Error::ok || result == dh2::skin::Error::nonfinite);
        }
    }
    assert(bytes == before);
}
int main(int argc, char** argv) {
    assert(argc == 2); std::ifstream f(argv[1], std::ios::binary);
    std::vector<std::uint8_t> original((std::istreambuf_iterator<char>(f)), {});
    assert(original.size() > 192);
    probe(original, original.size());
    dh2::resources::BresView b{};
    assert(dh2_bres_open(&b, original.data(), original.size()) == dh2::resources::BresError::ok);
    dh2::skin::Skin skin{}; assert(dh2_skin_open(&skin, &b, 0) == dh2::skin::Error::ok);
    const auto controller_offset = word(original.data() + b.root_offset + 0x74);
    const std::uint32_t fields[] = {controller_offset, controller_offset + 4, controller_offset + 8,
        skin.record, skin.record + 4, skin.record + 0x70, skin.record + 0x74,
        skin.record + 0x78, skin.record + 0x7c, skin.record + 0x80, skin.record + 0x98};
    for (std::uint32_t i = 0; i < 5000; ++i) {
        auto bytes = original;
        if (i % 3 == 0) {
            // Sizes really shrink; range checks cannot borrow hidden tail bytes.
            bytes.resize(next() % bytes.size());
        } else if (i % 3 == 1) {
            write(bytes, fields[next() % (sizeof(fields) / sizeof(fields[0]))], next());
        } else {
            for (std::uint32_t j = 0, n = 1 + next() % 8; j < n; ++j)
                bytes[next() % bytes.size()] = static_cast<std::uint8_t>(next());
        }
        probe(bytes, bytes.size());
    }
    assert(dh2_skin_open(nullptr, &b, 0) == dh2::skin::Error::argument);
    assert(dh2_skin_open(&skin, nullptr, 0) == dh2::skin::Error::argument);
    std::puts("skin safety: 5000 corruption/truncation probes, guards, aliases, immutable inputs");
}
