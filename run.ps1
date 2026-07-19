param($build)

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

if($null -eq $build)
{
    $build = "release"
}

if($build -eq "asan" -or $build -eq "debug" -or $build -eq "release")
{
    Write-Host "`ncompiling puddle...`n" -Fore Cyan

    premake5 vs2026
    &MSBuild ./build/puddle.slnx -p:Configuration=$build
}
else
{
    echo -e "`nERROR: invalid make config: '$build'." -Fore Red
    exit -2;
}

if($LASTEXITCODE -ne 0)
{
    Write-Host "`nERROR: failed to compile puddle.`n" -Fore Red
    exit -1
}

Write-Host ""

# if [[ $2 = "--compile-only" ]]
# then
#     exit 0
# fi

Write-Host "running string_view unit tests..." -Fore Cyan
 
&./bin/$build/svtest.exe
