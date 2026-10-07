"""Reuse actual V5 retained-script fixture with exactly one V6 player graph."""
from pathlib import Path
root=Path(__file__).resolve().parents[3];base=root/'port/level-world'
source=base/'tests/character_skill_native_v5_host.cpp'
text=source.read_text()
needle='  auto& player=*unit.player;const auto& slots=player.state().skills;'
assert text.count(needle)==1
replacement='''  auto& player=*unit.player;
  // Source CancelSneaking's fixed class146 native delete uses THIS existing
  // BuffOwner, same property groups and same V3 TimerStore, not another graph.
  check(player.native_buffs()!=nullptr,"V6 native buff borrow");
  check(player.session().classes().rows.size()>146,"Source fixed buff class range");
  const auto initial_buffs=player.buff_count(),initial_groups=player.session().property_view().group_count;
  BuffResult24 added{};check(dh2_character_buff_add(&added,player.native_buffs(),146,100000,1,256,-1,"native_v6_source146")==1,"Same-owner native buff add");
  check(added.instance&&player.buff_count()==initial_buffs+1,"Owned native Buff instance");
  BuffSnapshot48 snapshot{};bool found_buff=false;
  for(unsigned n=0;n<player.buff_count();++n){check(player.buff_snapshot(&snapshot,n)==1,"Actual native Buff snapshot");if(snapshot.instance==added.instance){found_buff=true;break;}}
  check(found_buff,"Source class146 native instance");
  const auto timer_id=snapshot.timer_id;check(timer_id>=0&&player.session().timers().slots[timer_id].active,"Same V3 Buff timer active");
  check(snapshot.sheet&&player.session().property_view().group_count,"Same property group publication");
  BuffResult24 removed{};check(player.native_delete_buff(&removed,146)==1,"Source same-owner PROPS_DelBuff");
  check(player.buff_count()==initial_buffs&&!player.session().timers().slots[timer_id].active,"Same timer native removal");
  check(player.session().property_view().group_count==initial_groups,"Same property group removal");
  std::uint8_t source_byte415=0;
  check(player.native_cancel_sneaking(&source_byte415)==0&&source_byte415==1,"Same owner source CancelSneaking byte write");
  const auto& slots=player.state().skills;'''
text=text.replace(needle,replacement,1)
text='#include "../character_player_skills_v6.hpp"\n#define CharacterPlayerSkillsV3 CharacterPlayerSkillsV6\n'+text+'\n#undef CharacterPlayerSkillsV3\n'
target=base/'tests/character_player_skills_v6_host.cpp'
target.write_text(text)
print('Prepared retained V6 player and native Buff proof')
