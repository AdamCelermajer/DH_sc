// Batch g1-game-items-save-ObjectiveList-953dbe-0 (part 1 of ObjectiveList): the constructor, CreateObjectiveWithPyData,
// SetOwner, Eval, IsValid, InstallObjectiveMarkers, the four LoopOnAll overloads, the marker, register and compile
// forwards, _resetData, _saveData, _loadData and DBG_TraceDetailedObjectiveListInformation. Declarations and layout
// come from dh2/ObjectiveList.hpp and dh2/Objective.hpp. The destructors, AssignPyData and GetDesc are in part 2.
//
// Member pointers: the original passes a vtable byte offset (ptr) and a this adjustment with a virtual flag (adj)
// into LoopOnAll and dispatches through the vtable. Here each caller names the Objective member (&Objective::Compile
// and so on), so the compiler emits the same vtable dispatch with 64-bit slots. Offsets are not used.
//
// The original LoopOnAll returns the result of its last callee. The declarations in dh2/ObjectiveList.hpp return
// void, and the bodies here drop that result, as the rest of the class does.
//
// PARTIAL: CreateObjectiveWithPyData. The factory table s_ObjectiveImplDataMap is not declared in any header; see the
// comment on that function.

#include "dh2/ObjectiveList.hpp"

#include "dh2/Objective.hpp"
#include "pydata-accessors/structs_records.hpp"

// 0x47a3c0 ObjectiveList::ObjectiveList() (C2) and 0x47a3d4 ObjectiveList::ObjectiveList() (C1): the same body. The
// original stores 0 in m_count (+0x00), m_objectives (+0x04) and m_pyData (+0x08).
ObjectiveList::ObjectiveList() {
    m_count = 0;
    m_objectives = nullptr;
    m_pyData = nullptr;
}

// 0x47a3e8 ObjectiveList::CreateObjectiveWithPyData(Structs::v2QuestObjectiveStub*)
// PARTIAL. The original reads the row's type (the v2QuestObjective m_w1 field at +0x04 of the row), calls
// s_ObjectiveImplDataMap[type]() to make the objective, and stores the row at objective +0x0c and the type at
// objective +0x04. The table is a file-static array of 13 factory pointers (52 bytes at 0x969908 in libDungeonHunter2.so).
// Ten entries are GetObjectiveImplInstance specializations declared in dh2/global/GetObjectiveImplInstance.hpp; three
// (0x47bf20, 0x47bec4, 0x47be74) have no declaration yet. The type-to-index mapping is not recovered and no header
// declares the table, so this returns nullptr without building an objective.
Objective* ObjectiveList::CreateObjectiveWithPyData(Structs::v2QuestObjectiveStub* stub) {
    (void)stub;
    return nullptr;
}

// 0x47a414 ObjectiveList::SetOwner(Character*): stores the owner at +0x10 of every objective (Objective::m_owner).
void ObjectiveList::SetOwner(Character* owner) {
    for (int i = 0; i < m_count; ++i) {
        m_objectives[i]->m_owner = owner;
    }
}

// 0x47a444 ObjectiveList::Eval(): true for an empty list. Otherwise true when every objective is completed
// (Objective::m_completed at +0x14).
bool ObjectiveList::Eval() {
    for (int i = 0; i < m_count; ++i) {
        if (!m_objectives[i]->m_completed) {
            return false;
        }
    }
    return true;
}

// 0x47a498 ObjectiveList::IsValid(): true for an empty list. Otherwise true when every objective is valid
// (Objective::m_valid at +0x08).
bool ObjectiveList::IsValid() {
    for (int i = 0; i < m_count; ++i) {
        if (!m_objectives[i]->m_valid) {
            return false;
        }
    }
    return true;
}

// 0x47a4ec ObjectiveList::InstallObjectiveMarkers(int, int): calls InstallObjectiveMarker(markerKind, markerIndex) on every
// objective, through vtable slot 0x10 (Objective::InstallObjectiveMarker).
void ObjectiveList::InstallObjectiveMarkers(int markerKind, int markerIndex) {
    for (int i = 0; i < m_count; ++i) {
        m_objectives[i]->InstallObjectiveMarker(markerKind, markerIndex);
    }
}

// 0x47a540 ObjectiveList::LoopOnAll(void (Objective::*)(void)): calls fn on every objective, in order.
void ObjectiveList::LoopOnAll(void (Objective::*fn)()) {
    for (int i = 0; i < m_count; ++i) {
        (m_objectives[i]->*fn)();
    }
}

// 0x47a690 ObjectiveList::LoopOnAll(void (Objective::*)() const) const: the const twin of 0x47a540, same body.
void ObjectiveList::LoopOnAll(void (Objective::*fn)() const) const {
    for (int i = 0; i < m_count; ++i) {
        (m_objectives[i]->*fn)();
    }
}

// 0x47a6f8 ObjectiveList::LoopOnAll(void (Objective::*)(IStreamBase*), IStreamBase*): calls fn(stream) on every objective.
void ObjectiveList::LoopOnAll(void (Objective::*fn)(IStreamBase*), IStreamBase* stream) {
    for (int i = 0; i < m_count; ++i) {
        (m_objectives[i]->*fn)(stream);
    }
}

// 0x47a7c4 ObjectiveList::LoopOnAll(void (Objective::*)(__sFILE*) const, __sFILE*) const: the const FILE* twin, same body.
void ObjectiveList::LoopOnAll(void (Objective::*fn)(__sFILE*) const, __sFILE* stream) const {
    for (int i = 0; i < m_count; ++i) {
        (m_objectives[i]->*fn)(stream);
    }
}

// 0x47a5a8 ObjectiveList::_resetData(): Objective::_resetData (vtable slot 0x30) on every objective.
void ObjectiveList::_resetData() {
    LoopOnAll(&Objective::_resetData);
}

// 0x47a5cc ObjectiveList::RemoveObjectiveMarkers(): Objective::RemoveObjectiveMarker (vtable slot 0x14) on every objective.
void ObjectiveList::RemoveObjectiveMarkers() {
    LoopOnAll(&Objective::RemoveObjectiveMarker);
}

// 0x47a5f0 ObjectiveList::Unregister(): Objective::Unregister (vtable slot 0x1c) on every objective.
void ObjectiveList::Unregister() {
    LoopOnAll(&Objective::Unregister);
}

// 0x47a614 ObjectiveList::Register(): Objective::Register (vtable slot 0x18) on every objective.
void ObjectiveList::Register() {
    LoopOnAll(&Objective::Register);
}

// 0x47a638 ObjectiveList::InvalidateCompile(): the non-virtual Objective::InvalidateCompile on every objective.
void ObjectiveList::InvalidateCompile() {
    LoopOnAll(&Objective::InvalidateCompile);
}

// 0x47a66c ObjectiveList::Compile(): Objective::Compile (vtable slot 0x08) on every objective.
void ObjectiveList::Compile() {
    LoopOnAll(&Objective::Compile);
}

// 0x47a76c ObjectiveList::_saveData(IStreamBase*): Objective::_saveData (vtable slot 0x2c) on every objective.
void ObjectiveList::_saveData(IStreamBase* stream) {
    LoopOnAll(&Objective::_saveData, stream);
}

// 0x47a798 ObjectiveList::_loadData(IStreamBase*): Objective::_loadData (vtable slot 0x28) on every objective.
void ObjectiveList::_loadData(IStreamBase* stream) {
    LoopOnAll(&Objective::_loadData, stream);
}

// 0x47a838 ObjectiveList::DBG_TraceDetailedObjectiveListInformation(__sFILE*) const
// The original routes this through the const FILE* LoopOnAll with the slot 0x0c member pointer
// (Objective::DBG_TraceDetailedObjectiveInformation). That member returns int in dh2/Objective.hpp, so it cannot be
// named through the void member-pointer overload. The loop is written out with the same call on each objective, in order.
void ObjectiveList::DBG_TraceDetailedObjectiveListInformation(__sFILE* stream) const {
    for (int i = 0; i < m_count; ++i) {
        m_objectives[i]->DBG_TraceDetailedObjectiveInformation(stream);
    }
}
