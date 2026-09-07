param
(
    [Parameter(Position = 0)][string]$build,
    [Parameter(Position = 1)][string]$compile_only
)

if(-Not(Test-Path "./bin/" -PathType Container))
{
    mkdir "./bin/"
}

if($build -eq $null -or $build -eq "")
{
    $build = "release"
}

$args_always=@("-DBUILD_WINDOWS",
"src/win32_pd_path.c", "src/puddle_test.c", "src/string_view.c",
"-Iinclude",
"-Wextra", "-Wall", "-Wpedantic", "-Wconversion", "-Wshadow", "-Wsign-compare",
"-Wtype-limits", "-Wunused")

$args_release=@("-O2")

$args_debug=@("-DDEBUG", "-gcodeview", "-O0")
$args_debug_cl=@("/DEFINE:DEBUG", "/Zi", "/O0")

$args_asan=$args_debug_cl+@("/DEFINE:ASAN", "/fsanitize=address",
"/link", "/SUBSYSTEM:CONSOLE", "clang_rt.asan_dynamic-x86_64.lib",
"clang_rt.asan_dynamic_runtime_thunk-x86_64.lib")

function compile
{
    param( [string[]]$1 )

    Write-Host "identifying a compiler..."

    if(Get-Command clang -ErrorAction SilentlyContinue)
    {
        Write-Host "found clang."
        $script:compiler="clang"
        if($build -eq "asan")
        {
            $script:compiler="clang-cl"
        }
    }
    elseif(Get-Command gcc -ErrorAction SilentlyContinue)
    {
        Write-Host "found gcc."
        $script:compiler="gcc"
    }
    else
    {
        Write-Host "ERROR: no suitable compiler found." -Fore Red
    }

    Write-Host ""
    Write-Host "compiling puddle..." -Fore Cyan
    Write-Host ""

    if(-Not (Test-Path "./bin/$build/" -PathType Container))
    {
        mkdir "./bin/$build/"
    }

    Write-Host "compiling $build build with the following command:"
    Write-Host "$script:compiler $1"
    &$script:compiler @1
    if($LASTEXITCODE -ne 0)
    {
        Write-Host "`nERROR: $script:compiler failed to compile puddle.`n" -Fore Red
        exit -1
    }
    mv ./a.exe ./bin/$build/puddletest.exe
}

if($build -eq "release")
{
    compile ($args_always + $args_release)
}
elseif($build -eq "debug")
{
    compile ($args_always + $args_debug)
}
elseif($build -eq "asan")
{
    compile ($args_always + $args_asan)
}
else
{
    Write-Host "`nERROR: invalid make config: $build." -Fore Red
    exit 3;
}

Write-Host "`n"

if($compile_only -eq "--compile-only")
{
    exit 0
}

&./bin/$build/puddletest
