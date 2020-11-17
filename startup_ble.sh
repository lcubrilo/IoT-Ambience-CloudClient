#!/bin/sh

cd /home/pi/Projects/iotambience_cloudclient/bin
(
    while true; 
    do
    sudo ./cloud_client
    sleep 5
    done
) &