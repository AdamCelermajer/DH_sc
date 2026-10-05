"""Original offline SG_Save coordinator. Profile saveAll and StreamBuffer clear
remain labelled boundaries; this receipt does not prove file persistence."""
import sys, json
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
from character_menu_native_v1_original import Original, ROOT

class SaveWriter(Original):
 def hook(self, uc, address, size, userdata):
  if self.active:
   if address == 0x7fd794:
    self.effects.append(['online', False]); self.ret(self.online); return
   if address == 0x315fb8:
    self.effects.append(['saveAll', self.c.reg(0)]); self.ret(); return
   if address == 0x316a48:
    self.effects.append(['clear_volatile', self.c.reg(0)]); self.ret(); return
  super().hook(uc, address, size, userdata)

m = SaveWriter(); saved = m.alloc(512); profile = m.alloc(256); m.online = m.alloc(16)
m.c.uc.mem_write(m.online, bytes(16)); cases = []
for has_profile in (False, True):
 for blocked in (False, True):
  for prior_mode in (0, 1, 2, 99):
   m.c.pointer(saved + 8, profile if has_profile else 0)
   m.c.uc.mem_write(saved + 12, bytes([blocked]))
   m.c.pointer(saved + 0x178, prior_mode)
   m.effects = []; m.active = True
   try: m.c.invoke(0x464b2c, [saved], budget=100000)
   finally: m.active = False
   reached = has_profile and not blocked
   expected = [['online', False], ['online', False], ['saveAll', profile], ['online', False]] if reached else []
   assert m.effects == expected, (has_profile, blocked, prior_mode, m.effects)
   assert m.word(saved + 0x178) == (1 if reached else prior_mode)
   cases.append(dict(profile=has_profile, blocked=blocked, prior_mode=prior_mode, mode=m.word(saved+0x178), effects=m.effects))
(ROOT / 'port/engine-ui/reference/character-menu-native-v1/save-writer-offline-gold-v1.json').write_text(json.dumps(dict(boundary_fixtures=['fresh GetOnline (offline)', 'same-profile Savegame.saveAll', 'volatile StreamBuffer.clear'], cases=cases), indent=2)+'\n')
print(json.dumps(dict(validation='PASS', original_offline_save_cases=len(cases), file_writer_unproved=True)))
