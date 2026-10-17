#include "scrolling_combat_text_owner_v1.hpp"

#include <utility>

namespace dh2::ui {
namespace {
using Status = ScrollingCombatTextStatusV1;
using Result = ScrollingCombatTextResultV1;

Status fail(Result& out, Status status, std::string& error,
            const char* fallback) {
    out.status = status;
    if (error.empty()) error = fallback;
    return status;
}

bool source_style(const ScrollingCombatTextServicesV1& s, const char* name,
                  std::int32_t& id, Result& out, std::string& error) {
    if (!s.style_id) {
        error = "Source FlashAnimManager style lookup unavailable";
        out.status = Status::service_unavailable;
        return false;
    }
    if (!s.style_id(s.context, name, id, error)) {
        if (error.empty()) error = "Source FlashAnimManager style lookup failed";
        out.status = Status::service_failed;
        return false;
    }
    out.last_style_id = id;
    return true;
}

bool source_constant(const ScrollingCombatTextServicesV1& s, const char* group,
                     const char* name, std::int32_t& value, Result& out,
                     std::string& error) {
    if (!s.constant) {
        error = "Source PyDataConstants lookup unavailable";
        out.status = Status::service_unavailable;
        return false;
    }
    if (!s.constant(s.context, group, name, value, error)) {
        if (error.empty()) error = "Source PyDataConstants lookup failed";
        out.status = Status::service_failed;
        return false;
    }
    return true;
}

bool localized(const ScrollingCombatTextServicesV1& s, const char* symbol,
               std::string& value, Result& out, std::string& error) {
    std::int32_t id{};
    if (!source_constant(s, "StrID", symbol, id, out, error)) return false;
    if (!s.localized_string) {
        error = "Retained StringManager lookup unavailable";
        out.status = Status::service_unavailable;
        return false;
    }
    if (!s.localized_string(s.context, id, value, error)) {
        if (error.empty()) error = "Retained StringManager lookup failed";
        out.status = Status::service_failed;
        return false;
    }
    return true;
}

bool text_event(const ScrollingCombatTextServicesV1& s, const char* style,
                const char* symbol, const char* color_name, const float xyz[3],
                Result& out, std::string& error) {
    std::int32_t style_id{}, color{};
    std::string value;
    if (!source_style(s, style, style_id, out, error) ||
        !localized(s, symbol, value, out, error) ||
        !source_constant(s, "ScrollingCombatText", color_name, color, out,
                         error)) return false;
    if (!s.play_authored_text && !s.play_text) {
        error = "Source FlashAnimManager text playback unavailable";
        out.status = Status::service_unavailable;
        return false;
    }
    const bool played=s.play_authored_text
        ? s.play_authored_text(s.context,style,xyz,value.c_str(),color,error)
        : s.play_text(s.context,style_id,xyz,value.c_str(),color,error);
    if (!played) {
        if (error.empty()) error = "Source FlashAnimManager text playback failed";
        out.status = Status::service_failed;
        return false;
    }
    ++out.emitted;
    out.last_color = color;
    return true;
}

bool status_duration(const ScrollingCombatTextServicesV1& s,
                     std::uintptr_t source, std::int32_t property,
                     bool& active, Result& out, std::string& error) {
    if (!s.source_property) {
        error = "Source Character status property unavailable";
        out.status = Status::service_unavailable;
        return false;
    }
    std::int32_t fixed{};
    if (!s.source_property(s.context, source, property, fixed, error)) {
        if (error.empty()) error = "Source Character status property failed";
        out.status = Status::service_failed;
        return false;
    }
    // Source _GetProperty(..., property) >> 8, interpreted as a truth test.
    active = (fixed >> 8) != 0;
    return true;
}

bool numeric_event(const ScrollingCombatTextServicesV1& s, const char* style,
                   const char* color_name, const float xyz[3],
                   std::int32_t value, Result& out, std::string& error) {
    std::int32_t style_id{}, color{};
    if (!source_style(s, style, style_id, out, error) ||
        !source_constant(s, "ScrollingCombatText", color_name, color, out,
                         error)) return false;
    if (!s.play_authored_value && !s.play_value) {
        error = "Source FlashAnimManager numeric playback unavailable";
        out.status = Status::service_unavailable;
        return false;
    }
    const bool played=s.play_authored_value
        ? s.play_authored_value(s.context,style,xyz,value,color,error)
        : s.play_value(s.context,style_id,xyz,value,color,error);
    if (!played) {
        if (error.empty()) error = "Source FlashAnimManager numeric playback failed";
        out.status = Status::service_failed;
        return false;
    }
    ++out.emitted;
    out.last_color = color;
    out.last_value = value;
    return true;
}
} // namespace

Status apply_scrolling_combat_text_v1(
    const data::CombatResult& result, std::uintptr_t receiver,
    std::uintptr_t source_character,
    const ScrollingCombatTextServicesV1& s, Result& output,
    std::string& error) {
    error.clear();
    Result out{};
    if (!receiver || !source_character || receiver == source_character) {
        output = out;
        return fail(output, Status::invalid_argument, error,
                    "Combat-text Character identities are invalid");
    }

    if (!s.is_follower) {
        output = out;
        return fail(output, Status::service_unavailable, error,
                    "Source Character follower predicate unavailable");
    }
    bool follower = false;
    ++out.source_checks;
    if (!s.is_follower(s.context, receiver, follower, error)) {
        output = out;
        return fail(output, Status::service_failed, error,
                    "Source Character follower predicate failed");
    }
    if (follower) {
        output = out;
        return Status::complete;
    }

    if (!s.position) {
        output = out;
        return fail(output, Status::service_unavailable, error,
                    "Source Character target-position provider unavailable");
    }
    float xyz[3]{};
    if (!s.position(s.context, receiver, xyz, error)) {
        output = out;
        return fail(output, Status::service_failed, error,
                    "Source Character target-position provider failed");
    }

    if (result.outcomes & 1u) {
        if (!text_event(s, "anim_sct_block", "INGAME_ATTACK_MISS", "MissColor",
                        xyz, out, error)) {
            output = out;
            return out.status;
        }
        output = out;
        return Status::complete;
    }
    if (result.outcomes & 2u) {
        if (!text_event(s, "anim_sct_block", "INGAME_ATTACK_DODGE", "DodgeColor",
                        xyz, out, error)) {
            output = out;
            return out.status;
        }
        output = out;
        return Status::complete;
    }
    if (result.outcomes & 4u) {
        if (!text_event(s, "anim_sct_block", "INGAME_ATTACK_BLOCK", "BlockColor",
                        xyz, out, error)) {
            output = out;
            return out.status;
        }
    }

    // Fear uses property 143 or 187, selected by source mask bit 16.
    if (result.outcomes & 0x20u) {
        bool active = false;
        if (!status_duration(s, source_character,
                             (result.mask & 0x10000u) ? 187 : 143,
                             active, out, error)) {
            output = out;
            return out.status;
        }
        if (active && !text_event(s, "anim_sct_stun", "INGAME_ATTACK_FEAR",
                                  "FearColor", xyz, out, error)) {
            output = out;
            return out.status;
        }
    }
    // Slow uses property 146 or 189, selected by source mask bit 16.
    if (result.outcomes & 0x100u) {
        bool active = false;
        if (!status_duration(s, source_character,
                             (result.mask & 0x10000u) ? 189 : 146,
                             active, out, error)) {
            output = out;
            return out.status;
        }
        if (active && !text_event(s, "anim_sct_stun", "INGAME_ATTACK_SLOW",
                                  "SlowColor", xyz, out, error)) {
            output = out;
            return out.status;
        }
    }

    if (result.amount > 0) {
        const bool dot = (result.mask & 0x20000000u) != 0;
        bool dual = false;
        if (!dot) {
            if (!s.is_dual_wielding) {
                output = out;
                return fail(output, Status::service_unavailable, error,
                            "Source ItemInventory dual-wield query unavailable");
            }
            ++out.source_checks;
            if (!s.is_dual_wielding(s.context, source_character, dual, error)) {
                output = out;
                return fail(output, Status::service_failed, error,
                            "Source ItemInventory dual-wield query failed");
            }
        }
        const bool critical = (result.outcomes & 8u) != 0;
        const bool offhand = (result.mask & 0x04000000u) != 0;
        const char* style = dot ? "anim_sct_dot" :
            dual ? (offhand ? (critical ? "anim_sct_critleft"
                                       : "anim_sct_normaldamageleft")
                            : (critical ? "anim_sct_critright"
                                       : "anim_sct_normaldamageright"))
                 : (critical ? "anim_sct_crit" : "anim_sct_normaldamage");
        if (!s.is_local_player) {
            output = out;
            return fail(output, Status::service_unavailable, error,
                        "Source PlayerManager local-player query unavailable");
        }
        bool local_player = false;
        ++out.source_checks;
        if (!s.is_local_player(s.context, receiver, local_player, error)) {
            output = out;
            return fail(output, Status::service_failed, error,
                        "Source PlayerManager local-player query failed");
        }
        const char* color = local_player
            ? (critical ? "PlayerCritDamageColor" : "PlayerDamageColor")
            : (critical ? "CritDamageColor" : "DamageColor");
        if (!numeric_event(s, style, color, xyz, result.amount >> 8, out, error)) {
            output = out;
            return out.status;
        }
    }

    output = out;
    return Status::complete;
}

Status apply_scrolling_combat_xp_v1(
    std::uintptr_t receiver,std::int32_t displayed_xp,
    const ScrollingCombatTextServicesV1& s,Result& output,std::string& error){
    error.clear();Result out{};
    if(!receiver||displayed_xp<0){output=out;return fail(output,Status::invalid_argument,error,
        "Scrolling XP receiver/value is invalid");}
    std::int32_t style_id{},color{},string_id{};
    if(!source_style(s,"anim_sct_xp",style_id,out,error)||
       !source_constant(s,"ScrollingCombatText","XPColor",color,out,error)){
        output=out;return out.status;
    }
    if(!s.position){output=out;return fail(output,Status::service_unavailable,error,
        "Source Character target-position provider unavailable");}
    float xyz[3]{};
    if(!s.position(s.context,receiver,xyz,error)){
        output=out;return fail(output,Status::service_failed,error,
            "Source Character target-position provider failed");
    }
    if(!source_constant(s,"StrID","GAMEPLAYMENUS_REWARD_XP",string_id,out,error)){
        output=out;return out.status;
    }
    if(!s.localized_formatted_string){output=out;return fail(output,Status::service_unavailable,error,
        "Retained StringManager formatted-string provider unavailable");}
    std::string text;
    if(!s.localized_formatted_string(s.context,string_id,displayed_xp,text,error)){
        output=out;return fail(output,Status::service_failed,error,
            "Retained StringManager formatted-string lookup failed");
    }
    if(!s.play_authored_text&&!s.play_text){output=out;return fail(output,Status::service_unavailable,error,
        "Source FlashAnimManager XP text playback unavailable");}
    const bool played=s.play_authored_text
        ? s.play_authored_text(s.context,"anim_sct_xp",xyz,text.c_str(),color,error)
        : s.play_text(s.context,style_id,xyz,text.c_str(),color,error);
    if(!played){output=out;return fail(output,Status::service_failed,error,
        "Source FlashAnimManager XP text playback failed");}
    out.emitted=1;out.last_style_id=style_id;out.last_color=color;out.last_value=displayed_xp;
    output=out;return Status::complete;
}

} // namespace dh2::ui
