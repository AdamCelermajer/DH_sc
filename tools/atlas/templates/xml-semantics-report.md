# XML semantic resolution report

## Scope and evidence

This pass classifies XML attribute references using the original source's local
definitions and consumer roles. Its targets describe static source declarations;
they do not claim that a runtime loader admitted or used a particular object.

There is no direct visual counterpart for this index operation. The original
consumer evidence is the recovered level-loader handoff and property-map source:
`port/level-loader/OWNER-CONTEXT-REQUIREMENTS.md` distinguishes `template` from
`_templateName` (lines 48–49); `port/level-world/canonical_object_factory_v1.cpp`
reads `gametype` and `name` to select construction (line 12) and applies an
explicit `template` attribute (lines 29–30); and
`port/level-world/canonical_property_map_v1.cpp` registers `_templateName` as a
class property (line 59), then routes a nonempty value through `set_template`
and the original template-provider assertion (lines 134–150). The recovered
Android `PropertyMap::SetTemplate` at `0x513fec` stores a class-map template
name, while `PropertyMap::LoadTemplate` at `0x51419c` is an assertion stub. The
handoff reports no external template XML parser.
The resolver gives this proven Android selector role priority over any
same-name XML declaration in the resource and classifies it as an internal
property-map label (`NOT-NEEDED`), with external inheritance recorded as
unsupported/unverified. Evidence is recorded in
`port/level-world/reference/canonical-object-factory-v1/renderer-integration.md`
and its `original-functions.json`; the caller chain is summarized in
`port/level-world/reference/actor-initialization/NOTES.md`. This Android proof
is not applied to iPad references; the corresponding iPad consumer was not
recovered, so iPad labels remain unresolved.

The `charpropsname` consumer remains governed by the existing
`references._resolve_charpropsname_sites` evidence: native `GetCharPropertiesId`
at `0x3b3d38`, the `charpropsname` declaration at
`canonical_property_declarations_v1.inc:68`, and the recovered
`canonical_character_family_v4.cpp:40–47` gate where `char_template` takes
precedence. This resolver leaves `charpropsname`, `char_template`, and
`char_template_pydata` untouched. It does not reinterpret `gametype` or
`_objecttype` as resource names.

## Expected behavior and implementation

An explicit named template or prototype reference resolves only to one exact,
case-sensitive declaration in the same validated physical XML resource. The
target ID combines the logical source path, physical asset ID, and declaration
element ordinal; each target keeps its source file, source asset, platform,
element ordinal, and declaration tag. Duplicate declarations in that resource
remain ambiguous. Missing, malformed, unreadable, or foreign-resource
declarations do not resolve. A same-name declaration in another file or on the
other platform is not considered.

For packed iPad XML, source bytes are read through
`package_index.read_resource`, which validates the package mapping and decoding
recipe. Android source XML uses its inventoried source path. A malformed XML
document does not become an empty declaration set. The resolver deep-copies
references, retains stable reference IDs, and is idempotent. It never maps an
iPad numeric ID to Android.

## Verification

The focused check module covers a unique local match, foreign-resource name,
duplicate declarations, missing declaration, a proven `GameObject._templateName`
property-map label, an unproven non-GameObject label, preserved
`char_template` precedence, truncated XML, stable IDs, idempotence, and use of
the validated packed-resource reader. These are isolated static checks; they do
not verify runtime loader admission or game visuals.

## Results and remaining gaps

Integrated outcome counts are recorded in [enrichment counts](counts/enrichment.json)
and the canonical [reference ledger](data/references.json). The focused
[XML checks](data/xml-semantics-checks.json) retain duplicate, missing and
malformed-source cases alongside successful source associations.

Integration owner: root. The generation pipeline calls
`xml_semantics.resolve(ctx, refs, tables, assets)` after the existing
`references.resolve_reference` and before final reference/report serialization.
`xml_semantics.summarize(refs)` supplies compact outcome counts.
Runtime use and cross-file native template-provider mapping remain unverified
unless separate loader evidence establishes them.
