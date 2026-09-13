#!/usr/bin/env bash

shuf -i 1-10000 -n 150 -r | paste -sd ' '
