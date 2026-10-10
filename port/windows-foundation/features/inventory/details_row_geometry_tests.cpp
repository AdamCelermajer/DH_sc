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

    // B046: the Details slot rail. Every icon (SideList/btn_Type0..9, InvSlotId 0..9) and both arrows must hit only
    // their own control, in order from top to bottom, with no overlap with each other or with the arrows.
    const auto& details = original_inventory_details();
    DetailRailBox icon[10]{};
    for (unsigned slot = 0; slot < 10; ++slot) {
        const std::string label = "rail icon " + std::to_string(slot);
        check(details_rail_box(details, slot, icon[slot]), label + " has no authored art");
        check(icon[slot].x1 - icon[slot].x0 >= 10.0f && icon[slot].y1 - icon[slot].y0 >= 10.0f, label + " hit box is too small");
        check(icon[slot].x0 >= 0.0f && icon[slot].x1 <= 30.0f, label + " is outside the rail column");
        const float cx = (icon[slot].x0 + icon[slot].x1) * 0.5f, cy = (icon[slot].y0 + icon[slot].y1) * 0.5f;
        check(details_rail_slot_at(details, cx, cy) == int(slot), "centre of " + label + " must select that slot");
        check(details_rail_slot_at(details, icon[slot].x0 + 0.5f, icon[slot].y0 + 0.5f) == int(slot) &&
              details_rail_slot_at(details, icon[slot].x1 - 0.5f, icon[slot].y1 - 0.5f) == int(slot),
              "corners of " + label + " must stay on that icon");
        check(details_rail_slot_at(details, 100.0f, cy) == -1, "a point right of the rail must not select an icon");
        if (slot > 0) check(icon[slot - 1].y1 <= icon[slot].y0 + 1e-3f, label + " overlaps the icon above it");
    }
    const auto arrow_centre = [&](DetailAction action, float& x, float& y) {
        for (const auto& hit : details.actions) {
            if (hit.action != action || hit.triangles.size() < 3) continue;
            x = (hit.triangles[0].x + hit.triangles[1].x + hit.triangles[2].x) / 3;
            y = (hit.triangles[0].y + hit.triangles[1].y + hit.triangles[2].y) / 3;
            return true;
        }
        return false;
    };
    float up_x = 0, up_y = 0, down_x = 0, down_y = 0;
    check(arrow_centre(DetailAction::previous, up_x, up_y), "up arrow (btn_left) hit contour missing");
    check(arrow_centre(DetailAction::next, down_x, down_y), "down arrow (btn_right) hit contour missing");
    check(details_rail_slot_at(details, up_x, up_y) == -1, "up arrow centre must not select a rail icon");
    check(details_rail_slot_at(details, down_x, down_y) == -1, "down arrow centre must not select a rail icon");
    check(up_y < icon[0].y0 && down_y > icon[9].y1, "arrows must sit above icon 0 and below icon 9");
    check(up_x >= 0.0f && up_x <= 30.0f && down_x >= 0.0f && down_x <= 30.0f, "arrows must sit in the rail column");
    check(details_rail_slot_at(details, icon[0].x0 + 1.0f, icon[0].y0 - 2.0f) == -1 &&
          details_rail_slot_at(details, icon[9].x0 + 1.0f, icon[9].y1 + 2.0f) == -1,
          "the gap between the arrows and the rail icons must not select an icon");

    if (failures) { std::cerr << failures << " failure(s)\n"; return 1; }
    std::cout << "inventory_details_row_geometry PASS (row +1 y=[" << b1.y0 << "," << b1.y1 << "] x=[" << b1.x0 << "," << b1.x1 << "])\n";
    return 0;
}
