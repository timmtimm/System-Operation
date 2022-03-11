#!/bin/bash

#  nomor 1 a
grep -oE "(INFO.*)|(ERROR.*)" syslog.log

#   nomor 1 b
grep -oE 'ERROR.*' syslog.log
echo | grep -cE 'ERROR' syslog.log 

#  nomor 1 c
name=`grep -oE "(\([a-zA-Z]+\))" syslog.log | sort | uniq | grep -oP "(?<=\().*(?=\))"`
echo "$name" |
while read -r lines
do
    total=`grep -c "$lines" syslog.log`
    lines+=','$total
    echo $lines
done

# nomor 1 d
echo "Error","Count" > error_message.csv
error=`grep -oE "ERROR.*([A-Z][a-z]+)\s(['A-Za-z]+\s){1,5}" syslog.log | sort | uniq`
echo "$error" |
while read -r line
do
    total=`grep -c "$line" syslog.log`
    line+=','$total
    echo $line
done | sort  -rnk 2 -t ',' >> error_message.csv

# nomor 1 e
echo "Username", "INFO", "ERROR" > user_statistics.csv
name=`grep -oE "(\([a-zA-Z]+\))" syslog.log | sort | uniq | grep -oP "(?<=\().*(?=\))"`
for i in $name
do
    echo "$i, $(grep -cP "INFO.*($i)" syslog.log), $(grep -cP "ERROR.*($i)" syslog.log)"
done | sort >> user_statistics.csv
