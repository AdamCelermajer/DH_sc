#pragma once

#include <functional>
#include <utility>
#include <vector>

namespace dh::foundation::frontend {

// Owns caller-context cleanup actions and runs them in reverse acquisition
// order on every exit, including exceptions from presentation/update code.
class FrontendCleanupStackV1 final {
public:
    FrontendCleanupStackV1() = default;
    FrontendCleanupStackV1(const FrontendCleanupStackV1&) = delete;
    FrontendCleanupStackV1& operator=(const FrontendCleanupStackV1&) = delete;

    template<class Cleanup>
    void own(Cleanup&& cleanup) {
        cleanup_.emplace_back(std::forward<Cleanup>(cleanup));
    }

    ~FrontendCleanupStackV1() noexcept {
        for (auto it = cleanup_.rbegin(); it != cleanup_.rend(); ++it) {
            try { (*it)(); } catch (...) {}
        }
    }

private:
    std::vector<std::function<void()>> cleanup_;
};

} // namespace dh::foundation::frontend
