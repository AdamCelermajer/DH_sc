#include "net-session-online/glx_player_login.hpp"

#include <cstring>
#include <utility>

#include "net-session-online/deps/glx_stand_ins.hpp"

namespace {
// Keep-alive interval in ms, from the original's comparison constant.
constexpr std::int32_t kKeepAliveIntervalMs = 90000;

// The original checks only that the array is allocated and the index is not negative.
// The port also stops at the end of the vector, which the original does not check.
bool ElementPresent(const std::vector<std::int32_t>& list, std::int32_t index) {
    return index >= 0 && static_cast<std::size_t>(index) < list.size();
}
}  // namespace

// 0x8333b4 _ZN14GLXPlayerLogin13getProductGGIEi
std::int32_t GLXPlayerLogin::getProductGGI(std::int32_t index) const {
    if (ElementPresent(productGgi_, index)) {
        return productGgi_[static_cast<std::size_t>(index)];
    }
    return -1;
}

// 0x8333d8 _ZN14GLXPlayerLogin12getProductIDEi
std::int32_t GLXPlayerLogin::getProductID(std::int32_t index) const {
    if (ElementPresent(productId_, index)) {
        return productId_[static_cast<std::size_t>(index)];
    }
    return -1;
}

// 0x8333fc _ZN14GLXPlayerLogin14getProductNameEi
std::int32_t GLXPlayerLogin::getProductName(std::int32_t index) const {
    if (ElementPresent(productName_, index)) {
        return productName_[static_cast<std::size_t>(index)];
    }
    return 0;
}

// 0x83341c _ZN14GLXPlayerLogin19ShouldSendKeepAliveEv
bool GLXPlayerLogin::ShouldSendKeepAlive() const {
    if (!keepAliveEnabled_) {
        return false;
    }
    const std::int32_t now = static_cast<std::int32_t>(XP_API_GET_TIME());
    return kKeepAliveIntervalMs < now - lastKeepAliveMs_;
}

void GLXPlayerLogin::SetAPNSIPForTest(const char* value) {
    if (value == nullptr || std::strlen(value) == 0) {
        apnsIp_.reset();
        return;
    }
    apnsIp_.reset(new char[std::strlen(value) + 1]);
    std::strcpy(apnsIp_.get(), value);
}

void GLXPlayerLogin::SetProductsForTest(std::vector<std::int32_t> ggis, std::vector<std::int32_t> ids,
                                        std::vector<std::int32_t> names) {
    productCount_ = static_cast<std::int32_t>(ggis.size());
    productGgi_ = std::move(ggis);
    productId_ = std::move(ids);
    productName_ = std::move(names);
}
