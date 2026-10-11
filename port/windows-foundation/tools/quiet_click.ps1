<#
.SYNOPSIS
Runs ONE dh-foundation.exe job on a private hidden desktop (like quiet_run.ps1: silent, below normal priority) and injects
REAL Win32 mouse messages into its window while it runs (B103). Nothing touches the user's cursor or desktop.
.DESCRIPTION
The game window is found on the private desktop; a helper thread attached to that desktop gives it focus and posts
WM_MOUSEMOVE / WM_LBUTTONDOWN / WM_LBUTTONUP exactly as the OS delivers them to a window (client coordinates).
Clicks: "T:X:Y:HOLD" comma separated = T seconds after the window exists, client pixel X,Y, button held HOLD milliseconds.
A negative X posts WM_KEYDOWN/UP with virtual key -X (e.g. "5:-67:0:80" = C key).
Example: -Clicks "6:40:30:60,8:700:90:30"
#>
param(
  [Parameter(Mandatory=$true)][string]$Exe,
  [Parameter(Mandatory=$true)][string[]]$ExeArgs,
  [Parameter(Mandatory=$true)][string]$Cwd,
  [Parameter(Mandatory=$true)][string]$Log,
  [string]$Clicks = "",
  [int]$TimeoutSec = 60
)
$ErrorActionPreference = 'Stop'
Add-Type -TypeDefinition @'
using System; using System.Collections.Generic; using System.Diagnostics; using System.Runtime.InteropServices; using System.Threading; using System.Text;
public static class QuietClick {
  [StructLayout(LayoutKind.Sequential, CharSet=CharSet.Unicode)]
  struct STARTUPINFO { public int cb; public string lpReserved; public string lpDesktop; public string lpTitle;
    public int dwX,dwY,dwXSize,dwYSize,dwXCountChars,dwYCountChars,dwFillAttribute,dwFlags; public short wShowWindow,cbReserved2;
    public IntPtr lpReserved2,hStdInput,hStdOutput,hStdError; }
  [StructLayout(LayoutKind.Sequential)] struct PROCESS_INFORMATION { public IntPtr hProcess,hThread; public int dwProcessId,dwThreadId; }
  delegate bool EnumProc(IntPtr h, IntPtr l);
  [DllImport("user32.dll", CharSet=CharSet.Unicode, SetLastError=true)] static extern IntPtr CreateDesktop(string n,string d,IntPtr m,int f,uint a,IntPtr s);
  [DllImport("user32.dll")] static extern bool CloseDesktop(IntPtr h);
  [DllImport("user32.dll")] static extern bool SetThreadDesktop(IntPtr h);
  [DllImport("user32.dll")] static extern bool EnumDesktopWindows(IntPtr d, EnumProc p, IntPtr l);
  [DllImport("user32.dll")] static extern uint GetWindowThreadProcessId(IntPtr h, out uint pid);
  [DllImport("user32.dll")] static extern bool IsWindowVisible(IntPtr h);
  [DllImport("user32.dll")] static extern bool PostMessage(IntPtr h, uint m, IntPtr w, IntPtr l);
  [DllImport("user32.dll")] static extern bool GetClientRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] static extern bool AttachThreadInput(uint a, uint b, bool f);
  [DllImport("user32.dll")] static extern IntPtr SetFocus(IntPtr h);
  [DllImport("user32.dll")] static extern bool SetForegroundWindow(IntPtr h);
  [DllImport("user32.dll")] static extern IntPtr GetFocus();
  [DllImport("kernel32.dll")] static extern uint GetCurrentThreadId();
  [StructLayout(LayoutKind.Sequential)] struct RECT { public int l,t,r,b; }
  [DllImport("kernel32.dll", CharSet=CharSet.Unicode, SetLastError=true)]
  static extern bool CreateProcess(string app,string cmd,IntPtr pa,IntPtr ta,bool inh,uint fl,IntPtr env,string cwd,ref STARTUPINFO si,out PROCESS_INFORMATION pi);
  [DllImport("kernel32.dll")] static extern uint WaitForSingleObject(IntPtr h,uint ms);
  [DllImport("kernel32.dll")] static extern bool GetExitCodeProcess(IntPtr h,out uint code);
  [DllImport("kernel32.dll")] static extern bool TerminateProcess(IntPtr h,uint code);
  [DllImport("kernel32.dll")] static extern bool CloseHandle(IntPtr h);
  const uint WM_MOUSEMOVE=0x200, WM_LBUTTONDOWN=0x201, WM_LBUTTONUP=0x202;
  static IntPtr MakeL(int x,int y){ return (IntPtr)(((y&0xFFFF)<<16)|(x&0xFFFF)); }
  static IntPtr FindWin(IntPtr desk,int pid){
    IntPtr found=IntPtr.Zero;
    EnumDesktopWindows(desk,(h,l)=>{ uint p; GetWindowThreadProcessId(h,out p); if(IsWindowVisible(h)){ RECT r; if(GetClientRect(h,out r)&&r.r>0){found=h;return false;} } return true; },IntPtr.Zero);
    return found;
  }
  public static int Run(string cmdline,string cwd,int timeoutMs,double[] clicks,out string report){
    var sb=new StringBuilder();
    string deskName="dhclick_"+Process.GetCurrentProcess().Id;
    IntPtr desk=CreateDesktop(deskName,null,IntPtr.Zero,0,0x10000000u,IntPtr.Zero);
    if(desk==IntPtr.Zero) throw new Exception("CreateDesktop failed: "+Marshal.GetLastWin32Error());
    var si=new STARTUPINFO(); si.cb=Marshal.SizeOf(typeof(STARTUPINFO)); si.lpDesktop="WinSta0\\"+deskName;
    PROCESS_INFORMATION pi;
    if(!CreateProcess(null,cmdline,IntPtr.Zero,IntPtr.Zero,false,0x08000000u|0x00004000u,IntPtr.Zero,cwd,ref si,out pi)) throw new Exception("CreateProcess failed: "+Marshal.GetLastWin32Error());
    var injector=new Thread(()=>{
      try{
        if(!SetThreadDesktop(desk)){ sb.AppendLine("SetThreadDesktop failed "+Marshal.GetLastWin32Error()); return; }
        IntPtr hwnd=IntPtr.Zero; var sw=Stopwatch.StartNew();
        while(hwnd==IntPtr.Zero&&sw.ElapsedMilliseconds<timeoutMs){ hwnd=FindWin(desk,0); if(hwnd==IntPtr.Zero)Thread.Sleep(50); }
        if(hwnd==IntPtr.Zero){ sb.AppendLine("window not found"); return; }
        uint pid; uint tid=GetWindowThreadProcessId(hwnd,out pid);
        AttachThreadInput(GetCurrentThreadId(),tid,true); SetForegroundWindow(hwnd); SetFocus(hwnd);
        sb.AppendLine("window "+hwnd+" focus="+(GetFocus()==hwnd));
        var t0=Stopwatch.StartNew();
        for(int i=0;i+3<clicks.Length;i+=4){
          int wait=(int)(clicks[i]*1000-t0.ElapsedMilliseconds); if(wait>0)Thread.Sleep(wait);
          int x=(int)clicks[i+1], y=(int)clicks[i+2], hold=(int)clicks[i+3];
          if(x<0){ PostMessage(hwnd,0x100,(IntPtr)(-x),IntPtr.Zero); Thread.Sleep(hold); PostMessage(hwnd,0x101,(IntPtr)(-x),IntPtr.Zero); sb.AppendLine("key t="+(t0.ElapsedMilliseconds/1000.0).ToString("F2")+" vk="+(-x)); continue; }
          PostMessage(hwnd,WM_MOUSEMOVE,IntPtr.Zero,MakeL(x,y)); Thread.Sleep(30);
          PostMessage(hwnd,WM_LBUTTONDOWN,(IntPtr)1,MakeL(x,y)); Thread.Sleep(hold);
          PostMessage(hwnd,WM_LBUTTONUP,IntPtr.Zero,MakeL(x,y));
          sb.AppendLine("click t="+(t0.ElapsedMilliseconds/1000.0).ToString("F2")+" x="+x+" y="+y+" hold="+hold+"ms");
        }
      } catch(Exception e){ sb.AppendLine("injector error "+e.Message); }
    });
    injector.Start();
    uint w=WaitForSingleObject(pi.hProcess,(uint)timeoutMs);
    if(w==0x102){ try{ Process.Start(new ProcessStartInfo("taskkill.exe","/PID "+pi.dwProcessId+" /T /F"){CreateNoWindow=true,UseShellExecute=false}).WaitForExit(10000);}catch(Exception){} TerminateProcess(pi.hProcess,1); sb.AppendLine("timed out (killed)"); }
    injector.Join(2000);
    uint code; GetExitCodeProcess(pi.hProcess,out code);
    CloseHandle(pi.hProcess); CloseHandle(pi.hThread); CloseDesktop(desk);
    report=sb.ToString(); return (int)code;
  }
}
'@
$env:DH_AUDIO_SILENT = '1'
$q = { param($a) if ($a -match '[\s"]') { '"' + ($a -replace '"','\"') + '"' } else { $a } }
$cmd = 'cmd /c "' + (& $q $Exe) + ' ' + (($ExeArgs | ForEach-Object { & $q $_ }) -join ' ') + ' > ' + (& $q $Log) + ' 2>&1"'
$nums = New-Object System.Collections.Generic.List[double]
if ($Clicks) { foreach ($c in $Clicks.Split(',')) { foreach ($n in $c.Split(':')) { $nums.Add([double]$n) } } }
$report = ''
$code = [QuietClick]::Run($cmd, $Cwd, $TimeoutSec * 1000, $nums.ToArray(), [ref]$report)
Write-Output "exit=$code"
Write-Output $report
