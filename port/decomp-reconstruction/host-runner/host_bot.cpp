// Scripted play helpers of the host runner: high-level commands that drive the hero through the game's own controller and HUD touch paths
// (walk with path-finding, hunt enemies, loot, load a level directly, seeded random play). Script lines: see HOST-RUNNER.md.
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>

#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/GSLevel.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/ItemObject.hpp"
#include "dh2/Level.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/OpenableContainer.hpp"
#include "dh2/PFWorld.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/v2Controller.hpp"
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

enum Mode { kNone, kHunt, kLoot, kExplore, kRandom };
Mode g_mode = kNone;
int g_modeEnd = 0;
bool g_atkDown = false;
int g_lastSkill = 0, g_lastPotion = 0, g_lastMove = -100;
int g_exploreGoalFrame = 0;
float g_range = 3000.0f;
int g_stickDownUntil = 0, g_randNext = 0;

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
    for (ObjectBase* ob : app.m_objectManager->m_objects) {
        if (!ob->m_enabled || !ob->m_visible) continue;
        GameObject* g = nullptr;
        if (auto* it = dynamic_cast<ItemObject*>(ob)) g = it;
        else if (auto* ch = dynamic_cast<OpenableContainer*>(ob)) g = ch;
        if (!g) continue;
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

void tickHunt(Character* h) {
    Character* e = nearestEnemy(h, g_range);
    if (g_frame % 120 == 0) std::fprintf(g_runnerOut, "[bot] hunt f=%d target=%s d=%.0f hero=(%.0f,%.0f) hp=%d\n", g_frame, e ? e->m_name.c_str() : "-", e ? dist2d(h, e) : -1.0f, h->m_position.X, h->m_position.Y, h->GetHP());
    if (!e) { atkSet(false); return; }
    const float d = dist2d(h, e);
    if (d > 170.0f) {
        atkSet(false);
        trackProgress(h, e, d);
        if (g_frame - g_lastMove >= 8) { moveTo(h, e->m_position); g_lastMove = g_frame; }
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
        if (g_frame - g_lastMove >= 8) { moveTo(h, g->m_position); g_lastMove = g_frame; }
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
            case 9: if (g_rng.range(6) == 0) tapAt(kPauseX, kPauseY, 0); else tickHunt(h); break;
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
    } else if (cmd == "enemies" && h) {
        Application& app = ApplicationGetInstance();
        for (Character* o : app.m_objectManager->m_characters) {
            if (o == h) continue;
            std::fprintf(g_runnerOut, "[bot] char %s name='%s' pos=(%.0f,%.0f) d=%.0f hp=%d dead=%d en=%d vis=%d enemy=%d\n", typeid(*o).name(), o->m_name.c_str(), o->m_position.X, o->m_position.Y,
                         dist2d(h, o), o->GetHP(), o->IsDead() ? 1 : 0, o->m_enabled, o->m_visible, h->AI_IsEnemy(o) ? 1 : 0);
        }
    } else if (cmd == "objects" && h) {
        int n = 0;
        for (ObjectBase* ob : ApplicationGetInstance().m_objectManager->m_objects) {
            if (dynamic_cast<ItemObject*>(ob) || dynamic_cast<OpenableContainer*>(ob)) {
                GameObject* g = static_cast<GameObject*>(ob);
                std::fprintf(g_runnerOut, "[bot] obj %s name='%s' pos=(%.0f,%.0f) d=%.0f en=%d vis=%d\n", typeid(*ob).name(), ob->m_name.c_str(), g->m_position.X, g->m_position.Y, dist2d(h, g),
                             ob->m_enabled, ob->m_visible);
                ++n;
            }
        }
        std::fprintf(g_runnerOut, "[bot] %d loot objects\n", n);
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
