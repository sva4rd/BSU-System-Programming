#!/bin/bash
for interface in $(ls /sys/class/net); do
    echo $interface
done

