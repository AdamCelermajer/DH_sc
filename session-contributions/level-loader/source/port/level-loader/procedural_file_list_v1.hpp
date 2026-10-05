#pragma once
#include <string>
#include <string_view>
#include <vector>
namespace dh2::loader {
// Original GetFiles tokenization, retaining order and duplicate spelling.
// Each LF causes substring(0, LF-position-1), then consumes through that LF.
// The final unterminated tail is not emitted. Input is interpreted as a C
// byte string, so NUL ends it. This is not ordinary portable line splitting.
std::vector<std::string> procedural_file_list_v1(std::string_view bytes);
}
