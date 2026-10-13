# File semantics report

## Scope and evidence

This pass connects source references to exact logical file paths only where the
original Android consumer exposes a path construction rule. It preserves each
reference ID and attaches source-specific asset receipts from the Android and
iPad inventories. A matching URI across platforms establishes availability;
it does not establish byte or runtime equivalence.

The Mud Lord AI path is supported by the Android IDA pseudocode for
`CharAI::StepCreateScript` at `0x3cf04c` and `CharAI::SetScriptByName` at
`0x3ceeb0`. The former reads the selected AI row's script length and name and
calls the latter only for a nonempty value. The latter selects `AISExternal`
for names without the `__` builtin prefix. The recovered owner note
`port/level-world/reference/character-script-owner/NOTES.md` (Stage 2) records
the external path `data/scripts/ai/`. `LuaManager::AddFile` at `0x37b23c`
joins that path with the authored name, appends `.luac` when the name has no
`.lua` substring, skips names already loaded by that `LuaScript`, and returns
false if its resource provider cannot open the joined path. The relevant IDA
exports are under
`.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/`:
`pseudocode/003c/003cf04c.c`, `003c/003ceeb0.c`, and `0037/0037b23c.c`.
The native Android production gate is also visible in
`port/android-native/app/src/main/cpp/renderer_character_campaign_v62.inc`
around lines 225–237: it selects the actual resolved AI row, requires a
nonempty non-`__` script, and looks up `data/scripts/ai/<name>.luac` in the
same world script cache before accepting the script bytes.

The BDAE texture path is supported by Android IDA pseudocode for
`SceneManager::SetMaterialRimLightEffects` at `0x359d08`: it prefixes the
authored texture name with `data/3d/textures/` and passes the resulting URI to
`CTextureManager::getTexture`. The source material consumer requires a
`GL_Diffuse` or `GL_Xray` renderer and a `RimLight` parameter; the scene-node
overload at `0x35a188` iterates its materials and calls that function. Evidence
is in `pseudocode/0035/00359d08.c` and `0035/0035a188.c` in the same IDA
export. A BDAE NUL-terminated `.tga` string is sufficient to test the exact
logical URI against the indexed source files, but the raw-string site alone
does not prove that this particular value reaches the RimLight parameter at
runtime. The emitted texture reference therefore keeps its producer role at
`LIKELY` while the path construction rule is `PROVEN`.

## Expected behavior and implementation

For a validated `ai#group0.Script` cell, the resolver derives the external
script URI only for a safe bare stem and only when the matching AI table row
contains that same value. For a BDAE texture reference, it accepts only a
NUL-terminated `.tga` basename from a `data/3d/*.bdae` source and tests the full
`data/3d/textures/<basename>` URI. It does not search by basename alone. The
target's `asset_files` retain physical source path, source ID, platform, hash,
size, package-map confidence, and decode recipe when available.

An exact receipt in either supplied cache establishes availability in the
inventory union, honoring the user's partial-cache fallback instruction. iPad
receipts use the package map's explicit `logical_paths` entry and keep its
mapping confidence visible. Availability does not establish that the iPad build
uses the Android native consumer or the same decoded bytes: the path rule's
consumer proof is Android-only, and `native_equivalence` remains
`not-asserted`. Missing files in both inventories stay unresolved with
`FILE_MISSING`. Different Android/iPad hashes may represent platform variants
and can coexist; conflicting known hashes within one platform are ambiguous,
with `CONFLICTING_SAME_PLATFORM_HASHES` carried as a diagnostic rather than an
unsupported reason code. Builtin `__...` names stay outside the external-file
rule, and unrelated/unsafe references pass through untouched. Existing
table/call IDs and non-file semantic targets are retained; generic candidates
are replaced with exact receipts, missing-source evidence, or explicit
same-platform conflict diagnostics.

## Verification

The focused pure-Python audit used the atlas bundled runtime. Positive checks
resolved the Mud Lord row to Android and iPad logical-path receipts, resolved
`atlas_weapons_dh2.tga` and `envmap_swamp.tga` from BDAE texture sites, and
preserved each input reference ID. Negative checks left a resource missing from
both inventories unresolved with `FILE_MISSING`, avoided mapping a same-basename
file outside the texture root, rejected a traversal basename and mismatched AI
table row, kept `__monster__` outside the external-file rule, and reported
differing same-platform hashes as ambiguous. Additional checks verified that an
iPad-only receipt resolves inventory availability, differing Android/iPad
hashes remain coexisting platform variants, and `native_equivalence` stays
`not-asserted`. Re-running an annotated BDAE reference after removing its asset
cleared its stale file target.

## Results and remaining gaps

Implementation status: the isolated resolver smoke passed all 17 focused
positive and negative checks against the current ledgers. It processed 95,124
references and annotated 4,396 eligible rows: 4,390 resolved and six retained
as unresolved because no exact source receipt was available in either cache. The Mud Lord
AI reference kept its ID and resolved to `data/scripts/ai/swampking_core.luac`
with Android and iPad package-map receipts. The two requested TGA examples
resolved to `data/3d/textures/atlas_weapons_dh2.tga` and
`data/3d/textures/envmap_swamp.tga`, each with both platform receipts. The
reporting layer marks cross-platform identity `not-established` for all three.
This was a pure-Python isolated check; atlas stage integration and final graph
validation were not run.

This is a resolver component only. It does not prove that the iPad build uses
the same native path consumer, that the two platforms contain semantically
identical decoded bytes, or that the BDAE texture names are selected at runtime
for a particular weapon/material. Production atlas integration and final graph
validation remain with the integration owner.
