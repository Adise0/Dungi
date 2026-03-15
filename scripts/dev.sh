#!/usr/bin/env bash
set -euo pipefail

clear

if bash ./scripts/build.sh dev; then
  echo -e "\nBuild successful!\n"
else
  echo -e "\nBuild failed\n"
  exit 1
fi

if [[ "${RUN_AFTER:-1}" == "1" ]]; then
  echo "=========================="
  echo "     Running Dungi      "
  echo "=========================="
  ./build/dev/bin/Dungi.exe
else
  echo "Skipping execution (-n passed)"
fi
