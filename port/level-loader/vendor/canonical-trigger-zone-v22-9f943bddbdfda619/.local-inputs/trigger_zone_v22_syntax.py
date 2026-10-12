from pathlib import Path
r=Path(__file__).resolve().parent.parent
s=(r/'.local-inputs/generic_lua_script_v13_syntax.py').read_text()
s=s.replace("sources=['port/level-world/generic_lua_script_owner_v13.cpp','port/level-world/tests/generic_lua_script_owner_v13.cpp']","sources=['port/level-world/canonical_trigger_zone_v22.cpp']")
s=s.replace('generic-lua-script-v13-arm64-syntax.json','trigger-zone-v22-arm64-syntax.json').replace('generic Lua owner/test only','TriggerZone owner only')
exec(compile(s,str(__file__),'exec'))
