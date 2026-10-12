#include "core-platform/util.hpp"

#include <cstdio>
#include <cstring>
#include <string>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

// A stand-in for the glitch string type: it exposes data() and size() only.
struct FakeGlitchString {
    std::string text;
    char* data() { return &text[0]; }
    unsigned size() const { return static_cast<unsigned>(text.size()); }
};

int main() {
    std::string s = "Hello, WORLD [Z]";
    ToLowerCase(s, 0, -1);
    CHECK(s == "hello, world [z]");

    std::string t = "ABCDEF";
    ToLowerCase(t, 2, 3);  // inclusive range
    CHECK(t == "ABcdEF");

    std::string u = "ABC";
    ToLowerCase(u, 0, 99);  // past the end clamps to the last index
    CHECK(u == "abc");

    std::string v = "XYZ";
    ToLowerCase(v, 2, 1);  // empty range
    CHECK(v == "XYZ");

    std::string w;
    ToLowerCase(w, 0, -1);  // empty string is a no-op
    CHECK(w.empty());

    FakeGlitchString g{"MiXeD"};
    ToLowerCase(g, 0, -1);
    CHECK(g.text == "mixed");

    std::printf("%s\n", failures ? "text_case: FAILED" : "text_case: ok");
    return failures != 0;
}
