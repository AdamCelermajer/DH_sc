# Offline, asset-prep only: converts original-media/intro.mp4 into the intro movie that the game
# decodes with pl_mpeg (features/startup/intro_movie_v2.hpp): one MPEG Program Stream with
# MPEG-1 video (1024x576, 24 fps) and MP2 audio (48 kHz stereo). The game never runs ffmpeg.
# Usage:
#   powershell -NoProfile -File tools/convert_intro_video.ps1 -Source <intro.mp4> -OutDir <dir> -Ffmpeg <ffmpeg.exe> [-Ffprobe <ffprobe.exe>]
# Output: <OutDir>/intro_v1.mpg. Prints size, sha256 and the frame/duration checks used by the verifier.
param(
    [Parameter(Mandatory=$true)][string]$Source,
    [Parameter(Mandatory=$true)][string]$OutDir,
    [Parameter(Mandatory=$true)][string]$Ffmpeg,
    [string]$Ffprobe,
    [int]$Width=1024,
    [int]$Height=576,
    [int]$Fps=24,
    [string]$VideoBitrate='3000k',
    [string]$AudioBitrate='192k'
)
$ErrorActionPreference='Stop'
if(-not (Test-Path -LiteralPath $Source)) { throw "Source not found: $Source" }
if(-not (Test-Path -LiteralPath $Ffmpeg)) { throw "ffmpeg not found: $Ffmpeg" }
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
$srcFull=(Resolve-Path -LiteralPath $Source).Path
$outFile=Join-Path (Resolve-Path -LiteralPath $OutDir).Path 'intro_v1.mpg'
# mpeg1video: no B-frames (bf 0) and a GOP of one second keep the stream simple for the decoder.
& $Ffmpeg -v error -y -i $srcFull -map 0:v:0 -map 0:a:0 `
    -c:v mpeg1video -b:v $VideoBitrate -maxrate 3500k -bufsize 1835k -s "${Width}x${Height}" -r $Fps -g $Fps -bf 0 `
    -c:a mp2 -b:a $AudioBitrate -ar 48000 -ac 2 -f mpeg $outFile
if($LASTEXITCODE -ne 0) { throw "ffmpeg conversion failed ($LASTEXITCODE)" }
# B054: the segment table travels with the movie (read by the boot runner from <movie>.segments.txt).
Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'intro_v1.segments.txt') -Destination (Join-Path (Split-Path $outFile) 'intro_v1.segments.txt') -Force
$sha=(Get-FileHash -Algorithm SHA256 -LiteralPath $outFile).Hash
"file=$outFile bytes=$((Get-Item $outFile).Length) sha256=$sha"
"video=mpeg1video ${Width}x${Height} fps=$Fps bitrate=$VideoBitrate audio=mp2 48000Hz stereo bitrate=$AudioBitrate"
if($Ffprobe -and (Test-Path -LiteralPath $Ffprobe)) {
    & $Ffprobe -v error -count_frames -select_streams v:0 -show_entries stream=nb_read_frames,codec_name -of default=nw=1 $outFile
}
