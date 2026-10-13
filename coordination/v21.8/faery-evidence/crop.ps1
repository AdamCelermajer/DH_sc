param([string]$Src,[string]$Dst,[int]$X,[int]$Y,[int]$W,[int]$H,[int]$S=3)
Add-Type -AssemblyName System.Drawing
$b=[System.Drawing.Bitmap]::FromFile($Src)
$r=New-Object System.Drawing.Rectangle($X,$Y,$W,$H)
$c=$b.Clone($r,$b.PixelFormat)
$o=New-Object System.Drawing.Bitmap(($W*$S),($H*$S))
$g=[System.Drawing.Graphics]::FromImage($o)
$g.InterpolationMode=[System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
$g.DrawImage($c,0,0,($W*$S),($H*$S))
$o.Save($Dst,[System.Drawing.Imaging.ImageFormat]::Png)
$g.Dispose();$o.Dispose();$c.Dispose();$b.Dispose()
