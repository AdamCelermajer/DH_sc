<#
.SYNOPSIS
Runs several dh-foundation.exe jobs in parallel on a hidden Windows desktop, silent and low priority,
so nothing appears on screen, no window takes focus and no sound is played.
.DESCRIPTION
JobsFile is a JSON array of {name, exe, args[], cwd, log, timeoutSec}. Each job runs as
  cmd /c "exe args > log 2>&1" on a private desktop (CreateDesktop), with DH_AUDIO_SILENT=1 so the
WinMM output submits zeros (the mixer, counters and logs behave normally). Console windows are suppressed.
Results go to a JSON summary (name, exitCode, seconds, timedOut, log). Parse logs/captures afterwards.
Note: DH_AUDIO_SILENT needs an EXE built from source commit >= the one that added it.
#>
param(
  [Parameter(Mandatory=$true)][string]$JobsFile,
  [int]$Parallel = 8,
  [string]$Summary = "",
  [switch]$AllowSound
)
$ErrorActionPreference = 'Stop'
Add-Type -TypeDefinition @'
using System; using System.Collections.Generic; using System.Diagnostics; using System.Runtime.InteropServices; using System.Threading;
public static class QuietProc {
  [StructLayout(LayoutKind.Sequential, CharSet=CharSet.Unicode)]
  struct STARTUPINFO { public int cb; public string lpReserved; public string lpDesktop; public string lpTitle;
    public int dwX,dwY,dwXSize,dwYSize,dwXCountChars,dwYCountChars,dwFillAttribute,dwFlags; public short wShowWindow,cbReserved2;
    public IntPtr lpReserved2,hStdInput,hStdOutput,hStdError; }
  [StructLayout(LayoutKind.Sequential)] struct PROCESS_INFORMATION { public IntPtr hProcess,hThread; public int dwProcessId,dwThreadId; }
  [DllImport("user32.dll", CharSet=CharSet.Unicode, SetLastError=true)] static extern IntPtr CreateDesktop(string n,string d,IntPtr m,int f,uint a,IntPtr s);
  [DllImport("user32.dll")] static extern bool CloseDesktop(IntPtr h);
  [DllImport("kernel32.dll", CharSet=CharSet.Unicode, SetLastError=true)]
  static extern bool CreateProcess(string app,string cmd,IntPtr pa,IntPtr ta,bool inh,uint fl,IntPtr env,string cwd,ref STARTUPINFO si,out PROCESS_INFORMATION pi);
  [DllImport("kernel32.dll")] static extern uint WaitForSingleObject(IntPtr h,uint ms);
  [DllImport("kernel32.dll")] static extern bool GetExitCodeProcess(IntPtr h,out uint code);
  [DllImport("kernel32.dll")] static extern bool TerminateProcess(IntPtr h,uint code);
  [DllImport("kernel32.dll")] static extern bool CloseHandle(IntPtr h);
  public class Result { public int ExitCode; public double Seconds; public bool TimedOut; public string Error; }
  static Result RunOne(IntPtr desk,string deskName,string cmdline,string cwd,int timeoutMs){
    var r=new Result(); var sw=Stopwatch.StartNew();
    var si=new STARTUPINFO(); si.cb=Marshal.SizeOf(typeof(STARTUPINFO)); si.lpDesktop="WinSta0\\"+deskName;
    PROCESS_INFORMATION pi;
    uint flags=0x08000000u|0x00004000u; // CREATE_NO_WINDOW | BELOW_NORMAL_PRIORITY_CLASS
    if(!CreateProcess(null,cmdline,IntPtr.Zero,IntPtr.Zero,false,flags,IntPtr.Zero,cwd,ref si,out pi)){
      r.ExitCode=-1; r.Error="CreateProcess failed: "+Marshal.GetLastWin32Error(); return r; }
    uint w=WaitForSingleObject(pi.hProcess,(uint)timeoutMs);
    if(w==0x102){ TerminateProcess(pi.hProcess,1); WaitForSingleObject(pi.hProcess,5000); r.TimedOut=true; }
    uint code; GetExitCodeProcess(pi.hProcess,out code); r.ExitCode=(int)code;
    CloseHandle(pi.hProcess); CloseHandle(pi.hThread); r.Seconds=sw.Elapsed.TotalSeconds; return r;
  }
  public static Result[] RunMany(string[] cmdlines,string[] cwds,int[] timeoutsMs,int parallel){
    string deskName="dhquiet_"+Process.GetCurrentProcess().Id;
    IntPtr desk=CreateDesktop(deskName,null,IntPtr.Zero,0,0x10000000u,IntPtr.Zero);
    if(desk==IntPtr.Zero) throw new Exception("CreateDesktop failed: "+Marshal.GetLastWin32Error());
    var results=new Result[cmdlines.Length]; var sem=new SemaphoreSlim(parallel); var threads=new List<Thread>();
    for(int i=0;i<cmdlines.Length;i++){ int k=i; sem.Wait();
      var t=new Thread(()=>{ try{ results[k]=RunOne(desk,deskName,cmdlines[k],cwds[k],timeoutsMs[k]); } finally{ sem.Release(); } });
      t.Start(); threads.Add(t); }
    foreach(var t in threads) t.Join();
    CloseDesktop(desk); return results;
  }
}
'@
$jobs = Get-Content -Raw -LiteralPath $JobsFile | ConvertFrom-Json
if (-not $AllowSound) { $env:DH_AUDIO_SILENT = '1' }
$cmds=@(); $cwds=@(); $to=@()
foreach ($j in $jobs) {
  $argText = ($j.args | ForEach-Object { if ($_ -match '[\s"]') { '"' + ($_ -replace '"','\"') + '"' } else { $_ } }) -join ' '
  $cmds += ('cmd.exe /d /c ""{0}" {1} > "{2}" 2>&1"' -f $j.exe, $argText, $j.log)
  $cwds += $(if ($j.cwd) { $j.cwd } else { (Get-Location).Path })
  $to   += [int]($(if ($j.timeoutSec) { $j.timeoutSec } else { 300 }) * 1000)
}
$res = [QuietProc]::RunMany($cmds, $cwds, $to, $Parallel)
$out = for ($i=0; $i -lt $jobs.Count; $i++) {
  [pscustomobject]@{ name=$jobs[$i].name; exitCode=$res[$i].ExitCode; seconds=[math]::Round($res[$i].Seconds,1); timedOut=$res[$i].TimedOut; error=$res[$i].Error; log=$jobs[$i].log }
}
if ($Summary) { $out | ConvertTo-Json -Depth 4 | Set-Content -Encoding UTF8 -LiteralPath $Summary }
$out | Format-Table name,exitCode,seconds,timedOut,log -AutoSize | Out-String -Width 220
