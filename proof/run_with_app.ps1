# Starts dh2-save-editor.exe (server only, fixed port) and a headless Edge on it, runs one proof script, then stops
# exactly the two processes it started.
# usage: powershell -File proof\run_with_app.ps1 -App <folder with dh2-save-editor.exe> -Script windows_flow.js -- <script args>
param([string]$App, [string]$Script, [Parameter(ValueFromRemainingArguments = $true)][string[]]$Rest)
$repo = Split-Path -Parent $PSScriptRoot
Remove-Item "$App\last-url.txt" -ErrorAction SilentlyContinue
$srv = Start-Process -FilePath "$App\dh2-save-editor.exe" -ArgumentList "--no-browser --print-url --port 18765" -PassThru
Start-Sleep 2
$url = Get-Content "$App\last-url.txt"
$edge = Start-Process -FilePath "C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe" -ArgumentList "--headless=new", "--remote-debugging-port=9333", "--user-data-dir=$repo\build\edge-proof-profile", "--no-first-run", $url -PassThru
Start-Sleep 3
& node "$PSScriptRoot\$Script" 9333 @Rest
$rc = $LASTEXITCODE
taskkill /PID $edge.Id /T /F | Out-Null
taskkill /PID $srv.Id /T /F | Out-Null
Remove-Item "$App\last-url.txt" -ErrorAction SilentlyContinue
exit $rc
