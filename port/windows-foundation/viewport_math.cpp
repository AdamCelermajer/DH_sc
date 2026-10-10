#include "viewport_math.hpp"

#include <algorithm>
#include <cmath>

namespace dh::foundation {

ViewportBounds fitViewport(int hostWidth, int hostHeight, double authoredAspect,
                           ViewportFitMode mode) {
    if (hostWidth <= 0 || hostHeight <= 0 || !std::isfinite(authoredAspect) || authoredAspect <= 0)
        return {};
    if (mode == ViewportFitMode::Stretch) return {0,0,hostWidth,hostHeight};
    int width=hostWidth, height=hostHeight;
    if (static_cast<double>(hostWidth)/hostHeight > authoredAspect) {
        width=static_cast<int>(std::clamp(std::round(hostHeight*authoredAspect),1.0,static_cast<double>(hostWidth)));
    } else {
        height=static_cast<int>(std::clamp(std::round(hostWidth/authoredAspect),1.0,static_cast<double>(hostHeight)));
    }
    return {(hostWidth-width)/2,(hostHeight-height)/2,width,height};
}

bool normalizedToViewport(ViewportPoint normalized, ViewportBounds viewport,
                          ViewportPoint& pixel) {
    if (viewport.width <= 0 || viewport.height <= 0 || !std::isfinite(normalized.x) || !std::isfinite(normalized.y))
        return false;
    const ViewportPoint result{viewport.x+(normalized.x+1.0)*0.5*viewport.width,
                               viewport.y+(1.0-normalized.y)*0.5*viewport.height};
    if (!std::isfinite(result.x) || !std::isfinite(result.y)) return false;
    pixel=result;
    return true;
}

bool viewportToNormalized(ViewportPoint pixel, ViewportBounds viewport,
                          ViewportPoint& normalized) {
    if (viewport.width <= 0 || viewport.height <= 0 || !std::isfinite(pixel.x) || !std::isfinite(pixel.y))
        return false;
    const ViewportPoint result{2.0*(pixel.x-viewport.x)/viewport.width-1.0,
                               1.0-2.0*(pixel.y-viewport.y)/viewport.height};
    if (!std::isfinite(result.x) || !std::isfinite(result.y)) return false;
    normalized=result;
    return true;
}

} // namespace dh::foundation
