#include "player_level_transition_save_v1.hpp"

namespace dh2::player_level_transition_save_v1 {
namespace {
bool missing(const char* name, std::string& error) {
    error = std::string("SG_SavePlayer provider unavailable: ") + name;
    return false;
}
}

Owner::Owner(data::PlayerSavegameV1& save, std::uintptr_t character,
    std::uintptr_t level, Services services)
    : save_(save), character_(character), level_(level), services_(services) {}

bool Owner::save_player(bool force_unblock, std::string& error) {
    error.clear();
    if (active_) { error="SG_SavePlayer is already active"; return false; }
    active_=true;
    struct Reset { bool& value; ~Reset(){value=false;} } reset{active_};
    phase_=Phase::active_check;
    if (!character_ || !level_ || save_.character()!=character_) {
        error="SG_SavePlayer requires the same live Character, Level, and canonical Save";
        return false;
    }
    if (!services_.character_active) return missing("Character vtable +0x28", error);
    bool active_character=false;
    if (!services_.character_active(services_.context,character_,active_character,error)) return false;
    if (!active_character) { phase_=Phase::complete; return true; }

    const bool was_blocked=save_.source_save_blocked();
    if (force_unblock) {
        phase_=Phase::unblock;
        save_.set_source_save_blocked(false);
    }
    struct Restore {
        data::PlayerSavegameV1& save;
        bool force, prior;
        Phase& phase;
        ~Restore(){if(force){save.set_source_save_blocked(prior);if(phase!=Phase::complete)phase=Phase::restore_block;}}
    } restore{save_,force_unblock,was_blocked,phase_};

    std::int32_t level=0;
    phase_=Phase::character_level;
    if (!services_.character_level) return missing("Character::GetLevel", error);
    if (!services_.character_level(services_.context,character_,level,error)) return false;
    save_.set_player_level(level);

    std::uint32_t date=0;
    phase_=Phase::save_date;
    if (!services_.unix_time_seconds) return missing("PlayerSavegame::SG_SetSaveDate time(NULL)", error);
    if (!services_.unix_time_seconds(services_.context,date,error)) return false;
    save_.set_save_date(date);

    std::int32_t entry=0,difficulty=0;
    phase_=Phase::entry_point;
    if (!services_.current_level_entry_point)
        return missing("Level+0x110 entry point", error);
    if (!services_.current_level_entry_point(services_.context,level_,entry,error)) return false;
    if (!services_.active_difficulty) return missing("PlayerSavegame::m_difficultyLevel", error);
    if (!services_.active_difficulty(services_.context,difficulty,error)) return false;
    if (difficulty<0 || difficulty>=3 || !save_.set_level_entry_point(
            static_cast<std::uint32_t>(difficulty),entry)) {
        error="SG_SetLevelEntryPoint difficulty is outside the three source rows";
        return false;
    }

    phase_=Phase::persist;
    if (!services_.save_gameplay) return missing("canonical PlayerSavegame::SG_Save writer", error);
    if (!services_.save_gameplay(services_.context,save_,error)) return false;
    phase_=Phase::complete;
    return true;
}

} // namespace dh2::player_level_transition_save_v1
