"""Hash-bound source literal and actual HUD timeline inventory; not a SWF VM proof."""
import hashlib,json,struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 elfpath=ROOT/'.local-inputs/libDungeonHunter2.so';swfpath=ROOT/'.local-inputs/ui-layout-discovery/dqhud_droid.swf';raw=swfpath.read_bytes();assert raw[:3]==b'FWS';assert len(raw)==struct.unpack_from('<I',raw,4)[0]
 defs={}
 def tags(start,end):
  out=[];pos=start
  while pos<end:
   offset=pos;code=struct.unpack_from('<H',raw,pos)[0];pos+=2;tag=code>>6;size=code&63
   if size==63:size=struct.unpack_from('<I',raw,pos)[0];pos+=4
   assert pos+size<=end;body=raw[pos:pos+size];entry={'tag':tag,'offset':offset,'length':size,'clip_actions':tag in(26,70) and bool(body[0]&128)}
   if tag==39:
    cid,count=struct.unpack_from('<HH',body);child=tags(pos+4,pos+size);defs[cid]={'id':cid,'frames':count,'tags':child}
   if tag==26 and body[0]&2:entry['character_id']=struct.unpack_from('<H',body,3)[0]
   out.append(entry);pos+=size
   if tag==0:break
  return out
 n=raw[8]>>3;tags(8+(5+4*n+7)//8+4,len(raw))
 with elfpath.open('rb') as f:
  e=ELFFile(f);segments=[(s['p_vaddr'],s.data()) for s in e.iter_segments() if s['p_type']=='PT_LOAD']
  def read(a,n):return next(data[a-base:a-base+n] for base,data in segments if base<=a<base+len(data))
  def word(a):return struct.unpack('<I',read(a,4))[0]
  def string(pc,lit):
   a=pc+8+word(lit);return dict(address=hex(a),value=read(a,256).split(b'\0')[0].decode('utf8'))
  literals=[string(pc,lit) for pc,lit in ((0x41d8fc,0x41dc94),(0x41d944,0x41dca0),(0x41d95c,0x41dca4),(0x41d914,0x41dc98),(0x41d92c,0x41dc9c))]
  region=read(0x41e0f4,0x41e208-0x41e0f4)
 rows=[]
 for idx,cid in enumerate((90,147,152,113,31)):
  visited=set()
  def visit(i):
   if i not in defs or i in visited:return
   visited.add(i)
   for tag in defs[i]['tags']:
    if 'character_id'in tag:visit(tag['character_id'])
  visit(cid);action_tags=[x for i in visited for x in defs[i]['tags'] if x['tag'] in(12,59) or x['clip_actions']]
  assert not action_tags
  rows.append(dict(index=idx,**literals[idx],character_id=cid,frames=defs[cid]['frames'],direct_tags=sorted({t['tag'] for t in defs[cid]['tags']}),reachable_sprite_ids=sorted(visited),reachable_action_or_clip_action_count=len(action_tags)))
 report=dict(original_sha256=sha(elfpath),hud_swf_sha256=sha(swfpath),bounded_status_region=dict(start='0x41e0f4',end_exclusive='0x41e208',sha256=hashlib.sha256(region).hexdigest()),cache_base=string(0x41d8d4,0x41dc90),cache_style=string(0x41d8c0,0x41dc8c),cached_resolved_indices=dict(hp=36,max_hp=38,mp=41,max_mp=43,xp=33,max_xp=34),targets=rows,scope='Source literals decoded from original initCachedChars; original FastUpdate region bytes bound. Actual uncompressed dqhud sprite inventory, including recursively referenced sprite placements. No SWF parser/core instruction parity, timeline scheduling or source HUDStyle constant producer claimed.')
 p=ROOT/'port/engine-ui/reference/hud-player-values/bindings.json';p.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
