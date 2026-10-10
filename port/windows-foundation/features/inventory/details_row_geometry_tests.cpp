// P14 EQUIP / B045: Details list rows must be hittable across their visible body, and adjacent rows must not leave
// gaps or overlaps. The authored row hit was a 3.3-unit border sliver; the visible row is the selected/unselected art.
#include "inventory_details.hpp"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

using namespace dh::foundation;
using namespace dh::foundation::inventory;

namespace {
int failures = 0;
void check(bool ok, const std::string& text) {
    if (!ok) { std::cerr << "FAIL: " << text << '\n'; ++failures; }
}
struct Box { float x0, x1, y0, y1; bool valid; };
Box visible_box(const DetailRowArt& row) {
    Box b{0, 0, 0, 0, false};
    const auto grow = [&](const std::vector<HudGeometryVertex>& v) {
        for (const auto& p : v) {
            if (!b.valid) { b = {p.x, p.x, p.y, p.y, true}; continue; }
            b.x0 = std::min(b.x0, p.x); b.x1 = std::max(b.x1, p.x);
            b.y0 = std::min(b.y0, p.y); b.y1 = std::max(b.y1, p.y);
        }
    };
    for (const auto& batch : row.unselected.batches) grow(batch.triangles);
    for (const auto& batch : row.selected.batches) grow(batch.triangles);
    return b;
}
const DetailRowArt* find_row(int relative) {
    for (const auto& row : original_inventory_details().rows)
        if (row.relative_index == relative) return &row;
    return nullptr;
}
}  // namespace

int main() {
    const auto* r0 = find_row(0);
    const auto* r1 = find_row(1);
    const auto* rm1 = find_row(-1);
    check(r0 && r1 && rm1, "Details list rows -1, 0 and 1 must exist in the authored art");
    if (!r0 || !r1 || !rm1) { std::cerr << failures << " failure(s)\n"; return 1; }

    const auto b0 = visible_box(*r0), b1 = visible_box(*r1), bm1 = visible_box(*rm1);
    check(b0.valid && b1.valid && bm1.valid, "row visible art missing");
    // The body is far taller than the 3.3-unit border sliver the authored hit used to cover.
    check(b0.y1 - b0.y0 >= 20.0f && b1.y1 - b1.y0 >= 20.0f, "row hit must cover the visible row body, not only the border");
    // Adjacent rows abut: no gap (a click there would fall through) and no overlap (ambiguous selection).
    check(std::fabs(b1.y0 - b0.y1) < 1.0f && b1.y0 >= b0.y1 - 1e-3f, "row +1 must start where row 0 ends");
    check(std::fabs(bm1.y1 - b0.y0) < 1.0f && bm1.y1 <= b0.y0 + 1e-3f, "row -1 must end where row 0 starts");

    const float cx = (b0.x0 + b0.x1) * 0.5f;
    const auto mid = [](const Box& b) { return (b.y0 + b.y1) * 0.5f; };
    check(details_row_hit(*r0, cx, mid(b0)), "centre of row 0 must select row 0");
    check(details_row_hit(*r1, cx, mid(b1)), "centre of the second row (+1) must select row +1");
    check(details_row_hit(*rm1, cx, mid(bm1)), "centre of row -1 must select row -1");
    check(!details_row_hit(*r0, cx, mid(b1)), "row 0 must not claim the second row's centre");
    check(!details_row_hit(*r1, cx, mid(b0)), "row +1 must not claim the first row's centre");
    check(details_row_hit(*r1, cx, b1.y0 + 0.5f) && !details_row_hit(*r0, cx, b1.y0 + 0.5f), "boundary belongs to the next row only");
    check(!details_row_hit(*r1, b1.x0 - 20.0f, mid(b1)) && !details_row_hit(*r1, b1.x1 + 20.0f, mid(b1)), "row hit leaked outside its width");
    check(!details_row_hit(*r1, cx, b1.y1 + 15.0f), "row hit leaked below the row");

    if (failures) { std::cerr << failures << " failure(s)\n"; return 1; }
    std::cout << "inventory_details_row_geometry PASS (row +1 y=[" << b1.y0 << "," << b1.y1 << "] x=[" << b1.x0 << "," << b1.x1 << "])\n";
    return 0;
}
