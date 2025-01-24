#!/bin/bash
set -x
progdir="$(dirname $0)"

main() {
    if ! "$progdir/main-$PLATFORM"; then
        echo "App exited with an error"
        exit 1
    fi
}

main >"$LOGS_PATH/SDL Sample.txt" 2>&1
