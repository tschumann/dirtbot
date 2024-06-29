# get the location of this file
$scriptpath = $MyInvocation.MyCommand.Path
# get the directory path to this file
$wd = Split-Path $scriptpath
# set the working directory as this file's directory
Push-Location $wd

New-Item -ItemType Directory -Force -Path build/
Set-Location -Path build/

# build the project with tests
# TODO: get rid of hard-coded paths
python ../configure.py -s hl2dm --mms-path C:/Users/schum/Documents/GitHub/dirtbot/alliedmodders/metamod-source/ --sm-path C:/Users/schum/Documents/GitHub/dirtbot/alliedmodders/sourcemod/ --hl2sdk-root C:/Users/schum/Documents/GitHub/dirtbot/alliedmodders/ --enable-tests
ambuild

# add the engine's bin directory to the path because the tests depend on tier0.dll and vstdlib.dll (use dumpbin /IMPORTS)
# TODO: get rid of hard-coded paths
$env:Path += ";C:\Program Files (x86)\Steam\steamapps\common\Half-Life 2 Deathmatch\bin"
tests/testrunner/testrunner.exe
