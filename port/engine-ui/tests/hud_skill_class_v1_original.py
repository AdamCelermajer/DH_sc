"""Actual ResetProperties/_LoadClass/primitive getters/setters on cache rows.

The full original class instruction traversal executes. Owner resolved is the
supplied real character row with the script-produced SnS_Level word. Global
temporary/default identities and class-array ownership are explicit fixtures.
No class formula/getter/setter/recursive-group routine is replaced.
"""
import hashlib,json,struct,sys
from pathlib import Path
from hud_formatting_v1_original import Cpu,Original,words,REPO,ROOT
from localization_connection_probe import constants
sys.path.insert(0,str(REPO/'port/game-data/tools'))
sys.path.insert(0,str(REPO/'port/level-world/tools'))
from inspect_class_tables import parse
from prepare_actors import strings
def main():
 assets=REPO/'port/android-native/app/src/main/assets/data';table=parse(assets)['rows']
 raw=(assets/'character_properties_pyarray.bin').read_bytes();names,_=strings((assets/'character_properties_pyarraynames.bin').read_bytes());fields,_=strings((assets/'character_properties_pystructnames.bin').read_bytes());sns=fields.index('SnS_Level')
 c=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
 def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
 rows=c.data+0x100000;entries=rows+0x10000;owner=c.data+0x1000;target=c.data+0x8000;default=c.data+0x9000
 got=(0x3e2e34+word(0x3e3008))&0xffffffff;c.pointer(word(got+word(0x3e300c)),len(table));c.pointer(word(got+word(0x3e3010)),rows)
 # Build all rows, retaining exact runtime stride24 rather than serialized20.
 for i,row in enumerate(table):
  c.uc.mem_write(rows+i*12,words(0,len(row['entries']),entries))
  for f in row['entries']:c.uc.mem_write(entries,words(0,*f));entries+=24
 c.uc.mem_write(default,words(0)+raw[4:900])
 defaultgot=(0x3def24+word(0x3def2c))&0xffffffff;c.pointer(word(defaultgot+word(0x3def30)),default)
 lineargot=(0x3e2d88+word(0x3e2e18))&0xffffffff;c.pointer(lineargot+word(0x3e2e1c),target)
 records=[];observations=[]
 # Decode the bounded recorded Skill76 stream to choose its actual display
 # properties and string IDs. Full native ownership is checked in the host.
 skillraw=(assets/'skills_pyarray.bin').read_bytes();at=0
 def w():
  nonlocal at
  v=struct.unpack_from('<I',skillraw,at)[0];at+=4;return v
 def byte():
  nonlocal at
  v=skillraw[at];at+=1;return v
 def text():
  nonlocal at
  n=w();v=skillraw[at:at+n];at+=n;return v
 for _ in range(w()):n=w();at+=n*4
 skills=[]
 for _ in range(w()):
  anim=w();moving=byte();display=[w()for _ in range(w())];element=w();faery=byte();flags=w();required=w();script=text();assign=byte();current=w();description=w();icon=text();name=w();next_id=w();kind=w()
  skills.append(dict(script=script,display=display,current=current,next=next_id))
 textassets=REPO/'port/android-native/app/src/main/assets/original-cache/data';cst=constants(textassets/'pydata/common_text_pycst.bin');metadata=json.loads((REPO/'.local-inputs/localization-discovery/common-text-probe.json').read_text());strings_cache={}
 for p in metadata['packs']:
  for sheet in p['sheets']:
   b=(textassets/sheet['filename']).read_bytes();pos=2;values=[]
   for _ in range(struct.unpack_from('<H',b)[0]):n=struct.unpack_from('<H',b,pos)[0];pos+=2;values.append(b[pos:pos+n]);pos+=n
   strings_cache[p['pack'],sheet['sheet']]=values
 def integer_string(id):
  cfg=cst[b'StringConfig'];sheet=(id>>cfg[b'PackIDShift'])&cfg[b'PackIDMask'];index=(id>>cfg[b'StrIDShift'])&cfg[b'StrIDMask'];return strings_cache[0,sheet][index]
 defaults=[integer_string(cst[b'StrID'][key])for key in [b'GLOBAL_DECIMAL_SEPERATOR',b'GLOBAL_THOUSANDS_SEPERATOR',b'GLOBAL_THOUSANDS_GROUP_AT']];formatter=Original()
 scripts=[('prince_mage_staffmaster','MagePlayerBase','Skill_Mage_StaffMaster'),('prince_warrior_hardiness','KnightPlayerBase','Skill_Warrior_Hardiness')]
 for script,actor,class_name in scripts:
  actorid=names.index(actor);classid=next(i for i,r in enumerate(table)if r['name']==class_name)
  skill=next(s for s in skills if s['script']==script.encode())
  for level in range(0,21):
   resolved=bytearray(raw[4+actorid*896:4+(actorid+1)*896]);struct.pack_into('<I',resolved,sns*4,level*256)
   c.uc.mem_write(owner+0xa94,words(0)+resolved);c.uc.mem_write(target,b'\xcd'*900)
   c.invoke(0x3def34,[owner,target]);c.invoke(0x3e2e20,[owner,target,classid,1]);expected=bytes(c.uc.mem_read(target+4,896))
   strings_blob=b''.join(words(len(v))+v for v in(script.encode(),actor.encode(),class_name.encode()))
   display=struct.unpack('<224i',expected);values=[display[p]/256. for p in skill['display']];formatted=[]
   for key in ('current','next'):
    template=integer_string(skill[key]);_,value,_=formatter.run(template,values,pack=0,decimal=defaults[0],separator=defaults[1],group=defaults[2]);formatted.append(value)
   record=strings_blob+words(actorid,classid,level)+expected+b''.join(words(len(v))+v for v in formatted);records.append(words(len(record))+record)
   observations.append(dict(script=script,actor=actor,class_id=classid,level=level,temporary_sha256=hashlib.sha256(expected).hexdigest()))
 ref=ROOT/'reference/hud-formatting-v1';gold=ref/'skill-class-gold.bin';gold.write_bytes(words(0x31435348,len(records))+b''.join(records))
 report=dict(validation='PASS',original_instructions_executed=True,cases=len(records),words_compared=len(records)*224,SnS_Level_property=sns,gold_sha256=hashlib.sha256(gold.read_bytes()).hexdigest(),observations=observations,scope=__doc__)
 (ref/'skill-class-original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items()if k!='observations'}))
if __name__=='__main__':main()
