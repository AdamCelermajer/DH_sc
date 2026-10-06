#include "object_initialization_v1.hpp"
#include "stage_loader_v38_init_post.hpp"
#include <map>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace dh2::loader;
namespace {
std::string quote(const std::string& value) {
    std::string out = "\"";
    for (char c : value) {
        if (c == '"' || c == '\\') out += '\\';
        out += c;
    }
    return out + "\"";
}
struct Object {
    std::string label, type;
    bool updating{}, deleted{};
    InitializationFieldsV1 fields;
    std::uint64_t append{};
    std::uint64_t insert{};
    std::int32_t key{};
    bool deferred{};
    int init_a8{-1}, room_cc{-1};
};
struct FixtureManager: ObjectInitializationV1 {
 struct Handle {std::uint64_t words[3]{};};
 struct SourceActorBorrowV38 {std::uintptr_t identity{};Handle handle;Handle* shared_handle{&handle};};
 mutable std::map<std::uint64_t,SourceActorBorrowV38> actors;
 SourceActorBorrowV38* actor(std::uint64_t token)const{if(!token)return nullptr;auto& value=actors[token];value.identity=token;return &value;}
 std::uint32_t& source_init_phase7c_v38(){return phase;}
 const auto& modules()const{return ObjectInitializationV1::modules;}
 bool source_ordered_begin_v38(std::int32_t& key,const SourceActorBorrowV38*& out)const {auto i=objects.begin();if(i==objects.end()){out=nullptr;return false;}key=i->first;out=actor(i->second);return true;}
 bool source_ordered_next_v38(std::int32_t previous,std::int32_t& key,const SourceActorBorrowV38*& out)const {auto i=objects.upper_bound(previous);if(i==objects.end()){out=nullptr;return false;}key=i->first;out=actor(i->second);return true;}
 bool source_ordered_entry_v38(std::int32_t key,const SourceActorBorrowV38*& out)const {auto i=objects.find(key);if(i==objects.end()){out=nullptr;return false;}out=actor(i->second);return true;}
};
struct Services : ObjectInitializationServicesV1 {
    FixtureManager& state;
    std::vector<Object> data;
    std::vector<std::string> events;
    unsigned call{};
    std::string failing_operation;
    explicit Services(FixtureManager& s) : state(s) { data.emplace_back(); }
    std::string list(const std::vector<InitializationObjectV1>& values) const {
        std::string out = "[";
        for (const auto token : values) {
            if (out.size() > 1) out += ',';
            out += quote(data.at(token).label);
        }
        return out + "]";
    }
    bool event(const std::string& kind, std::uint64_t token, std::string& error,
               const std::string& extra = "") {
        events.push_back("{\"call\":" + std::to_string(call) + ",\"phase\":" + std::to_string(state.phase)
            + ",\"kind\":" + quote(kind) + (token ? ",\"object\":" + quote(data.at(token).label) : "") + extra + "}");
        if (failing_operation == kind) { error = "explicit unavailable service"; return false; }
        return true;
    }
    bool load_module(InitializationObjectV1 token, ObjectInitializationV1& s, std::string& error) override {
        if (!event("load_module", token, error)) return false;
        const auto appended = data.at(token).append;
        if (appended) {
            s.modules.push_back(appended);
            if (!event("append_module", appended, error)) return false;
        }
        const auto inserted = data.at(token).insert;
        if (inserted) {
            require_insert(s.objects.emplace(data.at(inserted).key, inserted).second);
            if (!event("insert_object", inserted, error)) return false;
        }
        return true;
    }
    bool make_handle(InitializationObjectV1 token, InitializationHandleV1& handle, std::string&) override {
        handle.words[0] = token; return true;
    }
    bool get_object(const InitializationHandleV1& handle, bool, InitializationObjectV1& token, std::string&) override {
        token = handle.words[0];
        if (data.at(token).deleted) token = 0;
        return true;
    }
    bool init_post(InitializationObjectV1 token, std::string& error) override {
        if (!event("init_post", token, error)) return false;
        auto& object = data.at(token);
        if (object.init_a8 >= 0) object.fields.a8 = static_cast<unsigned>(object.init_a8);
        return true;
    }
    bool test_enable_condition(InitializationObjectV1 token, bool force, std::string& error) override {
        return event("test_enable_condition", token, error, std::string(",\"force\":") + (force ? "true" : "false"));
    }
    bool type_name(InitializationObjectV1 token, std::string& type, std::string&) override {
        type = data.at(token).type; return true;
    }
    bool is_updatable(InitializationObjectV1 token, bool& value, std::string& error) override {
        value = data.at(token).updating; return event("is_updatable", token, error);
    }
    bool room_init_object_list(InitializationObjectV1 token, std::string& error) override {
        if (!event("room_init_object_list", token, error)) return false;
        auto& object = data.at(token);
        if (object.room_cc >= 0) object.fields.cc = static_cast<unsigned>(object.room_cc);
        return true;
    }
    bool fields(InitializationObjectV1 token, InitializationFieldsV1& fields, std::string&) override {
        fields = data.at(token).fields; return true;
    }
    bool clear_list(std::uint32_t offset, const std::vector<InitializationObjectV1>& objects, std::string& error) override {
        std::ostringstream hex; hex << "0x" << std::hex << offset;
        return event("clear_list", 0, error, ",\"list_offset\":" + quote(hex.str()) + ",\"objects\":" + list(objects));
    }
    static void require_insert(bool inserted) {
        if (!inserted) throw std::runtime_error("duplicate inserted fixture key");
    }
};
void require(bool ok, const char* reason) { if (!ok) throw std::runtime_error(reason); }
CanonicalInitPostServicesV38<FixtureManager> bridge(Services& old){
 using Actor=FixtureManager::SourceActorBorrowV38;using Handle=FixtureManager::Handle;
 CanonicalInitPostServicesV38<FixtureManager> s;
 s.load_module=[&](std::uintptr_t token,std::string& e){return old.load_module(token,old.state,e)?LifecycleStepV36::complete:LifecycleStepV36::failed;};
 s.make_handle=[&](const Actor* actor,Handle& handle,std::string& e){InitializationHandleV1 h;bool ok=old.make_handle(actor?actor->identity:0,h,e);std::copy(std::begin(h.words),std::end(h.words),std::begin(handle.words));return ok;};
 s.resolve=[&](Handle& handle,bool required,const Actor*& out,std::string& e){InitializationHandleV1 h;std::copy(std::begin(handle.words),std::end(handle.words),std::begin(h.words));std::uint64_t token{};bool ok=old.get_object(h,required,token,e);out=old.state.actor(token);return ok;};
 s.init_post=[&](const Actor& actor,std::string& e){return old.init_post(actor.identity,e);};
 s.test_enable_condition=[&](const Actor& actor,bool force,std::string& e){return old.test_enable_condition(actor.identity,force,e);};
 s.type_name=[&](const Actor& actor,std::string& name,std::string& e){return old.type_name(actor.identity,name,e);};
 s.is_updatable=[&](const Actor& actor,bool& value,std::string& e){return old.is_updatable(actor.identity,value,e);};
 s.room_init_object_list=[&](const Actor& actor,std::string& e){return old.room_init_object_list(actor.identity,e);};
 s.membership_fields=[&](const Actor& actor,std::uintptr_t& a8,std::uint8_t& ac,std::uintptr_t& cc,std::uint8_t& d0,std::string& e){InitializationFieldsV1 f;bool ok=old.fields(actor.identity,f,e);a8=f.a8;ac=f.ac;cc=f.cc;d0=f.d0;return ok;};
 s.append_list=[&](std::uint32_t offset,const Actor& actor,std::string&){auto& state=old.state;auto& list=offset==0x24?state.rooms:offset==0x2c?state.list_2c:state.list_44;list.push_back(actor.identity);return true;};
 s.clear_list=[&](std::uint32_t offset,std::string& e){auto& state=old.state;auto& list=offset==0x2c?state.list_2c:offset==0x44?state.list_44:state.list_34;if(!old.clear_list(offset,list,e))return false;list.clear();return true;};
 return s;
}

}
int main(int argc, char** argv) {
    try {
        (void)argc;(void)argv;
        unsigned count{}; require(static_cast<bool>(std::cin >> count), "missing fixture count");
        std::cout << "{\"cases\":[";
        for (unsigned row = 0; row < count; ++row) {
            std::string name; unsigned objects{}, modules{}; bool preseed{};
            require(static_cast<bool>(std::cin >> name >> objects >> modules >> preseed), "missing fixture header");
            FixtureManager state; Services services(state);
            for (unsigned i = 0; i < objects; ++i) {
                Object object; unsigned ac{}, d0{};
                require(static_cast<bool>(std::cin >> object.label >> object.type >> object.key >> object.updating >> object.deleted
                    >> object.fields.a8 >> ac >> object.fields.cc >> d0 >> object.append >> object.init_a8 >> object.room_cc
                    >> object.insert >> object.deferred),
                    "incomplete object fixture");
                object.fields.ac = static_cast<std::uint8_t>(ac); object.fields.d0 = static_cast<std::uint8_t>(d0);
                services.data.push_back(object);
                if (!object.deferred) require(state.objects.emplace(object.key, i + 1).second, "duplicate fixture key");
            }
            for (unsigned i = 0; i < modules; ++i) {
                InitializationObjectV1 token{}; require(static_cast<bool>(std::cin >> token), "missing module");
                state.ObjectInitializationV1::modules.push_back(token);
            }
            if (preseed) { state.list_2c.push_back(1); state.list_34.push_back(1); state.list_44.push_back(1); }
            std::vector<std::string> calls;
            auto pin=std::make_shared<int>(1);CanonicalInitPostV38<FixtureManager> dispatcher(pin,state,bridge(services));
            for (services.call = 0; services.call < 100; ++services.call) {
                const auto before = state.phase; const auto events = services.events.size();
                const auto result = dispatcher.step();
                require(result != LifecycleStepV36::failed, dispatcher.error().c_str());
                calls.push_back("{\"phase_before\":" + std::to_string(before) + ",\"phase_after\":" + std::to_string(state.phase)
                    + ",\"returned\":" + (result == LifecycleStepV36::complete ? "true" : "false")
                    + ",\"event_count\":" + std::to_string(services.events.size() - events) + "}");
                if (result == LifecycleStepV36::complete) break;
            }
            require(state.phase==5, "fixture did not complete");
            if (row) std::cout << ',';
            std::cout << "{\"name\":" << quote(name) << ",\"calls\":[";
            for (std::size_t i = 0; i < calls.size(); ++i) { if (i) std::cout << ','; std::cout << calls[i]; }
            std::cout << "],\"events\":[";
            for (std::size_t i = 0; i < services.events.size(); ++i) { if (i) std::cout << ','; std::cout << services.events[i]; }
            std::cout << "],\"room_list\":" << services.list(state.rooms) << ",\"transient_lists\":{\"0x2c\":"
                << services.list(state.list_2c) << ",\"0x34\":" << services.list(state.list_34)
                << ",\"0x44\":" << services.list(state.list_44) << "},\"module_list\":" << services.list(state.ObjectInitializationV1::modules) << "}";
        }
        std::cout << "]}\n";
    } catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
}
