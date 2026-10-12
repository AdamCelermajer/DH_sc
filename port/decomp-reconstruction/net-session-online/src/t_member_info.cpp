#include "net-session-online/t_member_info.hpp"

// 0x821dbc _ZN11tMemberInfoD1Ev
// Partial: the string buffer release is not reproduced (see the header).
tMemberInfo::~tMemberInfo() {
    value24_ = 0;
    memberId_ = -1;
}
