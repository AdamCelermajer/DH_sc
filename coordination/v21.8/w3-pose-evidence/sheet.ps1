param([string]$Dir,[string]$Frames,[string]$Out,[int]$X,[int]$Y,[int]$W,[int]$H,[int]$Cols=4,[double]$Scale=1.0)
Add-Type -AssemblyName System.Drawing
$fr=$Frames.Split(',')
$cw=[int]($W*$Scale);$ch=[int]($H*$Scale)
$rows=[math]::Ceiling($fr.Count/$Cols)
$sheet=New-Object System.Drawing.Bitmap ($Cols*$cw),($rows*$ch)
$g=[System.Drawing.Graphics]::FromImage($sheet)
$g.InterpolationMode='HighQualityBicubic'
$font=New-Object System.Drawing.Font 'Arial',14
for($i=0;$i -lt $fr.Count;$i++){
 $img=[System.Drawing.Image]::FromFile("$Dir\f$($fr[$i]).png")
 $src=New-Object System.Drawing.Rectangle $X,$Y,$W,$H
 $dst=New-Object System.Drawing.Rectangle (($i%$Cols)*$cw),([math]::Floor($i/$Cols)*$ch),$cw,$ch
 $g.DrawImage($img,$dst,$src,'Pixel')
 $g.DrawString($fr[$i],$font,[System.Drawing.Brushes]::Yellow,$dst.X+4,$dst.Y+4)
 $img.Dispose()
}
$g.Dispose();$sheet.Save($Out)
