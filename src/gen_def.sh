#!/bin/bash

echo "EXPORTS"
grep mms_ *.h|grep ")" |grep "("|grep -v "typedef"|grep -v "define"|tr "(" " "|sed -e "s/  / /g" -e "s/const //g" -e "s/unsigned //g" |awk '{print "\t" $2}'

