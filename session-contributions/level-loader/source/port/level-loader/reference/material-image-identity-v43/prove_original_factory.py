from pathlib import Path
exec((Path(__file__).parent/'prove_filename_branch.py').read_text().split('o=Oracle(')[0])
class Factory(Oracle):
 def storage(self,uc,a,n,u):
  if a==0x56d8b0:self.store(self.reg(0),self.ss(self.reg(1))+self.txt(self.reg(2)).encode());self.returned()
  elif a==0x5ed210:
   self.calls.append({'path':self.txt(self.reg(2)),'optional_identity':self.txt(self.reg(3))});self.pointer(self.reg(0),0);self.returned(self.reg(0))
  else:super().storage(uc,a,n,u)
o=Factory(Path(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so'),{'functions':[]});inp=o.data+0x1000;out=inp+0x500;directory=inp+0x1000;rows=[]
for prefix,s in [('data/3d/gameobjects','q:/data/iphone/3d/textures/atlas_dh2_game_objects_001.tga'),('data/3d/modules/swamp','q:/data/iphone/3d/textures/env_swamp.tga'),('data/3d/modules/crypt','q:/data/iphone/3d/textures/env_crypt.tga'),('data/3d/modules/swamp','data/3d/environments/textures/example.tga'),('data/3d/modules/swamp','../../Desktop/example.tga')]:
 o.store(directory,prefix);o.uc.mem_write(inp,s.encode()+b'\0');o.uc.mem_write(out,bytes(4));o.calls=[];o.invoke(0x6594a0,[out,0,directory,0,o.data+0x3000,inp,0]);want=[prefix+'/'+s,s];assert [c['path'] for c in o.calls]==want,(want,o.calls);rows.append({'directory_fixture':prefix,'actual_authored_uri':s,'actual_original_calls':o.calls})
report={'validation':'PASS','original_entry':'0x6594a0','cases':rows,'comparisons':len(rows),'scope':'Original factory path ordering/string operands with explicit string allocation/append and unavailable texture manager endpoint; no filesystem mount or GPU proof'}
Path(r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/material-image-identity-v43/original-factory-paths.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
