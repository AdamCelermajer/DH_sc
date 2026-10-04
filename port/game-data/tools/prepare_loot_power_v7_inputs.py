"""Extract exact original cache inputs; original readers identify NumProb span.

Original code executes only in this offline evidence tool, never in the game.
The emitted records are verbatim cache bytes, not reconstructed values.
"""
import hashlib,json,struct,sys,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from items_differential import Original

def sha(b):return hashlib.sha256(b).hexdigest()
def main():
    output=ROOT/'.local-inputs/player-loot-v7/cache'
    output.mkdir(parents=True,exist_ok=True)
    archive=Path(r'C:\Users\adamc\Downloads\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
    assert sha(archive.read_bytes())=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    files={}
    with zipfile.ZipFile(archive) as z:
        prefix='com.gameloft.android.GAND.GloftD2SS/files/data/pydata/'
        for stem in ('item_powers','item_powers_monopoly','loot_table'):
            for suffix in ('pyarray','pyarraynames','pystructnames'):
                name=stem+'_'+suffix+'.bin';blob=z.read(prefix+name)
                path=output/name
                if path.exists():assert path.read_bytes()==blob
                else:path.write_bytes(blob)
                files[name]={'archive_entry':prefix+name,'bytes':len(blob),'sha256':sha(blob)}
    original=Original(ROOT/'.local-inputs/libDungeonHunter2.so',{'functions':[]})
    original.blob=(output/'loot_table_pyarray.bin').read_bytes()
    readers=[]
    for address in (0x4ba4fc,0x4ba3c8,0x4ba27c,0x4ba12c,0x4b9fe0,0x4b9e8c,0x4b9d40,0x4b9bf4):
        begin=original.cursor;original.invoke(address,[original.stream],budget=50000000)
        readers.append({'address':hex(address),'begin':begin,'end':original.cursor})
    span=readers[-1];data=original.blob[span['begin']:span['end']]
    path=output/'num_prob_records_v7.bin'
    if path.exists():assert path.read_bytes()==data
    else:path.write_bytes(data)
    files[path.name]={'source':'loot_table_pyarray.bin','begin':span['begin'],
                     'end':span['end'],'sha256':sha(data),'bytes':len(data)}
    original.blob=(output/'item_powers_monopoly_pyarray.bin').read_bytes();original.cursor=0
    original.invoke(0x4baa30,[original.stream],budget=50000000)
    assert original.cursor==len(original.blob)
    original.blob=(output/'item_powers_pyarray.bin').read_bytes();original.cursor=0
    original.invoke(0x4bacc8,[original.stream],budget=50000000)
    list_end=original.cursor
    original.invoke(0x4bab7c,[original.stream],budget=50000000)
    assert original.cursor==len(original.blob)
    manifest={'validation':'PASS','files':files,'loot_readers':readers,
              'power_list_end':list_end,'original_sha256':sha((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()),
              'tool_sha256':sha(Path(__file__).read_bytes()),'scope':__doc__}
    ref=ROOT/'port/game-data/reference/loot-power-creation-v7';ref.mkdir(parents=True,exist_ok=True)
    target=ref/'cache-inputs.json';assert not target.exists()
    target.write_text(json.dumps(manifest,indent=2)+'\n')
    print(json.dumps({'validation':'PASS','num_prob_span':span,'power_list_end':list_end,
                      'input_files':len(files)}))
if __name__=='__main__':main()
