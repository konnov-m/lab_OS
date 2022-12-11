#!/bin/bash

VAR2="Clap!"

while read line; do
        if [[ "$VAR2" == "$line"  ]]; then
		echo "Clap detect"
                time=$(date +%s)
                meters=$(($(($time-$1))*300))
                echo "$meters"
		exit
        fi
done
