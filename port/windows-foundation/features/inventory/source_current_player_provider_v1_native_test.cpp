#include "source_current_player_provider_v1.hpp"

using namespace dh::foundation::inventory;
using namespace dh2::data;

namespace dh::foundation::actor_frame {
bool SourceCurrentLevelBackendV1::current(SourceCurrentLevelBorrowV1&,
                                           std::string& error) const {
    error = "test backend must not run without a retained Application";
    return false;
}
}

int main() {
    std::string error;
    SourceFixedLootCurrentLevelProviderV1 missing({}, {}, error);
    if (error.empty() || missing.ready()) return 1;

    OwnedInventoryResponseV4 response{};
    response.value = 73;
    const OwnedInventoryRequestV4 count{
        OwnedInventoryOperationV4::player_count, 0x4043a8, nullptr, nullptr, 0, 0};
    if (missing.query(count, response, error) || error.empty() || response.value != 73) return 2;

    return 0;
}
