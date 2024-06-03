#!/bin/bash

kill -SIGKILL $(ps -e | grep 3D | sed 's/ .*//')
