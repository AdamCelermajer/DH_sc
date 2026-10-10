#pragma once
#include "creation_adapter.hpp"

namespace dh::foundation::frontend::creation {
// FontTextColors.zero..nine decoded from original fonts_pycst.bin. RGB words,
// not RGBA; source '^r' closes the current font and restores enclosing style.
const std::array<std::uint32_t, 10>& source_font_colors() noexcept;
struct SourceLocalizedHtml {
    std::string html;
    std::string error;
    bool ok() const noexcept { return error.empty(); }
};
// Bounded original color-control projection for trusted cache/author HTML.
// Preserves existing paragraph/FONT tags. No varargs or language-space pass.
SourceLocalizedHtml source_localized_html(std::string_view raw);
struct DynamicTextBinding {
    // Match against art::TextField.path; preserve that field's source font,
    // height, color, alignment and geometry when rendering this value.
    std::string field_path;
    std::string source_text;
    std::string html_text;
};
struct DynamicTextResult {
    std::vector<DynamicTextBinding> fields;
    std::string error;
    bool ok() const noexcept { return error.empty(); }
};
DynamicTextResult class_text_bindings(std::string_view class_token);

struct SavedProfilePresentation {
    // Must be projected from the same saved profile/campaign owner as state.
    // Class label comes from CharacterTable StrID; it may be a specialization.
    std::string character_id;
    std::string class_token;
    std::string class_label;
    unsigned current_act;
    std::string localized_location;
    unsigned difficulty;
    std::string formatted_save_date;
    // Optional metadata must be source-backed. False means the saved format or
    // provider did not expose it; the projection then omits that UI field.
    bool current_act_known=false;
    bool difficulty_known=false;
};
using SavedProfilePresentationService = std::function<std::optional<SavedProfilePresentation>(
    const CharacterState&, std::string&)>;
// Reads name and level directly from SAME state; metadata cannot override them.
// No profile service means explicit failure and no bindings/placeholders.
DynamicTextResult saved_profile_text_bindings(const CharacterState&,
                                             const SavedProfilePresentationService& = {});
}
