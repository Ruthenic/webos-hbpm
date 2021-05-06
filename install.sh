#!/bin/sh

#Create bin directory
mkdir /media/internal/bin

#Download file and write it to /media/internal/bin/hbpm
wget -O /media/internal/bin/hbpm https://github.com/Ruthenic/webos-hbpm/releases/latest/download/hbpm

#Make it executable
chmod a+x /media/internal/bin/hbpm

#Annoy people to add it to $PATH since i cant figure out how the hell to do it myself
echo "Run 'export PATH=\"/media/internal/bin:/mnt/bsppart/bsp/usr/bin:/usr/sbin:/usr/bin:/sbin:/bin\"' to add hbpm to the count"
#TODO: automagically add to path, even on reboot
