#!/bin/bash
set -x
PAK_DIR="$(dirname $0)"

main() {
    if ! "$PAK_DIR/main-$PLATFORM"; then
        echo "App exited with an error"
        exit 1
    fi
}

main >"$LOGS_PATH/SDL Sample.txt" 2>&1
