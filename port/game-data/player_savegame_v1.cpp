#include "player_savegame_v1.hpp"
#include "properties.hpp"

#include <cstring>
#include <limits>

namespace dh2::data {
bool PlayerSavegameV1::initialize_new_profile_metadata(std::int32_t slot,std::string& error) {
    const auto e=reinterpret_cast<std::uintptr_t>(&error),s=reinterpret_cast<std::uintptr_t>(this);
    if(e>UINTPTR_MAX-sizeof(error)||s>UINTPTR_MAX-sizeof(*this)||(e<s+sizeof(*this)&&s<e+sizeof(error)))return false;
    if(slot_!=-1||character_||level_||class_!=-1||!name_.empty()||skills_initialized_||
       !skills_.empty()||!slots_[0].empty()||!slots_[1].empty()||level_name_loaded_||use_spawn_points_loaded_){
        error="indexed profile constructor requires a fresh unbound Save";return false;
    }
    slot_=slot;level_=1;class_=-1;unlocked_difficulty_=0;
    level_name_fields_.word50.fill(0);level_name_fields_.word5c.fill(0);
    // The two actual QuestSavegame constructors store 1 at +44/+48/+4c.
    level_name_fields_.quest_wordfc.fill(1);level_name_fields_.quest_word15c.fill(1);
    level_entry_points_.fill(0);properties_byte_194_=0;save_blocked_=false;
    error.clear();return true;
}
void PlayerSavegameV1::generate_profile_seeds(std::uint32_t time) noexcept {
    const std::array<std::uint32_t,3> words{time,time+0x537bu,time+0x537bu+0xfd2fu};
    for(std::size_t i=0;i<3;++i)std::memcpy(&level_name_fields_.word5c[i],&words[i],4);
}
bool PlayerSavegameV1::set_new_profile_locations(std::uint32_t count,std::string& error) {
    const auto e=reinterpret_cast<std::uintptr_t>(&error),s=reinterpret_cast<std::uintptr_t>(this);
    if(e>UINTPTR_MAX-sizeof(error)||s>UINTPTR_MAX-sizeof(*this)||(e<s+sizeof(*this)&&s<e+sizeof(error)))return false;
    // Other counts reach uninitialized source spawn bytes or overflow the
    // three fixed arrays. Native creation requires the actual table size 3.
    if(count!=3){error="actual three GameDifficulties rows required";return false;}
    for(std::size_t i=0;i<3;++i){level_name_fields_.word50[i]=41;level_entry_points_[i]=0;use_spawn_points_[i]=1;}
    level_name_loaded_=true;use_spawn_points_loaded_=true;error.clear();return true;
}
bool PlayerSavegameV1::load_properties(Bytes bytes,PropertyView& view,
                                      std::size_t& consumed,std::string& error) {
    const auto overlaps=[](const void* a,std::size_t an,const void* b,std::size_t bn){
        const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
        return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(an&&bn&&x<y+bn&&y<x+an);
    };
    const auto separate=[&](const void* p,std::size_t n){
        if(overlaps(p,n,this,sizeof(*this))||overlaps(p,n,&view,sizeof(view))||overlaps(p,n,bytes.data,bytes.size))return false;
        for(const auto* sheet:std::array<const std::int32_t*,6>{view.defaults,view.types,view.base,view.saved,view.gear,view.resolved})
            if(sheet&&overlaps(p,n,sheet,224*4))return false;
        return true;
    };
    if(!separate(&error,sizeof(error))||!separate(&consumed,sizeof(consumed))||
       overlaps(&error,sizeof(error),&consumed,sizeof(consumed)))return false;
    consumed=0;error.clear();
    if(!character_){error="source PROP Character/Debug assertion boundary";return false;}
    if(!bytes.data&&bytes.size){error="bounded PROP bytes required";return false;}
    const auto word=[&](std::uint32_t& value){
        if(bytes.size-consumed<4)return false;
        const auto* p=bytes.data+consumed;
        value=std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);
        consumed+=4;return true;
    };
    std::uint32_t count=0;
    if(!word(count)){error="truncated source PROP count";return false;}
    if(count!=224)return true;
    if(!view.types||!view.saved||reinterpret_cast<std::uintptr_t>(view.types)%alignof(std::int32_t)||
       reinterpret_cast<std::uintptr_t>(view.saved)%alignof(std::int32_t)||overlaps(view.saved,224*4,bytes.data,bytes.size)){
        error="actual disjoint PROP types and saved sheet required";return false;
    }
    for(const auto* sheet:std::array<const std::int32_t*,5>{view.defaults,view.types,view.base,view.gear,view.resolved})
        if(sheet&&overlaps(view.saved,224*4,sheet,224*4)){error="PROP saved sheet aliases another authority";return false;}
    for(std::size_t i=0;i<224;++i){
        std::uint32_t bits=0;
        if(!word(bits)){error="truncated source PROP word";return false;}
        const auto type=view.types[i]==-1?16u:std::uint32_t(view.types[i]);
        if(type&0x20)std::memcpy(view.saved+i,&bits,4);
    }
    if(consumed==bytes.size){error="truncated source PROP byte194";return false;}
    properties_byte_194_=bytes.data[consumed++];return true;
}
}

namespace {
using namespace dh2::data;

std::int32_t signed_word(std::uint32_t bits) noexcept {
    std::int32_t result;
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}

bool overlaps(const void* a, std::size_t an, const void* b, std::size_t bn) {
    const auto x = reinterpret_cast<std::uintptr_t>(a);
    const auto y = reinterpret_cast<std::uintptr_t>(b);
    if (an > UINTPTR_MAX - x || bn > UINTPTR_MAX - y) return true;
    return an && bn && x < y + bn && y < x + an;
}
bool metadata_outputs_separate(const void* owner, std::size_t owner_size,
                              Bytes bytes, std::size_t& consumed,
                              std::string& error) {
    return !overlaps(&consumed, sizeof(consumed), owner, owner_size) &&
           !overlaps(&error, sizeof(error), owner, owner_size) &&
           !overlaps(&consumed, sizeof(consumed), &error, sizeof(error)) &&
           !overlaps(&consumed, sizeof(consumed), bytes.data, bytes.size) &&
           !overlaps(&error, sizeof(error), bytes.data, bytes.size);
}

bool valid(const SavedSkillsView16V1* view) noexcept {
    return view && view->reserved == 0 &&
           (view->count == 0 || view->rows != nullptr) &&
           (!view->rows || reinterpret_cast<std::uintptr_t>(view->rows) %
                               alignof(SavedSkill8V1) == 0);
}

bool read(PlayerProfileSpan24V1& profile, std::uint32_t count,
          const std::uint8_t*& bytes,
          const SavedSkillsLoadServices32V1& services) {
    if (profile.cursor > profile.size || count > profile.size - profile.cursor)
        return false;
    bytes = profile.data + profile.cursor;
    profile.cursor += count;
    if (services.read_observer) services.read_observer(services.context, count);
    return true;
}

bool read_word(PlayerProfileSpan24V1& profile, std::uint32_t& value,
               const SavedSkillsLoadServices32V1& services) {
    const std::uint8_t* bytes = nullptr;
    if (!read(profile, 4, bytes, services)) return false;
    value = std::uint32_t(bytes[0]) | (std::uint32_t(bytes[1]) << 8) |
            (std::uint32_t(bytes[2]) << 16) | (std::uint32_t(bytes[3]) << 24);
    return true;
}

bool read_string_section(Bytes bytes, std::string& output, std::size_t& used,
                         std::string& error) {
    if (!bytes.data || bytes.size < 4) {
        error = "truncated source string section";
        return false;
    }
    const auto count = std::uint32_t(bytes.data[0]) |
                       (std::uint32_t(bytes.data[1]) << 8) |
                       (std::uint32_t(bytes.data[2]) << 16) |
                       (std::uint32_t(bytes.data[3]) << 24);
    if (signed_word(count) <= 0) {
        output.clear();
        used = 4;
        error.clear();
        return true;
    }
    if (count > bytes.size - 4 || bytes.data[3 + count] != 0) {
        error = "invalid or truncated source string payload";
        return false;
    }
    output.assign(reinterpret_cast<const char*>(bytes.data + 4), count - 1);
    used = 4 + count;
    error.clear();
    return true;
}

std::int32_t skill_table_id(const SkillTables& tables,
                            const std::uint8_t* bytes,
                            std::uint32_t size) {
    std::string name(reinterpret_cast<const char*>(bytes), size);
    const char* source_name = name.c_str();
    for (std::size_t i = 0; i < tables.skills.size(); ++i) {
        if (std::strcmp(source_name, tables.skills[i].table_name.c_str()) == 0)
            return static_cast<std::int32_t>(i);
    }
    return -1;
}
}  // namespace

extern "C" int dh2_player_skills_v1_load(
    dh2::data::SavedSkillsView16V1* view,
    dh2::data::PlayerProfileSpan24V1* profile,
    const dh2::data::SavedSkillsLoadServices32V1* services) noexcept {
    using namespace dh2::data;
    if (!valid(view) || !profile || !services || !services->skill_id ||
        !services->slot_value || profile->reserved0 || profile->reserved1 ||
        profile->cursor > profile->size || (!profile->data && profile->size))
        return -1;

    try {
        std::uint32_t count = 0;
        if (!read_word(*profile, count, *services)) return -2;
        for (std::int32_t i = 0; i < signed_word(count); ++i) {
            std::uint32_t name_size = 0;
            if (!read_word(*profile, name_size, *services)) return -2;
            const std::uint8_t* name = nullptr;
            if (signed_word(name_size) <= 0) {
                name = reinterpret_cast<const std::uint8_t*>("");
                name_size = 0;
            } else {
                if (!read(*profile, name_size, name, *services)) return -2;
                if (name[name_size - 1] != 0) return -1;
            }
            const auto id = services->skill_id(services->context, name, name_size);
            SavedSkill8V1* selected = nullptr;
            for (std::uint32_t row = 0; row < view->count; ++row) {
                if (view->rows[row].id == id) {
                    selected = &view->rows[row];
                    break;
                }
            }
            const std::uint8_t* level = nullptr;
            if (!read(*profile, 2, level, *services)) return -2;
            if (selected) {
                selected->level = static_cast<std::uint16_t>(
                    level[0] | (std::uint16_t(level[1]) << 8));
            }
        }

        for (std::uint32_t set = 0; set < 2; ++set) {
            if (!read_word(*profile, count, *services)) return -2;
            for (std::int32_t i = 0; i < signed_word(count); ++i) {
                std::uint32_t key_bits = 0;
                std::uint32_t value = 0;
                if (!read_word(*profile, key_bits, *services)) return -2;
                auto* target = services->slot_value(
                    services->context, set, signed_word(key_bits));
                if (!target) return -3;
                if (!read_word(*profile, value, *services)) return -2;
                *target = value;
            }
        }
        return 0;
    } catch (...) {
        return -3;
    }
}

extern "C" int dh2_saved_skill_v1_level(
    const dh2::data::SavedSkillsView16V1* view,
    std::uint32_t index) noexcept {
    return valid(view) && index < view->count ? view->rows[index].level : -1;
}

extern "C" int dh2_saved_skill_v1_set_level(
    dh2::data::SavedSkillsView16V1* view, std::uint32_t index,
    std::int32_t level) noexcept {
    if (!valid(view) || index >= view->count) return -1;
    view->rows[index].level = static_cast<std::uint16_t>(level);
    return 0;
}

namespace dh2::data {

bool PlayerSavegameV1::initialize_skills(const SkillTables& tables,
                                         std::int32_t selector,
                                         std::string& error) {
    if (tables.skill_lists.empty()) {
        error = "SkillList table is empty";
        return false;
    }
    std::size_t row = selector < 0 ||
                              static_cast<std::size_t>(selector) >=
                                  tables.skill_lists.size()
                          ? 3
                          : static_cast<std::size_t>(selector);
    if (row >= tables.skill_lists.size()) {
        error = "source SkillList fallback row 3 is unavailable";
        return false;
    }
    return initialize_skills_from_character_list(
        tables.skill_lists[row].members, error);
}

bool PlayerSavegameV1::initialize_skills_from_character_list(
    const std::vector<std::int32_t>& ids, std::string& error) {
    if (skills_initialized_) {
        error.clear();
        return true;
    }
    if (!character_ || ids.size() > std::numeric_limits<std::uint32_t>::max()) {
        error = "missing source Character or invalid skill-list size";
        return false;
    }
    try {
        std::vector<SavedSkill8V1> next;
        next.reserve(ids.size());
        for (const auto id : ids) next.push_back({id, 0, 0, 0});
        skills_.swap(next);
        slots_[0].clear();
        slots_[1].clear();
        skills_initialized_ = true;
        error.clear();
        return true;
    } catch (...) {
        error = "saved skill allocation failed";
        return false;
    }
}

bool PlayerSavegameV1::reload_skills_from_character_list(
    const std::vector<std::int32_t>& ids, std::string& error) {
    if (!character_ || ids.size() > std::numeric_limits<std::uint32_t>::max()) {
        error = "same-Save skill reload requires its Character and a valid selected list";
        return false;
    }

    // Match the source order: delete the previous array, publish it as null,
    // allocate and fill the newly selected list, then clear slot sets 0 and 1.
    // Swapping with an empty vector releases the old row allocation while
    // preserving this PlayerSavegame owner and all of its other fields.
    std::vector<SavedSkill8V1>().swap(skills_);
    skills_initialized_ = false;
    try {
        skills_.resize(ids.size());
    } catch (...) {
        error = "same-Save skill row allocation failed";
        return false;
    }
    skills_initialized_ = true;
    for (std::size_t i = 0; i < ids.size(); ++i)
        skills_[i] = {ids[i], 0, 0, 0};
    slots_[0].clear();
    slots_[1].clear();
    error.clear();
    return true;
}

int PlayerSavegameV1::load_skills(Bytes bytes, const SkillTables& tables,
                                  std::size_t& consumed,
                                  std::string& error) {
    consumed = 0;
    if (!character_ || !skills_initialized_ || bytes.size > UINT32_MAX ||
        (!bytes.data && bytes.size)) {
        error = "missing Character, initialized skills, or valid section";
        return -1;
    }
    struct Context {
        PlayerSavegameV1* owner;
        const SkillTables* tables;
    } context{this, &tables};
    const SavedSkillsLoadServices32V1 services{
        &context,
        [](void* opaque, const std::uint8_t* name, std::uint32_t size) {
            const auto& c = *static_cast<Context*>(opaque);
            return skill_table_id(*c.tables, name, size);
        },
        [](void* opaque, std::uint32_t set, std::int32_t key) -> std::uint32_t* {
            if (set >= 2) return nullptr;
            return &static_cast<Context*>(opaque)->owner->slots_[set][key];
        },
        nullptr};
    SavedSkillsView16V1 view{skills_.data(),
                             static_cast<std::uint32_t>(skills_.size()), 0};
    PlayerProfileSpan24V1 span{bytes.data, static_cast<std::uint32_t>(bytes.size),
                               0, 0, 0};
    const auto status = dh2_player_skills_v1_load(&view, &span, &services);
    consumed = span.cursor;
    error = status ? "source Skills section failed at byte " +
                         std::to_string(consumed)
                   : "";
    return status;
}

bool PlayerSavegameV1::load_name(Bytes bytes, std::size_t& consumed,
                                 std::string& error) {
    return read_string_section(bytes, name_, consumed, error);
}

bool PlayerSavegameV1::load_level(Bytes bytes, std::size_t& consumed,
                                  std::string& error) {
    if (!bytes.data || bytes.size < 4) {
        error = "truncated source level";
        return false;
    }
    const auto value = std::uint32_t(bytes.data[0]) |
                       (std::uint32_t(bytes.data[1]) << 8) |
                       (std::uint32_t(bytes.data[2]) << 16) |
                       (std::uint32_t(bytes.data[3]) << 24);
    level_ = signed_word(value);
    consumed = 4;
    error.clear();
    return true;
}

bool PlayerSavegameV1::load_class(
    Bytes bytes, const std::vector<std::string>& names, std::size_t& consumed,
    std::string& error) {
    std::string key;
    if (!read_string_section(bytes, key, consumed, error)) return false;
    class_ = -1;
    for (std::size_t i = 0; i < names.size(); ++i) {
        if (std::strcmp(key.c_str(), names[i].c_str()) == 0) {
            class_ = static_cast<std::int32_t>(i);
            break;
        }
    }
    return true;
}

bool PlayerSavegameV1::set_skill_level(std::uint32_t row,
                                       std::int32_t level,
                                       std::string& error) {
    SavedSkillsView16V1 view{skills_.data(),
                             static_cast<std::uint32_t>(skills_.size()), 0};
    if (dh2_saved_skill_v1_set_level(&view, row, level)) {
        error = "unsafe saved skill index";
        return false;
    }
    error.clear();
    return true;
}

bool PlayerSavegameV1::load_level_name(Bytes bytes, std::size_t& consumed,
                                     std::string& error) {
    if (!metadata_outputs_separate(this, sizeof(*this), bytes, consumed, error)) return false;
    consumed = 0;
    if (bytes.size > UINT32_MAX || (!bytes.data && bytes.size)) {
        error = "invalid LNAM section";
        return false;
    }
    PlayerProfileSpan24V1 span{bytes.data, static_cast<std::uint32_t>(bytes.size), 0, 0, 0};
    SavedSkillsLoadServices32V1 services{};
    std::uint32_t value{};
    auto next = [&]() {
        if (!read_word(span, value, services)) {
            error = "truncated LNAM section";
            return false;
        }
        consumed = span.cursor;
        return true;
    };
    if (!next()) return false;
    level_name_fields_.level_id = value;
    for (std::size_t i = 0; i < 3; ++i) {
        if (!next()) return false;
        level_name_fields_.word50[i] = signed_word(value);
        if (!next()) return false;
        level_name_fields_.word5c[i] = signed_word(value);
        if (!next()) return false;
        level_name_fields_.quest_wordfc[i] = signed_word(value);
        level_name_fields_.quest_word15c[i] = signed_word(value);
    }
    level_name_loaded_ = true;
    error.clear();
    return true;
}

bool PlayerSavegameV1::load_level_entry_points(Bytes bytes, std::size_t& consumed,
                                             std::string& error) {
    if (!metadata_outputs_separate(this, sizeof(*this), bytes, consumed, error)) return false;
    consumed = 0;
    if (bytes.size > UINT32_MAX || (!bytes.data && bytes.size)) {
        error = "invalid LEPT section";
        return false;
    }
    PlayerProfileSpan24V1 span{bytes.data, static_cast<std::uint32_t>(bytes.size), 0, 0, 0};
    SavedSkillsLoadServices32V1 services{};
    for (auto& target : level_entry_points_) {
        std::uint32_t value{};
        if (!read_word(span, value, services)) {
            error = "truncated LEPT section";
            return false;
        }
        target = signed_word(value);
        consumed = span.cursor;
    }
    error.clear();
    return true;
}

bool PlayerSavegameV1::load_use_spawn_points(Bytes bytes, std::size_t& consumed,
                                          std::string& error) {
    if (!metadata_outputs_separate(this, sizeof(*this), bytes, consumed, error)) return false;
    consumed = 0;
    if (bytes.size > UINT32_MAX || (!bytes.data && bytes.size)) {
        error = "invalid LUSP section";
        return false;
    }
    PlayerProfileSpan24V1 span{bytes.data, static_cast<std::uint32_t>(bytes.size), 0, 0, 0};
    SavedSkillsLoadServices32V1 services{};
    for (auto& target : use_spawn_points_) {
        const std::uint8_t* value{};
        if (!read(span, 1, value, services)) {
            error = "truncated LUSP section";
            return false;
        }
        target = *value;
        consumed = span.cursor;
    }
    use_spawn_points_loaded_ = true;
    error.clear();
    return true;
}

bool PlayerSavegameV1::clear_use_spawn_point(std::size_t difficulty,
                                             std::string& error) {
    if (difficulty >= use_spawn_points_.size() || !use_spawn_points_loaded_) {
        error = "NativeStartGame requires a loaded LUSP row";
        return false;
    }
    use_spawn_points_[difficulty] = 0;
    error.clear();
    return true;
}

bool PlayerSavegameV1::set_skill_in_slot(
    std::int32_t slot, std::uint32_t row,
    const SavedSkillUpdateServicesV1& services, std::string& error) {
    if (!skills_initialized_ || !character_ || slot < 0 ||
        (row != UINT32_MAX && row >= skills_.size())) {
        error = "invalid source skill assignment";
        return false;
    }
    if (row == UINT32_MAX) {
        slots_[0].erase(slot);
        error.clear();
        return true;
    }
    if (!services.update_skills) {
        error = "required CharAI.UpdateSkills service unavailable";
        return false;
    }
    for (auto it = slots_[0].begin(); it != slots_[0].end();) {
        if (it->second == row)
            it = slots_[0].erase(it);
        else
            ++it;
    }
    slots_[0][slot] = row;
    return services.update_skills(services.context, character_, error);
}

std::int32_t PlayerSavegameV1::skill_id(std::uint32_t row) const noexcept {
    return row < skills_.size() ? skills_[row].id : -1;
}

std::int32_t PlayerSavegameV1::skill_level(std::uint32_t row) const noexcept {
    return row < skills_.size() ? skills_[row].level : -1;
}

std::int32_t PlayerSavegameV1::skill_in_slot(std::int32_t slot) const noexcept {
    const auto found = slots_[0].find(slot);
    return found == slots_[0].end() ? -1 : signed_word(found->second);
}

std::int32_t PlayerSavegameV1::skill_slot(std::uint32_t row) const noexcept {
    if (row >= skills_.size()) return -1;
    for (const auto& entry : slots_[0])
        if (entry.second == row) return entry.first;
    return -1;
}

void PlayerSavegameV1::initialize_faeries() noexcept {
    for (std::size_t i = 0; i < faeries_initialized_.size(); ++i) {
        if (!faeries_initialized_[i]) {
            faeries_[i] = {};
            faeries_initialized_[i] = true;
        }
    }
}

bool PlayerSavegameV1::load_current_faery(Bytes bytes, std::size_t& consumed,
                                          std::string& error) {
    consumed = 0;
    for (std::size_t i = 0; i < current_faery_.size(); ++i) {
        if (!faeries_initialized_[i]) {
            error = "source faery storage unavailable";
            return false;
        }
        if (!bytes.data || consumed > bytes.size || bytes.size - consumed < 4) {
            error = "truncated current faery";
            return false;
        }
        const auto* p = bytes.data + consumed;
        const auto bits = std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
                          (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
        current_faery_[i] = signed_word(bits);
        consumed += 4;
    }
    error.clear();
    return true;
}

bool PlayerSavegameV1::load_faeries(Bytes bytes, std::size_t& consumed,
                                    bool& mismatch, std::string& error) {
    consumed = 0;
    mismatch = false;
    if ((!bytes.data && bytes.size) || bytes.size > UINT32_MAX) {
        error = "invalid faery input";
        return false;
    }
    SavedSkillsLoadServices32V1 unused{};
    PlayerProfileSpan24V1 profile{bytes.data, static_cast<std::uint32_t>(bytes.size),
                                  0, 0, 0};
    for (std::size_t i = 0; i < faeries_.size(); ++i) {
        if (!faeries_initialized_[i]) {
            error = "source faery storage unavailable";
            consumed = profile.cursor;
            return false;
        }
        std::uint32_t id = 0, count = 0;
        if (!read_word(profile, id, unused)) {
            error = "truncated current faery";
            consumed = profile.cursor;
            return false;
        }
        current_faery_[i] = signed_word(id);
        if (!read_word(profile, count, unused)) {
            error = "truncated faery count";
            consumed = profile.cursor;
            return false;
        }
        if (count != 5) {
            mismatch = true;
            consumed = profile.cursor;
            error.clear();
            return true;
        }
        for (auto& row : faeries_[i]) {
            const std::uint8_t* value = nullptr;
            if (!read(profile, 2, value, unused)) {
                error = "truncated faery level";
                consumed = profile.cursor;
                return false;
            }
            row.level = static_cast<std::uint16_t>(value[0] | (std::uint16_t(value[1]) << 8));
            if (!read(profile, 1, value, unused)) {
                error = "truncated faery state";
                consumed = profile.cursor;
                return false;
            }
            row.state = value[0];
        }
    }
    consumed = profile.cursor;
    error.clear();
    return true;
}

bool PlayerSavegameV1::load_difficulty(
    Bytes bytes, void* context,
    bool (*store_selected)(void*, std::int32_t, std::string&),
    std::size_t& consumed, std::string& error) {
    consumed = 0;
    if (!store_selected || !bytes.data || bytes.size < 4 || bytes.size > UINT32_MAX) {
        error = "required CurrentDifficulty store or input unavailable";
        return false;
    }
    SavedSkillsLoadServices32V1 unused{};
    PlayerProfileSpan24V1 profile{bytes.data, static_cast<std::uint32_t>(bytes.size),
                                  0, 0, 0};
    std::uint32_t selected = 0;
    if (!read_word(profile, selected, unused)) return false;
    consumed = profile.cursor;
    if (!store_selected(context, signed_word(selected), error)) return false;
    std::uint32_t unlocked = 0;
    if (!read_word(profile, unlocked, unused)) {
        error = "truncated unlocked difficulty";
        return false;
    }
    unlocked_difficulty_ = signed_word(unlocked);
    consumed = profile.cursor;
    error.clear();
    return true;
}

bool PlayerSavegameV1::set_faery_level(std::uint32_t id, std::int32_t value,
                                       std::uint32_t difficulty,
                                       std::string& error) {
    if (difficulty >= 3 || id >= 5 || !faeries_initialized_[difficulty]) {
        error = "unsafe saved faery index";
        return false;
    }
    faeries_[difficulty][id].level = static_cast<std::uint16_t>(value);
    error.clear();
    return true;
}

bool PlayerSavegameV1::set_faery_state(std::uint32_t id, std::int32_t value,
                                       std::uint32_t difficulty,
                                       std::string& error) {
    if (difficulty >= 3 || id >= 5 || !faeries_initialized_[difficulty]) {
        error = "unsafe saved faery index";
        return false;
    }
    faeries_[difficulty][id].state = static_cast<std::uint8_t>(value);
    error.clear();
    return true;
}

bool PlayerSavegameV1::set_current_faery(std::uint32_t id,
                                         std::uint32_t difficulty,
                                         std::string& error) {
    if (difficulty >= current_faery_.size() ||
        !faeries_initialized_[difficulty]) {
        error = "source current-faery Save storage unavailable";
        return false;
    }
    if (id >= faeries_[difficulty].size()) {
        error = "source current-faery index exceeds the initialized Save row count";
        return false;
    }
    current_faery_[difficulty] = static_cast<std::int32_t>(id);
    error.clear();
    return true;
}

std::int32_t PlayerSavegameV1::faery_level(std::uint32_t id,
                                           std::uint32_t difficulty) const noexcept {
    return difficulty < 3 && id < 5 && faeries_initialized_[difficulty]
               ? faeries_[difficulty][id].level
               : 0;
}

std::int32_t PlayerSavegameV1::current_faery(std::uint32_t difficulty) const noexcept {
    return difficulty < 3 ? current_faery_[difficulty] : -1;
}

}  // namespace dh2::data

namespace dh2::player_saved_skill_callbacks_v1 {
namespace {
using data::IntegerListRow;

const IntegerListRow* selected_skill_list(const data::PropertyView* properties,
                                          const data::SkillTables* tables) noexcept {
    if (!properties || !properties->resolved || !tables ||
        tables->skill_lists.empty())
        return nullptr;
    const auto selector = properties->resolved[28];
    const auto row = selector < 0 ||
                             static_cast<std::size_t>(selector) >=
                                 tables->skill_lists.size()
                         ? std::size_t{3}
                         : static_cast<std::size_t>(selector);
    return row < tables->skill_lists.size() ? &tables->skill_lists[row] : nullptr;
}

Result missing() noexcept {
    return {Disposition::missing_projection, 0};
}
}  // namespace

Result get_skill_id_from_oid(const data::PropertyView* properties,
                             const data::SkillTables* tables,
                             std::int32_t object_id) noexcept {
    const auto* list = selected_skill_list(properties, tables);
    if (!list) return missing();
    for (std::size_t index = 0; index < list->members.size(); ++index) {
        if (list->members[index] == object_id)
            return {Disposition::append_integer,
                    static_cast<std::int32_t>(index)};
    }
    return {Disposition::append_integer, -1};
}

Result get_current_skill_info(const data::PropertyView* properties,
                              const data::SkillTables* tables,
                              const data::PlayerSavegameV1* savegame,
                              std::uintptr_t character_identity,
                              std::int32_t skill_list_index) noexcept {
    const auto* list = selected_skill_list(properties, tables);
    if (!list) return missing();
    if (skill_list_index < 0 ||
        static_cast<std::size_t>(skill_list_index) >= list->members.size())
        return {Disposition::no_return, 0};

    // Source GetCharSkill validates the selected SkillList member against the
    // actual Skill table before the later Savegame SG_GetSkillLevel call.
    const auto skill_table_id = list->members[static_cast<std::size_t>(skill_list_index)];
    if (skill_table_id < 0 ||
        static_cast<std::size_t>(skill_table_id) >= tables->skills.size())
        return {Disposition::source_assertion_boundary, 0};
    if (!savegame)
        return {Disposition::append_integer, -1};
    // The native source reads this owner from Character+0x14e8. The borrowed
    // adapter receives it separately, so refuse a save object bound to a
    // different Character before reading its row.
    if (!character_identity || savegame->character() != character_identity)
        return {Disposition::owner_mismatch, 0};
    if (!savegame->skills_initialized() ||
        static_cast<std::size_t>(skill_list_index) >= savegame->skills().size())
        return {Disposition::source_assertion_boundary, 0};
    return {Disposition::append_integer,
            savegame->skill_level(static_cast<std::uint32_t>(skill_list_index))};
}
}  // namespace dh2::player_saved_skill_callbacks_v1
