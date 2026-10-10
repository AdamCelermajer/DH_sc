#pragma once

namespace dh::foundation {

// An explicit host/QA choice, not a claim about the original driver's policy.
enum class ViewportFitMode { Stretch, Contain };

struct ViewportBounds {
    int x = 0, y = 0, width = 0, height = 0;
};

struct ViewportPoint { double x = 0, y = 0; };

// Top-left host pixel coordinates. Contain chooses a centered integer viewport;
// odd unused pixel counts place the extra pixel on the right/bottom.
// Invalid dimensions or authored aspect return empty bounds.
ViewportBounds fitViewport(int hostWidth, int hostHeight, double authoredAspect,
                           ViewportFitMode mode);

// These map projection-normalized coordinates to the SAME fitted viewport used
// by drawing/picking. NDC (-1,+1) is its top-left edge. Coordinates outside its
// edges remain valid, allowing callers to apply their own clipping policy.
bool normalizedToViewport(ViewportPoint normalized, ViewportBounds viewport,
                          ViewportPoint& pixel);
bool viewportToNormalized(ViewportPoint pixel, ViewportBounds viewport,
                          ViewportPoint& normalized);

} // namespace dh::foundation
