Write-Host "cleaning up puddle builds..." -Fore Yellow

if(Test-Path "./build")
{
    rm "./build/" -Recurse -Force
}

if(Test-Path "./bin")
{
    rm "./bin/" -Recurse -Force
}

if(Test-Path "./obj")
{
    rm "./obj/" -Recurse -Force
}

Write-Host "cleaned puddle!`n" -Fore Green
