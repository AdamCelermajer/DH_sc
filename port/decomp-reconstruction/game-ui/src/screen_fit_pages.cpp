// Port deviation (UI8, TRACK-B.md decision 15; not in the original): menu pages fill the screen without bars.
//
// The original stretches every Flash movie over the whole surface (scale mode 0). On a surface that is not 16:9 the port keeps the
// movie at its uniform 16:9 scale (MenuFlash2DCamera::Update) and, instead of letterboxing, lays each visible page out on the real
// screen every frame before it is rendered:
//   * a top-level clip of the page that covers the stage (background art, dim layers, full-screen hit areas) is STRETCHED to the
//     surface (non-uniform, about the stage centre) -- stonework and dim layers do not show a stretch, and this is what the original
//     did to the whole page;
//   * on the image pages (the splash) the covering clip is scaled uniformly to COVER the surface and cropped instead;
//   * every other top-level clip keeps its proportions and MOVES toward the edge of the quadrant its bound centre lies in, by the
//     surface margin (top half up, bottom half down, left half left, right half right; a band of +-10% around the stage centre does
//     not move on that axis). The hero-info panel of the main menu ends at the left edge, the button column at the right edge, the
//     back button in the real corner, centred dialogs stay centred, and clips parked outside the stage move further out.
// The matrix this code wrote last is remembered per clip, so a timeline placement or an animation that rewrites the matrix is
// taken as the new base (the same scheme as the HUD anchoring in HUDControls). Everything is inert when ScreenUsesAspectFit() is
// false (the binary's own screen sizes, or the port setting screenfit = 0) and on a 16:9 surface (margins 0).
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "dh2/MenuFX.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/MultiMenuManager.hpp"
#include "dh2/global/screen_size.hpp"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_sprite.h"

namespace {

struct PageEntry {
    gameswf::character* character;  // identity only; never dereferenced unless seen this frame
    const MenuFX* movie;            // the movie whose pass owns the entry (each pass only forgets its own entries)
    bool active;                    // `written` is what the character carries now
    bool seen;
    gameswf::matrix base, written;
};
constexpr int kMaxEntries = 768;
PageEntry g_entries[kMaxEntries];
int g_entryCount = 0;

const MenuFX* g_currentMovie = nullptr;

PageEntry* FindEntry(gameswf::character* c) {
    for (int i = 0; i < g_entryCount; ++i) {
        if (g_entries[i].character == c) {
            return &g_entries[i];
        }
    }
    if (g_entryCount >= kMaxEntries) {
        return nullptr;
    }
    PageEntry& e = g_entries[g_entryCount++];
    e.character = c;
    e.movie = g_currentMovie;
    e.active = false;
    e.seen = false;
    return &e;
}

struct PageFrame {
    float marginX, marginY;        // surface margin beyond the stage, twips, each side (0 on a 16:9 surface)
    float stageMinX, stageMinY, stageW, stageH;
    float centreX, centreY, bandX, bandY;
};

enum PlaceMode { kMove = 0, kStretch = 1, kCover = 2, kLeave = 3 };

// Lays out one top-level clip of a page. `release` puts the clip's own matrix back.
void PlaceClip(gameswf::character* c, const PageFrame& f, PlaceMode mode, bool release, float refX, float refY, bool parkedX, bool parkedY) {
    if (c == nullptr || !c->is(1) || c->get_parent() == nullptr) {
        return;
    }
    PageEntry* e = FindEntry(c);
    if (e == nullptr) {
        return;
    }
    e->seen = true;
    const gameswf::matrix current = c->get_matrix();
    const bool ours = e->active && std::memcmp(&current, &e->written, sizeof current) == 0;
    if (!ours) {
        e->base = current;
    }
    if (release || mode == kLeave || (f.marginX == 0.0f && f.marginY == 0.0f)) {
        if (ours) {
            c->set_matrix(e->base);
        }
        e->active = false;
        return;
    }
    if (ours) {
        c->set_matrix(e->base);  // measure under the base matrix
    }
    gameswf::rect bound;
    c->get_bound(&bound);  // parent space
    const float bw = static_cast<float>(bound.m_x_max) - static_cast<float>(bound.m_x_min);
    const float bh = static_cast<float>(bound.m_y_max) - static_cast<float>(bound.m_y_min);
    if (bw <= 0.0f || bh <= 0.0f) {
        e->active = false;
        return;
    }
    // Parent space <-> stage space: the page clip is normally placed at the identity; the general case goes through its world matrix.
    const gameswf::matrix* pw = c->get_parent()->get_world_matrix();
    gameswf::point bcParent(static_cast<float>(bound.m_x_min) + bw * 0.5f, static_cast<float>(bound.m_y_min) + bh * 0.5f);
    gameswf::point bcStage;
    pw->transform(&bcStage, bcParent);
    gameswf::matrix m = e->base;
    if (mode == kMove) {
        // Positions are spread from the stage centre by the same factor the background is stretched by, sizes stay: a clip at the
        // stage edge ends at the surface edge, a centred clip stays centred, a clip parked outside the stage moves further out.
        const float fx = (f.stageW + 2.0f * f.marginX) / f.stageW;
        const float fy = (f.stageH + 2.0f * f.marginY) / f.stageH;
        float dx = (refX - f.centreX) * (fx - 1.0f);
        float dy = (refY - f.centreY) * (fy - 1.0f);
        // A group parked entirely outside the stage on an axis (dialogs and buttons the page hides there) is pushed by the whole margin,
        // so it stays outside the enlarged surface too.
        if (parkedX) {
            dx = refX < f.centreX ? -f.marginX : f.marginX;
        }
        if (parkedY) {
            dy = refY < f.centreY ? -f.marginY : f.marginY;
        }
        if (dx == 0.0f && dy == 0.0f) {
            e->active = false;
            return;
        }
        // Stage delta -> parent delta through the inverse of the parent's linear part.
        const float a = pw->m_[0][0], b = pw->m_[0][1], cc = pw->m_[1][0], d = pw->m_[1][1];
        const float det = a * d - b * cc;
        if (det == 0.0f) {
            e->active = false;
            return;
        }
        m.m_[0][2] = e->base.m_[0][2] + (d * dx - b * dy) / det;
        m.m_[1][2] = e->base.m_[1][2] + (a * dy - cc * dx) / det;
    } else {
        // Stretch (or cover) the clip so that its bound spans the surface, about the stage centre. Bound and margins are compared in
        // stage units; the parent is assumed to carry no rotation (true for the menu pages).
        const float sx = pw->m_[0][0] != 0.0f ? static_cast<float>(pw->m_[0][0]) : 1.0f;
        const float sy = pw->m_[1][1] != 0.0f ? static_cast<float>(pw->m_[1][1]) : 1.0f;
        const float bwStage = bw * sx, bhStage = bh * sy;
        float kx = (f.stageW + 2.0f * f.marginX) / bwStage;
        float ky = (f.stageH + 2.0f * f.marginY) / bhStage;
        if (mode == kCover) {
            const float k = kx > ky ? kx : ky;
            kx = ky = k;
        }
        if (kx < 1.0f) kx = 1.0f;  // never shrink: a clip that already overflows stays
        if (ky < 1.0f) ky = 1.0f;
        // Centre of the surface in parent space.
        gameswf::point centreParent;
        pw->transform_by_inverse(&centreParent, gameswf::point(f.centreX, f.centreY));
        m.m_[0][0] = e->base.m_[0][0] * kx;
        m.m_[0][1] = e->base.m_[0][1] * kx;
        m.m_[1][0] = e->base.m_[1][0] * ky;
        m.m_[1][1] = e->base.m_[1][1] * ky;
        m.m_[0][2] = centreParent.m_x + kx * (e->base.m_[0][2] - bcParent.m_x);
        m.m_[1][2] = centreParent.m_y + ky * (e->base.m_[1][2] - bcParent.m_y);
    }
    c->set_matrix(m);
    e->written = c->get_matrix();
    e->active = true;
}

bool IsImagePage(const char* name) {
    return name != nullptr && std::strcmp(name, "menu_splash") == 0;
}

struct ChildInfo {
    gameswf::character* c;
    PlaceMode mode;
    float x0, y0, x1, y1;  // stage-space bound under the base matrix
    float ox, oy;          // stage-space origin (the base translation): stable while a clip animates (a loading bar grows)
    int group;
};

int GroupRoot(ChildInfo* items, int i) {
    while (items[i].group != i) {
        items[i].group = items[items[i].group].group;
        i = items[i].group;
    }
    return i;
}

void LayoutPage(MenuFX::State* state, const PageFrame& f, bool release) {
    if (state == nullptr) {
        return;
    }
    gameswf::character* page = state->GetCharacter();
    if (page == nullptr || !page->is(2)) {
        return;
    }
    const bool image = IsImagePage(state->m_name);
    auto* sprite = static_cast<gameswf::sprite_instance*>(page);
    const gameswf::matrix* pw = page->get_world_matrix();
    const float psx = pw->m_[0][0] != 0.0f ? static_cast<float>(pw->m_[0][0]) : 1.0f;
    const float psy = pw->m_[1][1] != 0.0f ? static_cast<float>(pw->m_[1][1]) : 1.0f;
    constexpr int kMaxChildren = 192;
    ChildInfo items[kMaxChildren];
    int n = 0;
    // Pass 1: classify every clip under its own (base) matrix. A container far larger than the stage (a holder of parked dialogs plus a
    // full-screen blocker) is opened and its children are laid out one by one, two levels deep.
    struct Collector {
        ChildInfo* items;
        int& n;
        const PageFrame& f;
        bool image;
        void run(gameswf::sprite_instance* parent, int depth) {
            const gameswf::matrix* pw = parent->get_world_matrix();
            for (int i = 0; i < parent->m_display_list.size() && n < kMaxChildren; ++i) {
                gameswf::character* child = parent->m_display_list.get_character(i);
                if (child == nullptr || !child->is(1)) {
                    continue;
                }
                PageEntry* e = FindEntry(child);
                const gameswf::matrix current = child->get_matrix();
                const bool ours = e != nullptr && e->active && std::memcmp(&current, &e->written, sizeof current) == 0;
                if (ours) {
                    child->set_matrix(e->base);
                }
                gameswf::rect bound;
                child->get_bound(&bound);
                if (ours) {
                    child->set_matrix(e->written);
                }
                gameswf::point a, b;
                pw->transform(&a, gameswf::point(bound.m_x_min, bound.m_y_min));
                pw->transform(&b, gameswf::point(bound.m_x_max, bound.m_y_max));
                const float x0 = a.m_x < b.m_x ? a.m_x : b.m_x, x1 = a.m_x < b.m_x ? b.m_x : a.m_x;
                const float y0 = a.m_y < b.m_y ? a.m_y : b.m_y, y1 = a.m_y < b.m_y ? b.m_y : a.m_y;
                const float bw = x1 - x0, bh = y1 - y0;
                const float bcx = (x0 + x1) * 0.5f, bcy = (y0 + y1) * 0.5f;
                const bool centred = std::fabs(bcx - f.centreX) <= 0.15f * f.stageW && std::fabs(bcy - f.centreY) <= 0.15f * f.stageH;
                PlaceMode mode = kMove;
                const bool covering = bw >= 0.85f * f.stageW && bh >= 0.85f * f.stageH && bw <= 1.6f * f.stageW && bh <= 1.6f * f.stageH && centred;
                // A covering clip that is itself a layout (two or more sprites inside: a bar frame, an animation, a panel) is opened, not
                // stretched; a plain art layer (shapes, one bitmap) is stretched.
                int innerSprites = 0;
                if (covering && child->is(2) && depth < 2) {
                    auto* inner = static_cast<gameswf::sprite_instance*>(child);
                    for (int k = 0; k < inner->m_display_list.size(); ++k) {
                        gameswf::character* g = inner->m_display_list.get_character(k);
                        if (g != nullptr && g->is(2)) {
                            ++innerSprites;
                        }
                    }
                }
                if (covering && innerSprites < 2) {
                    mode = image ? kCover : kStretch;
                } else if (((bw > 1.2f * f.stageW || bh > 1.2f * f.stageH) || covering) && child->is(2) && depth < 2) {
                    if (e != nullptr && e->active) {  // the container itself stays as authored
                        child->set_matrix(e->base);
                        e->active = false;
                    }
                    if (e != nullptr) {
                        e->seen = true;
                    }
                    run(static_cast<gameswf::sprite_instance*>(child), depth + 1);
                    continue;
                } else if (bw > 1.2f * f.stageW || bh > 1.2f * f.stageH) {
                    mode = kLeave;
                }
                ChildInfo& it = items[n];
                it.c = child;
                it.x0 = x0;
                it.x1 = x1;
                it.y0 = y0;
                it.y1 = y1;
                it.ox = bcx;
                it.oy = bcy;
                it.mode = mode;
                it.group = n;
                ++n;
            }
        }
    };
    Collector collector{items, n, f, image};
    collector.run(sprite, 0);
    // Pass 2: clips whose bounds touch (within 1.5% of the stage) move as one group, so a text stays on its strip and a button
    // column stays a column. Backgrounds are not grouped.
    const float gapX = f.stageW * 0.015f, gapY = f.stageH * 0.015f;
    // Not grouped (they move on their own): backgrounds, hidden clips, containers larger than the stage (dialog holders that span
    // everything) and clips parked outside the stage.
    auto groupable = [&](const ChildInfo& it) {
        if (it.mode != kMove || !it.c->m_visible) {
            return false;
        }
        const float bw = it.x1 - it.x0, bh = it.y1 - it.y0;
        if (bw > 1.2f * f.stageW || bh > 1.2f * f.stageH) {
            return false;
        }
        const bool parked = it.x1 <= f.stageMinX || it.x0 >= f.stageMinX + f.stageW || it.y1 <= f.stageMinY || it.y0 >= f.stageMinY + f.stageH;
        return !parked;
    };
    for (int i = 0; i < n; ++i) {
        if (!groupable(items[i])) {
            continue;
        }
        for (int j = i + 1; j < n; ++j) {
            if (!groupable(items[j])) {
                continue;
            }
            const bool overlap = items[i].x0 - gapX <= items[j].x1 && items[j].x0 - gapX <= items[i].x1 &&
                                 items[i].y0 - gapY <= items[j].y1 && items[j].y0 - gapY <= items[i].y1;
            if (overlap) {
                const int ri = GroupRoot(items, i), rj = GroupRoot(items, j);
                if (ri != rj) {
                    items[rj].group = ri;
                }
            }
        }
    }
    float gx0[kMaxChildren], gy0[kMaxChildren], gx1[kMaxChildren], gy1[kMaxChildren];
    for (int i = 0; i < n; ++i) {
        gx0[i] = items[i].x0;
        gy0[i] = items[i].y0;
        gx1[i] = items[i].x1;
        gy1[i] = items[i].y1;
    }
    for (int i = 0; i < n; ++i) {
        const int r = GroupRoot(items, i);
        if (r != i) {
            gx0[r] = gx0[r] < items[i].x0 ? gx0[r] : items[i].x0;
            gy0[r] = gy0[r] < items[i].y0 ? gy0[r] : items[i].y0;
            gx1[r] = gx1[r] > items[i].x1 ? gx1[r] : items[i].x1;
            gy1[r] = gy1[r] > items[i].y1 ? gy1[r] : items[i].y1;
        }
    }
    // Pass 3: place. The reference is the group's bound centre (a clip's origin is arbitrary: the loading bar's sits 150 px above its art).
    for (int i = 0; i < n; ++i) {
        const int r = GroupRoot(items, i);
        const float refX = (gx0[r] + gx1[r]) * 0.5f;
        const float refY = (gy0[r] + gy1[r]) * 0.5f;
        const char* dbgPage = std::getenv("DH2_FIT_DEBUG");
        if (dbgPage != nullptr && (std::strcmp(dbgPage, "1") == 0 || std::strncmp(dbgPage, state->m_name, std::strlen(dbgPage)) == 0)) {
            static int dbg = 0;
            if (dbg++ < 3000) {
                std::fprintf(stderr, "FITDBG page %s child %d '%s' vis=%d bound=%.0f,%.0f..%.0f,%.0f origin=%.0f,%.0f mode=%d group=%d ref=%.0f,%.0f%c",
                             state->m_name, i, items[i].c->get_name().c_str(), items[i].c->m_visible ? 1 : 0, items[i].x0 / 20.0f,
                             items[i].y0 / 20.0f, items[i].x1 / 20.0f, items[i].y1 / 20.0f, items[i].ox / 20.0f, items[i].oy / 20.0f,
                             static_cast<int>(items[i].mode), r, refX / 20.0f, refY / 20.0f, 10);
            }
        }
        // Parked = the clip's OWN bound lies outside the stage on that axis (a group may also hold an ornament that reaches into it).
        const bool parkedX = items[i].x1 <= f.stageMinX || items[i].x0 >= f.stageMinX + f.stageW;
        const bool parkedY = items[i].y1 <= f.stageMinY || items[i].y0 >= f.stageMinY + f.stageH;
        PlaceClip(items[i].c, f, items[i].mode, release, refX, refY, parkedX, parkedY);
    }
}

bool FrameFor(MenuFX* movie, PageFrame& f) {
    gameswf::root* root = movie != nullptr ? movie->GetRoot() : nullptr;
    if (root == nullptr || root->m_def.get_ptr() == nullptr) {
        return false;
    }
    const gameswf::rect& stage = root->m_def->m_frame_size;
    f.stageMinX = static_cast<float>(stage.m_x_min);
    f.stageMinY = static_cast<float>(stage.m_y_min);
    f.stageW = static_cast<float>(stage.m_x_max) - f.stageMinX;
    f.stageH = static_cast<float>(stage.m_y_max) - f.stageMinY;
    if (f.stageW <= 0.0f || f.stageH <= 0.0f) {
        return false;
    }
    const int boxW = FlashBoxWidth(root), boxH = FlashBoxHeight(root);
    const int offX = (Width_Screen - boxW) / 2, offY = (Height_Screen - boxH) / 2;
    f.marginX = boxW > 0 ? static_cast<float>(offX) * f.stageW / static_cast<float>(boxW) : 0.0f;
    f.marginY = boxH > 0 ? static_cast<float>(offY) * f.stageH / static_cast<float>(boxH) : 0.0f;
    if (f.marginX < 0.0f) f.marginX = 0.0f;
    if (f.marginY < 0.0f) f.marginY = 0.0f;
    f.centreX = f.stageMinX + f.stageW * 0.5f;
    f.centreY = f.stageMinY + f.stageH * 0.5f;
    f.bandX = f.stageW * 0.10f;
    f.bandY = f.stageH * 0.10f;
    return true;
}

}  // namespace

// Called by MenuManager::Draw for every movie after its camera update and before Render. `hudMovie`: the slot-3 movie, whose
// HUD states are laid out by HudAnchorUpdate; only its other pages (pause, customise dialogs) are handled here.
void MenuPageLayoutUpdate(MenuFX* movie, bool hudMovie) {
    if (movie == nullptr) {
        return;
    }
    g_currentMovie = movie;
    for (int i = 0; i < g_entryCount; ++i) {
        if (g_entries[i].movie == movie) {
            g_entries[i].seen = false;
        }
    }
    PageFrame f;
    const bool release = !ScreenUsesAspectFit() || !FrameFor(movie, f);
    if (release && g_entryCount == 0) {
        return;
    }
    MenuFX::StateArray& stack = movie->m_stack;
    for (int s = 0; s < stack.m_size; ++s) {
        MenuFX::State* state = stack.m_data[s];
        if (state == nullptr) {
            continue;
        }
        if (hudMovie && (std::strncmp(state->m_name, "menu_HUD_", 9) == 0 || std::strcmp(state->m_name, "menu_DebugHUD") == 0)) {
            continue;
        }
        LayoutPage(state, f, release);
    }
    // Also the current state when it is not on the stack (the splash and loading pages are set, not pushed).
    MenuFX::State* current = movie->GetCurrentState();
    bool onStack = false;
    for (int s = 0; s < stack.m_size; ++s) {
        onStack = onStack || stack.m_data[s] == current;
    }
    if (current != nullptr && !onStack &&
        !(hudMovie && (std::strncmp(current->m_name, "menu_HUD_", 9) == 0 || std::strcmp(current->m_name, "menu_DebugHUD") == 0))) {
        LayoutPage(current, f, release);
    }
    int kept = 0;
    for (int i = 0; i < g_entryCount; ++i) {
        if (g_entries[i].movie != movie || g_entries[i].seen) {  // only this movie's unseen entries are forgotten
            g_entries[kept++] = g_entries[i];
        }
    }
    g_entryCount = kept;
}
