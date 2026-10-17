#!/usr/bin/env python3
"""Run the current native source Lua selftests on an Android 17 emulator."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser()
    for n in ('adb','runner','build-report','report'):p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--serial',required=True);a=p.parse_args()
    def run(*args):return subprocess.check_output([str(a.adb),'-s',a.serial,*map(str,args)],text=True,encoding='utf-8',errors='replace',timeout=60).strip()
    build=json.loads(a.build_report.read_text(encoding='utf-8'))
    assert sha(a.runner)==build['artifacts'][a.runner.name]['sha256']
    for name,digest in build['source_sha256'].items():assert sha(ROOT/name)==digest,name
    assert sha(ROOT/'build.py')==build['build_tool_sha256']
    device={'serial':a.serial,'release':run('shell','getprop','ro.build.version.release'),'sdk':int(run('shell','getprop','ro.build.version.sdk')),
        'page_size':int(run('shell','getconf','PAGE_SIZE')),'abi':run('shell','getprop','ro.product.cpu.abi')}
    assert run('shell','getprop','ro.kernel.qemu')=='1'and device['release']=='17'and device['sdk']==37 and device['abi']=='x86_64'and device['page_size']in (4096,16384)
    remote='/data/local/tmp/dh2-lua-current-selftest'
    run('push',a.runner.resolve(),remote);assert run('shell','sha256sum',remote).split()[0]==sha(a.runner)
    run('shell','chmod','700',remote);output=run('shell',remote)
    assert output=='SELFTEST PASS\nNUMERIC PASS 504',output
    report={'complete_game':False,'device':device,'runner_sha256':sha(a.runner),'build_report_sha256':sha(a.build_report),'test_sha256':sha(Path(__file__)),
        'output':output,'passed':True,'scope':'Current compiled x86_64 runner executes all source runtime selftests, including health/mana, non-player hit policy/rollback, legacy properties/classes/equipment/gears, random and 504 numeric vectors. This is an emulator native runner, not original gameplay or ARM64 hardware execution.'}
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps(report))
if __name__=='__main__':main()
