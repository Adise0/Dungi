#!/usr/bin/env bash
set -euo pipefail

buildDir=$1
appName=$2

printf "[Build] Running pre-builld commands\n"

printf "  [Pre-Build] Copying assets...\n"
rm -rf ./${buildDir}/assets
cp -r ./assets ./${buildDir}
printf "  [Pre-Build] Assets copied\n\n"

printf "[Build] Pre-build commands complete\n"

printf "[Build] Building to "$buildDir"...\n"

cmake -S . -B "$buildDir"
cmake --build "$buildDir"

printf "[Build] Done!\n"
