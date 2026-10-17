#include "player_save_load_owner_v1.hpp"
#include <cstring>

namespace dh2::data {
namespace {
bool overlap(const void* a, std::size_t an, const void* b, std::size_t bn) {
    const auto x = reinterpret_cast<std::uintptr_t>(a);
    const auto y = reinterpret_cast<std::uintptr_t>(b);
    if (an > UINTPTR_MAX - x || bn > UINTPTR_MAX - y) return true;
    return an && bn && x < y + bn && y < x + an;
}
bool coherent(const PlayerSaveProfileV1& p) {
    return bool(p.identity) == bool(p.owner) && (p.identity || !p.campaign);
}
}
PlayerSaveLoadOwnerV1::PlayerSaveLoadOwnerV1(PlayerSavegameV1& save,
    PlayerSaveProfileV1& profile, PlayerSaveLoadServicesV1 services)
    : save_(save), profile_(profile), services_(std::move(services)) {}

bool PlayerSaveLoadOwnerV1::publish_profile(PlayerSaveProfileV1 profile,
                                           std::string& error) {
    if (overlap(&error, sizeof(error), &save_, sizeof(save_)) ||
        overlap(&error, sizeof(error), &profile_, sizeof(profile_)) ||
        overlap(&error, sizeof(error), this, sizeof(*this))) return false;
    if (!coherent(profile)) {
        error = "source profile identity and lifetime lease disagree";
        return false;
    }
    profile_ = std::move(profile);
    return true;
}
bool PlayerSaveLoadOwnerV1::send(PlayerSaveLoadRequestV1 request,
    PlayerSaveLoadResponseV1& response, std::string& error) {
    phase_ = std::uint32_t(request.operation) + 1;
    ++calls_;
    request.save = &save_;
    response = {};
    const auto services = services_;
    if (!services.owner || !services.invoke) {
        error = "genuine Save load service required at phase " + std::to_string(phase_);
        return false;
    }
    try {
        if (!services.invoke(request, response, error)) {
            if (error.empty()) error = "required Save load delivery failed at phase " + std::to_string(phase_);
            return false;
        }
    } catch (...) {
        if (error.empty()) error = "Save load service threw at phase " + std::to_string(phase_);
        return false;
    }
    // Later source reads observe live canonical +8, including provider changes.
    if (!coherent(profile_)) {
        error = "source profile binding became incoherent";
        return false;
    }
    return true;
}
bool PlayerSaveLoadOwnerV1::initialize(PlayerSaveLoadOpV1 op,
    std::uint32_t argument, std::string& error) {
    PlayerSaveLoadRequestV1 request{op};
    request.argument = argument;
    PlayerSaveLoadResponseV1 response;
    return send(request, response, error);
}
bool PlayerSaveLoadOwnerV1::section(const char* name,
    const PlayerSaveProfileV1& profile, bool reader, std::string& error) {
    ++section_calls_;
    PlayerSaveLoadRequestV1 request{PlayerSaveLoadOpV1::load_section};
    request.profile = profile;
    request.section = name;
    request.reader_enabled = reader;
    PlayerSaveLoadResponseV1 response;
    return send(request, response, error);
}
bool PlayerSaveLoadOwnerV1::load_fields(std::uint32_t mask, std::string& error) {
    if (!profile_.identity && save_.slot() != -1) {
        PlayerSaveLoadRequestV1 request{PlayerSaveLoadOpV1::filename};
        request.argument = std::uint32_t(save_.slot());
        PlayerSaveLoadResponseV1 response;
        if (!send(request, response, error)) return false;
        const auto filename = response.text;
        request = {PlayerSaveLoadOpV1::create_profile};
        request.filename = filename.c_str();
        if (!send(request, response, error)) return false;
        if (!response.profile.identity || !coherent(response.profile)) {
            error = "source Savegame constructor did not publish a real profile";
            return false;
        }
        if (!publish_profile(std::move(response.profile), error)) return false;
    }
    // One source null guard per block. Each subsequent tag rereads +8.
    if ((mask & 1) && profile_.identity) {
        for (const char* name : {"PNAM", "PLVL", "PCLS", "PDFL", "LNAM", "LEPT", "LUSP"})
            if (!section(name, profile_, true, error)) return false;
    }
    if (mask & 2) {
        if (!initialize(PlayerSaveLoadOpV1::init_levels, 0, error) ||
            !initialize(PlayerSaveLoadOpV1::init_skills, 0, error) ||
            !initialize(PlayerSaveLoadOpV1::init_faeries, 0, error) ||
            !initialize(PlayerSaveLoadOpV1::init_quests, 0, error) ||
            !initialize(PlayerSaveLoadOpV1::init_quests, 1, error)) return false;
    }
    if ((mask & 4) && profile_.identity) {
        for (const char* name : {"LVLS", "SKIL", "FAES"})
            if (!section(name, profile_, true, error)) return false;
        const auto captured = profile_;
        PlayerSaveLoadResponseV1 response;
        if (!send({PlayerSaveLoadOpV1::online}, response, error)) return false;
        bool reader = false;
        if (response.flag) {
            if (!send({PlayerSaveLoadOpV1::hosting_quest_flag}, response, error)) return false;
            reader = !response.flag;
        }
        if (!section("CFEE", captured, reader, error)) return false;
        for (const char* name : {"QEST", "PROP", "GEAR", "FTVL"})
            if (!section(name, profile_, true, error)) return false;
    }
    if ((mask & 8) && profile_.identity && !section("SKIL", profile_, true, error)) return false;
    if ((mask & 0x20) && profile_.identity && !section("PROP", profile_, true, error)) return false;
    if ((mask & 0x10) && profile_.identity) {
        if (!initialize(PlayerSaveLoadOpV1::init_quests, 0, error) ||
            !initialize(PlayerSaveLoadOpV1::init_quests, 1, error) ||
            !section("QEST", profile_, true, error)) return false;
    }
    return true;
}
bool PlayerSaveLoadOwnerV1::load_volatile(std::uint32_t mask, std::string& error) {
    if (!(mask & 0x14)) return true;
    PlayerSaveLoadResponseV1 response;
    if (!send({PlayerSaveLoadOpV1::online}, response, error)) return false;
    if (!response.flag) return true;
    if (!send({PlayerSaveLoadOpV1::local_hosting}, response, error)) return false;
    if (response.flag) {
        if (!send({PlayerSaveLoadOpV1::load_volatile_flag}, response, error)) return false;
        if (!response.flag) return true;
    }
    if (!send({PlayerSaveLoadOpV1::volatile_stream}, response, error)) return false;
    const auto stream = response.profile;
    if (!stream.identity || !coherent(stream)) {
        error = "actual volatile quest stream and lease required";
        return false;
    }
    PlayerSaveLoadRequestV1 request{PlayerSaveLoadOpV1::stream_size};
    request.profile = stream;
    if (!send(request, response, error)) return false;
    if (!response.amount) return true;
    request.operation = PlayerSaveLoadOpV1::stream_seek;
    request.argument = 0;
    if (!send(request, response, error)) return false;
    if (!send({PlayerSaveLoadOpV1::quest_definition}, response, error)) return false;
    request.operation = PlayerSaveLoadOpV1::unpack_quests;
    request.argument = 0;
    request.definition = response.value;
    return send(request, response, error);
}
bool PlayerSaveLoadOwnerV1::load(std::int32_t mask, std::string& error) {
    if (overlap(&error, sizeof(error), &save_, sizeof(save_)) ||
        overlap(&error, sizeof(error), &profile_, sizeof(profile_)) ||
        overlap(&error, sizeof(error), this, sizeof(*this))) return false;
    if (active_) { error = "Save load reentry"; return false; }
    if (!coherent(profile_)) { error = "invalid canonical profile binding"; return false; }
    struct Guard { bool& active; ~Guard() { active = false; } } guard{active_};
    active_ = true;
    error.clear(); phase_ = 0; calls_ = 0; section_calls_ = 0;
    return load_fields(std::uint32_t(mask), error) && load_volatile(std::uint32_t(mask), error);
}

bool load_player_metadata_section_v1(const PlayerSaveLoadRequestV1& request,
    const PlayerMetadataServicesV1& services, std::size_t& consumed,
    std::string& error) {
    if (overlap(&error, sizeof(error), &request, sizeof(request)) ||
        overlap(&consumed, sizeof(consumed), &request, sizeof(request)) ||
        overlap(&error, sizeof(error), &services, sizeof(services)) ||
        overlap(&consumed, sizeof(consumed), &services, sizeof(services)) ||
        overlap(&error, sizeof(error), &consumed, sizeof(consumed)) ||
        (request.save && (overlap(&error, sizeof(error), request.save, sizeof(*request.save)) ||
                          overlap(&consumed, sizeof(consumed), request.save, sizeof(*request.save))))) return false;
    consumed = 0;
    if (request.operation != PlayerSaveLoadOpV1::load_section || !request.save ||
        !request.section || !coherent(request.profile) || !request.profile.identity ||
        !request.profile.campaign) {
        error = "actual metadata request/Save/index required";
        return false;
    }
    const auto tag = request.section;
    bool supported = false;
    for (const char* name : {"PNAM", "PLVL", "PCLS", "PDFL", "LNAM", "LEPT", "LUSP"})
        if (!std::strcmp(tag, name)) supported = true;
    if (!supported) { error = "required non-metadata section provider"; return false; }
    const auto section = request.profile.campaign.section(tag);
    if (!request.reader_enabled || !section || !section->size) {
        error.clear();
        return true;
    }
    const auto bytes = request.profile.campaign.payload(tag);
    auto& save = *request.save;
    if (!std::strcmp(tag, "PNAM")) return save.load_name(bytes, consumed, error);
    if (!std::strcmp(tag, "PLVL")) return save.load_level(bytes, consumed, error);
    if (!std::strcmp(tag, "PCLS")) {
        if (!services.characters) {
            error = "genuine CharacterTable names required for PCLS";
            return false;
        }
        return save.load_class(bytes, services.characters->names, consumed, error);
    }
    if (!std::strcmp(tag, "PDFL")) return save.load_difficulty(bytes, services.context,
        services.store_selected_difficulty, consumed, error);
    if (!std::strcmp(tag, "LNAM")) return save.load_level_name(bytes, consumed, error);
    if (!std::strcmp(tag, "LEPT")) return save.load_level_entry_points(bytes, consumed, error);
    return save.load_use_spawn_points(bytes, consumed, error);
}
}
