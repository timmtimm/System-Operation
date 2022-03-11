#!/usr/bin/bash

pass=`date +'%m%d%Y'`

for i in */ ; do
    echo "$i"
    zip -r -P $pass Koleksi.zip . -i "$i*"
    rm -rf "$i"
done
