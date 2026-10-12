# Source recovery generator originally creates scalar extra cells. The final working tree additionally uses proven native-width weak-alias cells for56..59; regenerating source outputs must preserve that reviewed API extension.
from pathlib import Path
import sys,json,struct,re
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM,CS_MODE_LITTLE_ENDIAN
r=Path(__file__).resolve().parents[3];p=Path(__file__).resolve().parents[1];source=p/'source';base=r/'candidate/command-kind7-v65'
rows=json.loads((r/'reference/scriptmanager-v52/factory-table.json').read_text())['entries'];old=set(map(int,json.loads((r/'reference/scriptmanager-v52/data-schemas-v52.json').read_text())))|{4,7};primitive={int(k):v['width'] for k,v in json.loads((r/'reference/scriptmanager-v52/primitive-helpers.json').read_text()).items()};primitive.update({0x459090:4,0x3df1a0:4});scopes={};ctors={};newdata={};newcmd={};matrix=[]
with Path(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so').open('rb') as f:
 elf=ELFFile(f);segments=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD'];md=Cs(CS_ARCH_ARM,CS_MODE_ARM|CS_MODE_LITTLE_ENDIAN);meta={}
 for sym in elf.get_section_by_name('.dynsym').iter_symbols():
  if sym['st_value'] and sym['st_size']:meta.setdefault(sym['st_value'],[]).append({'symbol':sym.name,'size':sym['st_size'],'binding':sym['st_info']['bind']})
 def raw(a,n):
  out=b''
  for delta in range(0,n,256):
   count=min(256,n-delta)
   for seg in segments:
    if seg['p_vaddr']<=a+delta and a+delta+count<=seg['p_vaddr']+seg['p_filesz']:f.seek(seg['p_offset']+a+delta-seg['p_vaddr']);out+=f.read(count);break
   else:raise ValueError(hex(a+delta))
  return out
 def word(a):return struct.unpack('<I',raw(a,4))[0]
 def scope(a):
  if a not in scopes:
   size=max(v['size'] for v in meta[a]);b=raw(a,size);ins=list(md.disasm(b,a));scopes[a]={'address':hex(a),'symbols':meta[a],'bytes_hex':b.hex(),'instructions':[{'address':hex(i.address),'bytes':i.bytes.hex(),'mnemonic':i.mnemonic,'operands':i.op_str} for i in ins]};(source/(hex(a)+'.asm')).write_text('\n'.join(f'{i.address:08x} {i.bytes.hex()} {i.mnemonic} {i.op_str}' for i in ins)+'\n')
  return list(md.disasm(bytes.fromhex(scopes[a]['bytes_hex']),a))
 def ctor(a):
  if a in ctors:return ctors[a]
  regs={};stores=[];size=None;vp=None;calls=[]
  for i in scope(a):
   op=i.op_str;m=i.mnemonic
   if m in ('mov','mvn') and re.fullmatch(r'[a-z0-9]+, #(0x[0-9a-f]+|[0-9]+)',op):d,v=op.split(', ');n=int(v[1:],0);regs[d]=n if m=='mov' else (~n&0xffffffff)
   elif m=='mov' and re.fullmatch(r'[a-z0-9]+, [a-z0-9]+',op):d,s=op.split(', ');regs[d]=regs.get(s)
   elif m=='ldr' and '[pc,' in op:
    d=op.split(',')[0];off=int(re.search(r'#(0x[0-9a-f]+|[0-9]+)',op)[1],0);regs[d]=word(i.address+8+off)
   elif m=='add' and ', pc,' in op:d,_,s=op.split(', ');regs[d]=i.address+8+regs[s]
   elif m=='add' and re.fullmatch(r'[a-z0-9]+, [a-z0-9]+, #(0x[0-9a-f]+|[0-9]+)',op):d,s,v=op.split(', ');b=regs.get(s);n=int(v[1:],0);regs[d]=(b[0],b[1]+n) if isinstance(b,tuple) else b+n if isinstance(b,int) else None
   elif m=='ldr' and re.fullmatch(r'[a-z0-9]+, \[[a-z0-9]+, [a-z0-9]+\]',op):d,b,x=re.fullmatch(r'([a-z0-9]+), \[([a-z0-9]+), ([a-z0-9]+)\]',op).groups();regs[d]=word(regs[b]+regs[x])
   elif m=='bl':
    target=int(op[1:],16);calls.append(hex(target))
    if target in (0x310570,0x31056c):size=regs['r0'];regs['r0']=('fresh',0)
    else:regs['r0']=None
   elif m in ('str','strb','strh'):
    parsed=re.fullmatch(r'([a-z0-9]+), \[([a-z0-9]+)(?:, #(0x[0-9a-f]+|[0-9]+))?\](?:, #(0x[0-9a-f]+|[0-9]+))?',op)
    if parsed:
     s,b,offset,post=parsed.groups();target=regs.get(b)
     if isinstance(target,tuple) and target[0]=='fresh':
      off=target[1]+(int(offset,0) if offset else 0);v=regs.get(s);assert isinstance(v,int),(hex(a),hex(i.address),op);width={'str':4,'strh':2,'strb':1}[m];stores.append({'offset':off,'width':width,'value':v&((1<<(width*8))-1),'source':hex(i.address)})
      if off==0:vp=v
      if post:regs[b]=('fresh',target[1]+int(post,0))
   elif m=='pop' and 'pc' in op:break
  assert size and vp,(hex(a),size,vp);slots=struct.unpack('<IIII',raw(vp,16));ctors[a]={'factory':hex(a),'size':size,'vptr':hex(vp),'slots':[hex(v) for v in slots],'stores':stores,'calls':calls};return ctors[a]
 def nearest(ins,index,pattern,mnemonic=None):
  for before in reversed(ins[max(0,index-20):index]):
   if mnemonic and before.mnemonic!=mnemonic:continue
   match=re.fullmatch(pattern,before.op_str)
   if match:return before,match
  raise ValueError((hex(ins[index].address),pattern))
 def reader(a):
  ins=scope(a);actions=[]
  for index,i in enumerate(ins):
   if i.mnemonic not in ('bl','b'):continue
   target=int(i.op_str[1:],16)
   if target==0x4ff828:actions.append({'offset':4,'width':4,'helper':'0x459090','source_base_call':hex(i.address)})
   elif target in primitive:
    _,m=nearest(ins,index,r'r1, [a-z0-9]+, #(0x[0-9a-f]+|[0-9]+)','add');actions.append({'offset':int(m[1],0),'width':primitive[target],'helper':hex(target),'source':hex(i.address)})
   elif target==0x317454:
    _,ptr=nearest(ins,index,r'r0, \[[a-z0-9]+, #(0x[0-9a-f]+|[0-9]+)\]','str');_,length=nearest(ins,index,r'r2, \[[a-z0-9]+, #(0x[0-9a-f]+|[0-9]+)\]','ldr');actions.append({'offset':int(ptr[1],0),'width':0,'count_offset':int(length[1],0),'helper':hex(target),'source':hex(i.address)})
   elif target not in (0x310440,0x31056c):raise ValueError(('unknown reader call',hex(a),hex(target)))
  assert actions and actions[0]['offset']==4,hex(a);return actions
 for row in rows:
  cmd=ctor(int(row['command_factory'],16));data=ctor(int(row['data_factory'],16));slots=[int(v,16) for v in cmd['slots']];entry={'kind':row['kind'],'class':row['command_symbol'].split('<')[1].split('>')[0],'C1':cmd,'Data_C1':data,'Init':{'address':cmd['slots'][0],'symbols':meta[slots[0]]},'Execute':{'address':cmd['slots'][2],'symbols':meta[slots[2]]},'IsBlocking':{'address':cmd['slots'][3],'symbols':meta[slots[3]]}};matrix.append(entry)
  if row['kind'] not in old:
   assert int(cmd['slots'][0],16)==0x455628
   actions=reader(int(data['slots'][3],16));defined={}
   for st in data['stores']:
    if st['offset']:
     for n in range(st['width']):defined[st['offset']+n]=(st['value']>>(8*n))&255
   defaults={}
   for act in actions:
    if act['width']:
     if all(act['offset']+n in defined for n in range(act['width'])):defaults[act['offset']]={'width':act['width'],'bits':sum(defined[act['offset']+n]<<(8*n) for n in range(act['width']))}
    else:
     assert all(defined.get(act['offset']+n)==0 for n in range(4)),(row,act,defined);assert any(st['offset']==act['count_offset'] and st['width']==4 for st in actions)
   newdata[row['kind']]={'size':data['size'],'factory':row['data_factory'],'reader':data['slots'][3],'actions':actions,'constructor_defaults':defaults};newcmd[row['kind']]=entry
 for entry in matrix:
  for method in ('Init','Execute','IsBlocking'):
   a=int(entry[method]['address'],16);ins=scope(a);size=max(v['size'] for v in meta[a]);calls=[]
   for i in ins:
    if i.mnemonic in ('bl','b'):
     target=int(i.op_str[1:],16)
     if a<=target<a+size:continue
     calls.append({'source':hex(i.address),'target':hex(target),'symbols':meta.get(target,[])})
   entry[method]['direct_external_calls']=calls
(source/'constructor-data-runtime.json').write_text(json.dumps({'status':'SOURCE_WORKING_NO_RUNTIME_VERIFICATION','known_prior_kinds':sorted(old),'added_kinds':sorted(newcmd),'new_data_schemas':newdata,'new_command_classes':newcmd,'runtime_matrix':matrix,'bounded_named_method_scopes':scopes,'kind45_C1_nested_ObjectHandle_stores_authority':'retained original-command-C1-v58.json; current straight-line scope records prefix only','other_maps_sampled':False,'native_jobs':0},indent=2)+'\n')
h=(base/'script_command_receivers_v59.hpp').read_text();c=(base/'script_command_receivers_v59.cpp').read_text();d=(base/'command_c1_descriptors_v59.inc').read_text();s=(base/'script_data_schemas_v52.inc').read_text()
for kind,entry in newcmd.items():
 name=entry['class']+'ReceiverV59';stores=[v for v in entry['C1']['stores'] if v['offset']];extras={v['offset']:v['width'] for v in stores if v['offset']>=16};klass='class '+name+' final : public CanonicalScriptCommandV59 {\n'
 for off,width in extras.items():klass+=f' CommandOperandCellV59 field{off:x}_{{{width},{{}}}};\n'
 for const in ('','const '):klass+=f' {const}CommandOperandCellV59* operand_cell(std::uint32_t o) {const}noexcept override {{'+''.join(f'if(o=={off})return &field{off:x}_;' for off in extras)+'(void)o;return nullptr;}\n'
 klass+=f'public:\n {name}(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),{kind}){{apply_constructor_operands();}}\n std::int32_t receiver_kind() const noexcept override {{return {kind};}}\n}};\n';pos=h.index('bool create_script_command_receiver_v59');h=h[:pos]+klass+h[pos:]
 pos=c.index(' default:error="Unretained original class C1"');c=c[:pos]+f'case {kind}:candidate=std::make_shared<{name}>(found->second,services);break;\n'+c[pos:]
 row='{'+str(kind)+',{'+str(kind)+',"'+entry['class']+'",'+entry['C1']['factory']+'u,'+entry['Init']['address']+'u,'+str(entry['C1']['size'])+'u,{'+','.join('{'+str(v['offset'])+'u,'+str(v['width'])+'u,'+str(v['value'])+'u,'+v['source']+'u}' for v in stores)+'}}},\n';pos=d.index('};return table;');d=d[:pos]+row+d[pos:]
 data=newdata[kind];acts=','.join('{'+str(a['offset'])+','+str(a['width'])+','+str(a.get('count_offset',0))+'}' for a in data['actions']);defs=','.join('{'+str(off)+',{'+str(a['width'])+','+str(a['bits'])+'u}}' for off,a in data['constructor_defaults'].items());row='  {'+str(kind)+',{'+str(data['size'])+'u,'+data['factory']+'u,'+data['reader']+'u,{'+acts+'},{'+defs+'}}},\n';pos=s.index(' };return schemas;');s=s[:pos]+row+s[pos:]
for name,body in [('script_command_receivers_v59.hpp',h),('script_command_receivers_v59.cpp',c),('command_c1_descriptors_v59.inc',d),('script_data_schemas_v52.inc',s)]: (p/name).write_text(body)
assert h.count(' final : public CanonicalScriptCommandV59')==80;assert c.count('candidate=std::make_shared<Script_')==80
print(json.dumps({'working_tree':str(p),'classes_and_factories':80,'new_Data_schemas':len(newdata),'all_Init_addresses':sorted({v['Init']['address'] for v in matrix}),'named_scopes':len(scopes),'native_or_emulation_jobs':0}))
