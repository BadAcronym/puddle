param
(
    [Parameter(Position = 0)][string]$build,
    [Parameter(Position = 1)][string]$compile_only
)

if(-Not(Test-Path "./obj/"))
{
    mkdir "./obj/"
}

if(-Not(Test-Path "./bin/"))
{
    mkdir "./bin/"
}

if(-Not(Test-Path "./build/"))
{
    mkdir "./build/"
}

if($null -eq $build -or $build -eq "")
{
    $build = "release"
}

if($build -eq "asan" -or $build -eq "debug" -or $build -eq "release")
{
    Write-Host "`ncompiling puddle...`n" -Fore Cyan

    premake5 gmake
    pushd "./build/"
    make config=$build`_windows
    popd
}
else
{
    Write-Host "`nERROR: invalid make config: '$build'." -Fore Red
    exit -2;
}

if($LASTEXITCODE -ne 0)
{
    Write-Host "`nERROR: failed to compile puddle.`n" -Fore Red
    exit -1
}

Write-Host ""

if($compile_only -eq "--compile-only")
{
    exit 0;
}

Write-Host "running string_view unit tests..." -Fore Cyan

&./bin/$build/svtest.exe
