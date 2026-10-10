#pragma once
#include "character_menu.hpp"
#include "../../../game-data/data.hpp"
#include <memory>
#include "../../../engine-ui/hud_text_v1.hpp"
namespace dh::foundation { class AssetCatalog; }
namespace dh::foundation::character_menu {
struct MenuFontDefinition {
    std::uint32_t source_id=0;const char* name=nullptr;const char* original_ttf_uri=nullptr;
    bool bold=false,italic=false,define_font3=false,has_layout=false;
    // Actual source font constructor/read fields, in native font units.
    float descent=0,leading=0;
};
const MenuFontDefinition* original_menu_font(std::uint32_t source_id) noexcept;
// Exact source plain-text FIRST-line offset and align_line arithmetic, using
// actual run advance in local source pixels. Uses actual font definition's
// constructor/read metrics, not image glyph bounds or guessed halfheight.
// Output is authored480x320 baseline after field.matrix. Root transforms glyph
// offsets by matrix linear part then applies Frame.transform. Multiline/wrap/
// HTML/kerning remain actual glyph/formatter owner; no partial fallback.
bool layout_menu_text(const MenuTextField&,float actual_run_advance,
                      std::array<float,2>& baseline,std::string& error);
// Same algorithm for explicitly borrowed current font metrics (native font
// units) and ACTUAL source root scale word. Require matching source identity.
bool layout_menu_text(const MenuTextField&,float actual_run_advance,const MenuFontDefinition&,
                      float actual_root_scale_word,std::array<float,2>& baseline,std::string& error);
// Actual source AS NativeGetStringFromSymbol assignments. Empty means no proved
// static label assignment; never replaces source placeholders with guesses.
const char* original_menu_label_symbol(const std::string& source_field_path) noexcept;
class MenuLocalization {
    struct Impl;std::unique_ptr<Impl> impl_;
public:
    MenuLocalization();~MenuLocalization();
    MenuLocalization(MenuLocalization&&)noexcept;MenuLocalization& operator=(MenuLocalization&&)noexcept;
    // Actual shared common_text arrays, common/font constants and packed corpus.
    // Owns only localization cache, no duplicated character/inventory authority.
    bool load(const AssetCatalog&,const std::string& data_root,std::int32_t language_pack,std::string& error);
    bool symbol(const std::string& source_symbol,const CharacterState* actual_profile,std::string& value,std::string& error);
    // Native class header format: localized class + " " + localized
    // GAMEPLAYMENUS_LEVEL + " " + the same saved profile's level.
    bool class_level(const std::string& localized_class,const CharacterState* actual_profile,
                     std::string& value,std::string& error);
    // Native Character::GetClassName path: SG_GetPlayerClass indexes the same
    // 900-byte CharacterTable row; CharProperties copies its 896 data bytes
    // from row+4, so native row+24 is CharacterTable field[5] ClassString.
    bool character_class_level(const dh2::data::CharacterTable& source_characters,
                     std::int32_t source_class_id,const CharacterState* actual_profile,
                     std::string& value,std::string& error);
    bool string_id(std::int32_t actual_source_oid,std::string& value,std::string& error);
    bool bind_profile(const CharacterState* actual_profile,std::string& error);
    // Borrow THIS exact source StringManager cache and services to construct
    // ItemTextOwnerV5; lifetime ends on successful reload/destruction. Application
    // version/title endpoints stay missing unless their genuine owner binds them.
    bool borrow_text(dh2::ui::HudTextV1*& same_text,dh2::ui::HudTextEnvironmentV1&,
                     std::string& error);
    // Suitable for Bindings.text: unrecognized path returns empty success;
    // recognized source label requires actual corpus lookup (missing rejects).
    bool label(const std::string& source_field_path,const CharacterState* actual_profile,std::string& value,std::string& error);
};
}
