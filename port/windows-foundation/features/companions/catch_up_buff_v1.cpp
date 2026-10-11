#include "catch_up_buff_v1.hpp"

#include <cstddef>

namespace dh::foundation::companions {
namespace {

bool ident_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

// Lua line comments removed; string literals copied whole so "--" inside a string is kept.
std::string without_line_comments(std::string_view text) {
    std::string out;
    out.reserve(text.size());
    std::size_t i = 0;
    while (i < text.size()) {
        if (text[i] == '"') {
            out.push_back(text[i++]);
            while (i < text.size() && text[i] != '"' && text[i] != '\n') out.push_back(text[i++]);
            if (i < text.size() && text[i] == '"') out.push_back(text[i++]);
            continue;
        }
        if (text.compare(i, 2, "--") == 0) {
            while (i < text.size() && text[i] != '\n') ++i;
            continue;
        }
        out.push_back(text[i++]);
    }
    return out;
}

std::size_t skip_space(const std::string& s, std::size_t p) {
    while (p < s.size() && (s[p] == ' ' || s[p] == '\t' || s[p] == '\r' || s[p] == '\n')) ++p;
    return p;
}

// Identifier starting at p; empty when none.
std::string ident_at(const std::string& s, std::size_t p) {
    std::size_t e = p;
    if (e >= s.size() || (s[e] >= '0' && s[e] <= '9') || !ident_char(s[e])) return {};
    while (e < s.size() && ident_char(s[e])) ++e;
    return s.substr(p, e - p);
}

// Matches `literal` at p exactly (after the caller skipped whitespace).
bool expect(const std::string& s, std::size_t& p, std::string_view literal) {
    if (s.compare(p, literal.size(), literal) != 0) return false;
    p += literal.size();
    return true;
}

// `NAME = GetPyOID("ClassTable", "<row>")` with NAME starting at `at`; returns the row name.
std::string class_table_declaration(const std::string& s, const std::string& name) {
    for (std::size_t at = s.find(name); at != std::string::npos; at = s.find(name, at + 1)) {
        if (at > 0 && ident_char(s[at - 1])) continue;
        std::size_t p = at + name.size();
        if (p < s.size() && ident_char(s[p])) continue;
        p = skip_space(s, p);
        if (p >= s.size() || s[p] != '=' || (p + 1 < s.size() && s[p + 1] == '=')) continue;
        if (at > 0 && std::string_view("=~<>").find(s[at - 1]) != std::string_view::npos) continue;
        ++p;
        p = skip_space(s, p);
        std::size_t q = p;
        if (!expect(s, q, "GetPyOID")) continue;
        q = skip_space(s, q);
        if (!expect(s, q, "(")) continue;
        q = skip_space(s, q);
        if (!expect(s, q, "\"ClassTable\"")) continue;
        q = skip_space(s, q);
        if (!expect(s, q, ",")) continue;
        q = skip_space(s, q);
        if (q >= s.size() || s[q] != '"') continue;
        const std::size_t close = s.find('"', q + 1);
        if (close == std::string::npos) continue;
        const std::string row = s.substr(q + 1, close - q - 1);
        q = skip_space(s, close + 1);
        if (!expect(s, q, ")") || row.empty()) continue;
        return row;
    }
    return {};
}

} // namespace

std::string catch_up_buff_name_v1(std::string_view authored_ai_script) {
    const std::string s = without_line_comments(authored_ai_script);
    // The first ApplyBuff call in the script names the buff local (rene.luac: ApplyBuff(OID_HASTE, buff)).
    for (std::size_t at = s.find("ApplyBuff"); at != std::string::npos; at = s.find("ApplyBuff", at + 1)) {
        if (at > 0 && ident_char(s[at - 1])) continue;
        std::size_t p = skip_space(s, at + std::string_view("ApplyBuff").size());
        if (p >= s.size() || s[p] != '(') continue;
        p = skip_space(s, p + 1);
        const std::string local = ident_at(s, p);
        if (local.empty()) return {};
        return class_table_declaration(s, local);
    }
    return {};
}

} // namespace dh::foundation::companions
