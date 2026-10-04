"""Bind the completed whole HUD formatter/skill producer proof without rebuilding."""
import hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 production=['port/engine-ui/hud_text_format_v1.cpp','port/engine-ui/hud_text_v1.cpp','port/level-world/character_skill_info_v1.cpp','port/level-world/character_skill_properties_v1.cpp','port/level-world/character_skill_info_session_v1.cpp','port/level-world/character_hud_skill_text_v1.cpp','port/script-runtime/script_runtime_return_v1.c','port/script-runtime/script_return_observer_v1.h']
 paths=production+[x[:-4]+'.hpp'for x in production if x.endswith('.cpp')]
 paths+=['port/engine-ui/tests/hud_text_format_v1.cpp','port/engine-ui/tests/hud_text_format_v1_differential.py','port/engine-ui/tests/hud_formatting_v1_original.py','port/engine-ui/tests/hud_skill_class_v1_original.py','port/engine-ui/tests/hud_skill_text_v1.cpp','port/engine-ui/tests/hud_formatting_v1_host.py','port/level-world/tests/character_skill_info_v1.cpp','port/level-world/tests/character_skill_info_v1_differential.py','port/script-runtime/tests/script_first_return_v1.cpp','port/engine-ui/tools/check_hud_formatting_v1_android.py','port/engine-ui/tools/freeze_hud_formatting_v1.py','port/script-runtime/script_runtime.c','.local-inputs/hud-formatting-v1/runtime-build/CMakeLists.txt','.local-inputs/localization-discovery/common-text-probe.json']
 refs=['port/engine-ui/reference/hud-formatting-v1','port/level-world/reference/character-skill-info-v1','port/script-runtime/reference/first-return-v1','port/script-runtime/reference/game-bindings']
 for directory in refs:
  paths += [x.relative_to(ROOT).as_posix()for x in (ROOT/directory).rglob('*')if x.is_file() and x.name!='freeze-manifest.json']
 reports=['port/engine-ui/reports/hud-text-format-v1-arm64-differential.json','port/level-world/reports/character-skill-info-v1-arm64-differential.json','port/engine-ui/reports/hud-formatting-v1-host-audit.json','port/engine-ui/reports/hud-formatting-v1-android-readiness.json']
 documents=[json.loads((ROOT/x).read_text())for x in reports]
 for d in documents:assert d['validation']=='PASS'
 for d in documents:
  for name,value in d.get('source_sha256',{}).items():assert sha(ROOT/name)==value,('Stale proof source',name)
 host=documents[2]
 for source in production:assert host['source_and_gold_sha256'][source]==sha(ROOT/source),('Stale host source',source)
 paths+=reports
 mapping={x:sha(ROOT/x)for x in sorted(set(paths))}
 receipt=dict(validation='PASS',source_and_evidence_sha256=mapping,original_elf_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',cache_zip_sha256='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679',production_sources=production,results=host['results'],sanitizers=host['sanitizers'],integration=dict(ui=production[:2],world=production[2:6],runtime_replacement=production[6],runtime_replaces='port/script-runtime/script_runtime.c',link_both_runtime_translation_units=False),scope=dict(parse_ex_original_arm64=1087,skill_info_original_arm64=640,real_class_script_cases=42,real_formatted_strings=84,complete_live_session=False,session_adapter_compile_linked_only=True,whole_property_wrapper_argument_corpus=False,normal_uncached_class_provider_required=True,external_recalculation_provider_required=True,packaged_or_gpu_parity=False))
 dest=ROOT/'port/engine-ui/reference/hud-formatting-v1/freeze-manifest.json';dest.write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(dict(validation='PASS',source_files=len(mapping),receipt_sha256=sha(dest),host_report_sha256=sha(ROOT/reports[2]))))
if __name__=='__main__':main()
