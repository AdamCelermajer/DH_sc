# Offline, asset-prep only: converts original-media/intro.mp4 into the portable
# intro picture stream (features/startup/intro_stream_v1.hpp) and a WAV soundtrack.
# The game never runs ffmpeg. Usage:
#   powershell -NoProfile -File tools/convert_intro_video.ps1 -Source <intro.mp4> -OutDir <dir> -Ffmpeg <ffmpeg.exe>
# Outputs in OutDir: intro_v1.dhintro (picture stream) and intro_v1.wav (48 kHz, 16-bit, stereo).
param(
    [Parameter(Mandatory=$true)][string]$Source,
    [Parameter(Mandatory=$true)][string]$OutDir,
    [Parameter(Mandatory=$true)][string]$Ffmpeg,
    [int]$Width=640,
    [int]$Height=360,
    [int]$Fps=24
)
$ErrorActionPreference='Stop'
if(-not (Test-Path -LiteralPath $Source)) { throw "Source not found: $Source" }
if(-not (Test-Path -LiteralPath $Ffmpeg)) { throw "ffmpeg not found: $Ffmpeg" }
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
Add-Type -AssemblyName System.IO.Compression
Add-Type -TypeDefinition @"
using System;
using System.IO;
using System.IO.Compression;
public static class IntroPack {
    // Reads raw rgb565le frames from ffmpeg stdout, XORs each frame against the
    // previous one, deflates it, and writes the container (layout in intro_stream_v1.hpp).
    public static int Pack(Stream src, int width, int height, int fps, string dataPath, string outPath) {
        int frameBytes = width * height * 2;
        var buf = new byte[frameBytes];
        var prev = new byte[frameBytes];
        var offsets = new System.Collections.Generic.List<uint>();
        var sizes = new System.Collections.Generic.List<uint>();
        uint offset = 0;
        using (var data = new FileStream(dataPath, FileMode.Create, FileAccess.Write))
        {
            while (true) {
                int got = 0;
                while (got < frameBytes) {
                    int n = src.Read(buf, got, frameBytes - got);
                    if (n <= 0) break;
                    got += n;
                }
                if (got == 0) break;
                if (got != frameBytes) throw new InvalidDataException("partial frame from ffmpeg: " + got);
                var delta = new byte[frameBytes];
                for (int i = 0; i < frameBytes; ++i) delta[i] = (byte)(buf[i] ^ prev[i]);
                byte[] packed;
                using (var ms = new MemoryStream()) {
                    using (var ds = new DeflateStream(ms, CompressionLevel.Optimal, true)) ds.Write(delta, 0, delta.Length);
                    packed = ms.ToArray();
                }
                offsets.Add(offset);
                sizes.Add((uint)packed.Length);
                data.Write(packed, 0, packed.Length);
                offset += (uint)packed.Length;
                Buffer.BlockCopy(buf, 0, prev, 0, frameBytes);
            }
        }
        int count = offsets.Count;
        if (count == 0) throw new InvalidDataException("no frames decoded");
        using (var outFile = new FileStream(outPath, FileMode.Create, FileAccess.Write))
        using (var w = new BinaryWriter(outFile)) {
            w.Write(new byte[] {(byte)'D',(byte)'H',(byte)'2',(byte)'I',(byte)'N',(byte)'T',(byte)'R',(byte)'1'});
            w.Write((uint)1); w.Write((uint)width); w.Write((uint)height); w.Write((uint)fps); w.Write((uint)count);
            for (int i = 0; i < count; ++i) { w.Write(offsets[i]); w.Write(sizes[i]); }
            w.Flush();
            using (var d = new FileStream(dataPath, FileMode.Open, FileAccess.Read)) d.CopyTo(outFile);
        }
        File.Delete(dataPath);
        return count;
    }
}
"@
$stream="$OutDir/intro_v1.dhintro"
$wav="$OutDir/intro_v1.wav"
$temp="$OutDir/intro_v1.data.tmp"
$srcFull=(Resolve-Path -LiteralPath $Source).Path
# Picture: raw rgb565le frames on stdout at the target size and rate.
$psi=New-Object System.Diagnostics.ProcessStartInfo
$psi.FileName=$Ffmpeg
$psi.Arguments="-v error -i `"$srcFull`" -an -vf `"fps=$Fps,scale=${Width}:${Height}:flags=bicubic`" -f rawvideo -pix_fmt rgb565le -"
$psi.RedirectStandardOutput=$true
$psi.RedirectStandardError=$true
$psi.UseShellExecute=$false
$proc=[System.Diagnostics.Process]::Start($psi)
$frames=[IntroPack]::Pack($proc.StandardOutput.BaseStream,$Width,$Height,$Fps,$temp,$stream)
$err=$proc.StandardError.ReadToEnd()
$proc.WaitForExit()
if($proc.ExitCode -ne 0) { throw "ffmpeg picture failed ($($proc.ExitCode)): $err" }
# Soundtrack: 48 kHz 16-bit stereo PCM WAV.
& $Ffmpeg -v error -y -i $srcFull -vn -acodec pcm_s16le -ar 48000 -ac 2 $wav
if($LASTEXITCODE -ne 0) { throw "ffmpeg audio failed" }
$sha=(Get-FileHash -Algorithm SHA256 -LiteralPath $stream).Hash
$shaWav=(Get-FileHash -Algorithm SHA256 -LiteralPath $wav).Hash
"frames=$frames fps=$Fps size=${Width}x${Height}"
"stream=$stream bytes=$((Get-Item $stream).Length) sha256=$sha"
"audio=$wav bytes=$((Get-Item $wav).Length) sha256=$shaWav"
