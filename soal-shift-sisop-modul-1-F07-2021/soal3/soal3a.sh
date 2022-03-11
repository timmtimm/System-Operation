#!/bin/bash

for((loop=0; loop<24; loop=loop+1))
{
    host="https://loremflickr.com"
    to=$(curl -s -i "https://loremflickr.com/320/240/kitten" | grep "location" | awk -F ': ' '{print $2}' | tr -d $'\r')
    download="$host$to"
    name=$(echo "$to" | awk -F '/' '{print $4}')

    echo "$download" >> Foto.log
    wget $download

    if [ $(cat Foto.log | grep -c "${download}") -gt 1 ]
    then
        rm $name
    else
        number=$(ls | grep -c "Koleksi")
        number=$(echo "$number" + 1 | tr -d $'\r' | bc -l)

        if [ 1 -eq $(echo "${number} <10" | bc) ]
        then
            number="0${number}"
        fi

    filename="Koleksi_${number}.jpg"
    mv $name "$filename"
    fi
}
