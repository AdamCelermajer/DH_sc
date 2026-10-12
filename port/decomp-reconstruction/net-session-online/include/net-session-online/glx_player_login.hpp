// GLXPlayerLogin: the login and lobby-server service of the GLX player services. It
// holds the push-notification (APNS) settings and the product list, and sends the
// keep-alive. Only the accessors and ShouldSendKeepAlive are reconstructed here. The
// request senders, the response parsers and the ctor/dtor are queued.
//
// Layout (original 32-bit offsets, kept as comments; the GLXPlayerWebComponent base
// occupies +0x00..+0x3b):
//   +0x40 keep-alive enabled (byte)           +0x44 time of the last keep-alive (int)
//   +0x48 APNS server address (char*)         +0x4c APNS port (int)
//   +0x50 APNS feedback address (char*)       +0x54 APNS feedback port (int)
//   +0x58 APNS public PEM path (char*)        +0x5c APNS private key PEM path (char*)
//   +0x60 APNS private key password (char*)
//   +0x64 product count (int)                 +0x68 product GGI array (int)
//   +0x6c product id array (int)              +0x70 product name array (int)
#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "net-session-online/glx_player_web_component.hpp"

class GLXPlayerLogin : public GLXPlayerWebComponent {
public:
    // 0x833374 _ZN14GLXPlayerLogin9getAPNSIPEv
    const char* getAPNSIP() const { return apnsIp_.get(); }
    // 0x83337c _ZN14GLXPlayerLogin11getAPNSPortEv
    std::int32_t getAPNSPort() const { return apnsPort_; }
    // 0x833384 _ZN14GLXPlayerLogin17getAPNSFeedBackIPEv
    const char* getAPNSFeedBackIP() const { return apnsFeedbackIp_.get(); }
    // 0x83338c _ZN14GLXPlayerLogin19getAPNSFeedBackPortEv
    std::int32_t getAPNSFeedBackPort() const { return apnsFeedbackPort_; }
    // 0x833394 _ZN14GLXPlayerLogin20getAPNSPublicPemPathEv
    const char* getAPNSPublicPemPath() const { return apnsPublicPem_.get(); }
    // 0x83339c _ZN14GLXPlayerLogin24getAPNSPrivateKeyPemPathEv
    const char* getAPNSPrivateKeyPemPath() const { return apnsPrivateKeyPem_.get(); }
    // 0x8333a4 _ZN14GLXPlayerLogin25getAPNSPrivateKeyPasswordEv
    const char* getAPNSPrivateKeyPassword() const { return apnsPrivateKeyPassword_.get(); }
    // 0x8333ac _ZN14GLXPlayerLogin19getProductListCountEv
    std::int32_t getProductListCount() const { return productCount_; }
    // 0x8333b4 _ZN14GLXPlayerLogin13getProductGGIEi: default -1.
    std::int32_t getProductGGI(std::int32_t index) const;
    // 0x8333d8 _ZN14GLXPlayerLogin12getProductIDEi: default -1.
    std::int32_t getProductID(std::int32_t index) const;
    // 0x8333fc _ZN14GLXPlayerLogin14getProductNameEi: default 0.
    std::int32_t getProductName(std::int32_t index) const;
    // 0x83341c _ZN14GLXPlayerLogin19ShouldSendKeepAliveEv
    // True when keep-alive is enabled and more than 90000 ms have passed since the last one.
    bool ShouldSendKeepAlive() const;

    // Not in the original: test seams for the fields the getters read.
    void SetAPNSIPForTest(const char* value);
    void SetAPNSPortForTest(std::int32_t value) { apnsPort_ = value; }
    void SetProductsForTest(std::vector<std::int32_t> ggis, std::vector<std::int32_t> ids,
                            std::vector<std::int32_t> names);
    void SetKeepAliveForTest(bool enabled, std::int32_t lastTimeMs) {
        keepAliveEnabled_ = enabled;
        lastKeepAliveMs_ = lastTimeMs;
    }

private:
    bool keepAliveEnabled_ = false;                    // +0x40
    std::int32_t lastKeepAliveMs_ = 0;                 // +0x44
    std::unique_ptr<char[]> apnsIp_;                   // +0x48
    std::int32_t apnsPort_ = 0;                        // +0x4c
    std::unique_ptr<char[]> apnsFeedbackIp_;           // +0x50
    std::int32_t apnsFeedbackPort_ = 0;                // +0x54
    std::unique_ptr<char[]> apnsPublicPem_;            // +0x58
    std::unique_ptr<char[]> apnsPrivateKeyPem_;        // +0x5c
    std::unique_ptr<char[]> apnsPrivateKeyPassword_;   // +0x60
    std::int32_t productCount_ = 0;                    // +0x64
    std::vector<std::int32_t> productGgi_;             // +0x68
    std::vector<std::int32_t> productId_;              // +0x6c
    std::vector<std::int32_t> productName_;            // +0x70
};
