#!/bin/bash

date=$(date +'%d-%m-%Y')

if [ ! -d "$date" ]
then
    mkdir "$date"
fi

for((loop=0; loop<24; loop=loop+1))
{
    host="https://loremflickr.com"
    to=$(curl -s -i "https://loremflickr.com/320/240/kitten" | grep "location" | awk -F ': ' '{print $2}' | tr -d $'\r')
    download="$host$to"
    name=$(echo "$to" | awk -F '/' '{print $4}')

    echo "$download">>"${date}/Foto.log"
    wget $download -P "$date"

    if [ $(cat "${date}/Foto.log" | grep -c "${download}") -gt 1 ]
    then
        rm "$date/$name"
    else
        number=$(ls $date | grep -c "Koleksi")
        number=$(echo "$number" + 1 | tr -d $'\r' | bc -l)

        if [ 1 -eq $(echo "${number} <10" | bc) ]
        then
            number="0${number}"
        fi

    filename="Koleksi_${number}.jpg"
    mv $date/$name "$date/$filename"
    fi
}
