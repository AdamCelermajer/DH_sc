"""Original GetHostingPlayer gate/call ordering; manager/network/debug services and GetPlayerByInternalID explicit."""
import hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
c=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});manager=c.data+0x1000;debug=c.data+0x2000;app=c.data+0x3000;network=c.data+0x4000;vtable=c.data+0x5000;sessions=[c.data+0x6000,c.data+0x7000];player=c.data+0x8000;callback=c.data+0x1e000;c.uc.mem_write(callback,struct.pack('<I',0xe12fff1e));c.pointer(network,vtable);c.pointer(vtable+0x64,callback);trace=[];settings={};session_calls=0
def ret(v=0):c.put(0,v&0xffffffff);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,address,size,unused):
 global session_calls
 if address==0x7fd794:trace.append('debug');ret(debug)
 elif address==0x320e98:trace.append('network_enabled');ret(app)
 elif address==0x800f8c:trace.append('network_object');ret(network)
 elif address==callback:assert c.reg(0)==network;trace.append('network_valid');ret(settings['valid'])
 elif address==0x8100dc:trace.append('network_manager');ret(sessions[int(bool(session_calls and settings['mutate']))]);session_calls+=1
 elif address==0x8100e0:assert c.reg(0)==sessions[0];trace.append('initialized');ret(settings['initialized'])
 elif address==0x36dfb0:assert c.reg(0)==manager and c.reg(2)==0;trace.append(dict(lookup_internal_id=struct.unpack('<i',struct.pack('<I',c.reg(1)))[0],remote=False));ret(player)
c.uc.hook_add(UC_HOOK_CODE,hook);rows=[]
for net_debug,enabled,valid,initialized,host_id,mutate in itertools.product(range(2),range(2),range(2),range(2),[-1,0,37,-2147483648],range(2)):
 settings=dict(valid=valid,initialized=initialized,mutate=mutate);c.uc.mem_write(debug+5,bytes([net_debug]));c.uc.mem_write(app+0x24,bytes([enabled]));c.pointer(sessions[0]+0x170,host_id&0xffffffff);c.pointer(sessions[1]+0x170,(host_id+1)&0xffffffff);session_calls=0;trace.clear();assert c.invoke(0x36e09c,[manager])==player
 complete=net_debug and enabled and valid and initialized;expected_id=((host_id+mutate+2147483648)%4294967296)-2147483648 if complete else 0;assert trace[-1]==dict(lookup_internal_id=expected_id,remote=False)
 expected=['debug']+(['network_enabled'] if net_debug else [])+(['network_object','network_valid'] if net_debug and enabled else [])+(['network_manager','initialized'] if net_debug and enabled and valid else [])+(['network_manager'] if complete else [])+[dict(lookup_internal_id=expected_id,remote=False)];assert trace==expected
 rows.append(dict(debug=net_debug,enabled=enabled,valid=valid,initialized=initialized,host_id=host_id,mutate=mutate,trace=trace.copy()))
report=dict(validation='PASS',scope=__doc__,original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),manifest_sha256=sha(HERE/'producers/original-functions.json'),probe_sha256=sha(Path(__file__)),original_instructions_executed=True,cases=len(rows),ordered_services=sum(len(r['trace']) for r in rows),native_manager_implemented=False,rows=rows);(HERE/'hosting-probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['validation','cases','ordered_services']}))
