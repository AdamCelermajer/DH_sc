#include "item_instance.hpp"

#include <cstring>

namespace dh2::data {

std::int32_t ItemInstanceV1::signed_quantity() const noexcept {
    std::int16_t value;
    std::memcpy(&value, &quantity, sizeof(value));
    return value;
}

}
