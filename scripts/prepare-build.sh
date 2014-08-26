#!/bin/bash
#
# The Unix toolset used in the script is installed along with
# MSysGit/Git for Windows.
#

set -e
set -u

CURDIR=$(dirname "$0")
cd $CURDIR/..
rm -rf build
mkdir build
cd build

SYSNAME=$(uname -s)
if [[ "${SYSNAME:0:5}" == "MINGW" ]]; then
	# use "Visual Studio 9 2008" for VS 2008
	cmake -G "Visual Studio 12" ..
else
	cmake -G "Unix Makefiles" ..
fi
