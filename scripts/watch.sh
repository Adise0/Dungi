#!/usr/bin/env bash
set -euo pipefail

printf "Starting watcher...\n"

run_after=1
while getopts "n" opt; do
  case "$opt" in
  n) run_after=0 ;;
  esac
done
shift $((OPTIND - 1))

get_config() {
  local config="$1"
  local input="$2"
  if [[ $input =~ $config[[:space:]]*=[[:space:]]*(.*) ]]; then
    echo "${BASH_REMATCH[1]}"
    return 0
  fi
  return 1
}

appName=""
buildDir=""
defaultBuildDir="out/dev"

while IFS= read -r line; do
  if val=$(get_config "AppName" "$line"); then
    appName="$val"
    continue
  fi
  if val=$(get_config "BuildDir" "$line"); then
    buildDir="$val"
    continue
  fi
done <.config

if [[ -z $appName ]]; then
  echo "Error: AppName not found in .config" >&2
  exit 1
fi

if [[ -z "$buildDir" ]]; then
  echo "Warning: BuildDir not found in .config, using default dir $defaultBuildDir" >&2
  buildDir=$defaultBuildDir
fi

watchexec -w src -w assets -e cpp,h,html,js,css -r --stop-signal SIGTERM \
  -E RUN_AFTER="$run_after" \
  -E APP_NAME="$appName" \
  -E BUILD_DIR="$buildDir" \
  -- bash -lc '

    clear
    if bash ./scripts/build.sh ${BUILD_DIR} ${APP_NAME}; then
      echo -e "\nBuild successful!\n"
    else
      echo -e "\nBuild failed\n"
      exit 1
    fi

    if [[ "${RUN_AFTER:-1}" == "1" ]]; then
      echo "=========================="
      echo "       Running ${APP_NAME}"
      echo "=========================="
      ${BUILD_DIR}/${APP_NAME}
    else
      echo "Skipping execution (-n passed)"
    fi
  '
