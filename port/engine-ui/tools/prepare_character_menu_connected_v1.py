"""Snapshot the existing real gameplay fixture with a real retained AS sink.

This preserves the three-class setup and its stated offline platform limits.
It does not modify the parallel menu owner's test or claim authored menu flow.
"""
from pathlib import Path
import hashlib, json

ROOT = Path(__file__).resolve().parents[3]
source = ROOT / 'port/engine-ui/tests/character_menu_composite_v1.cpp'
text = source.read_text()
begin = text.index('struct ASHost {')
end = text.index('\n};', begin) + 3
text = text[:begin] + text[end:]
text = text.replace('#include <cctype>', '#include <cctype>\n#include "character_menu_connected_as_host_v1.hpp"')
needle = 'CharacterMenuQueriesOwnerV1 queries(qg);ASHost as;'
assert text.count(needle) == 1
text = text.replace(needle, 'CharacterMenuQueriesOwnerV1 queries(qg);ConnectedMenuASHostV1 as(queries,retained,assets+"/original-cache/data/menus");')
text = text.replace('queries.dispatch(', 'as.dispatch(')
text = text.replace('inventory_queries.dispatch(', 'as.dispatch(inventory_queries,')
# The generic replacement also matches the suffix of inventory_queries.
text = text.replace('inventory_as.dispatch(', 'as.dispatch(inventory_queries,')
assert 'inventory_as' not in text and 'ASHost as;' not in text
text = text.replace('AS_sink_and_offline_world_boundary_fixtures', 'startup_GPU_and_offline_world_boundary_fixtures')
text = text.replace('\\"complete_original_scripts\\":219,', '\\"complete_original_scripts\\":219,\\"real_retained_AS_transport\\":true,\\"authored_character_menu_flow\\":false,')
output = ROOT / 'port/engine-ui/tests/character_menu_connected_v1.cpp'
output.write_text('// Snapshot of character_menu_composite_v1.cpp; replaces only the AS fixture.\n' + text)
receipt = ROOT / '.local-inputs/character-menu-connected-v1-host/fixture-source.json'
receipt.parent.mkdir(parents=True, exist_ok=True)
receipt.write_text(json.dumps({'source': str(source.relative_to(ROOT)),
 'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
 'output': str(output.relative_to(ROOT)),
 'output_sha256': hashlib.sha256(output.read_bytes()).hexdigest(),
 'limits': 'real AS callback composition; original character-screen flow and live Android not exercised'}, indent=2)+'\n')
print(output)
