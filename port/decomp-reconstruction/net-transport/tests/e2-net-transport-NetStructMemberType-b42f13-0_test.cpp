// Test for batch e2-net-transport-NetStructMemberType-b42f13-0: SetValue change tracking and
// GetValue/TestValue for the float, std::string, ByteArray and CNetworkId instantiations.
// Erase needs a NetBitStream and is not covered here.
#include <cstdio>
#include <cstring>
#include <string>

#include "net-transport/net_struct.hpp"

// Stand-ins for the engine symbols this batch links against. The test links only this file, the batch
// source and tb_transport-NetStructMemberType-0.cpp, so the NetStructMember bodies are not pulled in
// (they need the whole NetBitStream and CMatching). Load and the destructor are never reached here.
// SetChanged only sets the flag; the change stamp is in tb_transport-NetStructMember-0.cpp.
std::uint64_t NetStruct::s_changeCounter = 0;
NetStructMember::~NetStructMember() {}
int NetStructMember::GetSizeBits() const { return 0; }
bool NetStructMember::Load(NetBitStream&) { return true; }
void NetStructMember::SetChanged() { changed_ = true; }
bool NetStructMember::IsChanged() {
    const bool result = changed_;
    changed_ = false;
    return result;
}

namespace {

int failures = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                \
        }                                                              \
    } while (0)

class FloatMember final : public NetStructMemberType<float> {
public:
    void Read(NetBitStream&) override {}
    void Write(NetBitStream&) const override {}
    bool TestValue(const float& v) const override { return NetStructMemberType<float>::TestValue(v); }
};

class StringMember final : public NetStructMemberType<std::string> {
public:
    void Read(NetBitStream&) override {}
    void Write(NetBitStream&) const override {}
    bool TestValue(const std::string& v) const override { return NetStructMemberType<std::string>::TestValue(v); }
};

class ByteArrayMember final : public NetStructMemberType<ByteArray> {
public:
    void Read(NetBitStream&) override {}
    void Write(NetBitStream&) const override {}
    bool TestValue(const ByteArray& v) const override { return NetStructMemberType<ByteArray>::TestValue(v); }
};

class NetworkIdMember final : public NetStructMemberType<CNetworkId> {
public:
    void Read(NetBitStream&) override {}
    void Write(NetBitStream&) const override {}
    bool TestValue(const CNetworkId& v) const override { return NetStructMemberType<CNetworkId>::TestValue(v); }
};

void TestFloat() {
    FloatMember m;
    CHECK(m.GetValue() == 0.0f);
    CHECK(!m.IsChanged());

    m.SetValue(1.5f);
    CHECK(m.GetValue() == 1.5f);
    CHECK(m.IsChanged());        // a new value marks the member changed
    m.SetValue(1.5f);
    CHECK(!m.IsChanged());       // the same value does not

    CHECK(m.TestValue(0.0f));    // TestValue is always true for float
}

void TestString() {
    StringMember m;
    m.SetValue(std::string("abc"));
    CHECK(m.GetValue() == "abc");
    CHECK(m.IsChanged());
    m.SetValue(std::string("abc"));
    CHECK(!m.IsChanged());
    m.SetValue(std::string("abcd"));  // different length
    CHECK(m.GetValue() == "abcd");
    CHECK(m.IsChanged());
    CHECK(m.TestValue(std::string()));
}

void TestByteArray() {
    ByteArrayMember m;
    const std::uint8_t bytes[3] = {1, 2, 3};

    ByteArray src;
    src.SetBuffer(bytes, 3);
    CHECK(src.size == 3);

    m.SetValue(src);
    CHECK(m.GetValue().size == 3);
    CHECK(std::memcmp(m.GetValue().data, bytes, 3) == 0);
    CHECK(m.IsChanged());

    m.SetValue(src);             // same bytes: no change
    CHECK(!m.IsChanged());

    const std::uint8_t other[3] = {1, 2, 4};
    ByteArray diff;
    diff.SetBuffer(other, 3);
    m.SetValue(diff);            // same size, different bytes: changed
    CHECK(m.IsChanged());
    CHECK(std::memcmp(m.GetValue().data, other, 3) == 0);

    CHECK(m.TestValue(src));

    CustomFree(src.data);
    CustomFree(diff.data);
    CustomFree(m.GetValue().data);
}

void TestNetworkId() {
    NetworkIdMember m;
    CHECK(m.TestValue(CNetworkId()));  // TestValue is always true for CNetworkId
    CHECK(!m.IsChanged());             // nothing was set
}

}  // namespace

int main() {
    TestFloat();
    TestString();
    TestByteArray();
    TestNetworkId();
    if (failures == 0) {
        std::printf("e2-net-transport-NetStructMemberType-b42f13-0 test: all passed\n");
        return 0;
    }
    std::printf("e2-net-transport-NetStructMemberType-b42f13-0 test: %d failure(s)\n", failures);
    return 1;
}
