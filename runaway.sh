#!/bin/bash

kill -9 $(ps -e | grep 3D_Graphics | awk '{$1=$1};1' | sed 's/ .*//')
