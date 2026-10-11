// v20 GENERAL: the catch-up buff is read from the authored AI script, not from a companion literal.
// Fixtures are verbatim excerpts of data/scripts/ai/rene.luac (tabs kept) plus negative forms.
#include "catch_up_buff_v1.hpp"

#include <cstdio>
#include <string>

using dh::foundation::companions::catch_up_buff_name_v1;

namespace {

int failures = 0;

void check(bool condition, const char* what) {
    if (!condition) {
        std::fprintf(stderr, "FAIL: %s\n", what);
        ++failures;
    }
}

// rene.luac line 25 and the CatchUp/CaughtUp lines that use it (verbatim).
const char* const rene_excerpt =
    "local OID_HASTE\t\t\t= GetPyOID(\"ClassTable\", \"Buff_Rene_Haste\");\n"
    "\tif(g_master and not g_is_catching_up) then\n"
    "\t\tlocal buff = CreateBuff(OID_HASTE, 0, false, 0);\n"
    "\n"
    "\t\tApplyBuff(OID_HASTE, buff);\t-- We make him to go faster\n"
    "\t\tEnableCollisions(false);\t\t-- So nothing prevents us from pathing\n"
    "local function CaughtUp()\n"
    "\t\tRemoveBuff(OID_HASTE);\t-- We make her to return to her normal speed\n";

} // namespace

int main() {
    // Authored form: the buff is the row named by the local that ApplyBuff passes.
    check(catch_up_buff_name_v1(rene_excerpt) == "Buff_Rene_Haste", "rene.luac haste row");

    // A different companion script with another row and another local name (no literal in the code).
    check(catch_up_buff_name_v1("local BUFF_X = GetPyOID(\"ClassTable\", \"Buff_Other\");\n"
                                "ApplyBuff(BUFF_X, b)\n") == "Buff_Other",
          "row name is data, not a fixed literal");

    // No ApplyBuff: no haste.
    check(catch_up_buff_name_v1("local OID_HASTE = GetPyOID(\"ClassTable\", \"Buff_Rene_Haste\");\n").empty(),
          "no ApplyBuff means no catch-up buff");

    // Commented-out ApplyBuff is not a call.
    check(catch_up_buff_name_v1("local OID_HASTE = GetPyOID(\"ClassTable\", \"Buff_Rene_Haste\");\n"
                                "-- ApplyBuff(OID_HASTE, buff);\n").empty(),
          "commented ApplyBuff ignored");

    // The local is not a class-table declaration (a different table): fail closed.
    check(catch_up_buff_name_v1("local OID_HASTE = GetPyOID(\"CharacterTable\", \"Swamp_Moth_Minions\");\n"
                                "ApplyBuff(OID_HASTE, b)\n").empty(),
          "non ClassTable declaration is not a buff");

    // ApplyBuff whose local is only compared (==) is not a declaration.
    check(catch_up_buff_name_v1("OID_HASTE == GetPyOID(\"ClassTable\", \"Buff_X\")\n"
                                "ApplyBuff(OID_HASTE, b)\n").empty(),
          "comparison is not a declaration");

    // Undeclared local: fail closed.
    check(catch_up_buff_name_v1("ApplyBuff(MISSING, b)\n").empty(), "undeclared buff local");

    // A string containing "--" is not a comment.
    check(catch_up_buff_name_v1("local B = GetPyOID(\"ClassTable\", \"Buff_--_Odd\");\n"
                                "ApplyBuff(B, b)\n") == "Buff_--_Odd",
          "double dash inside a string literal");

    if (failures == 0) std::printf("catch_up_buff_v1 tests passed\n");
    return failures == 0 ? 0 : 1;
}
