// Exports the original-ARM layout oracle (procedural_layout_original.py receipt) as a TSV reference.
const fs = require('fs');
const src = process.argv[2];
const j = JSON.parse(fs.readFileSync(src, 'utf8'));
const out = [];
out.push('# Original ARM32 layout oracle (P17 PROCGEN2). Source: port/level-loader/reports/procedural-layout-original.json');
out.push('# (generic-level-loader checkout). Original cache sha256 ' + j.cache_sha256 + ', engine sha256 ' + j.engine_sha256);
out.push('# R: rule seed generated final_state random_calls root_attempts place_calls unspawn_calls step_calls path_calls rule_calls');
out.push('# T: rule seed index block_source name grid_x grid_y height_bits list_block list_gameplay list_visual chances children');
for (const c of j.cases) {
  if (!c.name.startsWith('data/scene/')) continue; // the four synthetic fixtures are not authored rules
  for (const r of c.runs) {
    const e = r.events;
    out.push(['R', c.name, r.seed, r.success ? 1 : 0, r.final_state, e.random_calls, e.root_attempts, e.place_calls, e.unspawn_calls, e.step_calls, e.path_calls, e.rule_calls].join('\t'));
    for (const t of (r.tiles || [])) {
      const le = t.list_element || {};
      out.push(['T', c.name, r.seed, t.index, t.block_source, t.name, t.grid[0], t.grid[1], t.height_bits,
        le.name || '', le.gameplay || '', le.visual || '', le.chances, (t.children || []).join(',')].join('\t'));
    }
  }
}
fs.writeFileSync(process.argv[3], out.join('\n') + '\n');
console.log('rows', out.length);
