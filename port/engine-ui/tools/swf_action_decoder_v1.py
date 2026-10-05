# Reused validated source action decoder; no CLI side effects.
import json, struct, zlib, hashlib, re
from pathlib import Path

REPO=Path(__file__).resolve().parents[3]
DEST=Path(__file__).resolve().parent
ops={0:'End',4:'NextFrame',5:'PrevFrame',6:'Play',7:'Stop',10:'Add',11:'Subtract',12:'Multiply',13:'Divide',14:'Equals',15:'Less',16:'And',17:'Or',18:'Not',23:'Pop',28:'GetVariable',29:'SetVariable',32:'SetTarget2',33:'StringAdd',38:'Trace',48:'Random',59:'Delete',60:'Delete2',61:'DefineLocal',62:'CallFunction',63:'Return',64:'Modulo',65:'NewObject',66:'DefineLocal2',67:'InitArray',68:'InitObject',69:'TypeOf',70:'TargetPath',71:'Add2',72:'Less2',73:'Equals2',74:'ToNumber',75:'ToString',76:'PushDuplicate',77:'StackSwap',78:'GetMember',79:'SetMember',80:'Increment',81:'Decrement',82:'CallMethod',83:'NewMethod',84:'InstanceOf',85:'Enumerate2',96:'BitAnd',97:'BitOr',98:'BitXor',99:'BitLShift',100:'BitRShift',101:'BitURShift',102:'StrictEquals',103:'Greater',104:'StringGreater',129:'GotoFrame',131:'GetURL',135:'StoreRegister',136:'ConstantPool',138:'WaitForFrame',139:'SetTarget',140:'GotoLabel',141:'WaitForFrame2',142:'DefineFunction2',143:'Try',148:'With',150:'Push',153:'Jump',154:'GetURL2',155:'DefineFunction',157:'If',158:'Call',159:'GotoFrame2'}
# Opcode names taken from pinned upstream disassembler rather than inferred.
table=(REPO/'port/engine-ui/vendor/gameswf1714/gameswf/gameswf_disasm.cpp').read_text()
for op,name in re.findall(r'\{\s*(0x[0-9A-Fa-f]+),\s*"([^"\n]+)",\s*ARG_',table):
 ops[int(op,16)]=name
def string(b,p):
 e=b.index(0,p);return b[p:e].decode('utf8','replace'),e+1
def decode(b,start,end,pool=(),label=''):
 rows=[];p=start
 while p<end:
  a=p;op=b[p];p+=1;n=0
  if op>=128:n=struct.unpack_from('<H',b,p)[0];p+=2
  lim=p+n;r={'offset':a,'op':ops.get(op,hex(op)),'length':n}
  if lim>end:raise ValueError((label,a,lim,end))
  if op==136:
   c=struct.unpack_from('<H',b,p)[0];q=p+2;pool=[]
   for _ in range(c):s,q=string(b,q);pool.append(s)
   r['constants']=pool
  elif op==150:
   q=p;values=[]
   while q<lim:
    t=b[q];q+=1
    if t==0:v,q=string(b,q)
    elif t==1:v=struct.unpack_from('<f',b,q)[0];q+=4
    elif t==2:v=None
    elif t==3:v={'undefined':True}
    elif t==4:v={'register':b[q]};q+=1
    elif t==5:v=bool(b[q]);q+=1
    elif t==6:v=struct.unpack('<d',b[q+4:q+8]+b[q:q+4])[0];q+=8
    elif t==7:v=struct.unpack_from('<i',b,q)[0];q+=4
    elif t in(8,9):idx=b[q] if t==8 else struct.unpack_from('<H',b,q)[0];q+=1 if t==8 else 2;v={'constant':idx,'text':pool[idx] if idx<len(pool) else '<invalid>'}
    else:raise ValueError(('push',a,t))
    values.append(v)
   r['values']=values
  elif op in(142,155):
   q=p;name,q=string(b,q);argc=struct.unpack_from('<H',b,q)[0];q+=2;regcount=flags=None
   if op==142:regcount=b[q];flags=struct.unpack_from('<H',b,q+1)[0];q+=3
   args=[]
   for _ in range(argc):
    reg=None
    if op==142:reg=b[q];q+=1
    s,q=string(b,q);args.append({'register':reg,'name':s})
   size=struct.unpack_from('<H',b,q)[0];q+=2
   r.update(name=name,args=args,registers=regcount,flags=flags,body=decode(b,q,q+size,pool,label+'/'+name))
   # SWF actionLength is the function header only; body bytes follow it.
   lim=q+size
  elif op in(153,157):r['target']=lim+struct.unpack_from('<h',b,p)[0]
  elif op==135:r['register']=b[p]
  elif op in(139,140):r['text']=string(b,p)[0]
  elif n:r['bytes']=b[p:lim].hex()
  rows.append(r);p=lim
  if op==0:break
 return rows
class Bits:
 def __init__(self,b,p):self.b=b;self.p=p*8
 def u(self,n):
  v=0
  for _ in range(n):v=(v<<1)|((self.b[self.p//8]>>(7-self.p%8))&1);self.p+=1
  return v
 def skip(self,n):self.p+=n
 def end(self):return (self.p+7)//8
def matrix_end(b,p):
 bits=Bits(b,p)
 if bits.u(1):bits.skip(bits.u(5)*2)
 if bits.u(1):bits.skip(bits.u(5)*2)
 bits.skip(bits.u(5)*2);return bits.end()
def clips(b,t):
 p=t['offset'];flags=b[p];p+=1
 if not flags&128:return
 flags2=b[p] if t['tag']==70 else 0;p+=int(t['tag']==70)
 p+=2
 if t['tag']==70 and ((flags2&8)or((flags2&16)and(flags&2))):_,p=string(b,p)
 if flags&2:p+=2
 if flags&4:p=matrix_end(b,p)
 if flags&8:
  bits=Bits(b,p);a=bits.u(1);m=bits.u(1);n=bits.u(4);bits.skip(n*4*(a+m));p=bits.end()
 if flags&16:p+=2
 if flags&32:_,p=string(b,p)
 if flags&64:p+=2
 if t['tag']==70 and flags2&7:raise ValueError('unsupported PlaceObject3 filter/blend/cache before clip actions')
 p+=6 # reserved UI16, aggregate flags UI32 (version8)
 while True:
  event=struct.unpack_from('<I',b,p)[0];p+=4
  if not event:break
  size=struct.unpack_from('<I',b,p)[0];p+=4;end=p+size
  key=None
  if event&0x20000:key=b[p];p+=1
  yield p,end,event,key
  p=end
def flatten(ts,path='root'):
 for t in ts:
  if t['tag'] in(12,59):yield path,t
  if t['tag'] in(26,70):yield path,t
  if 'children'in t:yield from flatten(t['children'],path+'/sprite'+str(t['id']))
def lines(rows,depth=0):
 for r in rows:
  info={k:v for k,v in r.items() if k not in('body','offset','op','length')}
  yield '  '*depth+f"{r['offset']:08x} {r['op']} "+json.dumps(info,ensure_ascii=False)
  if 'body'in r:yield from lines(r['body'],depth+1)
