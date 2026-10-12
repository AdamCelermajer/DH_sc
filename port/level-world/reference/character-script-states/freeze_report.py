import hashlib,json
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
arm=ROOT/'reports/character-script-states-arm64-differential.json';host=ROOT/'reports/character-script-states-host-audit.json';keys=HERE/'string-key-probe.json'
a,h,k=[json.loads(p.read_text()) for p in [arm,host,keys]];assert all(p['validation']=='PASS' for p in [a,h,k]);assert a['gold_sha256']==h['original_gold_sha256']==sha(HERE/'state-fixtures.bin');assert h['arm64_report_sha256']==sha(arm)
dependency_drift={}
for proof in [a,h]:
 for name,digest in proof['source_sha256'].items():
  current=sha(REPO/name)
  if current!=digest:
   assert name.replace('\\','/')=='port/script-runtime/script_runtime.c'
   dependency_drift[name]=dict(tested_source_sha256=digest,current_source_sha256=current,reason='Parent-authorized later native argument-storage correction; tested scoped DSO/proof remains historical, central replay pending.')
report=dict(validation='PASS',scope='Frozen state module proof against its actual historical scoped DSO. Current dependency correction requires a separate central replay. No invented authored Crypt registrations or full original VM/frame claim.',source_sha256=h['source_sha256'],dependency_source_drift=dependency_drift,current_dependencies_replayed=not dependency_drift,original_sha256=a['original_sha256'],original_ARM32_vs_optimized_ARM64_cases=a['comparisons'],ordered_services=a['ordered_services'],native_atomic_rejections=a['native_atomic_rejections'],source_key_conversion_cases=k['comparisons'],actual_native_host_checks=h['host_audit']['checks'],sanitizer_findings=h['sanitizer_findings'],proof_sha256={str(p.relative_to(REPO)):sha(p) for p in [arm,host,keys]},notes_sha256=sha(HERE/'NOTES.md'),freeze_script_sha256=sha(Path(__file__)),source_Change_supported_keys=['string','nil','Boolean'],unsupported_Change_keys=['numeric/identity Value.getString formatting'],actual_Crypt_registry_calls=0,Owner_extended=False,full_original_VM_executed=False,packaged_APK=False)
out=ROOT/'reports/character-script-states-source-validation.json';out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',ARM64=a['comparisons'],host=h['host_audit']['checks'],guards=a['native_atomic_rejections'])))
