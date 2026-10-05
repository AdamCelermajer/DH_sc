from pathlib import Path
import subprocess,json,hashlib
H=Path(__file__).parent;R=Path('R:/port/game-data');B=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin')
names=['campaign_profile_files_v1.cpp','player_profile_index_v1.cpp','tests/campaign_profile_files_v1.cpp'];binary=H/'campaign-files-test-v69'
subprocess.run([str(B/'clang++.exe'),'--target=x86_64-linux-android26','-std=c++17','-O2','-Wall','-Wextra','-static-libstdc++']+[str(R/n) for n in names]+['-o',str(binary)],check=True)
adb=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-P','5038','-s','emulator-5580']
def call(*args):return subprocess.check_output(adb+list(args),encoding='utf8',timeout=45)
remote='/data/local/tmp/campaign-files-test-v69';call('push',str(binary),remote);call('shell','chmod','700',remote)
output=call('shell',remote);assert output.startswith('PASS')
(H/'campaign-files-test-v69.log').write_text(output)
(H/'campaign-files-test-v69.json').write_text(json.dumps({'status':'PASS','source_sha256':{n:hashlib.sha256((R/n).read_bytes()).hexdigest() for n in names},'scope':'Native filesystem cases over controlled temporary files. Policy recovered from original cache branches; not full original filesystem execution.'},indent=2)+'\n')
print(output)
