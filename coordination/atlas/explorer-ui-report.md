# Offline atlas explorer UI

## Scope and expected behavior

This is a static-data browser for the generated relationship index. It does not
change game behavior or certify runtime use. The page decodes the embedded
gzip-compressed UTF-8 JSON in the browser with the built-in
`DecompressionStream`; it has no remote scripts, styles, fonts, or other
dependencies. The server-side index generator owns the payload and endpoint
integration.

The left rail supports name, alias, ID, table, and indexed-field search; category
counts; a bounded first page with explicit “show more”; and shortcuts for Mud
Lord, BashDown, and Potion0. “Game entities” excludes raw Reference ledger rows
from its default result set. The raw Reference category stays available as an
explicit opt-in. Selecting an entity updates a hash deep link. The
detail view has Overview, Dependencies, Used by, Gaps, and Source tabs. Direct
relations are grouped by kind. A single Reference intermediary can be expanded
inline while retaining both edge statuses, and a Reference remains searchable
and directly selectable. Optional profile sections are compact and linked IDs
open their entity. Raw records are collapsed by default. Asset pages link to the
local `/api/source/<asset-id>` route; WAV previews use that same local route.

The default view avoids a whole-atlas graph hairball. Entity Overview includes
curated reachable Scripts, Models, Animations, Effects, Sounds, Skills, and
Loot/items links. The walk follows at most six edges, skips source-file,
engine-call, helper-definition, command, campaign-script, scene-file, and
spawn-candidate provenance links, and stops at 1,800 visited nodes or 7,000
edges. Resource groups show eight rows initially and reveal up to twelve more
per click. Actor profiles expose concise authored fields while the complete raw
record stays collapsed. Reachable unresolved references/profile gaps appear in
Overview and the Gaps tab. Shared-script/candidate links and partial coverage
remain labeled as source evidence, not runtime guarantees. Forward/reverse
closure buttons still traverse direct edges on demand with a visited set and a
finite 5,000-node bound. They cap each displayed category at 160 rows. Index
coverage is described as source coverage, not gameplay completion. Node and
edge values are inserted with DOM text nodes/textContent; indexed values are
not interpreted as HTML.

## Expected browser checks

The integration owner should substitute a representative generated payload and
check the rendered page in the real local server. Sample lookups should cover:

- **Mud Lord:** exact name and `Mud Lord` alias resolve to the SwampKing actor;
  Overview shows curated model, animation, effect, sound, script and loot links
  where those targets are present in the graph. Resource traversal does not
  pull in engine helper fanout; unresolved evidence stays visible. Reverse
  consumers are navigable.
- **BashDown:** the skill profile exposes its indexed animation/effect/audio
  relationships and edge status; relation links open their target hashes.
- **Potion0:** item source fields appear in Overview and outgoing audiovisual
  links remain visible, including absent negative-sentinel edges where the
  source index intentionally has none.
- **Reference intermediary:** the actor/skill view shows the two edge statuses
  on the expanded relation; the reference can be opened and inspected itself.
- **Deep links and reverse navigation:** opening a valid `#<encoded-node-id>`
  selects that exact entity; browser hash changes select another valid node;
  “Used by” opens the authored consumer.
- **Default and optional raw rows:** the default “Game entities” results omit
  Reference nodes, while selecting “References · raw index” browses/searches
  them. Category counts still report the reference inventory.
- **Empty search and paging:** a no-match query gives an empty result list and
  zero count without breaking category navigation. More than 100 matches show
  only the first 100 until requested; each click adds at most 100.
- **Missing hash:** an unknown hash leaves the welcome state usable. An ID that
  contains URL punctuation round-trips through the encoded hash.
- **Corrupt or incompatible payload:** invalid base64, invalid gzip/JSON, a
  payload missing `nodes` or `edges`, and browsers without gzip stream support
  produce a readable error state instead of a partial atlas.
- **Safe rendering:** names, aliases, edge labels, paths, and raw fields
  containing markup render as literal text. On a narrow viewport the category
  rail, tabs, relation rows, and fields remain usable without horizontal page
  overflow.

## Verification status

- Template refinement: complete in `explorer.html`, including optional raw
  Reference browsing, curated resource summaries, resource-walk caps, and
  mobile result-list visibility.
- Graph/profile shape inspection: read-only inspection of `graph.py`,
  `entity_profiles.py`, and the existing `coordination/atlas/data/graph.json`.
  The current actor profile contains source fields plus partial coverage; skill
  and item profiles expose `scripts`, `assets`, `audio`, and related table IDs.
  The inspected graph contains 105,585 nodes, of which 68,148 are References;
  SwampKing has explicit graph paths from its actor row through Reference rows
  to the authored loot, skill, and model tables. The UI therefore hides ledger
  rows from default browsing but follows those source paths for the resource
  summary.
- Local static inspection: the page uses built-in browser APIs and constructs
  indexed content with DOM methods; it contains no third-party imports.
- Isolated or browser testing: not performed in this helper session because the
  generated page and its serving routes are owned by the integration lead.
- Integrated visual verification: pending the real payload/server session.
- Remaining integration checks: confirm field/profile schema details, local
  asset/document routes, and the cases above against the generated atlas.
