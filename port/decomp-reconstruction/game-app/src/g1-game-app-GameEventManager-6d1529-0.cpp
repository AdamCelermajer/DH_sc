// Batch g1-game-app-GameEventManager-6d1529-0 (GameEventManager, part 1 of 1).
// Rebuilt from the Hex-Rays pseudocode. ReInit was checked against the assembly (tools/decomp_asm.py).
// The event list is std::vector<GameEvent*> at +0x00 (begin, end, capacity). The original's begin/end loops
// become index loops over m_events that re-read size() on every pass, as the original re-reads its end.
// The member-function-pointer call uses the host's pointer-to-member call. The original's ARM adjustments
// (adj >> 1 for this, adj & 1 for a virtual slot) are therefore not needed.
#include "dh2/GameEventManager.hpp"

#include <cstddef>
#include <cstring>

#include "dh2/GameEvent.hpp"
#include "dh2/global/runtime_support.hpp"

// 0x4796f0 GameEventManager::GetEventByID(int) const
// Null when the id is negative or not below the number of events; otherwise the event at that index.
GameEvent* GameEventManager::GetEventByID(int id) const {
    if (id < 0 || static_cast<std::size_t>(id) >= m_events.size()) {
        return nullptr;
    }
    return m_events[static_cast<std::size_t>(id)];
}

// 0x479718 GameEventManager::LoopOnAllEvents(void (GameEvent::*)())
// Calls the method on every event in order. Does nothing when the list is empty. The end is re-read after
// each call, as in the original.
void GameEventManager::LoopOnAllEvents(void (GameEvent::*method)()) {
    if (m_events.empty()) {
        return;
    }
    std::size_t i = 0;
    do {
        (m_events[i]->*method)();
        ++i;
    } while (i != m_events.size());
}

// 0x47977c GameEventManager::Compile()
void GameEventManager::Compile() {
    LoopOnAllEvents(&GameEvent::Compile);
}

// 0x4797b0 GameEventManager::LoopOnAllEvents(void (GameEvent::*)() const) const
// Const overload of 0x479718 with the same loop. The method is called on each event through a pointer to a
// const member function.
void GameEventManager::LoopOnAllEvents(void (GameEvent::*method)() const) const {
    if (m_events.empty()) {
        return;
    }
    std::size_t i = 0;
    do {
        (m_events[i]->*method)();
        ++i;
    } while (i != m_events.size());
}

// 0x479814 GameEventManager::DBG_TraceDetailedInformation() const
void GameEventManager::DBG_TraceDetailedInformation() const {
    LoopOnAllEvents(&GameEvent::DBG_TraceDetailedInformation);
}

// 0x479848 GameEventManager::GetEventByName(char const*) const
// partial: the original searches the name table Arrays::v2Events::m_memberNames for the first row whose name
// is equal (strcmp, 0 when equal), bounded by Arrays::v2Events::size, and returns GetEventByID of that row.
// That table is declared in pydata-accessors/member_names.hpp, and its struct Arrays::v2Events cannot share a
// translation unit with the w7_arrays_02.hpp declaration of members and size, so the name table is not read
// here. The search runs over the events instead, comparing GameEvent::m_name (+0x08, copied from the table
// by GameEventManager::Load). The first event whose name is equal is returned. That gives the same result as
// the original as long as the events are in table order, which Load makes true. Load is partial, so until it
// is complete no event has a name and this returns null.
GameEvent* GameEventManager::GetEventByName(const char* name) const {
    for (std::size_t i = 0; i < m_events.size(); ++i) {
        GameEvent* ev = m_events[i];
        if (ev && ev->m_name && std::strcmp(name, ev->m_name) == 0) {
            return ev;
        }
    }
    return nullptr;
}

// 0x4798c8 GameEventManager::Update()
// The update runs inside a profiling context named after the manager.
void GameEventManager::Update() {
    PushProfilingContext("GameEventManager::Update");
    LoopOnAllEvents(&GameEvent::Update);
    PopProfilingContext("GameEventManager::Update");
}

// 0x47992c GameEventManager::ReInit()
// For each event in turn: GameEvent::ReInit, then GameEvent::Compile on the same event. The assembly
// (0x479944..0x479964) confirms the pair. Does nothing when the list is empty. The original's function is
// typed to return the manager pointer, but the return value is the last call's register and is not used.
void GameEventManager::ReInit() {
    if (m_events.empty()) {
        return;
    }
    std::size_t i = 0;
    do {
        GameEvent* ev = m_events[i];
        ++i;
        ev->ReInit();
        ev->Compile();
    } while (i != m_events.size());
}

// 0x479968 GameEventManager::Unload()
// Each non-null event is destroyed in place and released with CustomFree. The original does not use delete,
// so the two steps are separate calls here too. After the loop the list is emptied and keeps its capacity
// (the original sets end = begin).
void GameEventManager::Unload() {
    if (m_events.empty()) {
        return;
    }
    std::size_t i = 0;
    do {
        GameEvent* ev = m_events[i];
        ++i;
        if (ev) {
            ev->~GameEvent();
            CustomFree(ev);
        }
    } while (i != m_events.size());
    m_events.clear();
}

// 0x479bec GameEventManager::~GameEventManager() (D1, complete-object destructor)
// Unload, then the vector member releases its storage. The original calls deallocate on the begin pointer
// with capacity end-begin; the vector destructor does the same.
GameEventManager::~GameEventManager() {
    Unload();
}

// 0x479cf4 GameEventManager::GameEventManager() (C1, complete-object constructor)
// The vector member starts empty, as std::vector<GameEvent*>::vector(this) does.

// 0x479d08 GameEventManager::GameEventManager() (C2, base-object constructor)
// Skipped: compiler-generated duplicate with the same body as 0x479cf4. The C++ constructor above covers both.
GameEventManager::GameEventManager() {}

// 0x479e5c GameEventManager::Load()
// partial: the early exit is exact. The original builds one GameEvent per row of Arrays::v2Events, and that
// loop is not written. It needs three things that cannot be read together yet:
//   - Arrays::v2Events::size and ::members (w7_arrays_02.hpp) and Arrays::v2Events::m_memberNames
//     (member_names.hpp). Both files define struct Arrays::v2Events, so one translation unit cannot include
//     both (see the note on GetEventByName).
//   - the row element. The row stride is not settled: array_strides.hpp gives a listing stride of 24, and
//     w7_structs_1.hpp describes Structs::v2Event as a class with a vtable and a 44-byte stride. Without a
//     host element type, the row pointer for AssignPyData cannot be built.
// Until the loop is written the list stays empty, so GetEventByID returns null. The loop must not run with
// null entries: LoopOnAllEvents calls each entry without a null check.
void GameEventManager::Load() {
    if (!m_events.empty()) {
        return;
    }
    // PARTIAL: the per-row loop (see above) is not written.
}

// 0x479f34 GameEventManager::~GameEventManager() (D2, base-object destructor)
// Skipped: compiler-generated duplicate with the same body as 0x479bec.
