// GLXPlayerObserver: callback interface of the GLX player services. The vtable
// (_ZTV17GLXPlayerObserver, 28 bytes) holds seven slots; only slot 0 has a name in
// the dump. The other slots are named here after their call sites in the web
// component. Those names are inferred and are marked as such.
//
// The original has no virtual destructor. One is added here so the observer can
// be owned through a base pointer; it is not counted in the slot numbers below.
#pragma once

#include <cstdint>

class GLXPlayerObserver {
public:
    virtual ~GLXPlayerObserver() = default;

    // 0x52df0c _ZN17GLXPlayerObserver15OnDownloadErrorEi (slot 0). Empty in the original.
    virtual void OnDownloadError(std::int32_t code) {}
    // Slot 1, called as observer->slot1(requestId) when a request times out.
    // Name inferred from the call site in GLXPlayerWebComponent::Update.
    virtual void OnRequestTimeout(std::int32_t requestId) {}
    // Slot 2, called as observer->slot2(arg, responseText, responseLength) after a
    // successful response. Name inferred from GLXPlayerWebComponent::OnUpdateSuccess.
    virtual void OnResponseText(std::int32_t arg, const char* text, std::int32_t length) {}
    // Slot 3, called as observer->slot3(code, nextIntToken) on a failed response.
    // Name inferred from GLXPlayerWebComponent::OnUpdateFailure.
    virtual void OnResponseError(std::int32_t code, std::int32_t detail) {}
};
