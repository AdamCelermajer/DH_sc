#pragma once
// B077 FOLLOW / v20 GENERAL: the catch-up haste buff of a follower, read from its authored AI script.
//
// Original evidence (data/scripts/ai/rene.luac, Preview 16 / APK 1.0.2 copy in the staged assets):
//   line 25: local OID_HASTE = GetPyOID("ClassTable", "Buff_Rene_Haste");
//   CatchUp: local buff = CreateBuff(OID_HASTE, 0, false, 0); ApplyBuff(OID_HASTE, buff); EnableCollisions(false);
//   CaughtUp: RemoveBuff(OID_HASTE); EnableCollisions(true);
// The buff is therefore named by the authored script, not by the engine. This reader finds the class-table
// name bound to the local that the script passes to ApplyBuff. Any other script (or a script with no
// ApplyBuff) yields no buff, so the follower walks at its normal rate. Nothing here names a companion.
#include <string>
#include <string_view>

namespace dh::foundation::companions {

// Empty when the script applies no class-table buff, or when its declaration is not in the authored form
// `[local] NAME = GetPyOID("ClassTable", "<row>")`. Lua `--` comments are ignored.
std::string catch_up_buff_name_v1(std::string_view authored_ai_script);

} // namespace dh::foundation::companions
