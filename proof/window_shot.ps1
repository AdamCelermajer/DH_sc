# Windows flow, real window: starts dh2-save-editor.exe as a user would (double click = no arguments), waits for its Edge
# app window, saves a screenshot of that window only, then closes what it started.
# usage: powershell -File proof\window_shot.ps1 -App <folder with dh2-save-editor.exe> -Out <png>
param([string]$App, [string]$Out)
Add-Type -AssemblyName System.Drawing
Add-Type @"
using System; using System.Runtime.InteropServices;
public class W32 { [StructLayout(LayoutKind.Sequential)] public struct RECT { public int L, T, R, B; }
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h); }
"@
$app = (Resolve-Path $App).Path
$p = Start-Process -FilePath "$app\dh2-save-editor.exe" -PassThru
$win = $null
for ($i = 0; $i -lt 40 -and -not $win; $i++) {
    Start-Sleep -Milliseconds 500
    $win = Get-Process msedge -ErrorAction SilentlyContinue | Where-Object { $_.MainWindowHandle -ne 0 -and $_.MainWindowTitle -like "*DH2 Save Editor*" } | Select-Object -First 1
}
if (-not $win) { Write-Output "no editor window"; taskkill /PID $p.Id /T /F | Out-Null; exit 1 }
Start-Sleep 3
[W32]::SetForegroundWindow($win.MainWindowHandle) | Out-Null
Start-Sleep 1
$r = New-Object W32+RECT
[W32]::GetWindowRect($win.MainWindowHandle, [ref]$r) | Out-Null
$bmp = New-Object System.Drawing.Bitmap ($r.R - $r.L), ($r.B - $r.T)
$g = [System.Drawing.Graphics]::FromImage($bmp)
$g.CopyFromScreen($r.L, $r.T, 0, 0, $bmp.Size)
$bmp.Save($Out, [System.Drawing.Imaging.ImageFormat]::Png)
Write-Output ("window '" + $win.MainWindowTitle + "' " + ($r.R - $r.L) + "x" + ($r.B - $r.T) + " -> " + $Out)
taskkill /PID $p.Id /T /F | Out-Null
