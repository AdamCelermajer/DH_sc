# Provider-first glyph and advance ownership

Actual `font::get_glyph0x7d01bc` is640bytes, captured in `port/level-world/reference/font-text/glyph-backend`. Its source proof executes6,912 cases, including original inline empty/direct/colliding code-map lookup; the optimized64bit projection matches all returned words and15,768 ordered service calls. Copied NaN words are exact;336 arithmetic-NaN cases compare class only. Host ASan/UBSan/LSan replay adds five required-service failure prefixes, six malformed guards and one synchronous nested lookup.

Source order is default advance512/index-1 → bitmap face → bitmap image if face exists → FT image → FT face even when the image is missing → embedded lookup only after both image providers fail. Provider calls may alter bounds/advance even when returning null; those writes survive fallback. Provider success keeps index-1 and its returned advance. Nonempty alignment-zone table (`font+7c`, not merely Font3 tag) multiplies advance20. Embedded success copies the authored advance if signed16 index is below advance count; otherwise it preserves current/default advance and uses the same zone multiplier. Embedded miss returns false without scaling or clearing other retained fields.

`swf_glyph_lookup.hpp/.cpp` provide the flat ordered projection. `GlyphLookup48` owns no bitmap/face; identities are full native uintptr. `GlyphLookupFont48` borrows live style/provider flags, zone count and advance backing. Mandatory services provide bitmap-face/image, FT-image/face and embedded lookup. Actual source hash lookup executes in the original proof; the caller's owned decoded code map can supply the equivalent service. Original weak-player lifetime and gc_ptr assignment/allocation remain explicit caller services, not reconstructed by this projection. Malformed native indices are rejected rather than reproducing unsafe original out-of-range reads.

Upstream r1714 currently looks up embedded index/shape before calling its provider, then overrides provider-success advance with `m_advance_table[index]`. This is a real fork difference. For the current genuine FT provider (which does not consume embedded shapes), the smallest source correction inside a NEW core snapshot is:

1. Keep initial `index=-1,advance=512`.
2. Call the actual FT provider before `m_code_table.get`; preserve its advance and apply the existing zone-count multiplier.
3. Remove the entire provider-success authored-advance overwrite block.
4. On provider miss, perform the embedded code/index/shape lookup and existing authored-advance fallback.

When a future glyph provider consumes embedded outline shapes, it needs a separately explicit provider contract: the original FT/bitmap calls do not accept that upstream shape argument. Retaining early shape lookup as an undocumented compatibility shortcut would leave index/order differences. Do not switch font-size scaling, suppress embedded strikes, substitute glyphs or change FT load flags to hide this fork distinction. The frozen r1714 core/vendor/façade and font backends were not edited by this task.

Host target: `tests/swf_glyph_lookup.cpp` + `swf_glyph_lookup.cpp`; argument `reference/swf-glyph-lookup/source-fixtures.bin`. It is a native source-projection audit, not full GameSWF/GPU or complete original FreeType raster parity. See separately frozen original font resolver and real Fontin/Arial HUD proofs.
