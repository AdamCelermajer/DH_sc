"""Bind native host replay inputs to the original ARM discovery corpus."""
import argparse,hashlib,json,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def native_timer(original):
 vptr,ident,repeat,duration,elapsed,active,paused,pad,event,ref=struct.unpack('<IIiIIBBHiI',original)
 return struct.pack('<IiIIBBHiQ',ident,repeat,duration,elapsed,active,paused,pad,event,ref)
def event_bytes(events):
 return struct.pack('<I',len(events))+b''.join(struct.pack('<iIQ',event if event<0x80000000 else event-0x100000000,offset//32,ref) for event,offset,ref in events)
def main():
 p=argparse.ArgumentParser();p.add_argument('--timer-host',type=Path);p.add_argument('--stance-host',type=Path);p.add_argument('--state-host',type=Path);p.add_argument('--chain-host',type=Path);a=p.parse_args()
 source=ROOT/'reference/character-timer-discovery';gold_path=source/'probe-gold.json';gold=json.loads(gold_path.read_bytes())
 assert sha(gold_path)=='fdc2415e2b41acac78a14ea4fd33118dbe78f46765f1b45d7ec3e8a0e0bc3f5f'
 assert gold['original_sha256']==sha(ROOT/'../../.local-inputs/libDungeonHunter2.so')
 timer=struct.pack('<II',0x31524d54,len(gold['timers']))
 for repeat,duration,elapsed,active,paused,dt,event,expected,events in gold['timers']:
  timer+=struct.pack('<i5Ii',repeat,duration,elapsed,active,paused,dt,event)+native_timer(bytes.fromhex(expected))+event_bytes(events)
 timer+=struct.pack('<I',len(gold['reentry']))
 for index,row in enumerate(gold['reentry']):
  raw=struct.pack('<16I',*row['timer_words']);timer+=struct.pack('<I',index)+native_timer(raw[:32])+native_timer(raw[32:])+event_bytes(row['events'])
 stance=struct.pack('<II',0x31415453,len(gold['stances']))
 for player,facts,count,result in gold['stances']:stance+=struct.pack('<Iii',player|(facts<<1),count,result)
 state=struct.pack('<II',0x31545453,len(gold['gate_projection']))
 for event,previous,result in gold['gate_projection']:state+=struct.pack('<III',event,previous,result)
 state+=struct.pack('<I',len(gold['expiry_chains']))
 for locked,blocked,forced,ais,order in gold['expiry_chains']:
  state+=struct.pack('<5I',locked,blocked,forced,ais,len(order))
  for name,value in order:state+=struct.pack('<II',('raise','ai-expired','state-event','attack-on-event').index(name),value)
 (ROOT/'reference/character-timer-discovery/state-timer-reference.bin').write_bytes(state)
 bindings=[]
 for module,data,host in (('character-timers',timer,a.timer_host),('character-stance',stance,a.stance_host)):
  directory=ROOT/'reference'/module;directory.mkdir(parents=True,exist_ok=True);path=directory/(module+'-reference.bin');path.write_bytes(data)
  report={'validation':'PASS' if host else 'REFERENCE_PREPARED','original_sha256':gold['original_sha256'],'original_discovery_gold_sha256':sha(gold_path),'reference_sha256':sha(path),'source_sha256':{str(s.relative_to(ROOT)):sha(s) for s in (ROOT/(module.replace('-','_')+'.cpp'),ROOT/(module.replace('-','_')+'.hpp'),ROOT/'tests'/(module.replace('-','_')+'.cpp'),Path(__file__))},'scope':'Original ARM observation replay against host C++ with explicit clock/debug/equipment/constant services. No native ARM64 or packaged parity claimed.'}
  if host:
   report['host_sanitizer_audit']=json.loads(host.read_text());assert report['host_sanitizer_audit']['mismatches']==0
   report['host_executable_sha256']=sha(ROOT/'../../.local-inputs/character-timer-discovery'/(module.replace('-','_')+'_host'))
   report['host_flags']='g++ -std=c++17 -O1 -g -Wall -Wextra -Werror -fsanitize=address,undefined -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off'
  output=ROOT/'reports'/(module+'-host-sanitizers.json');output.write_text(json.dumps(report,indent=2)+'\n');bindings.append({'module':module,'reference':str(path),'report':str(output)})
 if a.state_host and a.chain_host:
  old=ROOT/'reference/character-state/state-reference.bin'
  assert sha(old)=='9e11a899a29f9682959c00e988ad4fc04ee4a3f610de9044e933d47997d27a57'
  paths=[ROOT/'character_state.cpp',ROOT/'character_state.hpp',ROOT/'character_timers.cpp',ROOT/'character_timers.hpp',ROOT/'tests/character_state.cpp',ROOT/'tests/character_state_timers.cpp',Path(__file__)]
  report={'validation':'PASS','original_sha256':gold['original_sha256'],'prior_state_reference_sha256':sha(old),'timer_chain_reference_sha256':sha(ROOT/'reference/character-timer-discovery/state-timer-reference.bin'),'original_discovery_gold_sha256':sha(gold_path),'source_sha256':{str(s.relative_to(ROOT)):sha(s) for s in paths},'prior_corpus_host_replay':json.loads(a.state_host.read_text()),'new_gate_and_chain_host_replay':json.loads(a.chain_host.read_text()),'scope':'New 2a/2b/2c prefix plus original3910 corpus under host ASan/UBSan. AI expiry service and routing adapter are fixtures; no new ARM64/package proof. Existing original state gold/reports unchanged.'}
  assert report['prior_corpus_host_replay']['mismatches']==report['new_gate_and_chain_host_replay']['mismatches']==0
  report['host_executable_sha256']={name:sha(ROOT/'../../.local-inputs/character-timer-discovery'/name) for name in ('character_state_host','character_state_timers_host')}
  report['host_flags']='g++ -std=c++17 -O1 -g -Wall -Wextra -Werror -fsanitize=address,undefined -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off'
  (ROOT/'reports/character-state-timers-host-sanitizers.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps(bindings,indent=2))
if __name__=='__main__':main()
