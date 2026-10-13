# iPad packed reference sites report

## Scope and outcome

`packed_sites.extract(ctx, assets, tables)` reads iPad app resources in memory through `package_index.read_resource` and indexes XML attributes, BRES event/file strings, and calls plus literal arguments in validated plaintext Lua. Physical numbered-DAT paths and current raw/decoded SHA-256 values are preserved alongside the map-provided logical path, decode recipe, and iPad package version. It writes no unpacked originals. It does not parse SWF/UI internals, identify image contents, decompile Lua bytecode, or claim the iPad loader admitted every mapped file.

The logical-name map comes from the iPad package's embedded `file00000-plantext.dat` and is cross-checked against `index_map.tsv`. The package is Dungeon Hunter 2 HD for iPad, CFBundleVersion 1.0.0. Its paths are version-scoped aliases with `LIKELY` mapping confidence. Android IDA names are attached only as `INFERENCE` candidates; iPad native bindings and numeric Lua table IDs remain unresolved. Android and iOS script namespaces are separate.

## Static inventory evidence

`tools/atlas/sites.py` provides the existing XML attribute parser and bounded BRES event decoder. `tools/atlas/scripts.py` provides the Lua tokenizer and source call parser. The reader's original consumer invariant is that a path and its bytes stay identifiable while a caller discovers authored references; no direct gameplay visual applies to this infrastructure.

The bounded package index validated all 2,191 parseable XML-family resources (24/39 `.xml`, 40/40 `.lightset_xml`, 400/400 `.mlx`, 628/629 `.mgp`, 596/598 `.mvp`, 250/250 `.mgx`, and 253/253 `.mvx`). It also validated 2,901 BDAE/BRES resources and 217 plaintext Lua sources. A passing XML parse or BRES structure check establishes only the source format and available static sites; it does not prove runtime use.

## Expected behavior and implementation

The extractor keeps every indexed site traceable to its physical source, logical alias, build, and observed hash. XML/BRES sites use the established source parsers. Plaintext Lua keeps all string and numeric tokens attached to their containing source and associates literals with recognized calls. String arguments may become names-only resource references when the file suffix or API name provides a type clue; they remain `LIKELY`, with no table-ID semantics assigned. Numeric literals are retained with call arguments but are never resolved against Android tables.

The currently available tracked Android original-cache tree is only an exact-byte comparison source. Producer-role confidence becomes `CROSSCHECKED` only when the current decoded bytes match an Android counterpart by SHA-256 and byte count. Same logical names, staged iOS copies, or same-name Android IDA registrations do not establish iPad runtime behavior.

## Verification

A read-only extraction over current `coordination/atlas/data/assets.json` and the bounded package root completed with 5,329 eligible files, 5,309 resources passing current type validation, 26,951 reference sites, 217 Lua source records, 7,827 Lua call sites, and 442 distinct call-target nodes. The validated set included all 2,191 parseable XML-family files and all 2,901 BDAEs. `references_by_kind` sums exactly to the final reference count. A focused BDAE/Lua smoke extraction confirmed source recipe fields on Lua calls and the same count invariant.

The extractor retained 20 typed gaps for files declined by the bounded format validators (15 `.xml`, two `.luac`, one `.mgp`, and two `.mvp`). The current asset inventory records the validated first-four-byte Lua decode recipe and decoded hashes; a complete pass emitted no asset-metadata hash mismatches. Current raw and decoded hashes are recomputed from the bounded physical source and type-validated in-memory bytes before sites are emitted.

This is static source indexing and isolated parser validation, not integrated runtime or visual verification. Original files remain unchanged. The iPad package is not evidence for an iPhone build.

## Remaining gaps

- Twenty mapped files remain unresolved after their current payloads fail bounded format validation; do not infer a decode from their logical extensions.
- iPad package-loader admission, Lua execution order, and native function bindings were not recovered. Numeric Lua identifiers remain uninterpreted.
- UI movies, sprites, fonts, and localization are present in other bounded source roots, but this helper only indexes typed XML/BRES and Lua source relationships. They require their own format-aware inventory/extractors.
