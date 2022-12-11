#!/bin/bash
 
VAR2="Clap"
flag=0
time=0
 
while read line; do
   if ((flag == 1)); then
       time=$(($line))
       meters=$(($(($time-$1))*300))
       echo "meters=$meters"
       return 
   fi
   if [[ "$VAR2" == "$line"  ]]; then
       flag=1
	echo "flag = 1"
   fi
done

