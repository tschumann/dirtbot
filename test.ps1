# get the location of this file
$scriptpath = $MyInvocation.MyCommand.Path
# get the directory path to this file
$wd = Split-Path $scriptpath
# set the working directory as this file's directory
Push-Location $wd

New-Item -ItemType Directory -Force -Path build/
Set-Location -Path build/

Write-Output "----------------------------"
Write-Output "Building for Team Fortress 2"
Write-Output "----------------------------"

# build the project with tests
python ../configure.py -s tf2 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --enable-tests --target-arch x64
ambuild

# TODO: run the tests

Write-Output "------------------------------------"
Write-Output "Building for Half-Life 2: Deathmatch"
Write-Output "------------------------------------"

# build the project with tests
python ../configure.py -s hl2dm --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --enable-tests --target-arch x86
ambuild

Write-Output "-----------------------------------"
Write-Output "Testing for Half-Life 2: Deathmatch"
Write-Output "-----------------------------------"

# add the engine's bin directory to the path because the tests depend on tier0.dll and vstdlib.dll (use dumpbin /IMPORTS)
# TODO: get rid of hard-coded paths
$env:Path = "C:\Program Files (x86)\Steam\steamapps\common\Half-Life 2 Deathmatch\bin;" + $env:Path
tests/testrunner/testrunner.exe

# TODO: why isn't tests/testrunner/testrunner.exe being deleted?
Remove-Item -Path tests -Recurse -Force

Write-Output "-------------------------------------"
Write-Output "Building for Half-Life 2: Episode One"
Write-Output "-------------------------------------"

# build the project with tests
python ../configure.py -s episode1 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --enable-tests --target-arch x86
ambuild

Write-Output "------------------------------------"
Write-Output "Testing for Half-Life 2: Episode One"
Write-Output "------------------------------------"

# add the engine's bin directory to the path because the tests depend on tier0.dll and vstdlib.dll (use dumpbin /IMPORTS)
# TODO: get rid of hard-coded paths
$env:Path = "C:\Program Files (x86)\Steam\steamapps\common\Source SDK Base\bin;" + $env:Path
tests/testrunner/testrunner.exe

# TODO: why isn't tests/testrunner/testrunner.exe being deleted?
Remove-Item -Path tests -Recurse -Force
