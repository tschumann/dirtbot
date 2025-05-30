Set-StrictMode -Version 3.0

$ErrorActionPreference = "Stop"
$PSNativeCommandUserErrorActionPerference = $true

# get the location of this file
$scriptpath = $MyInvocation.MyCommand.Path
# get the directory path to this file
$wd = Split-Path $scriptpath
# set the working directory as this file's directory
Push-Location $wd

New-Item -ItemType Directory -Force -Path build/
Set-Location -Path build/

Write-Output "----------------------------------"
Write-Output "Building for Team Fortress 2 (x86)"
Write-Output "----------------------------------"

python ../configure.py -s tf2 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --target-arch x86
ambuild

Write-Output "----------------------------------"
Write-Output "Building for Team Fortress 2 (x64)"
Write-Output "----------------------------------"

python ../configure.py -s tf2 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --target-arch x64
ambuild

Copy-Item -Path "rcbot.2.tf2.x64\rcbot.2.tf2.dll" -Destination "..\release\dirtbot\Team Fortress 2\tf\addons\rcbot2\bin\x64"
# TODO: an extra .x64 is being added somewhere
Copy-item -Path "loader\RCBot2Meta.x64.x64\RCBot2Meta.x64.dll" -Destination "..\release\dirtbot\Team Fortress 2\tf\addons\rcbot2\bin"
Copy-item -Path "..\package\config\" -Destination "..\release\dirtbot\Team Fortress 2\tf\addons\rcbot2\" -Recurse -Force

Write-Output "------------------------------------------"
Write-Output "Building for Half-Life 2: Deathmatch (x86)"
Write-Output "------------------------------------------"

python ../configure.py -s hl2dm --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --target-arch x86
ambuild

Write-Output "------------------------------------------"
Write-Output "Building for Half-Life 2: Deathmatch (x64)"
Write-Output "------------------------------------------"

python ../configure.py -s hl2dm --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --target-arch x64
ambuild

Copy-Item -Path "rcbot.2.hl2dm.x64\rcbot.2.hl2dm.dll" -Destination "..\release\dirtbot\Half-Life 2 Deathmatch\hl2mp\addons\rcbot2\bin\x64"
# TODO: an extra .x64 is being added somewhere
Copy-item -Path "loader\RCBot2Meta.x64.x64\RCBot2Meta.x64.dll" -Destination "..\release\dirtbot\Half-Life 2 Deathmatch\hl2mp\addons\rcbot2\bin"
Copy-item -Path "..\package\config\" -Destination "..\release\dirtbot\Half-Life 2 Deathmatch\hl2mp\addons\rcbot2\" -Recurse -Force

Write-Output "-----------------------------"
Write-Output "Building for Black Mesa (x86)"
Write-Output "-----------------------------"

python ../configure.py -s bms --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --target-arch x86
ambuild

Copy-Item -Path "rcbot.2.bms\rcbot.2.bms.dll" -Destination "..\release\dirtbot\Black Mesa\bms\addons\rcbot2\bin"
Copy-item -Path "loader\RCBot2Meta\RCBot2Meta.dll" -Destination "..\release\dirtbot\Black Mesa\bms\addons\rcbot2\bin"
Copy-item -Path "..\package\config\" -Destination "..\release\dirtbot\Black Mesa\bms\addons\rcbot2\" -Recurse -Force

Write-Output "-------------------------------------------"
Write-Output "Building for Half-Life 2: Episode Two (x86)"
Write-Output "-------------------------------------------"

python ../configure.py -s ep2 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --target-arch x86
ambuild

Copy-Item -Path "rcbot.2.ep2\rcbot.2.ep2.dll" -Destination "..\release\dirtbot\Source SDK Base 2007\ageofchivalry\addons\rcbot2\bin"
Copy-item -Path "loader\RCBot2Meta\RCBot2Meta.dll" -Destination "..\release\dirtbot\Source SDK Base 2007\ageofchivalry\addons\rcbot2\bin"
Copy-item -Path "..\package\config\" -Destination "..\release\dirtbot\Source SDK Base 2007\ageofchivalry\addons\rcbot2\" -Recurse -Force

Write-Output "-------------------------------------------"
Write-Output "Building for Half-Life 2: Episode One (x86)"
Write-Output "-------------------------------------------"

python ../configure.py -s episode1 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --target-arch x86
ambuild

Copy-Item -Path "rcbot.2.ep1\rcbot.2.ep1.dll" -Destination "..\release\dirtbot\Source SDK Base\insurgency\addons\rcbot2\bin"
Copy-item -Path "loader\RCBot2Meta\RCBot2Meta.dll" -Destination "..\release\dirtbot\Source SDK Base\insurgency\addons\rcbot2\bin"
Copy-item -Path "..\package\config\" -Destination "..\release\dirtbot\Source SDK Base\insurgency\addons\rcbot2\" -Recurse -Force
