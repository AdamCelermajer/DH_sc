"""Actual packaged typed contribution kernels plus packaged quaternion math.

Both APK ELFs execute in one ARM64 instruction CPU. Cross-library calls resolve
to actual packaged dh2_quat_slerp; only imported libc and the existing Windows
UCRT sinf/acosf/sqrtf contract are modeled. This is the original audit's libm
contract, not historical Bionic parity. Copy branches preserve all bits,
including NaNs; arithmetic NaNs compare by classification. No device/build.
"""
import argparse,hashlib,json,struct,sys,time,zipfile
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE,UC_HOOK_BLOCK

REPO=Path(__file__).resolve().parents[3]
ROOT=REPO/'port/engine-animation'
sys.path.insert(0,str(ROOT/'tests'))
from animation_blend_differential import Cpu as BlendCpu,same_word
sys.path.insert(0,str(Path(__file__).resolve().parent))
from run_character_state_packaged import sha,require,write_json,relative,ORIGINAL_SHA
ANIMATION='lib/arm64-v8a/libdh2_engine_animation.so'
MATH='lib/arm64-v8a/libdh2_scene_materials.so'
GOLD=ROOT/'reference/animation-blend/contribution-fixtures.bin'
GOLD_SHA='393c538d7eb584de465e22992b3fc7580b5294bc310e859425471ede8fb71a80'
NAMES=('dh2_animation_blend_scalar','dh2_animation_blend_vector3','dh2_animation_blend_vector3','dh2_animation_blend_quaternion')
WIDTHS=(1,3,3,4)

class MultiCpu(BlendCpu):
 def __init__(self,animation,math):
  super().__init__(animation,True,{'functions':[]})
  self.modules=[];self.definitions={};self.resolved_cross_library=[];self.relocations=Counter()
  self.math_seen=set();self.slerp_calls=0
  for path,base in ((animation,self.base),(math,0x180000000)):
   with path.open('rb') as stream:
    elf=ELFFile(stream);require(elf.elfclass==64 and elf['e_machine']=='EM_AARCH64','packaged ELF64 AArch64 required')
    if path==math:
     loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD'];low=min(s['p_vaddr'] for s in loads)&~4095;high=(max(s['p_vaddr']+s['p_memsz'] for s in loads)+4095)&~4095
     require(base+high<self.data,'secondary module overlaps caller storage');self.uc.mem_map(base+low,high-low)
     for segment in loads:self.uc.mem_write(base+segment['p_vaddr'],segment.data())
    text=elf.get_section_by_name('.text');self.modules.append({'path':path,'base':base,'text_begin':base+text['sh_addr'],'text_end':base+text['sh_addr']+text['sh_size']})
    for section_name in ('.dynsym','.symtab'):
     section=elf.get_section_by_name(section_name)
     if not section:continue
     for symbol in section.iter_symbols():
      if symbol.name and symbol['st_shndx']!='SHN_UNDEF':
       target=symbol['st_value']+(0 if symbol['st_shndx']=='SHN_ABS' else base)
       if symbol.name not in self.definitions:self.definitions[symbol.name]=(target,path)
       if path==math and symbol.name=='dh2_quat_slerp':self.slerp_address=target
  require(hasattr(self,'slerp_address'),'packaged scene materials lacks genuine dh2_quat_slerp')
  # Resolve every dynamic relocation in both modules against actual definitions.
  for module in self.modules:
   with module['path'].open('rb') as stream:
    elf=ELFFile(stream)
    for section in elf.iter_sections():
     if section['sh_type'] not in ('SHT_REL','SHT_RELA'):continue
     symbols=elf.get_section(section['sh_link'])
     for relocation in section.iter_relocations():
      kind=relocation['r_info_type'];address=module['base']+relocation['r_offset'];addend=relocation['r_addend'] if section['sh_type']=='SHT_RELA' else 0
      if kind==1027:self.pointer(address,module['base']+addend)
      elif kind in (1025,1026,257):
       symbol=symbols.get_symbol(relocation['r_info_sym']);name=symbol.name
       if symbol['st_shndx']!='SHN_UNDEF':target=symbol['st_value']+(0 if symbol['st_shndx']=='SHN_ABS' else module['base'])
       elif name in self.definitions:
        target,provider=self.definitions[name]
        if provider!=module['path']:self.resolved_cross_library.append({'consumer':module['path'].name,'symbol':name,'provider':provider.name,'target':hex(target)})
       else:
        target=next((address for address,value in self.imports.items() if value==name),None)
        if target is None:
         target=self.extern+len(self.imports)*16;require(target<self.callback,'import storage exhausted');self.imports[target]=name
       self.pointer(address,target+addend)
      elif kind==0:continue
      else:raise AssertionError(f'Unsupported AArch64 dynamic relocation {kind} in {module["path"].name}')
      self.relocations[kind]+=1
  require(any(row['symbol']=='dh2_quat_slerp' and row['provider']==math.name for row in self.resolved_cross_library),'animation must import actual scene quaternion slerp')
  def slerp(uc,address,size,unused):self.slerp_calls+=1
  def math_block(uc,address,size,unused):
   module=self.modules[1]
   if module['text_begin']<=address<module['text_end']:self.math_seen.update(range(address,min(address+size,module['text_end']),4))
  self.uc.hook_add(UC_HOOK_CODE,slerp,begin=self.slerp_address,end=self.slerp_address)
  self.uc.hook_add(UC_HOOK_BLOCK,math_block)
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ('__memmove_chk','__memset_chk'):
   destination,source,count,capacity=[self.reg(i) for i in range(4)];require(count<=capacity and count<=0x2000000,'checked libc bounds')
   if count:uc.mem_write(destination,bytes((source&255,))*count if name=='__memset_chk' else bytes(uc.mem_read(source,count)))
   self.put(0,destination);self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:return super().external(uc,address,size,unused)

def replay(cpu):
 blob=GOLD.read_bytes();magic,count=struct.unpack_from('<II',blob);require(magic==0x314b4241 and count==5786,'typed blend corpus header/count')
 cursor=8;operations=Counter();copies=0;ordered_libm=0;original_sqrt=0;observed_sqrt=0
 values,weights,output=[cpu.data+offset for offset in (0x1000,0x3000,0x5000)]
 for index in range(count):
  operation,slots,flags,trig_count=struct.unpack_from('<4I',blob,cursor);cursor+=16;require(operation<4 and slots<=65536 and flags<=7,'typed corpus fields')
  width=WIDTHS[operation];value_bytes=blob[cursor:cursor+slots*width*4];cursor+=len(value_bytes);weight_bytes=blob[cursor:cursor+slots*4];cursor+=len(weight_bytes)
  expected=struct.unpack_from('<'+'I'*width,blob,cursor);cursor+=width*4;trace=[struct.unpack_from('<3I',blob,cursor+i*12) for i in range(trig_count)];cursor+=trig_count*12
  cpu.uc.mem_write(values,value_bytes or bytes(16));cpu.uc.mem_write(weights,weight_bytes or bytes(16));guard=struct.pack('<I',0xaabbccdd);cpu.uc.mem_write(output-4,guard*6);cpu.trig=[]
  wp=(values if flags&4 else weights) if slots and not(flags&2) else 0
  require(cpu.invoke(NAMES[operation],[output,values if slots else 0,wp,slots])==0,f'typed blend return {index}')
  actual=struct.unpack('<'+'I'*width,cpu.uc.mem_read(output,width*4))
  require(all(same_word(e,a,bool(flags&1)) for e,a in zip(expected,actual)),f'typed output {index} {operation} {expected} {actual}')
  require(bytes(cpu.uc.mem_read(values,len(value_bytes)))==value_bytes and bytes(cpu.uc.mem_read(weights,len(weight_bytes)))==weight_bytes,f'input mutation {index}')
  require(bytes(cpu.uc.mem_read(output-4,4))==guard and bytes(cpu.uc.mem_read(output+width*4,4))==guard,f'output guard {index}')
  expected_external=[row for row in trace if row[0]!=2];actual_external=[row for row in cpu.trig if row[0]!=2]
  require(len(expected_external)==len(actual_external),f'external libm count {index}')
  for e,a in zip(expected_external,actual_external):require(e[0]==a[0] and same_word(e[1],a[1]) and same_word(e[2],a[2]),f'libm argument/result order {index}')
  # Native compiler may lower sqrtf to FSQRT. Any remaining external sqrt calls
  # must match a source sqrt observation, in order. Full output words remain exact.
  expected_sqrt=[row for row in trace if row[0]==2];actual_sqrt=[row for row in cpu.trig if row[0]==2];at=0
  for actual_call in actual_sqrt:
   while at<len(expected_sqrt) and not all(same_word(x,y) for x,y in zip(expected_sqrt[at][1:],actual_call[1:])):at+=1
   require(at<len(expected_sqrt),f'sqrt dependency mismatch {index}');at+=1
  original_sqrt+=len(expected_sqrt);observed_sqrt+=len(actual_sqrt);ordered_libm+=len(actual_external);operations[operation]+=1;copies+=bool(flags&1)
 require(cursor==len(blob),'typed corpus tail');rejects=0
 for operation in (0,1,3):
  args=((0,values,weights,2),(output,0,weights,2),(output,values,0,2),(output,values,weights,0xffffffff),(output,values,weights,65537),(values,values,weights,2),(weights,values,weights,2),(values+4,values,weights,2),(output+1,values,weights,2),(output,values+1,weights,2),(output,values,weights+1,2),(output,0xfffffffffffffffc,weights,2))
  for argv in args:
   sentinel=struct.pack('<I',0x12345678)*32
   for ptr in (output,values,weights):cpu.uc.mem_write(ptr,sentinel)
   require(cpu.invoke(NAMES[operation],argv)==1,'typed malformed binding accepted')
   require(all(bytes(cpu.uc.mem_read(ptr,len(sentinel)))==sentinel for ptr in (output,values,weights)),'typed malformed mutation');rejects+=1
 require(cpu.slerp_calls>0 and len(cpu.math_seen)>0,'genuine packaged scene quaternion code not executed')
 return {'comparisons':count,'operation_counts':dict(operations),'copy_bit_exact_cases':copies,'ordered_external_sinf_acosf_calls':ordered_libm,'original_sqrtf_observations':original_sqrt,'native_external_sqrtf_calls':observed_sqrt,'atomic_rejection_checks':rejects,'genuine_packaged_slerp_calls':cpu.slerp_calls,'packaged_math_unique_instructions':len(cpu.math_seen),'mismatches':0}

def main():
 p=argparse.ArgumentParser();p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();started=time.monotonic();apk=a.apk.resolve();output=a.output.resolve()
 require(apk.is_file(),'APK missing');require(not output.exists(),'output exists; use a new report path to preserve historical proof');require(sha(GOLD)==GOLD_SHA,'typed gold identity');require(sha(REPO/'.local-inputs/libDungeonHunter2.so')==ORIGINAL_SHA,'original ELF identity')
 directory=REPO/'.local-inputs/animation-blend-packaged'/sha(apk);directory.mkdir(parents=True,exist_ok=True);libraries={}
 with zipfile.ZipFile(apk) as archive:
  for member in (ANIMATION,MATH):
   path=directory/Path(member).name;path.write_bytes(archive.read(member));libraries[member]=path
 cpu=MultiCpu(libraries[ANIMATION],libraries[MATH]);require(all(name in cpu.symbols for name in NAMES),'typed contribution exports missing');result=replay(cpu)
 inputs=[ROOT/'animation_blend.cpp',ROOT/'animation_blend.hpp',REPO/'port/engine-math/math.cpp',REPO/'port/engine-math/math.hpp',ROOT/'tests/animation_blend_differential.py']
 original_report=ROOT/'reports/animation-blend-arm64-differential.json'
 report={'validation':'PASS','actual_packaged_library_executed':True,'apk':str(apk),'apk_sha256':sha(apk),'libraries':{member:{'sha256':sha(path),'path':str(path)} for member,path in libraries.items()},'original_sha256':ORIGINAL_SHA,'reference_sha256':GOLD_SHA,'original_differential_report_sha256':sha(original_report),'source_sha256':{relative(path):sha(path) for path in inputs},'script_sha256':sha(Path(__file__)),'differential':result,'cross_library_resolutions':cpu.resolved_cross_library,'dynamic_relocations':dict(cpu.relocations),'native_import_calls':cpu.import_calls,'libm_dependency_model':'Existing Windows UCRT sinf/acosf/sqrtf model; genuine packaged quaternion/math algorithm executes. Compiler FSQRT allowed; no finite/signed-zero output tolerance. Not a historical Bionic libm claim.','nonfinite_input_policy':'All copy-branch bits exact including signaling/payload NaNs; arithmetic NaNs classification only; finite/signed-zero outputs exact.','scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,3)}
 write_json(output,report);print(json.dumps({key:value for key,value in report.items() if key not in ('source_sha256','cross_library_resolutions','native_import_calls','scope')},indent=2))
if __name__=='__main__':main()
