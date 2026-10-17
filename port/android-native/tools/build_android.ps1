param(
    [string] $SourceRoot,
    [string] $JavaHome,
    [string] $AndroidHome,
    [string] $ShortTempRoot,
    [switch] $HelpOnly,
    [switch] $Online
)

$ErrorActionPreference = 'Stop'

if (-not $SourceRoot) {
    $SourceRoot = (Resolve-Path -LiteralPath (Join-Path $PSScriptRoot '..\..\..')).Path
}
$SourceRoot = (Resolve-Path -LiteralPath $SourceRoot).Path

if (-not $JavaHome) {
    $JavaHome = $env:JAVA_HOME
}
if (-not $JavaHome) {
    throw 'Set JAVA_HOME or pass -JavaHome to a JDK installation.'
}
$JavaHome = (Resolve-Path -LiteralPath $JavaHome).Path
if (-not (Test-Path -LiteralPath (Join-Path $JavaHome 'bin\java.exe'))) {
    throw "No java.exe found under Java home: $JavaHome"
}

if (-not $AndroidHome) {
    if ($env:ANDROID_HOME) {
        $AndroidHome = $env:ANDROID_HOME
    } elseif ($env:ANDROID_SDK_ROOT) {
        $AndroidHome = $env:ANDROID_SDK_ROOT
    }
}
if (-not $AndroidHome) {
    throw 'Set ANDROID_HOME or ANDROID_SDK_ROOT, or pass -AndroidHome to the Android SDK.'
}
$AndroidHome = (Resolve-Path -LiteralPath $AndroidHome).Path

if (-not $ShortTempRoot) {
    $ShortTempRoot = Join-Path $env:SystemDrive 'Temp'
}
if (-not (Test-Path -LiteralPath $ShortTempRoot)) {
    New-Item -ItemType Directory -Path $ShortTempRoot -Force | Out-Null
}
$ShortTempRoot = (Resolve-Path -LiteralPath $ShortTempRoot).Path
if ($ShortTempRoot -match '\s' -or $ShortTempRoot.Length -gt 60) {
    throw "ShortTempRoot must be a short path without spaces for Java's Windows local socket: $ShortTempRoot"
}

$AndroidProject = Join-Path $SourceRoot 'port\android-native'
$GradleWrapper = Join-Path $AndroidProject 'gradlew.bat'
if (-not (Test-Path -LiteralPath $GradleWrapper -PathType Leaf)) {
    throw "Gradle wrapper not found: $GradleWrapper"
}

$oldJavaHome = $env:JAVA_HOME
$oldAndroidHome = $env:ANDROID_HOME
$oldJavaToolOptions = $env:JAVA_TOOL_OPTIONS
$oldLocation = Get-Location
$gradleExitCode = 1

try {
    $env:JAVA_HOME = $JavaHome
    $env:ANDROID_HOME = $AndroidHome

    $pipeOptions = "-Djdk.net.unixdomain.tmpdir=$ShortTempRoot -Djava.io.tmpdir=$ShortTempRoot"
    if ($oldJavaToolOptions) {
        $env:JAVA_TOOL_OPTIONS = "$oldJavaToolOptions $pipeOptions"
    } else {
        $env:JAVA_TOOL_OPTIONS = $pipeOptions
    }

    Push-Location -LiteralPath $AndroidProject
    try {
        $gradleArgs = @('--no-daemon', '--max-workers=1')
        if (-not $Online) {
            $gradleArgs += '--offline'
        }
        if ($HelpOnly) {
            $gradleArgs += 'help'
        } else {
            $gradleArgs += ':app:assembleDebug'
        }
        $gradleArgs += "-Pdh2SourceRoot=$SourceRoot"
        $gradleArgs += '--console=plain'

        & $GradleWrapper @gradleArgs
        $gradleExitCode = $LASTEXITCODE
    } finally {
        Pop-Location
    }
} finally {
    $env:JAVA_HOME = $oldJavaHome
    $env:ANDROID_HOME = $oldAndroidHome
    $env:JAVA_TOOL_OPTIONS = $oldJavaToolOptions
    Set-Location -LiteralPath $oldLocation
}

$global:LASTEXITCODE = $gradleExitCode
if ($gradleExitCode -ne 0) {
    throw "Gradle exited with code $gradleExitCode."
}
