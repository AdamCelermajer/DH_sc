# iPad Lua source-name and stored-constant links

## Evidence note for this implementation

This is Atlas source-index infrastructure. It does not change gameplay, Lua
execution, or a visible screen. There is no direct visual counterpart for the
resolver. The existing consumer context in
`coordination/v21.7/B155-B156-HANDOFF.md`, section 5, records the P1 video at
18:53 (Mud Lord plate), 18:56 (close-range bites), 19:08–19:09.5 (dive), and
19:12.5 (emerge with a blue splash). Those frames show why authored animation
names matter; they do not demonstrate iPad `GetPyOID` behavior or prove a
stored ordinal is a runtime ID.

The iPad source evidence is the validated plaintext Lua call ledger in
`coordination/atlas/data/packed-sites.json` and the corresponding mapped
`data/pydata/*_pyarraynames.bin`, `*_pystructnames.bin`, and `*_pycst.bin`
resources in the extracted `Dungeon Hunter 2 HD iPad CFBundleVersion 1.0.0`
package. The package map is cross-checked by `package_index`; names streams
are accepted only after its bounded, complete count/length-group parser
validates them and agrees with the recorded group/name counts. For constants,
the resolver reads through `package_index._source_file`, tries the exact bytes,
then applies `package_index._subtract_header` to the first four bytes only;
it accepts a result only when bounded UTF-8 group/member parsing consumes the
entire payload. The observed iPad `ai_pycst.bin` uses the latter recipe.

The existing Android IDA handoff documents `LuaScript::_GetPyOID` at
`0x0037f5fc` forwarding its two strings to `PyDataArrays::GetOID` at
`0x004bd640`; `coordination/ida-full-audit-2026-10-08/lane-06.md` also records
the Android AnimDict provider path. This is evidence about a separate Android
native build only. It supplies no iPad namespace registration, file admission,
native ID, or return-value contract, and the resolver does not use an iPhone
binary as authority.

The preserved behavior is a source association: an exact iPad Lua query can
point to an exact name or constant stored in the mapped iPad resource. A
namespace-to-name-file family is inferred only when at least two distinct
literal names uniquely support one logical file and that file leads every
other candidate. Its confidence remains `LIKELY`. Exact duplicates remain
`AMBIGUOUS`; a family tie is `AMBIGUOUS`; unsupported namespaces and absent
names remain `UNRESOLVED`. A target records the stored group and ordinal, with
`native_id_verified: false`. For `GetPyCst`, the stored signed value is
reported as a byte-level fact, not as a proven iPad API return.

The 2026-10-14 real-corpus run added 1,030 semantic reference rows from 7,827
iPad script calls: 427 `GetPyOID` calls and 603 `GetPyCst` calls. There were
1,021 `RESOLVED` rows and 9 `UNRESOLVED` rows. Of the name lookups, 418 have
`LIKELY` source-file/group associations. Nine stay unresolved: five
`GameObjectDamager` queries for `Level01Damage`, which lack repeated exact
family evidence, and four names absent from their inferred family. All 603
constant pairs match a unique validated iPad serialized constant group. No
row asserts a native ID or iPad API result.

The resolver also reconciled 932 existing packed Lua literal-reference rows
in place, preserving every original ID: 836 namespace/member tokens became
`NOT-NEEDED` after their exact nested provider pair was validated, 95 nested
`GetPyOID` names now point to the same derived source-name row, and one nested
`GetPyOID` name remains `UNRESOLVED` with its unresolved provider reference
attached. Combined resolver-owned rows have 1,116 `RESOLVED`, 836
`NOT-NEEDED`, and 10 `UNRESOLVED` outcomes. No existing reference is promoted
based only on another same-script name; the nested call expression, literal
role, script, and line must all match.

## Focused verification

`py -3 tools/atlas/ios_semantics_checks.py` passed 14 fixture checks covering
an exact source-name link, duplicate name candidates, an unsupported
namespace, a real family tie, incorrect stored group metadata, a header-shifted
constant stream, an absent constant, a wrong-platform source asset, dynamic
arguments, repeated resolver invocation, and truncated/trailing constant
bytes. These are isolated source-index checks; no game runtime or visual
behavior was exercised.

## Remaining limits

No iPad/iPhone native consumer, runtime loader admission, return value, or
stored-ordinal-to-native-ID rule is established. The package plantext mapping
is `LIKELY` evidence for logical source paths. Namespace family links are
inferences from repeated exact iPad script/source pairs. XML serialized
providers and field links are handled by the separate XML resolver. Do not
upgrade these links to native/API targets without a platform-matched consumer
contract.
