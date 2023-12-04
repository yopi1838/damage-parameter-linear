#!/bin/bash

#Source and destination folder
source_folder='.\Release'

#Powershell command
powershell -Command Copy-Item -Path "Release\jmodelyopi007.dll" -Destination "'C:\\Program Files\Itasca\3DEC700\exe64\plugins\jmodel'" -Force -Recurse
echo "Copy done"
