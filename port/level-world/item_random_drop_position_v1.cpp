#include "item_random_drop_position_v1.hpp"

#include <cmath>

namespace dh2::item_random_drop_position_v1 {
namespace {

bool draw(data::InventoryRandomServiceV4 random, std::int32_t bound,
          std::int32_t& value, std::string& error) {
    if (!random.next || !random.next(random.context, bound, 0, value, error)) {
        if (error.empty()) error = "Source Random::GetRandom service failed";
        return false;
    }
    if (value < 0 || value >= bound) {
        error = "Source Random::GetRandom service returned an out-of-range value";
        return false;
    }
    return true;
}

}  // namespace

bool generate(const Point& center, const Point* killer,
              data::InventoryRandomServiceV4 random, Point* output,
              std::string& error) {
    if (!output || !random.next) {
        error = "ItemObject::_GetRandomDropPos requires output and process RNG";
        return false;
    }

    if (!killer) {
        std::int32_t x_roll = 0;
        std::int32_t y_roll = 0;
        if (!draw(random, 500, x_roll, error) ||
            !draw(random, 500, y_roll, error)) return false;
        volatile float x_offset = static_cast<float>(x_roll - 250);
        volatile float x = x_offset + center.x;
        volatile float y_offset = static_cast<float>(y_roll - 250);
        volatile float y = y_offset + center.y;
        *output = {x, y, center.z};
        error.clear();
        return true;
    }

    // Match Point3D::normalize's squared-length and division order. In
    // particular, the original divides by zero when killer and center match;
    // IEEE NaNs then flow into the returned point after both RNG draws.
    volatile float dx = killer->x - center.x;
    volatile float dy = killer->y - center.y;
    volatile float dz = killer->z - center.z;
    volatile float xx = dx * dx;
    volatile float yy = dy * dy;
    volatile float xy = xx + yy;
    volatile float zz = dz * dz;
    volatile float squared = xy + zz;
    volatile float length = std::sqrt(static_cast<float>(squared));
    volatile float nx = dx / length;
    volatile float ny = dy / length;
    volatile float nz = dz / length;

    std::int32_t distance_roll = 0;
    std::int32_t lateral_roll = 0;
    if (!draw(random, 200, distance_roll, error) ||
        !draw(random, 300, lateral_roll, error)) return false;

    volatile float forward = static_cast<float>(distance_roll + 150);
    volatile float lateral = static_cast<float>(lateral_roll - 150);
    volatile float fx = nx * forward;
    volatile float fy = ny * forward;
    volatile float fz = nz * forward;

    // Vec3f_K=(0,0,1), so the original lateral basis is direction x K,
    // not K x direction. Keep the multiply/subtract sequence from 0x3ec79c.
    volatile float side_x = (ny * 1.0f) - (nz * 0.0f);
    volatile float side_y = (nz * 0.0f) - (nx * 1.0f);
    volatile float side_z = (nx * 0.0f) - (ny * 0.0f);
    volatile float x_spread = lateral * side_x;
    volatile float x_forward = x_spread + fx;
    volatile float x = x_forward + center.x;
    volatile float y_spread = lateral * side_y;
    volatile float y_forward = y_spread + fy;
    volatile float y = y_forward + center.y;
    volatile float z_spread = lateral * side_z;
    volatile float z_forward = z_spread + fz;
    volatile float z = z_forward + center.z;
    *output = {x, y, z};
    error.clear();
    return true;
}

}  // namespace dh2::item_random_drop_position_v1
