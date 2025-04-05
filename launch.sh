#!/bin/bash
set -x
PAK_DIR="$(dirname "$0")"
PAK_NAME="$(basename "$PAK_DIR")"
PAK_NAME="${PAK_NAME%.*}"

rm -f "$LOGS_PATH/$PAK_NAME.txt"
exec >>"$LOGS_PATH/$PAK_NAME.txt"
exec 2>&1

main() {
    if ! "$PAK_DIR/main-$PLATFORM"; then
        echo "App exited with an error"
        exit 1
    fi
}

main
