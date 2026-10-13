# Dot-source this file to enable `atlas find ...` and `atlas check` in this shell.
function atlas {
    & (Join-Path $PSScriptRoot 'atlas.ps1') @args
}
