"""Bundle the Knight's original Died sequence without changing attack assets."""
import argparse,hashlib,json,struct,zipfile
from pathlib import Path
from inventory import EXPECTED
from prepare_actors import strings
from prepare_player_combat import animation_tables

def main():
 p=argparse.ArgumentParser();p.add_argument('cache',type=Path);p.add_argument('--project',type=Path,default=Path(__file__).resolve().parents[2]/'android-native');a=p.parse_args()
 with a.cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==EXPECTED
 assets=a.project/'app/src/main/assets';data=assets/'data';names,_=strings((data/'character_properties_pyarraynames.bin').read_bytes());fields,_=strings((data/'character_properties_pystructnames.bin').read_bytes());row=names.index('KnightPlayerBase');props=struct.unpack_from('<224i',(data/'character_properties_pyarray.bin').read_bytes(),4+row*896);table=props[fields.index('AnimTable')]
 raw=(data/'animations_pystructnames.bin').read_bytes()
 for _ in range(4):states,n=strings(raw);raw=raw[n:]
 sequences,characters=animation_tables((data/'animations_pyarray.bin').read_bytes());paths,_=strings((data/'animations_dictionary_pyarray.bin').read_bytes());root=characters[table][states.index('Died')][0];sequence=sequences[root];assert sequence['loop']==0 and sequence['type']==0 and len(sequence['steps'])==1;step=sequence['steps'][0];assert step['redir']==0;clip=step['anim'];path=paths[clip]
 with zipfile.ZipFile(a.cache) as z:
  entries={n.lower():n for n in z.namelist()};entry=entries['com.gameloft.android.gand.gloftd2ss/files/'+path.lower()];raw=z.read(entry)
 name='animations/'+Path(path).name;(assets/name).write_bytes(raw)
 report={'cache_sha256':EXPECTED,'character':'KnightPlayerBase','row':row,'animation_table':table,'state':'Died','root_sequence':root,'inputs':[{'clip_id':clip,'asset':name,'entry':entry,'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}],'scope':'Original Died clip adapter after verified core kill. Full player FSM, revive/save/game-over and FX/audio remain separate.'}
 (assets/'player-defender-provenance.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))

if __name__=='__main__':main()
