// Test for batch g1-game-app-pack12-35588a-0. Checks SearchFailCache::add (the list keeps up to 10 entries, then starts
// over), the TestCharPropId and TestCharTemplate predicates, the Script_Wait counter (IsBlocking, Update, Execute), and
// that the Execute bodies make the isTracingScriptCmd read. Exit status 0 means every check passed.
//
// The batch source references functions of other classes (DebugSwitches, Application, Character, ItemInventory,
// v2Controller) and the ScriptCmdImpl base. Those are defined below as stubs. The Character pointers used here are
// pointers to a zeroed buffer, which the stubs never read. Each stub returns a value the test sets.
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/ItemInventory.hpp"
#include "dh2/ScriptCmdImpl.hpp"
#include "dh2/SearchFailCache.hpp"
#include "dh2/Script_StopSound.hpp"
#include "dh2/Script_UnEquipHands.hpp"
#include "dh2/Script_UnEquipItemFromSlot.hpp"
#include "dh2/Script_UnlockCharacter.hpp"
#include "dh2/Script_Wait.hpp"
#include "dh2/Script_WaitCamera.hpp"
#include "dh2/Script_WaitDialog.hpp"
#include "dh2/TestCharPropId.hpp"
#include "dh2/TestCharTemplate.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "dh2/v2Controller.hpp"

namespace {

int g_failures = 0;

#define EXPECT(cond)                                                             \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);          \
            ++g_failures;                                                        \
        }                                                                        \
    } while (0)

int g_traceCalls = 0;   // DebugSwitches::GetInstance calls (one per trace read)
int g_dt = 0;           // Application::GetDt result
int g_charPropsId = -1;
int g_charTemplateId = -1;

alignas(16) unsigned char g_appStorage[4096];   // stands in for the Application singleton; never read by the stubs
alignas(16) unsigned char g_debugStorage[64];   // stands in for the DebugSwitches singleton; never read by the stubs

}  // namespace

// ---- Stubs for the functions the batch source references --------------------------------------------------------

DebugSwitches* DebugSwitches::GetInstance() {
    ++g_traceCalls;
    return reinterpret_cast<DebugSwitches*>(g_debugStorage);
}

bool DebugSwitches::GetSwitch(const std::string& /*name*/) {
    return false;
}

Application& ApplicationGetInstance() {
    return *reinterpret_cast<Application*>(g_appStorage);
}

int Application::GetDt() {
    return g_dt;
}

Level* Application::GetCurrentLevel() const {
    return nullptr;
}

int Character::SafeGetCharPropsId() {
    return g_charPropsId;
}

int Character::SafeGetCharPropsTemplateId() {
    return g_charTemplateId;
}

ItemInstance* ItemInventory::GetEquippedItem(unsigned int /*slot*/) {
    return nullptr;
}

int ItemInventory::GetItemIndex(const ItemInstance* /*item*/) const {
    return -1;
}

bool v2Controller::s_blocked = false;

void ScriptCmdImpl::Init() {}
void ScriptCmdImpl::Update() {}
ScriptCmdImpl::~ScriptCmdImpl() {}

int main() {
    // SearchFailCache::add. Ten entries are kept. The 11th add appends, because 10 entries is not more than 10. The
    // 12th add finds 11 entries, so the list starts over and keeps only the new entry.
    {
        SearchFailCache cache;
        for (std::uint32_t i = 0; i < 10; ++i) {
            SearchFailCache::Entry e{i, i + 100, i + 200};
            cache.add(e);
        }
        EXPECT(cache.m_entries.size() == 10u);

        SearchFailCache::Entry e10{10, 110, 210};
        cache.add(e10);
        EXPECT(cache.m_entries.size() == 11u);

        SearchFailCache::Entry e11{11, 111, 211};
        cache.add(e11);
        EXPECT(cache.m_entries.size() == 1u);
        EXPECT(cache.m_entries[0].unk_00 == 11u);
        EXPECT(cache.m_entries[0].unk_04 == 111u);
        EXPECT(cache.m_entries[0].unk_08 == 211u);

        // The entry added after the restart is appended normally.
        SearchFailCache::Entry e12{12, 112, 212};
        cache.add(e12);
        EXPECT(cache.m_entries.size() == 2u);
        EXPECT(cache.m_entries[1].unk_00 == 12u);
    }

    // TestCharPropId and TestCharTemplate: a null character never matches. Otherwise the id is compared with the
    // character's SafeGetCharPropsId or SafeGetCharPropsTemplateId.
    {
        alignas(16) char storage[64] = {};
        Character* character = reinterpret_cast<Character*>(storage);
        g_charPropsId = 7;
        g_charTemplateId = 9;

        EXPECT(!TestCharPropId::Compare(7, nullptr));
        EXPECT(TestCharPropId::Compare(7, character));
        EXPECT(!TestCharPropId::Compare(8, character));

        EXPECT(!TestCharTemplate::Compare(9, nullptr));
        EXPECT(TestCharTemplate::Compare(9, character));
        EXPECT(!TestCharTemplate::Compare(7, character));
    }

    // Script_Wait. IsBlocking holds while the counter is below the duration, Update adds the frame time, and Execute
    // resets the counter (the duration copy is partial and is not checked here).
    {
        Script_Wait wait;
        wait.m_elapsed = 0;
        wait.m_duration = 300;
        EXPECT(wait.IsBlocking());

        g_dt = 120;
        wait.Update();
        EXPECT(wait.m_elapsed == 120);
        wait.Update();
        EXPECT(wait.m_elapsed == 240);
        EXPECT(wait.IsBlocking());

        wait.Update();
        EXPECT(wait.m_elapsed == 360);
        EXPECT(!wait.IsBlocking());

        const int before = g_traceCalls;
        wait.Execute(false, 0);
        EXPECT(wait.m_elapsed == 0);
        EXPECT(g_traceCalls == before + 1);
    }

    // The Execute bodies that are partial still make the isTracingScriptCmd read first, and the IsBlocking bodies that
    // return a constant return false.
    {
        Script_StopSound stop;
        Script_UnEquipHands hands;
        Script_UnEquipItemFromSlot slot;
        Script_UnlockCharacter unlock;
        Script_WaitCamera camera;
        Script_WaitDialog dialog;

        const int before = g_traceCalls;
        stop.Execute(false, 0);
        hands.Execute(false, 0);
        slot.Execute(false, 0);
        unlock.Execute(false, 0);
        camera.Execute(false, 0);
        dialog.Execute(false, 0);
        EXPECT(g_traceCalls == before + 6);

        EXPECT(!stop.IsBlocking());
        EXPECT(!hands.IsBlocking());
        EXPECT(!slot.IsBlocking());
        EXPECT(!unlock.IsBlocking());
    }

    if (g_failures == 0) {
        std::printf("g1-game-app-pack12-35588a-0 test: all checks passed\n");
    } else {
        std::printf("g1-game-app-pack12-35588a-0 test: %d failure(s)\n", g_failures);
    }
    return g_failures == 0 ? 0 : 1;
}
