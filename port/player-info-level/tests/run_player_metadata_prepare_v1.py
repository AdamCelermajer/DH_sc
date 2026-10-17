"""Bounded Manage metadata regions and actual setters versus selected code."""
from __future__ import annotations
import argparse,hashlib,json,os,random,struct,subprocess,uuid
from pathlib import Path
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R8,UC_ARM_REG_PC,UC_ARM_REG_SP,UC_ARM_REG_LR
from player_locality_v1_original import image,get,put,signed,ELF_SHA
from run_player_info_record_v1 import Original as RecordOriginal,BASES,SIZE,STOP
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/player-info-level'
PIN=MODULE/'reference/player-metadata-prepare-v1/original-functions.json'
BASE=BASES[0];SAVE=0x10003000;CSTRING=0x10004000;DEBUG=0x1000c000;GAME=0x1000c100
ENTRY=0x3728b8;MIN=-2147483648
BOUNDARIES={0x372990,0x372ec0,0x372f50,0x372b6c,0x372908,0x372f9c}
def names(index):
    values=[(b'Rogue',b'Rogue'),(b'Warrior',b'Rogue'),(b'',b'Mage'),(b'A',b'A\0B'),(b'A\0B',b'A'),(b'Warrior','מכשף'.encode()),(b'A'*45,b'B'*46),(b'',b'')]
    return values[index]
def cases():
    base=[1,1,2,0,0,0,0,-1,1,1,2,7,1,9,0,0,MIN,0,MIN,MIN];rows=[]
    def add(**changes):
        row=base.copy()
        for k,v in changes.items():row[int(k[1:])]=v
        rows.append(row)
    for active in [0,1,7]:
        for local in [0,1,7,255]:
            for slot in [-1,0,2,-2]:
                for existing in [0,1]:add(v0=active,v1=local,v2=slot,v3=existing)
    for initial in [0,1]:
        for changed in [-1,0,1]:
            for variant in range(8):add(v6=initial,v7=changed,v12=variant)
    for cls in [-2147483648,-1,0,1,2,2147483647]:
        for old in [cls,1]:
            for existing in [0,1]:add(v8=old,v10=cls,v3=existing)
    for level in [-2147483648,-1,0,1,9,2147483647]:
        for old in [level,1]:add(v9=old,v11=level)
    for residue in [0,-1,1,7,-2147483648]:
        for serial in [0,9,0xffffffff,-1]:add(v13=serial,v14=residue,v15=residue)
    for debug,game in [(0,0),(1,0),(7,0),(0,1),(0,255)]:add(v4=debug,v5=game)
    for slot in [-1,0,7,-2]:add(v16=slot)
    for existing in [0,1]:add(v17=existing)
    for cls,level in [(0,0),(3,9),(-1,-1),(MIN,MIN)]:add(v18=cls,v19=level)
    rng=random.Random(ENTRY)
    for _ in range(30):add(v3=rng.randrange(2),v6=rng.randrange(2),v7=rng.choice([-1,0,1]),v8=rng.choice([-1,0,2]),v9=rng.choice([-1,0,7]),v10=rng.choice([-1,0,2]),v11=rng.choice([-1,0,7]),v12=rng.randrange(8),v13=rng.choice([0,9,-1]),v14=rng.choice([-1,0,7]),v15=rng.choice([-1,0,7]))
    return rows
class Original(RecordOriginal):
    def __init__(self,path):
        self.in_metadata=False
        super().__init__(path)
        data,symbols,_=image(path);self.metadata_pins=json.loads(PIN.read_text())
        for pin in self.metadata_pins['functions']+self.metadata_pins['declared_callee_dependencies']:
            address=int(pin['elf_address'],0);symbol=symbols[pin['original_symbol']]
            assert (symbol['st_value'],symbol['st_size'])==(address,pin['size'])
            assert hashlib.sha256(data[address:address+pin['size']]).hexdigest()==pin['sha256']
        self.metadata_ranges=[]
        for pin in self.metadata_pins['reached_regions']:
            address=int(pin['elf_address'],0);size=pin['size']
            assert hashlib.sha256(data[address:address+size]).hexdigest()==pin['sha256']
            self.metadata_ranges.append((address,size))
        self.ranges+=self.metadata_ranges
    def hook(self,u,address,size,context):
        if not self.in_metadata:return super().hook(u,address,size,context)
        row=self.row
        if address in BOUNDARIES:u.emu_stop();return
        a,b,c,d=[u.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]]
        def ret(value=0):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
        if address==0x36d48c:
            assert a==BASE;self.meta_trace.append([0,row[0],0])
            if row[7]>=0:put(u,BASE+0x660,0x10008000 if row[7] else 0)
            if row[16]!=MIN:put(u,BASE+0x664,row[16])
            if row[17]:put(u,BASE+0x680,SAVE)
            ret(row[0]);return
        if address==0x310570:assert (a,b)==(0x198,0);self.meta_trace.append([1,a,b]);ret(SAVE);return
        if address==0x4655ac:assert (a,c,d)==(SAVE,1,0);self.meta_trace.append([2,signed(b),c]);ret(a);return
        if address==0x32bd08:self.meta_trace.append([3,row[4],0]);u.mem_write(DEBUG+0x11,bytes([row[4]]));ret(DEBUG);return
        if address==0x320e98:
            self.meta_trace.append([4,row[5],0]);u.mem_write(GAME+0x28,bytes([row[5]]))
            if row[18]!=MIN:put(u,SAVE+0x34,row[18])
            if row[19]!=MIN:put(u,SAVE+0x30,row[19])
            ret(GAME);return
        if address==0x313c48:
            text=bytes(u.mem_read(b,256)).split(b'\0',1)[0];ret(int(self.text(a)==text));return
        if address==0x371ccc:self.meta_trace.append([6,0,0])
        if address==0x370ef0:self.meta_trace.append([7,signed(b),0])
        if address==0x370e48:self.meta_trace.append([8,signed(b),0])
        if address in [0x370f04,0x370e5c]:put(u,u.reg_read(UC_ARM_REG_SP)+0x20,row[14 if address==0x370f04 else 15])
        super().hook(u,address,size,context)
    def execute(self,row):
        self.in_metadata=False;self.prepare(bytes(SIZE),0);u=self.u
        self.row=row;self.meta_trace=[];put(u,BASE+0x660,0x10008000 if row[6] else 0)
        u.mem_write(BASE+0x66c,bytes([row[1]]));put(u,BASE+0x664,row[2]);put(u,BASE+0x680,SAVE if row[3] else 0)
        put(u,BASE+0x380,row[8]);put(u,BASE+0x330,row[9]);put(u,SAVE+0x34,row[10]);put(u,SAVE+0x30,row[11])
        player_name,saved_name=names(row[12]);self.string(BASE+0x2d0,player_name)
        u.mem_write(CSTRING,saved_name+b'\0');put(u,SAVE+0x2c,CSTRING)
        u.mem_write(self.serial,struct.pack('<Q',row[13]&0xffffffffffffffff))
        for offset,revision in [(0x2b0,123),(0x360,456),(0x310,789)]:u.mem_write(BASE+offset+8,struct.pack('<Q',revision))
        self.words=set();self.in_metadata=True;u.reg_write(UC_ARM_REG_SP,0x20007800);u.reg_write(UC_ARM_REG_LR,STOP)
        u.reg_write(UC_ARM_REG_R0,BASE);u.reg_write(UC_ARM_REG_R8,0x994a98)
        u.emu_start(ENTRY,STOP+4,count=30000);assert u.reg_read(UC_ARM_REG_PC) in BOUNDARIES
        result=dict(loading=int(bool(get(u,BASE+0x680))),character=int(bool(get(u,BASE+0x660))),**{'class':signed(get(u,BASE+0x380))},level=signed(get(u,BASE+0x330)),name=self.text(BASE+0x2d0).hex(),serial=struct.unpack('<Q',u.mem_read(self.serial,8))[0],revisions=[struct.unpack('<Q',u.mem_read(BASE+offset+8,8))[0] for offset in [0x2b0,0x360,0x310]],trace=self.meta_trace)
        self.in_metadata=False;return result,self.words.copy()
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--original',type=Path,required=True);parser.add_argument('--output',type=Path,required=True);parser.add_argument('--compiler',required=True);parser.add_argument('--library',type=Path,required=True);parser.add_argument('--cache',type=Path);args=parser.parse_args()
    args.output.mkdir(parents=True,exist_ok=True);rows=cases();original=Original(args.original);expected=[];words=set()
    for row in rows:result,reached=original.execute(row);expected.append(result);words|=reached
    inputs=args.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n',encoding='utf-8')
    exe=args.output/'metadata_host.exe';library=args.library.resolve();data_library=next(library.parent.rglob('libdh2_game_data.dll.a'))
    command=[args.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',str(MODULE/'tests/player_metadata_prepare_v1_host.cpp'),str(library),str(data_library),'-o',str(exe)]
    build=subprocess.run(command,capture_output=True,text=True);assert build.returncode==0,build.stderr
    dlls=sorted(library.parent.parent.rglob('*.dll'));env=os.environ.copy();env['PATH']=os.pathsep.join([*(str(p.parent) for p in dlls),str(Path(args.compiler).parent),env['PATH']])
    native=subprocess.run([str(exe),str(inputs)],env=env,capture_output=True,text=True);assert native.returncode==0,native.stderr
    actual=json.loads(native.stdout);assert len(actual['results'])==len(rows)
    mismatches=[dict(index=i,input=row,original=e,native=n) for i,(row,e,n) in enumerate(zip(rows,expected,actual['results'])) if e!=n]
    (args.output/'comparison.json').write_text(json.dumps(dict(original=expected,native=actual,mismatches=mismatches),indent=2)+'\n',encoding='utf-8')
    commands=json.loads((library.parent.parent/'compile_commands.json').read_text());selected=[entry for entry in commands if Path(entry['file']).name=='player_metadata_prepare_v1.cpp'];assert len(selected)==1
    sources=['player_metadata_prepare_v1.cpp','player_metadata_prepare_v1.hpp','tests/player_metadata_prepare_v1_host.cpp','tests/run_player_metadata_prepare_v1.py','reference/player-metadata-prepare-v1/original-functions.json']
    report=dict(status='FAIL' if mismatches else 'PASS',arm_comparisons=len(rows),distinct_pinned_words=len(words),native_policy_checks=actual['policy_checks'],source_sha256={source:hashlib.sha256((MODULE/source).read_bytes()).hexdigest() for source in sources},binary_sha256={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in [exe,*dlls]},selected_commands=selected,scope=original.metadata_pins['scope'],original_sha256=ELF_SHA,android_compilation=False,live_gameplay=False,mismatches=mismatches[:3])
    if args.cache:
        cache=args.cache.resolve();cache_files=['character_properties_pyarray.bin','character_properties_pyarraynames.bin','character_properties_pystructnames.bin']
        cache_hashes={name:hashlib.sha256((cache/name).read_bytes()).hexdigest() for name in cache_files}
        test=MODULE/'tests/player_metadata_prepare_v1_integration.cpp';transport=ROOT/'port/android-native/app/src/main/cpp/native_player_profile.cpp';composition_exe=args.output/'metadata_composition.exe'
        build=subprocess.run([args.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2','-I'+str(ROOT/'port/game-data'),'-I'+str(ROOT/'port/level-world'),str(test),str(transport),str(library),str(data_library),'-o',str(composition_exe)],capture_output=True,text=True)
        assert build.returncode==0,build.stderr
        composed=subprocess.run([str(composition_exe),str(cache),str(args.output/('synthetic-'+uuid.uuid4().hex))],env=env,capture_output=True,text=True);assert composed.returncode==0,composed.stderr
        report['selected_composition']=json.loads(composed.stdout);assert report['selected_composition']['validation']=='PASS'
        assert cache_hashes=={name:hashlib.sha256((cache/name).read_bytes()).hexdigest() for name in cache_files}
        report['cache_sha256']=cache_hashes;report['binary_sha256'][composition_exe.name]=hashlib.sha256(composition_exe.read_bytes()).hexdigest()
        for source in [test,transport,transport.with_suffix('.hpp')]:report['source_sha256'][source.relative_to(ROOT).as_posix()]=hashlib.sha256(source.read_bytes()).hexdigest()
    (args.output/'report.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','binary_sha256','selected_commands','mismatches']},indent=2))
    if mismatches:print(json.dumps(mismatches[:1],indent=2))
    return 1 if mismatches else 0
if __name__=='__main__':raise SystemExit(main())
