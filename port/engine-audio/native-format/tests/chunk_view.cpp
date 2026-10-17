#include "../chunk_view.hpp"

#include <array>
#include <charconv>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace v = dh2::engine_audio::native_format;
namespace {
std::size_t checks = 0;
void require(bool b, const char* why) {
    ++checks;
    if (!b) throw std::runtime_error(why);
}
void put32(std::uint8_t* p, std::uint32_t n) {
    for (unsigned i = 0; i != 4; ++i) p[i] = static_cast<std::uint8_t>(n >> (8 * i));
}
std::uint32_t tag(const char* p) {
    return v::read_u32_le(reinterpret_cast<const std::uint8_t*>(p));
}
bool unchanged(const v::ChunkView& p) {
    return p.tag == 123 && p.header_offset == 456 && p.payload == nullptr && p.payload_size == 789;
}
bool unchanged(const v::ContainerLayout& p) {
    return p.first_section_read_bytes == 1 && p.declared_file_bytes == 2 && p.audio_base == 3 &&
        p.chunk_file_offset == 4 && p.chunk_bytes == 5 && p.audio_bytes == 6;
}
void guards() {
    alignas(std::max_align_t) std::array<std::uint8_t, 128> raw{};
    auto* b = raw.data();
    put32(b, v::kMagicVoxN); put32(b + 4, 16); put32(b + 16, 128); put32(b + 20, 64);
    for (std::size_t n = 0; n != 9; ++n) {
        v::HeaderView h{111};
        const bool ok = v::read_header(b, n, &h);
        require(ok == (n == 8), "header truncation");
        require(h.first_section_read_bytes == (ok ? 16u : 111u), "header failure preservation");
    }
    v::HeaderView h{111};
    require(!v::read_header(nullptr, 8, &h) && h.first_section_read_bytes == 111, "null header input");
    require(!v::read_header(b, 8, nullptr), "null header output");
    require(!v::read_header(b, 8, reinterpret_cast<v::HeaderView*>(b + 1)), "unaligned header output");
    require(!v::read_header(b, 8, reinterpret_cast<v::HeaderView*>(b)), "header input/output alias");
    put32(b, 0); require(!v::read_header(b, 8, &h) && h.first_section_read_bytes == 111, "bad magic");
    put32(b, v::kMagicVoxN);
    const auto* near_end = reinterpret_cast<const std::uint8_t*>(std::numeric_limits<std::uintptr_t>::max() - 3);
    require(!v::read_header(near_end, 8, &h), "input address overflow before read");
    for (std::size_t n = 0; n != 24; ++n) {
        v::ContainerLayout l{1,2,3,4,5,6};
        require(v::read_container_layout(b, n, 128, &l) == v::LayoutReadStatus::kMalformed && unchanged(l),
                "prefix truncation preservation");
    }
    v::ContainerLayout l{1,2,3,4,5,6};
    require(v::read_container_layout(b, 24, 128, &l) == v::LayoutReadStatus::kLayout &&
            l.chunk_file_offset == 24 && l.chunk_bytes == 32 && l.audio_bytes == 64, "container window");
    l={1,2,3,4,5,6};
    for (const std::uint32_t n : {0u, 1u, 8u, 15u, 17u, 0xffffffffu}) {
        put32(b + 4, n);
        require(v::read_container_layout(b, 24, 128, &l) == v::LayoutReadStatus::kUnsupportedHeader && unchanged(l),
                "unsupported header count");
    }
    put32(b + 4, 16);
    for (const std::uint32_t n : {0u, 24u, 31u, 129u, 0xffffffffu}) {
        put32(b + 20, n);
        require(v::read_container_layout(b, 24, 128, &l) == v::LayoutReadStatus::kMalformed && unchanged(l),
                "audio boundary rejection");
    }
    put32(b + 20, 64);
    require(v::read_container_layout(b, 24, 127, &l) == v::LayoutReadStatus::kMalformed && unchanged(l), "size mismatch");
    require(v::read_container_layout(b, 24, 23, &l) == v::LayoutReadStatus::kMalformed && unchanged(l), "prefix exceeds physical");
    require(v::read_container_layout(nullptr, 24, 128, &l) == v::LayoutReadStatus::kMalformed, "null layout input");
    require(v::read_container_layout(b, 24, 128, nullptr) == v::LayoutReadStatus::kMalformed, "null layout output");
    require(v::read_container_layout(b, 24, 128, reinterpret_cast<v::ContainerLayout*>(b + 1)) ==
            v::LayoutReadStatus::kMalformed, "unaligned layout output");
    require(v::read_container_layout(b, 64, 128, reinterpret_cast<v::ContainerLayout*>(b + 16)) ==
            v::LayoutReadStatus::kMalformed, "layout input/output alias");
    if (sizeof(std::size_t) > 4)
        require(v::read_container_layout(b, 24, static_cast<std::size_t>(0x100000000ull), &l) ==
                v::LayoutReadStatus::kMalformed && unchanged(l), "physical length exceeds u32");
    put32(b + 16, 32); put32(b + 20, 32);
    require(v::read_container_layout(b, 24, 32, &l) == v::LayoutReadStatus::kLayout &&
            l.chunk_bytes == 0 && l.audio_bytes == 0, "minimal empty container");

    put32(b, tag("Segm")); put32(b + 4, 4); b[8] = 9;
    for (std::size_t n = 1; n != 12; ++n) {
        std::size_t off=0; v::ChunkView c{123,456,nullptr,789};
        require(v::next_chunk(b, n, &off, &c) == v::ChunkReadStatus::kMalformed && off == 0 && unchanged(c),
                "chunk header/payload truncation");
    }
    std::size_t off=0; v::ChunkView c{123,456,nullptr,789};
    require(v::next_chunk(b, 12, &off, &c) == v::ChunkReadStatus::kChunk && off == 12 &&
            c.tag == v::kTagSegm && c.payload == b + 8 && c.payload_size == 4 && c.payload[0] == 9, "exact payload boundary");
    c={123,456,nullptr,789};
    require(v::next_chunk(b, 12, &off, &c) == v::ChunkReadStatus::kEnd && off == 12 && unchanged(c), "end preservation");
    off=0; require(v::next_chunk(nullptr, 0, &off, &c) == v::ChunkReadStatus::kEnd && unchanged(c), "empty null span");
    require(v::next_chunk(nullptr, 8, &off, &c) == v::ChunkReadStatus::kMalformed && unchanged(c), "null nonempty span");
    require(v::next_chunk(b, 12, nullptr, &c) == v::ChunkReadStatus::kMalformed, "null offset");
    require(v::next_chunk(b, 12, &off, nullptr) == v::ChunkReadStatus::kMalformed, "null chunk output");
    require(v::next_chunk(b, 12, reinterpret_cast<std::size_t*>(b + 1), &c) == v::ChunkReadStatus::kMalformed, "unaligned offset");
    require(v::next_chunk(b, 12, &off, reinterpret_cast<v::ChunkView*>(b + 1)) == v::ChunkReadStatus::kMalformed, "unaligned chunk output");
    require(v::next_chunk(b, 64, reinterpret_cast<std::size_t*>(b), &c) == v::ChunkReadStatus::kMalformed, "input/offset alias");
    require(v::next_chunk(b, 64, &off, reinterpret_cast<v::ChunkView*>(b + 16)) == v::ChunkReadStatus::kMalformed, "input/chunk alias");
    alignas(std::max_align_t) std::array<std::uint8_t,128> output{};
    require(v::next_chunk(b, 12, reinterpret_cast<std::size_t*>(output.data()),
            reinterpret_cast<v::ChunkView*>(output.data())) == v::ChunkReadStatus::kMalformed, "offset/output alias");
    off=13; require(v::next_chunk(b, 12, &off, &c) == v::ChunkReadStatus::kMalformed && off == 13 && unchanged(c), "offset beyond buffer");
    off=std::numeric_limits<std::size_t>::max();
    require(v::next_chunk(b, 12, &off, &c) == v::ChunkReadStatus::kMalformed && unchanged(c), "max offset no wrap");
    off=0; put32(b + 4, 0xffffffffu);
    require(v::next_chunk(b, 12, &off, &c) == v::ChunkReadStatus::kMalformed && off == 0 && unchanged(c), "max payload no wrap");
    require(v::next_chunk(near_end, 8, &off, &c) == v::ChunkReadStatus::kMalformed && unchanged(c), "chunk address overflow");
    put32(b + 4, 0);
    require(v::next_chunk(b, 8, &off, &c) == v::ChunkReadStatus::kChunk && off == 8 && c.payload_size == 0,
            "zero-length payload progresses");
    const std::array<std::pair<const char*,std::uint32_t>,9> known{{
        {"Afmt",v::kTagAfmt},{"Segm",v::kTagSegm},{"Cuse",v::kTagCuse},
        {"Grps",v::kTagGrps},{"Grpe",v::kTagGrpe},{"Rule",v::kTagRule},
        {"Plst",v::kTagPlst},{"Stat",v::kTagStat},{"Trsn",v::kTagTrsn}}};
    for (const auto& row : known)
        require(row.second == tag(row.first) && v::is_known_tag(row.second), "literal ASCII tag regression");
    require(v::kTagGprs==tag("Grps") && v::kTagGpre==tag("Grpe") &&
            v::kTagPLst==tag("Plst") && v::kTagTrns==tag("Trsn"), "candidate aliases corrected");
    for (const char* s : {"eSgm","Gprs","Gpre","PLst","Trns","????"})
        require(!v::is_known_tag(tag(s)), "incorrect/unknown tags unsupported");
    put32(b, tag("????")); put32(b + 4, 0); off=0;
    require(v::next_chunk(b, 8, &off, &c) == v::ChunkReadStatus::kChunk && !v::is_known_tag(c.tag), "unknown tag framed raw");
    put32(b, tag("Afmt")); put32(b + 4, 12); b[8]=0xff; b[9]=0xff; off=0;
    require(v::next_chunk(b, 20, &off, &c) == v::ChunkReadStatus::kChunk &&
            c.payload[0] == 0xff && c.payload[1] == 0xff && c.payload_size == 12,
            "unknown compression bytes preserved without interpretation");
    std::cout << "{\"status\":\"PASS\",\"checks\":" << checks << ",\"mismatches\":0}" << std::endl;
}

void inspect(const char* path, const char* physical) {
    std::uint64_t physical_size=0;
    const std::string word(physical);
    const auto parsed=std::from_chars(word.data(),word.data()+word.size(),physical_size);
    if (parsed.ec!=std::errc{} || parsed.ptr!=word.data()+word.size() || physical_size>std::numeric_limits<std::size_t>::max())
        throw std::runtime_error("invalid physical size");
    std::ifstream file(path,std::ios::binary|std::ios::ate);
    if (!file) throw std::runtime_error("cannot open metadata fixture");
    const auto end=file.tellg();
    if (end<0 || end>1048576) throw std::runtime_error("metadata fixture exceeds one MiB bound");
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(end));
    file.seekg(0);
    if (!bytes.empty() && !file.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size())))
        throw std::runtime_error("short fixture read");
    v::ContainerLayout layout{};
    const auto status=v::read_container_layout(bytes.data(),bytes.size(),static_cast<std::size_t>(physical_size),&layout);
    if (status!=v::LayoutReadStatus::kLayout) {
        std::cout << "{\"status\":\"" << (status==v::LayoutReadStatus::kUnsupportedHeader?"unsupported_header":"malformed") << "\"}" << std::endl;
        return;
    }
    if (layout.chunk_file_offset>bytes.size() || layout.chunk_bytes>bytes.size()-layout.chunk_file_offset)
        throw std::runtime_error("metadata fixture misses chunk window");
    std::cout << "{\"status\":\"ok\",\"chunk_start\":" << layout.chunk_file_offset
        << ",\"chunk_bytes\":" << layout.chunk_bytes << ",\"audio_base\":" << layout.audio_base
        << ",\"audio_bytes\":" << layout.audio_bytes << ",\"chunks\":[";
    std::size_t off=0, count=0; v::ChunkView c{};
    const auto* chunks=bytes.data()+layout.chunk_file_offset;
    for (;;) {
        const auto s=v::next_chunk(chunks,layout.chunk_bytes,&off,&c);
        if (s==v::ChunkReadStatus::kEnd) break;
        if (s!=v::ChunkReadStatus::kChunk) throw std::runtime_error("malformed chunk window");
        if (count++) std::cout << ',';
        std::cout << "{\"tag\":" << c.tag << ",\"offset\":" << c.header_offset+layout.chunk_file_offset
            << ",\"size\":" << c.payload_size << ",\"known\":" << (v::is_known_tag(c.tag)?"true":"false") << ",\"payload_hex\":\"";
        constexpr char digits[]="0123456789abcdef";
        for (std::size_t i=0;i!=c.payload_size;++i) std::cout << digits[c.payload[i]>>4] << digits[c.payload[i]&15];
        std::cout << "\"}";
    }
    std::cout << "],\"final_offset\":" << off << ",\"payloads_decoded\":false}" << std::endl;
}
}
int main(int argc,char** argv) {
    try {
        if (argc==2 && std::string(argv[1])=="--guards") guards();
        else if (argc==4 && std::string(argv[1])=="--inspect") inspect(argv[2],argv[3]);
        else throw std::runtime_error("usage: --guards | --inspect metadata_fixture physical_file_size");
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
    return 0;
}
