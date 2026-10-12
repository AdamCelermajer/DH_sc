// Batch e1-net-transport-NetStructMemberType-0: part 1 of the NetStructMemberType<T> instantiations
// (GetValue, TestValue, SetValue, Erase for int, unsigned int, bool, std::string and ByteArray).
//
// The explicit specializations are declared in include/net-transport/net_struct.hpp, right after the
// template. The member layout is value_ at +0x20 (the generic template). The bool instantiation
// differs: the original keeps its value at +0x1d (see the PARTIAL notes below).

#include "net-transport/net_struct.hpp"

// 0x36d2e4 NetStructMemberType<bool>::~NetStructMemberType() (D1)
// skipped: the empty complete-object destructor is the compiler-generated one. The template has no
// user-declared destructor, so the implicit one has the same empty body.

// 0x36d2e8 NetStructMemberType<unsigned int>::GetValue() const
// Pseudocode returns a1 + 32; assembly is ADD R0, R0, #0x20; BX LR. The result is the address of
// the value, so the port returns a const reference.
template <>
const std::uint32_t& NetStructMemberType<std::uint32_t>::GetValue() const {
    return value_;  // +0x20
}

// 0x36d2f0 NetStructMemberType<int>::GetValue() const
// Same as the unsigned int form: this + 0x20.
template <>
const std::int32_t& NetStructMemberType<std::int32_t>::GetValue() const {
    return value_;  // +0x20
}

// 0x36d2f8 NetStructMemberType<std::string>::GetValue() const
// Same as the int form: this + 0x20.
template <>
const std::string& NetStructMemberType<std::string>::GetValue() const {
    return value_;  // +0x20
}

// 0x36d300 NetStructMemberType<unsigned int>::~NetStructMemberType() (D1)
// skipped: empty compiler-generated destructor, same as 0x36d2e4.

// 0x36d304 NetStructMemberType<int>::~NetStructMemberType() (D1)
// skipped: empty compiler-generated destructor, same as 0x36d2e4.

// 0x36d308 NetStructMemberType<bool>::GetValue() const
// PARTIAL: the pseudocode returns a1 + 29, so the original value is at +0x1d. The port's template keeps
// value_ at +0x20 for every instantiation, so this returns the reference to the port's value_. The
// returned value is the same, but the layout does not match the binary.
template <>
const bool& NetStructMemberType<bool>::GetValue() const {
    return value_;  // +0x1d in the original, +0x20 here
}

// 0x36d388 NetStructMemberType<ByteArray>::TestValue(ByteArray const&)
// Pseudocode and assembly: MOV R0, #1; BX LR. Always true; the argument is not read.
template <>
bool NetStructMemberType<ByteArray>::TestValue(const ByteArray& /*value*/) const {
    return true;
}

// 0x36d390 NetStructMemberType<ByteArray>::GetValue() const
// Same as the int form: this + 0x20.
template <>
const ByteArray& NetStructMemberType<ByteArray>::GetValue() const {
    return value_;  // +0x20 (data pointer at +0x20, size at +0x24)
}

// 0x36d46c NetStructMemberType<std::string>::TestValue(std::string const&)
// Always true (MOV R0, #1; BX LR).
template <>
bool NetStructMemberType<std::string>::TestValue(const std::string& /*value*/) const {
    return true;
}

// 0x36d474 NetStructMemberType<int>::TestValue(int const&)
// Always true (MOV R0, #1; BX LR).
template <>
bool NetStructMemberType<std::int32_t>::TestValue(const std::int32_t& /*value*/) const {
    return true;
}

// 0x36d47c NetStructMemberType<unsigned int>::TestValue(unsigned int const&)
// Always true (MOV R0, #1; BX LR).
template <>
bool NetStructMemberType<std::uint32_t>::TestValue(const std::uint32_t& /*value*/) const {
    return true;
}

// 0x36d484 NetStructMemberType<bool>::TestValue(bool const&)
// Always true (MOV R0, #1; BX LR).
template <>
bool NetStructMemberType<bool>::TestValue(const bool& /*value*/) const {
    return true;
}

// 0x36d4ec NetStructMemberType<int>::~NetStructMemberType() (D0, deleting destructor)
// skipped: the compiler generates the deleting destructor. It stores the NetStructMember vtable
// (ZTV15NetStructMember + 8) in the object, then calls CustomFree. The port deletes through the
// virtual destructor of NetStructMember, and CustomFree is the engine allocator.

// 0x36d588 NetStructMemberType<unsigned int>::~NetStructMemberType() (D0, deleting destructor)
// skipped: same as 0x36d4ec.

// 0x36d5f0 NetStructMemberType<bool>::~NetStructMemberType() (D0, deleting destructor)
// skipped: same as 0x36d4ec.

// 0x36d9f0 NetStructMemberType<int>::SetValue(int const&)
// Pseudocode: if (value_ != *value) { value_ = *value; SetChanged(); }. Assembly matches (LDR, CMP,
// BXEQ LR, STR, then a tail call to NetStructMember::SetChanged). The original returns the member
// pointer through R0. The port's SetValue returns void, so that is not kept.
template <>
void NetStructMemberType<std::int32_t>::SetValue(const std::int32_t& value) {
    if (value_ != value) {  // +0x20
        value_ = value;     // +0x20
        SetChanged();       // NetStructMember::SetChanged (0x814f84)
    }
}

// 0x36da08 NetStructMemberType<unsigned int>::SetValue(unsigned int const&)
// Same as the int form.
template <>
void NetStructMemberType<std::uint32_t>::SetValue(const std::uint32_t& value) {
    if (value_ != value) {  // +0x20
        value_ = value;     // +0x20
        SetChanged();       // NetStructMember::SetChanged (0x814f84)
    }
}

// 0x36da20 NetStructMemberType<bool>::SetValue(bool const&)
// PARTIAL: the pseudocode reads and writes the byte at +0x1d (SetValue compares the byte at +29 and
// stores it there). The port's value_ is at +0x20, so the comparison and store use the port's member.
// The SetChanged call and its ordering match.
template <>
void NetStructMemberType<bool>::SetValue(const bool& value) {
    if (value_ != value) {  // +0x1d in the original, +0x20 here
        value_ = value;     // +0x1d in the original, +0x20 here
        SetChanged();       // NetStructMember::SetChanged (0x814f84)
    }
}

// 0x36dd64 NetStructMemberType<int>::Erase(NetBitStream &)
// PARTIAL: the pseudocode saves the value, calls NetStructMember::Load(this, stream) and then restores
// the saved value. Its result is returned as an int. The port's NetStructMember::Load returns bool, so
// the value is converted to int; the original's return width is not confirmed.
template <>
int NetStructMemberType<std::int32_t>::Erase(NetBitStream& stream) {
    const std::int32_t saved = value_;               // LDR R5, [R0, #0x20]
    const int result = NetStructMember::Load(stream);  // BL NetStructMember::Load (0x81510c)
    value_ = saved;                                  // STR R5, [R4, #0x20]
    return result;
}
