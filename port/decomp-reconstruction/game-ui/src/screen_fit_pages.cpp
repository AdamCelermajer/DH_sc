// Port deviation (UI8/UI10, TRACK-B.md decision 15; not in the original): menu pages fill the screen without bars.
//
// The original stretches every Flash movie over the whole surface (scale mode 0). On a surface that is not 16:9 the port keeps the
// movie at its uniform 16:9 scale (MenuFlash2DCamera::Update) and lays each visible page out on the real screen every frame:
//   * the PAGE clip is stretched by the surface factor (fx, fy) about the stage centre, exactly as the original would stretch it:
//     backgrounds, columns and dividers reach the edges and stay aligned with each other;
//   * every SMALL clip inside it (a bound under 75% of the stage both ways: buttons, texts, icons, frames, panels) is scaled back by
//     (1/fx, 1/fy) about its own centre, so it keeps its proportions where the stretch put it; a small clip whose bound lies inside a
//     sibling's bound (a text on its strip, a value in its row) keeps its original offset from that sibling instead;
//   * on an image page (the splash) the page is scaled uniformly to COVER the surface and cropped.
// The matrix this code wrote last is remembered per clip, so a timeline placement or an animation that rewrites the matrix is taken
// as the new base (the same scheme as the HUD anchoring in HUDControls). Everything is inert when ScreenUsesAspectFit() is false
// (the binary's own screen sizes, or the port setting screenfit = 0) and on a 16:9 surface (factors 1).
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
constexpr int kMaxEntries = 1024;
PageEntry g_entries[kMaxEntries];
int g_entryCount = 0;
const MenuFX* g_currentMovie = nullptr;
const char* g_dbgPage = "";
bool DbgOn() {
    const char* e = std::getenv("DH2_FIT_DEBUG");
    return e != nullptr && (std::strcmp(e, "1") == 0 || std::strcmp(e, g_dbgPage) == 0);
}

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
    float fx, fy;                    // surface / stage stretch per axis (>= 1)
    float stageMinX, stageMinY, stageW, stageH;
    float centreX, centreY;          // stage centre, twips
};

// The clip's base matrix (what the timeline placed), with the clip restored to it. Returns the entry (null when the table is full).
PageEntry* BaseOf(gameswf::character* c, gameswf::matrix& base) {
    PageEntry* e = FindEntry(c);
    const gameswf::matrix current = c->get_matrix();
    if (e == nullptr) {
        base = current;
        return nullptr;
    }
    e->seen = true;
    const bool ours = e->active && std::memcmp(&current, &e->written, sizeof current) == 0;
    if (!ours) {
        e->base = current;
    } else {
        c->set_matrix(e->base);
    }
    e->active = false;
    base = e->base;
    return e;
}

void Commit(PageEntry* e, gameswf::character* c, const gameswf::matrix& m) {
    c->set_matrix(m);
    if (e != nullptr) {
        e->written = c->get_matrix();
        e->active = true;
    }
}

// Scales `base` by (kx, ky) about the point (px, py) given in the clip's PARENT space.
gameswf::matrix ScaledAbout(const gameswf::matrix& base, float kx, float ky, float px, float py) {
    gameswf::matrix m = base;
    m.m_[0][0] = base.m_[0][0] * kx;
    m.m_[0][1] = base.m_[0][1] * kx;
    m.m_[1][0] = base.m_[1][0] * ky;
    m.m_[1][1] = base.m_[1][1] * ky;
    m.m_[0][2] = px + kx * (base.m_[0][2] - px);
    m.m_[1][2] = py + ky * (base.m_[1][2] - py);
    return m;
}

struct Sibling {
    gameswf::character* c;
    PageEntry* e;
    gameswf::matrix base;
    float x0, y0, x1, y1;  // bound in PARENT space under the base matrix
    bool small, visible;
    int container;         // index of the smallest enclosing small sibling, -1
    float cx, cy;          // bound centre, parent space (base)
    float nx, ny;          // bound centre after placement, parent space
    bool placed;
};

constexpr int kMaxSiblings = 160;

// Lays out the children of `parent`. (ax, ay) is the extra non-uniform scale the ancestors carry (the page stretch, or 1 after a
// counter-scaled ancestor); a parent whose children are laid out here is itself still stretched.
void LayoutChildren(gameswf::sprite_instance* parent, const PageFrame& f, float ax, float ay, int depth, bool release) {
    Sibling sib[kMaxSiblings];
    int n = 0;
    for (int i = 0; i < parent->m_display_list.size() && n < kMaxSiblings; ++i) {
        gameswf::character* c = parent->m_display_list.get_character(i);
        if (c == nullptr || !c->is(1)) {
            continue;
        }
        Sibling& s = sib[n];
        s.c = c;
        s.e = BaseOf(c, s.base);
        gameswf::rect b;
        c->get_bound(&b);
        const float bx0 = static_cast<float>(b.m_x_min), bx1 = static_cast<float>(b.m_x_max);
        const float by0 = static_cast<float>(b.m_y_min), by1 = static_cast<float>(b.m_y_max);
        s.x0 = bx0 < bx1 ? bx0 : bx1;
        s.x1 = bx0 < bx1 ? bx1 : bx0;
        s.y0 = by0 < by1 ? by0 : by1;
        s.y1 = by0 < by1 ? by1 : by0;
        s.cx = (s.x0 + s.x1) * 0.5f;
        s.cy = (s.y0 + s.y1) * 0.5f;
        s.visible = c->m_visible;
        s.container = -1;
        s.placed = false;
        ++n;
    }
    if (release) {
        for (int i = 0; i < n; ++i) {
            if (sib[i].c->is(2) && depth < 3) {
                LayoutChildren(static_cast<gameswf::sprite_instance*>(sib[i].c), f, 1.0f, 1.0f, depth + 1, true);
            }
        }
        return;  // bases are restored by BaseOf
    }
    // Sizes in unstretched stage units: the parent's world matrix carries the page stretch (ax, ay) and the authored scale.
    const gameswf::matrix* pw = parent->get_world_matrix();
    const float psx = std::fabs(static_cast<float>(pw->m_[0][0])) > 1e-6f ? std::fabs(static_cast<float>(pw->m_[0][0])) : 1.0f;
    const float psy = std::fabs(static_cast<float>(pw->m_[1][1])) > 1e-6f ? std::fabs(static_cast<float>(pw->m_[1][1])) : 1.0f;
    for (int i = 0; i < n; ++i) {
        const float wUn = (sib[i].x1 - sib[i].x0) * psx / ax;
        const float hUn = (sib[i].y1 - sib[i].y0) * psy / ay;
        sib[i].small = wUn > 0.0f && hUn > 0.0f && wUn < 0.75f * f.stageW && hUn < 0.75f * f.stageH;
        if (DbgOn()) {
            static int count = 0;
            if (count++ < 2000 && depth == 0) {
                std::fprintf(stderr, "FITDBG d%d '%s' vis=%d parent=(%.0f,%.0f)-(%.0f,%.0f) un=%.0fx%.0f small=%d%c", depth,
                             sib[i].c->get_name().c_str(), sib[i].visible ? 1 : 0, sib[i].x0 / 20, sib[i].y0 / 20, sib[i].x1 / 20,
                             sib[i].y1 / 20, wUn / 20, hUn / 20, sib[i].small ? 1 : 0, 10);
            }
        }
    }
    // Containers among small visible siblings (smallest enclosing bound, 2% tolerance).
    const float tolX = 0.02f * f.stageW * ax / psx, tolY = 0.02f * f.stageH * ay / psy;
    for (int i = 0; i < n; ++i) {
        if (!sib[i].small) {
            continue;
        }
        float bestArea = 1e30f;
        const float own = (sib[i].x1 - sib[i].x0) * (sib[i].y1 - sib[i].y0);
        for (int j = 0; j < n; ++j) {
            if (j == i || !sib[j].small || !sib[j].visible) {
                continue;
            }
            const float area = (sib[j].x1 - sib[j].x0) * (sib[j].y1 - sib[j].y0);
            if (area <= own) {
                continue;
            }
            const bool inside = sib[j].x0 - tolX <= sib[i].x0 && sib[i].x1 <= sib[j].x1 + tolX && sib[j].y0 - tolY <= sib[i].y0 &&
                                sib[i].y1 <= sib[j].y1 + tolY;
            if (inside && area < bestArea) {
                bestArea = area;
                sib[i].container = j;
            }
        }
    }
    // Place: containers before their contents (a loop until every clip is placed; chains are short).
    const bool stretched = ax != 1.0f || ay != 1.0f;
    for (int pass = 0; pass < 8; ++pass) {
        bool progress = false;
        for (int i = 0; i < n; ++i) {
            Sibling& s = sib[i];
            if (s.placed) {
                continue;
            }
            if (!s.small) {
                // Large clip: stays stretched with the page; its children are laid out (up to three levels down).
                if (s.c->is(2) && depth < 3) {
                    LayoutChildren(static_cast<gameswf::sprite_instance*>(s.c), f, ax, ay, depth + 1, false);
                }
                s.nx = s.cx;
                s.ny = s.cy;
                s.placed = true;
                progress = true;
                continue;
            }
            if (s.container >= 0 && !sib[s.container].placed) {
                continue;
            }
            if (!stretched) {
                s.nx = s.cx;
                s.ny = s.cy;
                s.placed = true;
                progress = true;
                continue;
            }
            // Target centre in parent space: where the stretch put it, or the container's new centre plus the authored offset.
            float tx = s.cx, ty = s.cy;
            if (s.container >= 0) {
                const Sibling& k = sib[s.container];
                tx = k.nx + (s.cx - k.cx) / ax;
                ty = k.ny + (s.cy - k.cy) / ay;
            }
            // Counter-scale about the bound centre, then move the centre to the target.
            gameswf::matrix m = ScaledAbout(s.base, 1.0f / ax, 1.0f / ay, s.cx, s.cy);
            m.m_[0][2] += tx - s.cx;
            m.m_[1][2] += ty - s.cy;
            Commit(s.e, s.c, m);
            s.nx = tx;
            s.ny = ty;
            s.placed = true;
            progress = true;
            if (DbgOn()) {
                static int count = 0;
                if (count++ < 3000) {
                    gameswf::rect wb;
                    s.c->get_bound(&wb);
                    s.c->get_parent()->get_world_matrix()->transform(&wb);
                    std::fprintf(stderr, "FITDBG place d%d '%s' a=(%.2f,%.2f) base c=(%.0f,%.0f) -> (%.0f,%.0f) cont=%d stage=(%.0f,%.0f)-(%.0f,%.0f)%c", depth,
                                 s.c->get_name().c_str(), ax, ay, s.cx / 20, s.cy / 20, tx / 20, ty / 20, s.container,
                                 static_cast<float>(wb.m_x_min) / 20, static_cast<float>(wb.m_y_min) / 20, static_cast<float>(wb.m_x_max) / 20,
                                 static_cast<float>(wb.m_y_max) / 20, 10);
                }
            }
        }
        if (!progress) {
            break;
        }
    }
}

bool IsImagePage(const char* name) {
    return name != nullptr && std::strcmp(name, "menu_splash") == 0;
}

void LayoutPage(MenuFX::State* state, const PageFrame& f, bool release) {
    if (state == nullptr) {
        return;
    }
    gameswf::character* page = state->GetCharacter();
    if (page == nullptr || !page->is(2) || page->get_parent() == nullptr) {
        return;
    }
    gameswf::matrix base;
    g_dbgPage = state->m_name;
    PageEntry* e = BaseOf(page, base);
    auto* sprite = static_cast<gameswf::sprite_instance*>(page);
    if (release || (f.fx == 1.0f && f.fy == 1.0f)) {
        LayoutChildren(sprite, f, 1.0f, 1.0f, 0, true);
        return;
    }
    float kx = f.fx, ky = f.fy;
    if (IsImagePage(state->m_name)) {
        kx = ky = (f.fx > f.fy ? f.fx : f.fy);  // cover and crop
    }
    // The page stretched about the stage centre (the centre in the page's parent space).
    gameswf::point centreParent;
    page->get_parent()->get_world_matrix()->transform_by_inverse(&centreParent, gameswf::point(f.centreX, f.centreY));
    Commit(e, page, ScaledAbout(base, kx, ky, centreParent.m_x, centreParent.m_y));
    if (DbgOn()) {
        static int count = 0;
        if (count++ < 600) {
            const gameswf::matrix w = page->get_matrix();
            std::fprintf(stderr, "FITDBG page %s k=(%.2f,%.2f) centreParent=(%.0f,%.0f) base t=(%.0f,%.0f) s=(%.2f,%.2f) -> t=(%.0f,%.0f) s=(%.2f,%.2f)%c",
                         state->m_name, kx, ky, centreParent.m_x / 20, centreParent.m_y / 20, base.m_[0][2] / 20, base.m_[1][2] / 20,
                         static_cast<float>(base.m_[0][0]), static_cast<float>(base.m_[1][1]), w.m_[0][2] / 20, w.m_[1][2] / 20,
                         static_cast<float>(w.m_[0][0]), static_cast<float>(w.m_[1][1]), 10);
        }
    }
    LayoutChildren(sprite, f, kx, ky, 0, false);
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
    const float marginX = boxW > 0 && offX > 0 ? static_cast<float>(offX) * f.stageW / static_cast<float>(boxW) : 0.0f;
    const float marginY = boxH > 0 && offY > 0 ? static_cast<float>(offY) * f.stageH / static_cast<float>(boxH) : 0.0f;
    f.fx = (f.stageW + 2.0f * marginX) / f.stageW;
    f.fy = (f.stageH + 2.0f * marginY) / f.stageH;
    f.centreX = f.stageMinX + f.stageW * 0.5f;
    f.centreY = f.stageMinY + f.stageH * 0.5f;
    return true;
}

bool IsHudState(const char* name) {
    return name != nullptr && (std::strncmp(name, "menu_HUD_", 9) == 0 || std::strcmp(name, "menu_DebugHUD") == 0);
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
        if (state == nullptr || (hudMovie && IsHudState(state->m_name))) {
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
    if (current != nullptr && !onStack && !(hudMovie && IsHudState(current->m_name))) {
        LayoutPage(current, f, release);
    }
    int kept = 0;
    for (int i = 0; i < g_entryCount; ++i) {
        if (g_entries[i].movie != movie || g_entries[i].seen) {
            g_entries[kept++] = g_entries[i];
        }
    }
    g_entryCount = kept;
}

// Called by RenderFX::UpdateCursor before the hit test. The layout is applied in MenuManager::Draw, but a page whose timeline
// re-places its clips every frame (a sliding-in frame, an animated dialog) is back at its authored matrices when the cursor is
// tested at Update time; laying it out again here keeps the hit test on what was drawn. (Idempotent: the Draw pass re-applies.)
void MenuPageLayoutForCursor(const RenderFX* fx) {
    MenuManager* manager = MenuManager::GetInstance();
    if (fx == nullptr || manager == nullptr || manager->m_multiMenuManager == nullptr) {
        return;
    }
    for (int slot = 0; slot < 4; ++slot) {
        MenuFX* movie = manager->m_multiMenuManager->m_menuFX[slot];
        if (movie != nullptr && static_cast<const RenderFX*>(movie) == fx) {
            MenuPageLayoutUpdate(movie, slot == 3);
            return;
        }
    }
}
