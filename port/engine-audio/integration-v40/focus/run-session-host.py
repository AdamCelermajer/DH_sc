from pathlib import Path
import subprocess,json,hashlib
ROOT=Path(__file__).resolve().parents[4];OUT=Path(__file__).resolve().parent;BUILD=ROOT/'.local-inputs/audio-v40-session';BUILD.mkdir(parents=True,exist_ok=True)
sources=['port/engine-audio/integration-v40/focus/'+name+'.cpp'for name in ('audio_native_session_v40','audio_lifecycle_gate_v40','session-fixture')]+['port/level-world/vox_play3d_owner_v2.cpp']+['port/engine-audio/'+name+'.cpp'for name in ('audio_clock_v40','audio_gameplay_runtime_v40','audio_source_bindings_v38','audio_sample_v34','audio_mixer_v34','audio_catalog_v34','audio_bank_v34','audio_native_envelope_v34')]
results=[]
for optimization in ('O1','O2'):
    exe=f'.local-inputs/audio-v40-session/session-{optimization}'
    args=['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec','g++','-std=c++17','-'+optimization,'-DDH2_AUDIO_NATIVE_SESSION_FIXTURE','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fsanitize=address,undefined','-fno-omit-frame-pointer','-pthread',*sources,'-o',exe]
    result=subprocess.run(args,cwd=ROOT,capture_output=True,text=True,timeout=180);assert result.returncode==0,result.stderr
    for mode in ('success','failed_close','close_during_init','throw_read','failed_tick'):
        args=['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec',exe,'.']+([mode]if mode!='success' else [])
        result=subprocess.run(args,cwd=ROOT,capture_output=True,text=True,timeout=20);print(result.stdout,result.stderr);assert result.returncode==0
        results.append(dict(optimization=optimization,mode=mode,exit_code=0,output=result.stdout.strip()))
report=dict(runs=results,scope='Borrowed fake output-control factory only; actual runtime parses exact original three assets. No AAudio/device/World-positive playback acceptance. Failed close runs in bounded child process and intentionally retains owner until process exit.',dependency_warning_boundary='Existing frozen/parent dependencies retain host misleading-indentation suppression; owned production session separately compiled strictly for both Android ABIs.',source_sha256={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()for p in sources})
(OUT/'session-host-validation.json').write_text(json.dumps(report,indent=2)+'\n')
