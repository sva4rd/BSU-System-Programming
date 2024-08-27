#!/bin/bash
filename=$1
permission=$2
touch $filename
chmod $permission $filename

