"""Execute original Tile module projection and MVX selection over original layouts.

Generate, Tile::SaveAsModuleXML, SetModuleMVXProperties, original XML parsing,
string/path selection, float operations and recursive naming execute ARM32.
Module construction, property registration/default loading, property writes,
property serialization and Module destruction are explicit recorded service
boundaries. This compares generated overrides and setter attempts, not complete
PropertyMap output, default values, XML byte serialization or whole Level load.
File callbacks supply original cache bytes; no mesh/path/position answer is a hook.
"""
import argparse,ctypes,hashlib,json,pathlib,re,struct,sys,zipfile
from procedural_layout_original import create_original_layout_cpu

def sha(path):
    with path.open('rb') as file:return hashlib.file_digest(file,'sha256').hexdigest()

def create_module_cpu(engine,dependencies,pack):
    base=type(create_original_layout_cpu(engine,dependencies))
    class Modules(base):
        def __init__(self):
            super().__init__();self.module_rows=[];self.module_maps={};self.read_files=[]
            self.file_service=self.data+0x1f00000;self.stream_service=self.file_service+0x1000
            self.file_vtable=self.file_service+0x2000;self.stream_vtable=self.file_service+0x3000
            got=0x491528+8+self.word(0x4917c0);global_variable=self.word(got+self.word(0x4917d0))
            application=self.file_service+0x4000;services=application+0x1000
            self.pointer(global_variable+0x10,services)
            self.pointer(services+0x34,self.file_service);self.pointer(self.file_service,self.file_vtable)
            self.pointer(self.file_vtable+0x88,self.callback+32);self.pointer(self.file_vtable+0x78,self.callback+48)
            self.pointer(self.stream_service,self.stream_vtable)
            self.pointer(self.stream_vtable+8,self.callback+64);self.pointer(self.stream_vtable+0x18,self.callback+80)
            self.file_bytes=b''
            self.keys={name[len(prefix):].lower():name for name in pack.namelist() if name.startswith(prefix)}
        def external(self,uc,address,size,unused):
            name=self.imports.get(address)
            if address==self.callback+32:
                uri=self.text(self.reg(1));key=uri.lower().replace('\\','/')
                self.read_files.append({'uri':uri,'found':key in self.keys})
                if key in self.keys:
                    self.file_bytes=pack.read(self.keys[key]);self.returned(self.stream_service)
                else:self.returned(0)
            elif address==self.callback+48:self.returned(0)
            elif address==self.callback+64:self.returned(len(self.file_bytes))
            elif address==self.callback+80:
                assert self.reg(0)==self.stream_service
                count=self.reg(2)|(self.reg(3)<<32);assert count==len(self.file_bytes),count
                if count:uc.mem_write(self.reg(1),self.file_bytes)
                self.returned(count)
            elif name=='__aeabi_f2d':
                value=struct.unpack('<f',struct.pack('<I',self.reg(0)))[0]
                lo,hi=struct.unpack('<2I',struct.pack('<d',value));self.put(1,hi);self.returned(lo)
            elif name=='sprintf' and self.text(self.reg(1))=='%f,%f,%f':
                from unicorn.arm_const import UC_ARM_REG_SP
                sp=uc.reg_read(UC_ARM_REG_SP)
                values=[struct.unpack('<d',struct.pack('<2I',self.reg(2),self.reg(3)))[0],
                        struct.unpack('<d',uc.mem_read(sp,8))[0],struct.unpack('<d',uc.mem_read(sp+8,8))[0]]
                out=('%f,%f,%f'%tuple(values)).encode();assert len(out)<256
                uc.mem_write(self.reg(0),out+b'\0');self.returned(len(out))
            else:return super().external(uc,address,size,unused)
        def project_modules(self):
            from unicorn import UC_HOOK_CODE
            def boundary(uc,address,size,unused):
                if address==0x389fa8:
                    self.module_maps[self.reg(0)+4]=[];self.returned(self.reg(0))
                elif address in (0x513d78,0x5136ec,0x389444):self.returned()
                elif address==0x51387c:
                    p,key,value=self.reg(0),self.text(self.reg(1)),self.reg(2)
                    assert p in self.module_maps
                    self.module_maps[p].append({'name':key,'value':self.text(value) if value else None});self.returned()
                elif address==0x5134f0:
                    p=self.reg(0);assert p in self.module_maps
                    attempts=self.module_maps[p];properties={row['name']:row['value'] for row in attempts if row['value'] is not None}
                    self.module_rows.append({'properties':properties,'setter_attempts':attempts});self.returned()
            hook=self.uc.hook_add(UC_HOOK_CODE,boundary)
            try:
                tile=self.word(self.app+0x114)
                result=self.invoke(0x491d90,[tile,self.data+0x1ef0000,0],budget=30000000) if tile else 0
            finally:self.uc.hook_del(hook)
            return {'modules':self.module_rows,'returned_index':result,'file_reads':self.read_files}
    return Modules()

prefix='com.gameloft.android.GAND.GloftD2SS/files/'
def main():
    ap=argparse.ArgumentParser()
    for name in ('engine','dependency-root','cache','original','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
    ap.add_argument('--match',default='');a=ap.parse_args();gold=json.loads(a.original.read_text())
    assert gold['original_execution_complete'] and sha(a.cache)==gold['cache_sha256'] and sha(a.engine)==gold['engine_sha256']
    rows=[]
    with zipfile.ZipFile(a.cache) as pack:
        for case in gold['cases']:
            if not case['name'].startswith('data/') or (a.match and a.match not in case['name']):continue
            raw=pack.read(prefix+case['name']);assert hashlib.sha256(raw).hexdigest()==case['input_sha256']
            blocks=[(block['name'],pack.read(prefix+block['uri']),block['uri']) for block in case['blocks']]
            row={'name':case['name'],'runs':[]}
            try:
                for run in case['runs']:
                    cpu=create_module_cpu(a.engine,a.dependency_root,pack)
                    tree,insertions,pointers=cpu.prepare(raw,blocks,case['folder'],case['target'])
                    actual=cpu.generate(run['seed'],pointers,100000000)
                    assert {key:value for key,value in actual.items() if key!='adapters'}=={key:value for key,value in run.items() if key!='adapters'}
                    before=cpu.word(cpu.app);projection=cpu.project_modules();assert cpu.word(cpu.app)==before
                    assert len(projection['modules'])==len(run['tiles'])
                    row['runs'].append({'seed':run['seed'],**projection})
            except Exception as e:row.update(failure=str(e),pc=hex(cpu.uc.reg_read(cpu.pc)))
            rows.append(row);print(json.dumps({'name':row['name'],'runs':len(row['runs']),'failure':row.get('failure')}),flush=True)
            if 'failure' in row:break
    root=pathlib.Path(__file__).resolve().parents[3];helpers={}
    for module in tuple(sys.modules.values()):
        path=getattr(module,'__file__',None)
        if path:
            path=pathlib.Path(path).resolve()
            if path.suffix=='.py' and path.is_relative_to(root/'port'):helpers[str(path.relative_to(root))]=sha(path)
    complete=len(rows)==(35 if not a.match else len([c for c in gold['cases'] if c['name'].startswith('data/') and a.match in c['name']])) and not any('failure' in row for row in rows)
    report={'scope':__doc__,'engine_sha256':gold['engine_sha256'],'cache_sha256':gold['cache_sha256'],
            'layout_original_receipt_sha256':sha(a.original),'script_sha256':sha(pathlib.Path(__file__)),
            'dependency_sources_sha256':helpers,'cases':rows,'selected_execution_complete':complete,
            'original_execution_complete':complete and not a.match,'full_property_serialization_verified':False,
            'native_module_projection_verified':False,'android_procedural_rendering_verified':False,'full_loader_verified':False}
    a.out.write_text(json.dumps(report,indent=2)+'\n')
    return 0 if complete else 1
if __name__=='__main__':raise SystemExit(main())
