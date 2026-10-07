"""Isolated actual-DSO-linked sanitized replay; does not touch central builds."""
import argparse,hashlib,json,shlex,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+str(p)[2:].replace('\\','/')
def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,default=ROOT/'reports/particle-factory-host-audit.json');a=p.parse_args();folder=REPO/'.local-inputs/fx-particle-factory-discovery/host';folder.mkdir(parents=True,exist_ok=True)
 cpp=ROOT/'particle_factory.cpp';test=ROOT/'tests/particle_factory.cpp';gold=ROOT/'reference/fx-particle-factory/particle-factory-fixtures.bin';lib=folder/'libdh2_particle_factory_audit.so';exe=folder/'particle_factory_audit';flags=['-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror','-Wno-misleading-indentation']
 def command(argv):
  result=subprocess.run(['wsl','--','bash','-lc',shlex.join(argv)],capture_output=True,text=True);assert result.returncode==0,(result.returncode,result.stdout,result.stderr);return result
 sources=[ROOT/'particle_factory.hpp',cpp,test,Path(__file__),REPO/'port/engine-resources/resources.hpp',REPO/'port/engine-resources/resources.cpp'];before={str(x.relative_to(REPO)).replace('\\','/'):sha(x)for x in sources}
 compiler=command(['g++','--version']).stdout.strip();command(['g++',*flags,'-shared','-fPIC',linux(cpp),linux(REPO/'port/engine-resources/resources.cpp'),'-o',linux(lib)]);command(['g++',*flags,linux(test),'-L'+linux(folder),'-ldh2_particle_factory_audit','-Wl,-rpath,'+linux(folder),'-o',linux(exe)])
 result=command(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(exe),linux(gold),linux(REPO/'.local-inputs/character-skeleton-fx-assets/zombie_spawn_fx.bdae')]);assert not result.stderr,result.stderr;observed=json.loads(result.stdout);assert observed['validation']=='PASS';assert before=={str(x.relative_to(REPO)).replace('\\','/'):sha(x)for x in sources}
 report={**observed,'source_sha256':before,'library_sha256':sha(lib),'executable_sha256':sha(exe),'reference_sha256':sha(gold),'original_instruction_report_sha256':sha(ROOT/'reports/particle-factory-arm64-differential.json'),'isolated_particle_factory_dso_executed':True,'main_world_library_executed':False,'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'compiler':compiler,'flags':flags,'scope':'Original-derived context/generation constructor and registry gold replay through genuine isolated particle-factory DSO. No particle registry/factory/emission, Scene/GPU or complete FX playback claim.'};a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items()if k not in ('source_sha256','compiler','flags','scope')}))
if __name__=='__main__':main()
