#!/bin/bash

#Source and destination folder
source_folder='.\Release'

#Powershell command
powershell -Command Copy-Item -Path "x64\Release\jmodelyopi009.dll" -Destination "'C:\\Program Files\Itasca\ItascaSoftware910\exe64\plugins\jmodel'" -Force -Recurse
echo "Copy done"

sleep 0.5