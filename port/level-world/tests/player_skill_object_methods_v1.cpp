// Host test for the Player skill VM's GameObject table convention: GetSelf and
// GetTargetListTop return method-bearing `{_this=identity}` tables, every
// Kind::method row resolves on them, the AIS combat callbacks hand scripts the
// same tables, and a method reaches the provider as a NativeRequest::method call
// with the receiver as argument 0. Usage: player_skill_object_methods_v1_audit
// <android assets directory>
#include "../player_skill_session_v1.hpp"
extern "C" {
#include "../../pydata-names/names.h"
}
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace s = dh2::player_skill_session_v1;
namespace d = dh2::data;
using Tables = dh2::player_skill_tables_adapter::Tables;
using Fn = dh2::character_native_bindings::Function;
constexpr std::uintptr_t CHAR = 0x100000001ull, AIS = 0x200000001ull, ENEMY = 0x100000042ull;

void check(bool value, const char* why) {
    if (!value) throw std::runtime_error(why);
}
std::vector<std::uint8_t> read(const std::filesystem::path& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("missing resource: " + path.string());
    return {std::istreambuf_iterator<char>(f), {}};
}
d::Bytes bytes(const std::vector<std::uint8_t>& b) { return {b.data(), b.size()}; }

struct Return {
    std::uint32_t count = 99, type = 99;
    float number = 0;
    bool boolean = false;
    std::uintptr_t identity = 0;
    std::string text;
    unsigned observed = 0;
};
int observe(void* p, const dh2_script_first_return_v1* r, char*, std::size_t) {
    auto& out = *static_cast<Return*>(p);
    out.count = r->count;
    out.type = r->type;
    out.number = r->number;
    out.boolean = r->boolean != 0;
    out.identity = r->identity;
    out.text = r->text ? std::string(r->text, r->text_bytes) : std::string{};
    ++out.observed;
    return 0;
}

struct Fixture {
    struct Call {
        std::string name;
        bool method;
        std::uint32_t count;
        std::uint32_t first_type;
        std::uintptr_t first_identity;
        float second_number;
    };
    std::shared_ptr<const Tables> tables;
    dh2::ais_player_init_vcb::State ais{AIS, 0xffffffff};
    std::unique_ptr<s::Session> session;
    std::vector<Call> calls;
    std::map<std::string, std::shared_ptr<const std::vector<std::uint8_t>>> overlays;
    int list_mode = 0; // 0 empty, 1 one Character, 2 one object
    int summon_mode = 0; // 0 rejected (no return), 1 returns the new Character

    explicit Fixture(const std::filesystem::path& assets) {
        const auto load = [&](const char* stem, const char* suffix) {
            return read(assets / "data" / (std::string(stem) + suffix + ".bin"));
        };
        std::string error;
        d::SkillTables skills;
        d::FaeryTables faeries;
        auto a = load("skills", "_pyarray"), b = load("skills", "_pyarraynames"), c = load("skills", "_pystructnames");
        check(d::load_skill_tables(bytes(a), bytes(b), bytes(c), skills, error), error.c_str());
        a = load("faeries", "_pyarray");
        b = load("faeries", "_pyarraynames");
        c = load("faeries", "_pystructnames");
        check(d::load_faery_tables(bytes(a), bytes(b), bytes(c), faeries, error), error.c_str());
        tables = Tables::create(std::move(skills), std::move(faeries), error);
        check(bool(tables), error.c_str());
        s::Configuration config;
        config.character = CHAR;
        config.ais = &ais;
        config.tables = tables;
        config.providers.context = this;
        config.providers.resolve = resolve;
        config.providers.native = native;
        s::Vm vm(dh2_script_vm_create_deferred(8 * 1024 * 1024));
        check(bool(vm), "VM creation failed");
        session = s::Session::adopt(std::move(vm), std::move(config), error);
        check(bool(session), error.c_str());
        check(!session->bind_ais_functions(error), error.c_str());
        check(!session->bind_character_functions(error), error.c_str());
    }
    static int resolve(void* p, const std::string& path, s::Resource& out, std::string& error) {
        auto& f = *static_cast<Fixture*>(p);
        const auto found = f.overlays.find(path);
        if (found == f.overlays.end()) {
            error = "no fixture for " + path;
            return -1;
        }
        out = {path, found->second};
        return 0;
    }
    static void put_number(dh2_script_value& v, float n) { v = {}; v.type = DH2_SCRIPT_NUMBER; v.number = n; }
    static int native(void* p, const s::NativeRequest& q, const dh2_script_value* a, std::uint32_t count,
                      dh2_script_value* out, std::uint32_t cap, std::uint32_t* returned, char* text, std::size_t size) {
        auto& f = *static_cast<Fixture*>(p);
        *returned = 0;
        if (q.domain != s::Domain::character) {
            if (text && size) std::snprintf(text, size, "fixture has no AIS native %s", q.name);
            return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        }
        f.calls.push_back({q.name, q.method, count, count ? a[0].type : 0, count ? a[0].identity : 0,
                           count > 1 ? a[1].number : 0.f});
        check(cap >= 5, "native result capacity");
        const auto function = q.character_function;
        if (function == Fn::game_object_get_self) {
            out[0] = {}; out[0].type = DH2_SCRIPT_IDENTITY; out[0].identity = CHAR; *returned = 1; return 0;
        }
        if (function == Fn::game_object_get_id && !q.method) {
            out[0] = {}; out[0].type = DH2_SCRIPT_IDENTITY; out[0].identity = CHAR; *returned = 1; return 0;
        }
        if (function == Fn::game_object_summon && !q.method) {
            // GameObject::_Summon pushes the new Character, or nothing when the
            // request is rejected.
            if (f.summon_mode == 0) return 0;
            out[0] = {}; out[0].type = DH2_SCRIPT_IDENTITY; out[0].identity = ENEMY + 1; *returned = 1; return 0;
        }
        if (function == Fn::game_object_get_target_list_top) {
            if (f.list_mode == 0) {
                out[0] = {}; out[0].type = DH2_SCRIPT_IDENTITY; out[0].identity = 0; *returned = 1; return 0;
            }
            out[0] = {}; out[0].type = DH2_SCRIPT_IDENTITY; out[0].identity = ENEMY;
            put_number(out[1], 123.5f);
            put_number(out[2], 30.f);
            out[3] = {}; out[3].type = DH2_SCRIPT_BOOLEAN; out[3].boolean = f.list_mode == 1;
            put_number(out[4], 0.f);
            *returned = 5; return 0;
        }
        if (q.method && function == Fn::game_object_get_name) {
            out[0] = {}; out[0].type = DH2_SCRIPT_STRING; out[0].text = "Target"; out[0].text_bytes = 6;
            *returned = 1; return 0;
        }
        if (q.method && function == Fn::game_object_is_dead) {
            out[0] = {}; out[0].type = DH2_SCRIPT_BOOLEAN; out[0].boolean = 1; *returned = 1; return 0;
        }
        if (q.method && function == Fn::character_get_prop_hp) {
            put_number(out[0], 10); put_number(out[1], 20); put_number(out[2], 50); *returned = 3; return 0;
        }
        if (q.method || function == Fn::game_object_play_fx) return 0; // recorded effect-only natives
        if (text && size) std::snprintf(text, size, "fixture has no native %s", q.name);
        return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
    }
    int load_source(const std::string& path, const std::string& source) {
        overlays[path] = std::make_shared<const std::vector<std::uint8_t>>(source.begin(), source.end());
        s::LoadResult result{};
        std::string error;
        const int status = session->load_resolved(path, &result, error);
        check(status == 0 && result.source_success, error.empty() ? "fixture source failed" : error.c_str());
        return status;
    }
    int call(const char* name, Return& out, std::vector<dh2_script_value> args = {}, std::uint32_t index = 0) {
        std::string error;
        out = {};
        const int status = session->call(name, args.data(), static_cast<std::uint32_t>(args.size()), index, observe, &out, error);
        if (status) std::cerr << "call " << name << " status " << status << ": " << error << '\n';
        return status;
    }
};

int main(int argc, char** argv) {
    try {
        check(argc == 2, "usage: player_skill_object_methods_v1_audit <assets>");
        const std::filesystem::path assets = argv[1];
        Fixture f(assets);
        check(f.session->statistics().method_bindings >= 30, "Kind::method rows were not bound");

        f.load_source("fixture/objects",
            "function t_self_type() return type(GetSelf()) end\n"
            "function t_self_this() return GetSelf()._this end\n"
            "function t_self_id() return GetSelf():GetID() == GetID() end\n"
            "function t_empty_count() return select('#', GetTargetListTop()) end\n"
            "function t_empty_type() return type((GetTargetListTop())) end\n"
            "function t_top_count() return select('#', GetTargetListTop()) end\n"
            "function t_top_type() local t = GetTargetListTop(); return type(t) end\n"
            "function t_top_second() local t, distance = GetTargetListTop(); return distance end\n"
            "function t_top_third() local t, d, angle = GetTargetListTop(); return angle end\n"
            "function t_top_is_character() local t, d, a, c = GetTargetListTop(); return c end\n"
            "function t_top_extra() local t, d, a, c, e = GetTargetListTop(); return e end\n"
            "function t_top_id() local t = GetTargetListTop(); return t:GetID() end\n"
            "function t_method_playfx() local t = GetTargetListTop(); t:PlayFX(77, 1.5); return 1 end\n"
            "function t_method_name() local t = GetTargetListTop(); return t:GetName() end\n"
            "function t_method_dead() local t = GetTargetListTop(); return t:IsDead() end\n"
            "function t_method_hp() local t = GetTargetListTop(); local cur, max, pct = t:GetPropHP(); return cur + max + pct end\n"
            "function t_method_lock() local t = GetTargetListTop(); t:LOCK(); t:UNLOCK(); return 1 end\n"
            "function t_has_method(name) return type(GetSelf()[name]) end\n"
            "function t_global_playfx() PlayFX(5); return 1 end\n"
            "function t_summon_type() local s = Summon(110, false, 1.0, 2.0, 3.0); return type(s) end\n"
            "function t_summon_id() local s = Summon(110, false, 1.0, 2.0, 3.0); s:SetMaster(GetSelf()); return s:GetID() end\n"
            "function t_summon_nil() return Summon(110, false, 1.0, 2.0, 3.0) == nil end\n");

        Return r;
        check(!f.call("t_self_type", r) && r.type == DH2_SCRIPT_STRING && r.text == "table", "GetSelf must return an object table");
        check(!f.call("t_self_this", r) && r.type == DH2_SCRIPT_IDENTITY && r.identity == CHAR, "object table carries the identity in _this");
        check(!f.call("t_self_id", r) && r.type == DH2_SCRIPT_BOOLEAN && r.boolean, "GetSelf():GetID() == GetID()");

        // Empty list: pushPointer(nullptr) alone.
        f.list_mode = 0;
        check(!f.call("t_empty_count", r) && r.number == 1.f, "empty GetTargetListTop returns one value");
        check(!f.call("t_empty_type", r) && r.text == "userdata", "empty GetTargetListTop returns the null Pointer");

        // Non-empty list: userdata table, distance, degrees, IsCharacter, extra.
        f.list_mode = 1;
        check(!f.call("t_top_count", r) && r.number == 5.f, "GetTargetListTop returns five values");
        check(!f.call("t_top_type", r) && r.text == "table", "first value is a UserData table");
        check(!f.call("t_top_second", r) && r.number == 123.5f, "second value is the distance");
        check(!f.call("t_top_third", r) && r.number == 30.f, "third value is the angle");
        check(!f.call("t_top_is_character", r) && r.type == DH2_SCRIPT_BOOLEAN && r.boolean, "fourth value is IsCharacter");
        check(!f.call("t_top_extra", r) && r.number == 0.f, "fifth value is the extra word");
        check(!f.call("t_top_id", r) && r.identity == ENEMY, "target:GetID() is the identity");
        f.list_mode = 2;
        check(!f.call("t_top_is_character", r) && r.type == DH2_SCRIPT_BOOLEAN && !r.boolean, "an object entry reports IsCharacter false");

        // Methods reach the provider as method calls with the receiver first.
        f.list_mode = 1;
        f.calls.clear();
        check(!f.call("t_method_playfx", r), "target:PlayFX failed");
        check(f.calls.size() == 2 && f.calls[0].name == "GetTargetListTop" && !f.calls[0].method, "GetTargetListTop is a function call");
        check(f.calls[1].name == "PlayFX" && f.calls[1].method && f.calls[1].count == 3 &&
              f.calls[1].first_identity == ENEMY && f.calls[1].first_type == 7 && f.calls[1].second_number == 77.f,
              "PlayFX arrives as a method with the receiver identity (type 7) before the source arguments");
        check(!f.call("t_method_name", r) && r.type == DH2_SCRIPT_STRING && r.text == "Target", "target:GetName()");
        check(!f.call("t_method_dead", r) && r.type == DH2_SCRIPT_BOOLEAN && r.boolean, "target:IsDead()");
        check(!f.call("t_method_hp", r) && r.number == 80.f, "target:GetPropHP() returns three values");
        f.calls.clear();
        check(!f.call("t_method_lock", r), "target:LOCK()/UNLOCK()");
        check(f.calls.size() == 3 && f.calls[1].name == "LOCK" && f.calls[2].name == "UNLOCK", "LOCK/UNLOCK are methods");

        // Global form keeps its receiver-less call.
        f.calls.clear();
        std::string error;
        check(!f.call("t_global_playfx", r), "global PlayFX call");
        check(!f.calls.empty() && f.calls.back().name == "PlayFX" && !f.calls.back().method && f.calls.back().first_identity == 0,
              "PlayFX() on the Player is the receiver-less global call");

        // Summon: the global returns the GameObject table of the new Character
        // (so statue:SetMaster(GetSelf()) is a method call on it) or nil.
        f.summon_mode = 1;
        f.calls.clear();
        check(!f.call("t_summon_type", r) && r.text == "table", "Summon returns an object table for the new Character");
        check(f.calls.size() == 1 && f.calls[0].name == "Summon" && !f.calls[0].method && f.calls[0].count == 5 &&
              f.calls[0].first_type == DH2_SCRIPT_NUMBER && f.calls[0].second_number == 0.f,
              "Summon reaches the provider as the receiver-less global with the source arguments");
        f.calls.clear();
        check(!f.call("t_summon_id", r) && r.identity == ENEMY + 1, "summoned:GetID() is the new Character identity");
        check(f.calls.size() >= 3 && f.calls[1].name == "GetSelf" && f.calls[2].name == "SetMaster" && f.calls[2].method &&
              f.calls[2].first_identity == ENEMY + 1 && f.calls[2].first_type == 7,
              "statue:SetMaster(GetSelf()) is a method call on the summoned Character");
        f.summon_mode = 0;
        check(!f.call("t_summon_nil", r) && r.type == DH2_SCRIPT_BOOLEAN && r.boolean, "a rejected Summon returns nil");

        // Every method name the shipped Player scripts call resolves.
        std::set<std::string> names;
        const std::regex method_call(":([A-Z][A-Za-z0-9_]*)\\(");
        for (const auto& entry : std::filesystem::directory_iterator(assets / "scripts" / "skills")) {
            const auto name = entry.path().filename().string();
            if (name.rfind("prince_", 0) && name.rfind("faerie_", 0)) continue;
            const auto data = read(entry.path());
            const std::string text(data.begin(), data.end());
            for (std::sregex_iterator it(text.begin(), text.end(), method_call), end; it != end; ++it)
                names.insert((*it)[1].str());
        }
        check(names.size() >= 25, "script method scan found too few names");
        std::vector<std::string> missing;
        for (const auto& name : names) {
            dh2_script_value arg{};
            arg.type = DH2_SCRIPT_STRING;
            arg.text = name.c_str();
            arg.text_bytes = name.size();
            Return typed;
            check(!f.call("t_has_method", typed, {arg}), "method lookup call failed");
            if (typed.text != "function") missing.push_back(name);
        }
        if (!missing.empty()) {
            std::string list;
            for (const auto& name : missing) list += name + ' ';
            std::cerr << "unresolved script methods: " << list << '\n';
        }
        check(missing.empty(), "a method used by shipped Player scripts does not resolve");

        // AIS combat callbacks hand scripts the same method-bearing tables.
        f.load_source("fixture/combat",
            "AddToVFTable('OnTargetHit','fixture_player_hit');\n"
            "function fixture_player_hit(a, d)\n"
            "  hit_attacker_id = a:GetID(); hit_defender_id = d:GetID()\n"
            "  hit_same = (d:GetID() == GetID())\n"
            "end\n");
        dh2::ais_player_init_vcb::Result vcb{};
        check(!f.session->initialize_vcb(&vcb, error), error.c_str());
        check(!f.session->dispatch_combat_result(AIS, dh2::ais_combat_result_dispatch_v1::Callback::target_hit, ENEMY, CHAR, error),
              error.c_str());
        dh2_script_value value{};
        check(!dh2_script_vm_get_global(f.session->vm(), "hit_attacker_id", &value) && value.type == DH2_SCRIPT_IDENTITY && value.identity == ENEMY,
              "attacker:GetID() inside a stolen OnTargetHit handler");
        check(!dh2_script_vm_get_global(f.session->vm(), "hit_same", &value) && value.type == DH2_SCRIPT_BOOLEAN && value.boolean,
              "defender:GetID() == GetID() inside a stolen OnTargetHit handler");

        std::cout << "{\"validation\":\"PASS\",\"method_bindings\":" << f.session->statistics().method_bindings
                  << ",\"script_methods_resolved\":" << names.size() << "}\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "FAIL: " << e.what() << '\n';
        return 1;
    }
}
