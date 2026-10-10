#include "menu_text_layout_v1.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation::character_menu;

namespace {
void check(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

bool near(float a, float b) { return std::abs(a - b) < 0.0001f; }

MenuTextLayoutFieldV1 skill_field() {
    MenuTextLayoutFieldV1 field;
    field.movie = SourceMenuMovieV1::character_menu;
    field.page_character = 496;
    field.field_character = 466; // actual skill_description DefineEditText
    field.local_rect = {0, 20, 0, 48};
    field.matrix = {2, 0, 0, 2, 5, 7};
    field.margins = {1, 1, 0};
    field.alignment = 2;
    field.source_height = 10;
    field.paragraph_leading = 2;
    field.source_font_character = 103; // parsed SWF DefineEditText font id
    field.source_rgba = {1, 1, 204.f / 255.f, 1}; // page496/field466 authored RGBA
    return field;
}

MenuTextAdvanceV1 unit_advance() {
    return [](std::string_view text, float& advance, std::string&) {
        advance = static_cast<float>(text.size());
        return true;
    };
}
}

int main() {
    try {
        std::string error;
        SourceTextFlagsV1 flags;
        check(source_text_flags_v1(SourceMenuMovieV1::character_menu, 496, 466,
                                   flags, error), error.c_str());
        check(flags.raw == 0xed32 && flags.word_wrap && flags.multiline &&
                  flags.read_only && flags.no_select && flags.html,
              "SWF skill-description flags differ from parsed DefineEditText");
        check(source_text_flags_v1(SourceMenuMovieV1::character_menu, 547, 527,
                                   flags, error) && flags.raw == 0xed22 &&
                  flags.word_wrap && flags.multiline && !flags.no_select && flags.html,
              "SWF Faery description flags differ from parsed DefineEditText");
        check(source_text_flags_v1(SourceMenuMovieV1::character_menu, 456, 194,
                                   flags, error) && flags.raw == 0x8d32 && flags.html,
              "Inventory Details source field flags differ from parsed DefineEditText");
        check(source_text_flags_v1(SourceMenuMovieV1::character_menu, 599, 585,
                                   flags, error) && flags.raw == 0xed22 && flags.word_wrap &&
                  flags.multiline && flags.html,
              "Quest Log source details field flags differ from parsed DefineEditText");
        check(source_text_flags_v1(SourceMenuMovieV1::generic_frontend, 95, 58,
                                   flags, error) && flags.raw == 0x0d30 &&
                  !flags.has_text && !flags.multiline && !flags.word_wrap,
              "Authored name entry flags differ from parsed DefineEditText");
        check(!source_text_flags_v1(SourceMenuMovieV1::character_menu, 386, 466,
                                    flags, error),
              "Field id from another page was silently accepted");

        auto field = skill_field();
        MenuTextLayoutV1 layout;
        check(menu_text_layout_v1(field, "one two three four", unit_advance(), layout, error), error.c_str());
        check(layout.flags.raw == 0xed32 && layout.lines.size() == 2,
              "Source word-wrap/multiline flags did not produce wrapped runs");
        check(layout.lines[0].text == "one two three" && layout.lines[1].text == "four" &&
                  near(layout.lines[0].advance, 13) && near(layout.lines[1].advance, 4),
              "Source-measured word wrap changed text or advance");
        check(near(layout.lines[0].baseline[0], 8) && near(layout.lines[0].baseline[1], 27) &&
                  near(layout.lines[1].baseline[0], 17) && near(layout.lines[1].baseline[1], 51),
              "Source alignment, field transform, or paragraph leading changed baselines");
        check(layout.lines[0].clip_rect == field.local_rect &&
                  layout.lines[1].clip_rect == field.local_rect &&
                  layout.lines[0].clip_matrix == field.matrix,
              "Multiline runs lost source-local clip rectangle or matrix");

        check(menu_text_layout_v1(field,
                  "<FONT COLOR=\"#9ADEFF\">red green</FONT><BR><P>third &amp; final</P>",
                  unit_advance(), layout, error), error.c_str());
        check(layout.lines.size() == 3 && layout.lines[0].text == "red green" &&
                  layout.lines[1].text == "third & final" && layout.lines[2].text.empty(),
              "Source FONT/BR/P/entity subset did not create authored paragraphs");
        check(layout.lines[0].runs.size() == 1 &&
                  layout.lines[0].runs[0].font_character == 103 &&
                  near(layout.lines[0].runs[0].rgba[0], 154.f / 255.f) &&
                  near(layout.lines[0].runs[0].rgba[1], 222.f / 255.f) &&
                  near(layout.lines[0].runs[0].rgba[2], 1.f) &&
                  near(layout.lines[0].runs[0].rgba[3], 1.f) &&
                  layout.lines[1].runs[0].rgba == field.source_rgba,
              "Source authored font/default color inheritance was not retained per styled run");
        auto parsed_before = layout.lines;
        check(!menu_text_layout_v1(field, "<B>unsupported</B>", unit_advance(), layout, error) &&
                  error.find("Unsupported source HTML tag") != std::string::npos &&
                  layout.lines.size() == parsed_before.size(),
              "Unsupported source HTML did not fail explicitly and preserve output");
        check(!menu_text_layout_v1(field, "&copy;", unit_advance(), layout, error) &&
                  error.find("Unsupported source HTML entity") != std::string::npos,
              "Unsupported source entity was silently accepted");

        // A word longer than the available line uses the source formatter's
        // glyph-by-glyph fallback, with UTF-8 code points kept intact.
        check(menu_text_layout_v1(field, "abcdefghijklmnop", unit_advance(), layout, error), error.c_str());
        check(layout.lines.size() == 2 && layout.lines[0].text == "abcdefghijklmn" &&
                  layout.lines[1].text == "op",
              "Overlong source word was not split at measured glyph boundaries");
        check(menu_text_layout_v1(field, u8"abéghijklmnopqrst", unit_advance(), layout, error), error.c_str());
        for (const auto& line : layout.lines) check(line.text.find("\xc3\xa9") != std::string::npos ||
                                                        line.text.size() <= 14,
                                                    "UTF-8 code point split across source lines");

        MenuTextLayoutFieldV1 stats;
        stats.movie = SourceMenuMovieV1::character_menu;
        stats.page_character = 386;
        stats.field_character = 378; // right-side numeric value field
        stats.local_rect = {0, 8, 0, 12};
        stats.matrix = {1, 0, 0, 1, 100, 40};
        stats.source_height = 10;
        stats.source_font_character = 287; // original Stats page fields use font character 287
        stats.source_rgba = {1, 1, 1, 1};
        check(menu_text_layout_v1(stats, "long one-line statistic", unit_advance(), layout, error), error.c_str());
        check(layout.flags.raw == 0x8d22 && !layout.flags.multiline &&
                  !layout.flags.word_wrap && layout.lines.size() == 1 &&
                  layout.lines[0].text == "long one-line statistic" &&
                  layout.lines[0].runs.size() == 1 && layout.lines[0].runs[0].font_character == 287 &&
                  near(layout.lines[0].baseline[0], 100) && near(layout.lines[0].baseline[1], 50),
              "Existing one-line stats output changed or was clipped during layout");

        auto old = layout.lines;
        check(!menu_text_layout_v1(stats, "bad\nline", unit_advance(), layout, error) &&
                  layout.lines.size() == old.size() && layout.lines[0].text == old[0].text,
              "Single-line field accepted a newline or failed layout mutated output");
        check(!menu_text_layout_v1(field, "x", {}, layout, error),
              "Missing actual HudGlyphFont advance provider guessed widths");

        std::cout << "PASS source menu text layout v1: 84 exact SWF flag keys, HTML subset/styles, measured wrap/multiline, source baselines/clips, one-line stats preserved\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
