// Test for batch g1-game-character-AISDefault-7ff887-2.
//
// The checks run the hooks on zero-filled storage for the AISDefault, the Character it names and the v2Controller.
// The vptrs stay zero, so only direct member calls and the stubs below are reached (no virtual call is checked:
// OnInteract and OnAttack call Interact and IsCharacter virtually and are not run here). The path list
// (GameObject::m_pfObject.m_path) is constructed in place so that empty() is meaningful. The stubs record every
// callee in order, so each check compares the call sequence with the original control flow.

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <list>
#include <new>
#include <string>
#include <vector>

#include "dh2/AISDefault.hpp"
#include "dh2/CharAI.hpp"
#include "dh2/CharStateMachine.hpp"
#include "dh2/Character.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/LuaScript.hpp"
#include "dh2/PFObject.hpp"
#include "dh2/v2Controller.hpp"

namespace {

using Calls = std::vector<std::string>;
using PathList = std::list<const PFGInnerEdge*>;

Calls g_log;
int g_failures = 0;

// Values the stubs return.
bool g_enemy = false;
bool g_seeking = false;
bool g_inRange = false;
bool g_idle = false;
bool g_attacking = false;
bool g_hasHit = false;
bool g_hasMissed = false;

// Positions the stubs return, and the last point passed to Cmd_MoveTo.
const GameObject* g_target = nullptr;
Point3D<float> g_selfPos{};
Point3D<float> g_targetPos{};
Point3D<float> g_spot{};
Point3D<float> g_lastMove{};

// Zero-filled storage for the objects the batch touches.
alignas(Character) unsigned char g_charStorage[sizeof(Character)];
alignas(AISDefault) unsigned char g_aiStorage[sizeof(AISDefault)];
alignas(v2Controller) unsigned char g_ctrlStorage[sizeof(v2Controller)];
alignas(GameObject) unsigned char g_targetStorage[sizeof(GameObject)];
int g_edgeDummy = 0;

void Expect(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

bool SamePoint(const Point3D<float>& a, const Point3D<float>& b) {
    return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
}

Character* SelfCharacter() { return reinterpret_cast<Character*>(g_charStorage); }
AISDefault* AI() { return reinterpret_cast<AISDefault*>(g_aiStorage); }
GameObject* Target() { return reinterpret_cast<GameObject*>(g_targetStorage); }

// Sets the path list of the character to empty or to one entry.
void SetPathEmpty(bool empty) {
    PathList& path = SelfCharacter()->m_pfObject.m_path;
    path.clear();
    if (!empty) {
        path.push_back(reinterpret_cast<const PFGInnerEdge*>(&g_edgeDummy));
    }
}

// Builds the fixture: a character with its controller, a target, and the AI that belongs to the character.
// The character storage is zeroed once in main (the path list is constructed there in place); each check only resets
// the fields it reads.
void Setup() {
    std::memset(g_aiStorage, 0, sizeof g_aiStorage);
    std::memset(g_ctrlStorage, 0, sizeof g_ctrlStorage);
    std::memset(g_targetStorage, 0, sizeof g_targetStorage);

    Character* self = SelfCharacter();
    self->m_controller = reinterpret_cast<v2Controller*>(g_ctrlStorage);

    AISDefault* ai = AI();
    ai->m_character = self;
    CharAI* cai = self;
    cai->m_target = Target();
    g_target = Target();
    cai->m_targetSeeking = true;

    g_log.clear();
    g_enemy = g_seeking = g_inRange = g_idle = g_attacking = false;
    g_hasHit = g_hasMissed = false;
    g_selfPos = Point3D<float>{1.0f, 2.0f, 3.0f};
    g_targetPos = Point3D<float>{0.5f, 0.0f, -1.0f};
    g_spot = Point3D<float>{7.0f, 8.0f, 9.0f};
    g_lastMove = Point3D<float>{};
    SetPathEmpty(true);
}

}  // namespace

// ---- Stubs for the symbols the batch source calls (defined by other batches in the tree) ------------------------

bool CharAI::AI_IsEnemy(const GameObject*) const {
    g_log.push_back("IsEnemy");
    return g_enemy;
}

bool CharAI::AI_IsTargetSeeking() const {
    g_log.push_back("IsTargetSeeking");
    return g_seeking;
}

bool CharAI::AI_IsInInteractionRange(const GameObject*) const {
    g_log.push_back("IsInRange");
    return g_inRange;
}

void CharAI::AI_SetTarget(GameObject* target, bool force) {
    g_log.push_back(std::string("SetTarget ") + (target ? "obj" : "null") + (force ? " true" : " false"));
}

void CharAI::AI_SyncLastTarget() {
    g_log.push_back("SyncLastTarget");
}

void CharAI::AI_InteractWith(GameObject* obj) {
    g_log.push_back(std::string("InteractWith ") + (obj ? "obj" : "null"));
}

int CharAI::AI_PauseUpdate(unsigned int ms) {
    g_log.push_back("PauseUpdate " + std::to_string(ms));
    return 0;
}

Character* CharAI::AI_GetTargetAsCharacter() {
    return nullptr;  // not reached by the checks (OnAttack and OnInteract are not run here)
}

bool CharStateMachine::SM_IsIdle(bool) const {
    g_log.push_back("SM_IsIdle");
    return g_idle;
}

bool CharStateMachine::SM_IsAttacking() const {
    g_log.push_back("SM_IsAttacking");
    return g_attacking;
}

void v2Controller::Cmd_Stop() {
    g_log.push_back("Stop");
}

void v2Controller::Cmd_Attack(GameObject* target) {
    g_log.push_back(std::string("Attack ") + (target ? "target" : "null"));
}

void v2Controller::Cmd_MoveTo(const Point3D<float>& pos) {
    g_log.push_back("MoveTo");
    g_lastMove = pos;
}

const Point3D<float>& GameObject::GetTargetPosition() const {
    return static_cast<const void*>(this) == static_cast<const void*>(g_target) ? g_targetPos : g_selfPos;
}

Point3D<float> GameObject::GetInteractionSpot() const {
    return g_spot;
}

bool LuaScript::IsInVFTable(const char* funcName) const {
    if (std::strcmp(funcName, "OnTargetHit") == 0) {
        return g_hasHit;
    }
    if (std::strcmp(funcName, "OnTargetMissed") == 0) {
        return g_hasMissed;
    }
    return false;
}

void Character::F_MeleeAttack(AttackResult&, Character*, Character*, bool, bool) {}
void Character::F_ApplyResult(const AttackResult&, Character*, Character*, bool) {}
bool Character::IsMonster() const { return false; }
bool Character::IsMerchant() const { return false; }
Point3D<float> Character::GetLeftFootPosition() const { return Point3D<float>{}; }
Point3D<float> Character::GetRightFootPosition() const { return Point3D<float>{}; }

// ---- Checks -----------------------------------------------------------------------------------------------------

namespace {

void CheckUpdate() {
    Setup();
    AI()->unk_bc = 199;
    AI()->AISDefault::OnUpdate();
    Expect(AI()->unk_bc == 199 && g_log.empty(), "OnUpdate: counter 199 does nothing");

    Setup();
    AI()->unk_bc = 200;
    AI()->AISDefault::OnUpdate();
    Expect(AI()->unk_bc == 0, "OnUpdate: counter reset above 199");
    Expect(g_log == Calls({"PauseUpdate 1000", "Stop"}), "OnUpdate: pause 1000 ms, then stop");
}

void CheckInitVCB() {
    Setup();
    int r = AI()->AISDefault::InitVCB();
    Expect(r == 0 && AI()->unk_b8 == 0, "InitVCB: no hooks defined");

    Setup();
    g_hasHit = true;
    r = AI()->AISDefault::InitVCB();
    Expect(r == 0 && AI()->unk_b8 == 0x800, "InitVCB: OnTargetHit only");

    Setup();
    g_hasMissed = true;
    r = AI()->AISDefault::InitVCB();
    Expect(r == 0x1000 && AI()->unk_b8 == 0x1000, "InitVCB: OnTargetMissed only");

    Setup();
    g_hasHit = g_hasMissed = true;
    r = AI()->AISDefault::InitVCB();
    Expect(r == 0x1000 && AI()->unk_b8 == 0x1800, "InitVCB: both hooks");
}

void CheckMelee() {
    Setup();
    g_enemy = true;
    AI()->AISDefault::OnTargetInMeleeRange();
    Expect(g_log == Calls({"IsEnemy", "Stop", "Attack target"}), "melee: enemy stops and attacks the target");

    Setup();
    g_inRange = true;
    AI()->AISDefault::OnTargetInMeleeRange();
    Expect(g_log == Calls({"IsEnemy", "IsInRange", "Stop", "InteractWith null"}), "melee: interacts in range");

    Setup();
    AI()->AISDefault::OnTargetInMeleeRange();
    Expect(g_log == Calls({"IsEnemy", "IsInRange"}), "melee: nothing out of range");
}

void CheckCloseRange() {
    // Enemy, not sought: nothing else happens.
    Setup();
    g_enemy = true;
    AI()->AISDefault::OnTargetInCloseRange();
    Expect(g_log == Calls({"IsEnemy", "IsTargetSeeking"}), "close: enemy not sought");

    // Enemy, sought, idle, empty path: back away to 2*self - target, then drop the target.
    Setup();
    g_enemy = g_seeking = g_idle = true;
    AI()->AISDefault::OnTargetInCloseRange();
    Expect(g_log == Calls({"IsEnemy", "IsTargetSeeking", "SM_IsIdle", "MoveTo", "SetTarget null false", "SyncLastTarget"}),
           "close: enemy idle backs away and drops the target");
    Expect(SamePoint(g_lastMove, Point3D<float>{1.5f, 4.0f, 7.0f}), "close: away point is 2*self - target");
    Expect(static_cast<CharAI*>(SelfCharacter())->m_targetSeeking == false, "close: seeking cleared");

    // Enemy, sought, not idle or attacking, path not empty: nothing moves.
    Setup();
    g_enemy = g_seeking = true;
    SetPathEmpty(false);
    AI()->AISDefault::OnTargetInCloseRange();
    Expect(g_log == Calls({"IsEnemy", "IsTargetSeeking", "SM_IsIdle", "SM_IsAttacking"}),
           "close: busy walking does not move");

    // Enemy, sought, attacking, path not empty: moves but keeps the target.
    Setup();
    g_enemy = g_seeking = g_attacking = true;
    SetPathEmpty(false);
    AI()->AISDefault::OnTargetInCloseRange();
    Expect(g_log == Calls({"IsEnemy", "IsTargetSeeking", "SM_IsIdle", "SM_IsAttacking", "MoveTo"}),
           "close: attacking moves while the path is not empty");

    // Not an enemy, in interaction range: stop and interact.
    Setup();
    g_inRange = true;
    AI()->AISDefault::OnTargetInCloseRange();
    Expect(g_log == Calls({"IsEnemy", "IsInRange", "Stop", "InteractWith null"}), "close: neutral in range interacts");

    // Not an enemy, out of range, empty path: walk to the spot, then drop the target.
    Setup();
    AI()->AISDefault::OnTargetInCloseRange();
    Expect(g_log == Calls({"IsEnemy", "IsInRange", "MoveTo", "SetTarget null false", "SyncLastTarget"}),
           "close: neutral walks to the interaction spot");
    Expect(SamePoint(g_lastMove, g_spot), "close: walks to the interaction spot");

    // Not an enemy, out of range, path not empty: nothing.
    Setup();
    SetPathEmpty(false);
    AI()->AISDefault::OnTargetInCloseRange();
    Expect(g_log == Calls({"IsEnemy", "IsInRange"}), "close: neutral with a path does nothing");
}

void CheckSeek(bool ranged) {
    // Enemy branch of the ranged hook: stop and attack.
    if (ranged) {
        Setup();
        g_enemy = true;
        AI()->AISDefault::OnTargetInRangedRange();
        Expect(g_log == Calls({"IsEnemy", "Stop", "Attack target"}), "ranged: enemy stops and attacks");
    }
    const auto run = [&]() {
        if (ranged) {
            AI()->AISDefault::OnTargetInRangedRange();
        } else {
            AI()->AISDefault::OnTargetOutOfRange();
        }
    };
    const char* tag = ranged ? "ranged" : "out-of-range";
    const Calls prefix = ranged ? Calls({"IsEnemy"}) : Calls();

    // Not sought: nothing.
    Setup();
    run();
    Calls expect = prefix;
    expect.push_back("IsTargetSeeking");
    Expect(g_log == expect, tag);

    // Sought, idle, empty path: walk to the spot, then drop the target.
    Setup();
    g_seeking = g_idle = true;
    run();
    expect = prefix;
    expect.insert(expect.end(), {"IsTargetSeeking", "SM_IsIdle", "MoveTo", "SetTarget null false", "SyncLastTarget"});
    Expect(g_log == expect, tag);
    Expect(SamePoint(g_lastMove, g_spot), tag);

    // Sought, neither idle nor attacking, path not empty: nothing.
    Setup();
    g_seeking = true;
    SetPathEmpty(false);
    run();
    expect = prefix;
    expect.insert(expect.end(), {"IsTargetSeeking", "SM_IsIdle", "SM_IsAttacking"});
    Expect(g_log == expect, tag);

    // Sought, attacking, path not empty: walk to the spot and keep the target.
    Setup();
    g_seeking = g_attacking = true;
    SetPathEmpty(false);
    run();
    expect = prefix;
    expect.insert(expect.end(), {"IsTargetSeeking", "SM_IsIdle", "SM_IsAttacking", "MoveTo"});
    Expect(g_log == expect, tag);
}

}  // namespace

int main() {
    // The path list is constructed in place once, before any check uses it.
    std::memset(g_charStorage, 0, sizeof g_charStorage);
    ::new (static_cast<void*>(&SelfCharacter()->m_pfObject.m_path)) PathList();

    CheckUpdate();
    CheckInitVCB();
    CheckMelee();
    CheckCloseRange();
    CheckSeek(true);
    CheckSeek(false);

    if (g_failures == 0) {
        std::printf("AISDefault part 3: all checks passed\n");
        return 0;
    }
    std::printf("AISDefault part 3: %d check(s) failed\n", g_failures);
    return 1;
}
