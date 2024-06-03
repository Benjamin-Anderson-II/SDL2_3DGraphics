#!/bin/bash

kill -9 $(ps -e | grep 3D | sed 's/ .*//')
