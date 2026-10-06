import pathlib,runpy,subprocess,sys
root=pathlib.Path(__file__).resolve().parents[3]
kind=sys.argv[1]
# Reuse the verified platform configuration and rebuild the Module relay,
# whose private file-occurrence layout changes with this source fix.
runpy.run_path(str(pathlib.Path(__file__).parent/'build_canonical_module_files.py'),run_name='__main__')
targets=['dh2_loader_cached_file_reentry_probe','dh2_loader_connected_source_probe','dh2_loader_canonical_source_adapter_probe']
if kind in ('host','sanitizers'):
    build=root.parent/'build'/('connected-owner-'+kind)
    linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
    args=['wsl.exe','-d','Ubuntu','--','cmake','--build',linux(build)]
else:
    build=root.parent/'build'/('connected-owner-android-'+kind)
    args=[r'C:\Users\adamc\AppData\Local\Android\Sdk\cmake\3.22.1\bin\cmake.exe','--build',str(build)]
subprocess.run(args+['--target',*targets,'-j','4'],check=True)
