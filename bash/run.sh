#!/usr/bin/env bash

cd "$(dirname "$0")"

if [ $# -lt 1 ]; then
    echo "ERROR: No program specified!"
    echo "Usage: $0 <program_name> [arguments...]"
    echo "Available programs: lab1, lab2"
    exit 1
fi

CHOICE=$1
shift

cd ..

case "$CHOICE" in
    "lab1")
        ./build/lab_1/lab1 "$@"
        ;;
    "lab2")
        ./build/lab_2/lab2 "$@"
        ;;
    *)
        echo "ERROR: Program '$CHOICE' is not supported."
        echo "Available options are: lab1, lab2"
        exit 1
        ;;
esac
