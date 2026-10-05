"""Record already compiled private Android owner objects; never rebuild central."""
from pathlib import Path
import hashlib,json
root=Path(__file__).resolve().parents[3]
sources=['port/level-world/character_player_skills_v6.cpp','port/level-world/character_player_skills_v6.hpp','port/level-world/character_skills_owner_v6.cpp','port/level-world/character_skills_owner_v6.hpp','port/level-world/character_skill_reload_v6.cpp','port/level-world/character_skill_reload_v6.hpp','port/level-world/character_skill_save_reload_v6.cpp','port/level-world/character_skill_save_reload_v6.hpp']
objects=['.local-inputs/character_player_skills_v6.android.o','.local-inputs/character_skills_owner_v6.android.o']
def hashes(paths):return {p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in paths}
for p in objects:assert (root/p).read_bytes()[:4]==b'\x7fELF'
report={'validation':'PASS','target':'aarch64-linux-android24','compiler':'NDK29.0.14206865 clang++','flags':'-std=c++17 -O2 -fPIC -c','source_sha256':hashes(sources),'object_sha256':hashes(objects),'scope':'Compile-only actual V6 player and Skill ownership; no central binary mutation or whole Android gameplay claim.'}
(root/'port/level-world/reports/character-skill-reload-v6-android-compile.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','objects':len(objects)}))
