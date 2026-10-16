// Scripted play helpers of the host runner: high-level commands that drive the hero through the game's own controller and HUD touch paths
// (walk with path-finding, hunt enemies, loot, load a level directly, seeded random play). Script lines: see HOST-RUNNER.md.
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <typeinfo>

#include "dh2/SpawnPoint.hpp"
#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/GSLevel.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/ItemObject.hpp"
#include "dh2/Level.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/OpenableContainer.hpp"
#include "dh2/DestructibleContainer.hpp"
#include "dh2/PFWorld.hpp"
#include "dh2/PFRoom.hpp"
#include "dh2/PFFloor.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/ScriptManager.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/global/Globals.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/v2Controller.hpp"
#include "dh2/TriggerZoneExitLevel.hpp"
#include "dh2/CheckpointZone.hpp"
#include "dh2/Door.hpp"
#include <algorithm>
#include "pydata-accessors/arrays.hpp"
#include "pydata-accessors/structs_records.hpp"
#include "host_runner.hpp"

extern "C" int appOnTouch(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

namespace dh2host {
namespace {

float g_scale = 1.0f;
int g_frame = 0;

// Touch in script coordinates (2400x1080 surface space).
void touch(int phase, int x, int y, int slot) {
    appOnTouch(phase, static_cast<int>(x * g_scale), static_cast<int>(y * g_scale), g_frame * 33, 1, 0, 0, slot);
}

// HUD buttons (2400x1080, from FIRST-BOOT.md).
const int kAtkX = 2052, kAtkY = 846, kSk1X = 1824, kSk1Y = 696, kSk2X = 2226, kSk2Y = 510, kPotX = 2226, kPotY = 108, kPauseX = 294, kPauseY = 322;

struct Rng {
    unsigned long long s = 88172645463325252ull;
    unsigned next() { s ^= s << 13; s ^= s >> 7; s ^= s << 17; return static_cast<unsigned>(s >> 11); }
    int range(int n) { return n <= 0 ? 0 : static_cast<int>(next() % static_cast<unsigned>(n)); }
};
Rng g_rng;

enum Mode { kNone, kHunt, kLoot, kExplore, kRandom, kExit, kWalkTo, kRoute };
// Route mode: the level's checkpoint zones in name order, then its enabled exit zones. The hero walks from one to the next (fighting what comes
// within 600 units), interacts when it stalls (doors, levers), and gives a waypoint up after 600 frames without progress.
struct Waypoint { std::string name; Point3D<float> pos; bool exit; GameObject* zone; };
int g_clickX = 0, g_clickY = 0, g_clickFrame = -1;   // a scheduled Flash click: down, move after 2 frames, up after 6
struct QueuedClick { int frame, x, y; };
std::vector<QueuedClick> g_clickQueue;                  // further scheduled clicks (rand: pause, then the pause menu's Continue)
int g_exitButtonY = 884;   // fast-travel panel: 884 = travel to the exit's level, 984 = world map
std::vector<Waypoint> g_route;
size_t g_routeIndex = 0;
int g_routeSince = 0, g_routeReached = 0, g_routeSkipped = 0;
float g_routeBest = 1e12f;
Point3D<float> g_walkTarget{0.0f, 0.0f, 0.0f};
int g_kills = 0;
bool g_auto = false;   // autoplay helpers: tap through the death screen and skip cut scenes
int g_deadSince = -1, g_lastAutoTap = 0, g_deaths = 0, g_skips = 0;
float g_walked = 0.0f;   // distance the hero moved since the last levelstat reset
float g_lastX = 0.0f, g_lastY = 0.0f;
bool g_haveLast = false;
Mode g_mode = kNone;
int g_modeEnd = 0;
bool g_atkDown = false;
int g_lastSkill = 0, g_lastPotion = 0, g_lastMove = -100;
int g_exploreGoalFrame = 0;
float g_range = 3000.0f;
int g_stickDownUntil = 0, g_randNext = 0;

// Every registered object (ObjectManager::m_objectMap; the m_objects list is not filled in this build).
std::vector<ObjectBase*> allObjects() {
    std::vector<ObjectBase*> out;
    Application& app = ApplicationGetInstance();
    if (app.m_objectManager == nullptr) return out;
    for (auto& entry : app.m_objectManager->m_objectMap) {
        if (entry.second.m_object != nullptr) out.push_back(entry.second.m_object);
    }
    return out;
}

Character* hero() {
    Application& app = ApplicationGetInstance();
    if (!app.m_playerManager || !GSLevel::s_level) return nullptr;
    return app.m_playerManager->GetLocalPlayerCharacter();
}

float dist2d(const GameObject* a, const GameObject* b) {
    const float dx = a->m_position.X - b->m_position.X, dy = a->m_position.Y - b->m_position.Y;
    return std::sqrt(dx * dx + dy * dy);
}

std::map<const GameObject*, int> g_ignoreUntil;   // targets the hero could not get closer to, ignored until this frame
const GameObject* g_curTarget = nullptr;
float g_progressDist = 1e9f;
int g_progressFrame = 0;

// Gives up on a target the hero does not get closer to (no path, blocked): ignored for a while.
void trackProgress(Character* h, const GameObject* t, float d) {
    if (t != g_curTarget) { g_curTarget = t; g_progressDist = d; g_progressFrame = g_frame; return; }
    if (d < g_progressDist - 25.0f) { g_progressDist = d; g_progressFrame = g_frame; return; }
    if (g_frame - g_progressFrame > 90) { g_ignoreUntil[t] = g_frame + 900; g_curTarget = nullptr; (void)h; }
}

Character* nearestEnemy(Character* h, float maxd) {
    Application& app = ApplicationGetInstance();
    if (!app.m_objectManager) return nullptr;
    Character* best = nullptr;
    float bd = maxd;
    for (Character* o : app.m_objectManager->m_characters) {
        if (o == h || !o->m_enabled || !o->m_visible || o->IsDead()) continue;
        if (!h->AI_IsEnemy(o)) continue;
        auto ig = g_ignoreUntil.find(o);
        if (ig != g_ignoreUntil.end() && g_frame < ig->second) continue;
        const float d = dist2d(h, o);
        if (d < bd) { bd = d; best = o; }
    }
    return best;
}

GameObject* nearestLoot(Character* h, float maxd) {
    Application& app = ApplicationGetInstance();
    if (!app.m_objectManager) return nullptr;
    GameObject* best = nullptr;
    float bd = maxd;
    for (ObjectBase* ob : allObjects()) {
        if (!ob->m_enabled || !ob->m_visible) continue;
        GameObject* g = nullptr;
        if (auto* it = dynamic_cast<ItemObject*>(ob)) g = it;
        else if (auto* ch = dynamic_cast<OpenableContainer*>(ob)) g = ch;
        else if (auto* dc = dynamic_cast<DestructibleContainer*>(ob)) g = dc;
        if (!g) continue;
        auto ig = g_ignoreUntil.find(g);
        if (ig != g_ignoreUntil.end() && g_frame < ig->second) continue;
        const float d = dist2d(h, g);
        if (d < bd) { bd = d; best = g; }
    }
    return best;
}

void atkSet(bool down) {
    if (down == g_atkDown) return;
    g_atkDown = down;
    touch(down ? 1 : 0, kAtkX, kAtkY, 1);
}
void tapAt(int x, int y, int slot) { touch(1, x, y, slot); touch(0, x, y, slot); }

void moveTo(Character* h, const Point3D<float>& p) { h->m_controller->Cmd_MoveTo(p); }

// Navigation for far targets. The game's own search (GameObject::PathTo -> PFWorld::FindPath) is capped at m_maxPath nodes (30 by default),
// so a long move answers nothing. The runner keeps a 150-unit grid of the level's path-finding floor (PFWorld::GetFloorHeightAt, filled
// lazily), finds a grid path with a breadth-first search, and hands the game hops of about 450 units (Cmd_MoveTo, which then paths them).
struct NavGrid {
    const void* level = nullptr;
    float x0 = 0, y0 = 0, z = 0, step = 150.0f;
    int w = 0, hgt = 0;
    std::vector<signed char> cells;   // -1 unknown, 0 no floor, 1 floor
    std::vector<Point3D<float>> path;
    Point3D<float> pathGoal{1e9f, 1e9f, 0.0f};
    int pathFrame = -100000;
};
NavGrid g_nav;

bool navFloor(int i, int j) {
    if (i < 0 || j < 0 || i >= g_nav.w || j >= g_nav.hgt) return false;
    signed char& c = g_nav.cells[static_cast<size_t>(j) * g_nav.w + i];
    if (c < 0) {
        Point3D<float> p{g_nav.x0 + i * g_nav.step, g_nav.y0 + j * g_nav.step, g_nav.z};
        c = Singleton<PFWorld>::s_inst.GetFloorHeightAt(p, nullptr, nullptr, nullptr, nullptr, false) ? 1 : 0;
    }
    return c == 1;
}

bool navPlan(Character* h, const Point3D<float>& target) {
    PFWorld& w = Singleton<PFWorld>::s_inst;
    if (g_nav.level != GSLevel::s_level || g_nav.cells.empty()) {
        g_nav = NavGrid();
        g_nav.level = GSLevel::s_level;
        g_nav.x0 = w.m_boundsMin.X;
        g_nav.y0 = w.m_boundsMin.Y;
        g_nav.z = h->m_position.Z;
        g_nav.w = std::min(1200, static_cast<int>((w.m_boundsMax.X - w.m_boundsMin.X) / g_nav.step) + 1);
        g_nav.hgt = std::min(1200, static_cast<int>((w.m_boundsMax.Y - w.m_boundsMin.Y) / g_nav.step) + 1);
        if (g_nav.w <= 0 || g_nav.hgt <= 0) return false;
        g_nav.cells.assign(static_cast<size_t>(g_nav.w) * g_nav.hgt, -1);
    }
    g_nav.z = h->m_position.Z;
    auto cellOf = [](float v, float o) { return static_cast<int>(std::lround((v - o) / g_nav.step)); };
    const int si = cellOf(h->m_position.X, g_nav.x0), sj = cellOf(h->m_position.Y, g_nav.y0);
    int ti = cellOf(target.X, g_nav.x0), tj = cellOf(target.Y, g_nav.y0);
    // A target off the floor (an enemy on a ledge, a zone centre): the nearest floor cell within 5 cells.
    if (!navFloor(ti, tj)) {
        bool found = false;
        for (int r = 1; r <= 5 && !found; ++r)
            for (int dj = -r; dj <= r && !found; ++dj)
                for (int di = -r; di <= r && !found; ++di)
                    if (navFloor(ti + di, tj + dj)) { ti += di; tj += dj; found = true; }
        if (!found) return false;
    }
    std::vector<int> prev(static_cast<size_t>(g_nav.w) * g_nav.hgt, -2);
    std::vector<int> queue;
    queue.reserve(4096);
    const int start = sj * g_nav.w + si, goal = tj * g_nav.w + ti;
    if (si < 0 || sj < 0 || si >= g_nav.w || sj >= g_nav.hgt) return false;
    prev[start] = -1;
    queue.push_back(start);
    bool reached = false;
    for (size_t q = 0; q < queue.size() && queue.size() < 200000; ++q) {
        const int cur = queue[q];
        if (cur == goal) { reached = true; break; }
        const int ci = cur % g_nav.w, cj = cur / g_nav.w;
        static const int kDi[8] = {1, -1, 0, 0, 1, 1, -1, -1}, kDj[8] = {0, 0, 1, -1, 1, -1, 1, -1};
        for (int k = 0; k < 8; ++k) {
            const int ni = ci + kDi[k], nj = cj + kDj[k];
            if (ni < 0 || nj < 0 || ni >= g_nav.w || nj >= g_nav.hgt) continue;
            const int n = nj * g_nav.w + ni;
            if (prev[n] != -2 || !navFloor(ni, nj)) continue;
            if (k >= 4 && (!navFloor(ci + kDi[k], cj) || !navFloor(ci, cj + kDj[k]))) continue;   // no corner cutting
            prev[n] = cur;
            queue.push_back(n);
        }
    }
    g_nav.path.clear();
    if (!reached) return false;
    for (int c = goal; c >= 0; c = prev[c]) g_nav.path.push_back(Point3D<float>{g_nav.x0 + (c % g_nav.w) * g_nav.step, g_nav.y0 + (c / g_nav.w) * g_nav.step, h->m_position.Z});
    std::reverse(g_nav.path.begin(), g_nav.path.end());
    g_nav.pathGoal = target;
    return true;
}

void stepToward(Character* h, const Point3D<float>& target) {
    const float dx = target.X - h->m_position.X, dy = target.Y - h->m_position.Y;
    const float d = std::sqrt(dx * dx + dy * dy);
    if (d < 500.0f) {
        moveTo(h, target);
        return;
    }
    const float gx = g_nav.pathGoal.X - target.X, gy = g_nav.pathGoal.Y - target.Y;
    if (g_nav.path.empty() || gx * gx + gy * gy > 300.0f * 300.0f || g_frame - g_nav.pathFrame > 240) {
        g_nav.pathFrame = g_frame;
        navPlan(h, target);
    }
    if (!g_nav.path.empty()) {
        // The furthest path point within 450 units of the hero, starting from the point nearest to him.
        size_t best = 0;
        float bestD = 1e18f;
        for (size_t i = 0; i < g_nav.path.size(); ++i) {
            const float ex = g_nav.path[i].X - h->m_position.X, ey = g_nav.path[i].Y - h->m_position.Y;
            if (ex * ex + ey * ey < bestD) { bestD = ex * ex + ey * ey; best = i; }
        }
        size_t hop = best;
        while (hop + 1 < g_nav.path.size()) {
            const float ex = g_nav.path[hop + 1].X - h->m_position.X, ey = g_nav.path[hop + 1].Y - h->m_position.Y;
            if (ex * ex + ey * ey > 450.0f * 450.0f) break;
            ++hop;
        }
        moveTo(h, g_nav.path[hop]);
        return;
    }
    const float base = std::atan2(dy, dx);
    for (int k = 0; k < 9; ++k) {
        const float off = (k == 0 ? 0.0f : ((k + 1) / 2) * 0.35f * ((k & 1) ? 1.0f : -1.0f));
        for (float r : {700.0f, 400.0f}) {
            Point3D<float> p{h->m_position.X + r * std::cos(base + off), h->m_position.Y + r * std::sin(base + off), h->m_position.Z};
            if (Singleton<PFWorld>::s_inst.GetFloorHeightAt(p, nullptr, nullptr, nullptr, nullptr, false)) {
                moveTo(h, p);
                return;
            }
        }
    }
    moveTo(h, target);
}

void tickHunt(Character* h) {
    static Character* s_last = nullptr;
    if (s_last != nullptr) {
        // The character may have been deleted since (spawned monsters are): only touch it while it is still in the character list.
        bool live = false;
        for (Character* c : ApplicationGetInstance().m_objectManager->m_characters) live = live || c == s_last;
        if (!live) s_last = nullptr;
        else if (s_last->IsDead()) { ++g_kills; s_last = nullptr; }
    }
    Character* e = nearestEnemy(h, g_range);
    if (e != nullptr && dist2d(h, e) < 400.0f) s_last = e;
    if (g_frame % 120 == 0) std::fprintf(g_runnerOut, "[bot] hunt f=%d target=%s d=%.0f hero=(%.0f,%.0f) hp=%d\n", g_frame, e ? e->m_name.c_str() : "-", e ? dist2d(h, e) : -1.0f, h->m_position.X, h->m_position.Y, h->GetHP());
    if (!e) { atkSet(false); return; }
    const float d = dist2d(h, e);
    if (d > 170.0f) {
        atkSet(false);
        trackProgress(h, e, d);
        if (g_frame - g_lastMove >= 20) { stepToward(h, e->m_position); g_lastMove = g_frame; }
    } else {
        h->m_controller->Cmd_LookAt(e->m_position);
        atkSet(true);
        if (g_frame - g_lastSkill >= 50) {
            g_lastSkill = g_frame;
            const int which = g_rng.range(2);
            tapAt(which ? kSk1X : kSk2X, which ? kSk1Y : kSk2Y, 2);
        }
    }
    if (h->GetHP() * 100 < h->GetTotalHP() * 45 && g_frame - g_lastPotion > 120) { g_lastPotion = g_frame; tapAt(kPotX, kPotY, 3); }
}

void tickLoot(Character* h) {
    GameObject* g = nearestLoot(h, g_range);
    if (!g) { atkSet(false); return; }
    if (dist2d(h, g) > 140.0f) {
        atkSet(false);
        trackProgress(h, g, dist2d(h, g));
        if (g_frame - g_lastMove >= 20) { stepToward(h, g->m_position); g_lastMove = g_frame; }
    } else {
        atkSet(true);
    }
}

void tickExplore(Character* h) {
    if (g_frame >= g_exploreGoalFrame) {
        g_exploreGoalFrame = g_frame + 60 + g_rng.range(120);
        for (int tries = 0; tries < 12; ++tries) {
            const float a = static_cast<float>(g_rng.range(628)) / 100.0f, r = 200.0f + g_rng.range(1500);
            Point3D<float> p{h->m_position.X + r * std::cos(a), h->m_position.Y + r * std::sin(a), h->m_position.Z};
            if (Singleton<PFWorld>::s_inst.GetFloorHeightAt(p, nullptr, nullptr, nullptr, nullptr, false)) { moveTo(h, p); break; }
        }
    }
}

// Seeded random play: movement goals, stick drags, attack holds, skill/potion taps, rare pause taps.
void tickRandom(Character* h) {
    if (g_frame >= g_randNext) {
        g_randNext = g_frame + 5 + g_rng.range(40);
        switch (g_rng.range(10)) {
            case 0: case 1: case 2: g_exploreGoalFrame = 0; tickExplore(h); break;
            case 3: {
                atkSet(false);
                const float a = g_rng.range(628) / 100.0f;
                touch(1, 420, 830, 0);
                touch(2, 420 + static_cast<int>(110 * std::cos(a)), 830 + static_cast<int>(110 * std::sin(a)), 0);
                g_stickDownUntil = g_frame + 10 + g_rng.range(50);
            } break;
            case 4: case 5: atkSet(true); break;
            case 6: atkSet(false); break;
            case 7: tapAt(g_rng.range(2) ? kSk1X : kSk2X, g_rng.range(2) ? kSk1Y : kSk2Y, 2); break;
            case 8: tapAt(kPotX, kPotY, 3); break;
            case 9:
                if (g_rng.range(6) == 0) {
                    // Pause (the real button at (176,340), a click) and Continue in the pause menu a little later.
                    g_clickQueue.push_back({g_frame + 2, 176, 340});
                    g_clickQueue.push_back({g_frame + 40 + g_rng.range(60), 1190, 340});
                    g_randNext = g_frame + 160;
                } else {
                    tickHunt(h);
                }
                break;
        }
    }
    if (g_stickDownUntil && g_frame >= g_stickDownUntil) { g_stickDownUntil = 0; touch(0, 420, 830, 0); }
    if (g_frame % 20 == 0) tickHunt(h);
}

}  // namespace

// Called every frame before the game runs. Handles the timed modes.
void botTick(int frame, const Options& opt) {
    g_frame = frame;
    g_scale = opt.scriptScale;
    if (Character* w = hero()) {
        if (g_haveLast) {
            const float dx = w->m_position.X - g_lastX, dy = w->m_position.Y - g_lastY;
            const float step = std::sqrt(dx * dx + dy * dy);
            if (step < 400.0f) g_walked += step;  // a warp (spawn, respawn) is not walking
        }
        g_lastX = w->m_position.X;
        g_lastY = w->m_position.Y;
        g_haveLast = true;
    } else {
        g_haveLast = false;
    }
    if (g_clickFrame < 0 && !g_clickQueue.empty() && frame >= g_clickQueue.front().frame) {
        g_clickX = g_clickQueue.front().x;
        g_clickY = g_clickQueue.front().y;
        g_clickFrame = frame;
        g_clickQueue.erase(g_clickQueue.begin());
    }
    if (g_clickFrame >= 0) {
        if (frame == g_clickFrame) touch(1, g_clickX, g_clickY, 0);
        else if (frame == g_clickFrame + 2) touch(2, g_clickX, g_clickY, 0);
        else if (frame == g_clickFrame + 6) { touch(0, g_clickX, g_clickY, 0); g_clickFrame = -1; }
    }
    if (g_auto && frame - g_lastAutoTap > 45) {
        Character* a = hero();
        if (a != nullptr && a->IsDead()) {
            if (g_deadSince < 0) { g_deadSince = frame; ++g_deaths; }
            if (frame - g_deadSince > 90) { touch(1, 1200, 540, 0); touch(2, 1200, 540, 0); touch(0, 1200, 540, 0); g_lastAutoTap = frame; }
        } else {
            g_deadSince = -1;
            if (ScriptManager::s_inst.m_inCutScene) { touch(1, 160, 90, 0); touch(2, 160, 90, 0); touch(0, 160, 90, 0); g_lastAutoTap = frame; ++g_skips; }
        }
    }
    if (g_mode == kNone) return;
    if (frame >= g_modeEnd) {
        atkSet(false);
        if (g_stickDownUntil) { g_stickDownUntil = 0; touch(0, 420, 830, 0); }
        std::fprintf(g_runnerOut, "[bot] mode ended at frame %d\n", frame);
        g_mode = kNone;
        return;
    }
    Character* h = hero();
    if (!h || h->IsDead()) return;
    if (g_mode == kRoute) {
        if (g_routeIndex >= g_route.size()) return;
        const Waypoint& w = g_route[g_routeIndex];
        const float dx = w.pos.X - h->m_position.X, dy = w.pos.Y - h->m_position.Y;
        const float d = std::sqrt(dx * dx + dy * dy);
        if (d < g_routeBest - 30.0f) { g_routeBest = d; g_routeSince = g_frame; }
        const bool touching = w.exit && w.zone != nullptr && w.zone->IsTouching(h);
        if (touching) {
            // TriggerZoneExitLevel::Update shows the HUD fast-travel panel (DisplayFastTravel) while the hero touches the zone.
            atkSet(false);
            h->m_controller->Cmd_Stop();
            std::fprintf(g_runnerOut, "[route] f=%d touching %s: pressing the fast-travel button at y=%d\n", g_frame, w.name.c_str(), g_exitButtonY);
            g_clickX = 1206;
            g_clickY = g_exitButtonY;
            g_clickFrame = g_frame + 15;
            ++g_routeReached;
            ++g_routeIndex;
            g_mode = kNone;
            return;
        }
        if (d < 200.0f || g_frame - g_routeSince > 600) {
            const bool reached = d < 200.0f;
            std::fprintf(g_runnerOut, "[route] f=%d %s %s%s d=%.0f\n", g_frame, reached ? "reached" : "gave up on", w.name.c_str(), w.exit ? " (exit)" : "", d);
            reached ? ++g_routeReached : ++g_routeSkipped;
            ++g_routeIndex;
            g_routeBest = 1e12f;
            g_routeSince = g_frame;
            atkSet(false);
            return;
        }
        Character* e = nearestEnemy(h, 600.0f);
        if (e != nullptr) { tickHunt(h); return; }
        if (g_frame - g_routeSince > 150 && (g_frame / 40) % 3 == 0) { atkSet(true); return; }   // stalled: interact (door, lever, chest)
        atkSet(false);
        if (g_frame - g_routeSince > 150) {
            // Stalled: a detour. A random reachable floor point within 1500 units that is closer to the waypoint than the hero is.
            if (g_frame - g_lastMove >= 45) {
                for (int tries = 0; tries < 40; ++tries) {
                    const float a = static_cast<float>(g_rng.range(628)) / 100.0f, r = 300.0f + g_rng.range(1200);
                    Point3D<float> p{h->m_position.X + r * std::cos(a), h->m_position.Y + r * std::sin(a), h->m_position.Z};
                    const float pd = std::sqrt((w.pos.X - p.X) * (w.pos.X - p.X) + (w.pos.Y - p.Y) * (w.pos.Y - p.Y));
                    if (pd < d - 100.0f && Singleton<PFWorld>::s_inst.GetFloorHeightAt(p, nullptr, nullptr, nullptr, nullptr, false)) {
                        moveTo(h, p);
                        break;
                    }
                }
                g_lastMove = g_frame;
            }
            return;
        }
        if (g_frame - g_lastMove >= 20) { stepToward(h, Point3D<float>{w.pos.X, w.pos.Y, h->m_position.Z}); g_lastMove = g_frame; }
        return;
    }
    if (g_mode == kWalkTo) {
        if (g_frame - g_lastMove >= 20) { stepToward(h, Point3D<float>{g_walkTarget.X, g_walkTarget.Y, h->m_position.Z}); g_lastMove = g_frame; }
        return;
    }
    if (g_mode == kExit) {
        GameObject* best = nullptr;
        float bd = 1e12f;
        for (ObjectBase* ob : allObjects()) {
            auto* z = dynamic_cast<TriggerZoneExitLevel*>(ob);
            if (z == nullptr || !z->m_enabled) continue;
            const float d = dist2d(h, z);
            if (d < bd) { bd = d; best = z; }
        }
        if (best != nullptr && g_frame - g_lastMove >= 15) { stepToward(h, best->m_position); g_lastMove = g_frame; }
        return;
    }
    switch (g_mode) {
        case kHunt: tickHunt(h); break;
        case kLoot: tickLoot(h); break;
        case kExplore: tickExplore(h); break;
        case kRandom: tickRandom(h); break;
        default: break;
    }
}

// Commands that act once: goto, walkdir, loadlevel, hunt/loot/explore/rand N, enemies, objects, kill, atk.
void botCommand(int frame, const std::string& line) {
    g_frame = frame;
    std::istringstream ss(line);
    std::string cmd;
    ss >> cmd;
    Character* h = hero();
    if (cmd == "loadlevel") {
        std::string name;
        ss >> name;
        std::fprintf(g_runnerOut, "[bot] LoadLevel %s\n", name.c_str());
        ApplicationGetInstance().LoadLevel(name.c_str(), 0, 0, false, true, 0, false, 0, 0);
    } else if (cmd == "goto" && h) {
        float x, y;
        ss >> x >> y;
        moveTo(h, Point3D<float>{x, y, h->m_position.Z});
    } else if (cmd == "route") {
        int n = 6000;
        std::string how;
        ss >> n >> how;
        g_exitButtonY = how == "worldmap" ? 984 : 884;
        g_route.clear();
        std::vector<Waypoint> exits;
        for (ObjectBase* ob : allObjects()) {
            if (auto* c = dynamic_cast<CheckpointZone*>(ob)) g_route.push_back({ob->m_name, c->m_position, false, c});
            else if (auto* z = dynamic_cast<TriggerZoneExitLevel*>(ob); z != nullptr && ob->m_enabled) exits.push_back({ob->m_name, z->m_position, true, z});
        }
        std::sort(g_route.begin(), g_route.end(), [](const Waypoint& a, const Waypoint& b) { return a.name < b.name; });
        g_route.insert(g_route.end(), exits.begin(), exits.end());
        for (const Waypoint& w : g_route) std::fprintf(g_runnerOut, "[route] waypoint %s%s at (%.0f,%.0f)\n", w.name.c_str(), w.exit ? " (exit)" : "", w.pos.X, w.pos.Y);
        g_routeIndex = 0;
        g_routeSince = frame;
        g_routeBest = 1e12f;
        g_routeReached = g_routeSkipped = 0;
        g_mode = kRoute;
        g_modeEnd = frame + n;
    } else if (cmd == "walkto") {
        int n = 600;
        ss >> g_walkTarget.X >> g_walkTarget.Y >> n;
        g_mode = kWalkTo;
        g_modeEnd = frame + n;
    } else if (cmd == "walkdir" && h) {
        float x, y;
        ss >> x >> y;
        h->m_controller->Cmd_HeadTowards(Point3D<float>{x, y, 0.0f});
    } else if (cmd == "stop" && h) {
        h->m_controller->Cmd_Stop();
    } else if (cmd == "hunt" || cmd == "loot" || cmd == "explore" || cmd == "rand") {
        int seed = 1, n = 600;
        float range = 3000.0f;
        if (cmd == "rand") ss >> seed;
        ss >> n >> range;
        if (cmd == "rand") g_rng.s = 88172645463325252ull ^ (0x9E3779B97F4A7C15ull * static_cast<unsigned long long>(seed + 1));
        g_range = range;
        g_mode = cmd == "hunt" ? kHunt : cmd == "loot" ? kLoot : cmd == "explore" ? kExplore : kRandom;
        g_modeEnd = frame + n;
        g_exploreGoalFrame = 0;
        g_randNext = frame;
    } else if (cmd == "idle") {
        g_mode = kNone;
        atkSet(false);
    } else if (cmd == "types") {
        std::map<std::string, int> histogram;
        for (ObjectBase* ob : allObjects()) ++histogram[typeid(*ob).name()];
        for (const auto& entry : histogram) std::fprintf(g_runnerOut, "[bot] type %s %d\n", entry.first.c_str(), entry.second);
    } else if (cmd == "enemies" && h) {
        Application& app = ApplicationGetInstance();
        for (Character* o : app.m_objectManager->m_characters) {
            if (o == h) continue;
            std::fprintf(g_runnerOut, "[bot] char %s name='%s' pos=(%.0f,%.0f) d=%.0f hp=%d dead=%d en=%d vis=%d enemy=%d\n", typeid(*o).name(), o->m_name.c_str(), o->m_position.X, o->m_position.Y,
                         dist2d(h, o), o->GetHP(), o->IsDead() ? 1 : 0, o->m_enabled, o->m_visible, h->AI_IsEnemy(o) ? 1 : 0);
        }
    } else if (cmd == "objects" && h) {
        int n = 0;
        for (ObjectBase* ob : allObjects()) {
            if (dynamic_cast<ItemObject*>(ob) || dynamic_cast<OpenableContainer*>(ob)) {
                GameObject* g = static_cast<GameObject*>(ob);
                std::fprintf(g_runnerOut, "[bot] obj %s name='%s' pos=(%.0f,%.0f) d=%.0f en=%d vis=%d\n", typeid(*ob).name(), ob->m_name.c_str(), g->m_position.X, g->m_position.Y, dist2d(h, g),
                             ob->m_enabled, ob->m_visible);
                ++n;
            }
        }
        std::fprintf(g_runnerOut, "[bot] %d loot objects\n", n);
    } else if (cmd == "exit") {
        int n = 900;
        ss >> n;
        g_mode = kExit;
        g_modeEnd = frame + n;
    } else if (cmd == "levelstat") {
        // One line per level for the playthrough table: loaded, walked distance, enemies (alive/dead), loot objects left, exits, hero inventory.
        std::string tag;
        ss >> tag;
        Level* level = GSLevel::s_level;
        Application& app = ApplicationGetInstance();
        int alive = 0, dead = 0, lootLeft = 0, exits = 0;
        if (h != nullptr && app.m_objectManager != nullptr) {
            for (Character* o : app.m_objectManager->m_characters) {
                if (o == h || !h->AI_IsEnemy(o)) continue;
                if (o->IsDead()) ++dead; else if (o->m_enabled) ++alive;
            }
            for (ObjectBase* ob : allObjects()) {
                if (!ob->m_enabled) continue;
                if ((dynamic_cast<ItemObject*>(ob) || dynamic_cast<OpenableContainer*>(ob) || dynamic_cast<DestructibleContainer*>(ob)) && ob->m_visible) ++lootLeft;
                if (dynamic_cast<TriggerZoneExitLevel*>(ob)) ++exits;
            }
        }
        std::fprintf(g_runnerOut, "[levelstat] %s f=%d level=%s loadStep=%d hero=%s pos=(%.0f,%.0f) hp=%d/%d walked=%.0f enemiesAlive=%d enemiesDead=%d lootLeft=%d exits=%d route=%d/%d/%zu kills=%d deaths=%d skips=%d items=%d gold=%d\n",
                     tag.c_str(), frame, level ? level->m_levelName.c_str() : "-", level ? level->m_loadStep : -1, h ? "yes" : "no", h ? h->m_position.X : 0.0f,
                     h ? h->m_position.Y : 0.0f, h ? h->GetHP() : 0, h ? h->GetTotalHP() : 0, g_walked, alive, dead, lootLeft, exits,
                     g_routeReached, g_routeSkipped, g_route.size(), g_kills, g_deaths, g_skips, h ? static_cast<int>(h->m_items.size()) : 0, h ? h->m_gold : 0);
        g_walked = 0.0f;
        g_kills = 0;
        g_deaths = 0;
        g_skips = 0;
    } else if (cmd == "near" && h) {
        float r = 800.0f;
        ss >> r;
        for (ObjectBase* ob : allObjects()) {
            auto* g = dynamic_cast<GameObject*>(ob);
            if (g == nullptr || g == h || dist2d(h, g) > r) continue;
            std::fprintf(g_runnerOut, "[bot] near %s name='%s' pos=(%.0f,%.0f) d=%.0f en=%d vis=%d goId=%d entry=%d\n", typeid(*ob).name(), ob->m_name.c_str(), g->m_position.X, g->m_position.Y,
                         dist2d(h, g), ob->m_enabled ? 1 : 0, ob->m_visible ? 1 : 0, static_cast<int>(ob->m_goId), dynamic_cast<SpawnPoint*>(g) != nullptr ? static_cast<SpawnPoint*>(g)->m_entrypointId : -1);
        }
        std::fprintf(g_runnerOut, "[bot] hero ooi=%s aabb (%.0f,%.0f,%.0f)-(%.0f,%.0f,%.0f)\n", h->m_objectOfInterest ? h->m_objectOfInterest->m_name.c_str() : "-", h->m_absAabb[0],
                     h->m_absAabb[1], h->m_absAabb[2], h->m_absAabb[3], h->m_absAabb[4], h->m_absAabb[5]);
        for (ObjectBase* ob : allObjects()) {
            auto* z = dynamic_cast<TriggerZoneExitLevel*>(ob);
            if (z == nullptr || dist2d(h, z) > r) continue;
            std::fprintf(g_runnerOut, "[bot] exitzone '%s' aabb (%.0f,%.0f,%.0f)-(%.0f,%.0f,%.0f) touching=%d level=%s\n", ob->m_name.c_str(), z->m_absAabb[0], z->m_absAabb[1], z->m_absAabb[2],
                         z->m_absAabb[3], z->m_absAabb[4], z->m_absAabb[5], z->IsTouching(h) ? 1 : 0, z->m_levelName.c_str());
        }
    } else if (cmd == "floor") {
        // Path-finding floor along a line: '#' floor, '.' none, every 100 units from (x1,y1) to (x2,y2).
        float x1, y1, x2, y2, z = -1e9f;
        ss >> x1 >> y1 >> x2 >> y2 >> z;
        const float len = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        const int n = static_cast<int>(len / 100.0f) + 1;
        std::string line;
        for (int i = 0; i <= n && i < 400; ++i) {
            const float t = n ? static_cast<float>(i) / n : 0.0f;
            Point3D<float> p{x1 + (x2 - x1) * t, y1 + (y2 - y1) * t, z > -1e8f ? z : (h ? h->m_position.Z : 0.0f)};
            float height = 0.0f;
            line += Singleton<PFWorld>::s_inst.GetFloorHeightAt(p, &height, nullptr, nullptr, nullptr, false) ? '#' : (Singleton<PFWorld>::s_inst.GetFloorHeightAt(p, &height, nullptr, nullptr, nullptr, true) ? 'o' : '.');
        }
        std::fprintf(g_runnerOut, "[bot] floor (%.0f,%.0f)->(%.0f,%.0f): %s\n", x1, y1, x2, y2, line.c_str());
    } else if (cmd == "rooms" || cmd == "roomsall") {
        float x = 0.0f, y = 0.0f;
        ss >> x >> y;
        const bool all = cmd == "roomsall";
        int total = 0;
        for (PFRoom* room : Singleton<PFWorld>::s_inst.m_rooms) {
            ++total;
            if (!all && (x < room->m_boundsMin.X || x > room->m_boundsMax.X || y < room->m_boundsMin.Y || y > room->m_boundsMax.Y)) continue;
            std::fprintf(g_runnerOut, "[bot] room '%s' box (%.0f,%.0f,%.0f)-(%.0f,%.0f,%.0f) floors=%zu flags=0x%x\n", room->m_name.c_str(), room->m_boundsMin.X, room->m_boundsMin.Y,
                         room->m_boundsMin.Z, room->m_boundsMax.X, room->m_boundsMax.Y, room->m_boundsMax.Z, room->m_floors.size(), room->m_flags);
            for (PFFloor* fl : room->m_floors) {
                const bool hit = fl->GetFloorHeightAt(Point3D<float>{x, y, 0.0f}, nullptr, nullptr);
                std::fprintf(g_runnerOut, "[bot]   floor '%s' type=0x%x flags=0x%x tris=%d box (%.0f,%.0f)-(%.0f,%.0f) pos=(%.0f,%.0f) hit(%.0f,%.0f)=%d\n", fl->m_name.c_str(), fl->m_typeMask, fl->m_objectFlags,
                             fl->m_triangleCount, fl->m_boundsMin.X, fl->m_boundsMin.Y, fl->m_boundsMax.X, fl->m_boundsMax.Y, fl->m_position.X, fl->m_position.Y, x, y, hit ? 1 : 0);
            }
        }
        std::fprintf(g_runnerOut, "[bot] %d rooms in the world\n", total);
    } else if (cmd == "cheats") {
        // The cheat switch (handle_cheats_inGame, 0x9f640a, exported .bss byte). Nothing in the shipped binary writes it (its xrefs are the
        // three readers: Application::_Update, Level::_LoadProcess, SavegameManager::toggleOption), so a test build sets it from outside.
        int on = 1;
        ss >> on;
        handle_cheats_inGame = on != 0;
        std::fprintf(g_runnerOut, "[bot] handle_cheats_inGame = %d\n", on != 0 ? 1 : 0);
    } else if (cmd == "cheat") {
        // SavegameManager::toggleOption(name): what Application::_Update's debug touch regions call (GiveGold, LevelUp).
        std::string name;
        ss >> name;
        const int before = h ? h->GetLevel() : -1;
        ApplicationGetInstance().m_savegameManager->toggleOption(name.c_str());
        std::fprintf(g_runnerOut, "[bot] toggleOption(%s): hero level %d -> %d, gold %d\n", name.c_str(), before, h ? h->GetLevel() : -1, h ? h->m_gold : 0);
    } else if (cmd == "herostat" && h) {
        char skills[96] = "";
        for (unsigned i = 0; i < 8; ++i) std::snprintf(skills + std::strlen(skills), sizeof skills - std::strlen(skills), "%d ", h->SG_GetSkillLevel(i));
        std::fprintf(g_runnerOut, "[bot] hero level=%d hp=%d/%d gold=%d items=%zu skills=%s\n", h->GetLevel(), h->GetHP(), h->GetTotalHP(), h->m_gold, h->m_items.size(), skills);
    } else if (cmd == "badbools") {
        // Objects whose ObjectBase bool flags hold something other than 0/1 (uninitialised: ASan fills new blocks with 0xbe).
        for (ObjectBase* ob : allObjects()) {
            const auto vis = *reinterpret_cast<const unsigned char*>(&ob->m_visible);
            const auto en = *reinterpret_cast<const unsigned char*>(&ob->m_enabled);
            if (vis > 1 || en > 1)
                std::fprintf(g_runnerOut, "[bot] badbool %s name='%s' visible=%u enabled=%u\n", typeid(*ob).name(), ob->m_name.c_str(), vis, en);
        }
    } else if (cmd == "auto") {
        int on = 1;
        ss >> on;
        g_auto = on != 0;
    } else if (cmd == "levels") {
        const auto* levels = reinterpret_cast<const Structs::LevelDeclaration*>(Arrays::LevelList::members);
        for (std::uint32_t i = 0; levels != nullptr && i < Arrays::LevelList::size; ++i) {
            std::fprintf(g_runnerOut, "[bot] level %u file=%s hub=%d random=%d stable=%d state=%d\n", i,
                         levels[i].LevelFile ? static_cast<const char*>(levels[i].LevelFile) : "-", levels[i].Hub, levels[i].IsRandom ? 1 : 0,
                         levels[i].Dbg_IsStable ? 1 : 0, levels[i].LevelState);
        }
    } else if (cmd == "kill" && h) {
        h->m_controller->unk_009 = true;  // test helper: kill the hero (death/respawn flow)
        h->m_controller->Cmd_Kill(nullptr, false);
    } else if (cmd == "atk") {
        int on = 1;
        ss >> on;
        atkSet(on != 0);
    } else {
        std::fprintf(g_runnerOut, "[bot] unknown or unavailable command '%s'\n", line.c_str());
    }
    std::fflush(g_runnerOut);
}

}  // namespace dh2host
