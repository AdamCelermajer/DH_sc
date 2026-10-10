#include "original_art.hpp"
#include <algorithm>
#include <cmath>

namespace dh::foundation::frontend::art {
bool load(const std::filesystem::path& root, std::string& error) {
    std::error_code ec;
    if (!std::filesystem::is_regular_file(root / atlas_relative_path, ec)) {
        error = "Original MenusGraphics_droid atlas missing under " + root.string();
        return false;
    }
    error.clear(); return true;
}
bool geometry(Screen screen, int width, int height, HudGeometry& output, std::string& error) {
    if (width <= 0 || height <= 0) { error = "Invalid frontend viewport"; return false; }
    const float scale = std::min(width / 480.f, height / 320.f);
    const float x = (width - 480.f*scale)*.5f, y = (height - 320.f*scale)*.5f;
    HudGeometry next;
    next.width = static_cast<float>(width); next.height = static_cast<float>(height);
    next.batches = original_art(screen).batches;
    for (auto& batch : next.batches) for (auto& v : batch.triangles) {
        v.x = x + v.x*scale; v.y = y + v.y*scale;
    }
    output = std::move(next); error.clear(); return true;
}
bool contains(const HitRegion& region, float x, float y) noexcept {
    if (!std::isfinite(x)||!std::isfinite(y)) return false;
    for (std::size_t i=0;i+2<region.triangles.size();i+=3) {
        const auto& a=region.triangles[i];const auto& b=region.triangles[i+1];const auto& c=region.triangles[i+2];
        const float area=(b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
        if (std::abs(area)<1e-6f) continue;
        const float e0=(b.x-a.x)*(y-a.y)-(b.y-a.y)*(x-a.x);
        const float e1=(c.x-b.x)*(y-b.y)-(c.y-b.y)*(x-b.x);
        const float e2=(a.x-c.x)*(y-c.y)-(a.y-c.y)*(x-c.x);
        if ((e0>=0&&e1>=0&&e2>=0)||(e0<=0&&e1<=0&&e2<=0)) return true;
    }
    return false;
}
}
