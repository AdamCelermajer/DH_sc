#include "net-session-online/glx_player_login.hpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    GLXPlayerLogin login;
    CHECK(login.getAPNSIP() == nullptr);
    CHECK(login.getAPNSPort() == 0);
    CHECK(login.getAPNSPrivateKeyPassword() == nullptr);
    CHECK(login.getProductListCount() == 0);
    CHECK(login.getProductGGI(0) == -1);
    CHECK(login.getProductName(0) == nullptr);
    CHECK(!login.ShouldSendKeepAlive());        // disabled

    login.SetAPNSIPForTest("push.example");
    login.SetAPNSPortForTest(2195);
    CHECK(std::strcmp(login.getAPNSIP(), "push.example") == 0);
    CHECK(login.getAPNSPort() == 2195);

    login.SetProductsForTest({11, 12}, {21, 22}, {"alpha", "beta"});
    CHECK(login.getProductListCount() == 2);
    CHECK(login.getProductGGI(1) == 12);
    CHECK(login.getProductID(0) == 21);
    CHECK(std::strcmp(login.getProductName(1), "beta") == 0);
    CHECK(login.getProductID(2) == -1);
    CHECK(login.getProductGGI(-1) == -1);

    login.SetKeepAliveForTest(true, 0);
    // The stand-in clock reads 0, so the elapsed time is 0 and no keep-alive is due.
    CHECK(!login.ShouldSendKeepAlive());

    // processApplePushServer (0x833a84) and processGetProductInfo (0x8344a0).
    {
        char push[] = "api.push|2195|feed.push|2196|pub.pem|priv.pem|secret";
        login.processApplePushServer(push);
        CHECK(std::strcmp(login.getAPNSIP(), "api.push") == 0);
        CHECK(login.getAPNSPort() == 2195);
        CHECK(login.getAPNSFeedBackPort() == 2196);
        CHECK(std::strcmp(login.getAPNSPrivateKeyPassword(), "secret") == 0);
        char products[] = "7^101^Gold|8^102^Silver";
        login.processGetProductInfo(products);
        CHECK(login.getProductListCount() == 2);
        CHECK(login.getProductGGI(1) == 8);
        CHECK(login.getProductID(0) == 101);
        CHECK(std::strcmp(login.getProductName(0), "Gold") == 0);
    }

    // OnUpdateFailure (0x8342f0) and OnUpdateSuccess (0x834658) need a response cursor, so
    // they are covered through StartResponseParser in glx_player_web_component_test.cpp.

    std::printf("%s\n", failures ? "glx_player_login: FAILED" : "glx_player_login: ok");
    return failures != 0;
}
