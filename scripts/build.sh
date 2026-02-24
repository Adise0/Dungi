#!/usr/bin/env bash
set -euo pipefail

buildDir=$1
appName=$2

printf "[Build] Running pre-builld commands\n"

printf "  [Pre-Build] Copying assets...\n"
cp -r ./assets ./${buildDir}
printf "  [Pre-Build] Done\n\n"

printf "  [Pre-Build] Creating dungeons folder...\n"
mkdir -p ./Dungeons
printf "  [Pre-Build] Done\n\n"

printf "[Build] Pre-build commands complete\n"

printf "[Build] Building to "$buildDir"...\n"

cmake -S . -B "$buildDir"
cmake --build "$buildDir"

printf "[Build] Done!\n"
