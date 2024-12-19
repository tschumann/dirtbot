#!/bin/bash

set -eu

cd $(dirname "${BASH_SOURCE[0]}")

wd=$(pwd)

mkdir -p build
pushd build

python3 ../configure.py -s hl2dm --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/
ambuild

cp "rcbot.2.hl2dm/rcbot.2.hl2dm.so" "../release/dirtbot/Half-Life 2 Deathmatch/hl2mp/addons/rcbot2/bin"
cp "loader/RCBot2Meta_i486/RCBot2Meta_i486.so" "../release/dirtbot/Half-Life 2 Deathmatch/hl2mp/addons/rcbot2/bin"

python3 ../configure.py -s episode1 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/
ambuild

cp "rcbot.2.ep1/rcbot.2.ep1.so" "../release/dirtbot/Source SDK Base/insurgency/addons/rcbot2/bin"
cp "loader/RCBot2Meta_i486/RCBot2Meta_i486.so" "../release/dirtbot/Source SDK Base/insurgency/addons/rcbot2/bin"

popd
