import struct,zlib,json,hashlib,sys
from pathlib import Path
root=Path(__file__).resolve().parents[3]
p=root/'port/android-native/app/src/main/assets/original-cache/data/menus'/ (sys.argv[1] if len(sys.argv)>1 else 'dqcharmenu_droid.swf')
raw=p.read_bytes();b=raw[:8]+zlib.decompress(raw[8:]) if raw[:3]==b'CWS' else raw
class Bits:
 def __init__(self,b,at):self.b=b;self.i=at*8
 def u(self,n):
  v=0
  for _ in range(n):v=(v<<1)|((self.b[self.i//8]>>(7-self.i%8))&1);self.i+=1
  return v
 def s(self,n):v=self.u(n);return v-(1<<n) if n and v&(1<<(n-1)) else v
 def end(self):return (self.i+7)//8
def rect(b,at):
 r=Bits(b,at);n=r.u(5);v=[r.s(n) for _ in range(4)];return v,r.end()
def matrix(b,at):
 r=Bits(b,at);a=d=1.;bb=c=0.
 if r.u(1):n=r.u(5);a=r.s(n)/65536;d=r.s(n)/65536
 if r.u(1):n=r.u(5);bb=r.s(n)/65536;c=r.s(n)/65536
 n=r.u(5);tx=r.s(n);ty=r.s(n);return [a,bb,c,d,tx,ty],r.end()
def tags(b,at,end):
 while at+2<=end:
  h=struct.unpack_from('<H',b,at)[0];at+=2;n=h&63;code=h>>6
  if n==63:n=struct.unpack_from('<I',b,at)[0];at+=4
  yield code,b[at:at+n];at+=n
  if code==0:break
exports={};shapes={};sprites={}
_,at=rect(b,8);at+=4
for code,t in tags(b,at,len(b)):
 if code==56:
  count=struct.unpack_from('<H',t)[0];q=2
  for _ in range(count):id=struct.unpack_from('<H',t,q)[0];q+=2;end=t.index(0,q);exports[id]=t[q:end].decode();q=end+1
 if code in (2,22,32):
  id=struct.unpack_from('<H',t)[0];bounds,q=rect(t,2);count=t[q];q+=1
  if count==255:count=struct.unpack_from('<H',t,q)[0];q+=2
  fills=[]
  for _ in range(count):
   kind=t[q];q+=1
   if kind==0:q+=4 if code==32 else 3
   elif kind in (0x40,0x41,0x42,0x43):bitmap=struct.unpack_from('<H',t,q)[0];q+=2;m,q=matrix(t,q);fills.append((bitmap,m))
   else:break
  if fills:shapes[id]=(bounds,fills)
 if code==39:
  id,frames=struct.unpack_from('<HH',t);frame=0;display={};labels={};snapshots={}
  for tc,v in tags(t,4,len(t)):
   if tc==26:
    flags=v[0];depth=struct.unpack_from('<H',v,1)[0];q=3
    if flags&2:display[depth]=struct.unpack_from('<H',v,q)[0]
   if tc==28:display.pop(struct.unpack_from('<H',v)[0],None)
   if tc==43:labels[frame]=v.split(b'\0')[0].decode()
   if tc==1:snapshots[frame]=list(display.values());frame+=1
  sprites[id]=(labels,snapshots)
def crop(id,depth=0):
 if depth>8:return None
 if id in shapes:
  bounds,fills=shapes[id]
  if len(fills)!=1:return None
  bitmap,m=fills[0];a,bb,c,d,tx,ty=m
  if bb or c or not a or not d:return None
  return dict(texture=exports.get(bitmap,''),bitmap=bitmap,x=round((bounds[0]-tx)/a),y=round((bounds[2]-ty)/d),width=round((bounds[1]-bounds[0])/a),height=round((bounds[3]-bounds[2])/d))
 if id in sprites:
  for child in sprites[id][1].get(0,[]):
   v=crop(child,depth+1)
   if v:return v
rows=[]
for id,name in exports.items():
 found=crop(id)
 if found:rows.append(dict(name=name,sprite=id,frame=0,**found))
for sprite,(labels,frames) in sprites.items():
 for label_frame,label in labels.items():
  found=None
  for child in frames.get(label_frame,[]):
   found=crop(child)
   if found:break
  if found:rows.append(dict(name=label,sprite=sprite,frame=label_frame,**found))
out=root/'.local-inputs'/('hud-icon-index-'+p.stem+'.json');out.write_text(json.dumps(dict(source_sha256=hashlib.sha256(raw).hexdigest(),rows=rows),indent=2))
print('Derived authored icon shapes:',len(rows))
