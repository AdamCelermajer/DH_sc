param([string]$Tag='b1',[string[]]$Names=@('Rocky','Wetty','Windy','Celest','Hotty'),[string]$Exe='C:\dh218\faery-v5.exe',
  [string]$Spawn='Swamp_LizadMan_Type1@450,557,100@100;Swamp_LizadMan_Type1@330,500,100@100',
  [int]$Frames=700,[string]$Caps='370,385,400,430',[string]$Key='16:-52:0:80',[string]$More='')
foreach($n in $Names){
  $ex=@('--test-faery',$n)
  foreach($s in $Spawn.Split(';')){if($s){$ex+=@('--spawn-test',$s)}}
  foreach($m in $More.Split(';')){if($m){$ex+=$m.Split('|')}}
  & C:\dh218\tools\faery\run-faery.ps1 -Case "$Tag-$n" -Exe $Exe -Frames $Frames -Clicks ("3:1135:226:80,6:1130:259:80,"+$Key) -Extra $ex -Captures ($Caps.Split(',')|%{[int]$_}) -Timeout 300 | Out-Null
}
