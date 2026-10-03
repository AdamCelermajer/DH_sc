"""Bounded original-only composition probes; sampling/binding are fixtures.

Executes applyAnimationValues65e350, NormalizeWeights366594, typed target
virtuals, quaternion interpolation612d00, and actual scene-node setters.
This does not validate a native blended pose implementation or asset binding.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from navigation_motion_differential import MotionCpu
from unicorn import UC_HOOK_CODE

def floats(values):return struct.pack('<'+'f'*len(values),*values)
def values(raw):return list(struct.unpack('<'+'f'*(len(raw)//4),raw))

def main():
    p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    manifest=json.loads((Path(__file__).parent/'original-functions.json').read_text())
    assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
    c=MotionCpu(a.engine,False,manifest)
    b=c.data+0x1000; bvt=b+0x200; slots=[b+0x300,b+0x400]; svts=[b+0x500,b+0x600]
    sv=b+0x700; w=b+0x800; tv=b+0x900; inf=b+0xa00; bv=b+0xb00
    nodes=[b+0x1000+i*0x200 for i in range(3)]; nvt=b+0x1700
    tracks=[b+0x1800+i*0x100 for i in range(3)]; trackvts=[b+0x1b00+i*0x100 for i in range(3)]
    buffers=[b+0x2000+i*0x100 for i in range(3)]
    service=b+0x3000; calls=[]; enabled=[True]*3; missing=-1; mutate=False
    def ret(value=0):c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
    def hook(uc,address,size,unused):
        if address in (service,service+4):
            i=(address-service)//4;calls.append({'op':'sample','slot':i,'timestamp':c.reg(1),'weights':values(bytes(uc.mem_read(w,8)))})
            if i==0 and mutate:uc.mem_write(w+4,floats([0.]))
            ret()
        elif address==service+8:ret(int(enabled[c.reg(1)]))
        elif address==service+12:ret(tracks[c.reg(1)])
        elif address in (0x59712c,0x5970c4,0x5970f4):
            calls.append({'op':{0x59712c:'position',0x5970c4:'scale',0x5970f4:'rotation'}[address],'node':nodes.index(c.reg(0)),'value':values(bytes(uc.mem_read(c.reg(1),16 if address==0x5970f4 else 12)))})
    c.uc.hook_add(UC_HOOK_CODE,hook)
    c.pointer(b,bvt);c.pointer(bvt+0x80,service+8)
    c.pointer(b+0x28,sv);c.pointer(b+0x2c,sv+8);c.pointer(b+0x34,w);c.pointer(b+0x38,w+8)
    c.pointer(b+0x58,tv);c.pointer(b+0x5c,tv+12);c.pointer(b+0x64,inf);c.pointer(b+0x4c,bv)
    for i in range(2):
        c.pointer(sv+4*i,slots[i]);c.pointer(slots[i],svts[i]);c.pointer(svts[i]+0x4c,service+4*i);c.pointer(svts[i]+0x58,service+12)
    c.pointer(nvt+0xa4,0x59712c);c.pointer(nvt+0x94,0x5970c4);c.pointer(nvt+0x9c,0x5970f4)
    for i,wrapper in enumerate((0x62859c,0x62d72c,0x6209b4)):
        c.pointer(nodes[i],nvt);c.pointer(tracks[i],trackvts[i]);c.pointer(trackvts[i]+0x18,wrapper)
        c.pointer(inf+4*i,0);c.pointer(bv+4*i,buffers[i])
    initial=[[2.,4.,8.,10.,20.,40.],[1.,2.,3.,4.,5.,6.],[0.,0.,0.,1.,0.,0.,0.,-1.]]
    records=[]
    for weights in ([1.,0.],[0.,1.],[.25,.75],[2.,6.],[1.,-1.],[0.,0.],[-1.,2.],[.5,.5]):
        for mode in range(4):
            enabled[:]=[True]*3;missing=-1;mutate=mode==3
            if mode==1:enabled[1]=False
            elif mode==2:missing=0
            calls.clear();c.uc.mem_write(w,floats(weights))
            for i in range(3):
                c.uc.mem_write(nodes[i]+0xac,bytes(40));c.pointer(nodes[i]+0x11c,0x100)
                c.pointer(tv+4*i,0 if i==missing else nodes[i]);c.uc.mem_write(buffers[i],floats(initial[i]))
            c.invoke(0x65e350,[b,12345])
            expected_slots=[]
            if weights[0]!=0:expected_slots.append(0)
            if weights[1]!=0 and not(mutate and weights[0]!=0):expected_slots.append(1)
            assert [row['slot'] for row in calls if row['op']=='sample']==expected_slots
            expected_targets=[name for i,name in enumerate(('position','scale','rotation')) if enabled[i] and i!=missing]
            assert [row['op'] for row in calls if row['op']!='sample']==expected_targets
            dirty=[struct.unpack('<I',c.uc.mem_read(node+0x11c,4))[0] for node in nodes]
            assert dirty==[0x100|(bit if enabled[i] and i!=missing else 0) for i,bit in enumerate((8,2,4))]
            normalized=values(bytes(c.uc.mem_read(w,8)))
            if weights==[0.,0.]:assert normalized==[1.,0.]
            if weights==[1.,-1.] and not mutate:assert normalized==[1.,-1.]
            records.append({'weights':weights,'mode':mode,'calls':list(calls),'normalized_weights':normalized,'dirty_words':dirty,'node_words':[bytes(c.uc.mem_read(node+0xac,40)).hex() for node in nodes]})
    result={'validation':'PASS','original_sha256':manifest['original_sha256'],'original_composition_cases':len(records),'ordered_sample_calls':sum(sum(r['op']=='sample' for r in row['calls']) for row in records),'ordered_target_setters':sum(sum(r['op']!='sample' for r in row['calls']) for row in records),'native_comparisons':0,'mismatches':0,'scope':'Original apply/normalize/typed/setter instructions. Slot sampling, union-target binding, target enabled and pointer identities are synthetic synchronous service fixtures. No native pose/asset or root/event end-to-end parity claim.','records':records,'import_calls':c.import_calls}
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:v for k,v in result.items() if k not in ('records','import_calls','scope')}))
if __name__=='__main__':main()
