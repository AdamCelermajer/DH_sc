#include "object_initialization_v1.hpp"

namespace dh2::loader {
namespace {
void reset_cursor(ObjectInitializationV1& s) {
    s.cursor_at_end = s.objects.empty();
    if (!s.cursor_at_end) s.cursor_key = s.objects.begin()->first;
}
void advance_cursor(ObjectInitializationV1& s) {
    const auto next = s.objects.upper_bound(s.cursor_key);
    s.cursor_at_end = next == s.objects.end();
    if (!s.cursor_at_end) s.cursor_key = next->first;
}
bool is_room(std::string name) {
    // Checked source domain is ASCII. Original uses strcasecmp, independently
    // of its case-sensitive factory dispatch registry.
    for (char& c : name) if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + ('a'-'A'));
    return name == "roomzone";
}
}
ObjectInitializationStepV1 step_object_initialization_v1(
    ObjectInitializationV1& s, ObjectInitializationServicesV1& services) {
    const auto fail = [&](const char* operation) {
        s.failed = true;
        s.error = std::string(operation) + (s.error.empty() ? ": service failed" : ": " + s.error);
        return ObjectInitializationStepV1::failed;
    };
    if (s.failed) return ObjectInitializationStepV1::failed;
    if (s.completed) return ObjectInitializationStepV1::complete;
    if (s.phase == 0) {
        s.module_cursor = 0;
        reset_cursor(s);
        s.phase = 1;
    }
    if (s.module_cursor == s.modules.size()) {
        if (s.phase == 1) {
            s.phase = 2;
            reset_cursor(s);
            ++s.phase;
        }
    } else if (s.phase == 1) {
        const auto object = s.modules[s.module_cursor];
        if (!services.load_module(object, s, s.error)) return fail("Module::LoadModule");
        ++s.module_cursor;
    }
    if (s.cursor_at_end) {
        ++s.phase;
        if (s.phase != 4) {
            s.completed = true;
            return ObjectInitializationStepV1::complete;
        }
        reset_cursor(s);
        for (const auto offset : {0x2cu, 0x44u, 0x34u}) {
            auto& list = offset == 0x2c ? s.list_2c : offset == 0x44 ? s.list_44 : s.list_34;
            if (!services.clear_list(offset, list, s.error)) return fail("clear_list");
            list.clear();
        }
        return ObjectInitializationStepV1::pending;
    }
    const auto found = s.objects.find(s.cursor_key);
    if (found == s.objects.end()) return fail("current registry node erased");
    const auto object = found->second;
    if (s.phase == 3) {
        InitializationHandleV1 handle;
        InitializationObjectV1 resolved{};
        if (!services.make_handle(object, handle, s.error)) return fail("ObjectHandle construction");
        if (!services.get_object(handle, false, resolved, s.error)) return fail("ObjectHandle::GetObject(false)");
        if (resolved) {
            if (!services.get_object(handle, true, resolved, s.error)) return fail("ObjectHandle::GetObject(true) before InitPost");
            if (!services.init_post(resolved, s.error)) return fail("ObjectBase::InitPost");
            if (!services.get_object(handle, true, resolved, s.error)) return fail("ObjectHandle::GetObject(true) after InitPost");
            if (!services.test_enable_condition(resolved, false, s.error)) return fail("ObjectBase::TestEnableCondition");
        }
    } else if (s.phase == 4 && object) {
        std::string type;
        if (!services.type_name(object, type, s.error)) return fail("object type name");
        if (is_room(type)) {
            s.rooms.push_back(object);
            if (!services.room_init_object_list(object, s.error)) return fail("RoomZone::InitObjectList");
        } else {
            bool updating{};
            if (!services.is_updatable(object, updating, s.error)) return fail("ObjectBase::IsUpdatable");
            if (updating) s.list_2c.push_back(object);
        }
        InitializationFieldsV1 fields;
        if (!services.fields(object, fields, s.error)) return fail("object membership fields");
        if ((!fields.ac && fields.a8) || (!fields.d0 && fields.cc)) s.list_44.push_back(object);
    }
    advance_cursor(s);
    return ObjectInitializationStepV1::pending;
}
}
