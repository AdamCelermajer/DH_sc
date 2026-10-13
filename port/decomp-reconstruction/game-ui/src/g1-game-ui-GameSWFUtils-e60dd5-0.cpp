// GameSWFUtils bodies, batch g1-game-ui-GameSWFUtils-e60dd5-0 (18 functions, 0x4163c8 to 0x41724c).
// Rebuilt from the Hex-Rays pseudocode of libDungeonHunter2.so and checked against the ARM assembly
// (tools/decomp_asm.py) where the pseudocode mistypes an argument. Offsets in comments are the original
// 32-bit ARM offsets.
//
// Calling convention: every GameSWFUtils function is static. IDA adds an implicit `this` to the demangled
// symbol, so the pseudocode calls the first explicit argument `this` and shifts the others (a2 is the second
// explicit argument, a3 the third). The assembly shows the static form: r0 is the first explicit argument
// (the character, the root or the MenuFX), r1 the second, r2 the third. Where the pseudocode types a register
// as a pointer but the code uses it as a number, the code's use is what is rebuilt.
//
// Virtual calls on gameswf::character use the binary's own vtable (tools/decomp_vtable.py,
// _ZTVN7gameswf9characterE): +0x008 is(int), +0x020 get_member, +0x094 set_play_state, +0x13c get_frame_count,
// +0x14c goto_frame. The class id 2 passed to is() is AS_SPRITE in the upstream enum. The id 32 used by the text
// setters and PreloadGlyph is not named in this build (upstream r1714 calls it AS_TEXTFORMAT).

#include <array>

#include "core-platform/debug_out.hpp"
#include "dh2/GameSWFUtils.hpp"
#include "dh2/MenuFX.hpp"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_value.h"

namespace {

// Class id the original passes to character::is() in SetTextHeight, SetLineSpacing and PreloadGlyph.
constexpr int kTextClassId = 32;

}  // namespace

// 0x4163c8 _ZN12GameSWFUtils13SetTextHeightEPN7gameswf9characterEi
// PARTIAL: the class check is rebuilt: is(32) through vtable +0x08, and a failed check returns 0 as the original
// does. The store of (float)height * 20.0 at +0x174 is left out, because the object that holds that float is not
// declared (layout request: "gameswf character +0x174 float, text height in twips"). Returns 1 after a passing
// check, as the original does.
int GameSWFUtils::SetTextHeight(gameswf::character* character, int height) {
    if (!character->is(kTextClassId)) {
        return 0;
    }
    (void)height;  // PARTIAL: the store to +0x174 (height * 20.0) is not made, see above.
    return 1;
}

// 0x41640c _ZN12GameSWFUtils14SetLineSpacingEPN7gameswf9characterEi
// PARTIAL: same as SetTextHeight with the store of (float)spacing * 20.0 at +0x160 left out (layout request:
// "gameswf character +0x160 float, line spacing in twips"). The class check and the return values are rebuilt.
int GameSWFUtils::SetLineSpacing(gameswf::character* character, int spacing) {
    if (!character->is(kTextClassId)) {
        return 0;
    }
    (void)spacing;  // PARTIAL: the store to +0x160 (spacing * 20.0) is not made, see above.
    return 1;
}

// 0x416450 _ZN12GameSWFUtils9GotoFrameEPN7gameswf9characterEi
// Sprite check, then get_frame_count (+0x13c, the result is not used), goto_frame(frame) (+0x14c) and
// set_play_state(1) (+0x94). Play state 1 is STOP in the upstream enum (PLAY = 0, STOP = 1), so the frame is shown
// and stopped, as gotoAndStop does.
int GameSWFUtils::GotoFrame(gameswf::character* character, int frame) {
    if (!character || !character->is(gameswf::AS_SPRITE)) {
        return 0;
    }
    character->get_frame_count();
    character->goto_frame(frame);
    character->set_play_state(gameswf::character::STOP);
    return 1;
}

// 0x4164c0 _ZN12GameSWFUtils14GetPixelScaleXEPN7gameswf4rootE
// PARTIAL: the original returns root +0x2c (m_display_width) divided by the movie's frame width in pixels,
// (movie +0xb8 - movie +0xb4) / 20, where movie is root +0x0c (m_def). The movie_def_impl layout is not declared in
// this tree (layout request: "movie_def_impl +0xb4 rect, x_min at +0xb4, x_max at +0xb8"). Returns 0.
float GameSWFUtils::GetPixelScaleX(gameswf::root* root) {
    (void)root;
    return 0.0f;  // PARTIAL: movie frame width not declared, see above.
}

// 0x4164fc _ZN12GameSWFUtils14GetPixelScaleYEPN7gameswf4rootE
// PARTIAL: root +0x30 (m_display_height) over (movie +0xc0 - movie +0xbc) / 20, same layout request as
// GetPixelScaleX (y_min at +0xbc, y_max at +0xc0). Returns 0.
float GameSWFUtils::GetPixelScaleY(gameswf::root* root) {
    (void)root;
    return 0.0f;  // PARTIAL: movie frame height not declared, see above.
}

// 0x416538 _ZN12GameSWFUtils17GetInvPixelScaleXEPN7gameswf4rootE
// PARTIAL: the reciprocal of GetPixelScaleX, ((movie +0xb8 - movie +0xb4) / 20) / root +0x2c, with the same layout
// request. Returns 0.
float GameSWFUtils::GetInvPixelScaleX(gameswf::root* root) {
    (void)root;
    return 0.0f;  // PARTIAL: movie frame width not declared, see GetPixelScaleX.
}

// 0x416578 _ZN12GameSWFUtils17GetInvPixelScaleYEPN7gameswf4rootE
// PARTIAL: the reciprocal of GetPixelScaleY, with the same layout request. Returns 0.
float GameSWFUtils::GetInvPixelScaleY(gameswf::root* root) {
    (void)root;
    return 0.0f;  // PARTIAL: movie frame height not declared, see GetPixelScaleY.
}

// 0x41684c _ZN12GameSWFUtils10MoveBtnPosEPN7gameswf9characterEff
// PARTIAL: the original builds the inverse of the parent's world matrix (matrix::set_inverse), concatenates the
// character's own world matrix with the translation (x * 20, y * 20) (out-of-range values become 0), and calls
// RenderFX::SetPosition(nullptr, character, x / 20, y / 20). Not rebuilt: character::get_world_matrix returns a
// pointer in the binary, while the upstream declaration returns by value, and RenderFX::SetPosition is not declared.
// Only the null check is made. Returns 1 for a non-null character, as the original does on this path.
int GameSWFUtils::MoveBtnPos(gameswf::character* character, float x, float y) {
    if (!character) {
        return 0;
    }
    (void)x;  // PARTIAL: matrix work and RenderFX::SetPosition left out, see above.
    (void)y;
    return 1;
}

// 0x4169e8 _ZN12GameSWFUtils19GetAbsolutePositionEPN7gameswf9characterE
// PARTIAL: the original starts from the character's translation (the matrix pointer at +0x4c, its values at +0x08
// and +0x14) and adds the translation of each parent, reached through the weak pointer at +0x3c and its raw pointer
// at +0x40, until there is no parent. The character's matrix is held by pointer in the binary, and upstream has it
// by value (layout request: "gameswf character +0x4c pointer to gameswf matrix, translation at +0x08 and +0x14"). The
// sum is not made and the result is (0, 0), the value the original starts from.
std::array<float, 2> GameSWFUtils::GetAbsolutePosition(gameswf::character* character) {
    (void)character;
    return {0.0f, 0.0f};  // PARTIAL: parent walk left out, see above.
}

// 0x416a7c _ZN12GameSWFUtils23GetAbsoluteBoundingRectEPN7gameswf9characterE
// PARTIAL: the original takes the parent's absolute position (GetAbsolutePosition of the parent at +0x40), calls
// character::get_bound(rect*) (vtable +0x12c), and returns (xmin + x) / 20, (xmax + x) / 20, (ymin + y) / 20,
// (ymax + y) / 20. GetAbsolutePosition is partial, so the offset is not known and the result is zeros. The original
// leaves the output unset when the character is null, and zeros are returned here.
std::array<float, 4> GameSWFUtils::GetAbsoluteBoundingRect(gameswf::character* character) {
    (void)character;
    return {0.0f, 0.0f, 0.0f, 0.0f};  // PARTIAL: depends on GetAbsolutePosition, see above.
}

// 0x416b3c _ZN12GameSWFUtils15GetLabeledFrameEPN7gameswf9characterEPKc
// PARTIAL: the original reads the current frame (get_current_frame, +0x138), looks the label up with
// goto_labeled_frame (+0x9c), reads the frame it reached, then goes back to the saved frame (+0x14c). The binary's
// slot +0x9c takes a tu_stringi const& and upstream declares a const char* overload, so the lookup is not made
// (declaration request: "gameswf::character::goto_labeled_frame(tu_stringi const&) virtual +0x9c"). Returns -1,
// which is the original's not-found result.
int GameSWFUtils::GetLabeledFrame(gameswf::character* character, const char* label) {
    (void)character;
    (void)label;
    return -1;  // PARTIAL: label lookup not made, see above.
}

// 0x416c14 _ZN12GameSWFUtils16SwfTextureLoaderEPKcii
// PARTIAL: the width and height arguments are not read by the original. The path is chosen from the menu name:
// the splash and dhalliance menus map to language-specific files (SavegameManager::getLanguage() through dword_99F778,
// with the tables at unk_8C81A8 to unk_8C8290 and the I9000 and droid variants), and any other name is formatted as
// "data/%s". The texture is then fetched with glitch::video::CTextureManager::getTexture (device record at
// dword_99F73C + 16, field +224), which is not declared, and the reference is dropped. Returns nullptr.
glitch::IReferenceCounted* GameSWFUtils::SwfTextureLoader(const char* path, int width, int height) {
    (void)path;
    (void)width;
    (void)height;
    return nullptr;  // PARTIAL: path table and CTextureManager::getTexture not rebuilt, see above.
}

// 0x416fc8 _ZN12GameSWFUtils14gameswf_loggerEbPKc
// PARTIAL: when the logger is disabled the original returns the result of _DEBUG_OUT("%s", message). The declared
// _DEBUG_OUT returns void, so that result is not available and 0 is returned. When enabled, the flag itself is
// returned (1 for true), as the original does.
int GameSWFUtils::gameswf_logger(bool enabled, const char* message) {
    if (!enabled) {
        _DEBUG_OUT("%s", message);
        return 0;  // PARTIAL: the original returns _DEBUG_OUT's result here (int in the binary).
    }
    return enabled;
}

// 0x416fe0 _ZN12GameSWFUtils8PlayAnimEP6MenuFXPN7gameswf9characterEPKc
// The menu's RenderFX::PlayAnim(character, anim, 0) is tried first. Otherwise the character's member named anim is
// read with get_member (+0x20), and the menu's AS callback of that name is invoked with no arguments. The value
// read by get_member is released on every path, which the as_value destructor does here.
int GameSWFUtils::PlayAnim(MenuFX* menu, gameswf::character* character, const char* anim) {
    if (!menu || !character) {
        return 0;
    }
    if (menu->PlayAnim(character, anim, 0)) {
        return 1;
    }
    tu_string name(anim);  // global tu_string in upstream base/container.h; converts to tu_stringi
    gameswf::as_value value;
    bool played = character->get_member(name, &value) &&
                  menu->InvokeASCallback(character, anim, nullptr, 0) != 0;
    return played ? 1 : 0;
}

// 0x417108 _ZN12GameSWFUtils8PlayAnimEP6MenuFXPKcS3_
// The target name is looked up with the menu's RenderFX::Find, and the character found is animated with the
// overload above. A null menu or target returns 0, and so does a null result from the lookup.
int GameSWFUtils::PlayAnim(MenuFX* menu, const char* target, const char* anim) {
    if (!menu || !target) {
        return 0;
    }
    gameswf::character* found = static_cast<gameswf::character*>(menu->Find(target));
    return PlayAnim(menu, found, anim);
}

// 0x417148 _ZN12GameSWFUtils18GetFrameIDFromNameEPN7gameswf9characterEPKc
// PARTIAL: the sprite check (is(AS_SPRITE) through vtable +0x08) is rebuilt and returns -1 on failure, as the
// original does. The lookup of the label's frame number (sprite_instance::get_frameid_from_label on a tu_string of
// the name) is not declared in this tree (declaration request: "gameswf::sprite_instance::get_frameid_from_label"),
// so the result is -1 (not found) for every name.
int GameSWFUtils::GetFrameIDFromName(gameswf::character* character, const char* name) {
    if (!character || !character->is(gameswf::AS_SPRITE)) {
        return -1;
    }
    (void)name;
    return -1;  // PARTIAL: frame lookup not made, see above.
}

// 0x4171f8 _ZN12GameSWFUtils9GotoFrameEPN7gameswf9characterEPKc
// The sprite check, the label lookup (GetFrameIDFromName) and the frame jump (GotoFrame(character, int)) as in the
// original. PARTIAL only through GetFrameIDFromName, which returns -1 until its lookup is declared, so this always
// returns 0 for now.
int GameSWFUtils::GotoFrame(gameswf::character* character, const char* label) {
    if (character && character->is(gameswf::AS_SPRITE)) {
        int frame = GetFrameIDFromName(character, label);
        if (frame != -1) {
            return GotoFrame(character, frame);
        }
    }
    return 0;  // PARTIAL: waits on GetFrameIDFromName (see above).
}

// 0x41724c _ZN12GameSWFUtils12PreloadGlyphEPKcPN7gameswf9characterEP6MenuFX
// PARTIAL: the class check (is(32) through vtable +0x08 on the character) is rebuilt, with 0 returned on failure as
// in the original. The glyph loop is not made: it walks the character's glyph records (44-byte entries behind the
// pointer at +0x50, and the record at +0x178 for the last call), and when text is null it takes the text buffer
// from +0x139, or from the pointer at +0x144 when the byte at +0x138 is 255. It then calls RenderFX::PreloadGlyphs,
// which is not declared (layout and declaration requests). Returns 0.
int GameSWFUtils::PreloadGlyph(const char* text, gameswf::character* character, MenuFX* menu) {
    if (!character || !character->is(kTextClassId)) {
        return 0;
    }
    (void)text;
    (void)menu;
    return 0;  // PARTIAL: glyph preload loop not made, see above.
}
