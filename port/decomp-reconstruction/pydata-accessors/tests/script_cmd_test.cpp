#include "pydata-accessors/script_cmd.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    Structs::ScriptCmd* a = Structs::GetNewScriptCmdDataInstance<Structs::FlushMessages>();
    CHECK(a != nullptr);
    CHECK(dynamic_cast<Structs::FlushMessages*>(a) != nullptr);
    delete a;

    Structs::ScriptCmd* b = Structs::GetNewScriptCmdDataInstance<Structs::PlayEffect>();
    auto* pe = dynamic_cast<Structs::PlayEffect*>(b);
    CHECK(pe != nullptr && pe->m_w8 == 0 && pe->m_w3 == 0);
    delete b;

    std::printf("%s\n", failures ? "script_cmd: FAILED" : "script_cmd: ok");
    return failures != 0;
}
