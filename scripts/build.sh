#!/bin/bash
#
# The Unix toolset used in the script is installed along with
# MSysGit/Git for Windows.
#

set -e
set -u

CURDIR=$(dirname "$0")
cd $CURDIR/../build

SYSNAME=$(uname -s)
if [[ "${SYSNAME:0:5}" == "MINGW" ]]; then
	MSBUILD_PATH=$(reg.exe query \
		"HKLM\\SOFTWARE\\Microsoft\\MSBuild\\ToolsVersions\\12.0" \
			//v MSBuildToolsPath \
		| grep REG_SZ | sed 's/.*REG_SZ[[:space:]]\+//')
	[[ -z "$MSBUILD_PATH" ]] && { echo "FAIL: MSBuild v12 not installed"; exit; }
	"${MSBUILD_PATH}MSBuild.exe" *.sln
else
	make
fi
