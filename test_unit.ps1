Set-StrictMode -Version 3.0

$ErrorActionPreference = "Stop"
$PSNativeCommandUserErrorActionPerference = $true

# get the location of this file
$scriptpath = $MyInvocation.MyCommand.Path
# get the directory path to this file
$wd = Split-Path $scriptpath
# set the working directory as this file's directory
Push-Location $wd

# TODO: get rid of hard-coded path
$steamappspath = "C:\Program Files (x86)\Steam\steamapps\"

# TODO: this depends on Steam being installed - quite hefty just to test for Windows and Linux - maybe copy the required engine files into the repo to make this stand-alone?

New-Item -ItemType Directory -Force -Path build/
Set-Location -Path build/

Write-Output "----------------------------------"
Write-Output "Building for Team Fortress 2 (x86)"
Write-Output "----------------------------------"

# build the project with tests
python ../configure.py -s tf2 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --enable-tests --target-arch x86
ambuild

Write-Output "---------------------------------"
Write-Output "Testing for Team Fortress 2 (x86)"
Write-Output "---------------------------------"

# add the engine's bin directory to the path because the tests depend on tier0.dll and vstdlib.dll (use dumpbin /IMPORTS)
$env:Path = "$steamappspath\common\Team Fortress 2\bin;" + $env:Path
tests/testrunner/testrunner.exe

Write-Output ""
Write-Output "=================================="
Write-Output ""

Write-Output "----------------------------------"
Write-Output "Building for Team Fortress 2 (x64)"
Write-Output "----------------------------------"

# build the project with tests
python ../configure.py -s tf2 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --enable-tests --target-arch x64
ambuild

Write-Output "---------------------------------"
Write-Output "Testing for Team Fortress 2 (x64)"
Write-Output "---------------------------------"

# add the engine's bin directory to the path because the tests depend on tier0.dll and vstdlib.dll (use dumpbin /IMPORTS)
$env:Path = "$steamappspath\common\Team Fortress 2\bin\x64;" + $env:Path
tests/testrunner/testrunner.exe

Write-Output ""
Write-Output "=================================="
Write-Output ""

Write-Output "------------------------------------------"
Write-Output "Building for Half-Life 2: Deathmatch (x86)"
Write-Output "------------------------------------------"

# build the project with tests
python ../configure.py -s hl2dm --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --enable-tests --target-arch x86
ambuild

Write-Output "-----------------------------------------"
Write-Output "Testing for Half-Life 2: Deathmatch (x86)"
Write-Output "-----------------------------------------"

# add the engine's bin directory to the path because the tests depend on tier0.dll and vstdlib.dll (use dumpbin /IMPORTS)
$env:Path = "$steamappspath\common\Half-Life 2 Deathmatch\bin;" + $env:Path
tests/testrunner/testrunner.exe

Write-Output ""
Write-Output "=================================="
Write-Output ""

Write-Output "------------------------------------------"
Write-Output "Building for Half-Life 2: Deathmatch (x64)"
Write-Output "------------------------------------------"

# build the project with tests
python ../configure.py -s hl2dm --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --enable-tests --target-arch x64
ambuild

Write-Output "-----------------------------------------"
Write-Output "Testing for Half-Life 2: Deathmatch (x64)"
Write-Output "-----------------------------------------"

# add the engine's bin directory to the path because the tests depend on tier0.dll and vstdlib.dll (use dumpbin /IMPORTS)
# TODO: using Team Fortress 2's engine for now
$env:Path = "$steamappspath\common\Team Fortress 2\bin\x64;" + $env:Path
tests/testrunner/testrunner.exe

Write-Output ""
Write-Output "=================================="
Write-Output ""

Write-Output "-------------------------------------------"
Write-Output "Building for Half-Life 2: Episode One (x86)"
Write-Output "-------------------------------------------"

# build the project with tests
python ../configure.py -s episode1 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --enable-tests --target-arch x86
ambuild

Write-Output "------------------------------------------"
Write-Output "Testing for Half-Life 2: Episode One (x86)"
Write-Output "------------------------------------------"

# add the engine's bin directory to the path because the tests depend on tier0.dll and vstdlib.dll (use dumpbin /IMPORTS)
$env:Path = "$steamappspath\common\Source SDK Base\bin;" + $env:Path
tests/testrunner/testrunner.exe

# delete this because building without tests fails if it's here
# TODO: why isn't tests/testrunner/testrunner.exe being deleted?
Remove-Item -Path tests -Recurse -Force
