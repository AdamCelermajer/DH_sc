from pathlib import Path
import sys,struct,json
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
shared=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc');sys.path.insert(0,str(shared/'port/game-data/tests'));from items_differential import Original as Base
class Original(Base):
 def __init__(self):
  super().__init__(shared/'.local-inputs/libDungeonHunter2.so',{'functions':[]});self.names={}
 def external(self,uc,address,size,unused):
  if address==self.callback+160:self.returned(self.names[self.reg(0)])
  elif self.imports.get(address)=='strncmp':
   def data(p,n):
    x=bytes(uc.mem_read(p,n));at=x.find(b'\0');return x if at<0 else x[:at]
   a,b=data(self.reg(0),self.reg(2)),data(self.reg(1),self.reg(2));self.returned((a>b)-(a<b))
  else:super().external(uc,address,size,unused)
c=Original();query=c.data+0x5000;text=c.data+0x5100;vt=c.data+0x5200;c.pointer(vt+0x24,c.callback+160);c.uc.mem_write(text,b'_colbox_\0');c.pointer(query+0x14,text);c.pointer(query+0x10,text+8)
cases=[(['root','mesh'],[-1,0]),(['root','prefix_colbox_wrong','_colbox_right'],[-1,0,0]),(['root','_colbox_first','_colbox_second'],[-1,0,0]),(['root','branch','_colbox_deep','_colbox_later'],[-1,0,1,0]),(['_colbox_root','child'],[-1,0])]
gold=[]
for names,parents in cases:
 nodes=[c.data+0x10000+i*0x400 for i in range(len(names))];c.names={}
 for i,(node,name) in enumerate(zip(nodes,names)):
  c.uc.mem_write(node,bytes(0x200));c.pointer(node,vt);p=node+0x200;c.uc.mem_write(p,name.encode()+b'\0');c.names[node]=p
 for i,node in enumerate(nodes):
  children=[nodes[j]+4 for j,p in enumerate(parents) if p==i];head=node+0xf4;c.pointer(head,children[0] if children else head)
  for j,child in enumerate(children):c.pointer(child,children[j+1] if j+1<len(children) else head)
 result=c.invoke(0x352b74,[c.data+0x6000,nodes[0],query,1]);selected=nodes.index(result) if result else -1
 expected=next((i for i,name in enumerate(names) if name.startswith('_colbox_')),-1);assert selected==expected,(names,selected,expected)
 gold.append({'names':names,'parents':parents,'selected':selected})
out=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc\port\level-loader\reference\render-pass-v40');(out/'original-helper-search-gold.json').write_text(json.dumps({'validation':'PASS','original_method':'0x352b74','query':'_colbox_','prefix_true':True,'cases':gold,'scope':'Whole original depth-first first-match helper prefix search. Typed source node names and standard strncmp are explicit fixture services; no native scene-factory type/cast claim.'},indent=2)+'\n')
print('PASS original helper prefix cases',len(gold))
