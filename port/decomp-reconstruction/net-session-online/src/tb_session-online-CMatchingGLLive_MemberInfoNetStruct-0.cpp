#include "net-session-online/tb/cmatching_gl_live_member_info_net_struct.hpp"

#include <string>

// 0x81e680 _ZN15CMatchingGLLive19MemberInfoNetStructC2Ev
// Partial. The original runs the NetStruct base constructor (0x81388c, partial in net-transport:
// its packet history map is not initialised there), stores the derived vptr, builds the
// NetStructString<16> member at +0x130 from an empty string, destroys the temporary, then
// calls NetStruct::DeclareMember on the member. The port reproduces the member construction and
// the DeclareMember call. The NetStructString<16> constructor body (0x81e5a0, partial in
// net-transport) is not reproduced: NetStructStringStub16 only stores the value. The packet
// history is default-constructed empty here, as in the precedent ServerBackupNetStruct.
CMatchingGLLive::MemberInfoNetStruct::MemberInfoNetStruct()
    : NetStructBaseStub(), member_(std::string()) {
    DeclareMember(&member_);
}

// 0x81d1f4 _ZN15CMatchingGLLive19MemberInfoNetStructD1Ev
// Partial. The original destroys a std::string at +0x38 first, then (after the vptr stores)
// erases the packet history tree when its node count at +0x11c is nonzero and resets the header
// to empty. The erase and reset match std::map::clear, done here when the map is not empty.
// The +0x38 string is not in the port layout: its owner and constructor are not identified,
// and that range is inside the NetStruct base, so its destruction is not reproduced. The
// vptr stores and the NetStructString<16> member's inline teardown are the compiler's job here.
CMatchingGLLive::MemberInfoNetStruct::~MemberInfoNetStruct() {
    if (!packetHistory_.empty()) {
        packetHistory_.clear();
    }
}
