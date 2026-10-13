# Global Atlas links and efficient viewer

Scope: every indexed entity in the supplied source union. Original inputs,
game code, builds and saves are read-only. Integration owner: root. Independent
source reviewer: atlas_corpus_link_review. Nine bounded agents handled source
rules, attribution, backend, frontend and review; root owns integration and UI
verification.

## Original evidence and implementation

The corresponding consumer is the reconstruction developer looking up an
entity and following source references. This index has no direct original
gameplay visual counterpart. Original logic evidence comes from the Android
IDA export, recovered consumer handoffs, validated source tables, Lua and
package bytes. Source rules and their limits are recorded in [RULES.md](RULES.md)
and the Lua, XML, character-template, resource and iPad semantic reports.

The viewer queries an ignored persistent SQLite cache rather than loading the
entire graph into the browser. Search and native functions are paginated;
entity packets, resource closure and browser caches are bounded. The portable
HTML remains a separate embedded snapshot. Typed queries, logical URIs, source
paths, gameplay-first name ranking, both graph directions, script sources and
indexed full IDA function bodies are available.

Native attribution joins use the exact canonical table/index or source row ID.
Struct-field indices and iPad stored source ordinals never become Android
gameplay row IDs. A native address, reviewed contract, port status and runtime
execution remain separate facts.

## Coverage and changes

- 448 actors, 127 skills, 1,322 items, 190 level records, and all 6,404 compact
  source profiles have native/script association records or explicit reasons.
- 3,579 animation, effect, sound and projectile profiles and all 6,334 direct
  physical or packed-logical media files have attribution dispositions. The
  complete physical asset inventory covers 13,355 files, including geometry,
  textures, UI and other inputs outside the direct-media subset.
- The reference ledger contains 96,154 sites: 60,872 resolved, 10,818 classified
  as non-resource data, and 24,464 unresolved with reasons. Compared with the
  previous committed ledger, 10,167 gaps gained supported targets, 8,333 gained
  non-resource classifications, and two ambiguous QueenAttack sites resolved
  through the source SDD member rather than duplicate XML labels. No previously
  resolved site lost its resolved disposition. The iPad provider pass adds
  1,030 source sites.
- All 7,982 Android Lua call sites retain source excerpts. All 498 available
  binding/address records have IDA paths, lines and excerpts; 271 of 288
  function names have addresses. The 257 unreviewed native contracts and 17
  names without exact native addresses remain explicit gaps.
- Both partial caches are used. The 45 distinct missing sounds.xml filenames,
  including sfx_faerie_ice.wav, remain missing. Resource attribution separately
  records 83 confirmed missing path edges and 108 candidate path edges.

## Verification and limits

The verification plan covered original-source inventory and stable IDs,
missing/duplicate/dynamic references, lexical scope and imported locals,
platform namespace separation, struct fields versus rows, ownership versus
decoder use, all media dispositions, invalid IDs and paths, cache invalidation,
read-only HTTP routes, native pagination and byte-exact full pseudocode access.
The focused results are in [global-link-verification.json](data/global-link-verification.json),
[graph verification](data/graph-verification.json), [mob verification](data/mob-verification.json),
[semantic verification](data/semantic-verification.json) and [source check](data/check.json).

Both current and independent empty-output generations passed the original
source audit. Final profile projections are rebuilt and checked in each output;
every generated artifact is compared byte-for-byte. The previously passed 22
generic checker rejection cases are retained, not claimed as rerun. Backend
measurements and protocol checks are in [the backend report](fast-backend-report.md).

Browser checks cover Mud Lord, Witch, BashDown, items, animation links, effect/audio
resource attribution, forward/reverse links and all 53 Mud Lord native functions through
pagination. Browser checks exposed and corrected localization-first shortcut
selection, canonical profile joins and object-valued badges. The final screenshot
is retained in the ignored runtime evidence folder.

Unresolved is intentional when bytes are absent, an exact name is missing,
source parsing fails, the consumer rule is unproven, or runtime selection is
unknown. Packed iPad links retain their own source namespace and likely mapping
confidence. The stripped iPad binary is catalogued; no iPhone binary or verified
iOS native ID map is assumed. This atlas does not certify gameplay, drops,
rendering or audibility.
