"""Create isolated V5 snapshot-origin entry from the unchanged proved core.
Existing original-kernel files remain byte-identical. Refuse unknown overwrite.
"""
import hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
source=ROOT/'port/level-world/character_target_search.cpp'
target=ROOT/'port/level-world/character_target_search_v5.cpp'
raw=source.read_text()
out=raw.replace('#include "character_target_search.hpp"','#include "character_target_search_v5.hpp"')
out=out.replace('dh2_target_list_init(','dh2_target_list_init_snapshot_v5(').replace('dh2_target_pop(','dh2_target_pop_snapshot_v5(')
out=out.replace('dh2_target_search(List40* l,const Registry8* registry,float radius,float cone,const Services16* s)',
 'dh2_target_search_snapshot_v5(List40* l,const Registry8* registry,float radius,float cone,const float captured_origin[3],const Services16* s,const SnapshotResolve16V5* resolver)')
out=out.replace('if(!valid_list(l)||!aligned(registry)||!aligned(registry->rooms)||!valid_services(s))return 1;',
 'if(!valid_list(l)||!aligned(registry)||!aligned(registry->rooms)||!valid_services(s)||!aligned(captured_origin,4)||(resolver&&(!aligned(resolver)||!resolver->invoke)))return 1;')
out=out.replace('// SearchEff captures heading first and a live pointer to the owner\'s selected\n // center second. Nested callbacks may alter fields, but not that pointer choice.',
 '// GameObject Lua wrapper has already captured its owner position. SearchEff\n // captures heading here; provider mutations do not change that stack origin.')
out=out.replace('const float* origin=center(l->owner);','const float* origin=captured_origin;')
out=out.replace('auto object=entry->object;Object48* character=nullptr;',
 'auto object=entry->object;Object48* character=nullptr;\n  if(resolver&&resolver->invoke(resolver->context,entry,&object))return 2;')
if target.exists()and target.read_text()!=out:raise RuntimeError('Refusing overwrite edited V5 target-search source')
target.write_text(out)
ref=ROOT/'port/level-world/reference/character-skill-native-v5'
(ref/'search-versioning-map-v5.json').write_text(json.dumps(dict(source=source.relative_to(ROOT).as_posix(),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),target=target.relative_to(ROOT).as_posix(),target_sha256=hashlib.sha256(target.read_bytes()).hexdigest(),scope=__doc__),indent=2)+'\n')
print('Prepared V5 snapshot-origin search kernel')
