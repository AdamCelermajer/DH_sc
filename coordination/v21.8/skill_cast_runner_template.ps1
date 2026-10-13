# skill_cast_runner_template.ps1 (v21.8 skill verification, TEMPLATE)
#
# Casts each active skill N times in hidden-window real runs and captures frames and the
# log lines that prove: (1) the cast was accepted, (2) an FX set was dispatched, (3) a sound
# was submitted. It writes one JSON row per cast plus a summary CSV under -OutRoot.
#
# Preconditions (NOT automated here, see SKILL-VERIFICATION-MATRIX.md, Step 3):
#   * The case save must have the skill on hotbar key -Keys[skill] (1..5). Seeds in
#     C:\dh217\seeds only cover the four whitelisted skills, so other skills are reported
#     BLOCKED without launching.
#   * A skill is launched only if its matrix row says "CAST in this branch" (cast whitelist).
#     Skills without a cast path cannot be verified by any launch, flag or level change.
#
# Usage:
#   powershell -NoProfile -ExecutionPolicy Bypass -File skill_cast_runner_template.ps1 -DryRun
#   powershell -NoProfile -ExecutionPolicy Bypass -File skill_cast_runner_template.ps1 `
#       -Exe C:\dh218\skillcast-exe\run.exe -Only BashDown,GroundSlam,Charge,JumpKick -Reps 3
#
# Log patterns (from port/windows-foundation/main.cpp):
#   accepted : "Source skill key=N slot=S frame=F ... phase=P skill=NAME"   accepted when P != 0
#              (RuntimeSkillCastPhaseV1: 0 rejected, 1 prepared_pending_use, 2 use_applied,
#               3 completed, 4 interrupted, 5 partial_failure)
#   FX       : "Source player step FX frame=F ... dispatched=B"           dispatched when B is 1
#   sound    : "Container sound ordinal=O uid=U status=S" or "Skill container sound: ..."
#              (a dedicated per-skill sound line does not exist yet; this is the closest one)
#   lifecycle: "Source skill lifecycle frame=F generation=G phase=P skill=NAME"

[CmdletBinding()]
param(
    [string]$Exe = '',
    [string]$Manifest = '',          # default: SKILL-VERIFICATION-MATRIX.json next to this script
    [string]$Package = 'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\windows-source-clock-v21.8-staged',
    [string]$Harness = 'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\v21.4\verify\tools\quiet_click_sync.ps1',
    [string]$OutRoot = 'C:\dh218\skillcast',
    [string[]]$Only = @(),
    [int]$Reps = 3,
    [int]$FirstCastFrame = 600,
    [int]$CastSpacingFrames = 300,   # >= the skill cooldown; cooldown values are not in the matrix yet
    [int]$Frames = 1500,
    [int]$TimeoutSec = 400,
    [hashtable]$Keys = @{ BashDown = 1; GroundSlam = 2; Charge = 3; JumpKick = 4 },
    [string[]]$GrantArgs = @(),      # reserved for a future test-only grant flag; empty today
    [switch]$DryRun
)

$ErrorActionPreference = 'Stop'

if (-not $Manifest) {
    $here = Split-Path -Parent $MyInvocation.MyCommand.Path
    $Manifest = Join-Path $here 'SKILL-VERIFICATION-MATRIX.json'
}
$matrix = Get-Content -LiteralPath $Manifest -Raw | ConvertFrom-Json
$skills = @($matrix.skills | Where-Object { $_.type -eq 'active' })
# -File passes "A,B" as one string; split it so both "-Only A,B" and "-Only A -Only B" work.
$Only = @($Only | ForEach-Object { $_ -split ',' } | Where-Object { $_ })
if ($Only.Count -gt 0) { $skills = @($skills | Where-Object { $Only -contains $_.skill }) }

$startupTemplate = Get-Content -LiteralPath (Join-Path $Package 'startup.args')
$runStamp = Get-Date -Format 'yyyyMMdd-HHmmss'
$runRoot = Join-Path $OutRoot $runStamp
$rows = New-Object System.Collections.Generic.List[object]

function Build-RunArgs([string]$caseRoot, [string[]]$extra) {
    $out = New-Object System.Collections.Generic.List[string]
    for ($i = 0; $i -lt $startupTemplate.Count; $i++) {
        $line = $startupTemplate[$i].TrimEnd("`r")
        $prev = if ($i -gt 0) { $startupTemplate[$i - 1].TrimEnd("`r") } else { '' }
        if ($prev -in '--assets', '--menu-assets') { $line = Join-Path $Package 'assets' }
        elseif ($prev -eq '--audio-assets') { $line = Join-Path $Package 'audio-assets' }
        elseif ($prev -eq '--audio-table') { $line = Join-Path $Package 'audio-assets\data\sounds\sounds_pyarray.bin' }
        elseif ($prev -eq '--save') { $line = Join-Path $caseRoot 'character.save' }
        elseif ($prev -eq '--game-save') { $line = Join-Path $caseRoot 'gameplay.save' }
        $out.Add($line)
    }
    $out.Add('--skip-boot')
    $out.Add('--frames'); $out.Add([string]$Frames)
    foreach ($e in $extra) { $out.Add($e.Replace('{CASE}', $caseRoot)) }
    return $out
}

foreach ($skill in $skills) {
    $name = $skill.skill
    $blocked = -not ($skill.port_status -like 'CAST in this branch*')
    $key = if ($Keys.ContainsKey($name)) { [int]$Keys[$name] } else { 0 }
    for ($rep = 1; $rep -le $Reps; $rep++) {
        $caseRoot = Join-Path $runRoot ("{0}\rep{1}" -f $name, $rep)
        $row = [ordered]@{ skill = $name; skill_id = $skill.skill_id; rep = $rep; key = $key; status = '';
                           accepted = @(); fx = @(); sound = @(); captures = @(); detail = '' }
        if ($blocked) {
            $row.status = 'BLOCKED: no cast path in this branch (matrix port_status)'
            $rows.Add([pscustomobject]$row); continue
        }
        if ($key -lt 1 -or $key -gt 5) {
            $row.status = 'BLOCKED: no hotbar key configured (-Keys) for this skill'
            $rows.Add([pscustomobject]$row); continue
        }
        if ($DryRun) {
            $row.status = 'DRYRUN: would launch'
            $rows.Add([pscustomobject]$row); continue
        }
        if (-not (Test-Path -LiteralPath $Exe)) { throw "Exe not found: $Exe" }
        New-Item -ItemType Directory -Path $caseRoot -Force | Out-Null
        Copy-Item -LiteralPath $Exe -Destination (Join-Path $caseRoot 'run.exe') -Force

        # One hotbar press per spacing; each press is a cast attempt (rep numbering inside a run).
        $extra = @()
        $captures = @()
        for ($c = 0; $c -lt 3; $c++) {
            $frame = $FirstCastFrame + $c * $CastSpacingFrames
            $extra += '--skill-key-frame'; $extra += ("{0}:{1}" -f $frame, $key)
            $captures += $frame
        }
        foreach ($g in $GrantArgs) { $extra += $g }
        foreach ($f in $captures) { $extra += '--capture-at'; $extra += ("{0}:{{CASE}}\f{0}.ppm" -f $f) }
        $runArgs = Build-RunArgs $caseRoot $extra
        $runArgs | Set-Content -LiteralPath (Join-Path $caseRoot 'run.args')

        $log = Join-Path $caseRoot 'run.log'
        & $Harness -Exe (Join-Path $caseRoot 'run.exe') -ExeArgs @('--startup-config', (Join-Path $caseRoot 'run.args')) `
            -Cwd $caseRoot -Log $log -Clicks '' -TimeoutSec $TimeoutSec | Out-Null

        $text = if (Test-Path -LiteralPath $log) { Get-Content -LiteralPath $log -Raw } else { '' }
        $row.accepted = @([regex]::Matches($text, 'Source skill key=\d+ slot=\d+ frame=\d+[^\r\n]*?phase=(\d+) skill=\S+') |
            Where-Object { $_.Groups[1].Value -ne '0' } | ForEach-Object { $_.Value })
        $row.fx = @([regex]::Matches($text, 'Source player step FX frame=\d+[^\r\n]*?dispatched=1[^\r\n]*') | ForEach-Object { $_.Value })
        $row.sound = @([regex]::Matches($text, '(Container sound ordinal=\d+ uid=\d+ status=(submitted|asset_missing))|(Skill container sound: [^\r\n]*)') |
            ForEach-Object { $_.Value })
        $row.captures = @(Get-ChildItem -LiteralPath $caseRoot -Filter '*.ppm' -ErrorAction SilentlyContinue | ForEach-Object { $_.FullName })
        $row.status = if ($row.accepted.Count -gt 0) { 'ACCEPTED' } else { 'NOT ACCEPTED (see run.log)' }
        $rows.Add([pscustomobject]$row)
    }
}

New-Item -ItemType Directory -Path $runRoot -Force | Out-Null
$rows | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath (Join-Path $runRoot 'cast-runs.json') -Encoding utf8
$rows | Select-Object skill, skill_id, rep, key, status,
    @{ n = 'accepted_lines'; e = { $_.accepted.Count } },
    @{ n = 'fx_lines'; e = { $_.fx.Count } },
    @{ n = 'sound_lines'; e = { $_.sound.Count } },
    @{ n = 'frames'; e = { $_.captures.Count } } |
    Export-Csv -LiteralPath (Join-Path $runRoot 'summary.csv') -NoTypeInformation -Encoding utf8
Write-Output ("runs: {0} rows; output {1}" -f $rows.Count, $runRoot)
$rows | Group-Object status | ForEach-Object { Write-Output ("  {0} x {1}" -f $_.Count, $_.Name) }
