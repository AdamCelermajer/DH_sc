"""Read-only boot/window evidence for the loader's own visible emulator."""
import ctypes,json,pathlib,subprocess,time
from ctypes import wintypes
root=pathlib.Path(__file__).resolve().parents[3]
assert root==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
base=[str(pathlib.Path.home()/'AppData/Local/Android/Sdk/platform-tools/adb.exe'),'-s','emulator-5590']
deadline=time.monotonic()+45
while True:
    boot=subprocess.run(base+['shell','getprop','sys.boot_completed'],capture_output=True,timeout=5)
    if boot.returncode==0 and boot.stdout.strip()==b'1':break
    if time.monotonic()>=deadline:raise TimeoutError('Loader emulator not booted yet')
    time.sleep(1)
name=subprocess.check_output(base+['emu','avd','name'],timeout=5).decode().replace('\r','').splitlines()
assert name[0]=='DH2_Loader_API37',name
user=ctypes.WinDLL('user32',use_last_error=True)
user.GetWindowTextLengthW.argtypes=[wintypes.HWND]
user.GetWindowTextW.argtypes=[wintypes.HWND,wintypes.LPWSTR,ctypes.c_int]
user.IsWindowVisible.argtypes=[wintypes.HWND]
user.GetWindowThreadProcessId.argtypes=[wintypes.HWND,ctypes.POINTER(wintypes.DWORD)]
user.GetWindowRect.argtypes=[wintypes.HWND,ctypes.POINTER(wintypes.RECT)]
callback=ctypes.WINFUNCTYPE(wintypes.BOOL,wintypes.HWND,wintypes.LPARAM)
windows=[]
@callback
def visit(handle,param):
    title=ctypes.create_unicode_buffer(user.GetWindowTextLengthW(handle)+1)
    user.GetWindowTextW(handle,title,len(title))
    if '5590' in title.value or 'DH2_Loader_API37' in title.value or 'DH2 Loader API37' in title.value:
        pid=wintypes.DWORD();user.GetWindowThreadProcessId(handle,ctypes.byref(pid))
        rect=wintypes.RECT();user.GetWindowRect(handle,ctypes.byref(rect))
        windows.append({'title':title.value,'pid':pid.value,'visible':bool(user.IsWindowVisible(handle)),
                        'bounds':[rect.left,rect.top,rect.right,rect.bottom]})
    return True
user.EnumWindows.argtypes=[callback,wintypes.LPARAM]
user.EnumWindows(visit,0)
assert any(w['visible'] for w in windows),f'No visible loader emulator window: {windows}'
receipt={'avd':name[0],'serial':'emulator-5590','booted':True,'windows':windows}
path=root/'port/level-loader/reports/visible-emulator-window.json'
path.write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps(receipt))
