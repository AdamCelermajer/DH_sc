#include "net-session-online/t_member_info.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // The destructor sets the id to -1 and clears the +0x24 word. Read the fields through a
    // pointer that outlives the object's destruction (the object is destroyed in place).
    tMemberInfo* info = new tMemberInfo();
    info->SetForTest(5, 9);
    CHECK(info->MemberIdForTest() == 5);
    info->~tMemberInfo();
    CHECK(info->MemberIdForTest() == -1);
    CHECK(info->Value24ForTest() == 0);
    ::operator delete(info);

    if (failures == 0) std::printf("t_member_info_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
