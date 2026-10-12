// Reads through ScriptCmd::read, AIChangeScript::read (two string buffers) and AIDoSkill::read
// (one string buffer behind a null guard), plus the destructors that free them.
#include "pydata-accessors/script_cmd.hpp"

#include <cstdio>
#include <cstring>
#include <vector>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

// Serves a fixed byte sequence; ReadBytes fails once it runs out.
class ByteStream : public IStreamBase {
public:
    explicit ByteStream(std::vector<unsigned char> bytes) : m_bytes(std::move(bytes)) {}
    bool ReadBytes(void* dst, std::size_t n) override {
        if (m_pos + n > m_bytes.size()) return false;
        std::memcpy(dst, m_bytes.data() + m_pos, n);
        m_pos += n;
        return true;
    }
private:
    std::vector<unsigned char> m_bytes;
    std::size_t m_pos = 0;
};

static void putWord(std::vector<unsigned char>& out, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<unsigned char>(v >> (8 * i)));
}

static void putText(std::vector<unsigned char>& out, const char* s) {
    for (std::size_t i = 0; s[i]; ++i) out.push_back(static_cast<unsigned char>(s[i]));
}

int main() {
    // ScriptCmd::read stores one little-endian int32 at +0x4.
    {
        std::vector<unsigned char> b;
        putWord(b, 0xfffffffeu);  // -2
        ByteStream s(b);
        Structs::ScriptCmd base;
        base.read(&s);
        CHECK(base.m_w1 == -2);
    }

    // AIChangeScript: int32 base word, then length 3 and "abc", then length 2 and "xy".
    {
        std::vector<unsigned char> b;
        putWord(b, 7);
        putWord(b, 3);
        putText(b, "abc");
        putWord(b, 2);
        putText(b, "xy");
        ByteStream s(b);
        Structs::AIChangeScript* c = new Structs::AIChangeScript();
        c->read(&s);
        CHECK(c->m_w1 == 7);
        CHECK(c->m_w2 == 3);
        CHECK(c->m_w4 == 2);
        CHECK(c->m_w3 != nullptr && std::strcmp(c->m_w3, "abc") == 0);
        CHECK(c->m_w5 != nullptr && std::strcmp(c->m_w5, "xy") == 0);
        delete c;  // frees both buffers
    }

    // AIDoSkill: int32 (+0x8), length 2 (+0xc) and "hi" behind a null guard at +0x10.
    {
        std::vector<unsigned char> b;
        putWord(b, 9);            // ScriptCmd word at +0x4
        putWord(b, 0xfffffffbu);  // -5 at +0x8
        putWord(b, 2);            // length at +0xc
        putText(b, "hi");
        ByteStream s(b);
        Structs::AIDoSkill* d = new Structs::AIDoSkill();
        d->read(&s);
        CHECK(d->m_w1 == 9);
        CHECK(d->m_w2 == -5);
        CHECK(d->m_w3 == 2);
        CHECK(d->m_w4 != nullptr && std::strcmp(d->m_w4, "hi") == 0);
        d->finalize();  // frees the buffer at +0x10
        d->m_w4 = nullptr;
        delete d;
    }

    // A short stream stops the read: ScriptCmd::read leaves the word at zero when no bytes arrive.
    {
        ByteStream s(std::vector<unsigned char>{1, 2});
        Structs::ScriptCmd base;
        base.read(&s);
        CHECK(base.m_w1 == 0);
    }

    std::printf("%s\n", failures ? "script_cmd_records: FAILED" : "script_cmd_records: ok");
    return failures != 0;
}
