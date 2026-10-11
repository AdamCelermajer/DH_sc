// Exports the original-ARM RandomGenerator differential (procedural_random_differential.py receipt) as TSV.
const fs = require('fs');
const j = JSON.parse(fs.readFileSync(process.argv[2], 'utf8'));
const out = [];
out.push('# Original ARM32 rnd::RandomGenerator differential (P17 PROCGEN2). Source: port/level-loader/reports/procedural-random-differential.json');
out.push('# (generic-level-loader checkout). Engine sha256 ' + j.engine_sha256 + '.');
out.push('# next <seed> <first 16 NextInt values>; between <seed> <min> <max> <value>; bounded <seed> <bound> <value>; hash <hex input> <value>');
let nexts = 0;
for (const c of j.cases) {
  if (c.operation === 'next') { nexts++; out.push(['next', c.seed, ...c.expected.slice(0, 16)].join('\t')); }
  else if (c.operation === 'between') out.push(['between', c.seed, c.minimum, c.maximum, c.expected.join(",")].join('\t'));
  else if (c.operation === 'bounded') out.push(['bounded', c.seed, c.bound, c.expected.join(",")].join('\t'));
  else if (c.operation === 'hash') out.push(['hash', c.input_hex, c.expected.join(",")].join('\t'));
}
fs.writeFileSync(process.argv[3], out.join('\n') + '\n');
console.log('rows', out.length, 'next cases', nexts, 'between', j.cases.filter(c=>c.operation==='between').length, 'bounded', j.cases.filter(c=>c.operation==='bounded').length);
