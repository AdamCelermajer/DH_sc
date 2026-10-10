#include "menu_text_layout_v1.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>

namespace dh::foundation::character_menu {
namespace {
constexpr std::uint16_t has_text = 0x8000;
constexpr std::uint16_t word_wrap = 0x4000;
constexpr std::uint16_t multiline = 0x2000;
constexpr std::uint16_t password = 0x1000;
constexpr std::uint16_t read_only = 0x0800;
constexpr std::uint16_t has_text_color = 0x0400;
constexpr std::uint16_t has_max_length = 0x0200;
constexpr std::uint16_t has_font = 0x0100;
constexpr std::uint16_t has_font_class = 0x0080;
constexpr std::uint16_t auto_size = 0x0040;
constexpr std::uint16_t has_layout = 0x0020;
constexpr std::uint16_t no_select = 0x0010;
constexpr std::uint16_t border = 0x0008;
constexpr std::uint16_t was_static = 0x0004;
constexpr std::uint16_t html = 0x0002;
constexpr std::uint16_t use_outlines = 0x0001;
constexpr float width_fudge = 4.0f; // original formatter's 80 twips

bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool codepoint_end(std::string_view text, std::size_t begin, std::size_t& end,
                   std::string& error) {
    const auto first = static_cast<unsigned char>(text[begin]);
    if (first < 0x80) {
        end = begin + 1;
        return true;
    }
    std::size_t count{};
    std::uint32_t value{}, minimum{};
    if (first >= 0xc2 && first <= 0xdf) { count = 1; value = first & 31; minimum = 0x80; }
    else if (first >= 0xe0 && first <= 0xef) { count = 2; value = first & 15; minimum = 0x800; }
    else if (first >= 0xf0 && first <= 0xf4) { count = 3; value = first & 7; minimum = 0x10000; }
    else return fail(error, "Malformed source text UTF-8 lead byte");
    if (count > text.size() - begin - 1) return fail(error, "Truncated source text UTF-8");
    end = begin + count + 1;
    for (std::size_t i = begin + 1; i < end; ++i) {
        const auto next = static_cast<unsigned char>(text[i]);
        if ((next & 0xc0) != 0x80) return fail(error, "Malformed source text UTF-8 continuation");
        value = (value << 6) | (next & 63);
    }
    if (value < minimum || value > 0x10ffff || (value >= 0xd800 && value <= 0xdfff))
        return fail(error, "Invalid source text UTF-8 code point");
    return true;
}

bool measured(const MenuTextAdvanceV1& advance, const std::string& text,
              float& value, std::string& error) {
    if (!advance) return fail(error, "Original HudGlyphFont advance provider unavailable");
    if (!advance(text, value, error)) return false;
    if (!std::isfinite(value) || value < 0) return fail(error, "Invalid original glyph run advance");
    return true;
}

struct StyledTextV1 {
    std::string text;
    std::array<float, 4> rgba{};
};
using StyledRunsV1 = std::vector<StyledTextV1>;
using StyledParagraphsV1 = std::vector<StyledRunsV1>;

bool same_color(const std::array<float, 4>& a, const std::array<float, 4>& b) {
    return a == b;
}

void append_text(StyledRunsV1& runs, std::string_view text,
                 const std::array<float, 4>& rgba) {
    if (text.empty()) return;
    if (!runs.empty() && same_color(runs.back().rgba, rgba)) runs.back().text.append(text);
    else runs.push_back({std::string(text), rgba});
}

bool parse_hex_color(std::string_view value, std::array<float, 4>& rgba,
                     std::string& error) {
    if (value.size() != 6) return fail(error, "Source FONT color must contain exactly six hex digits");
    unsigned rgb{};
    for (char c : value) {
        rgb <<= 4;
        if (c >= '0' && c <= '9') rgb |= static_cast<unsigned>(c - '0');
        else if (c >= 'A' && c <= 'F') rgb |= static_cast<unsigned>(c - 'A' + 10);
        else if (c >= 'a' && c <= 'f') rgb |= static_cast<unsigned>(c - 'a' + 10);
        else return fail(error, "Source FONT color contains a non-hex digit");
    }
    rgba[0] = static_cast<float>((rgb >> 16) & 255) / 255.f;
    rgba[1] = static_cast<float>((rgb >> 8) & 255) / 255.f;
    rgba[2] = static_cast<float>(rgb & 255) / 255.f;
    return true;
}

bool parse_font_tag(std::string_view tag, std::array<float, 4>& color,
                    std::string& error) {
    // The original FrontendText parser treats every FONT opening as a color
    // scope and searches its tag body for #RRGGBB. Keep this narrow to the
    // authored form while accepting case and quote variations.
    auto upper = std::string(tag);
    std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char c) {
        return static_cast<char>(std::toupper(c));
    });
    if (upper.rfind("FONT", 0) != 0) return fail(error, "Unsupported source HTML tag");
    const auto marker = upper.find("COLOR");
    if (marker == std::string::npos) return fail(error, "Source FONT tag is missing authored color");
    if (marker <= 4 || upper[4] != ' ')
        return fail(error, "Source FONT tag must use the authored COLOR attribute");
    auto equals = upper.find('=', marker + 5);
    if (equals == std::string::npos) return fail(error, "Malformed source FONT color attribute");
    auto at = equals + 1;
    while (at < upper.size() && upper[at] == ' ') ++at;
    if (at < upper.size() && (upper[at] == '\'' || upper[at] == '"')) ++at;
    if (at >= upper.size() || upper[at] != '#') return fail(error, "Source FONT color must use #RRGGBB");
    if (at + 7 > upper.size()) return fail(error, "Truncated source FONT color");
    if (!parse_hex_color(std::string_view(upper).substr(at + 1, 6), color, error)) return false;
    auto tail = at + 7;
    if (at > 0 && (upper[at - 1] == '\'' || upper[at - 1] == '"')) {
        if (tail >= upper.size() || upper[tail] != upper[at - 1])
            return fail(error, "Mismatched source FONT color quote");
        ++tail;
    }
    while (tail < upper.size() && upper[tail] == ' ') ++tail;
    if (tail != upper.size()) return fail(error, "Unsupported source FONT attributes");
    return true;
}

bool decode_entity(std::string_view entity, std::string& decoded, std::string& error) {
    if (entity == "&amp;") decoded = "&";
    else if (entity == "&lt;") decoded = "<";
    else if (entity == "&gt;") decoded = ">";
    else if (entity == "&quot;") decoded = "\"";
    else if (entity == "&nbsp;") decoded = " ";
    else return fail(error, "Unsupported source HTML entity");
    return true;
}

bool parse_markup(std::string_view input, const std::array<float, 4>& default_rgba,
                  StyledParagraphsV1& paragraphs, std::string& error) {
    StyledParagraphsV1 next(1);
    auto color = default_rgba;
    std::vector<std::array<float, 4>> colors;
    for (std::size_t at = 0; at < input.size();) {
        if (input[at] == '<') {
            const auto end = input.find('>', at + 1);
            if (end == std::string_view::npos) return fail(error, "Unclosed source HTML tag");
            auto tag = input.substr(at + 1, end - at - 1);
            while (!tag.empty() && tag.front() == ' ') tag.remove_prefix(1);
            while (!tag.empty() && tag.back() == ' ') tag.remove_suffix(1);
            std::string upper(tag);
            std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char c) {
                return static_cast<char>(std::toupper(c));
            });
            if (upper.rfind("FONT", 0) == 0) {
                colors.push_back(color);
                if (!parse_font_tag(tag, color, error)) return false;
            } else if (upper == "/FONT") {
                if (colors.empty()) return fail(error, "Source HTML FONT close has no matching open");
                color = colors.back();
                colors.pop_back();
            } else if (upper == "BR" || upper == "BR/") {
                next.emplace_back();
            } else if (upper == "P") {
                // Source FrontendText ignores opening P and breaks on /P.
            } else if (upper == "/P") {
                next.emplace_back();
            } else {
                return fail(error, "Unsupported source HTML tag");
            }
            at = end + 1;
        } else if (input[at] == '&') {
            const auto end = input.find(';', at + 1);
            if (end == std::string_view::npos || end - at >= 12)
                return fail(error, "Malformed source HTML entity");
            std::string decoded;
            if (!decode_entity(input.substr(at, end - at + 1), decoded, error)) return false;
            append_text(next.back(), decoded, color);
            at = end + 1;
        } else if (input[at] == '\r' || input[at] == '\n') {
            if (input[at] == '\r' && at + 1 < input.size() && input[at + 1] == '\n') ++at;
            next.emplace_back();
            ++at;
        } else {
            if (static_cast<unsigned char>(input[at]) < 0x20)
                return fail(error, "Unsupported control byte in source HTML text");
            std::size_t end{};
            if (!codepoint_end(input, at, end, error)) return false;
            append_text(next.back(), input.substr(at, end - at), color);
            at = end;
        }
    }
    if (!colors.empty()) return fail(error, "Source HTML FONT scope is not closed");
    paragraphs = std::move(next);
    return true;
}

void append_runs(StyledRunsV1& target, const StyledRunsV1& source) {
    for (const auto& run : source) append_text(target, run.text, run.rgba);
}

float measure_runs(const StyledRunsV1& runs, const MenuTextAdvanceV1& measure,
                   std::string& error, bool& ok) {
    float total{};
    for (const auto& run : runs) {
        float advance{};
        if (!measured(measure, run.text, advance, error)) { ok = false; return 0; }
        total += advance;
    }
    if (!std::isfinite(total)) { fail(error, "Source text run advance overflow"); ok = false; }
    return total;
}

bool split_styled_word(const StyledRunsV1& word, float width,
                       const MenuTextAdvanceV1& measure,
                       std::vector<std::pair<StyledRunsV1, float>>& lines,
                       std::string& error) {
    struct Glyph { std::string text; std::array<float, 4> rgba; };
    std::vector<Glyph> glyphs;
    for (const auto& run : word) {
        for (std::size_t at = 0; at < run.text.size();) {
            std::size_t end{};
            if (!codepoint_end(run.text, at, end, error)) return false;
            glyphs.push_back({run.text.substr(at, end - at), run.rgba});
            at = end;
        }
    }
    StyledRunsV1 current;
    float current_advance{};
    for (const auto& glyph : glyphs) {
        StyledRunsV1 candidate = current;
        append_text(candidate, glyph.text, glyph.rgba);
        bool ok = true;
        const float advance = measure_runs(candidate, measure, error, ok);
        if (!ok) return false;
        if (!current.empty() && advance > width) {
            lines.emplace_back(std::move(current), current_advance);
            current.clear();
            append_text(current, glyph.text, glyph.rgba);
            current_advance = measure_runs(current, measure, error, ok);
            if (!ok) return false;
        } else {
            current = std::move(candidate);
            current_advance = advance;
        }
    }
    if (!current.empty()) lines.emplace_back(std::move(current), current_advance);
    return true;
}

bool layout_paragraph(const StyledRunsV1& paragraph, bool wrap, float width,
                      const MenuTextAdvanceV1& measure,
                      std::vector<std::pair<StyledRunsV1, float>>& lines,
                      std::string& error) {
    const auto paragraph_start = lines.size();
    if (!wrap) {
        bool ok = true;
        const float advance = measure_runs(paragraph, measure, error, ok);
        if (!ok) return false;
        lines.emplace_back(paragraph, advance);
        return true;
    }

    struct Glyph { std::string text; std::array<float, 4> rgba; };
    std::vector<Glyph> glyphs;
    for (const auto& run : paragraph) {
        for (std::size_t at = 0; at < run.text.size();) {
            std::size_t end{};
            if (!codepoint_end(run.text, at, end, error)) return false;
            glyphs.push_back({run.text.substr(at, end - at), run.rgba});
            at = end;
        }
    }
    StyledRunsV1 current, pending_spaces;
    float current_advance{};
    std::size_t at{};
    while (at < glyphs.size()) {
        if (glyphs[at].text == " ") {
            append_text(pending_spaces, glyphs[at].text, glyphs[at].rgba);
            ++at;
            continue;
        }
        StyledRunsV1 word;
        while (at < glyphs.size() && glyphs[at].text != " ") {
            append_text(word, glyphs[at].text, glyphs[at].rgba);
            ++at;
        }
        StyledRunsV1 candidate = current;
        append_runs(candidate, pending_spaces);
        append_runs(candidate, word);
        bool ok = true;
        float candidate_advance = measure_runs(candidate, measure, error, ok);
        if (!ok) return false;
        if (!current.empty() && candidate_advance > width) {
            lines.emplace_back(std::move(current), current_advance);
            current.clear();
            current_advance = 0;
            pending_spaces.clear();
            StyledRunsV1 word_runs = word;
            const float word_advance = measure_runs(word_runs, measure, error, ok);
            if (!ok) return false;
            if (word_advance > width) {
                if (!split_styled_word(word, width, measure, lines, error)) return false;
            } else {
                current = std::move(word_runs);
                current_advance = word_advance;
            }
        } else if (candidate_advance > width && current.empty()) {
            pending_spaces.clear();
            if (!split_styled_word(word, width, measure, lines, error)) return false;
        } else {
            current = std::move(candidate);
            current_advance = candidate_advance;
            pending_spaces.clear();
        }
    }
    if (!pending_spaces.empty()) {
        append_runs(current, pending_spaces);
        bool ok = true;
        current_advance = measure_runs(current, measure, error, ok);
        if (!ok) return false;
    }
    if (!current.empty() || lines.size() == paragraph_start)
        lines.emplace_back(std::move(current), current_advance);
    return true;
}
}

bool source_text_flags_v1(SourceMenuMovieV1 movie, std::uint16_t page,
                          std::uint16_t field, SourceTextFlagsV1& output,
                          std::string& error) {
    for (const auto& row : source_text_flag_rows_v1) {
        if (row.movie != movie || row.page_character != page || row.field_character != field) continue;
        const auto f = row.raw_flags;
        SourceTextFlagsV1 next;
        next.raw = f;
        next.has_text = (f & has_text) != 0;
        next.word_wrap = (f & word_wrap) != 0;
        next.multiline = (f & multiline) != 0;
        next.password = (f & password) != 0;
        next.read_only = (f & read_only) != 0;
        next.has_text_color = (f & has_text_color) != 0;
        next.has_max_length = (f & has_max_length) != 0;
        next.has_font = (f & has_font) != 0;
        next.has_font_class = (f & has_font_class) != 0;
        next.auto_size = (f & auto_size) != 0;
        next.has_layout = (f & has_layout) != 0;
        next.no_select = (f & no_select) != 0;
        next.border = (f & border) != 0;
        next.was_static = (f & was_static) != 0;
        next.html = (f & html) != 0;
        next.use_outlines = (f & use_outlines) != 0;
        output = next;
        error.clear();
        return true;
    }
    return fail(error, "No source DefineEditText flags for this movie/page/character key");
}

bool menu_text_layout_v1(const MenuTextLayoutFieldV1& field, std::string_view text,
                         const MenuTextAdvanceV1& measure, MenuTextLayoutV1& output,
                         std::string& error) {
    SourceTextFlagsV1 flags;
    if (!source_text_flags_v1(field.movie, field.page_character, field.field_character,
                              flags, error)) return false;
    if (!measure || text.size() > 4096 || field.alignment > 2 ||
        !std::isfinite(field.source_height) || field.source_height <= 0 ||
        !std::isfinite(field.paragraph_leading) ||
        !std::isfinite(field.font_descent) || !std::isfinite(field.font_leading) ||
        !std::isfinite(field.root_scale_word) || field.root_scale_word <= 0)
        return fail(error, "Invalid source menu text layout inputs");
    if (flags.has_font && field.source_font_character == 0)
        return fail(error, "Authored DefineEditText font character is required");
    for (float v : field.local_rect) if (!std::isfinite(v)) return fail(error, "Invalid source text local RECT");
    for (float v : field.matrix) if (!std::isfinite(v)) return fail(error, "Invalid source text field matrix");
    for (float v : field.margins) if (!std::isfinite(v)) return fail(error, "Invalid source text paragraph margins");
    if (field.local_rect[1] <= field.local_rect[0] || field.local_rect[3] <= field.local_rect[2])
        return fail(error, "Empty source text local RECT");
    for (float c : field.source_rgba)
        if (!std::isfinite(c) || c < 0 || c > 1) return fail(error, "Invalid authored DefineEditText RGBA");

    const bool wrap = flags.multiline && flags.word_wrap;
    if (!flags.html && (text.find('<') != std::string_view::npos || text.find('&') != std::string_view::npos))
        return fail(error, "Markup is not admitted by source non-HTML field flags");
    if (!flags.multiline && (text.find('\n') != std::string_view::npos || text.find('\r') != std::string_view::npos ||
                             text.find("<BR") != std::string_view::npos || text.find("<br") != std::string_view::npos ||
                             text.find("</P>") != std::string_view::npos || text.find("</p>") != std::string_view::npos))
        return fail(error, "Line break is not admitted by source single-line field flags");
    for (unsigned char c : text)
        if (c < 0x20 && c != '\n' && c != '\r')
            return fail(error, "Unsupported control byte in source menu text");
    StyledParagraphsV1 paragraphs;
    if (flags.html) {
        if (!parse_markup(text, field.source_rgba, paragraphs, error)) return false;
    } else {
        paragraphs.emplace_back();
        append_text(paragraphs.back(), text, field.source_rgba);
    }
    const float available = field.local_rect[1] - field.local_rect[0] - field.margins[1] -
                            width_fudge - std::max(0.f, field.margins[0] + field.margins[2]);
    std::vector<std::pair<StyledRunsV1, float>> local_lines;
    for (const auto& paragraph : paragraphs) {
        const auto before = local_lines.size();
        if (!layout_paragraph(paragraph, wrap, available, measure, local_lines, error)) return false;
        if (local_lines.size() > before + 1 && !flags.multiline)
            return fail(error, "Wrapped lines are not admitted by source single-line field flags");
    }

    const float font_scale = field.source_height /
        (field.root_scale_word * 1024.f * (field.define_font3 ? 20.f : 1.f));
    const float first_y = field.source_height +
        (field.font_leading - field.font_descent) * font_scale;
    const float line_step = field.source_height + field.paragraph_leading +
        field.font_leading * font_scale;
    const float start_x = std::max(0.f, field.margins[0] + field.margins[2]);
    MenuTextLayoutV1 next;
    next.flags = flags;
    next.lines.reserve(local_lines.size());
    for (std::size_t i = 0; i < local_lines.size(); ++i) {
        const auto& source_line = local_lines[i];
        const float extra = (field.local_rect[1] - field.local_rect[0] -
                             field.margins[1]) - (start_x + source_line.second) - width_fudge;
        float x = start_x;
        if (field.alignment == 1) x += extra;
        else if (field.alignment == 2) x += extra * .5f;
        const float y = first_y + static_cast<float>(i) * line_step;
        const auto& m = field.matrix;
        MenuTextLineV1 line;
        for (const auto& run : source_line.first) {
            MenuTextRunV1 output_run;
            output_run.text = run.text;
            output_run.font_character = field.source_font_character;
            output_run.rgba = run.rgba;
            bool ok = true;
            output_run.advance = measure_runs(StyledRunsV1{run}, measure, error, ok);
            if (!ok) return false;
            line.text += output_run.text;
            line.runs.push_back(std::move(output_run));
        }
        line.advance = source_line.second;
        line.baseline = {m[0] * x + m[2] * y + m[4], m[1] * x + m[3] * y + m[5]};
        line.clip_rect = field.local_rect;
        line.clip_matrix = field.matrix;
        if (!std::isfinite(line.baseline[0]) || !std::isfinite(line.baseline[1]))
            return fail(error, "Source text baseline overflow");
        next.lines.push_back(std::move(line));
    }
    output = std::move(next);
    error.clear();
    return true;
}

} // namespace dh::foundation::character_menu
