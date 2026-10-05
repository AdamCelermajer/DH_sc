from pathlib import Path
p=Path(__file__).resolve().parents[1]/'tests/character_menu_composite_v1.cpp'
s=p.read_text()
old='&player==unit->player.get()'
assert s.count(old)==2
p.write_text(s.replace(old,'player.identity()==unit->player.get()'))
