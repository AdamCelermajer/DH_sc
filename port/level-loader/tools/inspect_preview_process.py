"""Read-only host process/event evidence for the loader emulator exit."""
import csv,io,json,pathlib,subprocess
root=pathlib.Path(__file__).resolve().parents[3];out=root/'port/level-loader/reports'
process=subprocess.run(['tasklist.exe','/fo','csv','/nh'],capture_output=True,text=True,timeout=10)
assert process.returncode==0,process.stderr
rows=[row for row in csv.reader(io.StringIO(process.stdout)) if row and ('qemu' in row[0].lower() or 'emulator' in row[0].lower())]
script="Get-WinEvent -FilterHashtable @{LogName='Application';StartTime=(Get-Date).AddMinutes(-45);Id=1000,1001} -MaxEvents 40 -ErrorAction SilentlyContinue | Where-Object { $_.Message -match 'qemu|emulator' } | Select-Object TimeCreated,Id,Message | ConvertTo-Json -Depth 3"
try:
    events=subprocess.run(['powershell.exe','-NoProfile','-NonInteractive','-Command',script],capture_output=True,text=True,timeout=20)
    event_result={'returncode':events.returncode,'stdout':events.stdout,'stderr':events.stderr}
except subprocess.TimeoutExpired:event_result={'observation_timeout':True}
report={'scope':__doc__,'processes':rows,'events':event_result}
(out/'preview-process-exit-evidence.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
