#!/bin/bash

CURDIR=$(dirname "$0")
echo "----------------"
echo "Building..."
$CURDIR/build.sh

echo "----------------"
echo "Running..."
$CURDIR/run.sh
