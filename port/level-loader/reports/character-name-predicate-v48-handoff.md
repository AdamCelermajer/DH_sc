# Character IsPlayer V48: original NAME field corrected and native verified

Original IsPlayer3a49f0 reads the live NAME CString30 buffer44 for type0. ObjectBase constructor independently creates archetype/gametype CString48 buffer5c. The earlier V41/V42 classification and actual archetype binding were wrong. This packet supersedes those semantic labels and bindings; their archived files remain historical evidence.

| Subtask | Status |
|---|---|
| Original constructor name30/buffer44 versus gametype48/buffer5c identity | Finished: whole ObjectBase C1/C2, GO0, three poisons each |
| Whole original IsPlayer name/gametype discriminators | Finished: 20 original cases |
| Actual checked actor alias borrows source_name_ | Finished: selected3file candidate equality verified |
| Real factory20 cases +20 live Name stores | Finished: coherent749 native PASS |
| Corrected same-actor spawn8 regression | Finished: coherent749 native PASS |
| Existing ordered NPC/Faery model regression | Finished: coherent749 native PASS; required38be5c remains |
| Qualified parent GameObject/whole Character activation | Pending separate V48 inherited-field/provider work |

The correction changes only three semantic files: actor is_player_fields_v41 lends its existing source_name_ cell; the pure predicate field is renamed live_name and its source comment/error identifies NAME30. Type!=0 retains type==1 and does not require a name. No new source storage, class/AI policy, model/template selection or save object is introduced.

The real factory test loads actual cache/design tables and selects existing AI rows as explicit predicate inputs in the same resolved sheet. It applies Name and gametype values through the existing PropertyMap, proves the returned pointer is the same source_name()/source_string(0x30), and observes later live Name stores through the original borrow. Gametype prefix cannot substitute for NAME. Missing type0 NAME preserves output; nonzero type bypasses NAME. RNG counts remain0.

The updated spawn regression corrects the historical fixture's0x48 archetype writes to original0x30 NAME stores. Its shared application/lifetime/failure cases still pass. The ordered model regression's NPC/nullFaery/ownedFaery responses remain unchanged and still stop at required whole GameObject.InitPost38be5c. Offline/lifecycle observers and model Debug/difficulty1/DEFAULT/master-save association are explicit fixtures.

Native proof is the parent's current coherent749 selected graph. Exact selected semantic source/test equality and native binary hashes were read back against coherent-loader-v49-integrated-verified.json. The serial isolated candidate runner is available for future changed-input investigations; a redundant compile was unnecessary after these same-source native checks passed. No emulator was launched.

This packet deliberately contains the NAME correction independently. The eight missing inherited scalar cells (seven ObjectBase plus idle-sound halfword) and ConditionList bridge remain separate work and are not adopted here.
