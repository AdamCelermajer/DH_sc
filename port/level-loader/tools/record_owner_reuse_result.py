import pathlib,subprocess,json,hashlib
root=pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc');build=root.parent/'build/canonical-owner-reuse'
probe=build/'loader/dh2_loader_canonical_source_adapter_probe';linux='/mnt/c/'+str(probe).replace('\\','/')[3:]
p=subprocess.run(['wsl.exe','-d','Ubuntu','--',linux],capture_output=True,timeout=30)
assert p.returncode==0 and not p.stderr,p.stderr;result=json.loads(p.stdout)
assert result['validation']=='PASS' and result['adapter_checks']==14
config=json.loads((build/'owner-reuse-check.json').read_text());assert config['validation']=='PASS' and not config['second_owner_target_created']
config.update({'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'result':result,'scope':'Existing canonical owner target integration fixture; not production runtime acceptance'})
(root/'port/level-loader/reports/canonical-owner-reuse-check.json').write_text(json.dumps(config,indent=2)+'\n');print(json.dumps(config,indent=2))
