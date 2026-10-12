from pathlib import Path
import subprocess,shlex,json,hashlib
p=Path(r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');build=p.parents[2]/'build/receiver-transport-v5-host';out=p/'reference/actor-image-binding-v45';linux=lambda f:'/mnt/c/'+str(f).replace('\\','/')[3:]
commands=subprocess.check_output(['wsl.exe','-d','Ubuntu','--','ninja','-C',linux(build),'-t','commands','dh2_loader_source_visual_helper_v41_probe'],text=True)
selected=next(line for line in commands.splitlines() if '/tests/source_visual_helper_v41_probe.cpp' in line and ' -c ' in line);args=shlex.split(selected);compile_end=args.index('-o');base=[];i=0
while i<compile_end:
 if args[i] in ['-MT','-MF']:i+=2;continue
 if args[i]=='-MD':i+=1;continue
 base.append(args[i]);i+=1
base+=['-I'+linux(p/'vendor/character-rng-integration-v5-loading/port/asset-payloads')]
results=[]
for file in [p/'generic_actor_image_binding_v45.cpp',p/'tests/generic_actor_image_binding_v45_probe.cpp']:
 command=['wsl.exe','-d','Ubuntu','--']+base+['-fsyntax-only',linux(file)];result=subprocess.run(command,capture_output=True,text=True);(out/(file.stem+'-syntax.log')).write_text(result.stdout+result.stderr);print(file.name,result.returncode,result.stderr[-3000:]);result.check_returncode();results.append({'file':str(file),'sha256':hashlib.sha256(file.read_bytes()).hexdigest(),'command':command,'status':'SYNTAX_PASS'})
receipt={'validation':'SYNTAX_PASS_ONLY','full_test_linked':False,'runtime_probe_run':False,'coherent_parent_rebuild_required':True,'flags_extracted_from_existing_v5_target_only_no_archive_link':True,'results':results};(out/'syntax-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');(out/'check_syntax.py').write_text(Path(__file__).read_text());print(json.dumps(receipt,indent=2))
