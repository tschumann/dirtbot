#!/bin/bash

set -eu

cd $(dirname "${BASH_SOURCE[0]}")

wd=$(pwd)

mkdir -p build
pushd build

echo "----------------------------------"
echo "Building for Team Fortress 2 (x86)"
echo "----------------------------------"

python3 ../configure.py -s tf2 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --enable-tests --target-arch x86
ambuild

# TODO: run the tests

echo "----------------------------------"
echo "Building for Team Fortress 2 (x64)"
echo "----------------------------------"

python3 ../configure.py -s tf2 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --enable-tests --target-arch x64
ambuild

# TODO: run the tests

echo "------------------------------------------"
echo "Building for Half-Life 2: Deathmatch (x86)"
echo "------------------------------------------"

python3 ../configure.py -s hl2dm --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --enable-tests --target-arch x86
ambuild

# TODO: run the tests

echo "------------------------------------------"
echo "Building for Half-Life 2: Deathmatch (x64)"
echo "------------------------------------------"

python3 ../configure.py -s hl2dm --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --enable-tests--target-arch x64
ambuild

# TODO: enable and run the tests

echo "----------------------------------"
echo "Building for Source SDK 2013 (x86)"
echo "----------------------------------"

python3 ../configure.py -s sdk213 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --target-arch x86
ambuild

# TODO: enable and run the tests

echo "-------------------------------------------"
echo "Building for Half-Life 2: Episode Two (x86)"
echo "-------------------------------------------"

python3 ../configure.py -s ep2 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --target-arch x86
ambuild

# TODO: enable and run the tests

echo "-------------------------------------------"
echo "Building for Half-Life 2: Episode One (x86)"
echo "-------------------------------------------"

python3 ../configure.py -s episode1 --mms-path $wd/alliedmodders/metamod-source/ --sm-path $wd/alliedmodders/sourcemod/ --hl2sdk-root $wd/alliedmodders/ --enable-tests --target-arch x86
ambuild

# TODO: run the tests
