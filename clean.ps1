Write-Host "cleaning up puddle builds..." -Fore Yellow

if(Test-Path "./bin")
{
    rm "./bin/" -Recurse -Force
}

foreach($file in (gci *.o))
{
    if(Test-Path $file)
    {
        Remove-Item $file
    }
}

Write-Host "cleaned puddle!`n" -Fore Green
