// Test for gameswf::removeHTML in batch v1-vendor-gameswf-gameswf-fddef6-3 (0x78b4d8).
//   bash tools/trackb/check_tu.sh --test <this file> <batch .cpp> <base/container.cpp> <base/utf8.cpp> <tu_string batch .cpp>
//   The tu_string methods used by removeHTML are defined in base/container.cpp, base/utf8.cpp and the
//   tu_string batch source (v1-vendor-gameswf-gameswf_tu_string-39e5bd-0.cpp), all under vendor-gameswf.
//
// Cases follow the binary's control flow (see the comment on removeHTML in the batch source): the text
// between the nearest '>' before the first "</" and that "</" is kept. The case with no '>' before "</"
// is undefined in the binary and is not tested.

#include <cstdio>
#include <cstring>

#include "base/container.h"

namespace gameswf {
void removeHTML(tu_string& text);
}

namespace {

int g_failures = 0;

void expect_text(const char* input, const char* expected) {
    tu_string s(input);
    gameswf::removeHTML(s);
    if (std::strcmp(s.c_str(), expected) != 0) {
        std::printf("FAIL removeHTML(\"%s\") = \"%s\", expected \"%s\"\n", input, s.c_str(), expected);
        ++g_failures;
    }
}

}  // namespace

int main() {
    expect_text("<b>bold</b>", "bold");
    expect_text("<font face=\"x\">text</font>", "text");
    expect_text("a<p>one</p>", "one");
    expect_text("<br></b>", "");          // nothing between '>' and "</"
    expect_text("plain text", "plain text");  // no "</": unchanged
    expect_text("", "");                  // empty: unchanged
    expect_text("<i>x</i> tail</i>", "x");     // only the first "</" is used

    if (g_failures == 0) {
        std::printf("removeHTML tests passed\n");
        return 0;
    }
    std::printf("%d failure(s)\n", g_failures);
    return 1;
}
