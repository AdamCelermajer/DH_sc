#pragma once

#include <cstdint>

namespace dh::foundation::character_menu {

enum class SourceMenuMovieV1 : std::uint8_t {
    generic_frontend,
    character_menu
};

struct SourceTextFlagRowV1 {
    SourceMenuMovieV1 movie;
    std::uint16_t page_character;
    std::uint16_t field_character;
    std::uint16_t raw_flags;
};

// Generated from DefineEditText (tag 37) records with
// port/windows-foundation/tools/export_hud_geometry.py::parse_edit_text.
// Keys are SWF movie + containing page sprite ID + DefineEditText character ID.
// Generic menu: dqmenus.swf, SHA256 3c1a8ff1d43e89fa5608fb38dc1b27540b07dbd018206f02c6fb8d3ba061c6a5.
// Character pages: dqcharmenu_droid.swf, SHA256 43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0.
inline constexpr SourceTextFlagRowV1 source_text_flag_rows_v1[]{
    {SourceMenuMovieV1::generic_frontend, 95, 56, 0x8d30},
    {SourceMenuMovieV1::generic_frontend, 95, 58, 0x0d30},
    {SourceMenuMovieV1::generic_frontend, 95, 67, 0x8d30},
    {SourceMenuMovieV1::generic_frontend, 416, 56, 0x8d30},
    {SourceMenuMovieV1::generic_frontend, 416, 405, 0x8d20},
    {SourceMenuMovieV1::generic_frontend, 416, 407, 0x8d30},
    {SourceMenuMovieV1::generic_frontend, 416, 409, 0x8d20},
    {SourceMenuMovieV1::generic_frontend, 459, 144, 0x8d20},
    {SourceMenuMovieV1::generic_frontend, 459, 146, 0x8d32},
    {SourceMenuMovieV1::generic_frontend, 459, 148, 0x8d20},
    {SourceMenuMovieV1::generic_frontend, 459, 150, 0x8d22},
    {SourceMenuMovieV1::generic_frontend, 459, 160, 0x8d30},
    {SourceMenuMovieV1::generic_frontend, 459, 170, 0x8d30},
    {SourceMenuMovieV1::generic_frontend, 459, 173, 0x8d20},
    {SourceMenuMovieV1::generic_frontend, 459, 351, 0x8d20},
    {SourceMenuMovieV1::generic_frontend, 459, 452, 0x8d32},
    {SourceMenuMovieV1::generic_frontend, 497, 144, 0x8d20},
    {SourceMenuMovieV1::generic_frontend, 497, 146, 0x8d32},
    {SourceMenuMovieV1::generic_frontend, 497, 148, 0x8d20},
    {SourceMenuMovieV1::generic_frontend, 497, 150, 0x8d22},
    {SourceMenuMovieV1::generic_frontend, 497, 160, 0x8d30},
    {SourceMenuMovieV1::generic_frontend, 497, 170, 0x8d30},
    {SourceMenuMovieV1::generic_frontend, 497, 173, 0x8d20},
    {SourceMenuMovieV1::generic_frontend, 497, 351, 0x8d20},
    {SourceMenuMovieV1::generic_frontend, 497, 452, 0x8d32},
    {SourceMenuMovieV1::character_menu, 328, 283, 0x8d22},
    {SourceMenuMovieV1::character_menu, 328, 285, 0x8d22},
    {SourceMenuMovieV1::character_menu, 328, 288, 0x8d22},
    {SourceMenuMovieV1::character_menu, 328, 290, 0x8d22},
    {SourceMenuMovieV1::character_menu, 328, 295, 0x8d22},
    {SourceMenuMovieV1::character_menu, 328, 298, 0x8d22},
    {SourceMenuMovieV1::character_menu, 328, 303, 0x8d32},
    {SourceMenuMovieV1::character_menu, 328, 305, 0x8d22},
    {SourceMenuMovieV1::character_menu, 328, 322, 0x8d32},
    {SourceMenuMovieV1::character_menu, 386, 331, 0x8d22},
    {SourceMenuMovieV1::character_menu, 386, 355, 0x8d22},
    {SourceMenuMovieV1::character_menu, 386, 357, 0x8d20},
    {SourceMenuMovieV1::character_menu, 386, 360, 0x8d22},
    {SourceMenuMovieV1::character_menu, 386, 362, 0x8d20},
    {SourceMenuMovieV1::character_menu, 386, 364, 0x8d20},
    {SourceMenuMovieV1::character_menu, 386, 367, 0x8d20},
    {SourceMenuMovieV1::character_menu, 386, 370, 0x8d20},
    {SourceMenuMovieV1::character_menu, 386, 376, 0x8d20},
    {SourceMenuMovieV1::character_menu, 386, 378, 0x8d22},
    {SourceMenuMovieV1::character_menu, 439, 246, 0x8d32},
    {SourceMenuMovieV1::character_menu, 439, 389, 0x8d22},
    {SourceMenuMovieV1::character_menu, 439, 394, 0x8d32},
    {SourceMenuMovieV1::character_menu, 439, 425, 0x8d22},
    {SourceMenuMovieV1::character_menu, 439, 432, 0x8d22},
    {SourceMenuMovieV1::character_menu, 439, 437, 0x8d32},
    {SourceMenuMovieV1::character_menu, 456, 104, 0x8d32},
    {SourceMenuMovieV1::character_menu, 456, 105, 0x8d32},
    {SourceMenuMovieV1::character_menu, 456, 161, 0x8d32},
    {SourceMenuMovieV1::character_menu, 456, 163, 0x8d22},
    {SourceMenuMovieV1::character_menu, 456, 165, 0x8d32},
    {SourceMenuMovieV1::character_menu, 456, 167, 0x8d32},
    {SourceMenuMovieV1::character_menu, 456, 174, 0x8d32},
    {SourceMenuMovieV1::character_menu, 456, 180, 0x8d22},
    {SourceMenuMovieV1::character_menu, 456, 182, 0x8d22},
    {SourceMenuMovieV1::character_menu, 456, 192, 0x8d32},
    {SourceMenuMovieV1::character_menu, 456, 194, 0x8d32},
    {SourceMenuMovieV1::character_menu, 456, 196, 0x8d32},
    {SourceMenuMovieV1::character_menu, 456, 443, 0x8d32},
    {SourceMenuMovieV1::character_menu, 456, 451, 0x8d32},
    {SourceMenuMovieV1::character_menu, 496, 295, 0x8d22},
    {SourceMenuMovieV1::character_menu, 496, 322, 0x8d32},
    {SourceMenuMovieV1::character_menu, 496, 464, 0x8d22},
    {SourceMenuMovieV1::character_menu, 496, 466, 0xed32},
    {SourceMenuMovieV1::character_menu, 496, 468, 0xed22},
    {SourceMenuMovieV1::character_menu, 496, 470, 0x8d22},
    {SourceMenuMovieV1::character_menu, 496, 473, 0x8d32},
    {SourceMenuMovieV1::character_menu, 496, 482, 0x8d22},
    {SourceMenuMovieV1::character_menu, 547, 322, 0x8d32},
    {SourceMenuMovieV1::character_menu, 547, 498, 0x8d22},
    {SourceMenuMovieV1::character_menu, 547, 527, 0xed22},
    {SourceMenuMovieV1::character_menu, 547, 544, 0x8d22},
    {SourceMenuMovieV1::character_menu, 599, 322, 0x8d32},
    {SourceMenuMovieV1::character_menu, 599, 561, 0x8d32},
    {SourceMenuMovieV1::character_menu, 599, 566, 0x8d20},
    {SourceMenuMovieV1::character_menu, 599, 570, 0x8d20},
    {SourceMenuMovieV1::character_menu, 599, 580, 0xed22},
    {SourceMenuMovieV1::character_menu, 599, 583, 0x8d22},
    {SourceMenuMovieV1::character_menu, 599, 585, 0xed22},
    {SourceMenuMovieV1::character_menu, 599, 593, 0xed22},
};

} // namespace dh::foundation::character_menu
