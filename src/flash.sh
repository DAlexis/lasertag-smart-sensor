#!/bin/bash

dir="debug"

if [ "$1" == "release" ];
then
    dir="release"
fi

st-flash write build/$dir/smart-sensor/smart-sensor.bin 0x8000000 
