"""Original triangle-selector mesh constructor and baking vs native ARM64.

The complete CTriangleSelector constructor, format dispatch/createTriangles,
vector reserve/append and baking execute. Mesh-buffer ownership/getters and
GPU buffer mapping are caller services; input bytes remain real scalar/index
streams. Native BRES deserialization, scene clone/ownership and floor metadata
lookup are separate boundaries.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
from selector_differential import Cpu as SelectorCpu,OriginalSelectorOracle
from navigation_differential import equal
from aggro_differential import float_bits
ROOT=Path(__file__).resolve().parents[1]
class Cpu(SelectorCpu):
 def load_asset_library(self,path):
  # Execute the packaged Attribute decoder itself. Never substitute Python
  # conversion for the native cross-library call.
  base=0x110000000;self.asset_symbols={};self.asset_calls=0
  with path.open('rb') as stream:
   elf=ELFFile(stream);loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD'];low=min(s['p_vaddr'] for s in loads)&~4095;high=(max(s['p_vaddr']+s['p_memsz'] for s in loads)+4095)&~4095;self.uc.mem_map(base+low,high-low)
   for s in loads:self.uc.mem_write(base+s['p_vaddr'],s.data())
   for section in ('.dynsym','.symtab'):
    sec=elf.get_section_by_name(section)
    if sec:
     for symbol in sec.iter_symbols():
      if symbol['st_shndx']!='SHN_UNDEF':self.asset_symbols[symbol.name]=base+symbol['st_value']
   for sec in elf.iter_sections():
    if sec['sh_type'] not in ('SHT_REL','SHT_RELA'):continue
    syms=elf.get_section(sec['sh_link'])
    for relocation in sec.iter_relocations():
     kind=relocation['r_info_type']
     if kind==1027:self.pointer(base+relocation['r_offset'],base+relocation['r_addend'])
     elif kind in (1025,1026):
      symbol=syms.get_symbol(relocation['r_info_sym'])
      if symbol['st_shndx']!='SHN_UNDEF':target=self.asset_symbols[symbol.name]
      else:target=self.extern+len(self.imports)*16;assert target<self.callback;self.imports[target]=symbol.name
      self.pointer(base+relocation['r_offset'],target)
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ('dh2_attribute_read','dh2_node_matrix') and hasattr(self,'asset_symbols'):
   self.asset_calls+=1;uc.reg_write(self.pc,self.asset_symbols[name])
  elif name=='__aeabi_ui2f':
   self.put(0,float_bits(float(self.reg(0))));self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif not self.arm64 and address==self.callback+176:
   self.mesh_getters+=1;self.put(0,len(self.mesh_buffers));uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif not self.arm64 and address==self.callback+192:
   self.mesh_getters+=1;self.pointer(self.reg(0),self.mesh_buffers[self.reg(2)]);uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif not self.arm64 and address==self.callback+208:
   self.put(0,self.local_box);uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif name=='memcmp':
   length=self.reg(2);assert length<=4096
   left=bytes(uc.mem_read(self.reg(0),length));right=bytes(uc.mem_read(self.reg(1),length))
   result=next((a-b for a,b in zip(left,right) if a!=b),0)
   self.put(0,result&0xffffffff);self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif name=='memchr':
   source=self.reg(0);length=self.reg(2);assert length<=4096
   index=bytes(uc.mem_read(source,length)).find(bytes([self.reg(1)&255])) if length else -1
   self.put(0,source+index if index>=0 else 0);self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif name=='strstr':
   def text(at):
    raw=bytes(uc.mem_read(at,4096));return raw[:raw.index(0)]
   source=self.reg(0);index=text(source).find(text(self.reg(1)));self.put(0,source+index if index>=0 else 0);self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)

class OriginalMeshOracle(OriginalSelectorOracle):
 def __init__(self,engine,manifest):
  super().__init__(engine,manifest,Cpu);c=self.cpu;c.mesh_getters=0;c.local_box=c.data+0x800000;self.mesh=c.data+0x600000;self.mesh_vt=self.mesh+0x100;self.input=self.mesh+0x200;self.mappings={};self.maps=0;self.stage=None;c.pointer(self.mesh,self.mesh_vt);c.pointer(self.mesh_vt+0x10,c.callback+176);c.pointer(self.mesh_vt+0x14,c.callback+192)
  def hook(uc,address,size,unused):
   if address==0x5a1adc:
    assert c.reg(1)==1 and c.reg(0) in self.mappings;self.maps+=1;uc.mem_write(c.reg(0)+0x13,b'\2');c.put(0,self.mappings[c.reg(0)]);uc.reg_write(c.pc,uc.reg_read(c.lr))
   elif self.stage=='flags' and address==0x520c60 or self.stage=='bounds' and address==0x520d68:uc.reg_write(c.pc,c.stop)
  c.uc.hook_add(UC_HOOK_CODE,hook)
 def create(self,parts,node,bake):
  c=self.cpu;self.heap=c.data+0x200000;self.mappings.clear();c.mesh_buffers=[];raw_at=c.data+0x700000
  for i,part in enumerate(parts):
   vertex,indices,vertices,draw,kind,components,stride,primitive=part;base=self.mesh+0x1000+i*0x400;streams=base+0x100;vb=base+0x200;ib=base+0x240;uc=c.uc;uc.mem_write(base,bytes(0x400));c.mesh_buffers.append(base);uc.mem_write(base+4,struct.pack('<I',64));c.pointer(base+0x14,streams);uc.mem_write(streams,struct.pack('<I',3));c.pointer(streams+0x14,vb);uc.mem_write(streams+0x1e,struct.pack('<3H',kind,components,stride));uc.mem_write(base+0x20,struct.pack('<I',draw));uc.mem_write(base+0x2e,struct.pack('<H',primitive));uc.mem_write(raw_at,vertex);self.mappings[vb]=raw_at;raw_at+=(len(vertex)+15)&~15
   if indices:c.pointer(base+0x18,ib);uc.mem_write(raw_at,indices);self.mappings[ib]=raw_at;raw_at+=(len(indices)+15)&~15
  c.pointer(self.input,self.mesh)
  if node:c.uc.mem_write(c.node_matrix,node[:65])
  c.invoke(0x596598,[c.selector,self.input,self.node if node else 0,bake],budget=20000000)
  first,last=struct.unpack('<2I',c.uc.mem_read(c.selector+0xc,8));assert (last-first)%36==0 and last-first<=4000000
  return bytes(c.uc.mem_read(first,last-first)) if last!=first else b''
 def flags(self,words,tags):
  c=self.cpu;at=c.data+0x800100;c.uc.mem_write(at,tags+b'\0');c.pointer(self.floor+0x3c,at);c.uc.mem_write(self.floor+0x20,struct.pack('<2I',words[1],words[0]));c.put(4,self.floor);self.stage='flags';c.invoke(0x520bd4,[]);self.stage=None;obj,floor=struct.unpack('<2I',c.uc.mem_read(self.floor+0x20,8));return struct.pack('<2I',floor,obj)
 def bounds(self,local,matrix):
  c=self.cpu;c.uc.mem_write(c.local_box,local);c.uc.mem_write(self.node+0x24,matrix[:65]);c.uc.mem_write(self.node+0x11c,struct.pack('<I',0x100));c.pointer(c.data+0x5030,c.callback+208);c.pointer(c.data+0x5034,0x59770c);world_ptr=c.invoke(0x59770c,[self.node]);world=bytes(c.uc.mem_read(world_ptr,24));assert struct.unpack('<I',c.uc.mem_read(self.node+0x11c,4))[0]==0 and c.invoke(0x59770c,[self.node])==world_ptr
  c.pointer(self.floor+0x40,self.node);c.uc.mem_write(self.point,bytes(12));c.put(4,self.floor);self.stage='bounds';c.invoke(0x520ce4,[self.point]);self.stage=None;return world,bytes(c.uc.mem_read(self.floor+0x44,24))

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--asset-library',type=Path);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args();start=time.monotonic();manifest=json.loads((ROOT/'reference/floor-source/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];oracle=OriginalMeshOracle(a.engine,manifest);new=Cpu(a.library,True,{'functions':[]});rng=random.Random(20261011)
 if a.asset_library:new.load_asset_library(a.asset_library)
 descriptors=new.data+0x1000;count_new=new.data+0x2000;output=new.data+0x10000;node_new=new.data+0x200000;stream_new=new.data+0x300000;records=[];triangle_count=0;baked=0;unindexed=0;type_counts={str(i):0 for i in range(7)};component_counts={};skipped=0
 def pack_matrix(mode):
  values=[float(i%5==0) for i in range(16)]
  if mode==1:return struct.pack('<16fI',*values,1)
  if mode==2:values=[rng.uniform(-2,2) for _ in range(16)];values[12:15]=[rng.uniform(-10000,10000) for _ in range(3)]
  return struct.pack('<16fI',*values,0)
 def make_part(kind,components,indexed=True,primitive=6):
  widths=(1,1,2,2,4,4,4);codes=('b','B','h','H','i','I','f');limits=((-128,127),(0,255),(-32768,32767),(0,65535),(-2147483648,2147483647),(0,4294967295),None);vertices=12;stride=components*widths[kind]+rng.choice((0,4,8));data=b''
  for v in range(vertices):
   values=[rng.uniform(-10000,10000) if kind==6 else rng.randint(*limits[kind]) for _ in range(components)];data+=struct.pack('<'+codes[kind]*components,*values)+bytes(stride-components*widths[kind])
  draw=rng.choice((3,6,9,12));indices=struct.pack('<'+'H'*draw,*(rng.randrange(vertices) for _ in range(draw))) if indexed else b''
  return data,indices,vertices,draw,kind,components,stride,primitive
 cases=[]
 for kind in range(7):
  for components in (1,2,3,4):
   for indexed in (False,True):
    for mode in (0,1,2):cases.append(([make_part(kind,components,indexed)],pack_matrix(mode) if mode else None,mode!=0))
 for i in range(160):
  parts=[make_part(rng.randrange(7),rng.choice((1,2,3,4)),rng.choice((False,True)),rng.choice((6,6,6,4))) for _ in range(rng.randrange(1,5))];cases.append((parts,pack_matrix(i%3) if i%4 else None,bool(i%2)))
 # Constructor baking ignores the stored identity hint, including a hint
 # inconsistent with the numeric matrix. Query transforms have other rules.
 for i in (14,26,38,50,62,74,86,98,110,122,134,146,158):
  parts,node,bake=cases[i];assert node and bake;cases[i]=(parts,node[:64]+struct.pack('<I',1),bake)
 for ci,(parts,node,bake) in enumerate(cases):
  expected=oracle.create(parts,node,bake);raw_at=stream_new;packed_parts=[]
  for vertex,indices,vertices,draw,kind,components,stride,primitive in parts:
   vertex_ptr=raw_at;new.uc.mem_write(raw_at,vertex);raw_at+=(len(vertex)+15)&~15;index_ptr=raw_at if indices else 0
   if indices:new.uc.mem_write(raw_at,indices);raw_at+=(len(indices)+15)&~15
   packed_parts.append(struct.pack('<Q4IQII',vertex_ptr,kind,components,stride,vertices,index_ptr,draw,primitive));type_counts[str(kind)]+=1;component_counts[str(components)]=component_counts.get(str(components),0)+1;skipped+=primitive!=6 or components==1;unindexed+=not bool(indices)
  new.uc.mem_write(descriptors,b''.join(packed_parts));new.uc.mem_write(count_new,b'\xcc'*4)
  if node:new.uc.mem_write(node_new,node)
  status=new.invoke('dh2_floor_mesh_triangles',[output,4096,count_new,descriptors,len(parts),node_new if node else 0,int(bake)],budget=20000000);count=struct.unpack('<I',new.uc.mem_read(count_new,4))[0];actual=bytes(new.uc.mem_read(output,count*36));assert status==0 and equal(expected,actual),(ci,'mesh extraction',len(expected)//36,count,expected.hex(),actual.hex());triangle_count+=count;baked+=bool(node and bake)
  rows=[]
  for vertex,indices,vertices,draw,kind,components,stride,primitive in parts:rows.append(struct.pack('<8I',vertices,draw,kind,components,stride,primitive,len(vertex),len(indices))+vertex+indices)
  records.append(struct.pack('<III',len(parts),int(node is not None),int(bake))+(node or pack_matrix(1))+b''.join(rows)+struct.pack('<I',count)+expected)
 flag_records=[];bounds_records=[];flags_new=new.data+0x210000;tags_new=new.data+0x211000;bounds_new=new.data+0x212000;local_new=new.data+0x213000
 tags=[b'',b'void',b'wall',b'hole',b'water',b'voidwallholewater',b'VOID WALL HOLE WATER',b'avoid wallflower whole waterfall',b'water\0void',b'holewater',b'wallwater',b'voidhole']
 for i in range(200):tags.append(b' '.join(rng.choice(tags[:8]) for _ in range(rng.randrange(5))))
 for text in tags:
  words=(rng.getrandbits(32),rng.getrandbits(32));expected=oracle.flags(words,text);new.uc.mem_write(flags_new,struct.pack('<2I',*words));new.uc.mem_write(tags_new,text+b'\0');assert new.invoke('dh2_floor_source_flags',[flags_new,tags_new,len(text)])==0;actual=bytes(new.uc.mem_read(flags_new,8));assert expected==actual,('flags',text,expected.hex(),actual.hex());flag_records.append(struct.pack('<3I',*words,len(text))+text+expected)
 for i in range(512):
  low=[rng.uniform(-10000,10000) for _ in range(3)];high=[x+rng.uniform(0,10000) for x in low];local=struct.pack('<6f',*low,*high);node=pack_matrix(i%3);world,expanded=oracle.bounds(local,node);new.uc.mem_write(local_new,local);new.uc.mem_write(node_new,node);assert new.invoke('dh2_floor_transform_bounds',[bounds_new,local_new,node_new])==0;assert equal(bytes(new.uc.mem_read(bounds_new,24)),world),('world bounds',i)
  assert new.invoke('dh2_floor_source_bounds',[bounds_new,bounds_new])==0;assert equal(bytes(new.uc.mem_read(bounds_new,24)),expanded),('floor bounds',i);bounds_records.append(local+node+world+expanded)
 reference=struct.pack('<4I',0x32534d46,len(records),len(flag_records),len(bounds_records))+b''.join(records)+b''.join(flag_records)+b''.join(bounds_records);a.reference_output.write_bytes(reference);report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'asset_library_sha256':hashlib.sha256(a.asset_library.read_bytes()).hexdigest() if a.asset_library else None,'native_attribute_calls':getattr(new,'asset_calls',0),'reference_sha256':hashlib.sha256(reference).hexdigest(),'mesh_constructor_comparisons':len(cases),'triangle_comparisons':triangle_count,'baked_constructor_cases':baked,'inconsistent_identity_hint_cases':13,'unindexed_parts':unindexed,'scalar_type_parts':type_counts,'component_parts':component_counts,'skipped_parts':skipped,'floor_tag_comparisons':len(flag_records),'world_and_floor_bounds_comparisons':len(bounds_records),'original_mesh_getters':oracle.cpu.mesh_getters,'original_buffer_maps':oracle.maps,'original_node_matrix_getters':oracle.cpu.matrix_getters,'original_allocations':oracle.allocations,'original_import_calls':oracle.cpu.import_calls,'mismatches':0,'scope':__doc__+'\nOriginal _LoadNavMesh tag and bounds ranges execute separately; metadata lookup, node clone and full loader services are not executed. getTransformedBoundingBox executes its dirty/cache paths and transformBoxEx.','elapsed_seconds':round(time.monotonic()-start,2)};a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
