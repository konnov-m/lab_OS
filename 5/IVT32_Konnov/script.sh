#!/bin/bash
echo "start file" 
flag=0
 
while read line; do
	echo "check data.."
   if [ $(($line)) -ge 20000 ] && [ $flag -eq 0 ]; then
       flag=1
	echo "light detect $line"
   elif ((flag == 1)); then
	echo "wait time"
       time=$(($line))
	echo "$time"
       ./clap_detecter.sh $time < sound_data
       flag=0
   fi
done

