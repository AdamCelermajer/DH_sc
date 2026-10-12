"""Compile and replay the complete original dynamic text layout corpus.
Only this new subsystem is built. Existing archives, frozen sources and the
visible Android checkpoint are neither rebound nor promoted by this receipt.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def wp(p):return '/mnt/c/'+str(p)[3:].replace('\\','/')
def run(args):
 q=subprocess.run(args,capture_output=True,text=True)
 if q.returncode:raise RuntimeError(json.dumps({'command':args,'stdout':q.stdout,'stderr':q.stderr}))
 return q.stdout.strip()
def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,default=ROOT/'port/engine-ui/reports/text-layout-v1-host-audit.json');p.add_argument('--exe',default='/home/adampalace/dh2-text-layout-v1-audit');a=p.parse_args();ui=ROOT/'port/engine-ui';ref=ui/'reference/text-layout-v1';files=[ui/'text_layout_v1.hpp',ui/'text_layout_v1.cpp',ui/'tests/text_layout_v1.cpp',ui/'tests/text_layout_v1_original.py',Path(__file__),ref/'original-functions.json',ref/'reference/original-functions.asm',ref/'capture_dependencies.py',ref/'dependencies-and-literals.json',ref/'whole-gold.bin',ref/'whole-gold-v2.bin'];before={str(x.relative_to(ROOT)).replace('\\','/'):sha(x) for x in files}
 command=['wsl','-e','g++','-std=c++17','-O2','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',wp(ui/'text_layout_v1.cpp'),wp(ui/'tests/text_layout_v1.cpp'),'-o',a.exe];run(command);result=json.loads(run(['wsl','-e','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',a.exe,wp(ref/'whole-gold-v2.bin')]));assert before=={str(x.relative_to(ROOT)).replace('\\','/'):sha(x) for x in files},'Text sources changed during whole audit'
 manifest=json.loads((ref/'original-functions.json').read_text());assert sha(ROOT/'.local-inputs/libDungeonHunter2.so')==manifest['original_sha256'];assert result['whole_original_cases']==960
 report=dict(validation='PASS',results=result,source_and_gold_sha256=before,executable_sha256=run(['wsl','-e','sha256sum',a.exe]).split()[0],compiler_command=command,original_sha256=manifest['original_sha256'],sanitizers=['address','undefined','leak'],sanitizer_findings=0,scope=__doc__,limits={'original_layout_parser_and_image_coordinators_execute':True,'array_string_hash_and_font_producers_are_explicit_original_test_services':True,'C_locale_lowercase_table_is_external_fixture':True,'real_retained_UI_adapter_complete':False,'glyph_and_image_GPU_backend_complete':False,'source_reset_format_and_preload_bodies_recovered':False,'APK_promoted':False,'physical_device_execution':False});a.output.parent.mkdir(parents=True,exist_ok=True);assert not a.output.exists(),'Preserve completed receipts';a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
