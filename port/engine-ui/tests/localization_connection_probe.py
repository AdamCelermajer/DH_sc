"""Full original NativeGetStringFromSymbol body; explicit AS/owner services."""
import hashlib,json,struct
from pathlib import Path
from localization_transform_probe import Cpu,ROOT,REPO
from unicorn import UC_HOOK_CODE
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(*v):return struct.pack('<'+'I'*len(v),*v)
def span(s):return words(len(s))+s
def constants(path):
 raw=path.read_bytes();at=0
 def w():
  nonlocal at
  v=struct.unpack_from('<I',raw,at)[0];at+=4;return v
 def s():
  nonlocal at
  n=w();v=raw[at:at+n];at+=n;return v
 groups={}
 for _ in range(w()):
  group=s();entries={}
  for _ in range(w()):key=s();entries[key]=w()
  groups[group]=entries
 assert at==len(raw);return groups
def main():
 assets=REPO/'port/android-native/app/src/main/assets/original-cache/data';cst=constants(assets/'pydata/common_text_pycst.bin');cst.update(constants(REPO/'port/android-native/app/src/main/assets/data/fonts_pycst.bin'));c=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});c.pointer(0x99f698,1)
 def word(at):return struct.unpack('<I',c.uc.mem_read(at,4))[0]
 metadata=json.loads((REPO/'.local-inputs/localization-discovery/common-text-probe.json').read_text());arena=c.data+0x1000000
 def allocate(raw):
  nonlocal arena
  at=arena;c.uc.mem_write(at,raw);arena=(at+len(raw)+15)&~15;return at
 # Exact original-derived outputs for every caret/pipe entry, retaining raw
 # bytes on parseColors' false return exactly as source preload does.
 rawgold=(ROOT/'reference/localization/localization-fixtures.bin').read_bytes();cursor=8;processed={}
 for _ in range(struct.unpack_from('<I',rawgold,4)[0]):
  op,n=struct.unpack_from('<II',rawgold,cursor);cursor+=8;payload=rawgold[cursor:cursor+n];cursor+=n;n=struct.unpack_from('<I',rawgold,cursor)[0];cursor+=4;expected=rawgold[cursor:cursor+n];cursor+=n
  if op==0:
   spacing=struct.unpack_from('<I',payload)[0];n=struct.unpack_from('<I',payload,44)[0];value=payload[48:48+n];colors=struct.unpack_from('<10I',payload,4)
   if list(colors)==[cst[b'FontTextColors'][k]for k in (b'zero',b'one',b'two',b'three',b'four',b'five',b'six',b'seven',b'eight',b'nine')]:processed[spacing,value]=expected[4:]if struct.unpack_from('<I',expected)[0]else value
 manager=c.data+0x1000;c.uc.mem_write(manager,bytes(0x800));table=allocate(bytes(9*12));c.pointer(word(0x994a98+word(0x5075ac)),table)
 for row in metadata['packs']:
  pack=row['pack'];sheetrows=allocate(bytes(37*20));c.pointer(table+pack*12+4,37);c.pointer(table+pack*12+8,sheetrows)
  for record in row['sheets']:
   sheet=record['sheet'];name=record['name'].encode();filename=record['filename'][5:].encode();sr=sheetrows+sheet*20;c.pointer(sr+4,len(filename));c.pointer(sr+8,allocate(filename+b'\0'));c.pointer(sr+12,len(name));c.pointer(sr+16,allocate(name+b'\0'))
   raw=(assets/record['filename']).read_bytes();count=struct.unpack_from('<H',raw)[0];at=2;values=[]
   for _ in range(count):n=struct.unpack_from('<H',raw,at)[0];at+=2;value=raw[at:at+n];at+=n;values.append(allocate(processed.get((4<=pack<=6,value),value)+b'\0'))
   c.pointer(manager+(pack*37+sheet+2)*4,allocate(words(*values)));c.uc.mem_write(manager+0x53c+(pack*37+sheet)*2,struct.pack('<H',count))
 app=word(0x994a98+word(0x445060));c.pointer(app+0x34,manager);c.pointer(app+0x2c,1);c.pointer(app+0x40,2)
 player=allocate(bytes(0x700));character=allocate(bytes(0x1500));profile=allocate(bytes(0x40));nameptr=allocate(b'Prince\0');c.pointer(character+0x14e8,profile);c.pointer(profile+0x2c,nameptr)
 symbol=c.data+0x4000;tu=symbol+0x1000;fn=tu+0x1000;args=fn+0x100;value=args+0x100;result=value+0x100;c.pointer(fn,result);c.pointer(fn+12,args);c.pointer(fn+20,0);c.pointer(args,value);c.uc.mem_write(tu,b'\xff'+bytes(15));c.pointer(tu+12,symbol)
 menu_flag=word(0x994a98+word(0x445064));trace=[];delivered=None
 def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def hook(uc,address,size,user):
  nonlocal delivered
  if address==0x796fb4:ret(symbol) # explicit projected AS first text argument
  elif address==0x420a84:ret(tu)
  elif address==0x7972d8:
   p=c.reg(1);p=word(p+12)if c.uc.mem_read(p,1)==b'\xff'else p+1;delivered=c.string(p);ret() # owned AS result publication observer
  elif address==0x337888:trace.append('debug_load');ret()
  elif address==0x337a88:trace.append('debug_query');ret(0)
  elif address==0x4c4bdc:
   group,key=c.string(c.reg(1)),c.string(c.reg(2));trace.append('constant:'+group.decode()+':'+key.decode());ret(cst[group][key])
  elif address==0x36e478:
   assert c.reg(1)==0 and c.reg(2)==1;trace.append('player_character');ret(player)
  elif address==0x3bb7e8:trace.append('player_name') # genuine getter body executes
 c.uc.hook_add(UC_HOOK_CODE,hook);rows=[];gold=[]
 targets=['GAMEPLAYMENUS_FASTTRAVEL']+[f'GLOBAL_DEATH{i}_01'for i in range(1,7)]+['global_death1_01','GLOBAL_MISSING_01','UNKNOWN_SYMBOL','_MISSING','MENU_ERROR_NO_USERNAME','MENU_ERROR_NO_USERNAME_MENU_ERROR_NO_PASSWORD']
 for pack in range(-1,9):
  c.pointer(manager+4,pack&0xffffffff)
  for present in [False,True]:
   c.pointer(player+0x660,character if present else 0)
   for key in targets:
    c.uc.mem_write(symbol,key.encode()+b'\0');c.uc.mem_write(menu_flag,b'\0');trace.clear();delivered=None
    try:c.invoke(0x444ebc,[fn],budget=30000000)
    except Exception:
     print({'pack':pack,'symbol':key,'pc':hex(c.uc.reg_read(c.pc)),'registers':[hex(c.reg(i))for i in range(9)],'trace':trace});raise
    assert delivered is not None;flag=c.uc.mem_read(menu_flag,1)[0];found=delivered!=b'notfound';record={'pack':pack,'present':present,'symbol':key,'text':delivered.decode(),'sets_menu_string_flag':flag,'trace':list(trace)};rows.append(record);payload=words(pack&0xffffffff,present)+span(key.encode())+span(b'Prince');expected=words(found,flag)+span(delivered)+words(len(trace))+b''.join(span(x.encode())for x in trace);gold.append(span(payload)+span(expected))
 p=ROOT/'reference/localization/localization-connection-fixtures.bin';p.write_bytes(words(0x32434f4c,len(gold))+b''.join(gold));report={'validation':'PASS','original_sha256':sha(REPO/'.local-inputs/libDungeonHunter2.so'),'original_NativeGetStringFromSymbol_body_executed':True,'comparisons':len(rows),'menu_flag_address':hex(menu_flag),'gold_sha256':sha(p),'rows':rows,'providers':['original-derived owned333sheet values; actual original ordered common_text metadata','explicit original projected AS text/tu_string and result publication observer','explicit original Debug load/query observers','actual common_text/fonts constants input words','dynamic GetLocalPlayer owner record fixture; actual Character.SG_GetPlayerName body'],'scope':'Actual NativeGetStringFromSymbol→getStringFromSymbol→parse→ParsePlayerName complete text-only HUD path with preloaded owned sheets; original stream/file/language/application/game-player ownership and AS conversion/publication supplied services.'};(ROOT/'reference/localization/connection-probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','comparisons':len(rows),'gold_sha256':sha(p),'first':rows[0]}))
if __name__=='__main__':main()
