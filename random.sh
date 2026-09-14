#!/usr/bin/env bash

shuf -i 1-10000 -n $1 -r | paste -sd ' '
