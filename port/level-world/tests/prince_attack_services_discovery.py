"""Original-only attack dependency probes; no native or live search ownership claim."""
import hashlib,itertools,json,math,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/game-data/tests'));sys.path.insert(0,str(ROOT/'tests'))
from items_differential import Original
from navigation_differential import Cpu
REF=ROOT/'reference/prince-live-attack-services';ELF=REPO/'.local-inputs/libDungeonHunter2.so'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(*x):return struct.pack('<'+'I'*len(x),*(v&0xffffffff for v in x))
def fbits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def value(x):return struct.unpack('<f',words(x))[0]
def main():
 manifests=[json.loads((REF/d/'original-functions.json').read_text()) for d in ('reference','helpers','leaf','reader','producer','equipment')]
 manifest={'original_sha256':manifests[0]['original_sha256'],'functions':[f for m in manifests for f in m['functions']]};assert sha(ELF)==manifest['original_sha256']
 c=Original(ELF,manifest);w=c.word
 with ELF.open('rb') as f:
  e=ELFFile(f);symbols={s['st_value']:s.name for s in e.get_section_by_name('.symtab').iter_symbols() if s['st_value'] and s.name}
 def bound(at):return {'address':hex(at),'symbol':symbols.get(at,'unresolved')}
 vt=0x965f38;bindings={'character_vtable':hex(vt),'slots':{hex(k):bound(w(vt+k)) for k in (0x24,0x28,0x34,0x88,0x90,0x94,0xc4,0x124,0x128)},'float_helpers':{}}
 for k in (0x30e2f8,0x30e9ac,0x30e4b4,0x30e70c):
  before=c.import_calls.copy();result=c.invoke(k,[fbits(1),fbits(2)]);calls=[name for name,count in c.import_calls.items() if count>before.get(name,0)];assert len(calls)==1
  bindings['float_helpers'][hex(k)]={'import':calls[0],'one_vs_two_result':result}
 got=0x3d0034+w(0x3d0074);listvt=w(got+w(0x3d007c))+8;bindings['source_object_list']={'vtable':hex(listvt),'symbol':symbols.get(listvt-8,'unresolved'),'slots':{hex(k):bound(w(listvt+k)) for k in (8,12,16,24,28)}}
 # Replay original ItemRecord gold into its original scalar row layout.
 goldpath=REPO/'port/game-data/reference/items/record-fixtures.bin';blob=goldpath.read_bytes();magic,records,count,*_=struct.unpack_from('<7I',blob);assert magic==0x314d5449 and count==1322;at=28;rows=[]
 for i in range(records):
  n=struct.unpack_from('<I',blob,at)[0];at+=4+n;row=blob[at:at+164];at+=164
  for j in range(2):n=struct.unpack_from('<I',blob,at)[0];at+=4+n
  if i<count:rows.append(row)
 owner=c.data+0x10000;target=c.data+0x14000;inventory=owner+0x37c;entries=c.data+0x18000;equip=entries+0x100;ref=entries+0x200;instance=entries+0x300;out=entries+0x400;items=c.data+0x20000
 c.uc.mem_write(owner,bytes(0x1800));c.pointer(inventory,0x967768);c.pointer(inventory+0x14,entries);c.pointer(entries,equip);c.pointer(equip+4,ref);c.pointer(ref,instance)
 c.uc.mem_write(items,b''.join(rows));ptr=entries+0x800;got=0x3f9e1c+w(0x3f9e2c);c.pointer(got+w(0x3f9e30),ptr);c.pointer(ptr,items)
 invrecords=[]
 for i,row in enumerate(rows):
  c.pointer(instance+4,i);c.uc.mem_write(out,words(0x12345678,0x23456789,0x3456789a));melee=c.invoke(0x3fff30,[inventory,out]);m=w(out)
  c.uc.mem_write(out,words(0x12345678,0x23456789,0x3456789a));ranged=c.invoke(0x3ffebc,[inventory,out,out+4,out+8]);limits=struct.unpack('<3I',c.uc.mem_read(out,12));kind=struct.unpack_from('<i',row,88)[0];isrange=kind in (4,5)
  assert melee==int(not isrange) and ranged==int(isrange) and m==(0x12345678 if isrange else struct.unpack_from('<I',row,156)[0]);assert limits==(struct.unpack_from('<3I',row,152) if isrange else (0x12345678,0x23456789,0x3456789a))
  invrecords.append({'item':i,'type':kind,'melee':melee,'melee_radius_word':m,'ranged':ranged,'ranged_parameter_words':limits})
 # Full original AIProps reader produces every radius row from actual bytes.
 aipath=REPO/'port/android-native/app/src/main/assets/data/ai_pyarray.bin';c.blob=aipath.read_bytes();c.cursor=4;aicount=struct.unpack_from('<I',c.blob)[0];aibase=c.data+0x60000;airows=[]
 for i in range(aicount):
  address=aibase+i*68;c.uc.mem_write(address,bytes(68));c.invoke(0x506f3c,[address,c.stream]);airows.append(w(address+32))
 assert c.cursor==len(c.blob)
 got=0x3d4c68+w(0x3d4c94);aiptr=entries+0x900;c.pointer(got+w(0x3d4c98),aiptr);c.pointer(aiptr,aibase)
 got=0x3a3000+w(0x3a301c);aicountptr=entries+0x910;c.pointer(got+w(0x3a3020),aicountptr);c.pointer(aicountptr,aicount)
 c.pointer(owner+0xffc,44);ai=owner+0x3c8;c.pointer(ai+4,owner);radiusrecords=[]
 for id in (-1,0,8,44,aicount):
  c.pointer(owner+0xffc,id&0xffffffff);selected=id if 0<=id<aicount else 8
  for item in (-1,0,1,10,50,100,500,1000,1321):
   c.pointer(equip+4,0 if item<0 else ref);c.pointer(instance+4,max(item,0));raw=c.invoke(0x3d4c34,[ai]);kind=struct.unpack_from('<i',rows[item],88)[0] if item>=0 else -1
   weapon=struct.unpack_from('<i',rows[item],156)[0] if item>=0 and kind not in (4,5) else 0;expected=fbits(float(weapon)+value(airows[selected]));assert raw==expected
   radiusrecords.append({'ai_property':id,'selected_ai':selected,'item':item,'radius_bits':raw})
 # Independent original geometric bodies with named radius/range/handle fixtures.
 p=Cpu(ELF,False,manifest);po=p.data+0x1000;pa=po+0x3c8;pt=p.data+0x4000;pvt=p.data+0x8000;pv=[p.data+0x9000+i*16 for i in range(2)];facts={};trace=[]
 for v in pv:p.uc.mem_write(v,bytes.fromhex('1eff2fe1'))
 p.pointer(po,pvt);p.pointer(pt,pvt);p.pointer(pvt+0x90,pv[0]);p.pointer(pvt+0x128,pv[1]);p.pointer(pa+4,po)
 def ret(v=0):p.put(0,v);p.uc.reg_write(p.pc,p.uc.reg_read(p.lr))
 def hook(uc,address,size,unused):
  if address==0x33ff8c:trace.append('handle');ret(pt)
  elif address==pv[0]:trace.append('interaction');ret(facts['interaction'])
  elif address==pv[1]:
   trace.append('range_parameters')
   for j,v in enumerate(facts['limits']):p.uc.mem_write(p.reg(1+j),words(v))
   ret(facts['range_available'])
  elif address==0x3d4c34:trace.append('owner_radius' if p.reg(0)==pa else 'target_radius');ret(fbits(facts['owner_radius'] if p.reg(0)==pa else facts['target_radius']))
  elif address==0x3d4f98:trace.append('interaction_range');ret(17)
  elif address in (0x337888,0x3140ec,0x318254,0x708f00,0x310440):ret()
  elif address==0x337a88:ret(0)
 p.uc.hook_add(UC_HOOK_CODE,hook);ranges=[]
 for kind,present,explicit,ctype,interaction,distance in itertools.product((0,1),(0,1),(0,1),(0,1),(7,8),(0,49.5,50,99.5,100,100.5,101)):
  facts={'interaction':interaction,'range_available':1,'limits':(50,100,-1),'owner_radius':60.,'target_radius':40.};p.pointer(pa+0x40,pt if present else 0);p.pointer(pt+0xf4,ctype);p.uc.mem_write(po+0x160,struct.pack('<3f',0,0,0));p.uc.mem_write(pt+0x160,struct.pack('<3f',distance,0,0));trace.clear();result=p.invoke((0x3d6188,0x3d6604)[kind],[pa,pt if explicit else 0]);exists=present or explicit
  expected=0 if not exists else int(50*50<=distance*distance and 100*100>=distance*distance) if kind else 17 if ctype or interaction!=8 else int(100*100>distance*distance)
  assert result==expected,(kind,present,explicit,ctype,interaction,distance,result,expected)
  ranges.append({'kind':kind,'present':present,'explicit':explicit,'character_type':ctype,'interaction':interaction,'distance':distance,'result':result,'services':trace.copy()})
 report={'validation':'PASS','original_only_cases':len(invrecords)+len(radiusrecords)+len(ranges),'native_comparisons':0,'mismatches':0,'original_sha256':sha(ELF),'script_sha256':sha(Path(__file__)),'bindings':bindings,'item_corpus_sha256':sha(goldpath),'ai_asset_sha256':sha(aipath),'original_ai_reader_rows':aicount,'prince_row44_melee_radius_bits':airows[44],'inventory_queries':invrecords,'radius_queries':radiusrecords,'range_queries':ranges,'scope':__doc__}
 (REF/'dependency-probes.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if not k.endswith('_queries')}))
if __name__=='__main__':main()
