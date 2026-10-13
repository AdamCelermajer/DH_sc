// Checks the MenuFX bodies of batch r2a-game-ui-pack0-65b527-0 that run without the engine: SetFocusDefault, SetMenu,
// PopMenu (in game-ui/src/g1-game-ui-MenuFX-65b527-1.cpp), the first loop of RegisterStates and the empty-stack and
// null-target results of GetStateHandler.
// The RenderFX calls the bodies make are test doubles defined here, together with the RenderFX virtuals that the MenuFX
// vtable names, so the MenuFX object is built with its real constructor and virtual calls dispatch normally. Every
// RenderFX call and every State transition is appended to one log, and each scenario compares that log with the sequence
// the original makes.

#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <new>
#include <string>
#include <vector>

#include "dh2/MenuFX.hpp"

namespace {

int g_failures = 0;

void Check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

// Dummy characters. The bodies only compare and pass these pointers, so the storage is never read.
char g_chars[6];
const char* const kCharNames[6] = {"ctx", "btn1", "btn2", "root", "charA", "charB"};

gameswf::character* Char(int index) {
    return reinterpret_cast<gameswf::character*>(&g_chars[index]);
}

std::string NameOf(const gameswf::character* character) {
    if (character == nullptr) {
        return "null";
    }
    for (int i = 0; i < 6; ++i) {
        if (character == reinterpret_cast<const gameswf::character*>(&g_chars[i])) {
            return kCharNames[i];
        }
    }
    return "?";
}

std::vector<std::string> g_log;

// Results of the animation doubles, set per scenario.
int g_focusOutPlays = 0;  // PlayAnim(..., "focus_out", ...)
int g_focusInPlays = 0;   // PlayAnim(..., "focus_in", ...)
int g_hidePlays = 1;      // PlayAnim(..., "hide", ...)
int g_showPlays = 1;      // PlayAnim(..., "show", ...)

// FindCharacters double: the button list it returns.
int g_buttonCount = 0;
gameswf::character* g_buttons[4] = {};

void ResetDoubles() {
    g_log.clear();
    g_focusOutPlays = 0;
    g_focusInPlays = 0;
    g_hidePlays = 1;
    g_showPlays = 1;
    g_buttonCount = 0;
}

// State double: records the virtual transitions the bodies make on a State.
struct RecordingState : MenuFX::State {
    explicit RecordingState(const char* name) : MenuFX::State(name) {}

    void Show() override { g_log.push_back(std::string("Show:") + m_name); }
    void Hide() override { g_log.push_back(std::string("Hide:") + m_name); }
    void GotFocus() override { g_log.push_back(std::string("GotFocus:") + m_name); }
    void LostFocus() override { g_log.push_back(std::string("LostFocus:") + m_name); }
};

RecordingState g_a("menu_a");
RecordingState g_b("menu_b");

MenuFX::State* g_stackBuf[8];
MenuFX::State* g_stateBuf[8];
alignas(MenuFX) unsigned char g_menuStorage[sizeof(MenuFX)];
MenuFX* g_menu = nullptr;

// RenderFX test doubles. The RenderFX functions below are the ones MenuFX.hpp declares and the bodies call.
}  // namespace

RenderFX::~RenderFX() {}
void RenderFX::Load(const char*, gameswf::player_context*) {}
void RenderFX::Unload() {}
void RenderFX::Update(int, bool) {}
void RenderFX::UpdateCursor(RenderFX::Cursor&, int) {}
void RenderFX::EventListener::OnEvent(RenderFX::Event&) {}
void RenderFX::EventListener::OnFSCommand(const char*, const char*) {}
bool RenderFX::EventListener::CanHandleEvent(RenderFX::Event&) {
    return true;
}

RenderFX::CharacterArray* RenderFX::FindCharacters(gameswf::character* context, const char* prefix, int index) {
    (void)context;
    g_log.push_back(std::string("FindCharacters:") + prefix + ":" + std::to_string(index));
    m_foundCharacters.data = g_buttons;
    m_foundCharacters.count = g_buttonCount;
    return &m_foundCharacters;
}

int RenderFX::PlayAnim(gameswf::character* character, const char* anim, int flag) {
    g_log.push_back("PlayAnim:" + NameOf(character) + ":" + anim + ":" + std::to_string(flag));
    if (std::strcmp(anim, "focus_out") == 0) {
        return g_focusOutPlays;
    }
    if (std::strcmp(anim, "focus_in") == 0) {
        return g_focusInPlays;
    }
    if (std::strcmp(anim, "hide") == 0) {
        return g_hidePlays;
    }
    return g_showPlays;
}

void RenderFX::SetContext(gameswf::character* context) {
    g_log.push_back("SetContext:" + NameOf(context));
}

gameswf::character* RenderFX::GetFlashRoot() const {
    return Char(3);
}

void RenderFX::ResetFocus(int flag) {
    g_log.push_back("ResetFocus:" + std::to_string(flag));
}

void RenderFX::SetFocus(gameswf::character* character, int flag) {
    g_log.push_back("SetFocus:" + NameOf(character) + ":" + std::to_string(flag));
}

namespace {

void Expect(const std::vector<std::string>& expected, const char* what) {
    if (g_log == expected) {
        return;
    }
    std::printf("FAIL: %s\n  got:", what);
    for (const std::string& entry : g_log) {
        std::printf(" [%s]", entry.c_str());
    }
    std::printf("\n");
    ++g_failures;
}

// Clean menu for a scenario: empty stack, empty registry, flags 0, the context character, and no doubles.
void ResetMenu() {
    g_menu->m_flags = 0;
    g_menu->m_context = Char(0);
    g_menu->m_stack.m_data = g_stackBuf;
    g_menu->m_stack.m_size = 0;
    g_menu->m_stack.m_capacity = 8;
    g_menu->m_states.m_data = g_stateBuf;
    g_menu->m_states.m_size = 0;
    g_menu->m_states.m_capacity = 8;
    g_a.m_character = Char(4);
    g_b.m_character = Char(5);
    g_a.m_status = 0;
    g_b.m_status = 0;
    ResetDoubles();
}

// Registry with menu_a and menu_b, and the given stack contents (bottom first).
void Registry() {
    g_stateBuf[0] = &g_a;
    g_stateBuf[1] = &g_b;
    g_menu->m_states.m_data = g_stateBuf;
    g_menu->m_states.m_size = 2;
}

void StackOf(std::initializer_list<MenuFX::State*> bottomFirst) {
    int count = 0;
    for (MenuFX::State* state : bottomFirst) {
        g_stackBuf[count++] = state;
    }
    g_menu->m_stack.m_data = g_stackBuf;
    g_menu->m_stack.m_size = count;
}

void TestSetFocusDefault() {
    ResetMenu();
    g_menu->SetFocusDefault();
    Expect({"FindCharacters:btn:3", "ResetFocus:0"}, "SetFocusDefault with no buttons resets the focus");

    ResetMenu();
    g_buttonCount = 2;
    g_buttons[0] = Char(1);
    g_buttons[1] = Char(2);
    g_menu->SetFocusDefault();
    Expect({"FindCharacters:btn:3", "SetFocus:btn1:0"}, "SetFocusDefault focuses the first button");
}

void TestPushOntoEmptyStack() {
    ResetMenu();
    Registry();
    g_menu->PushMenu("menu_a");
    Expect({"SetContext:charA", "PlayAnim:charA:show:0", "Show:menu_a", "GotFocus:menu_a"},
           "PushMenu onto an empty stack shows the new state");
    Check(g_menu->m_stack.m_size == 1 && g_menu->m_stack.m_data[0] == &g_a, "the pushed state is the only entry");
    Check(g_a.m_status == 1, "the pushed state has status 1");
}

void TestPushOntoTop() {
    ResetMenu();
    Registry();
    StackOf({&g_a});
    g_a.m_status = 1;
    g_focusOutPlays = 1;
    g_menu->PushMenu("menu_b");
    Expect({"LostFocus:menu_a", "PlayAnim:charA:focus_out:0", "SetContext:charB", "PlayAnim:charB:show:0",
            "Show:menu_b", "GotFocus:menu_b"},
           "PushMenu over a top that plays focus_out");
    Check(g_a.m_status == 4, "a focus_out top gets status 4");
    Check(g_b.m_status == 1, "the pushed state gets status 1");
    Check(g_menu->m_stack.m_size == 2 && g_menu->GetCurrentState() == &g_b, "the stack holds both states, b on top");
}

void TestPushWhenFocusOutFails() {
    ResetMenu();
    Registry();
    StackOf({&g_a});
    g_a.m_status = 1;
    g_focusOutPlays = 0;
    g_menu->PushMenu("menu_b");
    Expect({"LostFocus:menu_a", "PlayAnim:charA:focus_out:0", "PlayAnim:charA:hide:0", "SetContext:charB",
            "PlayAnim:charB:show:0", "Show:menu_b", "GotFocus:menu_b"},
           "PushMenu falls back to hide when focus_out plays nothing");
    Check(g_a.m_status == 2, "a hidden top gets status 2");
}

void TestPopMenu() {
    ResetMenu();
    StackOf({&g_a, &g_b});
    g_a.m_status = 0;
    g_b.m_status = 1;
    g_focusInPlays = 1;
    g_menu->PopMenu();
    Expect({"LostFocus:menu_b", "Hide:menu_b", "PlayAnim:charB:hide:0", "SetContext:root", "SetContext:charA",
            "PlayAnim:charA:focus_in:0", "GotFocus:menu_a"},
           "PopMenu hides the top and focuses the state below");
    Check(g_b.m_status == 2, "the popped state gets status 2");
    Check(g_a.m_status == 3, "the state below gets status 3");
    Check(g_menu->m_stack.m_size == 1 && g_menu->GetCurrentState() == &g_a, "the stack shrinks to the state below");
}

void TestPopWhenFocusInFails() {
    ResetMenu();
    StackOf({&g_a, &g_b});
    g_focusInPlays = 0;
    g_menu->PopMenu();
    Expect({"LostFocus:menu_b", "Hide:menu_b", "PlayAnim:charB:hide:0", "SetContext:root", "SetContext:charA",
            "PlayAnim:charA:focus_in:0", "PlayAnim:charA:show:0", "GotFocus:menu_a"},
           "PopMenu shows the state below when focus_in plays nothing");
}

void TestPopLastState() {
    ResetMenu();
    StackOf({&g_a});
    g_a.m_status = 1;
    g_menu->PopMenu();
    Expect({"LostFocus:menu_a", "Hide:menu_a", "PlayAnim:charA:hide:0", "SetContext:root"},
           "PopMenu of the only state hides it and sets the root context");
    Check(g_menu->m_stack.m_size == 0, "the stack is empty after popping the only state");
    Check(g_a.m_status == 2, "the popped only state gets status 2");
}

void TestPopEmptyStack() {
    ResetMenu();
    g_menu->PopMenu();
    Expect({}, "PopMenu on an empty stack does nothing");
}

void TestSwitchMenu() {
    ResetMenu();
    Registry();
    StackOf({&g_a});
    g_a.m_status = 1;
    g_menu->SwitchMenu("menu_b");
    Expect({"LostFocus:menu_a", "Hide:menu_a", "PlayAnim:charA:hide:0", "SetContext:charB", "PlayAnim:charB:show:0",
            "Show:menu_b", "GotFocus:menu_b"},
           "SwitchMenu replaces the top");
    Check(g_menu->m_stack.m_size == 1 && g_menu->GetCurrentState() == &g_b, "the switched state replaces the top");
    Check(g_a.m_status == 2, "the replaced state gets status 2");
}

void TestSwitchOnEmptyStack() {
    ResetMenu();
    Registry();
    g_menu->SwitchMenu("menu_a");
    Expect({"SetContext:charA", "PlayAnim:charA:show:0", "Show:menu_a", "GotFocus:menu_a"},
           "SwitchMenu on an empty stack activates the state without a top to leave");
    Check(g_menu->m_stack.m_size == 0, "the state is not put on an empty stack, as in the original");
    Check(g_a.m_status == 1, "the activated state gets status 1");
}

void TestUnknownName() {
    ResetMenu();
    Registry();
    g_menu->PushMenu("missing");
    Expect({}, "an unknown menu name does nothing on an empty stack");
    Check(g_menu->m_stack.m_size == 0, "an unknown name leaves the stack alone");

    StackOf({&g_a});
    g_menu->SwitchMenu("missing");
    Expect({}, "an unknown menu name does nothing on a non-empty stack");
    Check(g_menu->m_stack.m_size == 1 && g_menu->GetCurrentState() == &g_a, "an unknown name keeps the top");
}

void TestPushWithFlag40() {
    ResetMenu();
    Registry();
    StackOf({&g_a});
    g_a.m_status = 1;
    g_menu->m_flags = 0x40;
    g_menu->PushMenu("menu_b");
    Expect({"LostFocus:menu_a", "SetContext:charB", "Show:menu_b", "GotFocus:menu_b"},
           "flags bit 0x40 skips the focus_out and show animations");
    Check(g_a.m_status == 1, "a top that is not animated keeps its status");
}

void TestPushWithFlag1() {
    ResetMenu();
    Registry();
    g_menu->m_flags = 0x01;
    g_menu->PushMenu("menu_a");
    Expect({"SetContext:charA", "PlayAnim:charA:show:0", "FindCharacters:btn:3", "ResetFocus:0", "Show:menu_a",
            "GotFocus:menu_a"},
           "flags bit 0x01 sets the default focus before the state is shown");
}

void TestPopWithFlag40() {
    ResetMenu();
    StackOf({&g_a, &g_b});
    g_menu->m_flags = 0x40;
    g_menu->PopMenu();
    Expect({"LostFocus:menu_b", "Hide:menu_b", "SetContext:root", "SetContext:charA", "GotFocus:menu_a"},
           "flags bit 0x40 skips the hide, focus_in and show animations on pop");
}

void TestRegisterStatesFirstLoop() {
    ResetMenu();
    MenuFX::State* states[2] = {&g_a, &g_b};
    g_menu->RegisterStates(states, 2);
    Check(g_menu->m_states.m_size == 2, "RegisterStates adds both states to the registry");
    Check(g_menu->m_states.m_data[0] == &g_a && g_menu->m_states.m_data[1] == &g_b,
          "RegisterStates keeps the given order");
    Check(g_a.m_menu == g_menu && g_b.m_menu == g_menu, "RegisterStates sets the owning menu of each state");
}

void TestGetStateHandlerExact() {
    ResetMenu();
    Check(g_menu->GetStateHandler(nullptr) == nullptr, "GetStateHandler on an empty stack is null");
    StackOf({&g_a, &g_b});
    Check(g_menu->GetStateHandler(nullptr) == &g_b, "GetStateHandler with no target returns the top state");
}

}  // namespace

int main() {
    g_menu = new (g_menuStorage) MenuFX();

    TestSetFocusDefault();
    TestPushOntoEmptyStack();
    TestPushOntoTop();
    TestPushWhenFocusOutFails();
    TestPopMenu();
    TestPopWhenFocusInFails();
    TestPopLastState();
    TestPopEmptyStack();
    TestSwitchMenu();
    TestSwitchOnEmptyStack();
    TestUnknownName();
    TestPushWithFlag40();
    TestPushWithFlag1();
    TestPopWithFlag40();
    TestRegisterStatesFirstLoop();
    TestGetStateHandlerExact();

    g_menu->~MenuFX();
    if (g_failures == 0) {
        std::printf("r2a-game-ui-pack0-65b527-0 test: all checks passed\n");
        return 0;
    }
    std::printf("r2a-game-ui-pack0-65b527-0 test: %d failure(s)\n", g_failures);
    return 1;
}
