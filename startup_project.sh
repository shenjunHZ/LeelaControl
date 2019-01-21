#!/bin/sh

#set LD PATH
export LD_LIBRARY_PATH=/usr/lib/arm-linux-gnueabihf:$LD_LIBRARY_PATH
echo $PATH

#set env params
export TARGET=arm-linux-gnueabihf

#start clion project
/home/junshen/tools/clion-2017.3.1/bin/clion.sh &
