from pathlib import Path
exec(Path(__file__).with_name('produce_level_savegame_backend_v2.py').read_text().split('records=[]')[0])
got=(0x314744+8+word(0x314738+8+0x560))&0xffffffff
active=word(got+word(0x31476c+8+0x538));queue=word(got+word(0x314980+8+0x350));state=(0x31474c+8+word(0x314740+8+0x560))&0xffffffff;app=word(got+word(0x3149cc+8+0x300))
device=cpu.data+0x11000;manager=cpu.data+0x12000;mvt=cpu.data+0x13000;job=cpu.data+0x14000;file=cpu.data+0x15000;fvt=cpu.data+0x16000;rawvec=cpu.data+0x18000;rawdata=cpu.data+0x21000
w(app+16,device);w(device+0x34,manager);w(manager,mvt);w(file,fvt)
for off in [0x94,0x78]:a=cpu.data+0xec00+off;w(mvt+off,a);w(a,0xe12fff1e)
for off in [0x1c,0x20,0x2c]:a=cpu.data+0xdf00+off;w(fvt+off,a);w(a,0xe12fff1e)
callbacks[0x34]='indexedsize';w(vt+0x34,cpu.data+0xf034);w(cpu.data+0xf034,0xe12fff1e)
output=bytearray();outputpos=0;operations=[]
def jobhook(uc,a,n,p):
 global output,outputpos
 if a==0x313cdc:
  dst,src=cpu.reg(0),cpu.reg(1);uc.mem_write(dst,bytes(uc.mem_read(src,32)));uc.mem_write(src+29,b'\0');ret()
 elif a==0x3140b0:w(queue,queue);w(queue+4,queue);ret()
 elif a==0x313720:w(cpu.reg(0),0);uc.mem_write(cpu.reg(0)+29,b'\0');operations.append(['finish']);ret()
 elif a==cpu.data+0xec94:output=bytearray();outputpos=0;operations.append(['open',bool(cpu.reg(2))]);ret(file)
 elif a==cpu.data+0xec78:w(cpu.reg(1),0);operations.append(['close']);ret()
 elif a in [cpu.data+0xdf20,cpu.data+0xdf2c]:outputpos=cpu.reg(2)|(cpu.reg(3)<<32);operations.append(['seek',outputpos]);ret()
 elif a==cpu.data+0xdf1c:
  size=cpu.reg(2)|(cpu.reg(3)<<32);data=bytes(uc.mem_read(cpu.reg(1),size));operations.append(['write',outputpos,data.hex()]);
  if outputpos+size>len(output):output.extend(bytes(outputpos+size-len(output)))
  output[outputpos:outputpos+size]=data;outputpos+=size;ret(size)
 elif a==cpu.data+0xf034:ret(min(2048,len(buffer)-cpu.reg(1)*2048))
 elif a==cpu.data+0xf01c:uc.mem_write(rawdata,bytes(buffer))
cpu.uc.hook_add(UC_HOOK_CODE,jobhook)
records=[]
for size in [4,2047,2048,2049,4097]:
 buffer=bytearray((i*17+size)%256 for i in range(size));buffer[:4]=struct.pack('<I',2);original=bytes(buffer);cursor=readpos=0;operations=[];uc.mem_write(rawdata,original);uc.mem_write(active,bytes(32));uc.mem_write(state,bytes(32));uc.mem_write(job,bytes(48));w(stream,vt);w(stream+0x1c,rawvec);w(stream+0x20,rawvec+(((size+2047)//2048)+1)*4)
 for i in range((size+2047)//2048):w(rawvec+i*4,rawdata+i*2048)
 w(queue,job);w(queue+4,job);w(job,queue);w(job+4,queue);w(job+8,stream);name=cpu.data+0x17000;uc.mem_write(name,b'save\0');w(job+0x1c,name+4);w(job+0x20,name);uc.mem_write(job+0x24,b'\0\1')
 cpu.symbols['jobs']=0x314734;returns=[]
 for step in range(16):
  result=cpu.invoke('jobs',[]);returns.append(result)
  if not result:break
 assert output==original,(size,len(output),operations);records.append({'size':size,'input':original.hex(),'returns':returns,'operations':operations[:]})
out=root/'port/level-world/reference/level-savegame-v2';out.mkdir(exist_ok=True)
(out/'jobs-gold.json').write_text(json.dumps({'original_elf_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'cases':records,'scope':'Whole original UpdateJobs314734 executes source2048-block progression, invalid header before chunk, final captured count commit, close/finish and busy state. Original queued list node, Job copy/erase/finish, FileManager open/close and stream byte virtuals are declared fixtures. Platform file syscall outcomes and backup path are not oracle accepted.'},indent=2)+'\n');print('Original UpdateJobs5 block/commit cases PASS')
lines=['#pragma once','struct SaveJobGoldenV2{unsigned size,updates;const char* trace;};','static const SaveJobGoldenV2 save_jobs_gold_v2[]={']
for r in records:
 trace=[]
 for op in r['operations']:
  if op[0]=='finish':continue
  if op[0]=='open':trace.append('open1')
  elif op[0]=='seek':trace.append('seek'+str(op[1]))
  elif op[0]=='close':trace.append('close')
  elif op[0]=='write':
   b=bytes.fromhex(op[2]);h=14695981039346656037
   for x in b:h=((h^x)*1099511628211)&0xffffffffffffffff
   trace.append(f'write{op[1]}:{len(b)}:{h}')
 lines.append('{%d,%d,%s},'%(r['size'],len(r['returns']),json.dumps(','.join(trace))))
lines.append('};');(out/'jobs-gold.hpp').write_text('\n'.join(lines)+'\n')
