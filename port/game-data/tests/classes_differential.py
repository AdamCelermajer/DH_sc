"""Original class dispatch/arithmetic/getters/setters versus compiled ARM64.

Fixture initializes class arrays and cached/buff sheets. Cached identity is
selected to avoid original RecalcProperty, gears and dynamic bonus resolution.
No arithmetic, sheet-access or group-dispatch functions are mocked.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../engine-resources/tests'));sys.path.insert(0,str(ROOT/'tools'))
from cpu import Cpu
from inspect_class_tables import parse
from prepare_actors import strings

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--data',type=Path,required=True);p.add_argument('--characters',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args()
 manifest=json.loads((ROOT/'reference/classes/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});table=parse(a.data)['rows'];count=len(table)
 def word(address):return struct.unpack('<I',old.uc.mem_read(address,4))[0]
 got=(0x3e2e34+word(0x3e3008))&0xffffffff
 sizevar=word(got+word(0x3e300c));rowsvar=word(got+word(0x3e3010));old.uc.mem_write(sizevar,struct.pack('<I',count))
 oldrows,newrows=old.data+0x100000,new.data+0x100000;old.pointer(rowsvar,oldrows);op,np=old.data+0x140000,new.data+0x140000
 for i,row in enumerate(table):
  entries=row['entries'];old.uc.mem_write(oldrows+i*12,struct.pack('<III',0,len(entries),op));new.uc.mem_write(newrows+i*16,struct.pack('<QII',np,len(entries),0))
  for f in entries:old.uc.mem_write(op,struct.pack('<i5i',0,*f));new.uc.mem_write(np,struct.pack('<5i',*f));op+=24;np+=20
 oldsheet,newsheet=old.data+0x1000,new.data+0x1000;owner=old.data+0x4000;oldbuff=owner+0xa94;newbuff=new.data+0x4000
 # Original _Linear reads the sheet directly when it is the static cached
 # sheet; initialize that GOT identity to the caller's fixture sheet.
 linear_got=(0x3e2d88+word(0x3e2e18))&0xffffffff;old.pointer(linear_got+word(0x3e2e1c),oldsheet)
 rng=random.Random(20261002);cases=0;buff_cases=0
 for repeat in range(12):
  for id in range(count):
   values=[rng.randrange(-0x80000000,0x80000000) if repeat>=6 else rng.randrange(-2560,256000) for _ in range(224)];values[19]=(1,2,10,50,-1,0,0x7fffffff,-0x80000000,256,512,12800,0x10000)[repeat]
   buff=[rng.randrange(-0x80000000,0x80000000) for _ in range(224)];enabled=repeat%2==1
   raw=struct.pack('<224i',*values);old.uc.mem_write(oldsheet,b'\0'*4+raw);new.uc.mem_write(newsheet,raw);old.uc.mem_write(oldbuff,b'\0'*4+struct.pack('<224i',*buff));new.uc.mem_write(newbuff,struct.pack('<224i',*buff))
   old.invoke(0x3e2e20,[owner,oldsheet,id,int(enabled)]);result=new.invoke('dh2_class_apply',[newrows,count,id,newsheet,newbuff if enabled else 0]);assert result==0,(repeat,id,result)
   expected=bytes(old.uc.mem_read(oldsheet+4,896));actual=bytes(new.uc.mem_read(newsheet,896));assert actual==expected,(repeat,id,[(i,x,y) for i,(x,y) in enumerate(zip(struct.unpack('<224i',expected),struct.unpack('<224i',actual))) if x!=y][:8]);cases+=1;buff_cases+=enabled
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'original_class_dispatch':'0x3e2e20','classes':count,'bit_exact_entire_property_sheet_cases':cases,'buff_snapshot_cases':buff_cases,'unmocked_original_getters_setters_group_dispatch':True,'all_224_original_offsets_verified_identity':all(word(0x999cf0+i*4)==i*4 for i in range(224)),'scope':'Cached-sheet and explicit buff snapshot application, including original type 6 fallthrough. Original uncached RecalcProperty, gears/bonuses, character level selection and full combat are outside scope.'}
 names,_=strings((a.characters/'character_properties_pyarraynames.bin').read_bytes());fields,_=strings((a.characters/'character_properties_pystructnames.bin').read_bytes());raw=(a.characters/'character_properties_pyarray.bin').read_bytes();assert len(fields)==224 and len(names)==448 and fields[19]=='Level' and fields[38]=='Max_HP';snapshots=[];selected=[]
 for i,name in enumerate(names):
  source=raw[4+i*896:4+(i+1)*896];values=struct.unpack('<224i',source);id=values[fields.index('ClassID')];old.uc.mem_write(oldsheet,b'\0'*4+source);new.uc.mem_write(newsheet,source)
  old.invoke(0x3e2e20,[owner,oldsheet,id,0]);result=new.invoke('dh2_class_apply',[newrows,count,id,newsheet,0]);assert result==0,(name,result)
  expected=bytes(old.uc.mem_read(oldsheet+4,896));actual=bytes(new.uc.mem_read(newsheet,896));assert actual==expected,name;output=struct.unpack('<224i',actual)
  checksum=14695981039346656037
  for byte in actual:checksum=((checksum^byte)*1099511628211)&0xffffffffffffffff
  snapshots.append({'character':name,'class_id':id,'level_raw':values[19],'base_sheet_sha256':hashlib.sha256(actual).hexdigest(),'base_sheet_fnv1a64':f'{checksum:016x}'})
  if name in ('Crypt_Skeleton','CryptSlime','CryptSlime_RE','Crypt_Ghost'):selected.append({**snapshots[-1],'max_hp_raw':output[38],'max_mp_raw':output[43],'hp_raw':output[fields.index('HP')]})
 report['original_character_sheet_cases']=len(snapshots);report['character_snapshots']=snapshots;report['crypt_base_snapshots']=selected
 report['input_sha256']={file.name:hashlib.sha256(file.read_bytes()).hexdigest() for folder,names in ((a.data,('character_classes_pyarray.bin','character_classes_pyarraynames.bin','character_classes_pystructnames.bin')),(a.characters,('character_properties_pyarray.bin','character_properties_pyarraynames.bin','character_properties_pystructnames.bin'))) for file in (folder/name for name in names)}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='character_snapshots'}))
if __name__=='__main__':main()
