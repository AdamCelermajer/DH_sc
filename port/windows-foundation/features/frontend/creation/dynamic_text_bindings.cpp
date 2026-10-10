#include "dynamic_text_bindings.hpp"
#include <exception>
#include <cstring>
#include <utility>

namespace dh::foundation::frontend::creation {
const std::array<std::uint32_t, 10>& source_font_colors() noexcept {
    static constexpr std::array<std::uint32_t, 10> colors{{
        16777215, 10289050, 10149631, 13933311, 16771226,
        65535, 16711680, 6710886, 3355443, 11184810
    }};
    return colors;
}
SourceLocalizedHtml source_localized_html(std::string_view raw) {
    SourceLocalizedHtml output;
    if (raw.size() > 65536) { output.error = "Source label exceeds bounded 65536-byte domain"; return output; }
    bool escape = false;
    constexpr const char* hex = "0123456789ABCDEF";
    for (unsigned char c : raw) {
        if (!escape) {
            if (c == '^') escape = true;
            else output.html += c == '|' ? '\x11' : static_cast<char>(c);
            continue;
        }
        escape = false;
        if (c >= '0' && c <= '9') {
            const auto value = source_font_colors()[c-'0'];
            output.html += "<font color=\"#";
            for (int shift = 20; shift >= 0; shift -= 4) output.html += hex[(value >> shift) & 15];
            output.html += "\">";
        } else if (c == 'n') output.html += '\n';
        else if (c == 'r') output.html += "</font>";
        else if (std::strchr("#*^dfghikpstv", c)) { output.html += '^'; output.html += static_cast<char>(c); }
        // Source drops unknown controls and a dangling trailing caret.
    }
    return output;
}
namespace {
std::string html_escape(std::string_view text) {
    std::string result;
    for (char c : text) {
        if (c == '&') result += "&amp;";
        else if (c == '<') result += "&lt;";
        else if (c == '>') result += "&gt;";
        else if (c == '\n') result += "<br>";
        else result += c;
    }
    return result;
}
std::string class_html(std::string_view text) {
    // Actual source FontTextColors.two = 10149631 = 0x9ADEFF.
    // This bounded projection handles only controls present in these strings.
    return source_localized_html(html_escape(text)).html;
}
DynamicTextBinding plain(std::string path, std::string value) {
    return {std::move(path), value, html_escape(value)};
}
}
DynamicTextResult class_text_bindings(std::string_view token) {
    DynamicTextResult result;
    const auto* choice = find_class(token);
    if (!choice) { result.error = "Original base-class text binding unavailable"; return result; }
    // Recovered menu.english translations, selected by native Update 0x428498.
    static constexpr const char* titles[] = {"Warrior", "Rogue", "Mage"};
    static constexpr const char* descriptions[] = {
        "The Warrior relies on his strength and endurance to survive.\nUpgrades to ^2Berserker^r or ^2Crusader^r.",
        "A finesse fighter, the Rogue favors speed and precision over power. Upgrades to ^2Death Walker^r or ^2Archer^r.",
        "Learned in forbidden arts, the Mage uses both Faerie and Dark Energy. Upgrades to ^2Shadowmancer^r or ^2Illusionist^r."
    };
    result.fields.push_back(plain("menu_SelectClass/class_title/text", titles[choice->menu_index]));
    const std::string description = descriptions[choice->menu_index];
    result.fields.push_back({"menu_SelectClass/class_description/text", description, class_html(description)});
    result.fields.push_back(plain("menu_SelectClass/btn_Confirm/text", "Confirm"));
    result.fields.push_back(plain("menu_SelectClass/MENU_MENUTITLE_CHOOSE_CLASS/text", "Choose a class"));
    return result;
}
DynamicTextResult saved_profile_text_bindings(const CharacterState& state,
                                             const SavedProfilePresentationService& service) {
    DynamicTextResult result;
    auto validation = validate_character_state(state);
    if (!validation.ok()) { result.error = validation.errors.front(); return result; }
    if (!service) { result.error = "Saved-profile campaign presentation service unavailable"; return result; }
    std::optional<SavedProfilePresentation> presentation;
    try { presentation = service(state, result.error); }
    catch (const std::exception& e) { result.error = e.what(); }
    catch (...) { result.error = "Saved-profile presentation service failed"; }
    if (!presentation || !result.error.empty()) {
        if (result.error.empty()) result.error = "Saved-profile presentation service produced no projection";
        return result;
    }
    const auto& p = *presentation;
    if (p.character_id != state.id || p.class_token != state.class_id) {
        result.error = "Saved-profile presentation does not belong to SAME CharacterState"; return result;
    }
    // Literal paths are the original SWF first-frame text receivers.
    const std::string prefix = "menu_MainMenu/PlayerInfos/";
    result.fields.push_back(plain("menu_MainMenu/SlotText/text", state.name));
    std::string class_label=p.class_label;
    if(class_label.empty()){
        // Base class names are source-authored translations. Specialization
        // labels require the actual CharacterTable StrID projection above.
        const auto base=class_text_bindings(state.class_id);
        if(base.ok()&&!base.fields.empty())class_label=base.fields.front().source_text;
    }
    if(class_label.empty()){
        result.error="Saved-profile class label is unavailable for the selected source class";
        return result;
    }
    result.fields.push_back(plain(prefix + "player_class/text", class_label));
    result.fields.push_back(plain(prefix + "Hud_Level/text", "LEVEL " + std::to_string(state.stats.level)));
    // Emit the exact SWF text receivers even when this portable save schema
    // has no source fact for them. Empty text clears the timeline's static
    // Warrior placeholder instead of presenting it as selected-profile data.
    result.fields.push_back(plain(prefix + "Hud_Act/text", p.current_act_known ? "Act " + std::to_string(p.current_act) : ""));
    result.fields.push_back(plain(prefix + "Hud_Location/text", p.localized_location));
    if(p.difficulty_known&&p.difficulty>2){result.error="Saved-profile difficulty is outside the original three modes";result.fields.clear();return result;}
    result.fields.push_back(plain(prefix + "Last_Save/text", p.formatted_save_date.empty() ? "" : "Last Save"));
    result.fields.push_back(plain(prefix + "Last_Save_Infos/text", p.formatted_save_date));
    static constexpr const char* difficulties[] = {"Normal", "Hard", "Heroic"};
    result.fields.push_back(plain(prefix + "DifficultyTitle/text", p.difficulty_known ? "Difficulty:" : ""));
    result.fields.push_back(plain(prefix + "Difficulty/text", p.difficulty_known ? difficulties[p.difficulty] : ""));
    return result;
}
}
