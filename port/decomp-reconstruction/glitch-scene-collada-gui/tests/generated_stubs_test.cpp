#include "glitch-scene-collada-gui/generated_stubs.hpp"

#include <cstdio>

// Each stub's constant or empty body is checked against the Hex-Rays pseudocode
// (see the address comments in generated_stubs.hpp).
using namespace glitch::collada::stubs;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CHECK(stub_60e62c(nullptr, nullptr) == 0u);
    CHECK(stub_60e62c(nullptr) == 0u);
    CHECK(stub_60ee1c(nullptr) == 16u);
    stub_60ee24(nullptr);
    CHECK(stub_60ee28(nullptr) == 16u);
    stub_60ee30(nullptr);
    CHECK(stub_60ee34(nullptr) == 16u);
    stub_60ee3c(nullptr);
    CHECK(stub_60ee40(nullptr) == 16u);
    stub_60ee48(nullptr);
    CHECK(stub_60ee4c(nullptr) == 16u);
    stub_60ee54(nullptr);
    CHECK(stub_60ee58(nullptr) == 12u);
    stub_60ee60(nullptr);
    CHECK(stub_60ee64(nullptr) == 12u);
    stub_60ee6c(nullptr);
    CHECK(stub_60ee70(nullptr) == 8u);
    stub_60ee78(nullptr);
    CHECK(stub_60ee7c(nullptr) == 8u);
    stub_60ee84(nullptr);
    CHECK(stub_60ee88(nullptr) == 8u);
    stub_60ee90(nullptr);
    stub_60ee9c(nullptr);
    CHECK(stub_60eea0(nullptr) == 3u);
    CHECK(stub_60ef0c(nullptr) == 12u);
    CHECK(stub_60ef14(nullptr) == 12u);
    CHECK(stub_60ef24(nullptr) == 12u);
    CHECK(stub_60ef2c(nullptr) == 12u);
    CHECK(stub_60ef3c(nullptr) == 12u);
    CHECK(stub_60ef44(nullptr) == 12u);
    CHECK(stub_60ef54(nullptr) == 12u);
    CHECK(stub_60ef5c(nullptr) == 12u);
    CHECK(stub_60efa0(nullptr) == 12u);
    CHECK(stub_60efa8(nullptr) == 12u);
    CHECK(stub_60efb8(nullptr) == 12u);
    CHECK(stub_60efc0(nullptr) == 12u);
    CHECK(stub_60efd0(nullptr) == 12u);
    CHECK(stub_60efd8(nullptr) == 12u);
    CHECK(stub_60efe8(nullptr) == 12u);
    CHECK(stub_60eff0(nullptr) == 12u);
    CHECK(stub_60f034(nullptr) == 16u);
    CHECK(stub_60f058(nullptr) == 16u);
    CHECK(stub_60f07c(nullptr) == 16u);
    CHECK(stub_60f0a0(nullptr) == 16u);
    CHECK(stub_60f0c4(nullptr) == 16u);
    stub_62fee4(nullptr);
    stub_62fee8(nullptr);
    CHECK(stub_62feec(nullptr) == 1717920100u);
    stub_62fef8(nullptr);
    stub_62ff04(nullptr);
    CHECK(stub_62ff38(nullptr) == 1u);
    CHECK(stub_637b78(nullptr) == 1734697316u);
    CHECK(stub_644a3c(nullptr) == 0u);
    CHECK(stub_646160(nullptr) == 1835360612u);
    stub_646188(nullptr);
    CHECK(stub_6493f8(nullptr) == 1298489700u);
    CHECK(stub_649904(nullptr) == 1u);
    stub_6499d0(nullptr);
    CHECK(stub_64bb84(nullptr) == 1885692260u);
    CHECK(stub_657994(nullptr) == 0u);
    CHECK(stub_65cd1c(nullptr) == 1852137828u);
    stub_65f288(nullptr);
    stub_662750(nullptr);
    stub_66275c(nullptr);
    stub_662768(nullptr);
    CHECK(stub_6669f8(nullptr) == 1936023908u);
    stub_6676b4(nullptr);
    stub_6676c0(nullptr);
    stub_6676c4(nullptr);
    CHECK(stub_667774(nullptr) == 0u);
    CHECK(stub_667cbc(nullptr) == 0u);
    stub_669b6c(nullptr);
    CHECK(stub_66b938(nullptr) == 0u);
    CHECK(stub_66d078(nullptr) == 0u);
    CHECK(stub_66e770(nullptr) == 0u);
    CHECK(stub_66f6b8(nullptr) == 1u);
    CHECK(stub_66f6c0(nullptr) == 1u);
    CHECK(stub_6e3110(nullptr) == 4u);
    CHECK(stub_6e4db4(nullptr) == 4u);
    CHECK(stub_6e5168(nullptr) == 1667588452u);
    CHECK(stub_6e5a2c(nullptr) == 1936618083u);
    std::printf("%s\n", failures ? "generated_stubs: FAILED" : "generated_stubs: ok");
    return failures != 0;
}
