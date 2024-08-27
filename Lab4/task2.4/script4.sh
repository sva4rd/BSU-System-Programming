#!/bin/bash

for ((i=1; i<=10; i++)); do
    filename="file$i.$(mcookie)"
    echo $filename
done
