#!/bin/bash
#
# The Unix toolset used in the script is installed along with
# MSysGit/Git for Windows.
#

set -e
set -u

CURDIR=$(dirname "$0")
cd $CURDIR/..
APPNAME=$(basename `pwd`)
./build/Debug/$APPNAME.exe
