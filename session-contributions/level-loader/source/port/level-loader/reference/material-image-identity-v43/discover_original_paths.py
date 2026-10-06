from pathlib import Path
import zipfile,struct,json,collections,subprocess,hashlib
out=Path(r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/material-image-identity-v43');out.mkdir(parents=True,exist_ok=True)
elf=Path(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so')
exe=r'C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-objdump.exe'
for name,a,b in [('factory',0x6594a0,0x659768),('construct-image',0x60fc70,0x60fd54),('filename-hacks',0x34e96c,0x34ebbc),('texture-manager',0x5ed210,0x5ed380)]:
 (out/(name+'.asm')).write_text(subprocess.check_output([exe,'-d','--demangle',f'--start-address={a}',f'--stop-address={b}',str(elf)],text=True))
# ELF virtual string reads without depending on pyelftools.
eb=elf.read_bytes();ph=struct.unpack_from('<I',eb,28)[0];stride,count=struct.unpack_from('<HH',eb,42)
segments=[struct.unpack_from('<8I',eb,ph+i*stride) for i in range(count)]
def etext(va):
 for typ,off,addr,_,fs,_,_,_ in segments:
  if typ==1 and addr<=va<addr+fs:
   pos=off+va-addr;return eb[pos:eb.index(b'\0',pos)].decode(errors='replace')
 raise ValueError(hex(va))
strings={hex(va):etext(va) for va in [0x6594d8+8+0x267780,0x659614+8+0x267644,0x34ea0c+8+0x57972c,0x34ea3c+8+0x571cfc]}
(out/'literal-strings.json').write_text(json.dumps(strings,indent=2))
zp=Path(r'C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');z=zipfile.ZipFile(zp);prefix='com.gameloft.android.gand.gloftd2ss/files/'
directory={n.lower()[len(prefix):]:n for n in z.namelist() if n.lower().startswith(prefix) and not n.endswith('/')}
paths=collections.Counter();examples={};records=0;bres=0;errors=[];samplers=collections.Counter();array_examples=[]
for key,n in directory.items():
 if not key.endswith('.bdae'):continue
 b=z.read(n)
 if not b.startswith(b'BRES'):continue
 bres+=1;w=lambda p:struct.unpack_from('<I',b,p)[0];text=lambda p:b[p:b.index(b'\0',p)].decode() if p else '';r=w(32)
 try:
  for i in range(w(r+0x4c)):
   q=w(r+0x50)+20*i;s=text(w(q+8));paths[s]+=1;examples.setdefault(s,key);records+=1
  for i in range(w(r+0x5c)):
   q=w(r+0x60)+36*i
   for j in range(w(q+16)):
    p=w(q+20)+24*j
    if w(p+8)==11:
     samplers[text(w(p))]+=1
     if w(p+12)!=1 or w(w(p+16))!=1:array_examples.append([key,text(w(q)),text(w(p)),w(p+12)])
 except Exception as e:errors.append([key,str(e)])
resolved=[];missing=[];prefixes=collections.Counter()
for s,count in paths.items():
 prefixes[s[:s.rfind('/')+1]]+=count
 # Explicit corpus normalization candidate, not yet original producer proof.
 canonical=s.lower().replace('\\','/')
 if canonical.startswith('q:/data/iphone/'):canonical='data/'+canonical[len('q:/data/iphone/'):]
 result={'authored':s,'canonical_candidate':canonical,'references':count,'example':examples[s]}
 (resolved if canonical in directory else missing).append(result)
report={'original_sha256':hashlib.sha256(eb).hexdigest(),'cache_sha256':hashlib.sha256(zp.read_bytes()).hexdigest(),'cache_files':len(directory),'bres_files':bres,'image_records':records,'unique_images':len(paths),'prefixes':dict(prefixes),'sampler_names':dict(samplers),'sampler_array_examples':array_examples,'decode_errors':errors,'exact_candidate_found':resolved,'candidate_missing':missing}
(out/'image-census.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ('exact_candidate_found','candidate_missing')},indent=2));print('found',len(resolved),'missing',len(missing));print(json.dumps(missing[:12],indent=2));print(strings)
