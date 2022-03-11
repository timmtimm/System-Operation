#!/bin/bash

kucing()
{
    for((loop=0; loop<24; loop=loop+1))
    {
    host="https://loremflickr.com"
    to=$(curl -s -i "https://loremflickr.com/320/240/kitten" | grep "location" | awk -F ': ' '{print $2}' | tr -d $'\r')
    download="$host$to"
    name=$(echo "$to" | awk -F '/' '{print $4}')
    loc="Kucing_$currentDate"
    
    echo "$download">>"$loc/Foto.log"
    wget $download -P "$loc"

    if [ $(cat "${loc}/Foto.log" | grep -c "${download}") -gt 1 ]
    then
        rm "$loc/$name"
    else
        number=$(ls $loc | grep -c "Kucing")
        number=$(echo "$number" + 1 | tr -d $'\r' | bc -l)

        if [ 1 -eq $(echo "${number} <10" | bc) ]
        then
            number="0${number}"
        fi

    filename="Kucing_${number}.jpg"
    mv $loc/$name "$loc/$filename"
    fi
    }
}

kelinci()
{
    for((loop=0; loop<24; loop=loop+1))
    {
    host="https://loremflickr.com"
    to=$(curl -s -i "https://loremflickr.com/320/240/bunny" | grep "location" | awk -F ': ' '{print $2}' | tr -d $'\r')
    download="$host$to"
    name=$(echo "$to" | awk -F '/' '{print $4}')
    loc="Kelinci_$currentDate"
    
    echo "$download">>"$loc/Foto.log"
    wget $download -P "$loc"

    if [ $(cat "${loc}/Foto.log" | grep -c "${download}") -gt 1 ]
    then
        rm "${loc}/$name"
    else
        number=$(ls $loc | grep -c "Kelinci")
        number=$(echo "$number" + 1 | tr -d $'\r' | bc -l)

        if [ 1 -eq $(echo "${number} <10" | bc) ]
        then
            number="0${number}"
        fi

    filename="Kelinci_${number}.jpg"
    mv $loc/$name "$loc/$filename"
    fi
    }
}

currentDate=$(date +'%d-%m-%Y')
yesterdayDate=$(date -d "yesterday" +'%d-%m-%Y')

if [ $(ls -l | grep "$yesterdayDate" | grep -c "Kucing") -eq 0 ]
then
    if [ ! -d "Kucing_$currentDate" ]
    then
        mkdir "Kucing_$currentDate"
    fi

    kucing

else
    if [ ! -d "Kelinci_$currentDate" ]
    then
        mkdir "Kelinci_$currentDate"
    fi
    
    kelinci
fi
